#include <iostream>

#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>

#include <QFileDialog>

#include "CreateConfigWindow.h"
#include "ui_CreateConfigWindow.h"

using std::cout;
using std::endl;
using std::string;

/**
 * Constructor.
 */
CreateConfigWindow::CreateConfigWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CreateConfigWindow)
{
    ui->setupUi(this);

    model.addColumn("Visible", [](Repository::Pointer ptr) { return !ptr->isHidden(); });
    model.addColumn("Directory", [](Repository::Pointer ptr) { return QString::fromStdString(ptr->getDirectory()); } );
    model.addColumn("Branch", [](Repository::Pointer ptr) { return QString::fromStdString(ptr->currentBranch()); } );

    ui->repositoriesTable->setModel(&model);
}

/**
 * Destructor.
 */
CreateConfigWindow::~CreateConfigWindow()
{
    delete ui;
}

/**
 * Update ourself. This might not mean much.
 */
void CreateConfigWindow::update() {
    model.setVector(Configuration::singleton().getRepositories());
}

/**
 * Add a repository.
 */
void CreateConfigWindow::on_addRepoPB_clicked()
{
    // Get home directory.
    struct passwd * pwd = getpwuid(getuid());
    QString homeDir { pwd->pw_dir };

    QString dirName = QFileDialog::getExistingDirectory(this, "Repository Directory", homeDir);
    if (Configuration::singleton().addRepository(dirName.toStdString())) {
        Configuration::save();
        update();
    }
}

/**
 * Save and switch to the main window.
 */
void CreateConfigWindow::on_mainWindowPB_clicked()
{
}
