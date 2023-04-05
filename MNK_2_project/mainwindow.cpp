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
    this->setWindowTitle("Least Squares");
    ui->fitButton->setText("FIT");
    ui->exitButton->setText("EXIT");
    QPalette Pal(palette());//for red exit button
    Pal.setColor(QPalette::Button, QColor(53,0,0));
    ui->exitButton->setAutoFillBackground(true);
    ui->exitButton->setPalette(Pal);
    connect(ui->fitButton, &QPushButton::clicked, this, &MainWindow::fitButtonClicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::exitButtonClicked);
    ui->lineEdit->setPlaceholderText("prompt");
    ui->promptLabel->setText("input a path\nto the file with data:");
    ui->dataLabel->setText("number\t\tX\t\tY");
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::exitButtonClicked(){
    QMessageBox::StandardButton reply = QMessageBox::question(this, "please resubmit", "do you wanna quit?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        QCoreApplication::quit();
    }
}
int MainWindow::fitButtonClicked(){
    std::vector<double> vecX;//vectors will contain coordinates from the data file
    std::vector<double> vecY;
    std::ifstream fin;
    std::string path{""};
    path = ui->lineEdit->text().toStdString();
    fin.open(path);
    std::string errorString { "" };
    readData(vecX, vecY, fin, errorString);
    //error check
    if(errorString != ""){
        QMessageBox::warning(this, "ERROR", QString::fromStdString(errorString));
        return -1;//end of function execution
    }
    double slope, intercept;
    computate(vecX, vecY, slope, intercept, errorString);
    //error check
    if(errorString != ""){
        QMessageBox::warning(this, "ERROR", QString::fromStdString(errorString));
        return -1;//end of function execution
    }
    ui->textBrowser->clear();//clearing before appending
    QString line{""};
    for(int i = 0; i < (int)vecX.size(); i++){
        line = QString::number(i + 1) + "\t" + QString::number(vecX.at(i)) + "\t" + QString::number(vecY.at(i));
        std::string deliminator (40, '-');
        ui->textBrowser->append(line);//appends "{number of data point}\t{x}\t{y}"
        ui->textBrowser->append(QString::fromStdString(deliminator));//appends "----------"
    }
    ui->textBrowser->scrollToAnchor("any_anchor");//scrolls texBrowser to the upper bound
    //working with chart from now on
    QChart *chart = new QChart();
    chart->setAttribute(Qt::WA_DeleteOnClose);
    QLineSeries *series = new QLineSeries(chart);
    series->setName("approximation");
    for(int i = 0; i < (int)vecX.size(); i++){
        series->append(vecX.at(i),vecX.at(i) * slope + intercept);
    }
    QScatterSeries *series1 = new QScatterSeries(chart);
    series1->setName("data points");
    series1->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    series1->setMarkerSize(10.0);
    for(int i = 0; i < (int)vecX.size(); i++){
        series1->append(vecX.at(i),vecY.at(i));
    }
    chart->addSeries(series);
    chart->addSeries(series1);
    chart->createDefaultAxes();
    // Customize title as an equation
    const QString title = QString::fromStdString("equation: y = "+std::to_string(slope)+" * x + "+std::to_string(intercept));
    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::blue));
    chart->setTitle(title);
    // Customize series
    QPen pen(QRgb(0x000000));
    pen.setWidth(5);
    series->setPen(pen);
    QPen pen1(QRgb(0xfdb157));
    pen.setWidth(5);
    series1->setPen(pen1);
    chart->setAnimationOptions(QChart::AllAnimations);
    // Customize chart background
    QLinearGradient backgroundGradient;
    backgroundGradient.setStart(QPointF(0, 0));
    backgroundGradient.setFinalStop(QPointF(0, 1));
    backgroundGradient.setColorAt(0.0, QRgb(0xd2d0d1));
    backgroundGradient.setColorAt(1.0, QRgb(0x4c4547));
    backgroundGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    chart->setBackgroundBrush(backgroundGradient);
    // Customize plot area background
    QLinearGradient plotAreaGradient;
    plotAreaGradient.setStart(QPointF(0, 1));
    plotAreaGradient.setFinalStop(QPointF(1, 0));
    plotAreaGradient.setColorAt(0.0, QRgb(0x555555));
    plotAreaGradient.setColorAt(1.0, QRgb(0xaa5555));
    plotAreaGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    chart->setPlotAreaBackgroundBrush(plotAreaGradient);
    chart->setPlotAreaBackgroundVisible(true);
    // Customize axis colors
    QPen axisPen(QRgb(0xd18952));
    axisPen.setWidth(2);
    chart->axes().at(0)->setLinePen(axisPen);
    chart->axes().at(1)->setLinePen(axisPen);
    QBrush axisBrush(Qt::white);
    chart->axes().at(0)->setLabelsBrush(axisBrush);
    chart->axes().at(1)->setLabelsBrush(axisBrush);
    //reloading the chart
    if(chartView){// won't delete anything on the first run (nothing to delete)
        chartView->chart()->close();
        chartView->close();
    }else{
        ui->horizontalSpacer->changeSize(5,5,QSizePolicy::Minimum,QSizePolicy::Minimum);// shift the spacer in the favour of the chart on the first run
    }
    chartView = new QChartView(chart);
    chartView->setAttribute(Qt::WA_DeleteOnClose);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->horizontalLayout3->addWidget(chartView);
    return 0;
}


