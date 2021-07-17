#ifndef QIMAGEVIEWER_RECENTFILESMANAGER_HPP
#define QIMAGEVIEWER_RECENTFILESMANAGER_HPP

#include <QList>
#include <QSettings>

class RecentFilesManager
{
    public:
        RecentFilesManager();
        QList<QVariant> recentFiles() const;
        void addRecentFile(const QString &filePath);
        const QString RECENT_FILES = "recent files";

    private:
        QList<QVariant> recentFilesList;
        QSettings settings;
};

#endif //QIMAGEVIEWER_RECENTFILESMANAGER_HPP