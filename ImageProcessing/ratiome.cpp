#include "ratiome.h"
#include "ui_ratiome.h"
#include <QDebug>

RatioME::RatioME(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RatioME)
{
    ui->setupUi(this);
}

RatioME::~RatioME()
{
    delete ui;
}

void RatioME::on_buttonBox_accepted()
{
    bool ok;
    double E=ui->eEdit->text().toDouble(&ok);
    if(!ok) E=2.0;
    int M=ui->mEdit->text().toInt(&ok);
    if(!ok) M=128;
    emit okClicked(M,E);
    qDebug()<<"M="<<M<<"   "<<"E="<<E<<endl;
}
