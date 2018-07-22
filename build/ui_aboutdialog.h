/********************************************************************************
** Form generated from reading UI file 'aboutdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *pixMap;
    QLabel *label_2;
    QLabel *label_version;
    QFrame *line;
    QLabel *label_buildDate;
    QLabel *label_qtVersion;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QStringLiteral("AboutDialog"));
        AboutDialog->setWindowModality(Qt::ApplicationModal);
        AboutDialog->resize(282, 201);
        verticalLayout = new QVBoxLayout(AboutDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(AboutDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        pixMap = new QLabel(AboutDialog);
        pixMap->setObjectName(QStringLiteral("pixMap"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pixMap->sizePolicy().hasHeightForWidth());
        pixMap->setSizePolicy(sizePolicy);
        pixMap->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(pixMap);

        label_2 = new QLabel(AboutDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        label_version = new QLabel(AboutDialog);
        label_version->setObjectName(QStringLiteral("label_version"));

        verticalLayout->addWidget(label_version);

        line = new QFrame(AboutDialog);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        label_buildDate = new QLabel(AboutDialog);
        label_buildDate->setObjectName(QStringLiteral("label_buildDate"));

        verticalLayout->addWidget(label_buildDate);

        label_qtVersion = new QLabel(AboutDialog);
        label_qtVersion->setObjectName(QStringLiteral("label_qtVersion"));

        verticalLayout->addWidget(label_qtVersion);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(AboutDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(AboutDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AboutDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AboutDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "About Websocket", Q_NULLPTR));
        label->setText(QApplication::translate("AboutDialog", "Websocket is written by <a href=\"mailto:zeroforce0n@gmail.com\">zeroforce0n@gmail.com</a>", Q_NULLPTR));
        pixMap->setText(QString());
        label_2->setText(QApplication::translate("AboutDialog", "Copyright (C) 2014-2018", Q_NULLPTR));
        label_version->setText(QApplication::translate("AboutDialog", "Version: ", Q_NULLPTR));
        label_buildDate->setText(QApplication::translate("AboutDialog", "<Build Date>", Q_NULLPTR));
        label_qtVersion->setText(QApplication::translate("AboutDialog", "<html><head/><body><p>&lt;Qt version&gt;</p></body></html>", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
