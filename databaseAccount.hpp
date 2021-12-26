#ifndef BOOKSTORE_DATABASEACCOUNT_HPP
#define BOOKSTORE_DATABASEACCOUNT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include "error.hpp"
using namespace std;
#define MAXSIZE 100

struct UserInfValue {
    char password[31];
    char user_name[31];
    int Priority;

    UserInfValue(string password_, string user_name_, int priority) : Priority(priority) {
        strcpy(password, password_.c_str());
        strcpy(user_name, user_name_.c_str());
    }

    UserInfValue() = default;
};

//账户信息
struct UserInf {
    char index[31];//userid
    UserInfValue value;

    explicit UserInf(string user_ID_, UserInfValue value_) {
        strcpy(index, user_ID_.c_str());
        value = value_;
    }

    UserInf() = default;

    void Initialize(string user_ID_, UserInfValue userInfValue) {
        strcpy(index, user_ID_.c_str());
        value = userInfValue;
    }

    friend bool operator<(const UserInf &lhs, const UserInf &rhs) {
        return string(lhs.index) < string(rhs.index);
    }

    friend bool operator>(const UserInf &lhs, const UserInf &rhs) {
        return string(lhs.index) > string(rhs.index);
    }

    friend bool operator>=(const UserInf &lhs, const UserInf &rhs) {
        return string(lhs.index) >= string(rhs.index);
    }

    friend bool operator<=(const UserInf &lhs, const UserInf &rhs) {
        return string(lhs.index) <= string(rhs.index);
    }
};

//账户信息读写
class AccountInf {
private:
    IndexBase<UserInf> indexbase;
    UserInf database;
    fstream file_catalogue;
    string catalogue_name;
public:
    AccountInf(string catalogue_name_) {
        catalogue_name = catalogue_name_;
        file_catalogue.open(catalogue_name);
        if (!file_catalogue) {
            file_catalogue.open(catalogue_name, ios_base::out);
            int rear = 0;
            file_catalogue.write(reinterpret_cast<char *>(&rear), sizeof(int));
            file_catalogue.close();
            file_catalogue.open(catalogue_name);
            indexbase.next = 0;
            indexbase.size = 1;
            file_catalogue.seekg(0, ios::beg);
            int n = 1;
            UserInf root;
            UserInfValue boss("sjtu", "root", 7);
            root.Initialize("root", boss);
            database = root;
            indexbase.node[0] = database;
            file_catalogue.write(reinterpret_cast<char *>(&n), sizeof(int));
            file_catalogue.write(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<UserInf>));
        }
    }

    ~AccountInf() {
        file_catalogue.close();
    }


    void InsertValue(UserInf &database_) {
        file_catalogue.seekg(0, ios::beg);
        database = database_;
        int catalogue_rear;//共有几个块
        int index_now = 0;//当前指针位置
        file_catalogue.read(reinterpret_cast<char *>(&catalogue_rear), sizeof(int));
        file_catalogue.seekg(sizeof(int), ios::beg);
        indexbase.next = 1;
        while (indexbase.next != 0) {
            file_catalogue.seekg(index_now * sizeof(IndexBase<UserInf>) + sizeof(int), ios::beg);
            file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<UserInf>));
            if (database <= indexbase.node[indexbase.size - 1] ||
                indexbase.next == 0) {
                indexbase.node[indexbase.size] = database;
                long long n;
                n = (upper_bound(indexbase.node, indexbase.node + indexbase.size, database) -
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
        file_catalogue.seekg((index_now) * sizeof(IndexBase<UserInf>) + sizeof(int), ios::beg);
        file_catalogue.write(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<UserInf>));

    }

    UserInf FindValue(string &user_id) {
        file_catalogue.seekg(0, ios::beg);
        strcpy(database.index, user_id.c_str());
        int catalogue_rear;
        int index_now = 0;
        file_catalogue.read(reinterpret_cast<char *>(&catalogue_rear), sizeof(int));

        if (catalogue_rear == 0) {
            return database;
        } else {
            indexbase.next = 1;//enter loop
            while (indexbase.next != 0) {
                file_catalogue.seekg(sizeof(int) + index_now * sizeof(IndexBase<UserInf>), ios::beg);
                file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<UserInf>));
                if (string(indexbase.node[0].index) <= string(user_id) &&
                    string(user_id) <= string(indexbase.node[indexbase.size - 1].index)) {
                    long long n;
                    n = (lower_bound(indexbase.node, indexbase.node + indexbase.size, database) -
                         indexbase.node);
                    if (string(indexbase.node[n].index) != user_id) {
                        strcpy(database.index, "null");
                        return database;
                    } else database = indexbase.node[n];
                    return database;
                }
                index_now = indexbase.next;
            }
        }
        strcpy(database.index, "null");
        return database;
    }

    void DeleteValue(string &user_id) {
        file_catalogue.seekg(0, ios::beg);
        int catalogue_rear;
        file_catalogue.read(reinterpret_cast<char *>(&catalogue_rear), sizeof(int));
        if (catalogue_rear == 0) {
            return;
        }
        indexbase.next = 1;//enter loop
        int index_now = 0;
        while (indexbase.next != 0) {
            file_catalogue.seekg(sizeof(int) + index_now * sizeof(IndexBase<UserInf>), ios::beg);
            file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<UserInf>));
            if (indexbase.size == 0) {
                index_now = indexbase.next;
                continue;
            }
            strcpy(database.index, user_id.c_str());
            if (string(user_id) < string(indexbase.node[0].index))throw MyError();
            if (string(indexbase.node[0].index) <= user_id &&
                user_id <= string(indexbase.node[indexbase.size - 1].index)) {
                long long n;
                n = (lower_bound(indexbase.node, indexbase.node + indexbase.size, database) -
                     indexbase.node);
                if (string(indexbase.node[n].index) != user_id)throw MyError();
                for (int j = n; j < indexbase.size - 1; j++) {
                    indexbase.node[j] = indexbase.node[j + 1];
                }
                indexbase.size--;
                file_catalogue.seekg(sizeof(int) + index_now * sizeof(IndexBase<UserInf>), ios::beg);
                file_catalogue.write(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<UserInf>));
                return;
            }
            index_now = indexbase.next;
        }
        throw MyError();
    }


    void splitBlock(int &catalogue_rear) {
        IndexBase<UserInf> new_index;
        new_index.next = indexbase.next;
        indexbase.next = catalogue_rear;
        for (int i = MAXSIZE / 2; i < MAXSIZE; i++) {
            new_index.node[i - (MAXSIZE / 2)] = indexbase.node[i];
        }
        new_index.size = MAXSIZE / 2;
        indexbase.size = MAXSIZE / 2;
        file_catalogue.seekg((catalogue_rear) * sizeof(IndexBase<UserInf>) + sizeof(int), ios::beg);
        file_catalogue.write(reinterpret_cast<char *>(&new_index), sizeof(IndexBase<UserInf>));
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
            file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<UserInf>));
            cout << indexbase.next << " " << indexbase.size << " ";
            cout << endl;
            for (int i = 0; i < indexbase.size; i++) {
                cout << string(indexbase.node[i].index);
            }
            cout << endl;
            if (indexbase.next == 0)break;
            file_catalogue.seekg(sizeof(int) + indexbase.next * sizeof(IndexBase<UserInf>), ios::beg);
        }
    }


};

//登录栈结构体
struct LogStack {
    UserInf userInf;
    bool select = false;//是否有选中的图书
    bool exist[3] = {0};//选中的图书是否存在
    BookInf bookInf;
    int index;
};
#endif //BOOKSTORE_DATABASEACCOUNT_HPP
