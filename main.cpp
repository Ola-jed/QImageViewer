#include "imageviewer.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageViewer w;
    if(argc > 1)
    {
        QString imageParam{argv[1]};
        w.onOpen(imageParam);
    }
    w.show();
    return a.exec();
}
