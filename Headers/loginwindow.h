#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#endif // LOGINWINDOW_H

#include <QDialog>

namespace Ui {
    class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent =0);

    ~LoginWindow();

private slots:
    void handleLogin();
    void cancelLogin();

private:
    Ui::LoginWindow *ui;

};


