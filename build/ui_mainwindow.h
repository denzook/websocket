/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionE_xit;
    QAction *actionOpen;
    QAction *actionsave_text;
    QAction *actionClear_text;
    QAction *actionAbout_QT;
    QAction *actionAbout;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_3;
    QSplitter *widget;
    QWidget *layoutWidget;
    QVBoxLayout *layout2;
    QLabel *txtTable;
    QPlainTextEdit *editText;
    QWidget *layoutWidget2;
    QVBoxLayout *layout1;
    QFrame *line;
    QPushButton *btnRefresh;
    QTableView *tableView;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(419, 346);
        actionE_xit = new QAction(MainWindow);
        actionE_xit->setObjectName(QStringLiteral("actionE_xit"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionOpen->setEnabled(false);
        actionsave_text = new QAction(MainWindow);
        actionsave_text->setObjectName(QStringLiteral("actionsave_text"));
        actionsave_text->setEnabled(false);
        actionClear_text = new QAction(MainWindow);
        actionClear_text->setObjectName(QStringLiteral("actionClear_text"));
        actionAbout_QT = new QAction(MainWindow);
        actionAbout_QT->setObjectName(QStringLiteral("actionAbout_QT"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionAbout->setEnabled(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_3 = new QVBoxLayout(centralWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        widget = new QSplitter(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setOrientation(Qt::Vertical);
        layoutWidget = new QWidget(widget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layout2 = new QVBoxLayout(layoutWidget);
        layout2->setSpacing(6);
        layout2->setContentsMargins(11, 11, 11, 11);
        layout2->setObjectName(QStringLiteral("layout2"));
        layout2->setContentsMargins(0, 0, 0, 0);
        txtTable = new QLabel(layoutWidget);
        txtTable->setObjectName(QStringLiteral("txtTable"));

        layout2->addWidget(txtTable);

        editText = new QPlainTextEdit(layoutWidget);
        editText->setObjectName(QStringLiteral("editText"));
        editText->setReadOnly(true);

        layout2->addWidget(editText);

        widget->addWidget(layoutWidget);
        layoutWidget2 = new QWidget(widget);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layout1 = new QVBoxLayout(layoutWidget2);
        layout1->setSpacing(6);
        layout1->setContentsMargins(11, 11, 11, 11);
        layout1->setObjectName(QStringLiteral("layout1"));
        layout1->setContentsMargins(0, 0, 0, 0);
        line = new QFrame(layoutWidget2);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        layout1->addWidget(line);

        btnRefresh = new QPushButton(layoutWidget2);
        btnRefresh->setObjectName(QStringLiteral("btnRefresh"));

        layout1->addWidget(btnRefresh);

        tableView = new QTableView(layoutWidget2);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setSortingEnabled(true);
        tableView->setWordWrap(false);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->setVisible(false);

        layout1->addWidget(tableView);

        widget->addWidget(layoutWidget2);

        verticalLayout_3->addWidget(widget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 419, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
#ifndef QT_NO_SHORTCUT
        txtTable->setBuddy(editText);
#endif // QT_NO_SHORTCUT

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionsave_text);
        menuFile->addAction(actionClear_text);
        menuFile->addSeparator();
        menuFile->addAction(actionE_xit);
        menuAbout->addAction(actionAbout);
        menuAbout->addAction(actionAbout_QT);

        retranslateUi(MainWindow);
        QObject::connect(actionE_xit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "WebSocket", Q_NULLPTR));
        actionE_xit->setText(QApplication::translate("MainWindow", "E&xit", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("MainWindow", "&Open", Q_NULLPTR));
        actionsave_text->setText(QApplication::translate("MainWindow", "Save text", Q_NULLPTR));
        actionClear_text->setText(QApplication::translate("MainWindow", "Clear text", Q_NULLPTR));
        actionAbout_QT->setText(QApplication::translate("MainWindow", "About &QT", Q_NULLPTR));
        actionAbout->setText(QApplication::translate("MainWindow", "&About", Q_NULLPTR));
        txtTable->setText(QApplication::translate("MainWindow", "&Text status", Q_NULLPTR));
        btnRefresh->setText(QApplication::translate("MainWindow", "&Refresh", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "&Menu", Q_NULLPTR));
        menuAbout->setTitle(QApplication::translate("MainWindow", "&Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
