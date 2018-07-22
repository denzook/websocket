#include <QApplication>
#include <QMetaMethod>
#include "echoserver.h"

EchoServer::EchoServer(int port, bool useThread)
    : QObject(NULL), inPort (port), server(this)
{
    connect(&server, SIGNAL(newConnection()), this, SLOT(newConnections()));

    if(useThread)
    {
        this->moveToThread(&rThread);
        rThread.start();
    }
}

bool EchoServer::addAccepts(const QHostAddress &addr, const QHostAddress &outAddr, int outPort)
{
    QString a;
    QMap<QString,info>::iterator found;

    bool result=false;
    lock.lock();
    if((found = accepts.find(a=addr.toString())) != accepts.end())
    {

        if(outAddr.isEqual(found->addr) && outPort == found->port)
            result= true;
        //return false;
    }
    else
    {
        found = accepts.insert(a, info (outAddr, outPort));
        result=true;
    }
    lock.unlock();

    return result;
}

void EchoServer::remove(const QHostAddress &addr) // public
{
    if(QThread::currentThread() != QObject::thread())
    {
        QMetaObject::invokeMethod(this,"remove",Qt::BlockingQueuedConnection,Q_ARG(QHostAddress, addr));
        return;
    }

    QString a;
    QMap<QString,info>::iterator found;

    lock.lock();
    if((found = accepts.find(a=addr.toString())) != accepts.end())
    {
        if(found->target) found->target->close();
        accepts.erase(found);

//        QString s = found.value();
//        QMap<QString,info>::iterator found2;

//        accepts.erase(found);

//        if((found2 = infos.find(s)) != infos.end())
//        {
//            QList<QTcpSocket*> list;
//            foreach(QTcpSocket *socket, found2->socket)
//            {
//                if(addr.isEqual(socket->peerAddress())) list.push_back(socket);
//            }

//            foreach(QTcpSocket *socket, list)
//            {
//                socket->close();
//            }
//            if(found2->socket.size() == 0)
//            {
//                if(found2->target) found2->target->close();
//                infos.erase(found2);
//            }
//        }
    }
    lock.unlock();
}

bool EchoServer::isEmpty() // public
{
    bool empty;
    lock.lock();
    empty = accepts.isEmpty();
    lock.unlock();
    return empty;
}


int EchoServer::getPort() // public
{
    return inPort;
}

void EchoServer::listen() // public
{
//    if(thread && QThread::currentThread() != QObject::thread())
//    {
//        QMetaObject::invokeMethod(this, "listen", Qt::QueuedConnection);
//        return false;
//    }
    if(QThread::currentThread() != QObject::thread())
    {
        QMetaObject::invokeMethod(this,"listen");
        return;
    }
    if(!server.isListening() && !server.listen(QHostAddress(QHostAddress::Any), inPort))
    {
        failListen();
    }
}

QString EchoServer::getServerError() // public
{
    if(QThread::currentThread() != QObject::thread())
    {
        QString s;
        QMetaObject::invokeMethod(this,"getServerError", Qt::BlockingQueuedConnection,Q_RETURN_ARG(QString, s));
        return s;
    }
    return server.errorString();
}

static QString getIPv4(const QHostAddress &a)
{
    QString s = a.toString();
    return s.mid(s.lastIndexOf(":")+1);
}


int EchoServer::getElapsed(const QString &addr) // public
{
    QMap<QString,info>::iterator found;
    int elapsed = -1;
    lock.lock();
    if((found = accepts.find(addr)) != accepts.end())
    {
        elapsed = found->heartbeat.elapsed();
    }
    lock.unlock();
    return elapsed;
}

//bool EchoServer::stillGotSocket(const info *i, const QString &addr)
//{
//    bool result = false;
//    QHostAddress host (addr);
//    foreach(QTcpSocket *socket, i->socket)
//    {
//        if(host.isEqual(socket->peerAddress())) {
//            result = true;
//            break;
//        }
//    }
//    return result;
//}

void EchoServer::newConnections()
{
    QTcpSocket *socket;


    while(server.hasPendingConnections())
    {
        socket = server.nextPendingConnection();
        QMap<QString,info>::iterator found;
        lock.lock();
        if((found = accepts.find(getIPv4(socket->peerAddress()))) == accepts.end() &&
           (found = accepts.find(QHostAddress(QHostAddress::Any).toString())) == accepts.end()
            )
        {
            socket->close();
            socket->deleteLater();
            lock.unlock();
            continue;
        }

        QString key = found.key();
        info *k=&found.value();

        lock.unlock();
        if(k->target)
        {
            socket->close();
            socket->deleteLater();
            continue;
        }

        //if(k->target == NULL)
        {
            QTcpSocket *target = new QTcpSocket();
            target->connectToHost(k->addr, k->port);

            if(!target->waitForConnected())
            {
                socket->close();
                socket->deleteLater();
                emit disconnected(key, inPort, k->addr.toString(), k->port);
                continue;
            }
            k->target = target;
            k->heartbeat.restart();

            connect(target, &QTcpSocket::readyRead, this,[k,this](){
                QByteArray data = k->target->readAll();
//                foreach(QTcpSocket *socket, k->socket)
//                {
//                    socket->write(data.data(), data.length());
//                }
                if(k->socket) k->socket->write(data.data(), data.length());
                k->heartbeat.restart();
            });
            connect(target, &QTcpSocket::disconnected, this,[k,this](){
                k->target->deleteLater();
                k->target = NULL;
//                foreach(QTcpSocket *socket, k->socket)
//                {
//                    socket->close();
//                }
                if(k->socket) k->socket->close();
            });
        }
        //k->socket.push_back(socket);
        k->socket = socket;
        connect(socket, &QTcpSocket::readyRead, this, [socket, k, this](){
            QByteArray data = socket->readAll();
            if(k->target)
            {
                k->target->write(data.data(), data.length());
                k->heartbeat.restart();
            }
        });
        connect(socket, &QTcpSocket::disconnected,
                this, [key, k, this](){

            //QList<QTcpSocket*>*sockets = &k->socket;
            QTcpSocket*socket = static_cast<QTcpSocket*>(this->sender());
            qInfo()<<QString("Remove %1").arg(getIPv4(socket->peerAddress()));

            //sockets->removeAll(socket);
            //socket->deleteLater();

            if(k->socket == socket) k->socket = NULL;
            socket->deleteLater();

            //if(sockets->isEmpty())
            {
                emit disconnected(key, inPort, k->addr.toString(), k->port);
            }
        });
        emit connected(key, inPort, k->addr.toString(), k->port);
    }
}

void EchoServer::close(QThread *thread)
{
    if(QThread::currentThread() != QObject::thread())
    {
        QMetaObject::invokeMethod(this,"close",Qt::BlockingQueuedConnection,Q_ARG(QThread*, thread));
        this->rThread.quit();
        this->rThread.wait();
        return;
    }

    while(!accepts.isEmpty())
    {
        qInfo ()<<tr("erasing %1").arg(accepts.begin().key());
        remove(QHostAddress(accepts.begin().key()));
    }
    server.close();
    if(thread && thread != QThread::currentThread()) moveToThread(thread);
}


EchoServer::~EchoServer()
{
    close();
}

