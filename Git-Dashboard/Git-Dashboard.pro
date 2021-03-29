ICON    = Git-Dashboard.icns
QT       += core gui widgets

CONFIG  += c++17
CONFIG  += sdk_no_version_check

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15

INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib
LIBS += -L/usr/local/opt/openssl/lib
LIBS += -lshowgit-mac -lshow-mac -lgit2 -lcurl -lssl -lz -liconv -lssh2 -lcrypto
LIBS += -framework CoreFoundation -framework Security

SOURCES += \
    AboutWindow.cpp \
    Configuration.cpp \
    CreateConfigWindow.cpp \
    FetchWorker.cpp \
    GetPasswordsWindow.cpp \
    HostForm.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    AboutWindow.h \
    Configuration.h \
    CreateConfigWindow.h \
    FetchWorker.h \
    GenericItemModel.h \
    GetPasswordsWindow.h \
    HostForm.h \
    MainWindow.h \
    Version.h

FORMS += \
    AboutWindow.ui \
    CreateConfigWindow.ui \
    GetPasswordsWindow.ui \
    HostForm.ui \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../Git-Dashboard/Git-Dashboard/Git-Dashboard.icns

RESOURCES += \
    Images.qrc
