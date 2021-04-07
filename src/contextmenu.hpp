#ifndef QIMAGEVIEWER_CONTEXTMENU_HPP
#define QIMAGEVIEWER_CONTEXTMENU_HPP

#include <QMenu>
#include <QAction>

class ContextMenu : public QMenu
{
    Q_OBJECT

    public:
        explicit ContextMenu(const QString &title, QWidget *parent = nullptr);

    private:
        QAction *open;
        QAction *save;
        QAction *previous;
        QAction *next;
        QAction *slideshow;
        void makeConnections();
        void buildActions();
        void addActions();

    signals:
        void imageOpenRequested();
        void saveRequested();
        void previousRequested();
        void nextRequested();
        void slideShowRequested();
};


#endif //QIMAGEVIEWER_CONTEXTMENU_HPP
