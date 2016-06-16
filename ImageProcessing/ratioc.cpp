#include "ratioc.h"
#include "ui_ratioc.h"
#include <QDebug>

RatioC::RatioC(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RatioC)
{
    ui->setupUi(this);
}

RatioC::~RatioC()
{
    delete ui;
}
void RatioC::on_buttonBox_accepted()
{
    bool ok;
    double C=ui->cEdit->text().toDouble(&ok);
    if(!ok) C=20;
    emit okClicked(C);
    qDebug()<<C<<endl;
}
