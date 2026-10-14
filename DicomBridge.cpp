
#include "DicomBridge.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

DicomBridge::DicomBridge(QObject *parent)
    : QObject(parent),
      m_process(new QProcess(this))
{
    connect(m_process, &QProcess::finished, this,
            [this](int exitCode, QProcess::ExitStatus status)
            {
                const QByteArray output =
                    m_process->readAllStandardOutput();
                const QByteArray errors =
                    m_process->readAllStandardError();

                if (status != QProcess::NormalExit || exitCode != 0)
                {
                    emit errorOccurred(QString::fromUtf8(errors));
                    return;
                }

                QJsonParseError parseError;
                const auto doc =
                    QJsonDocument::fromJson(output, &parseError);

                if (parseError.error != QJsonParseError::NoError ||
                    !doc.isObject())
                {
                    emit errorOccurred("Invalid Python response.");
                    return;
                }

                const QJsonObject result = doc.object();

                const QString original =
                    result.value("original").toString();
                const QString overlay =
                    result.value("overlay").toString();

                if (original.isEmpty() || overlay.isEmpty())
                {
                    emit errorOccurred("Python returned no images.");
                    return;
                }

                emit segmentationReady(
                    "data:image/png;base64," + original,
                    "data:image/png;base64," + overlay);
            });

    connect(m_process, &QProcess::errorOccurred, this,
            [this](QProcess::ProcessError error)
            {
                if (error == QProcess::FailedToStart)
                {
                    emit errorOccurred(m_process->errorString());
                }
            });
}

void DicomBridge::selectAndSegment()
{
    if (m_process->state() != QProcess::NotRunning)
    {
        emit errorOccurred("Segmentation is already running.");
        return;
    }

    const QString filePath = QFileDialog::getOpenFileName(
        nullptr,
        "Select DICOM image",
        QString(),
        "DICOM files (*.dcm *.dicom);;All files (*)");

    if (filePath.isEmpty())
        return;

    // Set these paths for your development environment.
    const QString python = ".venv/bin/python";
    const QString script = "algo/segmentation.py";

    if (!QFileInfo(python).isFile() ||
        !QFileInfo(script).isFile())
    {
        emit errorOccurred("Python executable or script not found.");
        return;
    }

    m_process->setProgram(python);
    m_process->setArguments({script, filePath});
    m_process->start();
}