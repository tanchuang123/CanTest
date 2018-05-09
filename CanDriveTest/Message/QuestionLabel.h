#ifndef QUESTIONLABEL_H
#define QUESTIONLABEL_H

#include <QWidget>
#include <Windows.h>
namespace Ui {
class QuestionLabel;
}

class QuestionLabel : public QWidget
{
    Q_OBJECT

public:
    explicit QuestionLabel(QWidget *parent = 0);
    ~QuestionLabel();
    void showQuestion(const QString &text);
private:
    Ui::QuestionLabel *ui;
};

#endif // QUESTIONLABEL_H
