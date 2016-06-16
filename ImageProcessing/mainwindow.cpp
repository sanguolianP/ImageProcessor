#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),threshold(new Threshold(this)),
    threshold1(new Threshold(this)),threshold2(new Threshold(this)),
    threshold3(new Threshold(this)),threshold4(new Threshold(this)),
    me(new RatioME(this)),c(new RatioC(this)),
    mydlgSpatialfilter(new Dlgspatialfilter(this)),
    myDlgTransform(new Dlgtransform(this)),
    myRandIntensity(new RandIntensity(this)),



    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myPixmapItem = NULL;
    myGraphicsScene = new QGraphicsScene(this);
    imgProcess = NULL;
    spatialFilter = 1;

    connect(threshold,SIGNAL(okClicked(int)),this,SLOT(on_threshold_okClicked(int)));
    connect(threshold1,SIGNAL(okClicked(int)),this,SLOT(on_swell_okClicked(int)));
    connect(threshold2,SIGNAL(okClicked(int)),this,SLOT(on_erosion_okClicked(int)));
    connect(threshold3,SIGNAL(okClicked(int)),this,SLOT(on_open_okClicked(int)));
    connect(threshold4,SIGNAL(okClicked(int)),this,SLOT(on_close_okClicked(int)));
    connect(me,SIGNAL(okClicked(int,double)),this,SLOT(on_RatioME_okClicked(int,double)));
    connect(c,SIGNAL(okClicked(double)),this,SLOT(on_RatioC_okClicked(double)));
    connect(mydlgSpatialfilter,SIGNAL(okClicked(vector<double>,int)),this,SLOT(on_DlgSpatialFilter_okClicked(vector<double>,int)));
    connect(myDlgTransform,SIGNAL(okClicked(QMatrix)),this,SLOT(on_Dlgtransform_okClicked(QMatrix)));
    connect(myRandIntensity,SIGNAL(okClicked(int,int)),this,SLOT(on_randIntensity_okClicked(int,int)));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete myGraphicsScene;
    delete threshold;
    delete threshold1;
    delete threshold2;
    delete threshold3;
    delete threshold4;
    delete me;
    delete c;
    delete mydlgSpatialfilter;
    delete myDlgTransform;
    delete myRandIntensity;
}


void MainWindow::on_actionOpen_triggered()
{
    QFileDialog *myFileDialog=new QFileDialog(this);
    myFileDialog->setWindowTitle(tr("Open"));
    myFileDialog->setNameFilter("Image(*.bmp *.jpg *.png *.gif)");
    myFileDialog->setFileMode(QFileDialog::ExistingFile);
    if(myFileDialog->exec())
    {
        QStringList files=myFileDialog->selectedFiles();
        QDir d=myFileDialog->directory();
        myFile=searchFolder(d,files[0]);
        processImg=QImage(myFile);
        showImg(processImg);
    }
}

void MainWindow::on_open_clicked()
{
    QFileDialog *myFileDialog=new QFileDialog(this);
    myFileDialog->setWindowTitle(tr("Open"));
    myFileDialog->setNameFilter("Image(*.bmp *.jpg *.png)");
    myFileDialog->setFileMode(QFileDialog::ExistingFile);
    if(myFileDialog->exec())
    {
        QStringList files=myFileDialog->selectedFiles();
        QDir d=myFileDialog->directory();
        myFile=searchFolder(d,files[0]);
        processImg=QImage(myFile);
        showImg(processImg);
    }
}

QString MainWindow::searchFolder(QDir path, QString a)
{
    path.setFilter(QDir::Files|QDir::Hidden|QDir::NoSymLinks);
    path.setSorting(QDir::Name);
    QStringList filters;
    filters<<"*.jpg"<<"*.png"<<"*.bmp";
    path.setNameFilters(filters);
    list=path.entryInfoList();
    QString name=NULL;
    for(int i=0;i<list.size();i++)
    {
        if(getPath(i)==a)
        {
            name=getPath(i);
            m=i;
            break;
        }
    }
    return name;
}

QString MainWindow::getPath(int m)
{
    QFileInfo pathInfo=list.at(m);
    QString path=pathInfo.absoluteFilePath();
    return path;
}

void MainWindow::showImg(QImage img)
{
    if(myPixmapItem)
    {
        delete myPixmapItem;
        myPixmapItem=NULL;
    }
    if(imgProcess)
    {
        delete imgProcess;
        imgProcess=NULL;
    }
    myPixmapItem = new Mypixmapitem();
    myPixmapItem->setPixmap(QPixmap::fromImage(img));
    item_rect=myPixmapItem->boundingRect();
    myGraphicsScene->addItem(myPixmapItem);
    ui->show->setScene(myGraphicsScene);
    imgProcess=new imageprocesser(img);
//    on_actionReSet_triggered();
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName=QFileDialog::getSaveFileName(this,tr("Save Image"),"/",tr("Image files(*.bmp;;*.jpg;;*.png;;*.tif;;*.GIF;;All Files(*.*)"));
    if(fileName.isNull())
    {
        return;
    }
    else
    {
        if(!processImg.save(fileName))
        {
            QMessageBox::information(this,tr("Save Failed"),tr("Save Failed!"));
        }
        else
            QMessageBox::information(this,tr("Save Succeed"),tr("Save Succeed!"));
    }
}

void MainWindow::on_actionReset_triggered()
{
    myPixmapItem->reset();
    myGraphicsScene->setSceneRect(0,0,item_rect.width(),item_rect.height());
    ui->show->fitInView(item_rect,Qt::KeepAspectRatio);
    ui->show->centerOn(myPixmapItem);
}

void MainWindow::on_reset_clicked()
{
    myPixmapItem->reset();
    myGraphicsScene->setSceneRect(0,0,item_rect.width(),item_rect.height());
    ui->show->fitInView(item_rect,Qt::KeepAspectRatio);
    ui->show->centerOn(myPixmapItem);
}

void MainWindow::on_actionForward_triggered()
{
    if(m<list.length()-1)
        m++;
    else
        m=0;
    myFile=getPath(m);
    processImg=QImage(myFile);
    showImg(processImg);
}

void MainWindow::on_actionBack_triggered()
{
    if(m>0)
        m--;
    else
        m=list.length()-1;
    myFile=getPath(m);
    processImg=QImage(myFile);
    showImg(processImg);
}

void MainWindow::on_forward_clicked()
{
    if(m<list.length()-1)
        m++;
    else
        m=0;
    myFile=getPath(m);
    processImg=QImage(myFile);
    showImg(processImg);
}

void MainWindow::on_back_clicked()
{
    if(m>0)
        m--;
    else
        m=list.length()-1;
    myFile=getPath(m);
    processImg=QImage(myFile);
    showImg(processImg);
}

void MainWindow::on_actionRgb2Gray_triggered()
{
    processImg=imgProcess->rgb2Gray();
    showImg(processImg);
}

void MainWindow::on_actionBw_triggered()
{
    threshold->exec();
}

void MainWindow::on_threshold_okClicked(int bw)
{
    processImg=imgProcess->rgb2Bw(bw);
    showImg(processImg);
}

void MainWindow::on_actionColorReverse_triggered()
{
    processImg=imgProcess->colorReverse();
    showImg(processImg);
}

void MainWindow::on_actionContrastStretch_triggered()
{
    me->exec();
}

void MainWindow::on_RatioME_okClicked(int m,double e)
{
    processImg=imgProcess->contrastStretch(m,e);
    showImg(processImg);
}

void MainWindow::on_actionLogTransform_triggered()
{
    c->exec();
}

void MainWindow::on_RatioC_okClicked(double c)
{
    processImg=imgProcess->logTransform(c);
    showImg(processImg);
}

void MainWindow::on_actionHisteq_triggered()
{
    processImg=imgProcess->histeq();
    showImg(processImg);
}

void MainWindow::on_actionExactHisteq_triggered()
{
    processImg=imgProcess->exactHisteq();
    showImg(processImg);
}

void MainWindow::on_actionZeroPadding_triggered()
{
    mydlgSpatialfilter->exec();
    spatialFilter=1;
}

void MainWindow::on_actionDuplicatePadding_triggered()
{
    mydlgSpatialfilter->exec();
    spatialFilter=2;
}

void MainWindow::on_actionMirrorPadding_triggered()
{
    mydlgSpatialfilter->exec();
    spatialFilter=3;
}

void MainWindow::on_DlgSpatialFilter_okClicked(vector<double> data,int nCols)
{
    if(spatialFilter==1)
        processImg=imgProcess->spatialFilter1(data,nCols);
    if(spatialFilter==2)
        processImg=imgProcess->spatialFilter2(data,nCols);
    if(spatialFilter==3)
        processImg=imgProcess->spatialFilter3(data,nCols);
    showImg(processImg);
}

void MainWindow::on_actionMedianFilter_triggered()
{
    processImg=imgProcess->meadianFilter(3);
    showImg(processImg);
}

void MainWindow::on_action2D_FT_triggered()
{
    processImg=imgProcess->fft2d();
    showImg(processImg);
}

void MainWindow::on_action2D_IFT_triggered()
{
    processImg=imgProcess->ifft2d();
    showImg(processImg);
}

void MainWindow::on_actionAffine_Transform_triggered()
{
    myDlgTransform->exec();
}

void MainWindow::on_Dlgtransform_okClicked(const QMatrix &mat)
{
    processImg=imgProcess->transform(mat);
    showImg(processImg);
}

void MainWindow::on_actionRelief_triggered()
{
    processImg=imgProcess->relief();
    showImg(processImg);
}

void MainWindow::on_actionGaussian_Blur_triggered()
{
    processImg=imgProcess->gaussianBlur();
    showImg(processImg);
}

void MainWindow::on_actionOil_Paint_triggered()
{
    myRandIntensity->exec();
}

void MainWindow::on_randIntensity_okClicked(int radius,int intensity)
{
    processImg=imgProcess->oilPaint(radius,intensity);
    showImg(processImg);
}

void MainWindow::on_actionEdge_Extraction_triggered()
{
    processImg=imgProcess->edgeExtraction();
    showImg(processImg);
}

void MainWindow::on_actionSwell_triggered()
{
    threshold1->exec();
}

void MainWindow::on_swell_okClicked(int t)
{
    processImg=imgProcess->swell(t);
    showImg(processImg);
}

void MainWindow::on_actionErosion_triggered()
{
    threshold2->exec();
}

void MainWindow::on_erosion_okClicked(int t)
{
    processImg=imgProcess->erosion(t);
    showImg(processImg);
}

void MainWindow::on_actionOpen_Operation_triggered()
{
    threshold3->exec();
}

void MainWindow::on_open_okClicked(int t)
{
    processImg=imgProcess->openOperation(t);
    showImg(processImg);
}

void MainWindow::on_actionClose_Operation_triggered()
{
    threshold4->exec();
}

void MainWindow::on_close_okClicked(int t)
{
    processImg=imgProcess->closeOperation(t);
    showImg(processImg);
}

