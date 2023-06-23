#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mnkwindow.h"
#include "skvwindow.h"
#include "helpdialog.h"
#include <QTranslator>



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
    /// \brief mnkClicked()
    /// creates instance of MnkWindow and hides itself
    void mnkClicked();
    /// \brief skvClicked()
    /// creates instance of SkvWindow and hides itself
    void skvClicked();
    /// \brief helpClicked
    /// creates instance of HelpDialog and hides itself
    void helpClicked();
    /// \brief exitClicked()
    /// exits the whole application
    void exitClicked();
    /// \brief recieveChildClosed()
    /// when one of MnkWindow, SkvWindow or HelpDialog closes (we know because we recieve signal close()
    /// from those windows), MainWindow shows itself
    void recieveChildClosed();
    /// \brief translateFunction()
    /// loads ukrainian translations for the whole app when "UA" in the combobox is checked, and unloads otherwise
    void translateFunction();
private:
    Ui::MainWindow *ui;
    QTranslator* m_translator{nullptr};//custom translations
    QTranslator* m_systemTranslator{nullptr};//system translations
    //child windows:
    MnkWindow* m_mnkWindow{nullptr};
    SkvWindow* m_skvWindow{nullptr};
    HelpDialog* m_helpDialog{nullptr};
};
#endif // MAINWINDOW_H
