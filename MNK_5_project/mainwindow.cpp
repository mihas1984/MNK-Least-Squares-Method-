#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include "helpdialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //for red exit button:
    QPalette Pal(palette());
    Pal.setColor(QPalette::Button, QColor(53,0,0));
    ui->exitButton->setAutoFillBackground(true);
    ui->exitButton->setPalette(Pal);
    //connecting:
    connect(ui->mnkButton, &QPushButton::clicked, this, &MainWindow::mnkClicked);
    connect(ui->skvButton, &QPushButton::clicked, this, &MainWindow::skvClicked);
    connect(ui->helpButton, &QPushButton::clicked, this, &MainWindow::helpClicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::exitClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mnkClicked()
{
    mnkWindow = new MnkWindow();//not a child by the matter of fact, but treated as such, deletes on close
    connect(mnkWindow, SIGNAL(closes()), this, SLOT(recieveChildClosed()));
    mnkWindow->setWindowTitle("MNK");
    mnkWindow->setWindowFlags(mnkWindow->windowFlags() | Qt::Window);
    mnkWindow->show();
    hide();
}

void MainWindow::skvClicked()
{
    skvWindow = new SkvWindow();//not a child by the matter of fact, but treated as such, deletes on close
    connect(skvWindow, SIGNAL(closes()), this, SLOT(recieveChildClosed()));
    skvWindow->setWindowTitle("SKV");
    skvWindow->setWindowFlags(skvWindow->windowFlags() | Qt::Window);
    skvWindow->show();
    hide();
}

void MainWindow::helpClicked()
{
    helpDialog = new HelpDialog();//not a child by the matter of fact, but treated as such, deletes on close
    connect(helpDialog, SIGNAL(closes()), this, SLOT(recieveChildClosed()));
    helpDialog->setWindowTitle("HELP");
    helpDialog->setWindowFlags(helpDialog->windowFlags() | Qt::Window);
    helpDialog->show();
    hide();

}

void MainWindow::exitClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "please resubmit", "do you want to quit?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        QCoreApplication::quit();//exiting whole app
    }
}

void MainWindow::recieveChildClosed()
{
    this->show();
}

