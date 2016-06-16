#ifndef DLGSPATIALFILTER_H
#define DLGSPATIALFILTER_H

#include <QDialog>
#include <vector>
#include <QDebug>
#include <QRegExp>
#include <QStringList>
using namespace std;

namespace Ui {
class Dlgspatialfilter;
}

class Dlgspatialfilter : public QDialog
{
    Q_OBJECT

public:
    explicit Dlgspatialfilter(QWidget *parent = 0);
    ~Dlgspatialfilter();

private:
    Ui::Dlgspatialfilter *ui;
    void readOneRow(vector<double>& data,QString row,int& nCols,bool& ok);

private slots:
    void on_buttonBox_accepted();

signals:
    void okClicked(vector<double> data,int nCols);
};

#endif // DLGSPATIALFILTER_H
