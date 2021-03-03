QT       += core gui widgets

CONFIG  += c++17
CONFIG  += sdk_no_version_check

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15

INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib
LIBS += -lshowgit-mac -lshow-mac -lgit2 -lcurl -lssl -lz -liconv -lssh2 -lcrypto
LIBS += -framework CoreFoundation -framework Security

SOURCES += \
    Configuration.cpp \
    CreateConfigWindow.cpp \
    HostForm.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    Configuration.h \
    CreateConfigWindow.h \
    GenericItemModel.h \
    HostForm.h \
    MainWindow.h

FORMS += \
    CreateConfigWindow.ui \
    HostForm.ui \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target