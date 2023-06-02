#include "mnkwindow.h"
#include "ui_mnkwindow.h"
#include "mnk.hpp"
#include "exceptions.hpp"
#include <string>
#include <fstream>
#include <vector>
#include <QtWidgets/QMainWindow>
#include <QtCharts>
#include <QLabel>
#include <stdexcept>
#include <QFont>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QHeaderView>
#include <QAction>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QByteArray>
#include <QScreen>
#include <QPixmap>

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
    //for red exit button:
    QPalette Pal(palette());
    Pal.setColor(QPalette::Button, QColor(33,0,0));
    ui->exitButton->setAutoFillBackground(true);
    ui->exitButton->setPalette(Pal);
    //connecting buttons:
    connect(ui->fitButton, &QPushButton::clicked, this, &MnkWindow::fitButtonClicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &MnkWindow::exitButtonClicked);
    connect(ui->addRowButton, &QPushButton::clicked, this, &MnkWindow::addRowClicked);
    //filling tableView with 4 initial rows and 2 columns:
    model = new QStandardItemModel(2,2,this);
    CreatedefaultModel();
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //unresizable sections:
    QHeaderView* headView = new QHeaderView(Qt::Horizontal, ui->tableView);
    headView->setFixedWidth(200);
    headView->setMinimumSectionSize(100);
    headView->setMaximumSectionSize(100);
    ui->tableView->setHorizontalHeader(headView);
    //setting status
    ui->labelFileName->setText("UNTITLED.csv");
    ui->labelStatus->setText("");
    //connecting actions:
    connect(ui->actionSaveAs, &QAction::triggered, [=]() {saveAsCsv();});
    connect(ui->actionSave, &QAction::triggered, [=]() {saveCsv();});
    connect(ui->actionOpen, &QAction::triggered, [=]() {openCsv();});
    connect(ui->actionClose, &QAction::triggered, [=]() {closeCsv();});
    connect(ui->actionScreenCapture, &QAction::triggered, [=]() {screenCapture();});
    //connect listenning to table data modifications:
    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onModified()));
}

MnkWindow::~MnkWindow()
{
    delete ui;
}

void MnkWindow::readData(std::vector<std::string> &varNames, std::vector<std::string> &varUnits, std::vector<long double> &vecX, std::vector<long double> &vecY)
{
    //reading variables' names:
    ItemX = model->item(0,0);
    ItemY = model->item(0,1);
    if(!ItemX){throw InvalidUnitsNames("no names");}//Item can be nullptr or ""
    else if(ItemX->text() == ""){throw InvalidUnitsNames("no names");}
    if(!ItemY){throw InvalidUnitsNames("no names");}
    else if(ItemY->text() == ""){throw InvalidUnitsNames("no names");}
    varNames.push_back(ItemX->text().toStdString());
    varNames.push_back(ItemY->text().toStdString());
    //reading units' names:
    ItemX = model->item(1,0);
    ItemY = model->item(1,1);
    if(!ItemX){throw InvalidUnitsNames("no units");}//Item can be nullptr or ""
    else if(ItemX->text() == ""){throw InvalidUnitsNames("no units");}
    if(!ItemY){throw InvalidUnitsNames("no unints");}
    else if(ItemY->text() == ""){throw InvalidUnitsNames("no units");}
    varUnits.push_back(ItemX->text().toStdString());
    varUnits.push_back(ItemY->text().toStdString());
    //reading data points:
    int rowCount = model->rowCount();
    std::string::size_type sz;//for std::stold
    for(int i = 2; i < rowCount; i++){
        ItemX = model->item(i,0);
        ItemY = model->item(i,1);
        if(!ItemX){// on next iteration when both empty, throwing when only one of them has value
            if(!ItemY){continue;}//so many checks because Item can be nullptr or "", and we want program to behave the same and no segmentation faults
            else if(ItemY->text() == ""){continue;}
            else{throw InvalidData("no pair for data point");}
        }else if(ItemX->text() == ""){
            if(!ItemY){continue;}
            else if(ItemY->text() == ""){continue;}
            else{throw InvalidData("no pair for data point");}
        }else{//when ItemX present
            if(!ItemY){throw InvalidData("no pair for data point");}
            else if(ItemY->text() == ""){throw InvalidData("no pair for data point");}
            else{
                vecX.push_back(std::stold (ItemX->text().toStdString(),&sz));
                vecY.push_back(std::stold (ItemY->text().toStdString(),&sz));
            }
        }
    }
    if(!vecX.size() || !vecY.size()){
        throw InvalidData("no data points recorded");
    }
}

void MnkWindow::closeEvent(QCloseEvent *event)
{
    emit closes();
    event->accept();
}

void MnkWindow::exitButtonClicked()
{
    if(ui->labelStatus->text() == "Modified"){
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Unsaved modifications", "do you want to save changes?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            saveCsv();//saving
        }
    }
    close();
}

void MnkWindow::addRowClicked()
{
    QFont font("Times",14);
    model->insertRow(model->rowCount(QModelIndex()));
    QStandardItem* item = new QStandardItem(QString::number(currentRowNumber));
    item->setFont(font);
    model->setVerticalHeaderItem(1+currentRowNumber, item);
    currentRowNumber++;
}

void MnkWindow::saveAsCsv()
{
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save File"),"UNTITLED.csv", tr("CSV files (*.csv)"));
    if (filepath.trimmed().isEmpty()){// nothing but whitespace, means cancel was pressed
        return;
    }
    currentFilepath = filepath;
    QFile file(currentFilepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::critical(this, "ERROR", "error saving the file.");
        return;
    }
    QTextStream out(&file);
    //Names:
    ItemX = model->item(0,0);
    ItemY = model->item(0,1);
    if(ItemX){
        out << ItemX->text();
    }
    out << ";";
    if(ItemY){
        out << ItemY->text();
    }
    out << "\n";
    //Units:
    ItemX = model->item(1,0);
    ItemY = model->item(1,1);
    if(ItemX){
        out << ItemX->text();
    }
    out << ";";
    if(ItemY){
        out << ItemY->text();
    }
    out << "\n";
    //Data points:
    int rowCount = model->rowCount();
    for(int i = 2; i < rowCount; i++){
        ItemX = model->item(i,0);
        ItemY = model->item(i,1);
        if(ItemX){
            out << ItemX->text();
        }
        out << ";";
        if(ItemY){
            out << ItemY->text();
        }
        out << "\n";
    }
    ui->labelFileName->setText(currentFilepath);
    ui->labelStatus->setText("Saved");
}

void MnkWindow::openCsv()
{
    QString filename= QFileDialog::getOpenFileName(this, "Choose File");
    if(filename.isEmpty()){//cancel was pressed
        return;
    }
    //check if file is .csv:
    QString fileFormat = filename.right(4);
    if(fileFormat != ".csv"){
        QMessageBox::critical(this, "ERROR", "chosen file is not a .csv file.");
        return;
    }
    //openning:
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "ERROR", "error openning the file.");
        return;
    }
    //resetting chart:
    if(chartView){
        chartView->chart()->close();
        chartView->close();
        chartView = nullptr;
        ui->horizontalSpacer->changeSize(40,20,QSizePolicy::Minimum,QSizePolicy::Expanding);
    }
    //reading from csv to WordLists:
    currentFilepath = filename;
    QStringList xWordList;//x column
    QStringList yWordList;//y column
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if(line.back() == '\n'){
            line.erase(line.end()-1);//erase \n from end
        }
        if(line.back() == '\r'){
            line.erase(line.end()-1);//erase \r from end
        }
        QString xWord = line.split(';').first();
        QString yWord = line.split(';').last();
        xWordList.append(xWord);
        yWordList.append(yWord);
    }
    //adding rows if needed:
    if(xWordList.size() > model->rowCount()){
        for(int i = 4; i < xWordList.size(); i++){
            addRowClicked();
        }
    }
    //outputting to the tableView:
    for(int i = 0; i < xWordList.size(); i++){
        model->setData(model->index(i,0),xWordList.at(i));
        model->setData(model->index(i,1),yWordList.at(i));
    }
    ui->labelFileName->setText(currentFilepath);
    ui->labelStatus->setText("");
    ui->label_intercept->setText("__________");
    ui->label_slope->setText("__________");
}

void MnkWindow::closeCsv()
{
    //resetting statuses:
    currentFilepath = "";
    ui->labelFileName->setText("UNTITLED.csv");//UNTITLED.csv is a default starting point
    ui->labelStatus->setText("");
    ui->label_intercept->setText("__________");
    ui->label_slope->setText("__________");
    //resetting chart:
    if(chartView){
        chartView->chart()->close();
        chartView->close();
        chartView = nullptr;
        ui->horizontalSpacer->changeSize(40,20,QSizePolicy::Minimum,QSizePolicy::Expanding);
    }
    //clearing tableView:
    model->clear();
    CreatedefaultModel();
}

void MnkWindow::screenCapture()
{
    QScreen *screen;
    QPixmap pixmap;
    screen = QGuiApplication::primaryScreen();
    auto geom = this->geometry();
    pixmap = screen->grabWindow(0, geom.x(), geom.y(), geom.width(), geom.height());//capturing only this window
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save File"),"screenshot.png", tr("PNG files (*.png)"));//asking user to save file
    if (filepath.trimmed().isEmpty()){// nothing but whitespace, means cancel was pressed
        return;
    }
    //writing out png file:
    QFile file(filepath);
    file.open(QIODevice::WriteOnly);
    pixmap.save(&file, "PNG");
}

void MnkWindow::CreatedefaultModel()
{
    currentRowNumber = 1;

    QFont font("Times",14);
    ui->tableView->setFont(font);
    QStandardItem* item{nullptr};
    // 2x2 by default:
    item = new QStandardItem(QString("X"));
    item->setFont(font);
    model->setHorizontalHeaderItem(0, item);
    item = new QStandardItem(QString("Y"));
    item->setFont(font);
    model->setHorizontalHeaderItem(1, item);
    item = new QStandardItem(QString("Names"));
    item->setFont(font);
    model->setVerticalHeaderItem(0, item);
    item = new QStandardItem(QString("Units"));
    item->setFont(font);
    model->setVerticalHeaderItem(1, item);
    // inserting two more rows with number row names:
    addRowClicked();
    addRowClicked();
    ui->tableView->setModel(model);
}

void MnkWindow::onModified()
{
    ui->labelStatus->setText("Modified");
}

void MnkWindow::saveCsv()
{
    if(currentFilepath == ""){
        saveAsCsv();
        return;
    }
    QFile file(currentFilepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::critical(this, "ERROR", "error saving the file.");
        return;
    }
    QTextStream out(&file);
    ItemX = model->item(0,0);
    ItemY = model->item(0,1);
    if(ItemX){
        out << ItemX->text();
    }
    out << ";";
    if(ItemY){
        out << ItemY->text();
    }
    out << "\n";
    ItemX = model->item(1,0);
    ItemY = model->item(1,1);
    if(ItemX){
        out << ItemX->text();
    }
    out << ";";
    if(ItemY){
        out << ItemY->text();
    }
    out << "\n";

    int rowCount = model->rowCount();
    for(int i = 2; i < rowCount; i++){
        ItemX = model->item(i,0);
        ItemY = model->item(i,1);
        if(ItemX){
            out << ItemX->text();
        }
        out << ";";
        if(ItemY){
            out << ItemY->text();
        }
        out << "\n";
    }
    ui->labelStatus->setText("Saved");
}

int MnkWindow::fitButtonClicked()
{
    std::vector<long double> vecX;//vectors will contain coordinates from the data file
    std::vector<long double> vecY;
    std::vector<std::string> varNames;// names of x and y axis
    std::vector<std::string> varUnits;// units in which x and y are measured
    long double slope{}, intercept{};
    try{
        readData(varNames, varUnits, vecX, vecY);
        computate_equation(vecX, vecY, slope, intercept);
    }catch(const ComputationWarning& ex){
        QMessageBox::warning(this, "WARNING", QString::fromStdString((std::string)ex.what()));
    }
    catch(const std::exception& ex){
        std::string exWhat = ex.what();
        if (exWhat == "stold"){
            QMessageBox::critical(this, "ERROR", "error reading number");
        }else{
            QMessageBox::critical(this, "ERROR", QString::fromStdString((std::string)ex.what()));
        }
        return -1;//end of function execution
    }
    //computating errors:
    long double slope_error{};
    long double intercept_error{};
    computate_equation_error(vecX, vecY, slope, intercept, slope_error, intercept_error);
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
    // Customize series
    QPen lineSPen(QRgb(0x000000));//black
    lineSPen.setWidth(5);
    lineSeries->setPen(lineSPen);
    QPen scatterSPen(QRgb(0x822828));//dark-red
    scatterSPen.setWidth(0);
    scatterSeries->setPen(scatterSPen);
    chart->setAnimationOptions(QChart::AllAnimations);
    QBrush backgroundBrush(Qt::lightGray);
    chart->setBackgroundBrush(backgroundBrush);
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
    //customise axis titles
    QBrush axisTextBrush(QBrush(Qt::darkRed));
    chart->axes().at(0)->setTitleBrush(axisTextBrush);
    QString xAxisTitle = QString::fromStdString(varNames.at(0)) + ", " + QString::fromStdString(varUnits.at(0));
    chart->axes().at(0)->setTitleText(xAxisTitle);
    chart->axes().at(1)->setTitleBrush(axisTextBrush);
    QString yAxisTitle = QString::fromStdString(varNames.at(1)) + ", " + QString::fromStdString(varUnits.at(1));
    chart->axes().at(1)->setTitleText(yAxisTitle);
    chart->axes().at(1)->setTitleFont(QFont(fontInfo().family(), 14, QFont::Bold));//making font bigger
    chart->axes().at(0)->setTitleFont(QFont(fontInfo().family(), 14, QFont::Bold));
    //customise axis labels
    chart->axes().at(0)->setLabelsColor(QRgb(0xaa5555));//making axis numbers visible
    chart->axes().at(1)->setLabelsColor(QRgb(0xaa5555));
    chart->axes().at(1)->setLabelsFont(QFont(fontInfo().family(), 11, QFont::Normal));//making font bigger
    chart->axes().at(0)->setLabelsFont(QFont(fontInfo().family(), 11, QFont::Normal));
    //showing intercept and slope
    ui->label_slope->setText(QString::fromStdString(std::to_string(slope)) + " ± " + QString::fromStdString(std::to_string(slope_error)));//conversions because Qstring does not support long double
    ui->label_intercept->setText(QString::fromStdString(std::to_string(intercept)) + " ± " + QString::fromStdString(std::to_string(intercept_error)));//conversions because Qstring does not support long double
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
    ui->horizontalLayout_3->addWidget(chartView);
    return 0;
}
