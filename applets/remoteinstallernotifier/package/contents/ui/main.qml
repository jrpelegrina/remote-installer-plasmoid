import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQml.Models 2.3
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.components 2.0 as Components
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.kde.plasma.private.remoteinstallernotifier 1.0
							 
// Item - the most basic plasmoid component, an empty container.
Item {

	id:remoteinstallerApplet
	
	RemoteInstallerIndicator{
        id:remoteInstallerIndicator

    }


    Plasmoid.status: {
        /* Warn! Enum types are accesed through ClassName not ObjectName */
        switch (remoteInstallerIndicator.status){
            case RemoteInstallerIndicator.NeedsAttentionStatus: 
                return PlasmaCore.Types.NeedsAttentionStatus
            case RemoteInstallerIndicator.HiddenStatus:
                return PlasmaCore.Types.HiddenStatus
            case RemoteInstallerIndicator.PassiveStatus:
                return PlasmaCore.Types.PassiveStatus    
           
        }
        return  PlasmaCore.Types.PassiveStatus
        
    }

    Plasmoid.switchWidth: units.gridUnit * 5
    Plasmoid.switchHeight: units.gridUnit * 5

    Plasmoid.icon:remoteInstallerIndicator.iconName
    Plasmoid.toolTipMainText: remoteInstallerIndicator.toolTip
    Plasmoid.toolTipSubText: remoteInstallerIndicator.subToolTip

    Component.onCompleted: {
       plasmoid.removeAction("configure");
                 
    }

   
	Plasmoid.preferredRepresentation: Plasmoid.compactRepresentation
    Plasmoid.compactRepresentation: PlasmaCore.IconItem {
        source: plasmoid.icon
        MouseArea {
            anchors.fill: parent
            
        }
    }

       
 }	
