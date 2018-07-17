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
        return QSize(300,300);
    }
private slots:
private:
    QLabel *titleLabel;
    QLineEdit *inputLineEdit;
    QPushButton *okBtn;
    QPushButton *cancelBtn;
};

#endif // FRIENDINPUTDLG_H
