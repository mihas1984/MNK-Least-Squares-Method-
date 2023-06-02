#ifndef MNKWINDOW_H
#define MNKWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QCloseEvent>

namespace Ui {
class MnkWindow;
}

class MnkWindow : public QMainWindow
{
    Q_OBJECT

public:
    MnkWindow(QWidget *parent = nullptr);
    ~MnkWindow();
    /// \brief readData()
    /// reads names, units and data points from tableView to vectors for further processing
    /// \param varNames - array of 2 strings representing names for X and Y variables
    /// \param varUnits - array of 2 strings representing units' names for X and Y variables
    /// \param vecX - array of x values of data points
    /// \param vecY - array of y values of data points
    void readData(std::vector<std::string>& varNames, std::vector<std::string>& varUnits, std::vector<long double>& vecX, std::vector<long double>& vecY);
signals:
    /// \brief closes()
    ///emitted to mainwindow, when this dialog closes
    void closes();
private slots:
    /// \fn fitButtonClicked()
    /// \brief invokes functions \fn read_data() and \fn computate() in \file mnk.hpp
    /// via data points from the tableView
    /// creates a chart on which there are experimantal data points and linear approximation
    /// \returns \c 0 when work is correct, \c -1 when there are error flags with data
    int fitButtonClicked();
    /// \fn exitButtonClicked()
    /// \brief creates a messageBox for saving unsaved changes
    void exitButtonClicked();
    /// \brief addRowClicked()
    /// adds one more row to the tableView
    void addRowClicked();
    /// \brief onModified()
    /// changes label status of file to modified when changes are made to the table data
    void onModified();
    /// \brief saveCsv()
    /// reads from tableView and inputs it to current file, otherwise calls saveAsCsv()
    void saveCsv();
    /// \brief saveAsCsv()
    /// reads from tableView and inputs it to file chosen by user (creates file)
    void saveAsCsv();
    /// \brief openCsv()
    /// reads created csv files (separator is ";"), there shoud be two words per line
    /// outputs to the tableView
    void openCsv();
    /// \brief closeCsv()
    /// closes "file connection" sets TableView to defaults along with resetting chart if awailable
    void closeCsv();
    /// \brief screenCapture()
    /// makes a creenshot of app window and saves as .png file defined by user
    void screenCapture();
    /// \brief CreatedefaultModel()
    /// creates table "X-Y-Name-Units" 2x2 plus adding 2 numbered rows
    void CreatedefaultModel();
private:
    /// \brief closeEvent()
    /// intercepted to emit closes() signal to MainWindow
    /// \param event - close event (will be accepted)
    void closeEvent(QCloseEvent* event);
private:
    Ui::MnkWindow *ui;
    QChartView *chartView = nullptr;//chart
    QString currentFilepath{""};//current .csv file we're working with
    QStandardItem* ItemX{};//for reading items from tableView
    QStandardItem* ItemY{};//for reading items from tableView
    QStandardItemModel *model{};//model for tableView
    int currentRowNumber{1};//used for setting row Names
};

#endif // MNKWINDOW_H
