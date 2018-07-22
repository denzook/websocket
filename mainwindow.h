#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QList>
#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QLocalServer>
#include <QSystemTrayIcon>

#include "echoserver.h"
#include "version.h"


namespace Ui {
class MainWindow;
}

class SERVERS : public QObject
{
public:
    EchoServer servers ;
    SERVERS(int port, bool useThread, QObject *parent)  : QObject(parent) , servers(port, useThread)
    {

    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    struct pairsocket
    {
        QString input;
        QString dest;
        pairsocket &operator=(const pairsocket &b) { input = b.input; dest = b.dest; return *this;}
    };

    struct INFO
    {
        SERVERS *server;
        bool connected;
        QTime startTime;
    };

    QSystemTrayIcon sysIcon;
    QList <SERVERS*> ports;
    QMap<QString,INFO> servers;

    void errorMsg(const QString &err);

    void statusMsg(const QString &msg, bool err=false);
    void statusClear();

    QString getSettingFile() const;
    QString getLogFile() const;

    bool loadXML(QList<pairsocket> *out, const QString &fname, int retry=0);
    bool removeFromXML(const QString &file, const QList<pairsocket> &list, int retry = 0);

    void putLine(const QString &s);
    void syncPorts();

    bool getHostAddr(const QString &hostport, QHostAddress *host, int *port);
    int findPairFromTable(const pairsocket &pair);
    QString elapsedTime(int milliseconds);

    QDateTime settingFileDT;
    QString settingFile;
    int IDLE, REFRESH;

    QLocalServer singleApp;

    QTimer monitor;
    QTime refreshTimer;


    bool eventFilter(QObject *obj, QEvent *event);
    void hideEvent(QHideEvent *);
    void showEvent(QShowEvent *);

public:

    static QString SERVERNAME()     {
            return QString("WEBSOCKET_%1_%2.%3.%4")
                        .arg(qApp->applicationName())
                        .arg(WEBSOCKET_MAJOR)
                        .arg(WEBSOCKET_MINOR,2,10,QLatin1Char('0'))
                        .arg(WEBSOCKET_PATCH,2,10,QLatin1Char('0'));
    }
    explicit MainWindow(QString file=QString(), int idle=0, int refresh=0, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void updateTableView();
    void refresh(bool force=true);
    void timer();
    void connected(const QString &addr, int, const QString &,int);
    void disconnected(const QString &inAddr, int , const QString &,int);

    void disconnectSelected();
    void removeDisconnected();
    void copyTable();
    void sysIconActivate(QSystemTrayIcon::ActivationReason);

};

#endif // MAINWINDOW_H
