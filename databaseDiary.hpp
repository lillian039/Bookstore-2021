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
#include "databaseBook.hpp"

//记录财务信息
class FinanceRecord {
private:
    int time;
    double *money;
    double deal;
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

    double *totalFinance(int time_) {
        financeFile.seekg(0);
        financeFile.read(reinterpret_cast<char *>(&time), sizeof(int));
        if (time_ > time)throw MyError();
        money = new double[time_];//todo 记得delete
        financeFile.seekg(sizeof(int) + (time - time_) * sizeof(double));
        financeFile.read(reinterpret_cast<char *>(money), time_ * sizeof(double));
        return money;
    }

    int totalTime() {
        financeFile.seekg(0);
        financeFile.read(reinterpret_cast<char *>(&time), sizeof(int));
        return time;
    }

    void addNewFiance(double money_) {
        financeFile.seekg(0);
        financeFile.read(reinterpret_cast<char *>(&time), sizeof(int));
        deal = money_;
        financeFile.seekg(sizeof(int) + time * sizeof(double));
        financeFile.write(reinterpret_cast<char *>(&deal), sizeof(double));
        time++;
        financeFile.seekg(0);
        financeFile.write(reinterpret_cast<char *>(&time), sizeof(double));
    }
};

struct StuffIndex {
    char index[31];
    int value;

    StuffIndex() = default;

    friend bool operator<(const StuffIndex &lhs, const StuffIndex &rhs) {
        return string(lhs.index) < string(rhs.index);
    }

    friend bool operator>(const StuffIndex &lhs, const StuffIndex &rhs) {
        return string(lhs.index) > string(rhs.index);
    }

    friend bool operator>=(const StuffIndex &lhs, const StuffIndex &rhs) {
        return string(lhs.index) >= string(rhs.index);
    }

    friend bool operator<=(const StuffIndex &lhs, const StuffIndex &rhs) {
        return string(lhs.index) <= string(rhs.index);
    }

};

struct Operat {
    char operate[1026];//单条指令长达1025

    Operat(std::string &cmd) {
        strcpy(operate, cmd.c_str());
    }

    Operat() = default;
};

class StuffDatabase {
private:
    Operat operat;
    string filename;
    fstream stufffile;
    int n;
public:
    StuffDatabase() {
        stufffile.open("StuffReport");
        if (!stufffile) {
            stufffile.open("StuffReport", ios_base::out);
            n = 0;
            stufffile.close();
            stufffile.open("StuffReport");
            stufffile.write(reinterpret_cast<char *>(&n), sizeof(n));
        }
    }

    ~StuffDatabase() {
        stufffile.close();
    }

    Operat findInf(int &num) {
        stufffile.seekg((num - 1) * sizeof(Operat) + sizeof(int));
        stufffile.read(reinterpret_cast<char *>(&operat), sizeof(Operat));
        return operat;
    }

    int InsertInf(Operat &operat) {
        stufffile.seekg(0);
        stufffile.read(reinterpret_cast<char *>(&n), sizeof(int));
        n += 1;
        stufffile.seekg(0);
        stufffile.write(reinterpret_cast<char *>(&n), sizeof(int));
        stufffile.seekg((n - 1) * sizeof(Operat) + sizeof(int));
        stufffile.write(reinterpret_cast<char *>(&operat), sizeof(Operat));
        return n;
    }

};

class MyDearStuffs {
private:
    char StuffName[31];
    fstream stufffile;
    int n;
public:
    MyDearStuffs() {
        stufffile.open("MyStuffs");
        if (!stufffile) {
            stufffile.open("MyStuffs", ios_base::out);
            n = 0;
            stufffile.close();
            stufffile.open("MyStuffs");
            stufffile.write(reinterpret_cast<char *>(&n), sizeof(n));
        }
    }

    ~MyDearStuffs() {
        stufffile.close();
    }

    vector<string> findAllMyStuffs() {
        vector<string> stuffs;
        stufffile.seekg(0);
        stufffile.read(reinterpret_cast<char *>(&n), sizeof(int));
        for (int i = 0; i < n; i++) {
            stufffile.read(reinterpret_cast<char *>(&StuffName), sizeof(char) * 31);
            stuffs.push_back(string(StuffName));
        }
        return stuffs;
    }

    void InsertInf(string &name) {
        strcpy(StuffName, name.c_str());
        stufffile.seekg(0);
        stufffile.read(reinterpret_cast<char *>(&n), sizeof(int));
        n += 1;
        stufffile.seekg(0);
        stufffile.write(reinterpret_cast<char *>(&n), sizeof(int));
        stufffile.seekg((n - 1) * sizeof(char) * 31 + sizeof(int));
        stufffile.write(reinterpret_cast<char *>(&StuffName), sizeof(char) * 31);
    }
};

#endif //BOOKSTORE_DATABASEDIARY_HPP
