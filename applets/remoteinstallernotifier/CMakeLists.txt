
#######################################################################################
# Package
plasma_install_package(package org.kde.plasma.remoteinstallernotifier)


#######################################################################################
# Notes Library
add_definitions(-DTRANSLATION_DOMAIN="plasma_applet_org.kde.plasma.remoteinstallernotifier")

set(remoteinstallernotifier_SRCS
    plugin/plugin.cpp
    plugin/RemoteInstallerIndicator.cpp
    
   
)

add_library(remoteinstallernotifierplugin SHARED ${remoteinstallernotifier_SRCS})

target_link_libraries(remoteinstallernotifierplugin
                      Qt5::Quick
                      Qt5::Core
                      Qt5::Widgets
                      KF5::CoreAddons
                      KF5::I18n
                      KF5::Notifications
                      KF5::KIOWidgets
                      )

install(FILES plugin/qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/remoteinstallernotifier)
install(TARGETS remoteinstallernotifierplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/remoteinstallernotifier)

install(FILES "icons/remoteinstallernotifier.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/plasma/desktoptheme/default/icons/)
install(FILES "knotifications5/llxremoteinstallernotifier.notifyrc"
        DESTINATION ${DATAROOTDIR}/usr/share/knotifications5/)        
#install(FILES "autostart/lliurex-up-indicator"
#        DESTINATION ${DATAROOTDIR}/etc/bin)        
