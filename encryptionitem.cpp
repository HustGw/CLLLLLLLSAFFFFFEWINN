#include "encryptionitem.h"
extern bool initPageFlag;
extern bool initLableFlag;
extern QFileInfo openFileInfo;
extern QStringList m_fontList;
EncryptionItem::EncryptionItem(QWidget *parent) : QWidget(parent)
{
    QFont f_14;
    QFont f_12;
    QFont f_10;
    f_14.setFamily(m_fontList.at(0));
    f_12.setFamily(m_fontList.at(0));
    f_10.setFamily(m_fontList.at(0));
    f_14.setPixelSize(14);
    f_12.setPixelSize(12);
    f_10.setPixelSize(10);
    f_14.setWeight(QFont::Bold);
    f_12.setWeight(QFont::Normal);
    f_10.setWeight(QFont::Normal);

    if (initPageFlag){
        initPage = new QLabel(this);
        QFont font ("冬青黑体简体", 12, 75);
        initPage->setGeometry(241,162,285,30);
        initPage->setStyleSheet("color:#dfdfdf;");
        initPage->setFont(font);
        initPage->setText("请调取本地文件进行加密");
         initLableFlag = true;
        //checkBox->hide();
    }
    else{

        fileName = new QLabel(this);
        fileSize = new QLabel(this);
        fileDescription = new QLabel(this);
        //timeInfo = new QLabel(this);
        line = new QLabel(this);

        fileName->setFont(f_14);
        fileSize->setFont(f_10);
        fileDescription->setFont(f_12);
        fileSize->setStyleSheet("color:#9999AB");
        fileDescription->setStyleSheet("color:#9999AB");
        //timeInfo->setFont(QFont("Timers",8));;
        //timeInfo->setStyleSheet("color:#9999AB");
        line->setStyleSheet("color:rgb(139,139,139)");

        line->setGeometry(0,76,750,1);

        fileDescription->setStyleSheet("background:transparent");
        fileName->setStyleSheet("background:transparent;color:#3D3939");
        fileSize->setStyleSheet("background:transparent");
        //timeInfo->setStyleSheet("background:transparent");

        //downloadBtn = new QPushButton(this);
        fileIcon = new QLabel(this);

        encryptStaBtn = new QPushButton(this);
        starEncptBtn = new QPushButton(this);
        starEncptBtn->setGeometry(10,10,10,12);
        starEncptBtn->hide();

        //设置fileName、fileName fileDescription checkBox fileIcon 的位置
        fileName->setGeometry(95,12,150,16);
        //timeInfo->setGeometry(300,31,200,30 );
        fileSize->setGeometry(95,31,100,12);
        fileDescription->setGeometry(95,48,200,14);
        //downloadBtn->setGeometry(500,50,100,30);

        //checkBox->setGeometry(10,70,20,20);
        fileIcon->setGeometry(50,13,38,46);
        encryptStaBtn->setGeometry(95,47,100,12);
        encryptStaBtn->setFont(QFont("Timers",8));
        encryptStaBtn->setStyleSheet("color:#9999AB");
        encryptStaBtn->setStyleSheet("background:transparent");

        //QDateTime time = QDateTime::currentDateTime();
        //QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");
        //timeInfo->setText(time_str);

        encryptStaBtn->hide();

    }

    progressBar->setGeometry(0,0,695,65);
    QString strQSS = "QProgressBar { \
            text-align: center; \
            border: 1px ; \
            background: transparent; \
        } \
        QProgressBar::chunk { \
            background-color: rgba(230, 237, 255,0.8); \
        }";
    //progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
    progressBar->setOrientation(Qt::Horizontal);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setStyleSheet(strQSS);
    fInfo=openFileInfo;
    QString fName = fInfo.fileName();
    //emit starEcptItem(fName);
    //progressBar->setValue(20);
   // progressBar->setGeometry(450,85,100,20);
//    double dProgress = (progressBar->value() - progressBar->minimum()) * 100.0
//                    / (progressBar->maximum() - progressBar->minimum());
//    progressBar->setFormat(QString::fromLocal8Bit(u8"%1%").arg(QString::number(dProgress, 'f', 1)));
    //progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
}
//每一个Item绘制边界框
void EncryptionItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    if (!initPageFlag){
        painter.setPen(QColor(237,237,237));
        painter.drawLine(0,this->height()-1,this->width()-1,this->height()-1);
    }

//    painter.drawLine(31,0,31,this->height()-1);
//    painter.drawLine(this->width()-1,0,this->width()-1,this->height()-1);
//    painter.drawLine(31,this->height()-1,this->width()-1,this->height()-1);


}
