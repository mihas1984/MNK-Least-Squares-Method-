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
    ///
    /// \brief calculateClicked()
    /// triggers read(), calculate() and outputs to ui results
    /// calculates mean, error, median, range, observations number, and gets Student's coefficients from student_coeffs.hpp
    /// \return 0 if okay, -1 if errors occured
    int calculateClicked();
    ///
    /// \brief exitClicked()
    /// closes current window
    void exitClicked();
signals:
    ///
    /// \brief closes()
    ///emitted to mainwindow, when this dialog closes
    void closes();
private:
    ///
    /// \brief read()
    /// reads data samples from textEdit
    /// \param input
    /// \return 0 if sucessful, -1 if non-numeric data was found
    int read(std::vector<double>& input);
    ///
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
    int calculate(const std::vector<double> input, double &avg, double &avg_err, double &student, double& min, double& max, double& median);
    ///
    /// \brief closeEvent()
    /// intercepted to emit closes() signal to MainWindow
    /// \param event - close event (will be accepted)
    void closeEvent(QCloseEvent* event);

    Ui::SkvWindow *ui;
};

#endif // SKVWINDOW_H
