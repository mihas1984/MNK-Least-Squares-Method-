#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//here:
#include "secondwindow.h"
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//added:
public slots: // notice this. it makes it a slot
    void buttonClicked(); // to be called when clicked
private slots:
    //void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    //here:
    QPushButton *button=NULL; // the button must be a member to be used from other functions in mainwin
    QLineEdit *lineEdit=NULL;
    //QLineEdit *l=NULL;
    SecondWindow *secwin;
};
#endif // MAINWINDOW_H
