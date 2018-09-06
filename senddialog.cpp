#include "senddialog.h"
#include "ui_senddialog.h"
#include "mainwindow.h"
#include "finishencryptionitem.h"
extern QString file_id;
extern QString User_ID;

sendDialog::sendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sendDialog)
{
    db1 = ConnectionPool::openConnection();

    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    ui->setupUi(this);

    ui->label_3->setStyleSheet("QLabel{border:1px solid gray;}");
    ui->label_7->setStyleSheet("QLabel{border:1px solid gray;}");
    ui->pushButton_close->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_close2->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_trans->setCursor(QCursor(Qt::PointingHandCursor));

    ui->pushButton_close2->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/delete_button.png);background-color: #EEF0F5;}QPushButton:hover{border-image:url(:/new/mainwindow/pictures/delete_button_hover.png);background-color: #EEF0F5;}");
    ui->pushButton_close->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");
    ui->pushButton_trans->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);background-color:#3A8CFF;color:white;}QPushButton:hover{background-color: rgb(119,146,183);color:black;}QPushButton:pressed{background-color: #3A8CFF;color:white;}");
    QSqlQuery query(db1);
    //QListWidget *friendList = new QListWidget(ui->listWidget);
    //friendList->setGeometry(0,0,251,351);
    //ui->listWidget->setGeometry(0,0,251,351);
    bool friendSelSuc = query.exec("select * from friend where user_id ='"+User_ID+"' and status = 1");
    if(!friendSelSuc){
        qDebug()<<"查询好友失败";
        return;
    }
    else{
        qDebug()<<"查询好友成功";
        //将查询的好友插入视图中
        int count = 0;
        while(query.next()){
            QString nickName = query.record().value("friend_nickname").toString();
            QCheckBox * b1 = new QCheckBox(nickName);
            QListWidgetItem* a1 = new QListWidgetItem();
            a1->setSizeHint(QSize(200,40));
            b1->setFont(QFont("Timers",12,QFont::Bold));
            b1->setCursor(QCursor(Qt::PointingHandCursor));
            ui->listWidget->addItem(a1);
            ui->listWidget->setItemWidget(a1,b1);
            count++;
            connect(b1,SIGNAL(stateChanged(int)),this,SLOT(onStageChanged(int)));
        }
    }

}

void sendDialog::getCheckedItems(){
    int count = ui->listWidget->count();
    int reciever = 0;
    qDebug()<<count;
    bool isChecked = false;
    int count2 = ui->listWidget_2->count();
    for (int j = 0;j<count2;j++){
        QListWidgetItem * item = ui->listWidget_2->takeItem(0);
        delete item;
    }

    for (int i = 0;i<count;i++){

        QListWidgetItem * item = ui->listWidget->item(i);
        item->setSizeHint(QSize(200,40));
        QWidget * widget = ui->listWidget->itemWidget(item);
        QCheckBox * box =(QCheckBox*) widget;
        QString nick_name = box->text();

        isChecked = box ->isChecked();
        if(isChecked){
            QListWidgetItem *nick_name_item = new QListWidgetItem();
            nick_name_item->setText(nick_name);
            nick_name_item->setSizeHint(QSize(200,40));
            nick_name_item->setFont(QFont("Timers",12,QFont::Bold));
            ui->listWidget_2->insertItem(i,nick_name_item);
            reciever++;
        }
    }
    ui->label_2->setText(tr("已选择")+"<font color=green>"+QString::number(reciever)+"</font>"+tr("个收件人"));
}

sendDialog::~sendDialog()
{
    delete ui;
}


void sendDialog::closeEvent(QCloseEvent *){
    ConnectionPool::closeConnection(db1);
}

void sendDialog::onStageChanged(int stage){
        getCheckedItems();
}
void sendDialog::on_pushButton_clicked()
{

}

void sendDialog::on_pushButton_2_clicked()
{

}
void sendDialog::reciveData(QString data){
    file_id = data;
}
void sendDialog::reciveUserId(QString data){
    User_ID = data;
}


void sendDialog::on_pushButton_trans_clicked()
{
    QString fileName;
    int count = ui->listWidget_2->count();
    qDebug()<<file_id;
    QSqlQuery query(db1);
    bool select_file_name = query.exec("select * from varticle where article_id ='"+file_id+"'");
    if(!select_file_name){
        qDebug()<<"bug!~!!!!!!";
    }else{
        while(query.next()){
            fileName = query.record().value("article_name").toString();
            for(int i = 0;i<count;i++){
                QString file_size = query.record().value("article_size").toString();
                QDateTime time = QDateTime::currentDateTime();
                QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");
                QListWidgetItem *item = ui->listWidget_2->item(i);
                QString nick_name = item->text();
                QString friend_id;
                QString oemp_phone;
                QString emp_phone;
                query.exec("select * from friend where friend_nickname ='"+nick_name+"'");
                while(query.next()){
                     friend_id= query.record().value("friend_id").toString();
                }
                query.exec("select * from employee where emp_id  = '"+friend_id+"'");
                while(query.next()){
                     oemp_phone = query.record().value("emp_phone").toString();
                }
                query.exec("select * from employee where emp_id  = '"+User_ID+"'");
                while(query.next()){
                     emp_phone = query.record().value("emp_phone").toString();
                }
                query.prepare("insert into Decryption (id,file_id,file_name,emp_id,emp_phone,oemp_id,oemp_phone,status,createtime,file_size,is_solved,apply_time) values (?,?,?,?,?,?,?,0,?,?,0,0)");
                query.bindValue(0,QUuid::createUuid().toString());
                query.bindValue(1,file_id);
                query.bindValue(2,fileName);
                query.bindValue(3,User_ID);
                query.bindValue(4,emp_phone);
                query.bindValue(5,friend_id);
                query.bindValue(6,oemp_phone);
                query.bindValue(7,time_str);
                query.bindValue(8,file_size);
                bool success = query.exec();
                if(success){
                    qDebug()<<"插入待解密表成功";
                }else{
                    qDebug()<<"插入失败";
                }
            }
        }
        MsgBox *msgbox = new MsgBox(4,QStringLiteral("传输成功！"),this);
        msgbox->exec();
        this->close();
    }


}

void sendDialog::on_pushButton_close2_clicked()
{
      this->close();
}

void sendDialog::on_pushButton_close_clicked()
{
      this->close();
}

void sendDialog::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(5, 5, this->width()-10, this->height()-10);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 50);
    for(int i=0; i<5; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(5-i, 5-i, this->width()-(5-i)*2, this->height()-(5-i)*2);
        color.setAlpha(150 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}
void sendDialog::mousePressEvent(QMouseEvent *qevent)
{
    if(qevent->button() == Qt::LeftButton)
    {
        mouse_press = true;
        //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
        move_point = qevent->pos();;
    }
}
void sendDialog::mouseMoveEvent(QMouseEvent *qevent)
{
    //若鼠标左键被按下
    if(mouse_press)
    {
        //鼠标相对于屏幕的位置
        QPoint move_pos = qevent->globalPos();

        //移动主窗体位置
        this->move(move_pos - move_point);
    }
}
void sendDialog::mouseReleaseEvent(QMouseEvent *qevent)
{
    //设置鼠标为未被按下
    mouse_press = false;
}
