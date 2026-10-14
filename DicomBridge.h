
#pragma once

#include <QObject>
#include <QProcess>

class DicomBridge : public QObject
{
    Q_OBJECT

public:
    explicit DicomBridge(QObject *parent = nullptr);

    Q_INVOKABLE void selectAndSegment();

signals:
    void segmentationReady(
        const QString &originalUrl,
        const QString &overlayUrl);

    void errorOccurred(const QString &message);

private:
    QProcess *m_process;
};