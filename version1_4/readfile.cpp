#include "readfile.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <QDebug>

void func1(std::vector<int>& vec_X, std::vector<int>& vec_Y, std::ifstream& i_f){
    try{

        if (i_f)
        {
            std::stringstream ss;
            ss << i_f.rdbuf();
            std::string part;
            int i_part = 0;
            while (std::getline(ss, part, ' ')) {
                i_part++;
                std::stringstream part_stream(part);
                std::string data;

                        if (i_part == 1){
                            while(std::getline(part_stream, data, '-')){
                               vec_X.push_back(stoi(data));
                            }
                        }
                        else if (i_part == 2){
                            while(std::getline(part_stream, data, '-')){
                               vec_Y.push_back(stoi(data));
                            }
                        }
            }
        }
        else
        {
            //qDebug("File not found.\n");
            i_f.close();
        }
        i_f.close();


    }catch(std::invalid_argument&){
        //qDebug("invalid data\n");
    }
}
