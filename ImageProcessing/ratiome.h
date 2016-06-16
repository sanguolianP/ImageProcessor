#ifndef RATIOME_H
#define RATIOME_H

#include <QDialog>

namespace Ui {
class RatioME;
}

class RatioME : public QDialog
{
    Q_OBJECT

public:
    explicit RatioME(QWidget *parent = 0);
    ~RatioME();

private:
    Ui::RatioME *ui;

private slots:
    void on_buttonBox_accepted();
signals:
    void okClicked(int M,double E);

};

#endif // RATIOME_H
