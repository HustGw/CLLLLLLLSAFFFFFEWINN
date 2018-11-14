#include "newdownloaddialog.h"
#include "ui_newdownloaddialog.h"
extern QStringList newDownloadFileIdList;
extern QStringList m_fontList;
newDownloadDialog::newDownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newDownloadDialog)
{
    QFont f_1;
    //f_1.setFamily(m_fontList.at(0));
    f_1.setPixelSize(14);
    f_1.setWeight(QFont::Normal);
    QFont f_2;
    //f_2.setFamily(m_fontList.at(0));
    f_2.setPixelSize(14);
    f_2.setWeight(QFont::Bold);
    f_1.setFamily("微软雅黑");
    f_2.setFamily("微软雅黑");

    db1 = ConnectionPool::openConnection();
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->setupUi(this);
    ui->label_3->setFont(f_2);
    ui->label_6->setFont(f_1);
    ui->pushButtoncancel->setFont(f_1);
    ui->pushButtonconfirm->setFont(f_1);
    ui->pushButtonconfirm->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
    ui->pushButtonconfirm->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButtonconfirm->setText(QStringLiteral("确认下载"));
    ui->pushButtoncancel->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
    ui->pushButtoncancel->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButtoncancel->setText(QStringLiteral("忽略"));
    ui->pushButton_close->setCursor(QCursor(Qt::PointingHandCursor));
    connect(this,SIGNAL(downloadOne(QString)),parent,SLOT(downloadOneFile(QString)));
}

newDownloadDialog::~newDownloadDialog()
{
    delete ui;
}
void newDownloadDialog::clearlist(){
    int count = ui->listWidget->count();
    for (int j = 0;j<count;j++){
        QListWidgetItem * item = ui->listWidget->takeItem(0);
        delete item;
    }
}
void newDownloadDialog::change_list_view(){
    QFont f_1;
    //f_1.setFamily(m_fontList.at(0));
    f_1.setFamily("微软雅黑");
    f_1.setPixelSize(14);
    f_1.setWeight(QFont::Normal);
    int count = ui->listWidget->count();
    for (int j = 0;j<count;j++){
        QListWidgetItem * item = ui->listWidget->takeItem(0);
        delete item;
    }
    QSqlQuery query(db1);
    QSqlQuery query2(db1);
    for(int i = 0 ; i < newDownloadFileIdList.count() ; i++){
        QString record_id = newDownloadFileIdList[i];
        bool success = query.exec("select * from Decryption where id ='"+record_id+"'");
        if(!success){
            qDebug()<<"查询失败";
            return;
        }
        else{
            qDebug()<<"查询成功";
            while(query.next()){
                QString file_name = query.record().value("file_name").toString();
                QString sender_id = query.record().value("emp_id").toString();
                QString file_time = query.record().value("createtime").toString();
                QString sender_name;
                bool success1 = query2.exec("select * from employee where emp_id ='"+sender_id+"'");
                if(success1){
                    while(query2.next()){
                        sender_name = query2.record().value("emp_name").toString();
                    }
                }else{

                }
                QString the_words = "用户"+sender_name+"向您传输文件"+file_name+"  时间："+file_time.left(file_time.length()-3);
                QFontMetrics fm(f_1);
                int fontSize = fm.width(the_words);
                int fontSizeName = fm.width(file_name);
                QString filetype_extra = file_name.section(".",0,0).mid(file_name.section(".",0,0).length()-2)+"."+file_name.section(".",1,1).trimmed().toStdString().c_str() ;
               //int t = ui->listWidget->frameSize().width();
               if( fontSize >= 451 ) //与label自身相比较
               {
                   QString str = fontMetrics().elidedText(file_name, Qt::ElideRight, fontSizeName - (fontSize - 381),Qt::TextShowMnemonic) + filetype_extra;//返回一个带有省略号的字符串
                   the_words = "用户"+sender_name+"向您传输文件"+str+"  时间："+file_time.left(file_time.length()-3);        //重新设置label上的字符串

               }else{

               }
                QListWidgetItem *file_item = new QListWidgetItem();
                file_item->setText(the_words);
                file_item->setSizeHint(QSize(400,40));
                file_item->setFont(f_1);
                ui->listWidget->insertItem(i,file_item);
            }
        }
    }
}
void newDownloadDialog::on_pushButtonconfirm_clicked()
{
    emit Infor_numChange();
    for(int i = 0 ; i < newDownloadFileIdList.count() ; i++){
        QString record_id = newDownloadFileIdList[i];
        emit downloadOne(record_id);
    }    
    newDownloadFileIdList.clear();
    ConnectionPool::closeConnection(db1);
    this->close();
}

void newDownloadDialog::on_pushButtoncancel_clicked()
{
    emit Infor_numChange();
    isOpened = false;
    newDownloadFileIdList.clear();
    ConnectionPool::closeConnection(db1);
    this->close();
}

void newDownloadDialog::on_pushButton_close_clicked()
{
    emit Infor_numChange();
    isOpened = false;
    newDownloadFileIdList.clear();
    ConnectionPool::closeConnection(db1);
    this->close();
}
void newDownloadDialog::closeEvent(QCloseEvent *qevent)
{
    emit Infor_numChange();
    isOpened = false;
    newDownloadFileIdList.clear();
    ConnectionPool::closeConnection(db1);
}
void newDownloadDialog::mousePressEvent(QMouseEvent *qevent)
{
    if(qevent->button() == Qt::LeftButton)
    {
        mouse_press = true;
        //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
        move_point = qevent->pos();;
    }
}
void newDownloadDialog::mouseMoveEvent(QMouseEvent *qevent)
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
void newDownloadDialog::mouseReleaseEvent(QMouseEvent *qevent)
{
    //设置鼠标为未被按下
    mouse_press = false;
}
void newDownloadDialog::paintEvent(QPaintEvent *event)
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
