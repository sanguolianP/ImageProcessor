#include "randintensity.h"
#include "ui_randintensity.h"

RandIntensity::RandIntensity(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RandIntensity)
{
    ui->setupUi(this);
}

RandIntensity::~RandIntensity()
{
    delete ui;
}

void RandIntensity::on_buttonBox_accepted()
{
    bool ok;
    int radius=ui->radius->text().toInt(&ok);
    if(!ok) radius=3;
    int intensity=ui->intensity->text().toInt(&ok);
    if(!ok) intensity=20;
    emit okClicked(radius,intensity);
}
