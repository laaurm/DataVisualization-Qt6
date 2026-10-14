#include <QApplication>
#include <QMainWindow>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>
#include <QUrl>
#include <QObject>
#include <QDebug>
#include <QByteArray>
#include <QProcessEnvironment>

#include "DicomBridge.h"

class Backend : public QObject
{
    Q_OBJECT

public:
    explicit Backend(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    Q_INVOKABLE void showMessage(const QString &message)
    {
        qDebug() << "Message reçu depuis JavaScript:" << message;

        QMessageBox::information(
            nullptr,
            "Message C++",
            message);
    }

    Q_INVOKABLE QString getVersion() const
    {
        return QStringLiteral("ModernUI 1.0 - C++ / Qt 6");
    }
};

int main(int argc, char *argv[])
{
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS",
            QByteArray("--disable-gpu"));

    QApplication app(argc, argv);

    QApplication::setApplicationName("DataVisualization");
    QApplication::setOrganizationName("DataVisualization");

    QMainWindow window;
    window.setWindowTitle("Data Visualizationn");
    window.resize(1200, 760);
    window.setMinimumSize(800, 520);

    auto *view = new QWebEngineView(&window);
    auto *channel = new QWebChannel(&window);
    auto *backend = new Backend(&window);
    auto *bridge = new DicomBridge(&window);

    // Enregistrer le pont C++ accessible depuis JavaScript
    channel->registerObject("backend", backend);
    channel->registerObject("dicomBridge", bridge);

    view->page()->setWebChannel(channel);

    // Activer JavaScript
    view->settings()->setAttribute(
        QWebEngineSettings::JavascriptEnabled, true);

    // Charger le fichier HTML local
    const QString htmlPath =
        QDir(QCoreApplication::applicationDirPath())
            .filePath("app/index.html");

    if (!QFileInfo::exists(htmlPath))
    {
        QMessageBox::critical(
            nullptr,
            "Erreur",
            "Fichier introuvable :\n" + htmlPath);
        return 1;
    }

    view->load(QUrl::fromLocalFile(htmlPath));
    window.setCentralWidget(view);
    window.show();

    return app.exec();
}

#include "main.moc"