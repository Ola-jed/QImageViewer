#ifndef ImageViewer_HPP
#define ImageViewer_HPP

#include <QScreen>
#include <QStyle>
#include <QDebug>
#include <QPushButton>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QImageReader>
#include <QImage>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QDir>
#include <QDirIterator>
#include <QStack>
#include <QTransform>
#include <QFileInfo>
#include <QThread>
#include <QKeyEvent>
#include <QLabel>
#include <QIcon>
#include <QTimer>


class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();
private:
    double zoomFactor = 1.;
    QPushButton *openImage;
    QPushButton *quit;
    QPushButton *plus;
    QPushButton *minus;
    QPushButton *rotateDirect;
    QPushButton *rotateIndirect;
    QPushButton *reset;
    QPushButton *diapoButton;
    QPushButton *nextImage;
    QPushButton *previousImage;
    QLabel *imageLabel;
    QString imageName;
    QImage img;
    QPixmap pixmap;
    int width;
    int height;
    QDir imageDirectory;
    int nbNext = 0;
    QStack <QString> previousImages = {};
    void readImage(const QString &name);
    void readImageWithRotation(const QString &name,qreal angle);
    qreal angleRotation = 0;
    void scaleImage(double factor);
private slots:
    void onOpen();
    void onZoomPlus();
    void onZoomMinus();
    void onReset();
    void onNext();
    void onPrevious();
    void onRotateDirect();
    void onRotateIndirect();
    void onDiapo();
};
#endif // ImageViewer_HPP