#include <iostream>

#include <showlib/StringUtils.h>

#include "GetPasswordsWindow.h"
#include "ui_GetPasswordsWindow.h"
#include "Configuration.h"

using std::string;

using namespace ShowLib;

extern void showMainWindow();

/**
 * Constructor.
 */
GetPasswordsWindow::GetPasswordsWindow(ShowLib::StringVector &vec, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GetPasswordsWindow)
{
    ui->setupUi(this);

    sshFilesNeedingPasswords = vec;
    fileIndex = 0;

    setPrompts();

    // Force passwordTF into proper echo mode.
    on_revealCB_stateChanged(0);

    // Turn off the Next button.
    on_passwordTF_textChanged(tr(""));
}

/**
 * Destructor.
 */
GetPasswordsWindow::~GetPasswordsWindow()
{
    delete ui;
}

/**
 * Set the top prompt to indicate how many are remaining
 * and the fileNameLabel to the current file name.
 */
void
GetPasswordsWindow::setPrompts() {
    int count = sshFilesNeedingPasswords.size() - fileIndex;
    string prompt = string{"SSH Passwords Needed: "} + std::to_string(count);

    ui->topLabel->setText(QString::fromStdString(prompt));
    ui->fileNameLabel->setText(QString::fromStdString( *sshFilesNeedingPasswords.at(fileIndex) ));
}

/**
 * The revealCB controls whether we put the password filed
 * in password or normal mode.
 */
void GetPasswordsWindow::on_revealCB_stateChanged(int) {
    bool isChecked = ui->revealCB->isChecked();
    ui->passwordTF->setEchoMode(isChecked ? QLineEdit::Normal : QLineEdit::Password);
}

/**
 * Go to the next one.
 */
void GetPasswordsWindow::on_nextBtn_clicked() {
    std::string text = trim(ui->passwordTF->text().toStdString());

    Configuration & config = Configuration::singleton();
    config.setSSHPassword(*sshFilesNeedingPasswords.at(fileIndex), text);
    if (++fileIndex >= sshFilesNeedingPasswords.size()) {
        showMainWindow();
        close();
    }
    else {
        setPrompts();
        ui->passwordTF->setText(QString(""));
    }
}

void GetPasswordsWindow::on_passwordTF_textChanged(const QString &) {
    std::string text = trim(ui->passwordTF->text().toStdString());
    ui->nextBtn->setEnabled(text.length() > 0);
}

/**
 * User hit Return, which should do the same thing as hitting Next, but
 * only if there is text.
 */
void GetPasswordsWindow::on_passwordTF_returnPressed()
{
    std::string text = trim(ui->passwordTF->text().toStdString());
    if (text.length() > 0) {
        on_nextBtn_clicked();
    }
}
