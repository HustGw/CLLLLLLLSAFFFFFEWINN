#include "sharedialog.h"
#include "ui_sharedialog.h"
#include "finishencryptionitem.h"
#include "QrCode.hpp"

extern QString file_id;

shareDialog::shareDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::shareDialog)
{
    ui->setupUi(this);
    QString fake_address = "https://youxinzhongwang.com/theRea1Fi1eId&&"+file_id;
    ui->textEdit->setPlainText(fake_address);
    ui->textEdit->setReadOnly(true);
}

shareDialog::~shareDialog()
{
    delete ui;
}

void shareDialog::on_pushButton_clicked()
{
}

void shareDialog::paintEvent(QPaintEvent *event){
    QString link = "https://youxinzhongwang.com/theRea1Fi1eId&&"+file_id;
    QPixmap pix(180,180);
    pix.fill(Qt::white);
    QPainter painter(&pix);
    QSize size(180,180);
    QColor fg = Qt::black;

    paintQR(painter,size,link,fg);
    painter.end();

    ui->label_2->setPixmap(pix);

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
    QString filename1 = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png)")); //选择路径
    QScreen *screen = QGuiApplication::primaryScreen();
    screen->grabWindow(ui->label_2->winId()).save(filename1);
    QMessageBox::information(this,"提示","保存成功");

}

void shareDialog::on_pushButton_copy_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->textEdit->toPlainText());
    QMessageBox::information(this,"提示","拷贝成功");
}
