#include "groupsenddialog.h"
#include "ui_groupsenddialog.h"
#include "mainwindow.h"
extern QStringList m_fontList;
extern bool groupSendDialogFlag;
groupSendDialog::groupSendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::groupSendDialog)
{
    QFont f_1;
    //f_1.setFamily(m_fontList.at(0));
    f_1.setPixelSize(14);
    f_1.setWeight(QFont::Normal);
    QFont f_2;
    //f_2.setFamily(m_fontList.at(0));
    f_2.setPixelSize(14);
    f_2.setWeight(QFont::Bold);
    f_2.setFamily("微软雅黑");
    f_1.setFamily("微软雅黑");
    groupSendDialogFlag = false;
    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    db1 = ConnectionPool::openConnection();
    ui->setupUi(this);
    QSqlQuery query(db1);
    ui->label_3->setStyleSheet("QLabel{border:1px solid rgb(207,207,208);}");
    ui->label_7->setStyleSheet("QLabel{border:1px solid rgb(207,207,208);border-bottom:none;}");
    //ui->label_2->setFont(f_1);
    ui->label_8->setFont(f_1);
    ui->label_3->setFont(f_1);
    ui->label_7->setFont(f_1);
    ui->label->setFont(f_1);
    ui->label_6->setFont(f_2);

    ui->listWidget->setStyleSheet("QListWidget {border-top:none;"
                                  "border-bottom:1px solid rgb(207,207,208);"
                                  "border-left:1px solid rgb(207,207,208);"
                                  "border-right:1px solid rgb(207,207,208);}"
                                  "QListWidget::Item {border:none;padding-left:0px}");
    ui->listWidget_2->setStyleSheet("QListWidget {border-top:none;"
                                    "border-bottom:1px solid rgb(207,207,208);"
                                    "border-left:1px solid rgb(207,207,208);"
                                    "border-right:1px solid rgb(207,207,208);}"
                                    "QListWidget::Item {border:none;padding-left:20px}"
                                    "QListWidget::item:selected{background-color:#f6f7fa;border:1px solid #dae4ff;color:#3D3939}");
    ui->pushButton_close->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_close2->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_trans->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_close2->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/delete_button.png);background-color: #EEF0F5;}QPushButton:hover{border-image:url(:/new/mainwindow/pictures/delete_button_hover.png);background-color: #EEF0F5;}");
    ui->pushButton_close->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
    ui->pushButton_trans->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/trans_huge_btn.png);}QPushButton:hover{border-image:url(:/new/mainwindow/pictures/trans_huge_btn_hover.png)}");
    QString split_file_id;
    QString fileName_ = "分享文件：";
    QStringList list = file_id_list.split("||");
    QString numberLabel = "等<font color=green>"+QString::number(list.count()-1)+"</font>个文件";
    for(int j = 0;j<list.count();j++){
        split_file_id = list[j];
        QSqlQuery query(db1);
        bool select_file_name = query.exec("select * from varticle where article_id ='"+split_file_id+"'");
        if(!select_file_name){
            qDebug()<<"bug!~!!!!!!";
        }else{
            while(query.next()){
                fileName_ += query.record().value("article_name").toString()+"；";

            }
        }
    }
    fileName_.chop(1);
     QFontMetrics fm(f_1);
     int fontSize = fm.width(fileName_);
    //int fontSize = fontMetrics().width( fileName_ )+50;//获取之前设置的字符串的像素大小
    int t = ui->label->frameSize().width();
    if( fontSize >= 500 ) //与label自身相比较
    {
        QString str = fontMetrics().elidedText(fileName_, Qt::ElideRight, fontSize - (fontSize - 500) - 100,Qt::TextShowMnemonic);//返回一个带有省略号的字符串
        ui->label->setText(str + numberLabel);       //重新设置label上的字符串
        //ui->label->setGeometry(38,59,ui->label->frameSize().width() - (fontSize - ui->label->frameSize().width()),16);
        //ui->label_10->setGeometry(38+ui->label->frameSize().width() - (fontSize - ui->label->frameSize().width()),59,85,16);
       // ui->label_10->hide();
        //ui->label->setGeometry(38,59,700,16);
    }else{
        ui->label->setText(fileName_ + numberLabel);
//        ui->label->setGeometry(38,59,fontSize,16);

    }
    //ui->label->setText(fileName_);

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
            b1->setFont(f_2);
            b1->setCursor(QCursor(Qt::PointingHandCursor));
            b1->setStyleSheet("QCheckBox {color:#3D3939;spacing:-258px;padding-left:258px}"
                                               "QCheckBox::indicator {width: 13px;height: 13px;}"
                                               "QCheckBox::indicator:enabled:unchecked {image: url(:/new/mainwindow/pictures/checkBox.png);}"
                                               "QCheckBox::indicator:enabled:unchecked:hover {image: url(:/new/mainwindow/pictures/checkBoxHover);}"
                                               "QCheckBox::indicator:enabled:unchecked:pressed {image: url(:/new/mainwindow/pictures/checkBoxHover);}"
                                               "QCheckBox::indicator:enabled:checked {image: url(:/new/mainwindow/pictures/checkBoxChecked);}"
                                               "QCheckBox::indicator:enabled:checked:hover {image: url(:/new/mainwindow/pictures/checkBoxCheckedHover);}"
                                               "QCheckBox::indicator:enabled:checked:hover {image: url(:/new/mainwindow/pictures/checkBoxCheckedHover);}");
            ui->listWidget->addItem(a1);
            ui->listWidget->setItemWidget(a1,b1);
            count++;
            connect(b1,SIGNAL(stateChanged(int)),this,SLOT(onStageChanged(int)));
        }
    }

}

void groupSendDialog::getCheckedItems(){
    QFont f_2;
    //f_2.setFamily(m_fontList.at(0));
    f_2.setFamily("微软雅黑");
    f_2.setPixelSize(14);
    f_2.setWeight(QFont::Bold);
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
        QWidget * widget = ui->listWidget->itemWidget(item);
        QCheckBox * box =(QCheckBox*) widget;
        QString nick_name = box->text();
        isChecked = box ->isChecked();
        box->setStyleSheet("QCheckBox {color:#3D3939;spacing:-258px;padding-left:258px}"
                     "QCheckBox::indicator {width: 13px;height: 13px;}"
                     "QCheckBox::indicator:enabled:unchecked {image: url(:/new/mainwindow/pictures/checkBox.png);}"
                     "QCheckBox::indicator:enabled:unchecked:hover {image: url(:/new/mainwindow/pictures/checkBoxHover);}"
                     "QCheckBox::indicator:enabled:unchecked:pressed {image: url(:/new/mainwindow/pictures/checkBoxHover);}"
                     "QCheckBox::indicator:enabled:checked {image: url(:/new/mainwindow/pictures/checkBoxChecked);}"
                     "QCheckBox::indicator:enabled:checked:hover {image: url(:/new/mainwindow/pictures/checkBoxCheckedHover);}"
                     "QCheckBox::indicator:enabled:checked:hover {image: url(:/new/mainwindow/pictures/checkBoxCheckedHover);}");
        if(isChecked){
            QListWidgetItem *nick_name_item = new QListWidgetItem();
            nick_name_item->setText(nick_name);
            nick_name_item->setSizeHint(QSize(200,40));
            nick_name_item->setFont(f_2);
            ui->listWidget_2->insertItem(i,nick_name_item);
            reciever++;
        }
    }
    ui->label_2->setText(tr("已选择")+"<font color=green>"+QString::number(reciever)+"</font>"+tr("个收件人"));
}
void groupSendDialog::closeEvent(QCloseEvent *){
        file_id_list.clear();
    ConnectionPool::closeConnection(db1);
    groupSendDialogFlag = false;
}

void groupSendDialog::onStageChanged(int stage){
        getCheckedItems();
}

groupSendDialog::~groupSendDialog()
{
    delete ui;
}

void groupSendDialog::on_pushButton_clicked()
{

}

void groupSendDialog::on_pushButton_trans_clicked()
{
    bool select_flag = true;
    QString split_file_id;
    QStringList list = file_id_list.split("||");
    for(int j = 0;j<list.count();j++){
        split_file_id = list[j];
        QString fileName;
        int count = ui->listWidget_2->count();
        if(count == 0){
            select_flag =false;
            break;
        }else{
            select_flag = true;
        }
        qDebug()<<split_file_id;
        QSqlQuery query(db1);
        bool select_file_name = query.exec("select * from varticle where article_id ='"+split_file_id+"'");
        if(!select_file_name){
            qDebug()<<"bug!~!!!!!!";
        }else{
            while(query.next()){
                fileName = query.record().value("article_name").toString();
                QString file_size = query.record().value("article_size").toString();
                for(int i = 0;i<count;i++){
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
                    query.bindValue(1,split_file_id);
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
        }
    }
    if(select_flag == true){
        MsgBox *msgbox = new MsgBox(4,QStringLiteral("传输成功！"),this);
        msgbox->exec();
        file_id_list.clear();
        groupSendDialogFlag = false;
        this->close();
    }else {
        MsgBox *msgbox = new MsgBox(2,QStringLiteral("请选择需要传输的好友！"),this);
        msgbox->exec();
    }
}

void groupSendDialog::on_pushButton_close2_clicked()
{
    file_id_list.clear();
    groupSendDialogFlag = false;
    this->close();
}

void groupSendDialog::on_pushButton_close_clicked()
{
    file_id_list.clear();
    groupSendDialogFlag = false;
    this->close();
}
void groupSendDialog::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(5, 5, this->width()-10, this->height()-10);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(128, 128, 128, 50);
    for(int i=0; i<5; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(5-i, 5-i, this->width()-(5-i)*2, this->height()-(5-i)*2);
        color.setAlpha(120 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}
void groupSendDialog::mousePressEvent(QMouseEvent *qevent)
{
    if(qevent->button() == Qt::LeftButton)
    {
        mouse_press = true;
        //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
        move_point = qevent->pos();;
    }
}
void groupSendDialog::mouseMoveEvent(QMouseEvent *qevent)
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
void groupSendDialog::mouseReleaseEvent(QMouseEvent *qevent)
{
    //设置鼠标为未被按下
    mouse_press = false;
}
