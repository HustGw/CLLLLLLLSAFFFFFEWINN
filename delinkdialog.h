#ifndef DELINKDIALOG_H
#define DELINKDIALOG_H
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>

class DelinkDialog : public QDialog
{
    Q_OBJECT
public:
    DelinkDialog(QWidget* parent= 0);
    QSize sizeHint() const{
        return QSize(300,150);
    }
    QLabel *titleLabel;
    QLineEdit *inputLineEdit;
    QPushButton *okBtn;
    QPushButton *cancelBtn;
private slots:
    void cancel();
    void sendLink();
signals:
    void sendLinkToMain(QString link);
private:
};

#endif // DELINKDIALOG_H
