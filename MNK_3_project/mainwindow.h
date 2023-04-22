#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtCharts>
#include "filebrowser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    /// \fn fitButtonClicked()
    /// \brief invokes functions \fn read_data() in file \file readfile.hpp and \fn computate() in \file mnk.hpp
    /// via data points from the file (path to which is taken from lineEdit)
    /// creates a chart on which there are experimantal data points and linear approximation
    /// \returns \c 0 when work is correct, \c -1 when there are error flags with data
    /// \since MNK1_4
    int fitButtonClicked();
    /// \fn exitButtonClicked()
    /// \brief creates a messageBox for resubmition of quitting the app and stops the app if \c "YES" is pressed
    /// \since MNK2_1
    void exitButtonClicked();
    /// \fn browseButtonClicked()
    /// \brief opens a modal FileBrowser window
    /// \since MNK3_0
    void browseButtonClicked();
    /// \fn recievePath(const QString& path)
    /// \brief is a slot, for recieving path variable from the FileBrowser window. connected to signal \fn sendPath(QString) in \file filebrowser.cpp
    /// also sets the text of a lineEdit as that path
    /// \param path - a chosen path in the FileBrowser
    /// \since MNK3_0
    void recievePath(const QString& path);
private:
    Ui::MainWindow *ui;
    QChartView *chartView = nullptr;
    FileBrowser* filebrowser = nullptr;
};
#endif // MAINWINDOW_H
