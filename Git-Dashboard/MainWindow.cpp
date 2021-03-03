#include <iostream>
#include <math.h>

#include <QLayout>

#include "MainWindow.h"
#include "ui_MainWindow.h"

using std::cout;
using std::endl;

// Yeah, I know, this is naughty.
extern void showConfigWindow();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menubar->setVisible(true);

    Configuration & config = Configuration::singleton();
    int numRows = config.getRows();
    int numCols = config.getRows();
    const Repository::Vector & repos = config.getRepositories();
    int repoCount = repos.size();

    if (numRows == 0 && numCols == 0) {
        numRows = ceil(static_cast<double>(repoCount) / 4.0);
        if (numRows == 0) {
            numRows = 1;
        }
        numCols = 4;
    }
    else if (numRows > 0) {
        numCols = ceil(static_cast<double>(repoCount) / static_cast<double>(numRows));
    }
    else {
        numRows = ceil(static_cast<double>(repoCount) / static_cast<double>(numCols));
    }

    cout << "Number of repos: " << repoCount
         << ". Rows: " << numRows << ". Cols: " << numCols
         << endl;

    // Mark top rows visible.
    for (int index = 0; index < rows.size() && index < numRows; ++index) {
        QWidget * thisRow = rows.at(index);
        thisRow->setVisible(true);
    }

    // Mark missing rows invisible.
    for (int index = rows.size() - 1; index >= numRows; --index) {
        QWidget * thisRow = rows.at(index);
        thisRow->setVisible(false);
    }

    // Create more rows if necessary.
    for (int index = rows.size(); index < numRows; ++index) {
        QWidget * thisRow = new QWidget(ui->rowsContainer);
        QHBoxLayout * layout = new QHBoxLayout();
        thisRow->setLayout(layout);
        ui->rowsContainer->layout()->addWidget(thisRow);
        thisRow->show();

        rows.push_back(thisRow);
    }

    for (int index = hostForms.size(); index < repoCount; ++index) {
        Repository::Pointer repo = repos[index];
        QWidget * thisRow = rows.at(index / numCols);
        HostForm * form = new HostForm(repo, thisRow);
        thisRow->layout()->addWidget(form);
        form->show();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenConfiguration_triggered()
{
    showConfigWindow();
}
