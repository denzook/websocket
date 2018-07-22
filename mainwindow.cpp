#include "mainwindow.h"
#include "utils.h"
#include "aboutdialog.h"

#include "ui_mainwindow.h"

#include "version.h"

#include <QFile>
#include <QDir>

#include <QThread>
#include <QDomDocument>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QTextStream>
#include <QClipboard>

#include <QLocalServer>
#include <QLocalSocket>

//static QString MainWindow::SERVERNAME() const
//{
//    return QString("WEBSOCKET_%1_%2.%3.%4")
//                .arg(qApp->applicationName())
//                .arg(WEBSOCKET_MAJOR)
//                .arg(WEBSOCKET_MINOR,2,10,QLatin1Char('0'))
//                .arg(WEBSOCKET_PATCH,2,10,QLatin1Char('0'));
//    return "";
//}

MainWindow::MainWindow(QString file, int idle, int refresh, QWidget *parent) :
    QMainWindow(parent), settingFile(file), IDLE(idle), REFRESH(refresh),
    singleApp (parent),
    ui(new Ui::MainWindow)
{
    if(IDLE == 0) IDLE = 5*60*1000;
    if(REFRESH != 0 && REFRESH < 10000) REFRESH = 10000;
    if(settingFile.isEmpty()) settingFile = qApp->applicationName()+".xml";

    ui->setupUi(this);
    setWindowTitle(QString("WebSocket %1.%2.%3")
                   .arg(WEBSOCKET_MAJOR)
                   .arg(WEBSOCKET_MINOR)
                   .arg(WEBSOCKET_PATCH));
    qRegisterMetaType<QHostAddress >("QHostAddress");

    //setAttribute(Qt::WA_DeleteOnClose, false);

    // Setting up table
    QStandardItemModel *model;
    ui->tableView->setModel(model = new QStandardItemModel(ui->tableView));

    model->setHorizontalHeaderLabels(QStringList ()<<"Input IP" <<"Destination" << "Status" << "Elapsed time");

    // Setting up statusbar
    ui->statusBar->addWidget(new QLabel(ui->statusBar),1);
    statusClear();

    connect (ui->actionClear_text, SIGNAL(triggered()), ui->editText, SLOT(clear()));
    connect (ui->btnRefresh, SIGNAL(clicked()), this, SLOT(refresh()));
    connect (ui->actionAbout_QT, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect (ui->actionAbout, &QAction::triggered, [this](){ AboutDialog(this).exec(); });

    {
        QAction *act;
        ui->tableView->addAction(act = new QAction("&Copy",ui->tableView));
        connect(act, SIGNAL(triggered()), this, SLOT(copyTable()));
        act->setShortcut(QKeySequence::Copy);
        ui->tableView->addAction(act = new QAction("Select &All",ui->tableView));
        connect(act, SIGNAL(triggered()), ui->tableView, SLOT(selectAll()));
        act->setShortcut(QKeySequence::SelectAll);

        ui->editText->addAction(act = new QAction("C&lear",ui->tableView));
        connect(act, SIGNAL(triggered()), ui->editText, SLOT(clear()));
    }

    // Setting up Tray Icon
    if(QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMenu *menu = new QMenu (this);
        sysIcon.setIcon(qApp->windowIcon());

        QAction *act;
        act = menu->addAction("&Open window", this, [this](){
            show();
            if(isMaximized()) showMaximized();
            else showNormal();
            activateWindow();
        });
        menu->setDefaultAction(act);
        menu->addSeparator();
        menu->addAction("E&xit", qApp, SLOT(quit()));
        sysIcon.setContextMenu(menu);
        connect(&sysIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
                SLOT(sysIconActivate(QSystemTrayIcon::ActivationReason)));
        //QTimer::singleShot(1,this,SLOT(close()));
    }


    QTimer::singleShot(200, this, SLOT(timer()));
    refreshTimer.start();

    {
        auto func = [this](){
            while(singleApp.hasPendingConnections())
            {
                singleApp.nextPendingConnection()->deleteLater();
            }
            raise();
            show();
            activateWindow();
    //        {
    //            bool flag = (windowFlags () & Qt::WindowStaysOnTopHint) != 0;
    //            setWindowFlag(Qt::WindowStaysOnTopHint, true);
    //            show();
    //            if(!flag) setWindowFlag(Qt::WindowStaysOnTopHint, flag);
    //        }
            this->refresh(false);
        };
        connect(&singleApp, &QLocalServer::newConnection, func);
        singleApp.listen(SERVERNAME());
        func();
    }

    ui->editText->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->editText, static_cast<void(QWidget::*)(const QPoint&)>
            (&QWidget::customContextMenuRequested),[this](const QPoint &pt)
    {
        qApp->postEvent(ui->editText, new QContextMenuEvent(QContextMenuEvent::Other, pt));
    });
    ui->editText->installEventFilter(this);
    ui->tableView->installEventFilter(this);
    sysIcon.installEventFilter(this);


//    if(REFRESH)
//    {
//        static std::function<void()> func;
//        func = [&func,this](){
//            this->refresh(false);
//            if(REFRESH) QTimer::singleShot(REFRESH,func);
//        };
//        func();
//    }
}

void MainWindow::hideEvent(QHideEvent *)
{
    if(isMinimized() && !sysIcon.isVisible())
    {
        qApp->setQuitOnLastWindowClosed(false);
        sysIcon.show();
        close();
        //QTimer::singleShot(1,this,SLOT(close()));
    }
}

void MainWindow::sysIconActivate(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick)
    {
        sysIcon.contextMenu()->defaultAction()->trigger();
    }
}

void MainWindow::showEvent(QShowEvent *)
{
    if(sysIcon.isVisible())
    {
        sysIcon.hide();
        qApp->setQuitOnLastWindowClosed(true);
    }
}

void MainWindow::errorMsg(const QString &err)
{
    QMessageBox::critical(this, "Error", err);
}

void MainWindow::statusMsg(const QString &msg, bool err)
{
    if(err)
        ui->statusBar->findChild<QLabel*>()->setText(tr("<font color=red><b>ERROR:</b></font> ")+msg);
    else
        ui->statusBar->findChild<QLabel*>()->setText(msg);
}

void MainWindow::statusClear()
{
    ui->statusBar->findChild<QLabel*>()->setText("<font color=green>Ready</font>");
}

bool MainWindow::loadXML(QList<pairsocket> *out, const QString &fname, int retry)
{
    QDomDocument doc;
    QString msg; int line, col;
    QFile file(fname);
    bool result = false;

    while (!file.open(QIODevice::ReadOnly))
    {
        if(retry <= 0)
        {
            statusMsg(tr("Cannot open file '")+QFileInfo(fname).fileName()+"'",true);
            goto end;
        }
        QThread::msleep(50); retry --;
    }

    if(!doc.setContent(&file, &msg, &line, &col))
    {
        statusMsg(QString("@(line=%1, column=%2) %3)").arg(line).arg(col).arg(msg),true);
        goto end;
    }
    out->clear();

    {
        QDomElement items;
        for(QDomNode n = doc.documentElement();!n.isNull();n = n.nextSibling()) {
            QDomElement e = n.toElement(); // try to convert the node to an element.
            if(e.isNull()) continue;
            if(e.tagName().compare("items") != 0) continue;
            items = e;
            break;
        }
        if(!items.isNull())
        {
            for(QDomElement e= items.firstChildElement("item"); !e.isNull(); e = e.nextSiblingElement("item"))
            {
                QDomElement k;
                pairsocket pair;
                if(!(k = e.firstChildElement("ip")).isNull())pair.input = k.text();
                if(!(k = e.firstChildElement("dest")).isNull())pair.dest = k.text();
                if(!pair.dest.isEmpty() && !pair.input.isEmpty()) out->push_back(pair);
            }
        }
    }
    statusClear();
    result = true;

end:
    if(file.isOpen()) file.close();
    return result;
}

bool MainWindow::getHostAddr(const QString &hostport, QHostAddress *host, int *port)
{
    bool ok; QString s; int n = hostport.lastIndexOf(":");

    if((s = hostport.left(n).trimmed()).isEmpty()) host->setAddress(QHostAddress::Any);
    else host->setAddress(s);
    *port = hostport.mid(n+1).trimmed().toInt(&ok);

    return (!host->isNull() && ok && *port != 0);
}

QString MainWindow::getSettingFile() const
{
    QFileInfo info = QFileInfo (settingFile);
    QString file;
    if(info.fileName() == settingFile)
    {
        if( !QFile::exists(file = QFileInfo(qApp->applicationDirPath(),settingFile).absoluteFilePath()) &&
            !QFile::exists(file = QFileInfo(QDir::homePath(),settingFile).absoluteFilePath()))
        {
            return QString();
        }
    }
    else if(!QFile::exists(file = settingFile))
    {
        return QString();
    }
    return file;
}

QString MainWindow::getLogFile() const
{
    return QFileInfo(QDir::homePath(),qApp->applicationName()+".log").absoluteFilePath();
}

void MainWindow::syncPorts()
{
    for(QList<SERVERS*>::iterator jt = ports.begin();jt!=ports.end(); )
    {
        if((*jt)->servers.isEmpty())
        {
            SERVERS *s = *jt;
            jt = ports.erase(jt);
            s->servers.close();
            s->deleteLater();
        }
        else jt ++;
    }
}

void MainWindow::refresh(bool force)
{
    QString file = getSettingFile();
    QDateTime lastModified;

    if(REFRESH) refreshTimer.restart();

    if(!force)
        if(file.isEmpty() ||
                (!settingFileDT.isNull() && settingFileDT >=(lastModified=QFileInfo(file).lastModified())))
    {
        return;
    }

    if(file.isEmpty())
    {
        statusMsg(QString("Cannot locate settings file '%1'").arg(settingFile),true);
        return;
    }
    if(lastModified.isNull()) lastModified = QFileInfo(file).lastModified();


    QList<pairsocket> pair;
    QHostAddress host[2]; int port[2];
    if(!loadXML(&pair, file, 10)) return;

    statusMsg("Processing ...");
    ui->statusBar->repaint();
    //qApp->processEvents();

    // check address port
    for(QList<pairsocket>::iterator it = pair.begin(); it != pair.end(); it ++)
    {
        QHostAddress host; int port;
        if(!getHostAddr(it->input, &host, &port))
        {
            statusMsg(QString("Improper host address:port for '%1'").arg(it->input),true);
            return;
        }
        if(!getHostAddr(it->dest, &host, &port))
        {
            statusMsg(QString("Improper host address:port for '%1'").arg(it->dest),true);
            return;
        }
        if(host == QHostAddress::Any)
        {
            statusMsg(QString("Cannot assign 'any address' in '%1'").arg(it->dest),true);
            return;
        }
    }

    for(QMap<QString,INFO>::iterator jt = servers.begin();jt!=servers.end(); )
    {
        bool found = false;

        for(QList<pairsocket>::iterator it = pair.begin(); it != pair.end(); it ++)
        {
            QString s = it->input+","+it->dest;
            s.replace(" ","");
            if(jt.key() == s)
            {
                found = true; pair.erase(it); break;
            }
        }
        if(!found)
        {
            EchoServer *s = &jt->server->servers;
            QString pair = jt.key().split(",")[0];
            getHostAddr(pair, &host[0], &port[0]);
            jt = servers.erase(jt);
            s->remove(host[0]);
            putLine(tr("Disconnecting %1").arg(pair));
        }
        else jt ++;
    }

    bool error=false;
    // adding new ports
    for(QList<pairsocket>::iterator it = pair.begin(); it != pair.end(); it ++)
    {
        if( !getHostAddr(it->input, &host[0], &port[0]) ||
            !getHostAddr(it->dest, &host[1], &port[1])) continue;

        QString s = it->input+","+it->dest;
        QMap<QString,INFO>::iterator jt = servers.insert(s.replace(" ",""), INFO());
        jt->startTime.start();
        jt->connected = false;
        jt->server = NULL;

        for(SERVERS *p : ports)
        {
            if(p->servers.getPort() == port[0])
            {
                jt->server = p;
                break;
            }
        }
        auto func=[s, this](){
            QMap<QString,INFO>::iterator jt = servers.find(s);
            if(jt == servers.end()) return;
            servers.erase(jt);
            //syncPorts();
        };
        if(jt->server == NULL)
        {
            jt->server = new SERVERS (port[0], true, this);
            ports.push_back(jt->server);
            connect(&jt->server->servers, SIGNAL(connected(QString,int,QString,int)), this,
                            SLOT(connected(QString,int,QString,int)));
            connect(&jt->server->servers, SIGNAL(disconnected(QString,int,QString,int)), this,
                            SLOT(disconnected(QString,int,QString,int)));
            //connect(&jt->server->servers, &EchoServer::failAccept, this, func);
            connect(&jt->server->servers, &EchoServer::failListen, this, func);

        }
        putLine(tr("Listening %1").arg(it->input));
        if(!jt->server->servers.addAccepts(host[0],host[1],port[1])) func();
        else jt->server->servers.listen();

        // jt->server->servers.addAccepts(host[0],host[1],port[1]);
    }
    //for(SERVERS *p : ports) p->servers.waitUntilCommandEmpty();
    syncPorts();
    settingFileDT = lastModified;

    updateTableView();
    if(!error) statusClear();
    else statusMsg("Refer to text status", true);

}
void MainWindow::disconnected(const QString &inAddr, int inPort,
                              const QString &outAddr, int outPort)
{
    QMap<QString,INFO>::iterator found;
    QString addr = inAddr;
    if(inAddr == QHostAddress(QHostAddress::Any).toString()) addr = "";

    if((found = servers.find(QString("%1:%2,%3:%4").arg(addr).arg(inPort).arg(outAddr).arg(outPort))) !=
            servers.end())
    {
        EchoServer *s = &found->server->servers;
        s->remove(QHostAddress(inAddr));
        servers.erase(found);
        syncPorts();
        updateTableView();

        pairsocket pair;
        pair.input = QString("%1 : %2").arg(addr).arg(inPort);

        putLine(tr("Disconnecting ")+pair.input.trimmed());
        pair.input.replace(" ","");
        pair.dest = QString("%1:%2").arg(outAddr).arg(outPort);

        removeFromXML(getSettingFile(), QList<pairsocket>()<<pair, 10);

    }
}

void MainWindow::connected(const QString &inAddr, int inPort, const QString &outAddr, int outPort)
{
    EchoServer *server = (EchoServer *) this->sender();
    bool found = false;

    for(QMap<QString,INFO>::iterator jt = servers.begin();jt!=servers.end(); jt ++)
    {
        QHostAddress thost; int tport;
        if(server != &jt->server->servers) continue;
        if(!getHostAddr(jt.key().split(",")[0], &thost, &tport)) continue;
        if(thost.toString() != inAddr || tport != inPort)
            continue;

        if(!getHostAddr(jt.key().split(",")[1], &thost, &tport)) continue;
        if(thost.toString() != outAddr || tport != outPort)
            continue;

        found = true; jt->connected = true; break;
    }
    if(found) updateTableView();


}

void MainWindow::putLine(const QString &s)
{
    QString txt = QString("[%1] %2")
                    .arg(QDateTime::currentDateTime().toString("d MMMM yyyy, hh:mm:ss.zzz"))
                    .arg(s);
    QTextDocument *doc = ui->editText->document();
    ui->editText->insertPlainText((doc->isEmpty()?"":"\n")+txt);

    while(doc->characterCount() > 1024*1024)
    {
        QTextCursor text(doc);
        text.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor,1);
        text.removeSelectedText();
    }

    // put the copy at log too
    {
        QFile log(getLogFile());
        if(log.open(QFile::Append))
        {
            QTextStream (&log) << txt << endl;
            log.close();
        }
    }
}

int MainWindow::findPairFromTable(const pairsocket &pair)
{
    QStandardItemModel *model = static_cast<QStandardItemModel*>(ui->tableView->model());
    QString comp = pair.input + "_" + pair.dest;
    for(int j = 0; j < model->rowCount(); j ++)
    {
        if(comp == model->item(j,0)->text()+"_"+model->item(j,1)->text())
        {
            return j;
        }
    }
    return -1;
}

//void MainWindow::refreshTimer()
//{
//    refresh(false);
//    if(REFRESH)
//        QTimer::singleShot(REFRESH, this,SLOT(refreshTimer()));
//}

void MainWindow::timer()
{
    QList<pairsocket> toRemove;

    for(QMap<QString,INFO>::iterator it = servers.begin(); it != servers.end(); it ++)
    {
        QHostAddress addr; int port;
        QString key = it.key();
        if(!getHostAddr(key.split(",")[0], &addr, &port)) continue;

        if(IDLE && it->server->servers.getElapsed(addr.toString()) > IDLE)
        {
            pairsocket p;
            (p.input = key.split(",")[0]).replace(" ","");
            (p.dest = key.split(",")[1]).replace(" ","");
            toRemove.push_back(p);
        }
    }
    if(!toRemove.isEmpty()) {
        removeFromXML(getSettingFile(), toRemove, 10);
        refresh();
    }
    else if(REFRESH != 0 && refreshTimer.elapsed() > REFRESH) refresh(false);
    else updateTableView();
    QTimer::singleShot(200, this, SLOT(timer()));
}

QString MainWindow::elapsedTime(int milliseconds)
{
    int h = milliseconds / 3600/1000;
    int m = (milliseconds - h * 3600*1000) / 60000;
    int s = (milliseconds %(60*1000) + 500) / 1000;
    return QString("%1 : %2 : %3").arg(h).arg(m, 2, 10, QLatin1Char('0')).arg(s, 2, 10, QLatin1Char('0'));
}

void MainWindow::updateTableView()
{
    QStandardItemModel *model = static_cast<QStandardItemModel*>(ui->tableView->model());
    QSet<int>  disconnected;
    for(int i = 0; i < model->rowCount(); i ++) disconnected.insert(i);

    for(const QString &key : servers.keys())
    {
        int r;
        pairsocket pair;
        (pair.input = key.split(",")[0]).replace(":", " : ");
        (pair.dest = key.split(",")[1]).replace(":", " : ");

        if((r=findPairFromTable (pair)) == -1)
        {
            QList<QStandardItem*> list = QList<QStandardItem*>()
                    << new QStandardItem(pair.input)
                    << new QStandardItem(pair.dest)
                    << new QStandardItem("Listening")
                    << new QStandardItem(elapsedTime(servers.value(key).startTime.elapsed()));
            for(QStandardItem *i : list) i->setEditable(false);

            list.at(2)->setTextAlignment(Qt::AlignCenter);
            list.at(3)->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            model->appendRow(list);
        }
        else
        {
            model->item(r,2)->setText(servers.value(key).connected?"Connected":"Listening");
            model->item(r,3)->setText(elapsedTime(servers.value(key).startTime.elapsed()));
            disconnected.remove(r);
        }
    }
    //if(sysIcon)
    {
            int connected = model->rowCount()- disconnected.size();
            if(connected) sysIcon.setToolTip(QString("%1 connected").arg(connected));
            else sysIcon.setToolTip("");
    }
    for(QSet<int>::iterator it = disconnected.begin(); it != disconnected.end(); it++)
    {
        int r = *it;
        if(model->item(r,2)->text() != "Disconnected")
        {
            model->item(r,2)->setText("Disconnected");
            model->item(r,3)->setText("");
        }
    }
}

bool MainWindow::removeFromXML(const QString &fname, const QList<pairsocket> &list, int retry)
{
    QDomDocument doc;
    QString msg; int line, col;
    QFile file(fname);
    //QByteArray data;
    bool result = false;

    if(fname.isEmpty()) return false;

    while (!file.open(QIODevice::ReadWrite))
    {
        if(retry <= 0)
        {
            statusMsg(tr("Cannot open file '")+QFileInfo(fname).fileName()+"' for write",true);
            goto end;
        }
        QThread::msleep(50); retry --;
    }
    if(!doc.setContent(file.readAll(), &msg, &line, &col))
    {
        statusMsg(QString("@(line=%1, column=%2) %3)").arg(line).arg(col).arg(msg),true);
        goto end;
    }

    {
        QDomElement items;
        for(QDomNode n = doc.documentElement();!n.isNull();n = n.nextSibling()) {
            QDomElement e = n.toElement(); // try to convert the node to an element.
            if(e.isNull()) continue;
            if(e.tagName().compare("items") != 0) continue;
            items = e;
            break;
        }

        if(!items.isNull())
        {
            QDomNodeList nodes = items.childNodes();
            bool updated = false;
            for(int i = nodes.size()-1; i >= 0; i --)
            {
                QDomElement e = nodes.at(i).toElement(), k;
                if(e.tagName() != "item") continue;

                for(const pairsocket &p : list)
                {
                    QString s;
                    if(!(k = e.firstChildElement("ip")).isNull())(s = k.text()).replace(" ","");
                    if(s != p.input) continue;
                    if(!(k = e.firstChildElement("dest")).isNull())(s = k.text()).replace(" ","");
                    if(s != p.dest) continue;
                    items.removeChild(nodes.at(i));
                    updated= true;
                    break;
                }
            }
            if(updated)
            {
                file.seek(0);
                file.write(doc.toByteArray());
                file.resize(file.pos());
            }
        }
        result = true;
    }

end:
    if(file.isOpen()) file.close();
    return result;
}

void MainWindow::copyTable()
{
     QModelIndexList selected = ui->tableView->selectionModel()->selectedIndexes();
     QStandardItemModel *model = static_cast<QStandardItemModel*>(ui->tableView->model());
     int mincol=0;
     qSort(selected.begin(), selected.end(),[&mincol](const QModelIndex &a, const QModelIndex &b){
        mincol = qMin(qMin(mincol, a.column()),b.column());
        if(a.row() < b.row()) return true;
        else if(a.row() > b.row()) return false;
        else return a.column() < b.column();
     });
     QString out, header;

     for(int i = 0; i < model->columnCount(); i ++)
     {
         if(!header.isEmpty()) header.push_back("\t");
         header.push_back(model->horizontalHeaderItem(i)->text());
     }
     int row = 0, col = mincol;
     for(QModelIndex i : selected)
     {
         while(i.row() > row)
         {
             if(!out.isEmpty())out.push_back("\r\n");
             row ++;
             col = mincol;
         }
         while(i.column()>col)
         {
             out.push_back("\t");
             col ++;
         }
         out.push_back(model->itemFromIndex(i)->text());
     }
     if(!out.isEmpty())out.push_back("\r\n");
     qApp->clipboard()->setText(header+"\r\n"+out);
}


void MainWindow::disconnectSelected()
{
    QSet<int> sets;
    QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
    QStandardItemModel *model = static_cast<QStandardItemModel*>(ui->tableView->model());
    for(QModelIndex i : selected) sets.insert(i.row());
    QList<pairsocket> list;
    for(int i : sets)
    {
        pairsocket p;
        (p.input = model->item(i,0)->text()).replace(" ","");
        (p.dest = model->item(i,1)->text()).replace(" ","");
        list.push_back(p);
    }
    if(!list.isEmpty()) {
        this->removeFromXML(getSettingFile(), list);
        refresh();
    }
}

void MainWindow::removeDisconnected()
{
    updateTableView();
    QStandardItemModel *model = static_cast<QStandardItemModel*>(ui->tableView->model());
    for(int n = 0, start=-1, i = model->rowCount()-1; i>=0; i--)
    {
        if(model->item(i,2)->text() == "Disconnected")
        {
            if(n == 0) start = i;
            n ++;
            if(i > 0) continue;
        }
        if(n > 0)
        {
            model->removeRows(start-n+1, n);
        }
        n = 0;
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    if(ev->type() == QEvent::ContextMenu)
    {
        QContextMenuEvent *event = static_cast<QContextMenuEvent *>(ev);
        QMenu *menu = NULL;
        QAction *act;
        QPoint pt = event->pos();

        if(obj == ui->editText)
        {
            menu = ui->editText->createStandardContextMenu(pt);
            //menu->insertSeparator(NULL);

            menu->insertAction(NULL,act = Utils::findAction("C&lear", ui->editText->actions()));
            act->setEnabled(!ui->editText->document()->isEmpty());
        }

        if(obj == ui->tableView)
        {
            menu = new QMenu (this);
            bool selected = ui->tableView->selectionModel()->selectedIndexes().size()>0;
            act=menu->addAction("Disconnect selected", this, SLOT(disconnectSelected()));
            act->setEnabled(selected);
            act=menu->addAction("Remove disconnected", this, SLOT(removeDisconnected()));
            act->setEnabled(ui->tableView->model()->rowCount()>0);
            menu->insertSeparator(NULL);

            menu->insertAction(NULL, act=Utils::findAction("&Copy", ui->tableView->actions()));
            //act->setEnabled(selected);
            menu->insertAction(NULL, act=Utils::findAction("Select &All", ui->tableView->actions()));

            //menu.insertAction(NULL, Utils::findAction(obj, QKeySequence::Copy));
            //menu.insertAction(NULL, QAction("Disconnect selected", &menu));
        }

        if(menu != NULL)
        {
            menu->setAttribute(Qt::WA_DeleteOnClose);
            menu->popup(static_cast<QWidget*>(obj)->mapToGlobal(pt));

            return true;
        }
    }
    return false;
}

MainWindow::~MainWindow()
{
    delete ui;
}
