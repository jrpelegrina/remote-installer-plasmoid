if(NOT WIN32)
    # uses the command line tool 'quota', does not make sense on Windows
    add_subdirectory(remoteinstallernotifier)
endif(NOT WIN32)
