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
#include <QDirIterator>
#include <QList>
#include <QTransform>
#include <QEventLoop>
#include <QKeyEvent>
#include <QIcon>
#include <QTimer>
#include <QKeyEvent>
#include <QMimeData>
#include <QComboBox>

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();
protected:
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    const QString Amoled    = loadStyleFromFile(":style/Amoled.qss");
    const QString Aqua      = loadStyleFromFile(":style/Aqua.qss");
    const QString Console   = loadStyleFromFile(":style/Console.qss");
    const QString Diffness  = loadStyleFromFile(":style/Diffness.qss");
    const QString Dtor      = loadStyleFromFile(":style/Dtor.qss");
    const QString Elegant   = loadStyleFromFile(":style/ElegantDark.qss");
    const QString Irrorater = loadStyleFromFile(":style/Irrorater.qss");
    const QString Mac       = loadStyleFromFile(":style/Mac.qss");
    const QString Manjaro   = loadStyleFromFile(":style/Manjaro.qss");
    const QString Material  = loadStyleFromFile(":style/MaterialDark.qss");
    const QString Neon      = loadStyleFromFile(":style/Neon.qss");
    const QString Obit      = loadStyleFromFile(":style/Obit.qss");
    const QString Synet     = loadStyleFromFile(":style/Synet.qss");
    const QString Ubuntu    = loadStyleFromFile(":style/Ubuntu.qss");
    const QString World     = loadStyleFromFile(":style/World.qss");
    const QMap<QString,QString> THEME_NAMES{
        {"Amoled",Amoled},
        {"Aqua",Aqua},
        {"Console",Console},
        {"Diffness",Diffness},
        {"Dtor",Dtor},
        {"Elegant Dark",Elegant},
        {"Irrorater",Irrorater},
        {"Mac",Mac},
        {"Manjaro",Manjaro},
        {"Material Dark",Material},
        {"Neon",Neon},
        {"Obit",Obit},
        {"Synet",Synet},
        {"Ubuntu",Ubuntu},
        {"World",World}
    };
    long timeToWait{2000};
    double zoomFactor {1.};
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
    QAction *info;
    QPushButton *nextImage;
    QPushButton *previousImage;
    QLabel *imageLabel;
    QString currentImageName;
    QComboBox *themeChoice;
    QStatusBar *positionBar;
    QImage img;
    QPixmap pixmap;
    QSettings imgViewerSettings;
    int width;
    int height;
    QDir imageDirectory;
    int nbNext = 0;
    QList<QString> previousImages = {};
    QList<QString> nextImages = {};
    qreal angleRotation = 0;
    void readImage(const QString &name);
    void readImageWithRotation(const QString &name,qreal angle);
    void scaleImage(double factor);
    void applyStyle();
    void buildComponents();
    void buildThemeList();
    void buildMenu();
    void setShortcuts();
    void applyLayout();
    void fillNextElements();
    bool isRunningDiapo{false};
    bool appIsFullScreen{false};
    void setFullScreen(bool ok);
    void startDiapo();
    void endDiapo();
    void makeConnections();
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
    void onApplyOtherTheme(const QString &theme);
    void showInfo();
};
#endif // ImageViewer_HPP
