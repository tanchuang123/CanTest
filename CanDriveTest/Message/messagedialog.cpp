#include "messagedialog.h"
#include "ui_messagedialog.h"

messageDialog::messageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::messageDialog)
{
    ui->setupUi(this);
}

messageDialog::~messageDialog()
{
    delete ui;
}

void messageDialog::on_pushButton_ok_clicked()
{
    close();
}
void messageDialog::message_show(QString title, QString word)
{
    setWindowTitle(title);
    ui->label_status->setText(word);
    show();

}
