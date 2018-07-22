#include <QStack>
#include <QTime>
#include <QApplication>
#include <QProgressDialog>
#include "utils.h"

Utils::Utils()
{

}

QAction *Utils::findAction (QWidget *wdg, const QKeySequence &key)
{
    QList<QAction *> acts = wdg->actions();
    foreach(QAction *act, acts)
    {
        if(act->shortcut().matches(key) == QKeySequence::ExactMatch) return act;
    }

    return NULL;
}

QAction* Utils::findAction(const QString &s, const QList<QAction*> &list, bool matchStart)
{
    foreach(QAction *p, list)
    {
        if((matchStart && p->text().startsWith(s)) || p->text() == s) return p;
    }
    return NULL;
}



//int Utils::itemCopy(QString *out, QAbstractItemView *view, QHeaderView *header, QObject *obj)
//{
//    QModelIndexList list = view->selectionModel()->selectedIndexes();
//    if(list.size() == 0) return 0;
//    qSort(list.begin(), list.end(), [&](const QModelIndex &a, const QModelIndex &b)
//    {
//        QStack<QModelIndex> alist, blist;
//        for(QModelIndex p = a; p != QModelIndex(); p = p.parent()) alist.push(p);
//        for(QModelIndex p = b; p != QModelIndex(); p = p.parent()) blist.push(p);

//        while (alist.size() != 0 && blist.size() != 0)
//        {
//            QModelIndex *aa, *bb;
//            if((aa = &alist.last())->row() != (bb = &blist.last())->row())
//                return aa->row() < bb->row();
//            if(alist.size() == 1 || blist.size() == 1)
//            {
//                if(alist.size() != blist.size()) return alist.size()< blist.size();
//                if(!header) return aa->column() < bb->column();
//                return header->visualIndex(aa->column()) < header->visualIndex(bb->column());
//            }
//            alist.pop(); blist.pop();
//        }
//        return true;
//    });
//    QModelIndex prow;
//    int n = 0;
//    for(QModelIndexList::iterator it = list.begin(); it != list.end(); it ++)
//    {
//        QString data;
//        QModelIndex col0;
//        if(header && header->isSectionHidden(it->column()))continue;
//        n++;
//        if(!obj || !QMetaObject::invokeMethod(obj, "getCopyText", Qt::DirectConnection,
//                                             Q_RETURN_ARG(QString, data),
//                                             Q_ARG(QModelIndex, (*it))))
//                data = it->data().toString();
//        //if(!data.isEmpty())
//        {
//            if(prow != (col0=it->model()->index(it->row(),0,it->parent()))) {
//                for(int c = out->size()-1; true; c --)
//                {
//                    if(c < 0 || out->at(c) == '\n' || !out->at(c).isSpace()){
//                        if(!out->isEmpty()) out->remove(1+c, out->size());
//                        break;
//                    }
//                }
//                if(!out->isEmpty()) out->push_back("\n");
//                prow = col0;
//            }
//            else out->push_back("\t");
//            out->push_back(data);
//        }
//    }
//    for(int c = out->size()-1; true; c --)
//    {
//        if(c < 0 || out->at(c) == '\n' || !out->at(c).isSpace()){
//            if(!out->isEmpty()) out->remove(1+c, out->size());
//            break;
//        }
//    }
//    return n;
//}



//QModelIndex Utils::itemFind(const QString &data, const QModelIndex &from,
//                             bool caps, bool backward, bool wrap, bool whole,
//                             bool skipfirst, const QList<int> &blockCol,
//                             QAbstractItemModel *model,
//                             QObject *obj)
//{
//    QString s = data.trimmed();
//    bool first=true;
//    if(s.isEmpty() || model==NULL || model->rowCount() <= 0) return QModelIndex();
//    QModelIndex start = (from == QModelIndex())?model->index(0,0):from;

//    auto checkExpand = [](QObject *obj, const QModelIndex &i){
//      if(obj==NULL) return true;
//      bool result;
//      if(QMetaObject::invokeMethod(obj, "canExpand", Qt::DirectConnection,
//                                 Q_RETURN_ARG(bool, result),
//                                 Q_ARG(QModelIndex, i))) return result;
//        return false;
//    };

//    auto moveup = [&](QModelIndex *i){
//        int col = i->column(), row = i->row();
//        int endCol = i->model()->columnCount(i->parent());

//        if(++col >= endCol) {

//            if(!(i->flags()& Qt::ItemNeverHasChildren) && i->model()->hasChildren(*i)
//                && checkExpand (obj, *i)
//            ) { *i = i->child(0,0); return true; }
//            else { col = 0;
//                   while(true)
//                   {
//                        int endRow = i->model()->rowCount(i->parent());
//                        if(++row < endRow) break;
//                        if((*i = i->parent()) == QModelIndex()) return false;
//                        row = i->row(); col = 0;
//                   }
//            }
//        }
//        *i = i->model()->index(row, col, i->parent());
//        return true;
//    };
//    auto movedn = [&](QModelIndex *i){
//        int col = i->column(), row = i->row();
//        int endCol = i->model()->columnCount(i->parent());
//        if(--col < 0) {
//            if(--row < 0)
//            {
//                if((*i = i->parent()) == QModelIndex()) return false;
//            }
//            else
//            {
//                *i = i->model()->index(row, endCol-1, i->parent());
//                while(!(i->flags()& Qt::ItemNeverHasChildren) && i->model()->hasChildren(*i)
//                      && checkExpand (obj, *i))
//                    *i = i->child(i->model()->rowCount(*i)-1,i->model()->columnCount(*i)-1);
//            }
//            row = i->row();
//            endCol = i->model()->columnCount(i->parent());
//            col = endCol-1;
//        }
//        *i = i->model()->index(row, col, i->parent());
//        return true;
//    };

//    //QRegExp regex (data,caps?Qt::CaseInsensitive:Qt::CaseSensitive,QRegExp::Wildcard);
//    QProgressDialog dlg (NULL, Qt::WindowFlags()|Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
//    dlg.setRange(0,0);
//    dlg.setWindowTitle("Find");
//    dlg.setLabelText ("Searching ...");
//    dlg.setModal(true); dlg.setVisible(false);
//    int times=0; QTime clock; clock.start();
//    bool ok2;
//    qint64 val = QLocale().toLongLong(data, &ok2);

//    for(QModelIndex i=start; !dlg.wasCanceled(); )
//    {
//        if(clock.elapsed() > 20) { clock.restart();
//            if(times > 2200/20) { dlg.setVisible(true); qApp->processEvents(); }
//            else times ++;
//        }
//        //if(regex.exactMatch(i.data().toString())) return i;
//        if((skipfirst && !first) || !skipfirst)
//        {
//            bool ok;
//            if((!whole &&i.data().toString().contains(data,caps?Qt::CaseInsensitive:Qt::CaseSensitive))||
//               (whole &&i.data().toString().compare(data,caps?Qt::CaseInsensitive:Qt::CaseSensitive)==0)
//              ) return i;
//            if( ok2 && val == QLocale().toLongLong(i.data().toString(), &ok) && ok) return i;
//        }
//        if(!backward)
//        {
//            do
//            {
//                if(!moveup(&i))
//                {
//                    if(wrap) i = model->index(0,0);
//                    else return QModelIndex();
//                }
//                if(start == i) return QModelIndex();
//            } while(blockCol.contains(i.column()));
//        }
//        else
//        {
//            do
//            {
//                if(!movedn(&i))
//                {
//                    if(wrap)
//                    {
//                        i = model->index(model->rowCount()-1,model->columnCount()-1);
//                        while(!(i.flags()& Qt::ItemNeverHasChildren) && model->hasChildren(i)
//                              && checkExpand (obj, i)
//                            ) i = i.child(model->rowCount(i)-1,model->columnCount(i)-1);
//                    }
//                    else return QModelIndex();
//                }
//                if(start == i) return QModelIndex();
//            } while(blockCol.contains(i.column()));
//        }
//        first = false;
//        //if(i.column() == 0) qInfo ("Search = %s", i.data().toString().toUtf8().constData());
//    }
//    return QModelIndex();
//}

//void Utils::addComboNewItem (QComboBox *box, const QString &s)
//{
//    for(int c =0; c < box->count(); c ++)
//    {
//        if(box->itemText(c).compare(s, Qt::CaseInsensitive) == 0)
//        {
//            if(c != 0) { box->removeItem(c); box->insertItem(0, s); }
//            return;
//        }
//    }
//    if(box->count() >= box->maxCount()) box->removeItem(box->count()-1);
//    box->insertItem(0, s);
//}

//QString Utils::fromPtr(QString *s)
//{
//    if(s == nullptr) return QString();
//    return *s;
//}
