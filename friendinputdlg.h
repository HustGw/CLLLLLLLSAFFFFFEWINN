#ifndef FRIENDINPUTDLG_H
#define FRIENDINPUTDLG_H
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>

class friendInputDlg : public QDialog
{
    Q_OBJECT
public:
    friendInputDlg(QWidget* parent= 0);
    QSize sizeHint() const{
        return QSize(300,150);
    }
    QLabel *titleLabel;
    QLineEdit *inputLineEdit;
    QPushButton *okBtn;
    QPushButton *cancelBtn;
private slots:
    void sendName();
    void cancel();
signals:
    void sendNameToMain(QString name);

private:
};

#endif // FRIENDINPUTDLG_H
