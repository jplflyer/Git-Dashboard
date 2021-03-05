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

    palEvenOutside = palette();
    palAhead = palette();
    palBehind = palette();
    palMaster = palette();
    palBranch = palette();

    QColor colorEvenOutside;
    QColor colorAhead;
    QColor colorBehind;
    QColor colorMaster;
    QColor colorBranch;

    colorEvenOutside.setHsv(-1, 40, 200);
    colorAhead.setHsv(240, 150, 200);
    colorBehind.setHsv(0, 150, 200);
    colorMaster.setHsv(-1, 0, 255);
    colorBranch.setHsv(150, 150, 230);

    palEvenOutside.setColor(QPalette::Window, colorEvenOutside);
    palAhead.setColor(QPalette::Window, colorAhead);
    palBehind.setColor(QPalette::Window, colorBehind);
    palMaster.setColor(QPalette::Window, colorMaster);
    palBranch.setColor(QPalette::Window, colorBranch);

    ui->aheadFrame->setAutoFillBackground(true);
    ui->behindFrame->setAutoFillBackground(true);
    ui->mainContainer->setAutoFillBackground(true);

    QFont font = ui->repoNameL->font();
    font.setPointSize(font.pointSize() * 1.3);
    font.setBold(true);
    ui->repoNameL->setFont(font);

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
    string branch = repository->currentBranch();
    QPalette insidePalette =  (branch == "main" || branch == "master") ? palMaster : palBranch;
    int ahead = repository->gitRepository()->commitsAheadRemote();
    int behind = repository->gitRepository()->commitsBehindRemote();

    ui->mainContainer->setPalette(insidePalette);
    ui->aheadFrame->setPalette(ahead > 0 ? palAhead : insidePalette);
    ui->behindFrame->setPalette(behind > 0 ? palBehind : palEvenOutside);

    ui->branchValue->setText(QString::fromStdString(branch));
    ui->commitsAheadValue->setText(QString::number(ahead));
    ui->commitsBehindValue->setText(QString::number(behind));
}
