#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <fstream>
#include <vector>
#include <QtWidgets/QMainWindow>
#include <QtCharts>
#include <QLabel>
#include "readfile.hpp"
#include "mnk.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setting texts and titles:
    this->setWindowTitle("Least Squares");
    ui->fitButton->setText("FIT");
    ui->exitButton->setText("EXIT");
    ui->lineEdit->setPlaceholderText("prompt");
    ui->promptLabel->setText("browse to find the file with the data\n(or write the path to it by hand)");
    ui->dataLabel->setText("number\t\tX\t\tY");
    //for red exit button:
    QPalette Pal(palette());
    Pal.setColor(QPalette::Button, QColor(53,0,0));
    ui->exitButton->setAutoFillBackground(true);
    ui->exitButton->setPalette(Pal);
    //connecting:
    connect(ui->fitButton, &QPushButton::clicked, this, &MainWindow::fitButtonClicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::exitButtonClicked);
    connect(ui->browserButton, &QPushButton::clicked, this, &MainWindow::browseButtonClicked);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::browseButtonClicked(){
    filebrowser = new FileBrowser(this);
    filebrowser->setAttribute(Qt::WA_DeleteOnClose);
    filebrowser->setModal(true);//it is important that it is modal
    filebrowser->setWindowTitle("file browser");
    filebrowser->show();
}
void MainWindow::exitButtonClicked(){
    QMessageBox::StandardButton reply = QMessageBox::question(this, "please resubmit", "do you wanna quit?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        QCoreApplication::quit();//exiting whole app
    }
}
void MainWindow::recievePath(const QString& path)
{
    ui->lineEdit->setText(path);
}
int MainWindow::fitButtonClicked(){
    std::vector<double> vecX;//vectors will contain coordinates from the data file
    std::vector<double> vecY;
    std::ifstream fin;
    std::string path{""};
    path = ui->lineEdit->text().toStdString();
    fin.open(path);
    std::string errorString { "" };
    //error check for readData():
    if(readData(vecX, vecY, fin, errorString)){
        QMessageBox::warning(this, "ERROR", QString::fromStdString(errorString));
        return -1;//end of function execution
    }
    double slope, intercept;
    //error check for computate():
    if(computate(vecX, vecY, slope, intercept, errorString)){
        QMessageBox::warning(this, "ERROR", QString::fromStdString(errorString));
        return -1;//end of function execution
    }
    //working with textBrowser:
    ui->textBrowser->clear();//clearing before appending
    QString line{""};
    for(int i = 0; i < (int)vecX.size(); i++){
        line = QString::number(i + 1) + "\t" + QString::number(vecX.at(i)) + "\t" + QString::number(vecY.at(i));
        std::string deliminator (40, '-');
        ui->textBrowser->append(line);//appends "{number of data point}\t{x}\t{y}"
        ui->textBrowser->append(QString::fromStdString(deliminator));//appends "----------"
    }
    ui->textBrowser->scrollToAnchor("any_anchor");//scrolls texBrowser to the upper bound
    //working with chart from now on:
    QChart *chart = new QChart();
    chart->setAttribute(Qt::WA_DeleteOnClose);
    QLineSeries *lineSeries = new QLineSeries(chart);//approximation
    lineSeries->setName("approximation");
    for(int i = 0; i < (int)vecX.size(); i++){
        lineSeries->append(vecX.at(i),vecX.at(i) * slope + intercept);
    }
    QScatterSeries *scatterSeries = new QScatterSeries(chart);//experimental data points
    scatterSeries->setName("data points");
    scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    scatterSeries->setMarkerSize(10.0);
    scatterSeries->setColor(QRgb(0xFA9191));//light-red
    for(int i = 0; i < (int)vecX.size(); i++){
        scatterSeries->append(vecX.at(i),vecY.at(i));
    }
    chart->addSeries(lineSeries);
    chart->addSeries(scatterSeries);
    chart->createDefaultAxes();
    // Customize title as an equation
    const QString chartTitle = QString::fromStdString("equation: y = "+std::to_string(slope)+" * x + "+std::to_string(intercept));
    QFont titleFont;
    titleFont.setPixelSize(18);
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(QBrush(Qt::darkRed));
    chart->setTitle(chartTitle);
    // Customize series
    QPen lineSPen(QRgb(0x000000));//black
    lineSPen.setWidth(5);
    lineSeries->setPen(lineSPen);
    QPen scatterSPen(QRgb(0x822828));//dark-red
    scatterSPen.setWidth(0);
    scatterSeries->setPen(scatterSPen);
    chart->setAnimationOptions(QChart::AllAnimations);
    // Customize chart background
    QLinearGradient backgroundGradient;
    backgroundGradient.setStart(QPointF(0, 0));
    backgroundGradient.setFinalStop(QPointF(0, 1));
    backgroundGradient.setColorAt(0.0, QRgb(0xd2d0d1));//grey
    backgroundGradient.setColorAt(1.0, QRgb(0x4c4547));//dark-grey
    backgroundGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    chart->setBackgroundBrush(backgroundGradient);
    // Customize plot area background
    QLinearGradient plotAreaGradient;
    plotAreaGradient.setStart(QPointF(0, 1));
    plotAreaGradient.setFinalStop(QPointF(1, 0));
    plotAreaGradient.setColorAt(0.0, QRgb(0x555555));//grey
    plotAreaGradient.setColorAt(1.0, QRgb(0xaa5555));//red
    plotAreaGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    chart->setPlotAreaBackgroundBrush(plotAreaGradient);
    chart->setPlotAreaBackgroundVisible(true);
    // Customize axis colors
    QPen axisPen(QRgb(0x642828));//a bit darker dark-red
    axisPen.setWidth(2);
    chart->axes().at(0)->setLinePen(axisPen);//X-axis
    chart->axes().at(1)->setLinePen(axisPen);//Y-axis
    QBrush axisBrush(Qt::white);
    chart->axes().at(0)->setLabelsBrush(axisBrush);//X-axis
    chart->axes().at(1)->setLabelsBrush(axisBrush);//Y-axis
    //reloading the chart
    if(chartView){// won't delete anything on the first run (nothing to delete)
        chartView->chart()->close();
        chartView->close();
    }else{
        ui->horizontalSpacer->changeSize(5,5,QSizePolicy::Minimum,QSizePolicy::Minimum);// shift the spacer in favour of the chart on the first run
    }
    chartView = new QChartView(chart);
    chartView->setAttribute(Qt::WA_DeleteOnClose);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->horizontalLayout3->addWidget(chartView);
    return 0;
}



