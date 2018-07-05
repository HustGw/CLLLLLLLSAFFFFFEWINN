#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QDialog>

namespace Ui {
class LoginView;
}

class LoginView : public QDialog
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = 0);
    ~LoginView();

private slots:

    void on_LoginBtn_clicked();

private:
    Ui::LoginView *ui;
};

#endif // LOGINVIEW_H
