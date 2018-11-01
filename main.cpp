#include "mainwindow.h"
#include <QApplication>
#include "loginview.h"
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include "tcpclient.h"

int main(int argc, char *argv[])
{

    int nExitCode = 0;

    //QApplication a(argc, argv);
//    do{
        QApplication a(argc, argv);
        QTranslator *trans = new QTranslator;
        trans->load(":/new/src/widgets.qm");
        QCoreApplication::installTranslator(trans);
//        TcpClient *L = new TcpClient();
        TcpClient L;
    //判断是否点击登录按钮
        if(L.exec()==QDialog::Accepted){
            //MainWindow *w = new MainWindow();
            //            w->show();
            //             w->FileIsAllowed();

            MainWindow w;
            w.show();
            w.FileIsAllowed();

            nExitCode = a.exec();
            qDebug()<<nExitCode;
            Sleep(2000);

        }
        else{
            return 0;
        }
//    }while(nExitCode != 0);

    return nExitCode;

}
