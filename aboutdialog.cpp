/*
 * Copyright (C) 2014-2017 Johan Henriksson.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "aboutdialog.h"

#include "version.h"
#include <QProcess>
#include <QMovie>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowFlags()|Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
    
    m_ui.setupUi(this);
    QMovie *movie = new QMovie (":/images/frog.gif", QByteArray(), this);
    if(movie->isValid())
    {
        m_ui.pixMap->setMovie(movie);
        movie->start();
    }
    else {
        m_ui.pixMap->setVisible(false);
        //m_ui.pixMap->setPixmap(QPixmap(":/images/frog.gif","GIF"));
    }

    QString verStr;
    verStr.sprintf("Version: v%d.%d.%d", WEBSOCKET_MAJOR, WEBSOCKET_MINOR, WEBSOCKET_PATCH);
    m_ui.label_version->setText(verStr);

    //
    QString buildStr;
    buildStr = __DATE__;
    buildStr += " ";
    buildStr += __TIME__;
    m_ui.label_buildDate->setText("Built: " + buildStr);

    QString qtVersionStr;
    qtVersionStr.sprintf("Qt: %s (compiled) / %s (running)", QT_VERSION_STR, qVersion());
    m_ui.label_qtVersion->setText(qtVersionStr);

}
