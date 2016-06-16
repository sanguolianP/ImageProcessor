#include "dlgspatialfilter.h"
#include "ui_dlgspatialfilter.h"

Dlgspatialfilter::Dlgspatialfilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dlgspatialfilter)
{
    ui->setupUi(this);
}

Dlgspatialfilter::~Dlgspatialfilter()
{
    delete ui;
}

void Dlgspatialfilter::on_buttonBox_accepted()
{
    vector<double> data;
    int nCols=-1;
    bool ok;
    QString strMat=ui->filterMatrix_3->toPlainText().trimmed();
    strMat.replace("\n"," ");
    QStringList rows=strMat.split(QRegExp("\\s*;"));
    for(int i=0;i<rows.length();i++)
    {
        if(rows[i].trimmed().isEmpty()) continue;
        if(nCols<0)
        {
            readOneRow(data,rows[i].trimmed(),nCols, ok);
            if(!ok)
            {
                qDebug()<<"parse error!"<<endl;
                return;
            }
        }
        else
        {
            int tmp;
            readOneRow(data,rows[i].trimmed(),tmp,ok);
            if(!ok)
            {
                qDebug()<<"parse error!"<<endl;
                return;
            }
            if(tmp!=nCols)
            {
                qDebug()<<"parse error!"<<endl;
                return;
            }
        }

    }
    emit okClicked(data,nCols);
}

void Dlgspatialfilter::readOneRow(vector<double> &data, QString row, int &nCols, bool &ok)
{
    nCols=0;
    QStringList cols=row.split(QRegExp("\\s+|\\s*,"));
    for(int i=0;i<cols.size();i++)
    {
        if(cols[i].trimmed().isEmpty()) continue;
        double d=cols[i].toDouble(&ok);
        if(ok==false) return;
        data.push_back(d);
        nCols++;
    }
}
