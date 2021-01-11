#include "imageviewer.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageViewer w;
    w.setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,w.size(),QGuiApplication::primaryScreen()->availableGeometry()));
    w.show();
    return a.exec();
}