#include "finishdecryptionItem.h"
extern QStringList m_fontList;

FinishDecryptionItem::FinishDecryptionItem(QWidget *parent): QWidget(parent){

    QFont f_14;
    QFont f_12;
    QFont f_10;
//    f_14.setFamily(m_fontList.at(0));
//    f_12.setFamily(m_fontList.at(0));
//    f_10.setFamily(m_fontList.at(0));
    f_10.setFamily("微软雅黑");
    f_12.setFamily("微软雅黑");
    f_14.setFamily("微软雅黑");
    f_14.setPixelSize(14);
    f_12.setPixelSize(12);
    f_10.setPixelSize(10);
    f_14.setWeight(QFont::Bold);
    f_12.setWeight(QFont::Normal);
    f_10.setWeight(QFont::Normal);
    background = new QLabel(this);
    background->setGeometry(0,0,695,71);
    background->setStyleSheet("border-bottom:1px solid rgb(237,237,237)");
    label = new QLabel(this);
    label->setGeometry(0,0,34,16);
    label->setStyleSheet("QLabel{ border-image:url(:/new/mainwindow/pictures/finde_label.png); }");
    elseLabel = new QLabel(this);
    elseLabel->setGeometry(49,25,30,16);
    elseLabel->setStyleSheet("background-color:#91A7B9;color:white;");
    elseLabel->raise();
    elseLabel->setAlignment(Qt::AlignCenter);
    fileName = new QLabel(this);
    fileName->setStyleSheet("color:#3D3939");
    fileSize = new QLabel(this);
    fileDescription = new QLabel(this);

    fileLevel = new QLabel(this);
    fileLevel->setGeometry(305,42,200,14);
    fileLevel->setFont(f_12);
    fileLevel->setStyleSheet("color:#9999AB");

    timeLabel = new QLabel(this);
    timeLabel->setStyleSheet("background:transparent");
    timeLabel->setGeometry(294,31,230,10);
    timeLabel->setFont(f_10);
    timeLabel->setStyleSheet("color:#9999AB");

    fileName->setFont(f_14);
    fileSize->setFont(f_12);
    fileDescription->setFont(f_12);
    fileSize->setStyleSheet("color:#9999AB");
    fileDescription->setStyleSheet("color:#9999AB");

    openBtn = new QPushButton(this);
    pathOpenBtn = new QPushButton(this);
    deleteBtn = new QPushButton(this);

    fileIcon = new QLabel(this);
    checkBox = new QCheckBox(this);
    checkBox->setCursor(QCursor(Qt::PointingHandCursor));

    //location
    openBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/open_button.png); }"
                                       "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/open_button_hover.png); }");
    pathOpenBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/pathopen_button.png); }"
                                       "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/pathopen_button_hover.png); }");
    deleteBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/delete_button.png); }"
                                       "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/delete_button_hover.png); }");

    openBtn->setToolTip("打开文件");
    pathOpenBtn->setToolTip("打开路径");
    deleteBtn->setToolTip("删除条目");

    openBtn->setCursor(QCursor(Qt::PointingHandCursor));
    pathOpenBtn->setCursor(QCursor(Qt::PointingHandCursor));
    deleteBtn->setCursor(QCursor(Qt::PointingHandCursor));

    fileName->setGeometry(97,9,350,18);
    fileSize->setGeometry(97,29,300,12);
    fileDescription->setGeometry(97,42,200,14);

    openBtn->setGeometry(590,10,14,16);
    pathOpenBtn->setGeometry(625,11,15,14);
    deleteBtn->setGeometry(664,12,13,13);

    checkBox->setGeometry(15,25,13,13);
    checkBox->setStyleSheet("QCheckBox {color: black;}"
                            "QCheckBox::indicator {width: 13px;height: 13px;}"
                            "QCheckBox::indicator:enabled:unchecked {image: url(:/new/mainwindow/pictures/checkBox.png);}"
                            "QCheckBox::indicator:enabled:unchecked:hover {image: url(:/new/mainwindow/pictures/checkBoxHover);}"
                            "QCheckBox::indicator:enabled:unchecked:pressed {image: url(:/new/mainwindow/pictures/checkBoxHover);}"
                            "QCheckBox::indicator:enabled:checked {image: url(:/new/mainwindow/pictures/checkBoxChecked);}"
                            "QCheckBox::indicator:enabled:checked:hover {image: url(:/new/mainwindow/pictures/checkBoxCheckedHover);}"
                            "QCheckBox::indicator:enabled:checked:hover {image: url(:/new/mainwindow/pictures/checkBoxCheckedHover);}");

    fileIcon->setGeometry(45,10,38,46);
    connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));
}
void FinishDecryptionItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QColor(237,237,237));
    painter.drawLine(0,this->height()-1,this->width()-1,this->height()-1);
}
void FinishDecryptionItem::changeBackground(){
    background->setStyleSheet("background-color:#f6f7fa;border:1px solid #dae4ff");
}
void FinishDecryptionItem::onStateChanged(int state){
    if (state == Qt::Checked) // "选中"
        {
            fileIcon->setStyleSheet("background-color:#f6f7fa;");
            background->setStyleSheet("background-color:#f6f7fa;border:1px solid #dae4ff");
            fileName->setStyleSheet("background-color:#f6f7fa;color:#3D3939");
            label->setStyleSheet("QLabel{ border-image:url(:/new/mainwindow/pictures/finde_label.png);background-color:#f6f7fa; }");
            timeLabel->setStyleSheet("background-color:#f6f7fa;color:#9999AB");
            fileSize->setStyleSheet("color:#9999AB;background-color:#f6f7fa;");
            fileLevel->setStyleSheet("background-color:#f6f7fa;color:#9999AB");
            fileDescription->setStyleSheet("color:#9999AB;background-color:#f6f7fa;");
            openBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/open_button.png);background-color:#f6f7fa; }"
                                               "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/open_button_hover.png);background-color:#f6f7fa; }");
            pathOpenBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/pathopen_button.png);background-color:#f6f7fa; }"
                                               "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/pathopen_button_hover.png);background-color:#f6f7fa; }");
            deleteBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/delete_button.png);background-color:#f6f7fa; }"
                                               "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/delete_button_hover.png);background-color:#f6f7fa; }");
        }
        else // 未选中 - Qt::Unchecked
        {
            fileIcon->setStyleSheet("background-color:white;");
            background->setStyleSheet("background-color:white;border:none;border-bottom:1px solid rgb(237,237,237)");
            fileName->setStyleSheet("background-color:white;color:#3D3939");
            label->setStyleSheet("QLabel{ border-image:url(:/new/mainwindow/pictures/finde_label.png);background-color:white }");
            timeLabel->setStyleSheet("background-color:white;color:#9999AB");
            fileSize->setStyleSheet("color:#9999AB;background-color:white");
            fileDescription->setStyleSheet("color:#9999AB;background-color:white");
            fileLevel->setStyleSheet("color:#9999AB");
            openBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/open_button.png);background-color:white }"
                                               "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/open_button_hover.png);background-color:white }");
            pathOpenBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/pathopen_button.png);background-color:white }"
                                               "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/pathopen_button_hover.png);background-color:white }");
            deleteBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/delete_button.png);background-color:white }"
                                               "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/delete_button_hover.png);background-color:white }");
        }
}

void FinishDecryptionItem::on_openBtn_clicked(){
    QString openPath;
    QString openfile;
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();
    db = ConnectionPool::openConnection();
    QSqlQuery query(db);
       bool success = query.exec("select * from Decryption where file_id='" + name+"'");

       if(!success){
           MsgBox *msgbox = new MsgBox(2,QStringLiteral("未找到路径"),this);
           msgbox->exec();
           return;
       }else{
           while(query.next()){
           openPath = QString("file:///C:/CloundSafe/"+User_qqNum+"/Decrypt/file/") + query.record().value("file_name").toString();
           openfile = QString("C:/CloundSafe/"+User_qqNum+"/Decrypt/file/")+ query.record().value("file_name").toString();
           }
       }
       QFileInfo d_file(openfile);
       if(!d_file.isFile()){
           MsgBox *msgbox = new MsgBox(2,QStringLiteral("未找到文件，请检查文件是否已被删除或移动到\n新的位置。"),this);
           msgbox->exec();
       }else{
           QDesktopServices::openUrl(QUrl(openPath, QUrl::TolerantMode));
       }
    ConnectionPool::closeConnection(db);

}

void FinishDecryptionItem::on_pathOpenBtn_clicked(){
    QString openPath;
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();
    db = ConnectionPool::openConnection();
    QSqlQuery query(db);
       bool success = query.exec("select * from Decryption where file_id='" + name+"'");
       if(!success){
           MsgBox *msgbox = new MsgBox(2,QStringLiteral("未找到路径"),this);
           msgbox->exec();
           return;
       }else{
           while(query.next()){
           openPath = QString("file:///C:/CloundSafe/"+User_qqNum+"/Decrypt/file/");
           }
       }
    QDesktopServices::openUrl(QUrl(openPath, QUrl::TolerantMode));
    ConnectionPool::closeConnection(db);
}

