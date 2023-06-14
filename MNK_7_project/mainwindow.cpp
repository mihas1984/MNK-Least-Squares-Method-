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
    m_mnkWindow = new MnkWindow();//not a child by the matter of fact, but treated as such, deletes on close
    connect(m_mnkWindow, SIGNAL(closes()), this, SLOT(recieveChildClosed()));
    m_mnkWindow->setWindowTitle("MNK");
    m_mnkWindow->setWindowFlags(m_mnkWindow->windowFlags() | Qt::Window);
    m_mnkWindow->show();
    hide();
}

void MainWindow::skvClicked()
{
    m_skvWindow = new SkvWindow();//not a child by the matter of fact, but treated as such, deletes on close
    connect(m_skvWindow, SIGNAL(closes()), this, SLOT(recieveChildClosed()));
    m_skvWindow->setWindowTitle("SKV");
    m_skvWindow->setWindowFlags(m_skvWindow->windowFlags() | Qt::Window);
    m_skvWindow->show();
    hide();
}

void MainWindow::helpClicked()
{
    m_helpDialog = new HelpDialog();//not a child by the matter of fact, but treated as such, deletes on close
    connect(m_helpDialog, SIGNAL(closes()), this, SLOT(recieveChildClosed()));
    m_helpDialog->setWindowTitle("HELP");
    m_helpDialog->setWindowFlags(m_helpDialog->windowFlags() | Qt::Window);
    m_helpDialog->show();
    hide();

}

void MainWindow::exitClicked()
{
    QCoreApplication::quit();//exiting whole app
}

void MainWindow::recieveChildClosed()
{
    this->show();
}
