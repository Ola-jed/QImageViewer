#include "imageviewer.hpp"

ImageViewer::ImageViewer(QWidget *parent) : QMainWindow(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setMouseTracking(true);
    buildMenubarAndComponents();
    setShortcuts();
    applyStyle();
    applyLayout();
    setAcceptDrops(true);
    makeConnections();
    timeToWait = (imgViewerSettings.value("Time").toInt() > 1000)
            ? imgViewerSettings.value("Time").toInt()
            : 4000;
    centralWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);
    setMouseTracking(true);
}

/// Make the connections.
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
    connect(slideTime, &QAction::triggered,this, &ImageViewer::onSlideshowTime);
    connect(this,&QWidget::customContextMenuRequested,this,&ImageViewer::showContextMenu);
}

/// Menubar and Components creation.
void ImageViewer::buildMenubarAndComponents()
{
    // Building menubar items
    file     = menuBar()->addMenu("File");
    zoom     = menuBar()->addMenu("Zoom");
    rotation = menuBar()->addMenu("Rotation");
    advanced = menuBar()->addMenu("Advanced");
    // File menu actions
    openImage = file->addAction(QIcon(":assets/open.ico"),"Open");
    file->addSeparator();
    saveimage   = file->addAction(QIcon(":assets/save.ico"),"Save");
    saveimageAs = file->addAction(QIcon(":assets/saveas.ico"),"Save as");
    file->addSeparator();
    quit = file->addAction(QIcon(":assets/quit.ico"),"Quit");
    file->addSeparator();
    info = file->addAction(QIcon(":assets/info.ico"),"Info");
    file->addSeparator();
    recentlyOpened = file->addMenu("Recently opened");
    // Zoom menu actions
    plus  = zoom->addAction(QIcon(":assets/plus.ico"),"Zoom In");
    minus = zoom->addAction(QIcon(":assets/minus.ico"),"Zoom out");
    zoom->addSeparator();
    reset = zoom->addAction(QIcon(":assets/reset.ico"),"Reset");
    // Rotation menu actions
    rotateDirect   = rotation->addAction(QIcon(":assets/direct.ico"),"Rotate direct");
    rotateIndirect = rotation->addAction(QIcon(":assets/indirect.ico"),"Rotate indirect");
    rotation->addSeparator();
    rotation->addAction(reset);
    // Advanced menu actions
    rgbSwap        = advanced->addAction(QIcon(":assets/rgb.ico"),"Rgb swap");
    advanced->addSeparator();
    slideshowStart = advanced->addAction(QIcon(":assets/diaporama.ico"), "Slideshow");
    slideTime      = advanced->addAction(QIcon(":assets/timer.ico"), "Slideshow duration");
    advanced->addSeparator();
    randomImage    = advanced->addAction(QIcon(":assets/random.ico"),"Random play");
    // Other components
    previousImage  = new QPushButton(QIcon(":assets/previous.ico"),"");
    nextImage      = new QPushButton(QIcon(":assets/next.ico"),"");
    imageLabel     = new QLabel(this);
    positionBar    = new QStatusBar(this);
    setStatusBar(positionBar);
    imageLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
    disableElements();
}

/// Shortcuts.
void ImageViewer::setShortcuts()
{
    openImage->setShortcut(QKeySequence::Open);
    saveimage->setShortcut(QKeySequence::Save);
    saveimageAs->setShortcut(QKeySequence::SaveAs);
    info->setShortcut(QKeySequence::HelpContents);
    quit->setShortcut(QKeySequence::Quit);
    plus->setShortcut(QKeySequence::ZoomIn);
    minus->setShortcut(QKeySequence::ZoomOut);
    reset->setShortcuts({QKeySequence::Refresh,QKeySequence::Cancel});
}

/// Enable the menu elements because an element is printed
void ImageViewer::enableElements()
{
    saveimage->setDisabled(false);
    saveimageAs->setDisabled(false);
    rotateDirect->setDisabled(false);
    rotateIndirect->setDisabled(false);
    rgbSwap->setDisabled(false);
    slideshowStart->setDisabled(false);
    reset->setDisabled(false);
    plus->setDisabled(false);
    minus->setDisabled(false);
    randomImage->setDisabled(false);
}

/// Disable menu actions because no image is printed
void ImageViewer::disableElements()
{
    saveimage->setDisabled(true);
    saveimageAs->setDisabled(true);
    rotateDirect->setDisabled(true);
    rotateIndirect->setDisabled(true);
    rgbSwap->setDisabled(true);
    slideshowStart->setDisabled(true);
    reset->setDisabled(true);
    plus->setDisabled(true);
    minus->setDisabled(true);
    randomImage->setDisabled(true);
}

/// Stylesheet and window resizing.
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
}

/// Applying a layout to the main window.
void ImageViewer::applyLayout()
{
    container = new QWidget(this);
    auto buttonLay = new QHBoxLayout();
    buttonLay->setContentsMargins(3,3,3,0);
    buttonLay->setAlignment(Qt::AlignHCenter);
    buttonLay->addWidget(previousImage);
    buttonLay->addWidget(nextImage);
    auto containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addWidget(imageLabel, 17);
    containerLayout->addLayout(buttonLay, 1);
    setCentralWidget(container);
}

/// Dialog file to choose the image to open
void ImageViewer::onDialogOpen()
{
    currentImageName = QFileDialog::getOpenFileName(this);
    if(currentImageName.isEmpty() || currentImageName.isNull())
    {
        QMessageBox::warning(this,"Image","Enter a valid name");
        return;
    }
    onOpen(currentImageName);
}

/// Opening a picture
/// \param fileImage
void ImageViewer::onOpen(const QString &fileImage)
{
    // Storing the current directory
    imageDirectory.setPath(QFileInfo{fileImage}.absoluteDir().path()+"/");
    fillElements(fileImage);
    readImage(fileImage);
    setWindowTitle(fileImage);
}

/// Zoom in
void ImageViewer::onZoomPlus()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        scaleImage(1.17);
    }
}

/// Zoom out
void ImageViewer::onZoomMinus()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        scaleImage(0.8);
    }
}

/// Reset the image scaling or the rotation
void ImageViewer::onReset()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        zoomFactor    = 1;
        angleRotation = 0;
        QImageReader reader{currentImageName};
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
        imageLabel->setPixmap(pixmap);
        imageLabel->setScaledContents(true);
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    }
}

/// Scaling the image for the zoom.
/// \param factor
void ImageViewer::scaleImage(double factor)
{
    if((zoomFactor*factor > 0.05) && (zoomFactor*factor < 10))
    {
        zoomFactor *= factor;
        const auto size {imageLabel->pixmap(Qt::ReturnByValue).size() * zoomFactor};
        imageLabel->resize(size);
    }
}

/// Fill the list of next images in the directory
/// \param startElement
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

/// Read an image in the imageLabel.
/// \param name
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
    enableElements();
}

/// Read and image and rotate
/// \param name
/// \param angle
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
            return;
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

/// Swap rgb colors in the current image
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

/// Read the following image in the directory.
void ImageViewer::onNext()
{
    if(currentIndexInDir < directoryImages.size()-1)
    {
        currentIndexInDir++;
        const auto imgToRead{directoryImages.at(currentIndexInDir)};
        readImage(imgToRead);
    }
}

/// Read the previous image in the directory.
void ImageViewer::onPrevious()
{
    if(currentIndexInDir > 0)
    {
        currentIndexInDir--;
        const auto imgToRead{directoryImages.at(currentIndexInDir)};
        readImage(imgToRead);
    }
}

/// Saving the image in the current label printed in the current file
void ImageViewer::onSave() const
{
    if(!QPixmap::fromImage(img).isNull())
    {
        imageLabel->pixmap(Qt::ReturnByValue).toImage().save(currentImageName);
    }
}

/// Saving the image in the current label in a new file
void ImageViewer::onSaveAs()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        const auto imageSaveName{QFileDialog::getSaveFileName(this)};
        if((!imageSaveName.isEmpty()) && (imageLabel->pixmap(Qt::ReturnByValue).toImage().save(imageSaveName)))
        {
            QMessageBox::information(this,"Save as","Image saved successfully");
        }
        else
        {
            QMessageBox::warning(this,"Save as","Image could not be saved");
        }
    }
}

/// Rotation in direct angle
void ImageViewer::onRotateDirect()
{
    angleRotation -= 90;
    readImageWithRotation(currentImageName,angleRotation);
}

/// Rotation in indirect angle
void ImageViewer::onRotateIndirect()
{
    angleRotation += 90;
    readImageWithRotation(currentImageName,angleRotation);
}

/// Slideshow
void ImageViewer::onSlideshow()
{
    if(QPixmap::fromImage(img).isNull()) return;
    startSlideshow();
    QDirIterator imgDirIterator{imageDirectory};
    const auto index{directoryImages.indexOf(currentImageName)};
    QList<QString> imageList{directoryImages.begin()+index,directoryImages.end()};
    foreach(const auto& tmp,imageList)
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

/// Changing the duration of the slideshow
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

/// Updating the slideshow duration
/// \param time
void ImageViewer::changeSlideshowTime(int time)
{
    time = std::abs(time);
    timeToWait = time*1000;
    imgViewerSettings.setValue("Time",QString::number(timeToWait));
}

/// Start the slideshow
void ImageViewer::startSlideshow()
{
    nextImage->setVisible(false);
    previousImage->setVisible(false);
    menuBar()->setVisible(false);
    positionBar->setVisible(false);
    slideshowIsRunning = true;
    setFullScreen(true);
}

/// End the slideshow
void ImageViewer::endSlideshow()
{
    nextImage->setVisible(true);
    previousImage->setVisible(true);
    menuBar()->setVisible(true);
    positionBar->setVisible(true);
    slideshowIsRunning = false;
    setFullScreen(false);
}

/// Read a random image in the folder
void ImageViewer::onRandom()
{
    if(directoryImages.size() < 2) return;
    auto const randomIndex{QRandomGenerator::global()->bounded(0,directoryImages.size()-1)};
    readImage(directoryImages.at(randomIndex));
}

/// Context menu for the right click
/// \param pos
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

/// Drag event to open images.
/// \param e
void ImageViewer::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) e->acceptProposedAction();
}

/// Drop event to open Images.
/// \param event
void ImageViewer::dropEvent(QDropEvent *event)
{
    const auto urlList{event->mimeData()->urls()};
    currentImageName = urlList[0].toLocalFile();
    onOpen(currentImageName);
}

/// Handling all key events.
/// \param e
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

/// Mouse tracking on the status bar
/// \param ev
void ImageViewer::mouseMoveEvent(QMouseEvent *ev)
{
    const auto text{"x : "+QString::number(ev->pos().x())+" y : "+QString::number(ev->pos().y())};
    positionBar->showMessage(text);
}

/// Wheel event for the zoom
/// \param event
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

/// When the mouse is pressed, we leave the slideshow
/// \param ev
void ImageViewer::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    slideshowIsRunning = false;
}

/// The application goes full screen
/// \param ok
void ImageViewer::setFullScreen(bool ok)
{
    appIsFullScreen = ok;
    (appIsFullScreen) ? showFullScreen() : showNormal();
}

/// Show information about the current image in a popup
void ImageViewer::showInfo()
{
    if(img.isNull()) return;
    auto infoDialog = new ImageInfo(this,img,currentImageName);
    infoDialog->show();
}

/// Is the file supported ?
/// \param fileName
/// \return bool
bool ImageViewer::isSupportedImage(const QString &fileName) const
{
    return IMAGE_EXTENSIONS.contains(QFileInfo(fileName).suffix());
}

ImageViewer::~ImageViewer()
= default;