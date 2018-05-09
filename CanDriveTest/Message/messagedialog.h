#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
class messageDialog;
}

class messageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit messageDialog(QWidget *parent = 0);
    ~messageDialog();
    void message_show(QString title="",QString word="");
private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::messageDialog *ui;
};

#endif // MESSAGEDIALOG_H
