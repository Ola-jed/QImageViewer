#ifndef VIEWER_HPP
#define VIEWER_HPP

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


QT_BEGIN_NAMESPACE
namespace Ui { class Viewer; }
QT_END_NAMESPACE

class Viewer : public QMainWindow
{
    Q_OBJECT

public:
    Viewer(QWidget *parent = nullptr);
    ~Viewer();
private:
    Ui::Viewer *ui;
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
private slots:
    void onOpen();
    void onZoomPlus();
    void onZoomMinus();
    void onReset();
    void onNext();
    void onPrevious();
    void onRotateDirect();
    void onRotateIndirect();
};
#endif // VIEWER_HPP
