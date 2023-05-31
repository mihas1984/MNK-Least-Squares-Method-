#include "mnkwindow.h"
#include "ui_mnkwindow.h"
#include <string>
#include <fstream>
#include <vector>
#include <QtWidgets/QMainWindow>
#include <QtCharts>
#include <QLabel>
#include "readfile.hpp"
#include "mnk.hpp"
#include <stdexcept>
#include "exceptions.hpp"

MnkWindow::MnkWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MnkWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    //setting texts and titles:
    this->setWindowTitle("Least Squares");
    ui->fitButton->setText("FIT");
    ui->exitButton->setText("EXIT");
    ui->lineEdit->setPlaceholderText("prompt");
    ui->promptLabel->setText("browse to find the file with the data\n(or type the path to it manually)");
    ui->dataLabel->setText("point\t\tX\t\tY");
    //for red exit button:
    QPalette Pal(palette());
    Pal.setColor(QPalette::Button, QColor(33,0,0));
    ui->exitButton->setAutoFillBackground(true);
    ui->exitButton->setPalette(Pal);
    //connecting:
    connect(ui->fitButton, &QPushButton::clicked, this, &MnkWindow::fitButtonClicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &MnkWindow::exitButtonClicked);
    connect(ui->browserButton, &QPushButton::clicked, this, &MnkWindow::browseButtonClicked);
}

MnkWindow::~MnkWindow()
{
    delete ui;
}

void MnkWindow::closeEvent(QCloseEvent *event)
{
    emit closes();
    event->accept();
}

void MnkWindow::exitButtonClicked()
{
    close();
}

void MnkWindow::browseButtonClicked()
{
    filebrowser = new FileBrowser(this);
    filebrowser->setAttribute(Qt::WA_DeleteOnClose);
    filebrowser->setModal(true);//it is important that it is modal
    filebrowser->setWindowTitle("file browser");
    filebrowser->show();
}

void MnkWindow::recievePath(const QString &path)
{
    ui->lineEdit->setText(path);
}

int MnkWindow::fitButtonClicked()
{
    std::vector<long double> vecX;//vectors will contain coordinates from the data file
    std::vector<long double> vecY;
    std::vector<std::string> varNames;// names of x and y axis
    std::vector<std::string> varUnits;// units in which x and y are measured
    std::ifstream fin;
    std::string path{""};
    path = ui->lineEdit->text().toStdString();
    fin.open(path);
    long double slope{}, intercept{};
    try{
        readData(varNames, varUnits, vecX, vecY, fin);
        computate_equation(vecX, vecY, slope, intercept);
    }catch(const ComputationWarning& ex){
        QMessageBox::warning(this, "WARNING", QString::fromStdString((std::string)ex.what()));
    }
    catch(const std::exception& ex){
        QMessageBox::critical(this, "ERROR", QString::fromStdString((std::string)ex.what()));
        return -1;//end of function execution
    }
    //computating errors:
    long double slope_error{};
    long double intercept_error{};
    bool are_errors = !(computate_equation_error(vecX, vecY, slope, intercept, slope_error, intercept_error));
    //setting label with axis names:
    QString dataLabelText = "point\t\t" + QString::fromStdString(varNames.at(0)) + "\t\t" + QString::fromStdString(varNames.at(1));
    ui->dataLabel->setText(dataLabelText);
    //working with textBrowser:
    ui->textBrowser->clear();//clearing before appending
    QString line{""};
    for(int i = 0; i < (int)vecX.size(); i++){
        line = QString::number(i + 1) + "\t" + QString::number((double)vecX.at(i)) + "\t" + QString::number((double)vecY.at(i));
        std::string deliminator (40, '-');
        ui->textBrowser->append(line);//appends "{number of data point}\t{x}\t{y}"
        ui->textBrowser->append(QString::fromStdString(deliminator));//appends "----------"
    }
    ui->textBrowser->scrollToAnchor("any_anchor");//scrolls texBrowser to the upper bound
    //working with chart from now on:
    QChart *chart = new QChart();
    chart->setAttribute(Qt::WA_DeleteOnClose);
    //line series:
    QLineSeries *lineSeries = new QLineSeries(chart);//approximation
    lineSeries->setName("approximation");
    for(int i = 0; i < (int)vecX.size(); i++){
        lineSeries->append(vecX.at(i),vecX.at(i)*slope+intercept);
    }
    //scatter series:
    QScatterSeries *scatterSeries = new QScatterSeries(chart);//experimental data points
    scatterSeries->setName("data points");
    scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    scatterSeries->setMarkerSize(10.0);
    scatterSeries->setColor(QRgb(0xFA9191));//light-red
    for(int i = 0; i < (int)vecX.size(); i++){
        scatterSeries->append(vecX.at(i),vecY.at(i));
    }
    //series to the chart:
    chart->addSeries(lineSeries);
    chart->addSeries(scatterSeries);
    chart->createDefaultAxes();
    // Customize title as an equation
    QString chartTitle {""};
    if(are_errors){
        chartTitle = ("y = (" + QString::number((double)slope) + " ± " + QString::number((double)slope_error) + ") * x + ("
                      + QString::number((double)intercept) + " ± " + QString::number((double)intercept_error) + ")");
    }else{
        chartTitle = ("y = (" + QString::number((double)slope) + ") * x + ("
                      + QString::number((double)intercept) + ")");
    }
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
    backgroundGradient.setColorAt(0.0, QRgb(0x818181));//grey//0xd2d0d1//BDC3C7
    backgroundGradient.setColorAt(1.0, QRgb(0x000000));//dark-grey//0x4c4547
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
    QBrush axisBrush(Qt::white);//(QRgb(0xFFF000));//
    chart->axes().at(0)->setLabelsBrush(axisBrush);//X-axis
    chart->axes().at(1)->setLabelsBrush(axisBrush);//Y-axis
    //customise axis titles
    QBrush axisTextBrush(QBrush(Qt::darkRed));//custom red//0x922B21 QRgb
    chart->axes().at(0)->setTitleBrush(axisTextBrush);
    QString xAxisTitle = QString::fromStdString(varNames.at(0)) + ", " + QString::fromStdString(varUnits.at(0));
    chart->axes().at(0)->setTitleText(xAxisTitle);
    chart->axes().at(1)->setTitleBrush(axisTextBrush);
    QString yAxisTitle = QString::fromStdString(varNames.at(1)) + ", " + QString::fromStdString(varUnits.at(1));
    chart->axes().at(1)->setTitleText(yAxisTitle);
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
