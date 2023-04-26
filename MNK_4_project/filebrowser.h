#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QDialog>
#include <QtCore>
#include <QtGui>

namespace Ui {
class FileBrowser;
}

class FileBrowser : public QDialog
{
    Q_OBJECT

public:
    explicit FileBrowser(QWidget *parent = nullptr);
    ~FileBrowser();
private slots:
    /// \fn treeViewClicked(const QModelIndex& index)
    /// \brief sets the filemodel's base path to the folder clicked on
    /// \param index - the index of a folder we clicked on that we get
    /// \since MNK3_0
    void treeViewClicked(const QModelIndex& index);
    /// \fn listViewClicked(const QModelIndex& index)
    /// \brief sets the \variable chosen_file_path to the path of a file we clicked on
    /// \param index - the index of a file we clicked on that we get
    /// \since MNK3_0
    void listViewClicked(const QModelIndex& index);
    /// \fn cancelButtonClicked()
    /// \brief closes the FileBrowser window with the resulting code "Rejected"
    /// \since MNK3_0
    void cancelButtonClicked();
    /// \fn chooseButtonClicked()
    /// \brief if the \variable chosen_file_path is populated emits \fn sendPath(const QString& path)
    /// and closes the FileBrowser window with the resulting code "Accepted"
    /// \since MNK3_0
    void chooseButtonClicked();
signals:
    /// \fn sendPath(const QString& path)
    /// \brief is a signal, for sending \variable chosen_file_path from the FileBrowser window to the MainWindow.
    /// connected to slot \fn recievePath(QString) in \file mainwindow.cpp
    /// \param path
    /// \since MNK3_0
    void sendPath(const QString& path);
private:
    Ui::FileBrowser *ui;
    QFileSystemModel* dirmodel = nullptr;
    QFileSystemModel* filemodel = nullptr;
    QString chosen_file_path{""};
};

#endif // FILEBROWSER_H
