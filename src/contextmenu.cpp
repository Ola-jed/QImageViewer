#include "contextmenu.hpp"

ContextMenu::ContextMenu(const QString &title, QWidget *parent) : QMenu(title, parent)
{
    buildActions();
    addActions();
    makeConnections();
}

void ContextMenu::makeConnections()
{
    connect(open,&QAction::triggered,this,&ContextMenu::imageOpenRequested);
    connect(save,&QAction::triggered,this,&ContextMenu::saveRequested);
    connect(previous,&QAction::triggered,this,&ContextMenu::previousRequested);
    connect(next,&QAction::triggered,this,&ContextMenu::nextRequested);
    connect(slideshow,&QAction::triggered,this,&ContextMenu::slideShowRequested);
}

void ContextMenu::buildActions()
{
    open      = new QAction(QIcon(":assets/open.ico"),"Open");
    save      = new QAction(QIcon(":assets/save.ico"),"Save");
    previous  = new QAction(QIcon(":assets/previous.ico"),"Previous");
    next      = new QAction(QIcon(":assets/next.ico"),"Next");
    slideshow = new QAction(QIcon(":assets/diaporama.ico"),"Slideshow");
}

void ContextMenu::addActions()
{
    addAction(open);
    addAction(save);
    addAction(next);
    addAction(previous);
    addAction(slideshow);
}