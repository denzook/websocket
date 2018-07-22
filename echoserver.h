#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include <QObject>
#include <QTime>
#include <QTcpServer>
#include <QTcpSocket>

#include <QThread>
#include <QMutex>

#include <QHostAddress>


// 1 server 1 thread. For multiple address, try use diff port or diff ip addr
class EchoServer : public QObject
{
    Q_OBJECT

    int inPort;

    QTcpServer server;
    QThread rThread;
    QString errorMsg;
    QMutex lock;

    struct info
    {
        QHostAddress addr;
        int port;
        //QList<QTcpSocket *>socket;
        QTcpSocket *socket;
        QTcpSocket *target;
        QTime heartbeat;
        info (const QHostAddress &addr, int port){
            this->addr = addr; this->port = port; target = NULL; socket = NULL;
            heartbeat.start();
        }
    };
    // input address limited to 1 entry, and same ip is ignored
    QMap<QString, info> accepts;

    //QMap<QString,QList<info>> accepts; // key is input socket addr

    void remove(const QString &addr);
    //bool stillGotSocket(const info *i, const QString &addr);

public:
    ~EchoServer();

    Q_INVOKABLE void listen();
    Q_INVOKABLE QString getServerError();

    Q_INVOKABLE void remove(const QHostAddress &addr);

    // addAccept output addr
    bool addAccepts(const QHostAddress &addr, const QHostAddress &outAddr, int outPort);

    bool isEmpty();

    int getPort();
    int getElapsed(const QString &addrPort);

    explicit EchoServer(int port, bool useThread);


signals:
    void failListen();
    void connected(const QString &addr, int port, const QString &oAddr, int oport);//, int port, const QString &oaddr, int oport);
    void disconnected(const QString &addr, int port, const QString &oAddr, int oport);//, int inport, const QString &out, int outport);

private slots:
    void newConnections();

public slots:
    void close(QThread *thread = QThread::currentThread());

};

#endif // ECHOSERVER_H
