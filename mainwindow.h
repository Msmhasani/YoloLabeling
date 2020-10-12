#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QLabel>
#include <QGraphicsView>
#include <QRubberBand>

#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class Imagery : public QObject//, public QThread
{
    Q_OBJECT

public:
    Imagery(QWidget *parentW, QObject * parent);
    ~Imagery(void);


    QWidget * par;
    QObject * Opar;

    QPoint P_pnt , R_pnt;
    QPixmap the_pix;

    int w_1 , h_1;
    int cls = 0;
    int I_rows = 0 , I_cols = 0;

    float rat_x , rat_y;

    class  ImgInfo
    {
    public:
        QString imgName;
        std::vector<std::vector<float>> rects;
        bool txtFinished = false;
    };

    ImgInfo * imginf;

    cv::Mat cv_img;
    std::vector<std::vector<float>> recv;

    int rec_i = 0;
    int the_i = -1;
    int tmp_i = -1;
    int max_i = 0;
    bool getDir = true;
    bool newBox = false;
    bool saveBoxes = false;
    bool deleteBoxes = false;

    QStringList QSL;
    std::string sqimg , sqtxt , sqimg_1, sqimg_2;

//private slots:
    void getList();

public slots:
    void imagery_loop();

signals:
    void showImg(QPixmap);
    void L3txt(const QString& msg);
    void L2txt(const QString& msg);
    void changeAcceptColor(int r , int g , int b);

};


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *ev);

    QPainter pa;
    QPixmap the_img;
    QRubberBand * rubberBand;
    QPoint origin;

private:
    Ui::MainWindow * ui;
    QPalette pGreen , pWhite , pRed;

    Imagery * imagery;
    QThread *th1;
    QImage  imq;
    QRect qrec;

    QPoint P_pnt , R_pnt;

    int Big_i = 0;

    int wl1 , hl1 , xl1 , yl1;
    int L1x , L1y , L1ex , L1ey;
    float x_rat , y_rat;

private slots:
    void on_B1_clicked();
    void on_B2_clicked();
    void on_B3_clicked();
    void on_B5_clicked();
    void on_B4_clicked();
    void on_B6_clicked();

    void imgshow(QPixmap qimg);

    void L3txt(const QString& msg);
    void L2txt(const QString& msg);
    void changeAcceptColor(int r , int g , int b);

signals:
    void transferPressPnt();
    void transferReleasePnt();

};




#endif // MAINWINDOW_H
