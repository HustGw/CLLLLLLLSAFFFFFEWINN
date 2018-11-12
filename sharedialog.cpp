#include "sharedialog.h"
#include "ui_sharedialog.h"
#include "finishencryptionitem.h"
#include "QrCode.hpp"
#include "msgbox.h"
extern bool shareDialogFlag;
extern QString file_id;
extern QStringList m_fontList;
shareDialog::shareDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::shareDialog)
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
    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->setupUi(this);
    QString fake_address = "&&||"+file_id;
    ui->label->setFont(f_1);
    ui->label_3->setFont(f_1);
    ui->label_8->setFont(f_1);
    ui->label_6->setFont(f_2);
    ui->pushButtonclos->setFont(f_1);
    ui->pushButton_copy->setFont(f_1);
    ui->pushButton_sav->setFont(f_1);
    ui->pushButtonclos->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
    ui->pushButton_copy->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
    ui->pushButton_sav->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
    ui->pushButtonclos2->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/delete_button.png);background-color: #EEF0F5;}QPushButton:hover{border-image:url(:/new/mainwindow/pictures/delete_button_hover.png);background-color: #EEF0F5;}");

    ui->pushButtonclos->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButtonclos2->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_copy->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_sav->setCursor(QCursor(Qt::PointingHandCursor));

    ui->textEdit->setPlainText(fake_address);
    ui->textEdit->setReadOnly(true);
    ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->textEdit->setStyleSheet("QMenu {border: 1px solid black}"
                                        "QMenu::item:selected {background-color: grey;}");
}

shareDialog::~shareDialog()
{
    delete ui;
}

void shareDialog::on_pushButton_clicked()
{
}

void shareDialog::paintEvent(QPaintEvent *event){
    QString link = "&&||"+file_id;
    QPixmap pix(130,130);
    pix.fill(Qt::white);
    QPainter painter1(&pix);
    QSize size(130,130);
    QColor fg = Qt::black;

    paintQR(painter1,size,link,fg);
    painter1.end();

    ui->label_2->setPixmap(pix);

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


void shareDialog::paintQR(QPainter &painter,const QSize sz, const QString &data, QColor fg)
{
    // NOTE: At this point you will use the API to get the encoding and format you want, instead of my hardcoded stuff:
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(data.toUtf8().constData(), qrcodegen::QrCode::Ecc::LOW);
    const int s=qr.getSize()>0?qr.getSize():1;
    const double w=sz.width();
    const double h=sz.height();
    const double aspect=w/h;
    const double size=((aspect>1.0)?h:w);
    const double scale=size/(s+2);
    // NOTE: For performance reasons my implementation only draws the foreground parts in supplied color.
    // It expects background to be prepared already (in white or whatever is preferred).
    painter.setPen(Qt::NoPen);
    painter.setBrush(fg);
    for(int y=0; y<s; y++) {
        for(int x=0; x<s; x++) {
            const int color=qr.getModule(x, y);  // 0 for white, 1 for black
            if(0!=color) {
                const double rx1=(x+1)*scale, ry1=(y+1)*scale;
                QRectF r(rx1, ry1, scale, scale);
                painter.drawRects(&r,1);
            }
        }
    }
}

void shareDialog::on_pushButton_2_clicked()
{


}

void shareDialog::on_pushButton_copy_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->textEdit->toPlainText());
    MsgBox *msgbox = new MsgBox(4,QStringLiteral("拷贝成功！"),this);
    ui->label_8->setText("分享链接已复制！");
    msgbox->exec();
}

void shareDialog::on_pushButton_sav_clicked()
{
    QString filename1 = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png)")); //选择路径
    if(filename1!=nullptr){
        QScreen *screen = QGuiApplication::primaryScreen();
        screen->grabWindow(ui->label_2->winId()).save(filename1);
        MsgBox *msgbox = new MsgBox(4,QStringLiteral("保存成功！"),this);
        ui->label_8->setText("二维码已保存！");
        msgbox->exec();
    }else{
    }

}

void shareDialog::on_pushButtonclos_clicked()
{
    shareDialogFlag = false;
    this->close();
}

void shareDialog::on_pushButtonclos2_clicked()
{
    shareDialogFlag = false;
    this->close();
}

void shareDialog::mousePressEvent(QMouseEvent *qevent)
{
    if(qevent->button() == Qt::LeftButton)
    {
        mouse_press = true;
        //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
        move_point = qevent->pos();;
    }
}
void shareDialog::mouseMoveEvent(QMouseEvent *qevent)
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
void shareDialog::mouseReleaseEvent(QMouseEvent *qevent)
{
    //设置鼠标为未被按下
    mouse_press = false;
}

