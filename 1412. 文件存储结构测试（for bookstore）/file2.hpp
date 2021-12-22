#ifndef INC_1412__FOR_BOOKSTORE_FILE2_HPP
#define INC_1412__FOR_BOOKSTORE_FILE2_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <unistd.h>

#define MAXSIZE 300
#define MINSIZE 0

using namespace std;

struct DataBase {
    char index[65];
    int value;

    void Initialize(string index_, int value_) {
        strcpy(index, index_.c_str());
        value = value_;
    }

    friend bool operator<(const DataBase &lhs, const DataBase &rhs) {
        if (string(lhs.index) < string(rhs.index))return true;
        if (string(lhs.index) > string(rhs.index))return false;
        if (lhs.value < rhs.value)return true;
        return false;
    }

    friend bool operator>(const DataBase &lhs, const DataBase &rhs) {
        if (string(lhs.index) > string(rhs.index))return true;
        if (string(lhs.index) < string(rhs.index))return false;
        if (lhs.value > rhs.value)return true;
        return false;
    }

    bool operator<=(const DataBase &rhs) {
        if (string(index) < string(rhs.index))return true;
        if (string(index) == string(rhs.index) && value <= rhs.value)return true;
        return false;
    }

    bool operator>=(const DataBase &rhs) {
        if (string(index) > string(rhs.index))return true;
        if (string(index) == string(rhs.index) && value >= rhs.value)return true;
        return false;
    }

    bool operator>(const DataBase &rhs) {
        if (string(index) > string(rhs.index))return true;
        if (string(index) < string(rhs.index))return false;
        if (value > rhs.value)return true;
        return false;
    }
};

struct IndexBase {
    int next = 0;
    int size = 0;
    DataBase node[MAXSIZE];
};

class Ull {
private:
    IndexBase indexbase;
    DataBase database;
    fstream file_catalogue;
    string catalogue_name;
public:
    Ull(string catalogue_name_) {
        catalogue_name = catalogue_name_;
        file_catalogue.open(catalogue_name);
    }

    ~Ull() {
        file_catalogue.close();
    }

    static bool compare(const DataBase &lhs, const DataBase &rhs) {
        if (lhs < rhs)return true;
        return false;
    }

    void InsertValue(string index, int value) {
        file_catalogue.seekg(0, ios::beg);
        if (!file_catalogue)InitializeFirstOpen();
        database.Initialize(index, value);
        int catalogue_rear;//共有几个块
        int index_now = 0;//当前指针位置
        file_catalogue.read(reinterpret_cast<char *>(&catalogue_rear), sizeof(int));
        if (catalogue_rear == 0) {
            InitializeNoneBlock(catalogue_rear);
            return;
        } else {
            file_catalogue.seekg(sizeof(int), ios::beg);
            indexbase.next = 1;
            while (indexbase.next != 0) {
                file_catalogue.seekg(index_now * sizeof(IndexBase) + sizeof(int), ios::beg);
                file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase));
                if (database <= indexbase.node[indexbase.size - 1] ||
                    indexbase.next == 0) {
                    indexbase.node[indexbase.size] = database;
                    long long n;
                    n = (upper_bound(indexbase.node, indexbase.node + indexbase.size, database, compare) -
                         indexbase.node);
                    for (int i = indexbase.size; i > n; i--) {
                        indexbase.node[i] = indexbase.node[i - 1];
                    }
                    indexbase.node[n] = database;
                    indexbase.size++;
                    break;
                }
                index_now = indexbase.next;
            }
            if (indexbase.size == MAXSIZE)splitBlock(catalogue_rear);
        }
        file_catalogue.seekg((index_now) * sizeof(IndexBase) + sizeof(int), ios::beg);
        file_catalogue.write(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase));

    }

    void FindValue(const string index) {
        database.Initialize(index, 0);
        file_catalogue.seekg(0, ios::beg);
        if (!file_catalogue) {
            cout << "null" << endl;
            return;
        }
        vector<int> value_;
        int catalogue_rear;
        int index_now = 0;
        file_catalogue.read(reinterpret_cast<char *>(&catalogue_rear), sizeof(int));
        if (catalogue_rear == 0) {
            cout << "null" << endl;
            return;
        } else {
            indexbase.next = 1;//enter loop
            bool flag = false;
            while (indexbase.next != 0) {
                file_catalogue.seekg(sizeof(int) + index_now * sizeof(IndexBase), ios::beg);
                file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase));
                if (string(indexbase.node[0].index) <= index &&
                    index <= string(indexbase.node[indexbase.size - 1].index)) {
                    long long n;
                    n = (lower_bound(indexbase.node, indexbase.node + indexbase.size, database, compare) -
                         indexbase.node);
                    for (int i = n; i < indexbase.size; i++) {
                        if (indexbase.node[i].index != index)break;
                        flag = true;
                        value_.push_back(indexbase.node[i].value);
                    }
                    if (string(indexbase.node[indexbase.size - 1].index) != index)break;
                }
                index_now = indexbase.next;
            }
            if (!flag) {
                cout << "null" << '\n';
                return;
            }
            cout << value_[0];
            for (int j = 1; j < value_.size(); j++) {
                cout << ' ' << value_[j];
            }
            cout << '\n';
            return;
        }
    }

    void DeleteValue(const string index, int value) {
        file_catalogue.seekg(0, ios::beg);
        if (!file_catalogue) {
            return;
        }
        int catalogue_rear;
        file_catalogue.read(reinterpret_cast<char *>(&catalogue_rear), sizeof(int));
        if (catalogue_rear == 0) {
            return;
        }
        database.Initialize(index, value);
        indexbase.next = 1;//enter loop
        int index_now = 0;
        while (indexbase.next != 0) {
            file_catalogue.seekg(sizeof(int) + index_now * sizeof(IndexBase), ios::beg);
            file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase));
            if (indexbase.size == 0) {
                index_now = indexbase.next;
                continue;
            }
            if (index < string(indexbase.node[0].index))return;
            if (indexbase.node[0] <= database && database <= indexbase.node[indexbase.size - 1]) {
                long long n;
                n = (lower_bound(indexbase.node, indexbase.node + indexbase.size, database, compare) -
                     indexbase.node);
                if (indexbase.node[n].index != index || indexbase.node[n].value != value)return;
                for (int j = n; j < indexbase.size - 1; j++) {
                    indexbase.node[j] = indexbase.node[j + 1];
                }
                indexbase.size--;
                file_catalogue.seekg(sizeof(int) + index_now * sizeof(IndexBase), ios::beg);
                file_catalogue.write(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase));
                return;
            }
            index_now = indexbase.next;
        }
    }

    void InitializeFirstOpen() {
        file_catalogue.open(catalogue_name, ios_base::out);
        int rear = 0;
        file_catalogue.write(reinterpret_cast<char *>(&rear), sizeof(int));
        file_catalogue.close();
        file_catalogue.open(catalogue_name);
    }

    void InitializeNoneBlock(int &catalogue_rear) {
        indexbase.next = 0;
        indexbase.size = 1;
        file_catalogue.seekg(0, ios::beg);
        catalogue_rear++;
        indexbase.node[0] = database;
        file_catalogue.write(reinterpret_cast<char *>(&catalogue_rear), sizeof(int));
        file_catalogue.write(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase));
    }

    void splitBlock(int &catalogue_rear) {
        IndexBase new_index;
        new_index.next = indexbase.next;
        indexbase.next = catalogue_rear;
        for (int i = MAXSIZE / 2; i < MAXSIZE; i++) {
            new_index.node[i - (MAXSIZE / 2)] = indexbase.node[i];
        }
        new_index.size = MAXSIZE / 2;
        indexbase.size = MAXSIZE / 2;
        file_catalogue.seekg((catalogue_rear) * sizeof(IndexBase) + sizeof(int), ios::beg);
        file_catalogue.write(reinterpret_cast<char *>(&new_index), sizeof(IndexBase));
        catalogue_rear += 1;
        file_catalogue.seekg(0, ios::beg);
        file_catalogue.write(reinterpret_cast<char *>(&catalogue_rear), sizeof(int));
    }
    void printindexbase() {
        file_catalogue.seekg(0, ios::beg);
        if (!file_catalogue)return;
        int n;
        file_catalogue.read(reinterpret_cast<char *>(&n), sizeof(int));
        cout << n << endl;
        while (true) {
            file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase));
            cout << indexbase.next << " " << indexbase.size << " ";
            cout << endl;
            for (int i = 0; i < indexbase.size; i++) {
                cout << string(indexbase.node[i].index) << " " <<indexbase.node[i].value << ' ';
            }
            cout << endl;
            if (indexbase.next == 0)break;
            file_catalogue.seekg(sizeof(int) + indexbase.next * sizeof(IndexBase), ios::beg);
        }
    }



};

#endif //INC_1412__FOR_BOOKSTORE_FILE2_HPP
