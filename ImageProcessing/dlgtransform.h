#ifndef DLGTRANSFORM_H
#define DLGTRANSFORM_H

#include <QDialog>

namespace Ui {
class Dlgtransform;
}

class Dlgtransform : public QDialog
{
    Q_OBJECT

public:
    explicit Dlgtransform(QWidget *parent = 0);
    ~Dlgtransform();

private:
    Ui::Dlgtransform *ui;

private slots:
    void on_buttonBox_accepted();

signals:
    void okClicked(const QMatrix& mat);

};

#endif // DLGTRANSFORM_H
