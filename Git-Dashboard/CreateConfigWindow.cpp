#include <iostream>

#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>
#include <filesystem>

#include <QFileDialog>
#include <QInputDialog>

#include "CreateConfigWindow.h"
#include "ui_CreateConfigWindow.h"

using std::cout;
using std::endl;
using std::string;

extern void showMainWindow();

/**
 * Constructor.
 */
CreateConfigWindow::CreateConfigWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CreateConfigWindow)
{
    ui->setupUi(this);

    model.addColumn("Visible", [](Repository::Pointer ptr) { return !ptr->isHidden(); });
    model.addColumn("Project", [](Repository::Pointer ptr) { return QString::fromStdString(ptr->gitRemote()->name()); } );
    model.addColumn("Branch", [](Repository::Pointer ptr) { return QString::fromStdString(ptr->currentBranch()); } );

    Configuration & config = Configuration::singleton();

    ui->columnsSpin->setValue(config.getColumns());
    ui->rowsSpin->setValue(config.getRows());

    ui->repositoriesTable->setModel(&model);

    QHeaderView *headerView = new QHeaderView(Qt::Horizontal, ui->repositoriesTable);
    ui->repositoriesTable->setHorizontalHeader(headerView);
    headerView->setSectionResizeMode(1, QHeaderView::Stretch);
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
    QString dirName = QFileDialog::getExistingDirectory(this, "Repository Directory", getStartingDirectory());
    if (dirName.isEmpty()) {
        return;
    }
    rememberParent(dirName);

    Repository::Pointer repo =  Configuration::singleton().addRepository(dirName.toStdString());

    SSHKey::Pointer key = repo->getKey();
    if (key->getPassword().length() == 0) {
        bool ok;
        string prompt = string{"Password for "} + key->getPrivateFile();
        QString pw = QInputDialog::getText(
                    this,
                    tr("Enter Password"),
                    QString::fromStdString(prompt),
                    QLineEdit::Password,
                    tr(""),
                    &ok);

        if (ok && !pw.isEmpty()) {
            key->setPassword(pw.toStdString());
        }
    }

    Configuration::save();
    update();
}

/**
 * Where does our File Dialog start when adding a new repo.
 * We start at their home directory the first time and then
 * remember the parent directory after that.
 *
 * This method may not work on Windows -- no idea.
 */
QString
CreateConfigWindow::getStartingDirectory() {
    if (startFromDir.length() == 0) {
        struct passwd * pwd = getpwuid(getuid());
        startFromDir = pwd->pw_dir;
    }

    return QString::fromStdString(startFromDir);
}

/**
 * They asked us to open the repo represented by this directory.
 * We store the parent.
 */
void
CreateConfigWindow::rememberParent(const QString &dirName) {
    std::filesystem::path dirPath(dirName.toStdString());
    std::filesystem::path parentPath = dirPath.parent_path();

    startFromDir = parentPath.generic_string();
}

/**
 * Save and switch to the main window.
 */
void CreateConfigWindow::on_mainWindowPB_clicked()
{
    showMainWindow();
}

void CreateConfigWindow::on_rowsSpin_valueChanged(int value)
{
    Configuration::singleton().setRows(value);
    Configuration::save();
}

void CreateConfigWindow::on_columnsSpin_valueChanged(int value)
{
    Configuration::singleton().setColumns(value);
    Configuration::save();
}
