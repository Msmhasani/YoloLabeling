#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QDir>
#include <QFileDialog>
#include <iostream>
#include <qiterator.h>
#include <fstream>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Big_i = 0;

    ui->setupUi(this);
    ui->L1->setMouseTracking(true);

    std::string sdt = std::string("Version: ") + __DATE__ +  " " + __TIME__;
    ui->L4->setText(QString(sdt.c_str()));

    wl1 = ui->L1->width();
    hl1 = ui->L1->height();

    L1x = ui->L1->geometry().x() + 0;
    L1ex = ui->L1->geometry().right();

    L1y = ui->L1->geometry().y() + 12;
    L1ex = ui->L1->geometry().bottom();

    //  m_globals = new CGlobals();

    pGreen.setColor(QPalette::Base, QColor(1, 120, 255));
    pRed.setColor(QPalette::Base, QColor(255, 50, 50));
    pWhite.setColor(QPalette::Base, QColor(255, 255, 255));

    ui->B1->setStyleSheet("background-color: rgb(150,150,150)");
    ui->B2->setStyleSheet("background-color: rgb(255,70,70)");
    ui->B3->setStyleSheet("background-color: rgb(220,220,60)");
    ui->B4->setStyleSheet("background-color: rgb(210,255,210)");
    ui->B5->setStyleSheet("background-color: rgb(220,220,60)");
    //ui->B6->setStyleSheet("background-color: rgb(70,255,100)");

    imagery = new Imagery(parent,0);
    imagery->w_1 = wl1;
    imagery->h_1 = hl1;

    connect(imagery , SIGNAL(L3txt(QString)) , this , SLOT(L3txt(QString)));
    connect(imagery , SIGNAL(L2txt(QString)) , this , SLOT(L2txt(QString)));
    connect(imagery , SIGNAL(changeAcceptColor(int,int,int)) , this , SLOT(changeAcceptColor(int,int,int)));

    ///=========================================================================

    th1 = new QThread();
    imagery->moveToThread(th1);
    connect(th1, SIGNAL(started()), imagery, SLOT(imagery_loop()));
    connect(imagery , SIGNAL(showImg(QPixmap)) , this , SLOT(imgshow(QPixmap)));
    th1->start();

    ///=======connect(this, SIGNAL(on_B1_clicked()), imagery , SLOT(getList()));

}

void MainWindow::imgshow(QPixmap qimg)
{
    ui->L1->setPixmap(qimg);
}
void MainWindow::changeAcceptColor(int r , int g , int b)
{
    QString qs = "background-color: rgb(" + QString::number(r) +","+ QString::number(g)+","+ QString::number(b)+")";
    ui->B4->setStyleSheet(qs);
}
void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    origin = event->pos();
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

    QPalette pal;
    if (event->buttons() & Qt::LeftButton)
    {
        imagery->cls = 0;
        pal.setBrush(QPalette::Highlight, QBrush(Qt::red));
    }
    else if (event->buttons() & Qt::RightButton)
    {
        imagery->cls = 1;
        pal.setBrush(QPalette::Highlight, QBrush(Qt::green));
    }

    rubberBand->setPalette(pal);

    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();

    int xx = event->pos().x() - L1x - 1;
    if (xx < 0) xx = 0;
    if (xx > L1x + wl1 - 1) xx = wl1 -1;

    int yy = event->pos().y() - L1y - 1;
    if (yy < 0) yy = 0;
    if (yy > L1y + hl1 - 1) yy = hl1 -1;

    imagery->P_pnt.setX(xx);
    imagery->P_pnt.setY(yy);
    imagery->rec_i++;

    std::cout << "Mouse press.." << event->pos().x() <<" "<<event->pos().y()<<"\n\n";
    std::cout << "Mouse press.." << event->pos().x() <<" "<<event->pos().y()<<"\n\n";
}

void MainWindow::paintEvent(QPaintEvent *ev)
{

}

// This getting called when release the mouse outside the label_image
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    rubberBand->hide();
    delete rubberBand;

    int xe = event->pos().x() - L1x;
    if (xe < 0) xe = 0;
    if (xe > L1x + wl1 - 1) xe = wl1 -1;

    int ye = event->pos().y() - L1y ;
    if (ye < 0) ye = 0;
    if (ye > L1y + hl1 - 1) ye = hl1 -1;

    imagery->R_pnt.setX(xe);
    imagery->R_pnt.setY(ye);

    imagery->newBox = true;

    std::cout << "Mouse release.." << event->pos().x() <<" "<<event->pos().y()<<"\n\n";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::L3txt(const QString& msg)
{
    //    ui->L3->setPalette(pWhite);
    ui->L3->setText(msg);
    ui->L3->setAlignment(Qt::AlignCenter);
}
void MainWindow::L2txt(const QString& msg)
{
    //    ui->L3->setPalette(pWhite);
    ui->L2->setText(msg);
    ui->L2->setAlignment(Qt::AlignCenter);
}

void MainWindow::on_B1_clicked()
{
    imagery->getList();
}

void MainWindow::on_B2_clicked()
{
    imagery->deleteBoxes = true;
}

void MainWindow::on_B3_clicked()
{
    if (Big_i > 0) Big_i--;
    if (imagery->the_i > 0)
    {
        imagery->the_i--;
        changeAcceptColor(210 , 255 , 210);
    }
}

void MainWindow::on_B4_clicked()
{
    imagery->saveBoxes = true;
}

void MainWindow::on_B5_clicked()
{
    Big_i++;
    if (imagery->the_i + 1 < imagery->max_i)
    {
        imagery->the_i++;
        changeAcceptColor(210 , 255 , 210);
    }

}

void MainWindow::on_B6_clicked()
{
    QCoreApplication::quit();
}

///=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=------==-=-=
Imagery::Imagery(QWidget *parentW, QObject *parent): QObject(parent)
{
    Opar = parent;
    par = parentW;
    rec_i = 0;
    the_i = -1;
    tmp_i = -1;
    getDir = true;
}

Imagery::~Imagery(void)
{

}

void Imagery::getList()
{
    if ( getDir )
    {
        QString dir = QFileDialog::getExistingDirectory(par,
                                                        tr("Open Directory"),
                                                        "/home",
                                                        QFileDialog::ShowDirsOnly |
                                                        QFileDialog::DontResolveSymlinks);

        QDir directory(dir);
        //// QSL = directory.entryList(QStringList() << "*.jpg" << "*.bmp", QDir::Files);

        getDir = false;

//      QStringList FFF = directory.entryList(QStringList() << "*.jpg" << "*.bmp"<<"*.png", QDir::Files);
        QStringList FFF = directory.entryList(QStringList() << "*.bmp", QDir::Files);

        QStringList FF;

        ////foreach(QString filename, FFF)

        for(int i = 0; i < FFF.size(); i++)
        {
            std::string sf = FFF[i].toUtf8().constData();
            std::string nf = sf.substr(0,sf.size()-3);
            std::string tf = std::string(dir.toUtf8().constData()) + "/"+ nf + "txt";

            QString path = tf.c_str();

            if(QFileInfo(path).exists()){} // && !QDir(path).exists()) {}
            else QSL.push_back(FFF[i]);
        }

        std::cout << std::endl;

        imginf = new ImgInfo[QSL.size()];

        for(int i = 0; i < QSL.size(); i++)
        {
            imginf[i].imgName = dir+ "/"+ QSL[i];
            imginf[i].txtFinished = false;
        }

        QString qs = "/    " + QString::number(QSL.size());
        L3txt(qs);

        max_i = QSL.size();

        if (QSL.size()) the_i++;
        else getDir = true;
    }
}

void Imagery::imagery_loop()
{
    while (true)
    {
        if (the_i != tmp_i)
        {
            tmp_i = the_i;

            recv.clear();

            L2txt(QString::number(the_i+1));

            //QImage * imag;
            //imag = new QImage( imginf[the_i].imgName);
            //bool ch = imag.load( imginf[the_i].imgName );
            //QPixmap impix(imginf[the_i].imgName);
            //QPixmap impix = QPixmap::fromImage(*imag);

            sqimg = imginf[the_i].imgName.toUtf8().constData();
            std::string nf = sqimg.substr(0,sqimg.size()-3);
            sqtxt = nf + "txt";
            nf = sqimg.substr(0,sqimg.size() - 4); sqimg_1 = nf+"_Rected1.jpg";
            nf = sqimg.substr(0,sqimg.size()-3);  sqimg_2 = nf+"_Rected2.jpg";

            cv::Mat c_img = cv::imread(sqimg,1);
            cv::Mat ccimg;

            I_rows = c_img.rows;
            I_cols = c_img.cols;

            resize(c_img , cv_img , cv::Size(w_1, h_1));
            cv_img.copyTo(ccimg);

            cv::normalize(cv_img,cv_img,0,255,cv::NORM_MINMAX);

            //cv::imshow("1",im);cv::waitKey(0);
            QImage imq;
            if (cv_img.channels() == 1)
            {
                imq = QImage((uchar*)cv_img.data, cv_img.cols , cv_img.rows, int(cv_img.cols), QImage::Format_Indexed8);
            }
            else if (cv_img.channels() == 3)
            {
                imq = QImage((uchar*)cv_img.data, cv_img.cols , cv_img.rows, int(cv_img.cols * cv_img.channels()), QImage::Format_RGB888);
                imq = imq.rgbSwapped();
            }
            else continue;

            the_pix = QPixmap::fromImage(imq);
            the_pix.scaled(w_1, h_1, Qt::IgnoreAspectRatio);
            showImg(the_pix);

            while (tmp_i == the_i)
            {
                if (newBox)
                {
                    newBox=false;

                    int r_x , r_y , r_w , r_h;

                    if (P_pnt.x() < R_pnt.x() - 10)
                    {
                        r_x = P_pnt.x();
                        r_w = R_pnt.x() - r_x + 1;
                    }
                    else if (P_pnt.x() > R_pnt.x() + 10)
                    {
                        r_x = R_pnt.x();
                        r_w = P_pnt.x() - r_x + 1;
                    }
                    else continue;

                    if (P_pnt.y() < R_pnt.y() - 10)
                    {
                        r_y = P_pnt.y();
                        r_h = R_pnt.y() - r_y + 1;
                    }
                    else if (P_pnt.y() > R_pnt.y() + 10)
                    {
                        r_y = R_pnt.y();
                        r_h = P_pnt.y() - r_y + 1;
                    }
                    else continue;

                    //                    rat_x = c_img.cols / float(w_1);   /// Real Box
                    //                    rat_y = c_img.rows / float(h_1);   /// Real Box

                    rat_x = 1.0 / float(w_1);   /// YOLO Box
                    rat_y = 1.0 / float(h_1);   /// YOLO Box

                    float centx = (r_x + float(r_w) / 2.0)*(rat_x);// * c_img.cols);
                    float centy = (r_y + float(r_h) / 2.0)*(rat_y);// * c_img.rows);

                    std::vector<float> rrc;
                    rrc.push_back(cls);
                    rrc.push_back(centx);
                    rrc.push_back(centy);
                    rrc.push_back(r_w*rat_x); if(rrc[3] > 0.9999 ) rrc[3] = 0.9999;
                    rrc.push_back(r_h*rat_y); if(rrc[4] > 0.9999 ) rrc[4] = 0.9999;
                    recv.push_back(rrc);

                    cv::normalize(ccimg, ccimg,0,255,cv::NORM_MINMAX);

                    cv::rectangle(ccimg , cv::Rect(r_x , r_y , r_w , r_h) , cv::Scalar(0 , 255*cls , 255*(1-cls)) , 2);

                    if (ccimg.channels() == 1)
                    {
                        imq = QImage((uchar*)ccimg.data, ccimg.cols , ccimg.rows, int(ccimg.cols), QImage::Format_Indexed8);
                    }
                    else if (ccimg.channels() == 3)
                    {
                        imq = QImage((uchar*)ccimg.data, ccimg.cols , ccimg.rows, int(ccimg.cols * ccimg.channels()), QImage::Format_RGB888);
                        imq = imq.rgbSwapped();
                    }
                    the_pix = QPixmap::fromImage(imq);
                    the_pix.scaled(w_1, h_1, Qt::IgnoreAspectRatio);
                    showImg(the_pix);
                }
                //else QThread::msleep(1);
                if (deleteBoxes)
                {
                    deleteBoxes=false;
                    changeAcceptColor(210 , 255 , 210);

                    recv.clear();

                    cv_img.copyTo(ccimg);

                    cv::normalize(cv_img, cv_img,0,255,cv::NORM_MINMAX);

                    if (cv_img.channels() == 1)
                    {
                        imq = QImage((uchar*)cv_img.data, cv_img.cols , cv_img.rows, int(cv_img.cols), QImage::Format_Indexed8);
                    }
                    else if (cv_img.channels() == 3)
                    {
                        imq = QImage((uchar*)cv_img.data, cv_img.cols , cv_img.rows, int(cv_img.cols * cv_img.channels()), QImage::Format_RGB888);
                        imq = imq.rgbSwapped();
                    }
                    the_pix = QPixmap::fromImage(imq);
                    the_pix.scaled(w_1, h_1, Qt::IgnoreAspectRatio);
                    showImg(the_pix);
                }

                if(saveBoxes)
                {
                    saveBoxes=false;
                    std::ofstream ofss;
                    ofss.open(sqtxt.c_str());
                    for (int j=0;j<recv.size();j++)
                    {
                        for (int k =0;k<recv[j].size();k++)
                        {
                            ofss<<recv[j][k];
                            if (k < recv[j].size() - 1) ofss<<" ";
                            else ofss<<"\n";
                        }
                    }

                    cv::Mat Rimg;

                    cv::resize(ccimg,Rimg,cv::Size(I_cols, I_rows));

                    imwrite(sqimg_1 , Rimg);
//                  imwrite(sqimg_2 , ccimg);
                    changeAcceptColor(10 , 255 , 10);
                }
            }

        }
        else QThread::msleep(1);
    }
}

