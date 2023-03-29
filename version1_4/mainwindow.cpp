#include "mainwindow.h"
#include "ui_mainwindow.h"
//from here:
#include "secondwindow.h"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include "readfile.hpp"
#include "mnk.hpp"

#include <QtWidgets/QMainWindow>
#include <QtCharts>

#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create button
    button = new QPushButton("Click to fit", this);
    // hook up its clicked signal to a function in main
    connect(button, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    // move and resize it
    button->setGeometry(100, 100, 150, 100);
    // and show it
    button->show();



    lineEdit = new QLineEdit("prompt", this);
    lineEdit->setGeometry(10,10,400,50);
    lineEdit->show();


    QLabel *label = new QLabel(this);
    //label->geometry(100,10,200,50);
    label->setGeometry(450,10,200,50);
    label->setText("input a path\nto the file with data");
    label->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::buttonClicked(){
    //std::clog << "works\n";//works
    //std::clog << lineEdit->text();
    ////std::string a = lineEdit->text().toStdString();
    //std::clog <<  a;//works
    ////std::clog << a;
    /// //    SecondWindow secwin;
    //    secwin.setModal(true);//modal means disabling interaction with the previous one, even without this child window is modal cuz of exec
    //    secwin.show();// show makes it un-modal by default, but i set it to modal at l22, so modal
    //    secwin.exec();//this line should always be here
        //hide();//hides main window
        //secwin = new SecondWindow(this);//this means that mainwindow is the parent(when parent dies, child as well) (leave blank actually otherwise)

        std::vector<int> vec_X;
        std::vector<int> vec_Y;
        std::ifstream i_f;
        std::string path{""};
        path = lineEdit->text().toStdString();
        i_f.open(path);//"D:\\qt projects\\MNK\\version1_2\\text1.txt"
        func1(vec_X, vec_Y, i_f);
        double a1, b1;
        mnk1(vec_X, vec_Y, a1, b1);
    //    std::cout << a1 << std::endl;
    //    std::cout << b1 << std::endl;

    //    for (const auto& e : vec_X) {
    //        qDebug() << e << " ";
    //    }
    //    std::cout << std::endl;
    //    for (const auto& e : vec_Y) {
    //        qDebug() << e << " ";
    //    }



        //QApplication a(argc, argv);//

        QLineSeries *series = new QLineSeries();
        series->setName("line");
        for(int i = 0; i < vec_X.size(); i++){
            series->append(vec_X.at(i),vec_X.at(i)*a1+b1);
        }
    //    series->append(0,16);
    //    series->append(1,3);
    //    series->append(2,13);
    //    series->append(3.2,11.1);
    //    series->append(4,12);
    //    series->append(5,14);
    //    series->append(6,15);

        //by me:
        QScatterSeries *series1 = new QScatterSeries();
        series1->setName("scatter1");
        series1->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
        series1->setMarkerSize(10.0);
        for(int i = 0; i < vec_X.size(); i++){
            series1->append(vec_X.at(i),vec_Y.at(i));
        }
    //    series1->append(0,6);
    //    series1->append(1,11);
    //    series1->append(2,-3);
    //    series1->append(3,6);
    //    series1->append(4,5);
    //    series1->append(5,4);
    //    series1->append(6,9);

        QChart *chart = new QChart();
        //chart->legend()->hide();
        chart->addSeries(series);
        //by me:
        chart->addSeries(series1);
        //
        chart->createDefaultAxes();


        const QString title = QString::fromStdString("equation: y = "+std::to_string(a1)+" * x + "+std::to_string(b1));
        QFont font;
        font.setPixelSize(18);
        chart->setTitleFont(font);
        chart->setTitleBrush(QBrush(Qt::blue));
        chart->setTitle(title);

        QPen pen(QRgb(0x000000));
        pen.setWidth(5);
        series->setPen(pen);
        //by me:
        QPen pen1(QRgb(0xff0000));
        pen1.setWidth(5);
        series1->setPen(pen1);

        chart->setAnimationOptions(QChart::AllAnimations);

    //    QCategoryAxis *axisX = new QCategoryAxis();
    //    axisX->append("0",0);
    //    axisX->append("1",1);
    //    axisX->append("2",2);
    //    axisX->append("3",3);
    //    axisX->append("4",4);
    //    axisX->append("5",5);
    //    axisX->append("6",6);
    //    chart->setAxisX(axisX, series);
        //chart->setAxisX(axisX, series1);//idk



        //hide();//hides main window
        //secwin = new SecondWindow(this);
        //chart->show();

        QChartView *chartView = new QChartView(chart);
        //QChartView chartView(chart);//creates and deletes in a second
        //QChartView *chartView = new QChartView(this);
        chartView->setRenderHint(QPainter::Antialiasing);


        //chartView->set
        //SecondWindow secwin;
    //    secwin.setModal(true);//modal means disabling interaction with the previous one, even without this child window is modal cuz of exec
    //    secwin.show();// show makes it un-modal by default, but i set it to modal at l22, so modal
    //    secwin.exec();//this line should always be here
        //hide();//hides main window
        //this means that mainwindow is the parent(when parent dies, child as well) (leave blank actually otherwise)
        //QMainWindow w;

        QStackedWidget *stackedWidget = new QStackedWidget(this);
        secwin = new SecondWindow(stackedWidget);
        //stackedWidget->addWidget(secwin);
        stackedWidget->addWidget(chartView);


        //chartView->resize(420,300);
        //chartView->show();
        stackedWidget->setGeometry(250,175,500,350);
        //stackedWidget->resize(420, 300);
        stackedWidget->show();

        //secwin->resize(420,300);//

        //secwin->show();//model-less aproach


        //code is with memory leaks somewhere
        //actually if i close all windows manually there are no leaks
}

