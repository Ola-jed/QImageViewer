#ifndef ImageViewer_HPP
#define ImageViewer_HPP

#include "style.hpp"
#include "imageinfo.hpp"
#include <QMap>
#include <QScreen>
#include <QStyle>
#include <QSpinBox>
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QImageReader>
#include <QMenuBar>
#include <QAction>
#include <QRandomGenerator>
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QStatusBar>
#include <QDir>
#include <QSet>
#include <QDirIterator>
#include <QList>
#include <QTransform>
#include <QEventLoop>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QIcon>
#include <QTimer>
#include <QKeyEvent>
#include <QMimeData>
#include <QComboBox>

class ImageViewer : public QWidget
{
    Q_OBJECT

    public:
        explicit ImageViewer(QWidget *parent = nullptr);
        ~ImageViewer();

    protected:
        void keyPressEvent(QKeyEvent *) override;
        void mousePressEvent(QMouseEvent *) override;
        void mouseMoveEvent(QMouseEvent *) override;
        void dragEnterEvent(QDragEnterEvent *event) override;
        void dropEvent(QDropEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;
    //TODO
    // Add scrollbar for the zoom and popup menu on right click
    private:
        const QSet<QString> IMAGE_EXTENSIONS{"ico","jpg","jpeg","bmp","png","gif","pbm","pgm","ppm","xbm","xpm"};
        int timeToWait{2000};
        double zoomFactor {1.};
        QMenuBar *myMenu;
        QMenu *file;
        QMenu *zoom;
        QMenu *rotation;
        QMenu *advanced;
        QAction *openImage;
        QAction *saveimage;
        QAction *saveimageAs;
        QAction *quit;
        QAction *plus;
        QAction *minus;
        QAction *rotateDirect;
        QAction *rotateIndirect;
        QAction *reset;
        QAction *diapoButton;
        QAction *randomImage;
        QAction *rgbSwap;
        QAction *diapoTime;
        QAction *info;
        QPushButton *nextImage;
        QPushButton *previousImage;
        QLabel *imageLabel;
        QString currentImageName;
        QStatusBar *positionBar;
        QImage img;
        QPixmap pixmap;
        QSettings imgViewerSettings;
        int width;
        int height;
        QDir imageDirectory;
        int currentIndexInDir{0};
        QList<QString> directoryImages{};
        bool isRunningDiapo{false};
        bool appIsFullScreen{false};
        qreal angleRotation {0};
        bool isSupportedImage(const QString &fileName) const;
        void readImage(const QString &name);
        void readImageWithRotation(const QString &name,qreal angle);
        void scaleImage(double factor);
        void applyStyle();
        void buildComponents();
        void buildMenu();
        void setShortcuts();
        void applyLayout();
        void fillElements(const QString &startElement);
        void setFullScreen(bool ok);
        void startDiapo();
        void endDiapo();
        void makeConnections();

    private slots:
        void onDialogOpen();
        void onZoomPlus();
        void onZoomMinus();
        void onReset();
        void onNext();
        void onPrevious();
        void onRotateDirect();
        void onRotateIndirect();
        void onDiapo();
        void onRandom();
        void onSave() const;
        void onSaveAs();
        void swapRgb();
        void onDiapoTime();
        void changeDiapoTime(int time);
        void showInfo();

    public slots:
        void onOpen(const QString &fileImage);
};
#endif // ImageViewer_HPP