#ifndef RATIOC_H
#define RATIOC_H

#include <QDialog>

namespace Ui {
class RatioC;
}

class RatioC : public QDialog
{
    Q_OBJECT

public:
    explicit RatioC(QWidget *parent = 0);
    ~RatioC();

private:
    Ui::RatioC *ui;

signals:
    void okClicked(double C);
private slots:
    void on_buttonBox_accepted();

};

#endif // RATIOC_H
