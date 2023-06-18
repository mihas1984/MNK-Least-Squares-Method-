#include "mnkwindow.h"
#include "ui_mnkwindow.h"
#include "mnk.hpp"
#include "exceptions.hpp"
#include <string>
#include <fstream>
#include <vector>
#include <QtWidgets/QMainWindow>
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
#include <QRadioButton>
#include <algorithm>
#include <QtMath>


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
    ui->labelFileName->setText("UNTITLED.csv");
    ui->labelStatus->setText("");
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
    m_model = new QStandardItemModel(2,2,this);
    CreatedefaultModel();
    //toggling no errors radio buttons as default
    ui->radioButtonNoXErrs->toggle();
    ui->radioButtonNoYErrs->toggle();
    //connecting actions:
    connect(ui->actionSaveAs, &QAction::triggered, [=]() {saveAsCsv();});
    connect(ui->actionSave, &QAction::triggered, [=]() {saveCsv();});
    connect(ui->actionOpen, &QAction::triggered, [=]() {openCsv();});
    connect(ui->actionClose, &QAction::triggered, [=]() {closeCsv();});
    connect(ui->actionScreenCapture, &QAction::triggered, [=]() {screenCapture();});
    connect(ui->actionChartScreenshot, &QAction::triggered, [=]() {chartScreenShot();});
    //connect listenning to table data modifications:
    connect(m_model,SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onModified()));
    //connecting radioButtons:
    //for X:
    connect(ui->radioButtonAllXErrs, &QRadioButton::toggled, this, &MnkWindow::allXErrsToggled);
    connect(ui->radioButtonNoXErrs, &QRadioButton::toggled, this, &MnkWindow::noXErrsToggled);
    connect(ui->radioButtonFixedXErrs, &QRadioButton::toggled, this, &MnkWindow::fixedXErrsToggled);
    //for Y:
    connect(ui->radioButtonAllYErrs, &QRadioButton::toggled, this, &MnkWindow::allYErrsToggled);
    connect(ui->radioButtonNoYErrs, &QRadioButton::toggled, this, &MnkWindow::noYErrsToggled);
    connect(ui->radioButtonFixedYErrs, &QRadioButton::toggled, this, &MnkWindow::fixedYErrsToggled);
    //seting table to stretch:
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //fixed error lineedits disabled by default:
    ui->lineEditXFixedErr->setEnabled(false);
    ui->lineEditYFixedErr->setEnabled(false);
    //setting slope and intercept text copyable:
    ui->label_slope_intercept->setTextInteractionFlags(Qt::TextSelectableByMouse);
}

MnkWindow::~MnkWindow()
{
    delete ui;
}

void MnkWindow::readData(QVector<QString> &varNames, QVector<QString> &varUnits, QVector<long double> &vecX, QVector<long double> &vecY, QVector<long double> &vecXErrs, QVector<long double> &vecYErrs)
{//returns vector size 1 when fixed error, size 0 if no errors
    int rowCount = m_model->rowCount();
    int columnCount = m_model->columnCount();
    QStandardItem* ItemX{nullptr};
    QStandardItem* ItemY{nullptr};
    //reading variables' names:
    ItemX = m_model->item(0,0);
    ItemY = m_model->item(0,1);
    if(ItemX->text().toStdString().find_first_not_of(' ') == std::string::npos)
    {//there are only spaces(or nothing at all)(only spaces counts as unspecified):
        throw InvalidNames("no name for X specified");
    }
    if(ItemY->text().toStdString().find_first_not_of(' ') == std::string::npos)
    {//there are only spaces(or nothing at all)(only spaces counts as unspecified):
        throw InvalidNames("no name for Y specified");
    }
    varNames.push_back(ItemX->text());
    varNames.push_back(ItemY->text());
    //reading units' names:
    ItemX = m_model->item(1,0);
    ItemY = m_model->item(1,1);
    if(ItemX->text().toStdString().find_first_not_of(' ') == std::string::npos)
    {//there are only spaces(or nothing at all)(only spaces counts as unspecified):
        varUnits.push_back("");
    }else{
        varUnits.push_back(", " + ItemX->text());
    }
    if(ItemY->text().toStdString().find_first_not_of(' ') == std::string::npos)
    {//there are only spaces(or nothing at all)(only spaces counts as unspecified):
        varUnits.push_back("");
    }else{
        varUnits.push_back(", " + ItemY->text());
    }
    //finding errors(deviations):
    std::string::size_type sz;//for std::stold
    int deltaXColumnNumber{-1};//-1 means column is not found
    int deltaYColumnNumber{-1};
    if(ui->radioButtonFixedXErrs->isChecked()){
        long double fixedXErr{};
        if(ui->lineEditXFixedErr->text() == ""){
            throw InvalidDeviations("no fixed X error specified");
        }else{
            fixedXErr = std::stold(ui->lineEditXFixedErr->text().toStdString(),&sz);//std::stold will throw if data is non-numeric
            if(fixedXErr >= 0){
                vecXErrs.push_back(fixedXErr);
            }else{
                throw InvalidDeviations("fixed X error is a negative number");
            }
        }
    }else if(ui->radioButtonAllXErrs->isChecked()){
        for(int i = 0; i < columnCount; ++i){
            if(m_model->horizontalHeaderItem(i)->text() == "ΔX"){
                deltaXColumnNumber = i;
            }
        }
    }
    if(ui->radioButtonFixedYErrs->isChecked()){
        long double fixedYErr{};
        if(ui->lineEditYFixedErr->text() == ""){
            throw InvalidDeviations("no fixed Y error specified");
        }else{
            fixedYErr = std::stold(ui->lineEditYFixedErr->text().toStdString(),&sz);//std::stold will throw if data is non-numeric
            if(fixedYErr >= 0){
                vecYErrs.push_back(fixedYErr);
            }else{
                throw InvalidDeviations("fixed Y error is a negative number");
            }
        }
    }else if(ui->radioButtonAllYErrs->isChecked()){
        for(int i = 0; i < columnCount; ++i){
            if(m_model->horizontalHeaderItem(i)->text() == "ΔY"){
                deltaYColumnNumber = i;
            }
        }
    }
    //reading data points:
    QStandardItem* ItemXErr{nullptr};
    QStandardItem* ItemYErr{nullptr};
    for(int i = 2; i < rowCount; ++i){
        ItemX = m_model->item(i,0);
        ItemY = m_model->item(i,1);
        if(deltaXColumnNumber != -1){
            ItemXErr = m_model->item(i,deltaXColumnNumber);
        }
        if(deltaYColumnNumber != -1){
            ItemYErr = m_model->item(i,deltaYColumnNumber);
        }
        //if "empty" X and Y we skip the row, if only one of two we throw an exception, if both are not "empty" and delta columns are filled if available we read, else throw an exception
        if(ItemX->text().toStdString().find_first_not_of(' ') == std::string::npos){//there are only spaces(or nothing at all)
            if(ItemY->text().toStdString().find_first_not_of(' ') == std::string::npos){//there are only spaces(or nothing at all)
                continue;
            }else{
                throw InvalidData("no X specified on row #" + std::to_string(i-1));
            }
        }else{
            if(ItemY->text().toStdString().find_first_not_of(' ') == std::string::npos){//there are only spaces(or nothing at all)
                throw InvalidData("no Y specified on row #" + std::to_string(i-1));
            }else{
                if(deltaXColumnNumber != -1){
                    if(ItemXErr->text().toStdString().find_first_not_of(' ') == std::string::npos){//there are only spaces(or nothing at all)
                        throw InvalidDeviations("no X error found on row #" + std::to_string(i-1));
                    }else{
                        vecXErrs.push_back(std::stold (ItemXErr->text().toStdString(),&sz));
                    }
                }
                if(deltaYColumnNumber != -1){
                    if(ItemYErr->text().toStdString().find_first_not_of(' ') == std::string::npos){//there are only spaces(or nothing at all)
                        throw InvalidDeviations("no Y error found on row #" + std::to_string(i-1));
                    }else{
                        vecYErrs.push_back(std::stold (ItemYErr->text().toStdString(),&sz));
                    }
                }
                vecX.push_back(std::stold (ItemX->text().toStdString(),&sz));
                vecY.push_back(std::stold (ItemY->text().toStdString(),&sz));
            }
        }
    }
}

void MnkWindow::closeEvent(QCloseEvent *event)
{
    if(ui->labelStatus->text() == "Modified"){
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Unsaved modifications", "do you want to save changes?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            saveCsv();//saving
        }
    }
    emit closes();
    event->accept();
}

void MnkWindow::exitButtonClicked()
{
    close();
}

void MnkWindow::addRowClicked()
{
    QFont font("Times",14);
    m_model->insertRow(m_model->rowCount());
    int rowCount = m_model->rowCount();
    int columnCount = m_model->columnCount();
    QStandardItem* item = new QStandardItem(QString::number(rowCount-2));//two cells offset for units and names
    item->setFont(font);
    m_model->setVerticalHeaderItem(rowCount-1, item);
    //setting empty items:
    for(int i = 0; i < columnCount; ++i){
        m_model->setItem(rowCount-1,i,new QStandardItem(""));
    }
}

void MnkWindow::saveAsCsv()
{
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save File"),"UNTITLED.csv", tr("CSV files (*.csv)"));
    if (filepath.trimmed().isEmpty()){// nothing but whitespace, means cancel was pressed
        return;
    }
    QString fileFormat = filepath.right(4);
    if(fileFormat != ".csv"){
        QMessageBox::critical(this, "ERROR", "chosen file is not a .csv file.");
        return;
    }
    m_currentFilepath = filepath;
    if(!saveCsvGeneralFunction()){// if ok
        ui->labelFileName->setText(m_currentFilepath);
    }
}

void MnkWindow::openCsv()
{//has a lot of checks to prevent unwated crashes
    //saving previous modifications:
    if(ui->labelStatus->text() == "Modified"){
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Unsaved modifications", "do you want to save changes?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            saveCsv();//saving
        }
    }
    //choosing file:
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
    //resetting m_chart:
    if(m_chart){
        m_chart->close();
        m_chart = nullptr;
    }
    m_currentFilepath = filename;
    //reading first two lines with X name Y name and X units Y units:
    //names:
    QByteArray line = file.readLine();
    if(line == ""){
        QMessageBox::critical(this, "ERROR", "file is empty.");
        ui->labelStatus->setText("");
        closeCsv();//clearing
        return;
    }
    if(line.back() == '\n'){
        line.erase(line.end()-1);//erase \n from end
    }
    if(line.back() == '\r'){
        line.erase(line.end()-1);//erase \r from end
    }
    QList<QByteArray> lineArray = line.split(';');
    QString xName = lineArray.first();
    QString yName = lineArray.last();
    m_model->setData(m_model->index(0,0),xName);
    m_model->setData(m_model->index(0,1),yName);
    //units:
    if(!file.atEnd()){
        line = file.readLine();
    }else{
        QMessageBox::critical(this, "ERROR", "corrupted data in a .csv file.");
        ui->labelStatus->setText("");
        closeCsv();//clearing
        return;
    }
    if(line.back() == '\n'){
        line.erase(line.end()-1);//erase \n from end
    }
    if(line.back() == '\r'){
        line.erase(line.end()-1);//erase \r from end
    }
    lineArray = line.split(';');
    QString xUnits = lineArray.first();
    QString yUnits = lineArray.last();
    m_model->setData(m_model->index(1,0), xUnits);
    m_model->setData(m_model->index(1,1), yUnits);
    //reading third line with error specifications:
    bool isAllXErrs = false;
    bool isAllYErrs = false;
    if(!file.atEnd()){
        line = file.readLine();
    }else{
        QMessageBox::critical(this, "ERROR", "corrupted data in a .csv file.");
        ui->labelStatus->setText("");
        closeCsv();//clearing
        return;
    }
    if(line.back() == '\n'){
        line.erase(line.end()-1);//erase \n from end
    }
    if(line.back() == '\r'){
        line.erase(line.end()-1);//erase \r from end
    }
    lineArray = line.split(';');
    QString xErrSpecs = lineArray.first();
    QString yErrSpecs = lineArray.last();
    //radio buttons x:
    if(xErrSpecs == "noXErrs"){
        if(!ui->radioButtonNoXErrs->isChecked()){
            ui->radioButtonNoXErrs->toggle();
        }
    }else if(xErrSpecs == "allXErrs"){
        ui->radioButtonAllXErrs->toggle();
        isAllXErrs = true;
    }else if(xErrSpecs.split(':').first() == "fixedXErrs"){
        if(!ui->radioButtonFixedXErrs->isChecked()){
            ui->radioButtonFixedXErrs->toggle();
        }
        ui->lineEditXFixedErr->setText(xErrSpecs.split(':').last());
    }else{
        QMessageBox::critical(this, "ERROR", "corrupted data in a .csv file.");
        ui->labelStatus->setText("");
        closeCsv();//clearing
        return;
    }
    //radio buttons y:
    if(yErrSpecs == "noYErrs"){
        if(!ui->radioButtonNoYErrs->isChecked()){
            ui->radioButtonNoYErrs->toggle();
        }
    }else if(yErrSpecs == "allYErrs"){
        ui->radioButtonAllYErrs->toggle();
        isAllYErrs = true;
    }else if(yErrSpecs.split(':').first() == "fixedYErrs"){
        if(!ui->radioButtonFixedYErrs->isChecked()){
            ui->radioButtonFixedYErrs->toggle();
        }
        ui->lineEditYFixedErr->setText(yErrSpecs.split(':').last());
    }else{
        QMessageBox::critical(this, "ERROR", "corrupted data in a .csv file.");
        ui->labelStatus->setText("");
        closeCsv();//clearing
        return;
    }
    //arrays of data about to be filled:
    QStringList xWordList;//x column
    QStringList yWordList;//y column
    QStringList xErrWordList;//x err column
    QStringList yErrWordList;//y err column
    //reading all the other data into respective arrays:
    while (!file.atEnd()) {
        line = file.readLine();
        if(line.back() == '\n'){
            line.erase(line.end()-1);//erase \n from end
        }
        if(line.back() == '\r'){
            line.erase(line.end()-1);//erase \r from end
        }
        lineArray = line.split(';');
        if(lineArray.size() < 2){//no out of range exception otherwise
            QMessageBox::critical(this, "ERROR", "corrupted data in a .csv file.");
            ui->labelStatus->setText("");
            closeCsv();//clearing
            return;
        }
        QString xWord = lineArray.at(0);
        QString yWord = lineArray.at(1);
        xWordList.append(xWord);
        yWordList.append(yWord);
        if(isAllXErrs){
            if(lineArray.size() < 3){//no out of range exception otherwise
                QMessageBox::critical(this, "ERROR", "corrupted data in a .csv file.");
                ui->labelStatus->setText("");
                closeCsv();//clearing
                return;
            }
            QString xErrWord = lineArray.at(2);
            xErrWordList.append(xErrWord);
        }
        if(isAllYErrs){
            if(lineArray.size() < 4){//no out of range exception otherwise
                QMessageBox::critical(this, "ERROR", "corrupted data in a .csv file.");
                ui->labelStatus->setText("");
                closeCsv();//clearing
                return;
            }
            QString yErrWord = lineArray.at(3);
            yErrWordList.append(yErrWord);
        }
    }
    //finding delta x and delta y columns on the table:
    int columnDeltaX{-1};
    int columnDeltaY{-1};
    for(int i = 0; i < m_model->columnCount(); ++i){
        if(m_model->horizontalHeaderItem(i)->text() == "ΔX"){
            columnDeltaX = i;
        }
        if(m_model->horizontalHeaderItem(i)->text() == "ΔY"){
            columnDeltaY = i;
        }
    }
    //adding rows if needed:
    if(xWordList.size() > m_model->rowCount() - 2){
        for(int i = 2; i < xWordList.size(); ++i){
            addRowClicked();
        }
    }
    //outputting to the tableView:
    for(int i = 0; i < xWordList.size(); ++i){
        m_model->setData(m_model->index(i + 2,0),xWordList.at(i));
        m_model->setData(m_model->index(i + 2,1),yWordList.at(i));
        if(isAllXErrs){
            m_model->setData(m_model->index(i + 2,columnDeltaX),xErrWordList.at(i));
        }
        if(isAllYErrs){
            m_model->setData(m_model->index(i + 2,columnDeltaY),yErrWordList.at(i));
        }
    }
    ui->labelFileName->setText(m_currentFilepath);
    ui->labelStatus->setText("");
    ui->label_slope_intercept->setText("Intercept =   __________\nSlope =   __________");
}

void MnkWindow::closeCsv()
{
    if(ui->labelStatus->text() == "Modified"){
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Unsaved modifications", "do you want to save changes?", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes){
            saveCsv();//saving
        }
    }
    //resetting m_chart:
    if(m_chart){
        m_chart->close();
        m_chart = nullptr;
    }
    //clearing tableView:
    m_model->clear();
    CreatedefaultModel();
    //clearing toggled radio buttons:
    ui->radioButtonNoXErrs->toggle();
    ui->radioButtonNoYErrs->toggle();
    m_xErrCurrentlyChecked = "noXErrs";
    m_yErrCurrentlyChecked = "noYErrs";
    //resetting statuses:
    m_currentFilepath = "";
    ui->labelFileName->setText("UNTITLED.csv");//UNTITLED.csv is a default starting point
    ui->labelStatus->setText("");
    ui->label_slope_intercept->setText("Intercept =   __________\nSlope =   __________");
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
    QString fileFormat = filepath.right(4);
    if(fileFormat != ".png"){
        QMessageBox::critical(this, "ERROR", "chosen file is not a .png file.");
        return;
    }
    //writing out png file:
    QFile file(filepath);
    file.open(QIODevice::WriteOnly);
    pixmap.save(&file, "PNG");
}

void MnkWindow::chartScreenShot()
{
    if(!m_chart){
        QMessageBox::information(this, "nothing to save", "press \"FIT\" to get a chart.");
        return;
    }

    QString filepath = QFileDialog::getSaveFileName(this, tr("Save File"),"screenshot.png", tr("PNG files (*.png)"));//asking user to save file
    if (filepath.trimmed().isEmpty()){// nothing but whitespace, means cancel was pressed
        return;
    }
    QString fileFormat = filepath.right(4);
    if(fileFormat != ".png"){
        QMessageBox::critical(this, "ERROR", "chosen file is not a .png file.");
        return;
    }


    m_chart->savePng( filepath, m_chart->width(), m_chart->height() );
}

void MnkWindow::CreatedefaultModel()
{
    QFont font("Times",14);
    ui->tableView->setFont(font);
    QStandardItem* item{nullptr};
    // 4x4 by default:
    item = new QStandardItem(QString("X"));
    item->setFont(font);
    m_model->setHorizontalHeaderItem(0, item);
    item = new QStandardItem(QString("Y"));
    item->setFont(font);
    m_model->setHorizontalHeaderItem(1, item);
    item = new QStandardItem(QString("ΔX"));
    item->setFont(font);
    m_model->setHorizontalHeaderItem(2, item);
    item = new QStandardItem(QString("ΔY"));
    item->setFont(font);
    m_model->setHorizontalHeaderItem(3, item);
    item = new QStandardItem(QString("Names"));
    item->setFont(font);
    m_model->setVerticalHeaderItem(0, item);
    item = new QStandardItem(QString("Units"));
    item->setFont(font);
    m_model->setVerticalHeaderItem(1, item);
    //setting empty items:
    m_model->setItem(0,0,new QStandardItem(""));
    m_model->setItem(1,0,new QStandardItem(""));
    m_model->setItem(0,1,new QStandardItem(""));
    m_model->setItem(1,1,new QStandardItem(""));
    m_model->setItem(0,2,new QStandardItem(""));
    m_model->setItem(1,2,new QStandardItem(""));
    m_model->setItem(0,3,new QStandardItem(""));
    m_model->setItem(1,3,new QStandardItem(""));
    //inserting two more rows with number row names:
    addRowClicked();
    addRowClicked();
    ui->tableView->setModel(m_model);
    //disabling units and names for delta X column:
    item = m_model->item(0,2);
    item->setFlags(Qt::NoItemFlags);//disabling input
    item = m_model->item(1,2);
    item->setFlags(Qt::NoItemFlags);//disabling input
    //disabling units and names for delta Y column:
    item = m_model->item(0,3);
    item->setFlags(Qt::NoItemFlags);//disabling input
    item = m_model->item(1,3);
    item->setFlags(Qt::NoItemFlags);//disabling input
    //hiding delta columns:
    ui->tableView->setColumnHidden(2, true);
    ui->tableView->setColumnHidden(3, true);
}

int MnkWindow::saveCsvGeneralFunction()
{
    //choosing file:
    QFile file(m_currentFilepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::critical(this, "ERROR", "error saving the file.");
        return -1;
    }
    QTextStream out(&file);
    //locating X and Y delta columns:
    int rowCount = m_model->rowCount();
    int columnCount = m_model->columnCount();
    int deltaXColumnNumber{-1};
    int deltaYColumnNumber{-1};
    for(int i = 0; i < columnCount; ++i){
        if(m_model->horizontalHeaderItem(i)->text() == "ΔX"){
            deltaXColumnNumber = i;
        }
        if(m_model->horizontalHeaderItem(i)->text() == "ΔY"){
            deltaYColumnNumber = i;
        }
    }
    //outputting units and names:
    out << m_model->item(0, 0)->text() << ";" << m_model->item(0, 1)->text() << "\n";
    out << m_model->item(1, 0)->text() << ";" << m_model->item(1, 1)->text() << "\n";
    //outputting error specifics:
    out << m_xErrCurrentlyChecked;
    if(m_xErrCurrentlyChecked == "fixedXErrs"){
        out << ":" << ui->lineEditXFixedErr->text();
    }
    out<< ";";
    out << m_yErrCurrentlyChecked;
    if(m_yErrCurrentlyChecked == "fixedYErrs"){
        out << ":" << ui->lineEditYFixedErr->text();
    }
    out << '\n';
    //counting how many empty lines at the end will be left out:
    int emptyRowsFromEnd = 0;
    bool isBreak{false};//for breaking two loops
    for(int i = rowCount - 1; i > 1; --i){//ending at row #1 of data points
        for(int j = 0; j < columnCount; ++j){
            if(!ui->tableView->isColumnHidden(j) && m_model->item(i, j)->text().toStdString().find_first_not_of(' ') != std::string::npos){//there are not only spaces, or column is not visible
                isBreak = true;
            }
            if(isBreak){break;}
            ++emptyRowsFromEnd;
        }
        if(isBreak){break;}
    }
    emptyRowsFromEnd /= columnCount;
    //reading table and outputting to a file:
    for(int i = 2; i < rowCount - emptyRowsFromEnd; ++i){//starting from row #1 of data points
        QString line{""};
        line += m_model->item(i, 0)->text() + ";" + m_model->item(i, 1)->text();
        if(m_xErrCurrentlyChecked == "allXErrs"){
            line += ";" + m_model->item(i, deltaXColumnNumber)->text();
        }
        if(m_yErrCurrentlyChecked == "allYErrs"){
            if(m_xErrCurrentlyChecked != "allXErrs"){
                line += ";";//adding for Y nevertheless for fixed position in a file
            }
            line += ";" + m_model->item(i, deltaYColumnNumber)->text();
        }
        out << line << "\n";
    }
    ui->labelStatus->setText("Saved");
    return 0;
}

void MnkWindow::allXErrsToggled()
{
    if(m_xErrCurrentlyChecked != "allXErrs"){//showing delta X column
        m_xErrCurrentlyChecked = "allXErrs";
        ui->tableView->setColumnHidden(2, false);
    }else{//hiding delta X column
        ui->tableView->setColumnHidden(2, true);
    }
    onModified();//showing that things changed
}

void MnkWindow::noXErrsToggled()
{
    if(m_xErrCurrentlyChecked != "noXErrs"){//toggled
        m_xErrCurrentlyChecked = "noXErrs";
    }
    onModified();//showing that things changed
}

void MnkWindow::fixedXErrsToggled()
{
    if(m_xErrCurrentlyChecked != "fixedXErrs"){//enable lineedit
        m_xErrCurrentlyChecked = "fixedXErrs";
        ui->lineEditXFixedErr->setEnabled(true);
    }else{//disable lineedit
        ui->lineEditXFixedErr->setText("");
        ui->lineEditXFixedErr->setEnabled(false);
    }
    onModified();//showing that things changed
}

void MnkWindow::allYErrsToggled()
{
    if(m_yErrCurrentlyChecked != "allYErrs"){//showing delta Y column
        m_yErrCurrentlyChecked = "allYErrs";
        ui->tableView->setColumnHidden(3, false);
    }else{//hiding delta Y column
        ui->tableView->setColumnHidden(3, true);
    }
    onModified();//showing that things changed
}

void MnkWindow::noYErrsToggled()
{
    if(m_yErrCurrentlyChecked != "noYErrs"){//toggled
        m_yErrCurrentlyChecked = "noYErrs";
    }
    onModified();//showing that things changed
}

void MnkWindow::fixedYErrsToggled()
{
    if(m_yErrCurrentlyChecked != "fixedYErrs"){//toggled
        m_yErrCurrentlyChecked = "fixedYErrs";
        ui->lineEditYFixedErr->setEnabled(true);
    }else{
        ui->lineEditYFixedErr->setText("");
        ui->lineEditYFixedErr->setEnabled(false);
    }
    onModified();//showing that things changed
}

void MnkWindow::onModified()
{
    ui->labelStatus->setText("Modified");
}

void MnkWindow::saveCsv()
{
    if(m_currentFilepath == ""){
        saveAsCsv();
        return;
    }
    saveCsvGeneralFunction();
}

int MnkWindow::fitButtonClicked()
{
    QVector<long double> xLongVec;//vectors will contain coordinates from the data file (as well as coordinate errors)
    QVector<long double> yLongVec;
    QVector<long double> xLongVecErrs;
    QVector<long double> yLongVecErrs;
    QVector<QString> varNames;// names of x and y axis
    QVector<QString> varUnits;// units in which x and y are measured
    long double slope{}, intercept{};
    try{
        readData(varNames, varUnits, xLongVec, yLongVec, xLongVecErrs, yLongVecErrs);
        computate_equation(xLongVec, yLongVec, slope, intercept);
    }catch(const ComputationWarning& ex){
        QMessageBox::critical(this, "ERROR", QString::fromStdString((std::string)ex.what()));
        return -1;
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
    long double slopeError{};
    long double interceptError{};
    QString interceptSlopeText{""};
    computate_equation_error(xLongVec, yLongVec, slope, intercept, slopeError, interceptError);

    {//block for setting intercept-slope label, most of the job is to output double values in a scientific format and make it look clean otherwise
        std::ostringstream outDoubleFormatTextStream;
        outDoubleFormatTextStream << "Intercept =  ";
        if(((int)qFabs(intercept) / 10 || qFabs(intercept) < 1) && intercept){//if number is too big or too small and not a zero(zero also should be 0 and not 0.00000)
            outDoubleFormatTextStream << std::scientific << (double)intercept;
        }else{//removing trailing zeros:
            // Print value to a string
            std::stringstream ss;
            ss << std::fixed << intercept;
            std::string str = ss.str();
            // Ensure that there is a decimal point somewhere (there should be)
            if(str.find('.') != std::string::npos)
            {
                // Remove trailing zeroes
                str = str.substr(0, str.find_last_not_of('0')+1);
                // If the decimal point is now the last character, remove that as well
                if(str.find('.') == str.size()-1)
                {
                    str = str.substr(0, str.size()-1);
                }
            }
            outDoubleFormatTextStream << str;
        }
        outDoubleFormatTextStream << " ± ";
            if(((int)qFabs(interceptError) / 10 || qFabs(interceptError) < 1) && interceptError){//if number is too big or too small and not a zero(zero also should be 0 and not 0.00000)
            outDoubleFormatTextStream << std::scientific << (double)interceptError;
        }else{//removing trailing zeros:
            // Print value to a string
            std::stringstream ss;
            ss << std::fixed << interceptError;
            std::string str = ss.str();
            // Ensure that there is a decimal point somewhere (there should be)
            if(str.find('.') != std::string::npos)
            {
                // Remove trailing zeroes
                str = str.substr(0, str.find_last_not_of('0')+1);
                // If the decimal point is now the last character, remove that as well
                if(str.find('.') == str.size()-1)
                {
                    str = str.substr(0, str.size()-1);
                }
            }
            outDoubleFormatTextStream << str;
        }
        outDoubleFormatTextStream << "\nSlope =  ";
        if(((int)qFabs(slope) / 10 || qFabs(slope) < 1) && slope){//if number is too big or too small and not a zero(zero also should be 0 and not 0.00000)
            outDoubleFormatTextStream << std::scientific << (double)slope;
        }else{//removing trailing zeros:
            // Print value to a string
            std::stringstream ss;
            ss << std::fixed << slope;
            std::string str = ss.str();
            // Ensure that there is a decimal point somewhere (there should be)
            if(str.find('.') != std::string::npos)
            {
                // Remove trailing zeroes
                str = str.substr(0, str.find_last_not_of('0')+1);
                // If the decimal point is now the last character, remove that as well
                if(str.find('.') == str.size()-1)
                {
                    str = str.substr(0, str.size()-1);
                }
            }
            outDoubleFormatTextStream << str;
        }
        outDoubleFormatTextStream << " ± ";
            if(((int)qFabs(slopeError) / 10 || qFabs(slopeError) < 1) && slopeError){//if number is too big or too small and not a zero(zero also should be 0 and not 0.00000)
            outDoubleFormatTextStream << std::scientific << (double)slopeError;
        }else{//removing trailing zeros:
            // Print value to a string
            std::stringstream ss;
            ss << std::fixed << slopeError;
            std::string str = ss.str();
            // Ensure that there is a decimal point somewhere (there should be)
            if(str.find('.') != std::string::npos)
            {
                // Remove trailing zeroes
                str = str.substr(0, str.find_last_not_of('0')+1);
                // If the decimal point is now the last character, remove that as well
                if(str.find('.') == str.size()-1)
                {
                    str = str.substr(0, str.size()-1);
                }
            }
            outDoubleFormatTextStream << str;
        }
        interceptSlopeText = QString::fromStdString(outDoubleFormatTextStream.str());
        ui->label_slope_intercept->setText(interceptSlopeText);
    }

    //creating/recreating qCustomPlot:
    if(m_chart){
        m_chart->close();
        m_chart = nullptr;
    }
    m_chart = new QCustomPlot(this);
    ui->horizontalLayout_3->addWidget(m_chart);
    m_chart->setAttribute(Qt::WA_DeleteOnClose);
    m_chart->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //finding max/min X and Y:
    double minX = xLongVec.at(0);
    double maxX = xLongVec.at(0);
    double minY = yLongVec.at(0);
    double maxY = yLongVec.at(0);
    for(long double i : xLongVec){
        if(i < minX){
            minX = i;
        }
        if(i > maxX){
            maxX = i;
        }
    }
    for(long double i : yLongVec){
        if(i < minY){
            minY = i;
        }
        if(i > maxY){
            maxY = i;
        }
    }
    //bounds are 10% frame
    double maxXBound = maxX + ((maxX - minX)/10.0);
    double minXBound = minX - ((maxX - minX)/10.0);
    double maxYBound = maxY + ((maxY - minY)/10.0);
    double minYBound = minY - ((maxY - minY)/10.0);
    //setting m_chart display range:
    m_chart->xAxis->setRange(minXBound, maxXBound);
    m_chart->yAxis->setRange(minYBound, maxYBound);
    //for english decimal separator:
    m_chart->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
    //long double vectors to double vectors (Qt seemingly does not support long double):
    QVector<double> xVector;
    QVector<double> yVector;
    QVector<double> xErrVector;
    QVector<double> yErrVector;
    //x:
    for(int i = 0; i < xLongVec.size(); ++i){
        xVector.push_back(xLongVec.at(i));
    }
    //y:
    for(int i = 0; i < yLongVec.size(); ++i){
        yVector.push_back(yLongVec.at(i));
    }
    //y error:
    if(xLongVecErrs.size() == 1){//fixed error chosen
        for(int i = 0; i < xLongVec.size(); ++i){
            xErrVector.push_back(xLongVecErrs.at(0));
        }
    }else if(xLongVecErrs.size() > 1){
        for(int i = 0; i < xLongVec.size(); ++i){
            xErrVector.push_back(xLongVecErrs.at(i));
        }
    }
    //y error:
    if(yLongVecErrs.size() == 1){//fixed error chosen
        for(int i = 0; i < yLongVec.size(); ++i){
            yErrVector.push_back(yLongVecErrs.at(0));
        }
    }else if(yLongVecErrs.size() > 1){
        for(int i = 0; i < yLongVec.size(); ++i){
            yErrVector.push_back(yLongVecErrs.at(i));
        }
    }
    //approximation line graph:
    QPen ApproximationPen;
    ApproximationPen.setStyle(Qt::SolidLine);
    ApproximationPen.setWidth(3);
    ApproximationPen.setColor(QColor(214, 132, 58));//orange
    QCPItemStraightLine* approximationLine = new QCPItemStraightLine(m_chart);
    approximationLine->setPen(ApproximationPen);
    approximationLine->point1->setCoords(minXBound, slope * minXBound + intercept);//setting a line by 2 points
    approximationLine->point2->setCoords(maxXBound, slope * maxXBound + intercept);
    //scatter series(experimental data):
    QCPCurve* scatterSeries = new QCPCurve(m_chart->xAxis, m_chart->yAxis);
    scatterSeries->setPen(QPen(QColor(182,28,28)));//red
    scatterSeries->setLineStyle(QCPCurve::lsNone);
    scatterSeries->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 10));
    //horizontal error bar:
    QCPErrorBars *horizontalErrorBars = new QCPErrorBars(m_chart->yAxis, m_chart->xAxis);
    horizontalErrorBars->setAntialiased(false);
    horizontalErrorBars->setDataPlottable(scatterSeries);
    horizontalErrorBars->setPen(QPen(QColor(214, 207, 154)));//light grey
    //vertical error bar:
    QCPErrorBars *verticalErrorBars = new QCPErrorBars(m_chart->xAxis, m_chart->yAxis);
    verticalErrorBars->setAntialiased(false);
    verticalErrorBars->setDataPlottable(scatterSeries);
    verticalErrorBars->setPen(QPen(QColor(214, 207, 154)));//light grey
    // setting data:
    scatterSeries->setData(xVector, yVector);
    horizontalErrorBars->setData(xErrVector);
    verticalErrorBars->setData(yErrVector);
    // setup look of tick labels:
    m_chart->xAxis->ticker()->setTickCount(9);
    m_chart->xAxis->setNumberFormat("gbc");
    m_chart->yAxis->ticker()->setTickCount(9);
    m_chart->yAxis->setNumberFormat("gbc");
    // make top right axes clones of bottom left axes:
    m_chart->axisRect()->setupFullAxesBox();
    //axes labels:
    m_chart->xAxis->setLabel(varNames.at(0) + varUnits.at(0));
    m_chart->xAxis->setLabelFont(QFont("Times",14));
    m_chart->xAxis->setLabelColor(QColor(182,28,28));
    m_chart->yAxis->setLabel(varNames.at(1) + varUnits.at(1));
    m_chart->yAxis->setLabelFont(QFont("Times",14));
    m_chart->yAxis->setLabelColor(QColor(182,28,28));
    //coloring:
    m_chart->setBackground(QColor::fromRgb(46, 47, 48));
    //axis x:
    m_chart->xAxis->setTickPen(QPen(QColor(255,255,255)));
    m_chart->xAxis->setSubTickPen(QPen(QColor(255,255,255)));
    m_chart->xAxis->setBasePen(QPen(QColor(255,255,255)));
    m_chart->xAxis->setTickLabelColor(QColor(255,255,255));
    //axis y:
    m_chart->yAxis->setTickPen(QPen(QColor(255,255,255)));
    m_chart->yAxis->setSubTickPen(QPen(QColor(255,255,255)));
    m_chart->yAxis->setBasePen(QPen(QColor(255,255,255)));
    m_chart->yAxis->setTickLabelColor(QColor(255,255,255));
    //axis x2:
    m_chart->xAxis2->setTickPen(QPen(QColor(255,255,255)));
    m_chart->xAxis2->setSubTickPen(QPen(QColor(255,255,255)));
    m_chart->xAxis2->setBasePen(QPen(QColor(255,255,255)));
    //axis y2:
    m_chart->yAxis2->setTickPen(QPen(QColor(255,255,255)));
    m_chart->yAxis2->setSubTickPen(QPen(QColor(255,255,255)));
    m_chart->yAxis2->setBasePen(QPen(QColor(255,255,255)));
    return 0;
}
