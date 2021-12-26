#ifndef BOOKSTORE_DATABASEDIARY_HPP
#define BOOKSTORE_DATABASEDIARY_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include "error.hpp"

//记录财务信息
class FinanceRecord {
private:
    int time;
    int *money;
    int deal;
    fstream financeFile;
public:
    FinanceRecord() {
        financeFile.open("FinanceFile");
        if (!financeFile) {
            financeFile.open("FinanceFile", ios_base::out);
            financeFile.close();
            financeFile.open("FinanceFile");
            financeFile.seekg(0);
            time = 0;
            financeFile.write(reinterpret_cast<char *>(&time), sizeof(int));
        }
    }

    int* totalFinance(int time_) {
        financeFile.seekg(0);
        financeFile.read(reinterpret_cast<char *>(&time), sizeof(int));
        if (time_ > time)throw MyError();
        money = new int[time_];//todo 记得delete
        financeFile.seekg(sizeof(int)+(time-time_)*sizeof(int));
        financeFile.read(reinterpret_cast<char *>(money), time_ * sizeof(int));
        return money;
    }

    int totalTime() {
        financeFile.seekg(0);
        financeFile.read(reinterpret_cast<char *>(&time), sizeof(int));
        return time;
    }

    void addNewFiance(int money_) {
        financeFile.seekg(0);
        financeFile.read(reinterpret_cast<char *>(&time), sizeof(int));
        deal = money_;
        financeFile.seekg(sizeof(int) + time * sizeof(int));
        financeFile.write(reinterpret_cast<char *>(&deal), sizeof(int));
        time++;
        financeFile.seekg(0);
        financeFile.write(reinterpret_cast<char *>(&time), sizeof(int));
    }
};

/*class StuffOperat{

};

struct StuffIndex{
    char stuffname[31];
    int index;
};

struct Operat{
    char operate[1025];//单条指令长达1025
};
class StuffDatabase{

};*/
#endif //BOOKSTORE_DATABASEDIARY_HPP
