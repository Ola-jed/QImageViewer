#include "imageviewer.hpp"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setApplicationVersion("1.3");
    QCoreApplication::setApplicationName("QImageViewer");
    QCoreApplication::setOrganizationName("QImageViewer");
    ImageViewer w;
    if(argc > 1) // Open the given image
    {
        const QString imageParam{argv[1]};
        w.onOpen(imageParam);
    }
    w.show();
    return QApplication::exec();
}