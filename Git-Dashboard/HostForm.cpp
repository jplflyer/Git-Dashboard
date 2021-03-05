#include <iostream>

#include "HostForm.h"
#include "ui_HostForm.h"

using std::cout;
using std::endl;
using std::string;

/**
 * Constructor.
 */
HostForm::HostForm(Repository::Pointer repo, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HostForm),
    repository(repo)
{
    ui->setupUi(this);
    ui->repoNameL->setText(QString::fromStdString(repository->gitRemote()->name()));

    palEven = palette();
    palAhead = palette();
    palBehind = palette();
    palMaster = palette();
    palBranch = palette();

    QColor colorEven;
    QColor colorAhead;
    QColor colorBehind;
    QColor colorMaster;
    QColor colorBranch;

    colorEven.setHsv(-1, 40, 200);
    colorAhead.setHsv(240, 150, 200);
    colorBehind.setHsv(0, 150, 200);
    colorMaster.setHsv(-1, 0, 230);
    colorBranch.setHsv(150, 150, 230);

    palEven.setColor(QPalette::Window, colorEven);
    palAhead.setColor(QPalette::Window, colorAhead);
    palBehind.setColor(QPalette::Window, colorBehind);
    palMaster.setColor(QPalette::Window, colorMaster);
    palBranch.setColor(QPalette::Window, colorBranch);

    ui->aheadFrame->setAutoFillBackground(true);
    ui->behindFrame->setAutoFillBackground(true);
    ui->mainContainer->setAutoFillBackground(true);

    update();
}

HostForm::~HostForm()
{
    delete ui;
}

/**
 * Update.
 */
void
HostForm::update() {
    repository->fetch();

    string branch = repository->currentBranch();
    int ahead = repository->gitRepository()->commitsAheadRemote();
    int behind = repository->gitRepository()->commitsBehindRemote();

    ui->aheadFrame->setPalette(ahead > 0 ? palAhead : palEven);
    ui->behindFrame->setPalette(behind > 0 ? palBehind : palEven);

    ui->branchValue->setText(QString::fromStdString(branch));
    ui->commitsAheadValue->setText(QString::number(ahead));
    ui->commitsBehindValue->setText(QString::number(behind));

    if (branch == "main" || branch == "master") {
        ui->mainContainer->setPalette(palMaster);
    }
    else {
        ui->mainContainer->setPalette(palBranch);
    }

}
