#ifndef ImageViewer_HPP
#define ImageViewer_HPP
#include "style.hpp"

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
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QDir>
#include <QDirIterator>
#include <QList>
#include <QTransform>
#include <QFileInfo>
#include <QEventLoop>
#include <QKeyEvent>
#include <QLabel>
#include <QIcon>
#include <QTimer>
#include <QMimeData>

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
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
    QString currentImageName;
    QImage img;
    QPixmap pixmap;
    int width;
    int height;
    QDir imageDirectory;
    int nbNext = 0;
    QList<QString> previousImages = {};
    QList<QString> nextImages = {};
    void readImage(const QString &name);
    void readImageWithRotation(const QString &name,qreal angle);
    qreal angleRotation = 0;
    void scaleImage(double factor);
    void applyStyle();
    void buildComponents();
    void buildMenu();
    void setShortcuts();
    void applyLayout();
    void fillNextElements();
private slots:
    void onDialogOpen();
    void onOpen(const QString &fileImage);
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
