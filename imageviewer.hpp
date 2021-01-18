#ifndef ImageViewer_HPP
#define ImageViewer_HPP

#include <QScreen>
#include <QStyle>
#include <QPushButton>
#include <QSpinBox>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QImageReader>
#include <QImage>
#include <QMenuBar>
#include <QAction>
#include <QRandomGenerator>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QDir>
#include <QDirIterator>
#include <QList>
#include <QStack>
#include <QTransform>
#include <QFileInfo>
#include <QEventLoop>
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
    long TIME_TO_WAIT{2000};
    double zoomFactor = 1.;
    QMenuBar *myMenu;
    QMenu *file;
    QMenu *zoom;
    QMenu *rotation;
    QMenu *advanced;
    QAction *openImage;
    QAction *quit;
    QAction *plus;
    QAction *minus;
    QAction *rotateDirect;
    QAction *rotateIndirect;
    QAction *reset;
    QAction *diapoButton;
    QAction *randomImage;
    QAction *diapoTime;
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
    void applyStyle();
    void buildComponents();
    void buildMenu();
    void setShortcuts();
    void applyLayout();
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
    void onRandom();
    void onDiapoTime();
    void changeDiapoTime(int time);
};
#endif // ImageViewer_HPP
