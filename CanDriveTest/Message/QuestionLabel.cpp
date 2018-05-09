#include "QuestionLabel.h"
#include "ui_QuestionLabel.h"

QuestionLabel::QuestionLabel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuestionLabel)
{
    ui->setupUi(this);
}

QuestionLabel::~QuestionLabel()
{
    delete ui;
}
void QuestionLabel::showQuestion(const QString &text)
{
    ui->label->setText(text);
    show();
    Sleep(1000);
    close();
}
