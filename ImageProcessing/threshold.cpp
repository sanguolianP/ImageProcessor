#include "threshold.h"
#include "ui_threshold.h"

Threshold::Threshold(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Threshold)
{
    ui->setupUi(this);
}

Threshold::~Threshold()
{
    delete ui;
}

void Threshold::on_buttonBox_accepted()
{
    bool ok;
    int bw=ui->bwEdit->text().toInt(&ok);
    if(!ok) bw=128;
    emit okClicked(bw);
}
