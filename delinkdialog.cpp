#include "delinkdialog.h"
extern QStringList m_fontList;
DelinkDialog::DelinkDialog(QWidget *parent):QDialog(parent)
{
    QFont f_1;
    f_1.setFamily(m_fontList.at(0));
    f_1.setPixelSize(14);
    f_1.setWeight(QFont::Normal);
    QFont f_2;
    f_2.setFamily(m_fontList.at(0));
    f_2.setPixelSize(14);
    f_2.setWeight(QFont::Bold);
    this->resize(462,202);

  //获取主界面的宽度
    int width = this->width();
    setStyleSheet("QWidget{background-color: #FFFFFF;}");
  //初始化为未按下鼠标左键

  //设置标题栏隐藏
    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    titleLabel = new QLabel(this);
    titleLabel->setGeometry(6,6,width-12,38);
    titleText = new QLabel(this);
    titleText->setGeometry(54,19,200,15);
    titleText->setFont(f_2);
    titleText->setStyleSheet("background-color: #EEF0F5;");
    titleIcon = new QLabel(this);
    titleIcon->setGeometry(22,18,23,17);
    titleIcon->setStyleSheet("border-image: url(:/new/mainwindow/pictures/system_icon.png);"
                             "background-color: #EEF0F5;");
    titleText->setText(QStringLiteral("链接下载"));

    titleLabel->setStyleSheet("QLabel{font-family :微软雅黑;font-size : 5em;color : rgb(255,255,255);background-color: #EEF0F5;}");

    closeBtn = new QPushButton(this);
    closeBtn->setGeometry(436, 12, 13, 13);
    closeBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/delete_button.png);background-color: #EEF0F5;}QPushButton:hover{border-image:url(:/new/mainwindow/pictures/delete_button_hover.png);background-color: #EEF0F5;}");
    closeBtn->setCursor(QCursor(Qt::PointingHandCursor));


    askLabel = new QLabel(this);
    askLabel->setGeometry(28,56,150,15);
    askLabel->setText(QStringLiteral("链接地址："));
    askLabel->setFont(f_1);
    okBtn=new QPushButton(this);
    okBtn->setGeometry(228,144,89,27);
    okBtn->setText(QStringLiteral("确认下载"));
    okBtn->setFont(f_1);
    okBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
    okBtn->setCursor(QCursor(Qt::PointingHandCursor));
    cancleBtn=new QPushButton(this);
    cancleBtn->setGeometry(342,144,89,27);
    cancleBtn->setFont(f_1);
    cancleBtn->setText(QStringLiteral("取消下载"));
    cancleBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
    cancleBtn->setCursor(QCursor(Qt::PointingHandCursor));
    inputLineEdit = new QLineEdit(this);
    inputLineEdit->setGeometry(40,80,391,31);
    inputLineEdit->setPlaceholderText(tr("输入链接"));
    inputLineEdit->setFont(f_1);
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(sendLink()));
    connect(cancleBtn,SIGNAL(clicked(bool)),this,SLOT(cancel()));
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(closeBtn_press()));

}

void DelinkDialog::sendLink(){
    QString split_file_id;
        QString link = inputLineEdit->text().section("&&",1,1);
        if(link!=nullptr){
        QStringList list = link.split("||");
        for(int j = 1;j<list.count();j++){
            split_file_id = list[j];
            emit sendLinkToMain(split_file_id);
        }
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText("");
        LinkInsertFlag = 0;
        this->accept();
        }else{
            MsgBox *msgbox = new MsgBox(2,"请输入下载链接",this);
            msgbox->exec();
        }
}
void DelinkDialog::closeEvent(QCloseEvent * event){
    LinkInsertFlag = 0;
}
void DelinkDialog::cancel(){
    LinkInsertFlag = 0;
    this->accept();
}
void DelinkDialog::closeBtn_press()
{
    LinkInsertFlag = 0;
    close();
}
void DelinkDialog::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        mouse_press = true;
            dragPosition = event->globalPos() - frameGeometry().topLeft();
            event->accept();
        }
}
void DelinkDialog::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        if(mouse_press == true){
            move(event->globalPos() - dragPosition);
            event->accept();
        }
        }

}
void DelinkDialog::mouseReleaseEvent(QMouseEvent *event){
    mouse_press = false;
}
void DelinkDialog::paintEvent(QPaintEvent *event)
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


