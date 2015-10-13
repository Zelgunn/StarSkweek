#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    qsrand(QTime::currentTime().msec());

//    QImage image("C:/Users/degva_000/Documents/C++/build-SSkweek_Alpha-Desktop_Qt_5_5_0_MinGW_32bit-Debug/debug/images/deathstar.png");
//    QColor c;
//    for(int i = 0; i < image.width(); i++)
//    {
//        for(int j = 0; j < image.height(); j++)
//        {
//            c = QColor(image.pixel(i,j));
//            if(qGray(c.rgb()) > 200)
//                c.setAlpha(0);
//            else
//                c.setAlpha(120);
//            image.setPixel(i,j,c.rgba());
//        }
//    }

//    image.save("TEST.png");

    return a.exec();
}
