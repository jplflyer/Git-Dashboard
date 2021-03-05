#include <iostream>

#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>

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
    // Get home directory.
    struct passwd * pwd = getpwuid(getuid());
    QString homeDir { pwd->pw_dir };

    QString dirName = QFileDialog::getExistingDirectory(this, "Repository Directory", homeDir);

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
