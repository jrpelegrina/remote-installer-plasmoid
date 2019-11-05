/*
 * Copyright (C) 2015 Dominik Haumann <dhaumann@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "RemoteInstallerIndicator.h"
#include <KLocalizedString>
#include <KFormat>
#include <KNotification>
#include <KRun>
#include <QTimer>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QDebug>
#include <QFile>
#include <QDir>



RemoteInstallerIndicator::RemoteInstallerIndicator(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    , m_timer_run(new QTimer(this))
    
{
    

    WATCH_DIR="/tmp/.LLXRemoteInstallerClient";
    QString sh_token_path=WATCH_DIR.absoluteFilePath("llxremote_sh_token");
    llxremote_sh.setFileName(sh_token_path);
    QString apt_token_path=WATCH_DIR.absoluteFilePath("llxremote_apt_token");
    llxremote_apt.setFileName(apt_token_path);
    QString deb_token_path=WATCH_DIR.absoluteFilePath("llxremote_deb_token");
    llxremote_deb.setFileName(deb_token_path);


    connect(m_timer, &QTimer::timeout, this, &RemoteInstallerIndicator::worker);
    m_timer->start(5000);
    worker();
   
   
}


void RemoteInstallerIndicator::worker(){

    if (!is_working){
        if (RemoteInstallerIndicator::llxremote_sh.exists() || RemoteInstallerIndicator::llxremote_apt.exists() || RemoteInstallerIndicator::llxremote_deb.exists()) {
            isAlive();
        }else{
        	if (last_check>FREQUENCY){
        		m_timer->stop();
        		setStatus(HiddenStatus);
                
   		  	}else{
   		  		last_check=last_check+5;
   		  	}
   		}  	
   	}	  	

}    


void RemoteInstallerIndicator::isAlive(){

    is_working=true;
   	llxremote_mode="";

	if (RemoteInstallerIndicator::llxremote_sh.exists()){
		llxremote_mode="sh";

	}else{
		if (RemoteInstallerIndicator::llxremote_apt.exists()){
			llxremote_mode="apt";
		}else{
			llxremote_mode="deb";
		}
	}		

	changeTryIconState(llxremote_mode);
    
    connect(m_timer_run, &QTimer::timeout, this, &RemoteInstallerIndicator::checkRemoteInstaller);
    m_timer_run->start(5000);
    checkRemoteInstaller();



}

void RemoteInstallerIndicator::checkRemoteInstaller(){


	if (llxremote_mode=="sh"){
		if (!RemoteInstallerIndicator::llxremote_sh.exists()){
				m_timer_run->stop();
	        	is_working=false;
	        	setStatus(PassiveStatus);
	    }    	

	}else{
		if (llxremote_mode=="apt"){
			if (!RemoteInstallerIndicator::llxremote_apt.exists()){
				m_timer_run->stop();
	        	is_working=false;
	        	setStatus(PassiveStatus);	
	        }	

		}else{
			if (!RemoteInstallerIndicator::llxremote_deb.exists()){
				m_timer_run->stop();
	        	is_working=false;
	        	setStatus(PassiveStatus);
	        }
	    }
	 }       	


}	


RemoteInstallerIndicator::TrayStatus RemoteInstallerIndicator::status() const
{
    return m_status;
}


void RemoteInstallerIndicator::changeTryIconState(QString mode){

    const QString tooltip(i18n("LliureX Remote Installer"));
    setStatus(NeedsAttentionStatus);
    setIconName("remoteinstallernotifier");
    setToolTip(tooltip);
    QString title(i18n("LliureX Remote Installer is running"));
      
	if (mode=="sh"){
		QString sh_label(i18n(": Executable Mode"));
		const QString subtooltip(title+sh_label);
	    setSubToolTip(subtooltip);
        notification = KNotification::event(QStringLiteral("Run"), subtooltip, {}, "lliurex-remote-installer-indicator", nullptr, KNotification::CloseOnTimeout , QStringLiteral("llxremoteinstallernotifier"));
	       
	}else{
	  	if (mode=="apt"){
			QString apt_label(i18n(": Apt Mode"));
	   		const QString subtooltip(title+apt_label);
  		    setSubToolTip(subtooltip);
	        notification = KNotification::event(QStringLiteral("Run"), subtooltip, {}, "lliurex-remote-installer-indicator", nullptr, KNotification::CloseOnTimeout , QStringLiteral("llxremoteinstallernotifier"));

	   	}else{
			QString deb_label(i18n(": Deb Mode"));
	  		const QString subtooltip(title+deb_label);
  		    setSubToolTip(subtooltip);
	        notification = KNotification::event(QStringLiteral("Run"), subtooltip, {}, "lliurex-remote-installer-indicator", nullptr, KNotification::CloseOnTimeout , QStringLiteral("llxremoteinstallernotifier"));

	   	}	
	}
    
      
}


void RemoteInstallerIndicator::setStatus(RemoteInstallerIndicator::TrayStatus status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}

QString RemoteInstallerIndicator::iconName() const
{
    return m_iconName;
}

void RemoteInstallerIndicator::setIconName(const QString &name)
{
    if (m_iconName != name) {
        m_iconName = name;
        emit iconNameChanged();
    }
}

QString RemoteInstallerIndicator::toolTip() const
{
    return m_toolTip;
}

void RemoteInstallerIndicator::setToolTip(const QString &toolTip)
{
    if (m_toolTip != toolTip) {
        m_toolTip = toolTip;
        emit toolTipChanged();
    }
}

QString RemoteInstallerIndicator::subToolTip() const
{
    return m_subToolTip;
}

void RemoteInstallerIndicator::setSubToolTip(const QString &subToolTip)
{
    if (m_subToolTip != subToolTip) {
        m_subToolTip = subToolTip;
        emit subToolTipChanged();
    }
}