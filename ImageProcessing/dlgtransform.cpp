#include "dlgtransform.h"
#include "ui_dlgtransform.h"

#include <iostream>
using namespace std;

Dlgtransform::Dlgtransform(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dlgtransform)
{
    ui->setupUi(this);
}

Dlgtransform::~Dlgtransform()
{
    delete ui;
}

void Dlgtransform::on_buttonBox_accepted()
{
    QMatrix tform;
    bool ok;
    qreal m11,m12,m21,m22,dx,dy;
    m11=ui->lineEdit_10->text().toDouble(&ok);
    if(!ok) m11=1;
    m12=ui->lineEdit_11->text().toDouble(&ok);
    if(!ok) m12=0;
    m21=ui->lineEdit_13->text().toDouble(&ok);
    if(!ok) m21=0;
    m22=ui->lineEdit_14->text().toDouble(&ok);
    if(!ok) m22=1;
    dx=ui->lineEdit_16->text().toDouble(&ok);
    if(!ok) dx=0;
    dy=ui->lineEdit_17->text().toDouble(&ok);
    if(!ok) dy=0;
    tform.setMatrix(m11,m12,m21,m22,dx,dy);
    emit okClicked(tform);

    cout<<m11<<" ";
    cout<<m12<<" ";
    cout<<m21<<" ";
    cout<<m22<<" ";
    cout<<endl;
}
