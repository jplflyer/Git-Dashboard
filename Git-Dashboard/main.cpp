#include <QApplication>

#include <git2/global.h>

#include "Configuration.h"

#include "CreateConfigWindow.h"
#include "FetchWorker.h"
#include "GetPasswordsWindow.h"
#include "MainWindow.h"

extern void showMainWindow();
extern void showConfigWindow();

//======================================================================
//
//======================================================================
static MainWindow *mainWindow = nullptr;
static CreateConfigWindow *configWindow = nullptr;
static FetchWorker *fetchWorker = nullptr;

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
    if (fetchWorker == nullptr) {
        fetchWorker = new FetchWorker();
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
        // We haven't been run before.
        showConfigWindow();
    }
    else {
        StringVector sshFilesNeedingPasswords = config.sshFilesNeedingPasswords();
        if (sshFilesNeedingPasswords.size() > 0) {
            GetPasswordsWindow * win = new GetPasswordsWindow(sshFilesNeedingPasswords);
            win->setAttribute(Qt::WA_DeleteOnClose);
            win->show();
        }
        else {
            // No pws needed.
            showMainWindow();
        }
    }

    int rv = app.exec();
    git_libgit2_shutdown();
    return rv;
}
