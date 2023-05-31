#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mnkwindow.h"
#include "skvwindow.h"
#include "helpdialog.h"

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
    ///
    /// \brief mnkClicked()
    /// creates instance of MnkWindow and hides itself
    void mnkClicked();
    ///
    /// \brief skvClicked()
    /// creates instance of SkvWindow and hides itself
    void skvClicked();
    ///
    /// \brief helpClicked
    /// creates instance of HelpDialog and hides itself
    void helpClicked();
    ///
    /// \brief exitClicked()
    /// exits the whole application
    void exitClicked();
    ///
    /// \brief recieveChildClosed
    /// when one of MnkWindow, SkvWindow or HelpDialog closes (we know because we recieve signal close()
    /// from those windows), MainWindow shows itself
    void recieveChildClosed();
private:
    Ui::MainWindow *ui;
    MnkWindow* mnkWindow{nullptr};
    SkvWindow* skvWindow{nullptr};
    HelpDialog* helpDialog{nullptr};
};
#endif // MAINWINDOW_H
