#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtCharts>

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
    /// \returns \c void
    /// \since MNK2_1
    void exitButtonClicked();

private:
    Ui::MainWindow *ui;
    QChartView *chartView = nullptr;//will be changed on fitButtonClicked();
};
#endif // MAINWINDOW_H
