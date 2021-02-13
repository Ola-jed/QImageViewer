#include "imageviewer.hpp"

ImageViewer::ImageViewer(QWidget *parent) : QMainWindow(parent)
{
    buildComponents();
    buildMenu();
    buildThemeList();
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
    connect(themeChoice,&QComboBox::currentTextChanged,this,&ImageViewer::onApplyOtherTheme);
    connect(nextImage,&QPushButton::clicked,this,&ImageViewer::onNext);
    connect(previousImage,&QPushButton::clicked,this,&ImageViewer::onPrevious);
    connect(openImage,&QAction::triggered,this,&ImageViewer::onDialogOpen);
    connect(plus,&QAction::triggered,this,&ImageViewer::onZoomPlus);
    connect(minus,&QAction::triggered,this,&ImageViewer::onZoomMinus);
    connect(reset,&QAction::triggered,this,&ImageViewer::onReset);
    connect(quit,&QAction::triggered,this,&QApplication::quit);
    connect(info,&QAction::triggered,this,&ImageViewer::showInfo);
    connect(rotateDirect,&QAction::triggered,this,&ImageViewer::onRotateDirect);
    connect(rotateIndirect,&QAction::triggered,this,&ImageViewer::onRotateIndirect);
    connect(diapoButton,&QAction::triggered,this,&ImageViewer::onDiapo);
    connect(randomImage,&QAction::triggered,this,&ImageViewer::onRandom);
    connect(diapoTime,&QAction::triggered,this,&ImageViewer::onDiapoTime);
}

// Components creation.
void ImageViewer::buildComponents()
{
    file           = new QMenu("File",this);
    zoom           = new QMenu("Zoom",this);
    rotation       = new QMenu("Rotation",this);
    advanced       = new QMenu("Advanced",this);
    openImage      = new QAction(QIcon(":assets/open.ico"),"Open",this);
    quit           = new QAction(QIcon(":assets/quit.ico"),"Quit",this);
    plus           = new QAction(QIcon(":assets/plus.ico"),"Zoom In",this);
    minus          = new QAction(QIcon(":assets/minus.ico"),"Zoom out",this);
    rotateDirect   = new QAction(QIcon(":assets/direct.ico"),"Rotate direct",this);
    rotateIndirect = new QAction(QIcon(":assets/indirect.ico"),"Rotate indirect",this);
    reset          = new QAction(QIcon(":assets/reset.ico"),"Reset",this);
    diapoButton    = new QAction(QIcon(":assets/diaporama.ico"),"Diaporama",this);
    diapoTime      = new QAction(QIcon(":assets/timer.ico"),"Diaporama duration",this);
    randomImage    = new QAction(QIcon(":assets/random.ico"),"Random",this);
    info           = new QAction(QIcon(":assets/info.ico"),"Info",this);
    previousImage  = new QPushButton(QIcon(":assets/previous.ico"),"");
    nextImage      = new QPushButton(QIcon(":assets/next.ico"),"");
    imageLabel     = new QLabel(this);
    themeChoice    = new QComboBox(this);
}

// Building the qcombobox theme list.
void ImageViewer::buildThemeList()
{
    themeChoice->addItem("Amoled");
    themeChoice->addItem("Aqua");
    themeChoice->addItem("Console");
    themeChoice->addItem("Diffness");
    themeChoice->addItem("Dtor");
    themeChoice->addItem("Elegant Dark");
    themeChoice->addItem("Irrorater");
    themeChoice->addItem("Mac");
    themeChoice->addItem("Manjaro");
    themeChoice->addItem("Material Dark");
    themeChoice->addItem("Neon");
    themeChoice->addItem("Obit");
    themeChoice->addItem("Synet");
    themeChoice->addItem("Ubuntu");
    themeChoice->addItem("World");
    themeChoice->setCurrentText((imgViewerSettings.value("Theme").toString().isEmpty()) ? "Aqua" : imgViewerSettings.value("Theme").toString());
}

// Menu building.
void ImageViewer::buildMenu()
{
    myMenu = new QMenuBar(this);
    file->addAction(openImage);
    file->addAction(quit);
    zoom->addAction(minus);
    zoom->addAction(plus);
    zoom->addAction(reset);
    rotation->addAction(rotateDirect);
    rotation->addAction(rotateIndirect);
    rotation->addAction(reset);
    advanced->addAction(diapoButton);
    advanced->addAction(diapoTime);
    advanced->addAction(randomImage);
    myMenu->addMenu(file);
    myMenu->addSeparator();
    myMenu->addMenu(zoom);
    myMenu->addSeparator();
    myMenu->addMenu(rotation);
    myMenu->addSeparator();
    myMenu->addMenu(advanced);
    myMenu->addSeparator();
    myMenu->addAction(info);
}

// Shortcuts.
void ImageViewer::setShortcuts()
{
    openImage->setShortcut(QKeySequence::Open);
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
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    setStyleSheet(THEME_NAMES[themeChoice->currentText()]);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    previousImage->setDisabled(true);
    nextImage->setDisabled(true);
}

// Applying a layout to the mainwindow.
void ImageViewer::applyLayout()
{
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(myMenu,7);
    topLayout->addWidget(themeChoice);
    QHBoxLayout *buttonLay = new QHBoxLayout();
    buttonLay->setAlignment(Qt::AlignHCenter);
    buttonLay->addWidget(previousImage);
    buttonLay->addWidget(nextImage);
    QVBoxLayout *appLayout = new QVBoxLayout();
    appLayout->setContentsMargins(0,0,0,15);
    appLayout->addLayout(topLayout,2);
    appLayout->addWidget(imageLabel,16);
    appLayout->addLayout(buttonLay,2);
    auto central = new QWidget(this);
    central->setLayout(appLayout);
    setCentralWidget(central);
}

// Dialog file to choose the image to open
void ImageViewer::onDialogOpen()
{
    currentImageName = QFileDialog::getOpenFileName(this);
    if(currentImageName.isEmpty())
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
    QStringList partsOfPath = fileImage.split("/");
    QString directoryPath;
    for (auto i = 0;i < partsOfPath.length()-1;i++)
    {
        directoryPath.append((partsOfPath.at(i))+"/");
    }
    // Creating a QDir to store the current directory
    imageDirectory.setPath(directoryPath);
    fillNextElements();
    readImage(fileImage);
    setWindowTitle(fileImage);
}

// Zoom
void ImageViewer::onZoomPlus()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        scaleImage(12);
    }
}

void ImageViewer::onZoomMinus()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        scaleImage(0.8);
    }
}

void ImageViewer::onReset()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        zoomFactor = 1;
        readImage(currentImageName);
    }
}

// Scaling the image for the zoom.
void ImageViewer::scaleImage(double factor)
{
    zoomFactor *= factor;
    imageLabel->resize(zoomFactor*imageLabel->pixmap(Qt::ReturnByValue).size());
}

// Fill the list of next images in the directory
void ImageViewer::fillNextElements()
{
    nextImages     = {};
    previousImages = {};
    QDirIterator imageDirIt{imageDirectory};
    QString tmpImage;
    while(imageDirIt.hasNext() && (QFileInfo((tmpImage = imageDirIt.next())).isFile()))
    {
        if(tmpImage != currentImageName)
        {
            nextImages.push_back(tmpImage);
        }
    }
}

// Read an image in the imageLabel.
void ImageViewer::readImage(const QString &name)
{
    QImageReader reader(name);
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
    nextImage->setDisabled((nextImages.size() <= 0));
    previousImage->setDisabled((previousImages.size() <= 0));
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
        QImageReader reader(name);
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
        imageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    }
}

// Read the following image in the directory.
void ImageViewer::onNext()
{
    if(nextImages.size() > 0)
    {
        auto imgToRead{nextImages.takeFirst()};
        previousImages.push_front(currentImageName);
        readImage(imgToRead);
     }
}

// Read the previous image in the directory.
void ImageViewer::onPrevious()
{
    if(previousImages.size() > 0)
    {
        auto imgToRead{previousImages.takeFirst()};
        nextImages.push_front(currentImageName);
        readImage(imgToRead);
    }
}

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

// Diaporama.
void ImageViewer::onDiapo()
{
    startDiapo();
    QString tmpImageName;
    QDirIterator imgDirIterator{imageDirectory};
    QList<QString> imageList;
    while((imgDirIterator.hasNext()) && (QFileInfo(tmpImageName = imgDirIterator.next())).isFile())
    {
        imageList.push_back(tmpImageName);
    }
    foreach(auto tmp,imageList)
    {
        if(!isRunningDiapo) break;
        QTimer timer;
        QEventLoop loop; // Event loop to read the images during a time.
        connect(&timer,&QTimer::timeout,&loop,&QEventLoop::quit);
        timer.start(timeToWait);
        readImage(tmp);
        loop.exec();
        if(!isRunningDiapo) break;
    }
    endDiapo();
}

// Changing the duration of the diaporama
void ImageViewer::onDiapoTime()
{
    QMessageBox::information(this,"Diaporama duration","Choose the diaporama duration in seconds");
    QSpinBox *numberBox = new QSpinBox();
    numberBox->setRange(1,30);
    numberBox->setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QGuiApplication::primaryScreen()->availableGeometry()));
    numberBox->setFixedSize(100,100);
    connect(numberBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&ImageViewer::changeDiapoTime);
    numberBox->show();
}

// Updating the time
void ImageViewer::changeDiapoTime(int time)
{
    time         = std::abs(time);
    timeToWait   = static_cast<long>(time)*1000;
    imgViewerSettings.setValue("Time",QString::number(timeToWait));
}

// Start the diaporama.
void ImageViewer::startDiapo()
{
    nextImage->setVisible(false);
    previousImage->setVisible(false);
    myMenu->setVisible(false);
    isRunningDiapo = true;
    themeChoice->setVisible(false);
    setFullScreen(true);
}

// End the diaporama
void ImageViewer::endDiapo()
{
    nextImage->setVisible(true);
    previousImage->setVisible(true);
    myMenu->setVisible(true);
    themeChoice->setVisible(true);
    isRunningDiapo = false;
    setFullScreen(false);
}

void ImageViewer::onRandom()
{
    // We will get all the elements of the current directory.
    QDirIterator imgIterator{imageDirectory};
    QList<QString> otherImages;
    QString tmpName;
    while((imgIterator.hasNext()) && (QFileInfo(tmpName = imgIterator.next()).isFile()))
    {
        otherImages.push_back(tmpName);
    }
    // Get a random image.
    auto randomImage = otherImages[QRandomGenerator::global()->bounded(0,otherImages.size())];
    currentImageName = randomImage;
    readImage(randomImage);
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
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        // Extract the local paths of the files.
        currentImageName = urlList[0].toString().right(urlList[0].toString().length() - 7);
        onOpen(currentImageName);
    }
}

// Handling all key events.
void ImageViewer::keyPressEvent(QKeyEvent *e)
{
    isRunningDiapo = false;
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

// When the mouse is pressed, we leave the diapo
void ImageViewer::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
    isRunningDiapo = false;
}

// Applying the new theme with the theme name
void ImageViewer::onApplyOtherTheme(const QString &theme)
{
    imgViewerSettings.setValue("Theme",theme);
    setStyleSheet(THEME_NAMES[theme]);
}

void ImageViewer::setFullScreen(bool ok)
{
    appIsFullScreen = ok;
    (appIsFullScreen) ? showFullScreen() : showNormal();
}

void ImageViewer::showInfo()
{
    if(img.isNull()) return;
    ImageInfo *infoDialog = new ImageInfo(this,img,currentImageName);
    infoDialog->show();
}

ImageViewer::~ImageViewer()
{}
