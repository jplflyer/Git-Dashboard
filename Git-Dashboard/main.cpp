#include <QApplication>

#include <git2/global.h>

#include "Configuration.h"
#include "CreateConfigWindow.h"
#include "MainWindow.h"

extern void showMainWindow();
extern void showConfigWindow();

//======================================================================
//
//======================================================================
static MainWindow *mainWindow = nullptr;
static CreateConfigWindow *configWindow = nullptr;

//======================================================================
//
//======================================================================

/**
 * Show the main window.
 */
void showMainWindow() {
    if (mainWindow == nullptr) {
        mainWindow = new MainWindow();
    }
    mainWindow->show();
}

/**
 * Show the configuration window;
 */
void showConfigWindow() {
    if (configWindow == nullptr) {
        configWindow = new CreateConfigWindow();
    }
    configWindow->update();
    configWindow->show();
}

/**
 * Main entry point.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    git_libgit2_init();

    Configuration & config = Configuration::singleton();
    if (config.load() == 0) {
        // We've been run before.
        showMainWindow();
    }
    else {
        showConfigWindow();
    }

    int rv = a.exec();
    git_libgit2_shutdown();
    return rv;
}
