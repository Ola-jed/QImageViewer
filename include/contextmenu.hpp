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
    QAction *next;
    QAction *previous;
    QAction *slideshow;
    void addActions();
    void buildActions();
    void makeConnections();

signals:
    void nextRequested();
    void saveRequested();
    void previousRequested();
    void imageOpenRequested();
    void slideShowRequested();
};


#endif //QIMAGEVIEWER_CONTEXTMENU_HPP
