#ifndef INC_1412__FOR_BOOKSTORE_FILEDATA_HPP
#define INC_1412__FOR_BOOKSTORE_FILEDATA_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <unistd.h>

using namespace std;

struct DataBase {
    char index[65];
    int value;

    DataBase(string index_, int value_) {
        strcpy(index, index_.c_str());
        value = value_;
    }

    DataBase() = default;
};

struct IndexBase {
    int head;
    int rear;
    int next = 0;
    int front = 0;
    int size = 0;
    int index[150] = {0};//每个块存储150个地址
};

class fileData {
private:
    IndexBase index_;
    fstream file_index;
    fstream file_database;
public:
    fileData() = default;

    ~fileData() = default;

    void FindValue(const string index, string index_name, string file_name) {
        file_index.open(index_name);
        file_database.open(file_name);
        int index_head, index_rear, index_next, index_now;
        file_index.seekg(0, ios::beg);
        file_index.read(reinterpret_cast<char *>(&index_now), sizeof(int));
        InitializeIndexHead(index_now, index_head, index_rear, index_next);
        string head_, rear_;
        while (index_now != 0) {
            head_ = findString(index_head);
            rear_ = findString(index_rear);
            if (index >= head_ && index <= rear_) {//在范围内
                vector<int> value;
                file_index.seekg((index_now - 1) * sizeof(IndexBase) + sizeof(int) * 2, ios::beg);
                file_index.read(reinterpret_cast<char *>(&index_), sizeof(IndexBase));//将整个index拷贝下来
                bool flag = false;
                for (int i = 0; i < index_.size; i++) {
                    string name;
                    name = findString(index_.index[i]);
                    if (name != index && flag && i != index_.size - 1) {
                        sort(value.begin(), value.end());
                        for (int j = 0; j < value.size(); j++) {
                            cout << value[j] << ' ';
                        }
                        cout << endl;
                        return;
                    }
                    if (name == index) {
                        value.push_back(findInt(index_.index[i]));
                        flag = true;
                        if (i == index_.size - 1) {
                            sort(value.begin(), value.end());
                            for (int j = 0; j < value.size(); j++) {
                                cout << value[j] << ' ';
                            }
                            cout << endl;
                            return;
                        }
                    }
                }
                cout << "null" << endl;
                return;
            } else {
                index_now = index_next;
                InitializeIndexHead(index_now, index_head, index_rear, index_next);
            }
        }
        cout << "null" << endl;
        file_index.close();
        file_database.close();
        return;
    }

    string findString(int position) {
        char index1[65];
        file_database.seekg(sizeof(int) + (position - 1) * sizeof(DataBase), ios::beg);
        file_database.read(reinterpret_cast<char *>(&index1), sizeof(char) * 65);
        return string(index1);
    }

    int findInt(int position) {
        int value;
        file_database.seekg(sizeof(int) + (position - 1) * sizeof(DataBase) + offsetof(DataBase, value), ios::beg);
        file_database.read(reinterpret_cast<char *>(&value), sizeof(int));
        return value;
    }

    void InitializeFirstOpen(string index_name, string file_name) {
        int n = 0;
        file_database.open(file_name, ios_base::out);
        file_index.open(index_name, ios_base::out);
        file_database.seekg(0, ios::beg);
        file_index.seekg(0, ios::beg);
        file_database.write(reinterpret_cast<char *>(&n), sizeof(int));//rear地址为0
        file_index.write(reinterpret_cast<char *>(&n), sizeof(int));//head地址为0
        file_index.write(reinterpret_cast<char *>(&n), sizeof(int));//rear地址为0
        file_database.close();
        file_index.close();
        file_database.open(file_name);
        file_index.open(index_name);
    }

    void InitializeFileIndex(int &head_index, int &rear_index) {
        file_index.seekg(0, ios::beg);
        file_index.read(reinterpret_cast<char *>(&head_index), sizeof(int));
        file_index.read(reinterpret_cast<char *>(&rear_index), sizeof(int));
    }

    void InitializeIndexHead(int &index_, int &index_head, int &index_rear, int &index_next) {
        file_index.seekg((index_ - 1) * sizeof(IndexBase) + sizeof(int) * 2, ios::beg);
        file_index.read(reinterpret_cast<char *>(&index_head), sizeof(int));
        file_index.read(reinterpret_cast<char *>(&index_rear), sizeof(int));
        file_index.read(reinterpret_cast<char *>(&index_next), sizeof(int));
    }

    void InsertValue(string index, int value, string index_name, string file_name) {
        DataBase data_(index, value);//初始化要导入的信息
        file_index.open(index_name);
        file_database.open(file_name);
        if (!file_database) InitializeFirstOpen(index_name, file_name);//进行一波初始化
        int rear_data;//file中尾部的位置
        int position;//file中新放入的数据的位置
        file_database.seekg(0, ios::beg);
        file_database.read(reinterpret_cast<char *>(&rear_data), sizeof(int));
        file_database.seekg(sizeof(int) + rear_data * sizeof(DataBase), ios::beg);
        file_database.write(reinterpret_cast<char *>(&data_), sizeof(DataBase));
        file_database.seekg(0, ios::beg);
        rear_data++;
        position = rear_data;
        file_database.write(reinterpret_cast<char *>(&rear_data), sizeof(int));
        //构造index文件
        int head_index, rear_index;
        InitializeFileIndex(head_index, rear_index);//块头 块尾
        if (head_index == 0) {//进行初始化
            position = 1;
            index_.size = 1;
            index_.index[0] = position;
            index_.next = 0;
            index_.head = position;
            index_.rear = position;
            head_index = rear_index = 1;
            file_index.seekg(0, ios::beg);
            file_index.write(reinterpret_cast<char *>(&head_index), sizeof(int));
            file_index.write(reinterpret_cast<char *>(&rear_index), sizeof(int));
            file_index.write(reinterpret_cast<char *>(&index_), sizeof(IndexBase));
            file_database.close();
            file_index.close();
            return;
        } else {//找到head
            int index_head, index_rear, index_next, index_now;
            InitializeIndexHead(head_index, index_head, index_rear, index_next);
            index_now = head_index;
            string head_, rear_;
            while (true) {
                head_ = findString(index_head);
                rear_ = findString(index_rear);
                if ((index >= head_ && index <= rear_) || index_next == 0) {
                    file_index.seekg((index_now - 1) * sizeof(IndexBase) + sizeof(int) * 2, ios::beg);
                    file_index.read(reinterpret_cast<char *>(&index_), sizeof(IndexBase));
                    index_.index[index_.size] = position;
                    for (int i = 0; i < index_.size; i++) {
                        string name;
                        name = findString(index_.index[i]);
                        if (name > index) {
                            for (int j = index_.size; j > i; j--) {
                                index_.index[j] = index_.index[j - 1];
                            }
                            index_.index[i] = position;//刷新index[150]
                            break;
                        }
                    }
                    index_.size++;
                    index_.head = index_.index[0];
                    index_.rear = index_.index[index_.size - 1];
                    break;
                } else {
                    index_now = index_next;
                    InitializeIndexHead(index_now, index_head, index_rear, index_next);
                }
            }
            if (index_.size == 150) {//index_now rear_index
                IndexBase new_index;
                new_index.front = index_now;
                index_.next = rear_index;
                for (int i = 74; i < 150; i++) {
                    new_index.index[i - 74] = index_.index[i];
                }
                new_index.size = 75;
                index_.size = 75;
                new_index.head = new_index.index[0];
                new_index.rear = new_index.index[74];
                index_.head = index_.index[0];
                index_.rear = index_.index[74];
                file_index.seekg((index_rear - 1) * sizeof(IndexBase) + sizeof(int) * 2, ios::beg);
                index_rear++;
                file_index.seekg(sizeof(int), ios::beg);
                file_index.write(reinterpret_cast<char *>(&index_rear), sizeof(int));
            }
            file_index.seekg((index_now - 1) * sizeof(IndexBase) + sizeof(int) * 2, ios::beg);
            file_index.write(reinterpret_cast<char *>(&index_), sizeof(IndexBase));//刷新更新后的数据
        }
        file_database.close();
        file_index.close();
    }

    void DeleteValue(const string index, int value, string index_name, string file_name) {
        file_index.open(index_name);
        file_database.open(file_name);
        int index_head, index_rear, index_next, index_now;
        file_index.seekg(0, ios::beg);
        file_index.read(reinterpret_cast<char *>(&index_now), sizeof(int));
        InitializeIndexHead(index_now, index_head, index_rear, index_next);
        string head_, rear_;
        while (index_now != 0) {
            head_ = findString(index_head);
            rear_ = findString(index_rear);
            if (index >= head_ && index <= rear_) {//在范围内
                file_index.seekg((index_now - 1) * sizeof(IndexBase) + sizeof(int) * 2, ios::beg);
                file_index.read(reinterpret_cast<char *>(&index_), sizeof(IndexBase));//将整个index拷贝下来
                for (int i = 0; i < index_.size; i++) {
                    string name;
                    name = findString(index_.index[i]);
                    if (name == index) {
                        int value_;
                        value_ = findInt(index_.index[i]);
                        if (value_ == value) {
                            for (int j = i; j < index_.size; j++) {
                                index_.index[j] = index_.index[j + 1];
                            }
                            index_.size--;
                            index_.head = index_.index[0];
                            index_.rear = index_.index[index_.size - 1];
                            file_index.seekg((index_now - 1) * sizeof(IndexBase) + sizeof(int) * 2, ios::beg);
                            file_index.write(reinterpret_cast<char *>(&index_), sizeof(IndexBase));//将整个index写上去
                            file_database.close();
                            file_index.close();
                            return;
                        }
                    }
                }
                cout << "null" << endl;
                file_database.close();
                file_index.close();
                return;
            } else {
                index_now = index_next;
                InitializeIndexHead(index_now, index_head, index_rear, index_next);
            }
        }
        cout << "null" << endl;
        file_index.close();
        file_database.close();
        return;
    }
};


#endif //INC_1412__FOR_BOOKSTORE_FILEDATA_HPP
