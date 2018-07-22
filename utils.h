#include <QObject>
#include <QWidget>
#include <QAction>
#include <QModelIndex>
#include <QComboBox>
#include <QAbstractItemView>
#include <QHeaderView>

#ifndef UTILS_H
#define UTILS_H


class Utils
{
public:
    Utils();

    static QAction *findAction (QWidget *wdg, const QKeySequence &key);
    static QAction *findAction (const QString &s, const QList<QAction*> &list, bool matchStart=false);


//    static int itemCopy(QString *out, QAbstractItemView *view, QHeaderView *header=NULL, QObject *obj=NULL);


//

//    static QModelIndex itemFind(const QString &data, const QModelIndex &from,
//                                 bool caps, bool backward, bool wrap, bool whole,
//                                 bool skipfirst, const QList<int> &blockCol,
//                                 QAbstractItemModel *model, QObject *obj=NULL);
//    static void addComboNewItem (QComboBox *box, const QString &s);

    static QString fromPtr(QString *s);
};

#endif // UTILS_H
