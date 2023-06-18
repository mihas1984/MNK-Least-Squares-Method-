#ifndef MNKWINDOW_H
#define MNKWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "qcustomplot.h"

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
    /// reads from tableView and radiobuttons to arrays for further processing
    /// \param varNames - array of 2 elements will contain name for X and for Y
    /// \param varUnits - array of 2 elements will contain units names for X and for Y
    /// \param vecX - will contain X values of data points from table
    /// \param vecY - will contain Y values of data points from table
    /// \param vecXErrs - will contain X error values of data points from table
    /// \param vecYErrs - will contain Y error values of data points from table
    ///
    void readData(QVector<QString> &varNames, QVector<QString> &varUnits, QVector<long double> &vecX, QVector<long double> &vecY, QVector<long double> &vecXErrs, QVector<long double> &vecYErrs);
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
    /// changes label status of file to modified when changes are made to the table data or radiobuttons
    void onModified();
    /// \brief saveCsv()
    /// if current file is specified calls saveCsvGeneralFunction(), otherwise calls saveAsCsv()
    void saveCsv();
    /// \brief saveAsCsv()
    /// calls saveCsvGeneralFunction(), (has checks for correctness)
    void saveAsCsv();
    /// \brief openCsv()
    /// reads created csv files (separator is ";"), there shoud be two words per line, first line specifies errors
    /// outputs to the tableView
    void openCsv();
    /// \brief closeCsv()
    /// closes "file connection" sets TableView to defaults along with resetting chart if awailable
    void closeCsv();
    /// \brief screenCapture()
    /// makes a creenshot of app window and saves as .png file defined by user
    void screenCapture();
    /// \brief chartScreenShot()
    /// saves chart as .png to a file defined by user
    void chartScreenShot();
    /// \brief CreatedefaultModel()
    /// creates table "X-Y-deltaX-deltaY;Name-Units-1-2" 4x4 and hides delta columns by default
    void CreatedefaultModel();
    /// \brief saveCsvGeneralFunction()
    /// reads data from tableView and errors from radio buttons and inputs it to .csv file chosen by user (creates file)
    /// \return 0 if ok, -1 if file couldn't be opened
    int saveCsvGeneralFunction();
    /// \brief allXErrsToggled()
    /// when toggled on shows delta X column for errors, when toggled off hides that column
    void allXErrsToggled();
    /// \brief noXErrsToggled()
    /// default configuration for errors
    void noXErrsToggled();
    /// \brief fixedXErrsToggled()
    /// enables lineEdit beneath for inputting fixed, when toggled off - disables
    void fixedXErrsToggled();
    /// \brief allYErrsToggled()
    /// when toggled on shows delta Y column for errors, when toggled off hides that column
    void allYErrsToggled();
    /// \brief noYErrsToggled()
    /// default configuration for errors
    void noYErrsToggled();
    /// \brief fixedYErrsToggled()
    /// enables lineEdit beneath for inputting fixed, when toggled off - disables
    void fixedYErrsToggled();
private:
    /// \brief closeEvent()
    /// intercepted to emit closes() signal to MainWindow
    /// \param event - close event (will be accepted)
    void closeEvent(QCloseEvent* event);

    Ui::MnkWindow *ui;
    QString m_currentFilepath{""};//current .csv file we're working with
    QStandardItemModel* m_model{};//model for tableView
    QCustomPlot* m_chart{nullptr};
    QString m_xErrCurrentlyChecked{"noXErrs"};
    QString m_yErrCurrentlyChecked{"noYErrs"};
};

#endif // MNKWINDOW_H
