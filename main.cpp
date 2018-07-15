#include "mainwindow.h"
#include <QApplication>
#include "loginview.h"
#include <QtSql/QSqlDatabase>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont f("ZYSong18030",12,75);
   // a.setFont(f);
    MainWindow w;
    LoginView dlg;
    //判断是否点击登录按钮
    if(dlg.exec()==QDialog::Accepted){
        w.show();
        return a.exec();
    }

    else return 0;

}
