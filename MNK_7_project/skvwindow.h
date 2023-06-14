#ifndef SKVWINDOW_H
#define SKVWINDOW_H

#include <QMainWindow>
#include <vector>

namespace Ui {
class SkvWindow;
}

class SkvWindow : public QMainWindow
{
    Q_OBJECT

public:
    SkvWindow(QWidget *parent = nullptr);
    ~SkvWindow();
private slots:
    /// \brief calculateClicked()
    /// triggers read(), calculate() and outputs to ui results
    /// calculates mean, error, median, range, observations number, and gets Student's coefficients from student_coeffs.hpp
    /// \return 0 if okay, -1 if errors occured
    int calculateClicked();
    /// \brief exitClicked()
    /// closes current window
    void exitClicked();
signals:
    /// \brief closes()
    ///emitted to mainwindow, when this dialog closes
    void closes();
private:
    /// \brief eventFilter()
    /// when mouse is on label_2 or on tipWidget it shows tipWidget, otherwise not showing
    /// \param obj - either label_2 or tipWidget
    /// \param ev - event from them (QEvent::HoverEnter or QEvent::HoverLeave)
    /// \return true if the events are entering and leaving label_2 or tipWidget, false otherwise
    bool eventFilter(QObject *obj, QEvent *ev) override;
    ///
    /// \brief resizeEvent
    /// resizes mainWidget to fill the skvwindow, beacuse there are no layouts for possibility of creating elements outside the layout
    /// \param event - resizing event of skvwindow
    void resizeEvent(QResizeEvent *event);
    /// \brief read()
    /// reads data samples (numbers) from textEdit and confidence interval from combobox
    /// \param input - vector to be filled with data
    /// \param confidence_interval - value from combobox to be read
    /// \return 0 if sucessful, -1 if non-numeric data was found
    int read(std::vector<double>& input, double& confidence_interval);
    /// \brief calculate()
    /// calculates mean, error, median, range and observations number based on the data read
    /// shows error and ends the function when there are less than 2 numbers recorded
    /// \param input - array of data
    /// \param avg - mean
    /// \param avg_err - error for result
    /// \param student - Student's coefficient
    /// \param min - minimum element of the range
    /// \param max - maximum element of the range
    /// \param median
    /// \return 0 if okay, -1 if less than 2 data points recorded
    int calculate(const std::vector<double> input, const double confidence_interval, double &avg, double &avg_err, double &student, double& min, double& max, double& median);
    ///
    /// \brief closeEvent()
    /// intercepted to emit closes() signal to MainWindow
    /// \param event - close event (will be accepted)
    void closeEvent(QCloseEvent* event);

    Ui::SkvWindow *ui;
    bool isTipFocused{false};
    bool isLabelFocused{false};
};

#endif // SKVWINDOW_H
