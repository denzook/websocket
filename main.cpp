#include "mainwindow.h"
#include "version.h"
#include <QApplication>

#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QMessageBox>
#include <QTimer>

#include <QLocalSocket>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCommandLineParser parser;
    MainWindow *w;
    parser.setApplicationDescription(QString("WebSocket %1.%2.%3")
                                     .arg(WEBSOCKET_MAJOR)
                                     .arg(WEBSOCKET_MINOR)
                                     .arg(WEBSOCKET_PATCH));

    QCommandLineOption helpOption = parser.addHelpOption();

    QCommandLineOption fileOption(QStringList() << "f" << "file", "XML file settings", "file");
    parser.addOption(fileOption);

    QCommandLineOption idleOption(QStringList() << "i" << "idle", "Idle time in minutes", "idle");
    parser.addOption(idleOption);

    QCommandLineOption refreshOption(QStringList() << "r" << "refresh", "Refresh rate (min 10) in seconds", "refresh");
    parser.addOption(refreshOption);

    parser.parse(a.arguments());

    qApp->setWindowIcon(QIcon(QString(":/icons/icon32.png")));

    if(parser.isSet(helpOption))
    {
        QTimer::singleShot(0,[&parser](){
            QString txt = parser.helpText();
            int n  = txt.indexOf("\n");
            QMessageBox::about (
                        NULL, qApp->applicationName(),
                        (n == -1)?txt:txt.mid(n+1));

        });
    }
    else
    {
        QLocalSocket socket;
        socket.connectToServer(MainWindow::SERVERNAME());
        if(socket.waitForConnected(200)) return 0;

        w = new MainWindow (parser.isSet(fileOption)?parser.value(fileOption):QString(),
                            parser.isSet(idleOption)?parser.value(idleOption).toInt()*60000:0,
                            parser.isSet(refreshOption)?parser.value(refreshOption).toInt()*1000:0
                            );
        w->show();
    }

    return a.exec();
}
