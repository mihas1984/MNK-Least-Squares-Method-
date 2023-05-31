#include "skvwindow.h"
#include "ui_skvwindow.h"
#include "student_coeffs.hpp"
#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <QCloseEvent>
#include <QMessageBox>

SkvWindow::SkvWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SkvWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    //for red exit button:
    QPalette Pal(palette());
    Pal.setColor(QPalette::Button, QColor(33,0,0));
    ui->exitButton->setAutoFillBackground(true);
    ui->exitButton->setPalette(Pal);
    //connecting
    connect(ui->pushButton, &QPushButton::clicked, this, &SkvWindow::calculateClicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &SkvWindow::exitClicked);
}

SkvWindow::~SkvWindow()
{
    delete ui;
}

int SkvWindow::calculateClicked()
{
    std::vector<double> data{};
    double avg{};
    double avg_err{};
    double student_coeff{};
    double min{};
    double max{};
    double median{};
    if((read(data)) || (calculate(data, avg, avg_err, student_coeff, min, max, median))){
        return -1;//with errors
    }
    QString result = QString::number(avg) + " ± " + QString::number(avg_err);
    ui->resultLabel->setText(result);
    ui->countLabel->setText(QString::number(data.size()));
    ui->medianLabel->setText(QString::number(median));
    QString range =  QString::number(min) + " <-> " + QString::number(max);
    ui->rangeLabel->setText(range);
    ui->studentCoeffLabel->setText(QString::number(student_coeff));
    return 0;
}

void SkvWindow::exitClicked()
{
    close();
}

void SkvWindow::closeEvent(QCloseEvent *event)
{
    emit closes();
    event->accept();
}

int SkvWindow::read(std::vector<double> &input)
{
    input.clear();
    QString qtext = ui->textEdit->toPlainText();
    std::string text = qtext.toStdString();
    std::replace(text.begin(), text.end(), ',', '.');//replacing commas with dots if such available
    std::stringstream ss(text);
    std::string tmp{""};
    while(ss >> tmp){
        try{
            input.push_back(std::stod(tmp));
        }catch(...){
            QString errormessage = "error reading floating point numbers from the box, please make sure input data is clean.";
            QMessageBox::critical(this, "ERROR", errormessage);
            return -1;
        }
    }
    return 0;
}

int SkvWindow::calculate(const std::vector<double> data, double &avg, double &avg_err, double &student, double& min, double& max, double& median)
{
    std::vector<double> input = data;
    int inputsize = (int)input.size();
    if(inputsize < 2){
        QString errormessage = "there should be at least 2 input points.\n(nothing will be calculated)";
        QMessageBox::critical(this, "ERROR", errormessage);
        return -1;
    }
    std::sort(input.begin(), input.end());
    min = input.at(0);
    max = input.at(inputsize-1);
    if(inputsize % 2){
        median = input.at(inputsize/(int)2);
    }else{
        median = (input.at(inputsize/2) + input.at(inputsize/2-1))/2.0;
    }

    double sum{};
    for(int i = 0; i < inputsize; i++){
        sum += input.at(i);
    }
    avg = sum/(double)inputsize;
    if(inputsize > 120){
        student = student_coeffs[34];
    }else if(inputsize > 60){
        student = student_coeffs[33];
    }else if(inputsize > 40){
        student = student_coeffs[32];
    }else if(inputsize > 30){
        student = student_coeffs[31];
    }else{
        student = student_coeffs[inputsize];
    }
    double sum_dev_squares{};
    for(int i = 0; i < inputsize; i++){
        sum_dev_squares += (avg - input.at(i))*(avg - input.at(i));
    }
    double delim = inputsize*(inputsize -1);
    avg_err = student * std::sqrt(sum_dev_squares/(double)delim);
    return 0;
}
