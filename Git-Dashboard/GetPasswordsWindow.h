#pragma once

#include <QWidget>

#include <showlib/StringVector.h>

namespace Ui {
class GetPasswordsWindow;
}

class GetPasswordsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GetPasswordsWindow(ShowLib::StringVector &vec, QWidget *parent = nullptr);
    ~GetPasswordsWindow();

private slots:
    void on_revealCB_stateChanged(int arg1);
    void on_nextBtn_clicked();
    void on_passwordTF_textChanged(const QString &arg1);

private:
    Ui::GetPasswordsWindow *ui;
    ShowLib::StringVector sshFilesNeedingPasswords;
    int fileIndex = 0;

    void setPrompts();
};

