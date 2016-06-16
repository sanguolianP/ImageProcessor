#ifndef THRESHOLD_H
#define THRESHOLD_H

#include <QDialog>

namespace Ui {
class Threshold;
}

class Threshold : public QDialog
{
    Q_OBJECT

public:
    explicit Threshold(QWidget *parent = 0);
    ~Threshold();

private:
    Ui::Threshold *ui;


private slots:
    void on_buttonBox_accepted();
signals:
    void okClicked(int bw);

};

#endif // THRESHOLD_H
