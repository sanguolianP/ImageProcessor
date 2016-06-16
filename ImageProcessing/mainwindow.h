#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QStringList>
#include <QRectF>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QPointF>
#include <QDebug>
#include <QMatrix>
#include <QMessageBox>
#include "mypixmapitem.h"
#include "imageprocesser.h"
#include "threshold.h"
#include "ratiome.h"
#include "ratioc.h"
#include "dlgspatialfilter.h"
#include "dlgtransform.h"
#include "randintensity.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_open_clicked();

    void on_actionSave_triggered();

    void on_actionReset_triggered();

    void on_reset_clicked();

    void on_actionForward_triggered();

    void on_actionBack_triggered();

    void on_forward_clicked();

    void on_back_clicked();

    void on_actionRgb2Gray_triggered();

    void on_actionBw_triggered();

    void on_threshold_okClicked(int bw);

    void on_actionColorReverse_triggered();

    void on_actionContrastStretch_triggered();

    void on_RatioME_okClicked(int m,double e);

    void on_actionLogTransform_triggered();

    void on_RatioC_okClicked(double c);

    void on_actionHisteq_triggered();

    void on_actionExactHisteq_triggered();

    void on_actionZeroPadding_triggered();

    void on_actionDuplicatePadding_triggered();

    void on_actionMirrorPadding_triggered();

    void on_DlgSpatialFilter_okClicked(vector<double> data,int nCols);

    void on_actionMedianFilter_triggered();

    void on_action2D_FT_triggered();

    void on_action2D_IFT_triggered();

    void on_actionAffine_Transform_triggered();

    void on_Dlgtransform_okClicked(const QMatrix& mat);

    void on_actionRelief_triggered();

    void on_actionGaussian_Blur_triggered();

    void on_actionOil_Paint_triggered();

    void on_randIntensity_okClicked(int radius,int intensity);

    void on_actionEdge_Extraction_triggered();

    void on_actionSwell_triggered();

    void on_actionErosion_triggered();

    void on_actionOpen_Operation_triggered();

    void on_actionClose_Operation_triggered();

    void on_swell_okClicked(int t);

    void on_erosion_okClicked(int t);

    void on_open_okClicked(int t);

    void on_close_okClicked(int t);

private:

    Ui::MainWindow *ui;
    QString myFile;
    int Index;
    QGraphicsScene *myGraphicsScene;
    Mypixmapitem *myPixmapItem;
    QFileInfoList list;
    int m;
    QRectF item_rect;
    QImage processImg;

    imageprocesser *imgProcess;
    Threshold *threshold;
    Threshold *threshold1;
    Threshold *threshold2;
    Threshold *threshold3;
    Threshold *threshold4;
    RatioME *me;
    RatioC *c;
    Dlgspatialfilter *mydlgSpatialfilter;
    int spatialFilter;
    Dlgtransform *myDlgTransform;
    RandIntensity *myRandIntensity;


    QString searchFolder(QDir path,QString a);
    QString getPath(int m);
    void showImg(QImage img);
};

#endif // MAINWINDOW_H
