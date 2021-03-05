#include "imageviewer.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("QImageViewer");
    QCoreApplication::setOrganizationName("QImageViewer");
    ImageViewer w;
    if(argc > 1)
    {
        const QString imageParam{argv[1]};
        w.onOpen(imageParam);
    }
    w.show();
    return a.exec();
}
