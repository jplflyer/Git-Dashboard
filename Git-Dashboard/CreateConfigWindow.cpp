#include <iostream>
#include <algorithm>
#include <filesystem>

#include <pwd.h>
#include <unistd.h>

#include <showlib/NumericOperators.h>

#include <QFileDialog>
#include <QInputDialog>

#include "CreateConfigWindow.h"
#include "ui_CreateConfigWindow.h"
#include "GetPasswordsWindow.h"

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

    QItemSelectionModel * selModel = ui->repositoriesTable->selectionModel();
    connect(selModel, &QItemSelectionModel::selectionChanged, this, &CreateConfigWindow::selectionChanged);
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
 * Adjust the enabled property of buttons as appropriate.
 */
void CreateConfigWindow::fixButtons() {
    const Repository::Vector & repos = Configuration::singleton().getRepositories();
    bool inRange = ShowLib::inSizeRange(selectedRow, repos.size());

    ui->removePB->setEnabled(inRange);
    ui->upPB->setEnabled(inRange && selectedRow > 0);
    ui->downPB->setEnabled( ShowLib::inSizeRange(selectedRow, repos.size() - 1) );
}

/**
 * Add a repository.
 */
void CreateConfigWindow::on_addRepoPB_clicked() {
    QString dirName = QFileDialog::getExistingDirectory(this, "Repository Directory", getStartingDirectory());
    if (dirName.isEmpty()) {
        return;
    }
    rememberParent(dirName);

    Repository::Pointer repo =  Configuration::singleton().addRepository(dirName.toStdString());

    /*
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
    */

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
void CreateConfigWindow::on_mainWindowPB_clicked() {
    StringVector sshFilesNeedingPasswords = Configuration::singleton().sshFilesNeedingPasswords();
    if (sshFilesNeedingPasswords.size() > 0) {
        GetPasswordsWindow * win = new GetPasswordsWindow(sshFilesNeedingPasswords);
        win->setAttribute(Qt::WA_DeleteOnClose);
        win->show();
    }
    else {
        showMainWindow();
    }

    close();
}

/**
 * The number of rows field uses a spinner. They changed the value.
 */
void CreateConfigWindow::on_rowsSpin_valueChanged(int value) {
    Configuration::singleton().setRows(value);
    Configuration::save();
}

/**
 * The number of columns field uses a spinner. They changed the value.
 */
void CreateConfigWindow::on_columnsSpin_valueChanged(int value) {
    Configuration::singleton().setColumns(value);
    Configuration::save();
}

/**
 * The selection changed. Turn on with a click, off with command-click.
 */
void
CreateConfigWindow::selectionChanged(const QItemSelection &selected, const QItemSelection &) {
    selectedRow = -1;

    const QModelIndexList & indexes = selected.indexes();
    for (const QModelIndex &index: indexes) {
        selectedRow = index.row();
    }

    fixButtons();
}


/**
 * Move current repo up. We don't need to range check, as the
 * Configuration class already does it.
 */
void CreateConfigWindow::on_upPB_clicked() {
    if (Configuration::singleton().swapRepositories(selectedRow, selectedRow - 1)) {
        Configuration::save();
        --selectedRow;
        ui->repositoriesTable->selectRow(selectedRow);
        model.setVector(Configuration::singleton().getRepositories());
        fixButtons();
    }
}

/**
 * Move current repo down.
 */
void CreateConfigWindow::on_downPB_clicked() {
    if (Configuration::singleton().swapRepositories(selectedRow, selectedRow + 1)) {
        Configuration::save();
        ++selectedRow;
        ui->repositoriesTable->selectRow(selectedRow);
        model.setVector(Configuration::singleton().getRepositories());
        fixButtons();
    }
}

/**
 * Confirmation dialogs are for wimps.
 */
void CreateConfigWindow::on_removePB_clicked() {
    if (Configuration::singleton().removeRepository(selectedRow)) {
        ui->repositoriesTable->clearSelection();
        model.setVector(Configuration::singleton().getRepositories());
        fixButtons();
    }
}
