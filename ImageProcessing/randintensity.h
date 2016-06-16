#ifndef RANDINTENSITY_H
#define RANDINTENSITY_H

#include <QDialog>

namespace Ui {
class RandIntensity;
}

class RandIntensity : public QDialog
{
    Q_OBJECT

public:
    explicit RandIntensity(QWidget *parent = 0);
    ~RandIntensity();

private:
    Ui::RandIntensity *ui;

signals:
    void okClicked(int radius,int intensity);
private slots:
    void on_buttonBox_accepted();

};

#endif // RANDINTENSITY_H
