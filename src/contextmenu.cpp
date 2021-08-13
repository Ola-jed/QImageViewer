#include "contextmenu.hpp"

ContextMenu::ContextMenu(const QString &title, QWidget *parent) : QMenu(title, parent)
{
    buildActions();
    addActions();
    makeConnections();
}

/// Make the connections
void ContextMenu::makeConnections()
{
    connect(open, &QAction::triggered, this, &ContextMenu::imageOpenRequested);
    connect(save, &QAction::triggered, this, &ContextMenu::saveRequested);
    connect(previous, &QAction::triggered, this, &ContextMenu::previousRequested);
    connect(next, &QAction::triggered, this, &ContextMenu::nextRequested);
    connect(slideshow, &QAction::triggered, this, &ContextMenu::slideShowRequested);
}

/// Build actions list
void ContextMenu::buildActions()
{
    open      = new QAction(QIcon(":assets/open.ico"), "Open", this);
    save      = new QAction(QIcon(":assets/save.ico"), "Save", this);
    previous  = new QAction(QIcon(":assets/previous.ico"), "Previous", this);
    next      = new QAction(QIcon(":assets/next.ico"), "Next", this);
    slideshow = new QAction(QIcon(":assets/slideshow.ico"), "Slideshow", this);
}

/// Add all actions
void ContextMenu::addActions()
{
    addAction(open);
    addAction(save);
    addAction(next);
    addAction(previous);
    addAction(slideshow);
}