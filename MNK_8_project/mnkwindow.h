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
    void readData(QVector<QString> &varNames, QVector<QString> &varUnits, QVector<long double> &vecX, QVector<long double> &vecY, QVector<long double> &vecXErrs, QVector<long double> &vecYErrs);
signals:
    /// \brief closes()
    ///emitted to mainwindow, when this dialog closes
    void closes();
private slots:
    /// \fn fitButtonClicked()
    /// \brief invokes functions read_data() (from above) and  computation functions from mnk.hpp
    /// via data points from the tableView creates a chart
    /// on which there are experimantal data points and linear approximation (along with optional error bars)
    /// \returns 0 when work is correct, -1 when there are error flags with data
    int fitButtonClicked();
    /// \fn exitButtonClicked()
    /// \brief creates a messageBox for saving unsaved changes and quits
    void exitButtonClicked();
    /// \brief addRow()
    /// adds one more row to the tableView
    void addRow();
    /// \brief onModified()
    /// changes status of file to modified when changes are made to the table data or radiobuttons
    void onModified();
    /// \brief saveCsv()
    /// if current file is specified calls saveCsvGeneralFunction(), otherwise calls saveAsCsv()
    void saveCsv();
    /// \brief saveAsCsv()
    /// calls saveCsvGeneralFunction(), (has checks for correctness)
    void saveAsCsv();
    /// \brief openCsv()
    /// reads created csv files (separator is ";"), there shoud be two words per line, third line specifies errors
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
    /// shows lineEdit beneath for inputting fixed, when toggled off - hides
    void fixedXErrsToggled();
    /// \brief allYErrsToggled()
    /// when toggled on shows delta Y column for errors, when toggled off hides that column
    void allYErrsToggled();
    /// \brief noYErrsToggled()
    /// default configuration for errors
    void noYErrsToggled();
    /// \brief fixedYErrsToggled()
    /// shows lineEdit beneath for inputting fixed, when toggled off - hides
    void fixedYErrsToggled();
    /// \brief fixedRelativeXErrsToggled()
    /// shows lineEdit beneath for inputting fixed relative errors (accepted value in range 0 - 1), when toggled off - hides
    void fixedRelativeXErrsToggled();
    /// \brief fixedRelativeYErrsToggled()
    /// shows lineEdit beneath for inputting fixed relative errors (accepted value in range 0 - 1), when toggled off - hides
    void fixedRelativeYErrsToggled();
    /// \brief darkThemePressed()
    /// if not set sets dark widget palette and fusion style, and m_theme to "dark"
    /// also sets chart colors to dark ones
    void darkThemePressed();
    /// \brief lightThemePressed()
    /// if not set sets default widget palette and windows vista style, and m_theme to "light"
    /// also sets chart colors to light ones
    void lightThemePressed();
    /// \brief setLightThemeChart
    /// sets chart colors to light ones, called from lightThemePressed() and if "light" is set in m_theme creates light colored charts from now on
    void setLightThemeChart();
    /// \brief setDarkThemeChart
    /// sets chart colors to dark ones, called from darkThemePressed() and if "dark" is set in m_theme creates dark colored charts from now on
    void setDarkThemeChart();
    /// \brief cellsFocusedChanged()
    /// calls addRow() when among selected items from \param selected there are ones from the last row in the model
    /// \param selected - will serve as kind of a list of selected items
    /// \param deselected - ignored
    void cellsFocusedChanged(const QItemSelection & selected, const QItemSelection & deselected);
private:
    /// \brief closeEvent()
    /// intercepted to emit closes() signal to MainWindow. sets style back to dark one
    /// \param event - close event (will be accepted)
    void closeEvent(QCloseEvent* event);
    /// \brief resizeEvent()
    /// resizes mainWidgetForMNK to fill the window, beacuse there are no layouts for possibility of creating elements outside the layout
    /// (such a construction created for keeping tip widgets above all layouts without losing layouts' advantages)
    /// \param event - resizing event of MnkWindow
    void resizeEvent(QResizeEvent *event);
    /// \brief eventFilter()
    /// when mouse is on hint labels it shows personalized tipWidget, otherwise not showing
    /// \param obj - labelAllXErrsHint, or labelNoXErrsHint, or labelFixedRelativeXErrsHint, or labelFixedXErrsHint
    /// \param ev - event from them (QEvent::HoverEnter or QEvent::HoverLeave)
    /// \return true if the events are entering and leaving hint labels, false otherwise
    bool eventFilter(QObject *obj, QEvent *ev) override;

    Ui::MnkWindow *ui;
    QStandardItemModel* m_model{};//model for tableView
    QCustomPlot* m_chart{nullptr};
    QString m_xErrCurrentlyChecked{"noXErrs"};
    QString m_yErrCurrentlyChecked{"noYErrs"};
    QString m_theme{"dark"};
    QString m_currentFilepath{""};//current .csv file we're working with
    QString m_currentFileStatus{""};
};

#endif // MNKWINDOW_H
