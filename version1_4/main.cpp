#include "mainwindow.h"

#include <QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts>


#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include "readfile.hpp"
#include "mnk.hpp"
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
