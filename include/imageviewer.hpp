#ifndef ImageViewer_HPP
#define ImageViewer_HPP

#include "imageinfo.hpp"
#include "imagelabel.hpp"
#include "contextmenu.hpp"
#include "recentfilesmanager.hpp"
#include <QDir>
#include <QSet>
#include <QIcon>
#include <QTimer>
#include <QStyle>
#include <QPixmap>
#include <QScreen>
#include <QSpinBox>
#include <QMenuBar>
#include <QKeyEvent>
#include <QMimeData>
#include <QComboBox>
#include <QStatusBar>
#include <QEventLoop>
#include <QTransform>
#include <QMessageBox>
#include <QWheelEvent>
#include <QFileDialog>
#include <QImageReader>
#include <QApplication>
#include <QDirIterator>
#include <QMainWindow>
#include <QRandomGenerator>

class ImageViewer : public QMainWindow
{
Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();

protected:
    void keyPressEvent(QKeyEvent *) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;

private:
    const QSet<QString> IMAGE_EXTENSIONS{"ico", "jpg", "jpeg", "bmp", "png", "gif", "pbm", "pgm", "ppm",
                                         "xbm", "xpm"};
    int                 timeToWait{2000};
    double              zoomFactor{1.};
    RecentFilesManager  recentFilesManager;
    QWidget             *container;
    QMenu               *file;
    QMenu               *zoom;
    QMenu               *rotation;
    QMenu               *advanced;
    QMenu               *recentlyOpened;
    QAction             *openImage;
    QAction             *saveImage;
    QAction             *saveImageAs;
    QAction             *quit;
    QAction             *plus;
    QAction             *minus;
    QAction             *rotateDirect;
    QAction             *rotateIndirect;
    QAction             *reset;
    QAction             *slideshowStart;
    QAction             *randomImage;
    QAction             *rgbSwap;
    QAction             *slideTime;
    QAction             *info;
    QPushButton         *nextImage;
    QPushButton         *previousImage;
    ImageLabel          *imageLabel;
    QString             currentImageName;
    QStatusBar          *positionBar;
    QImage              img;
    QPixmap             pixmap;
    QSettings           imgViewerSettings;
    int                 width;
    int                 height;
    QDir                imageDirectory;
    int                 currentIndexInDir{0};
    QList<QString>      directoryImages{};
    bool                slideshowIsRunning{false};
    bool                appIsFullScreen{false};
    qreal               angleRotation{0};
    [[nodiscard]] bool isSupportedImage(const QString &fileName) const;
    void readImage(const QString &name);
    void readImageWithRotation(const QString &name, qreal angle);
    void scaleImage(double factor);
    void applyStyle();
    void buildMenubarAndComponents();
    void buildRecentlyOpenedFileList();
    void setShortcuts();
    void applyLayout();
    void fillElements(const QString &startElement);
    void setFullScreen(bool ok);
    void startSlideshow();
    void endSlideshow();
    void makeConnections();
    void disableElements();
    void enableElements();

private slots:
    void onDialogOpen();
    void onZoomPlus();
    void onZoomMinus();
    void onReset();
    void onNext();
    void onPrevious();
    void onRotateDirect();
    void onRotateIndirect();
    void onSlideshow();
    void onRandom();
    void onSave() const;
    void onSaveAs();
    void swapRgb();
    void onSlideshowTime();
    void changeSlideshowTime(int time);
    void showInfo();
    void showContextMenu(const QPoint &pos);
    void updateCursorPos(int x, int y);

public slots:
    void onOpen(const QString &fileImage);
};

#endif // ImageViewer_HPP