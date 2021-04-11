#include "imageviewer.hpp"

ImageViewer::ImageViewer(QWidget *parent) : QWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setMouseTracking(true);
    buildComponents();
    buildMenu();
    setShortcuts();
    applyStyle();
    applyLayout();
    setAcceptDrops(true);
    makeConnections();
    timeToWait = (imgViewerSettings.value("Time").toInt() > 1000) ? imgViewerSettings.value("Time").toInt() : 4000;
}

// Make the connections.
void ImageViewer::makeConnections()
{
    connect(nextImage,&QPushButton::clicked,this,&ImageViewer::onNext);
    connect(previousImage,&QPushButton::clicked,this,&ImageViewer::onPrevious);
    connect(openImage,&QAction::triggered,this,&ImageViewer::onDialogOpen);
    connect(saveimage,&QAction::triggered,this,&ImageViewer::onSave);
    connect(saveimageAs,&QAction::triggered,this,&ImageViewer::onSaveAs);
    connect(plus,&QAction::triggered,this,&ImageViewer::onZoomPlus);
    connect(minus,&QAction::triggered,this,&ImageViewer::onZoomMinus);
    connect(reset,&QAction::triggered,this,&ImageViewer::onReset);
    connect(quit,&QAction::triggered,this,&QApplication::quit);
    connect(info,&QAction::triggered,this,&ImageViewer::showInfo);
    connect(rotateDirect,&QAction::triggered,this,&ImageViewer::onRotateDirect);
    connect(rotateIndirect,&QAction::triggered,this,&ImageViewer::onRotateIndirect);
    connect(rgbSwap,&QAction::triggered,this,&ImageViewer::swapRgb);
    connect(slideshowStart, &QAction::triggered, this, &ImageViewer::onSlideshow);
    connect(randomImage,&QAction::triggered,this,&ImageViewer::onRandom);
    connect(slideTime, &QAction::triggered, this, &ImageViewer::onSlideshowTime);
    connect(this,&QWidget::customContextMenuRequested,this,&ImageViewer::showContextMenu);
}

// Components creation.
void ImageViewer::buildComponents()
{
    file           = new QMenu("File",this);
    zoom           = new QMenu("Zoom",this);
    rotation       = new QMenu("Rotation",this);
    advanced       = new QMenu("Advanced",this);
    openImage      = new QAction(QIcon(":assets/open.ico"),"Open",this);
    saveimage      = new QAction(QIcon(":assets/save.ico"),"Save",this);
    saveimageAs    = new QAction(QIcon(":assets/saveas.ico"),"Save as",this);
    quit           = new QAction(QIcon(":assets/quit.ico"),"Quit",this);
    plus           = new QAction(QIcon(":assets/plus.ico"),"Zoom In",this);
    minus          = new QAction(QIcon(":assets/minus.ico"),"Zoom out",this);
    rotateDirect   = new QAction(QIcon(":assets/direct.ico"),"Rotate direct",this);
    rotateIndirect = new QAction(QIcon(":assets/indirect.ico"),"Rotate indirect",this);
    reset          = new QAction(QIcon(":assets/reset.ico"),"Reset",this);
    slideshowStart = new QAction(QIcon(":assets/diaporama.ico"), "Diaporama", this);
    slideTime      = new QAction(QIcon(":assets/timer.ico"), "Slideshow duration", this);
    randomImage    = new QAction(QIcon(":assets/random.ico"),"Random play",this);
    rgbSwap        = new QAction(QIcon(":assets/rgb.ico"),"Rgb swap",this);
    info           = new QAction(QIcon(":assets/info.ico"),"Info",this);
    previousImage  = new QPushButton(QIcon(":assets/previous.ico"),"");
    nextImage      = new QPushButton(QIcon(":assets/next.ico"),"");
    imageLabel     = new QLabel(this);
    positionBar    = new QStatusBar(this);
    imageLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
}

// Menu building.
void ImageViewer::buildMenu()
{
    myMenu = new QMenuBar();
    file->addAction(openImage);
    file->addSeparator();
    file->addAction(saveimage);
    file->addAction(saveimageAs);
    file->addSeparator();
    file->addAction(quit);
    file->addSeparator();
    file->addAction(info);
    zoom->addAction(minus);
    zoom->addAction(plus);
    zoom->addAction(reset);
    rotation->addAction(rotateDirect);
    rotation->addAction(rotateIndirect);
    rotation->addAction(reset);
    advanced->addAction(rgbSwap);
    advanced->addSeparator();
    advanced->addAction(slideshowStart);
    advanced->addAction(slideTime);
    advanced->addSeparator();
    advanced->addAction(randomImage);
    myMenu->addMenu(file);
    myMenu->addSeparator();
    myMenu->addMenu(zoom);
    myMenu->addSeparator();
    myMenu->addMenu(rotation);
    myMenu->addSeparator();
    myMenu->addMenu(advanced);
}

// Shortcuts.
void ImageViewer::setShortcuts()
{
    openImage->setShortcut(QKeySequence::Open);
    saveimage->setShortcut(QKeySequence::Save);
    saveimageAs->setShortcut(QKeySequence::SaveAs);
    quit->setShortcut(QKeySequence::Quit);
    plus->setShortcut(QKeySequence::ZoomIn);
    minus->setShortcut(QKeySequence::ZoomOut);
    reset->setShortcuts({QKeySequence::Refresh,QKeySequence::Cancel});
}

// Stylesheet and window resizing.
void ImageViewer::applyStyle()
{
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QGuiApplication::primaryScreen()->availableGeometry()));
    setWindowIcon(QIcon(":assets/icon.ico"));
    resize(QGuiApplication::primaryScreen()->availableSize() * 3/5);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    info->setDisabled(true);
    previousImage->setDisabled(true);
    nextImage->setDisabled(true);
    setStyleSheet(STYLE);
}

// Applying a layout to the main window.
void ImageViewer::applyLayout()
{
    auto topLayout = new QHBoxLayout();
    topLayout->addWidget(myMenu);
    auto *buttonLay = new QHBoxLayout();
    buttonLay->setContentsMargins(3,3,3,0);
    buttonLay->setAlignment(Qt::AlignHCenter);
    buttonLay->addWidget(previousImage);
    buttonLay->addWidget(nextImage);
    auto *appLayout = new QVBoxLayout(this);
    appLayout->setContentsMargins(0,0,0,0);
    appLayout->addLayout(topLayout,1);
    appLayout->addWidget(imageLabel,17);
    appLayout->addLayout(buttonLay,1);
    appLayout->addWidget(positionBar,1);
}

// Dialog file to choose the image to open
void ImageViewer::onDialogOpen()
{
    currentImageName = QFileDialog::getOpenFileName(this);
    if(currentImageName.isEmpty() || currentImageName.isNull())
    {
        QMessageBox::warning(this,"Image","Enter a valid name");
    }
    else
    {
        onOpen(currentImageName);
    }
}

// Opening a picture
void ImageViewer::onOpen(const QString &fileImage)
{
    // Storing the current directory
    imageDirectory.setPath(QFileInfo{fileImage}.absoluteDir().path()+"/");
    fillElements(fileImage);
    readImage(fileImage);
    setWindowTitle(fileImage);
}

// Zoom
void ImageViewer::onZoomPlus()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        scaleImage(1.17);
    }
}

void ImageViewer::onZoomMinus()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        scaleImage(0.8);
    }
}

// Reset the image scaling or the rotation
void ImageViewer::onReset()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        zoomFactor    = 1;
        angleRotation = 0;
        readImage(currentImageName);
    }
}

// Scaling the image for the zoom.
void ImageViewer::scaleImage(double factor)
{
    if((zoomFactor*factor > 0.05) && (zoomFactor*factor < 10))
    {
        zoomFactor *= factor;
        const QSize size {imageLabel->pixmap(Qt::ReturnByValue).size() * zoomFactor};
        imageLabel->resize(size);
    }
}

// Fill the list of next images in the directory
void ImageViewer::fillElements(const QString &startElement)
{
    directoryImages.clear();
    directoryImages.push_back(startElement);
    QDirIterator imageDirIt{imageDirectory};
    QString tmpImage;
    while(imageDirIt.hasNext() && QFileInfo(tmpImage = imageDirIt.next()).isReadable())
    {
        if((tmpImage != currentImageName) && isSupportedImage(tmpImage))
        {
            directoryImages.push_back(tmpImage);
        }
    }
}

// Read an image in the imageLabel.
void ImageViewer::readImage(const QString &name)
{
    QImageReader reader{name};
    reader.setAutoTransform(true);
    img = reader.read();
    if (img.isNull())
    {
        QMessageBox::critical(this,"Image","Cannot open the image");
        return;
    }
    pixmap = QPixmap::fromImage(img);
    height = pixmap.height();
    width  = pixmap.width();
    info->setDisabled(false);
    nextImage->setDisabled(currentIndexInDir >= directoryImages.size()-1);
    previousImage->setDisabled(currentIndexInDir <= 0);
    imageLabel->setPixmap(pixmap);
    imageLabel->setScaledContents(true);
    currentImageName = name;
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setWindowTitle(name);
}

void ImageViewer::readImageWithRotation(const QString &name,qreal angle)
{
    if(!name.isEmpty())
    {
        QImageReader reader{name};
        reader.setAutoTransform(true);
        img = reader.read();
        if (img.isNull())
        {
            QMessageBox::critical(this,"Image","Cannot open the image");
        }
        QTransform transformation {};
        transformation = transformation.rotate(angle);
        pixmap = QPixmap::fromImage(img);
        height = pixmap.height();
        width  = pixmap.width();
        imageLabel->setPixmap((QPixmap::fromImage(img)).transformed(transformation).scaled(height,width));
        imageLabel->setScaledContents(true);
        currentImageName = name;
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    }
}

// Swap rgb colors in the current image
void ImageViewer::swapRgb()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        img    = img.rgbSwapped();
        pixmap = QPixmap::fromImage(img);
        imageLabel->setPixmap(pixmap);
        imageLabel->setScaledContents(true);
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    }
}

// Read the following image in the directory.
void ImageViewer::onNext()
{
    if(currentIndexInDir < directoryImages.size()-1)
    {
        currentIndexInDir++;
        const auto imgToRead{directoryImages.at(currentIndexInDir)};
        readImage(imgToRead);
    }
}

// Read the previous image in the directory.
void ImageViewer::onPrevious()
{
    if(currentIndexInDir > 0)
    {
        currentIndexInDir--;
        const auto imgToRead{directoryImages.at(currentIndexInDir)};
        readImage(imgToRead);
    }
}

// Saving the image in the current qlabel in the current file
void ImageViewer::onSave() const
{
    if(!QPixmap::fromImage(img).isNull())
    {
        imageLabel->pixmap(Qt::ReturnByValue).toImage().save(currentImageName);
    }
}

// Saving the image in the current qlabel in a new file
void ImageViewer::onSaveAs()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        const auto imageSaveName{QFileDialog::getSaveFileName(this)};
        if( (imageSaveName.isEmpty()) || (imageLabel->pixmap(Qt::ReturnByValue).toImage().save(imageSaveName)))
        {
            QMessageBox::information(this,"Save as","Image saved successfully");
        }
        else
        {
            QMessageBox::warning(this,"Save as","Image could not be saved");
        }
    }
}

// Rotation
void ImageViewer::onRotateDirect()
{
    angleRotation -= 90;
    readImageWithRotation(currentImageName,angleRotation);
}

void ImageViewer::onRotateIndirect()
{
    angleRotation += 90;
    readImageWithRotation(currentImageName,angleRotation);
}

// Slideshow
void ImageViewer::onSlideshow()
{
    if(QPixmap::fromImage(img).isNull()) return;
    startSlideshow();
    QDirIterator imgDirIterator{imageDirectory};
    const auto index{directoryImages.indexOf(currentImageName)};
    QList<QString> imageList{directoryImages.begin()+index,directoryImages.end()};
    foreach(auto tmp,imageList)
    {
        if(!slideshowIsRunning) break;
        QTimer timer;
        QEventLoop loop; // Event loop to read the images during a time.
        connect(&timer,&QTimer::timeout,&loop,&QEventLoop::quit);
        timer.start(timeToWait);
        readImage(tmp);
        loop.exec();
        if(!slideshowIsRunning) break;
    }
    endSlideshow();
}

// Changing the duration of the slideshow
void ImageViewer::onSlideshowTime()
{
    QMessageBox::information(this,"Slideshow duration","Choose the slideshow duration in seconds");
    auto numberBox = new QSpinBox();
    numberBox->setRange(1,30);
    numberBox->setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),
                                               QGuiApplication::primaryScreen()->availableGeometry()));
    numberBox->setFixedSize(100,100);
    connect(numberBox,QOverload<int>::of(&QSpinBox::valueChanged),this, &ImageViewer::changeSlideshowTime);
    numberBox->show();
}

// Updating the time
void ImageViewer::changeSlideshowTime(int time)
{
    time       = std::abs(time);
    timeToWait = time*1000;
    imgViewerSettings.setValue("Time",QString::number(timeToWait));
}

// Start the slideshow
void ImageViewer::startSlideshow()
{
    nextImage->setVisible(false);
    previousImage->setVisible(false);
    myMenu->setVisible(false);
    positionBar->setVisible(false);
    slideshowIsRunning = true;
    setFullScreen(true);
}

// End the slideshow
void ImageViewer::endSlideshow()
{
    nextImage->setVisible(true);
    previousImage->setVisible(true);
    myMenu->setVisible(true);
    positionBar->setVisible(true);
    slideshowIsRunning = false;
    setFullScreen(false);
}

// Read a random image in the folder
void ImageViewer::onRandom()
{
    if(directoryImages.size() < 2) return;
    auto const randomIndex{QRandomGenerator::global()->bounded(0,directoryImages.size()-1)};
    readImage(directoryImages.at(randomIndex));
}

// Context menu for the right click
void ImageViewer::showContextMenu(const QPoint &pos)
{
    auto contextMenu = new ContextMenu("Menu",this);
    connect(contextMenu,&ContextMenu::imageOpenRequested,this,&ImageViewer::onDialogOpen);
    connect(contextMenu,&ContextMenu::saveRequested,this,&ImageViewer::onSave);
    connect(contextMenu,&ContextMenu::previousRequested,this,&ImageViewer::onPrevious);
    connect(contextMenu,&ContextMenu::nextRequested,this,&ImageViewer::onNext);
    connect(contextMenu,&ContextMenu::slideShowRequested,this,&ImageViewer::onSlideshow);
    contextMenu->exec(mapToGlobal(pos));
}

// Drag event to open images.
void ImageViewer::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

// Drop event to open Images.
void ImageViewer::dropEvent(QDropEvent *event)
{
    const auto mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        const auto urlList {mimeData->urls()};
        currentImageName = urlList[0].toLocalFile();
        onOpen(currentImageName);
    }
}

// Handling all key events.
void ImageViewer::keyPressEvent(QKeyEvent *e)
{
    slideshowIsRunning = false;
    switch (e->key())
    {
        case Qt::Key_Left:
            onPrevious();
            break;
        case Qt::Key_Right:
            onNext();
            break;
        case Qt::Key_F11:
            setFullScreen(!appIsFullScreen);
            break;
        default:
            break;
    }
}

// Mouse tracking on the status bar
void ImageViewer::mouseMoveEvent(QMouseEvent *ev)
{
    QString text{"x : "+QString::number(ev->pos().x())+" y : "+QString::number(ev->pos().y())};
    positionBar->showMessage(text);
}

// Wheel event for the zoom
void ImageViewer::wheelEvent(QWheelEvent *event)
{
    const auto rotationPoint{event->angleDelta()};
    if(rotationPoint.y() > 0)
    {
        onZoomPlus();
    }
    else
    {
        onZoomMinus();
    }
}

// When the mouse is pressed, we leave the slideshow
void ImageViewer::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    slideshowIsRunning = false;
}

// The application goes fullscreen
void ImageViewer::setFullScreen(bool ok)
{
    appIsFullScreen = ok;
    (appIsFullScreen) ? showFullScreen() : showNormal();
}

// Show information about the current image in a popup
void ImageViewer::showInfo()
{
    if(img.isNull()) return;
    auto infoDialog = new ImageInfo(this,img,currentImageName);
    infoDialog->show();
}

// Is the file supported ?
bool ImageViewer::isSupportedImage(const QString &fileName) const
{
    return IMAGE_EXTENSIONS.contains(QFileInfo(fileName).suffix());
}

ImageViewer::~ImageViewer()
{}