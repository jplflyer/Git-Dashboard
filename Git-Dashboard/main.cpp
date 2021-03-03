#include <QApplication>

#include <git2/global.h>

#include "Configuration.h"

#include "CreateConfigWindow.h"
#include "GetPasswordsWindow.h"
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
    bool shouldUpdate = true;

    if (mainWindow == nullptr) {
        mainWindow = new MainWindow();
        shouldUpdate = false;
    }
    mainWindow->show();
    if (shouldUpdate) {
        mainWindow->configurationChanged();
    }
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
    QApplication app(argc, argv);

    git_libgit2_init();

    Configuration & config = Configuration::singleton();
    if (config.load() == 0) {
        // We've been run before.
        showMainWindow();
    }
    else {
        StringVector sshFilesNeedingPasswords = config.sshFilesNeedingPasswords();
        if (sshFilesNeedingPasswords.size() > 0) {
            GetPasswordsWindow * win = new GetPasswordsWindow(sshFilesNeedingPasswords);
            win->setAttribute(Qt::WA_DeleteOnClose);
            win->show();
        }
        else {
            showConfigWindow();
        }
    }

    int rv = app.exec();
    git_libgit2_shutdown();
    return rv;
}
