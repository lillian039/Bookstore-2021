#ifndef BOOKSTORE_DATABASE_HPP
#define BOOKSTORE_DATABASE_HPP

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
//写构造函数
struct UserInfValue {
    char password[30];
    char user_name[30];
    int Priority;
    UserInfValue(string password_,string user_name_,int priority):Priority(priority){
        strcpy(password,password_.c_str());
        strcpy(user_name,user_name_.c_str());
    }
    UserInfValue()=default;
};

//账户信息
struct UserInf {
    char user_ID[30];
    UserInfValue value;
    explicit UserInf(string user_ID_,UserInfValue value_){
        strcpy(user_ID,user_ID_.c_str());
        value=value_;
    }

    UserInf()=default;

    void Initialize(string user_ID_, UserInfValue userInfValue) {
        strcpy(user_ID, user_ID_.c_str());
        value=userInfValue;
    }

    friend bool operator<(const UserInf &lhs, const UserInf &rhs) {
        return string(lhs.user_ID) < string(rhs.user_ID);
    }

    friend bool operator>(const UserInf &lhs, const UserInf &rhs) {
        return string(lhs.user_ID) > string(rhs.user_ID);
    }

    friend bool operator>=(const UserInf &lhs, const UserInf &rhs) {
        return string(lhs.user_ID) >= string(rhs.user_ID);
    }

    friend bool operator<=(const UserInf &lhs, const UserInf &rhs) {
        return string(lhs.user_ID) <= string(rhs.user_ID);
    }
};

/*图书信息*/
struct BookInf {
    char ISBN[20] = "";
    char book_name[60] = "";
    char author[60] = "";
    char keyword[60] = "";
    int quantity = 0;
    float price = 0;

    friend ostream &operator<<(ostream &os, const BookInf &inf) {
        os << inf.ISBN << '\t' << inf.book_name << '\t' << inf.author << '\t'
           << inf.keyword << '\t' << inf.price << '\t' << inf.quantity << '\n';
        return os;
    }
};

//第一个索引 ISBN->information
struct BookInfISBN {
    char ISBN[20] = "";
    BookInf bookInf;

    void Initialize(string ISBN_, BookInf bookInf_) {
        strcpy(ISBN, ISBN_.c_str());
        bookInf = bookInf_;
    }

    friend bool operator<(const BookInfISBN &lhs, const BookInfISBN &rhs) {
        return string(lhs.ISBN) < string(rhs.ISBN);
    }

    friend bool operator>(const BookInfISBN &lhs, const BookInfISBN &rhs) {
        return string(lhs.ISBN) > string(rhs.ISBN);
    }

    friend bool operator>=(const BookInfISBN &lhs, const BookInfISBN &rhs) {
        return string(lhs.ISBN) >= string(rhs.ISBN);
    }

    friend bool operator<=(const BookInfISBN &lhs, const BookInfISBN &rhs) {
        return string(lhs.ISBN) <= string(rhs.ISBN);
    }

    friend ostream &operator<<(ostream &os, const BookInfISBN &isbn) {
        os << isbn.bookInf;
        return os;
    }
};

//第二个索引 author->ISBN
struct BookInfAuthor {
    char author[60] = "";
    char ISBN[20] = "";

    void Initialize(string author_, string ISBN_) {
        strcpy(ISBN, ISBN_.c_str());
        strcpy(author, author_.c_str());
    }

    friend bool operator<(const BookInfAuthor &lhs, const BookInfAuthor &rhs) {
        if (string(lhs.author) < string(rhs.author))return true;
        else if (string(lhs.author) > string(rhs.author))return false;
        else if (string(lhs.ISBN) < string(rhs.ISBN))return true;
        return false;
    }

    friend bool operator>(const BookInfAuthor &lhs, const BookInfAuthor &rhs) {
        if (string(lhs.author) > string(rhs.author))return true;
        else if (string(lhs.author) < string(rhs.author))return false;
        else if (string(lhs.ISBN) > string(rhs.ISBN))return true;
        return false;
    }

    friend bool operator>=(const BookInfAuthor &lhs, const BookInfAuthor &rhs) {
        if (string(lhs.author) > string(rhs.author))return true;
        else if (string(lhs.author) < string(rhs.author))return false;
        else if (string(lhs.ISBN) >= string(rhs.ISBN))return true;
        return false;
    }

    friend bool operator<=(const BookInfAuthor &lhs, const BookInfAuthor &rhs) {
        if (string(lhs.author) < string(rhs.author))return true;
        else if (string(lhs.author) > string(rhs.author))return false;
        else if (string(lhs.ISBN) <= string(rhs.ISBN))return true;
        return false;
    }
};

//第三个索引 keyword->ISBN
struct BookInfKeyword {
    char keyword[60] = "";
    char ISBN[20] = "";

    void Initialize(string keyword_, string ISBN_) {
        strcpy(ISBN, ISBN_.c_str());
        strcpy(keyword, keyword_.c_str());
    }

    friend bool operator<(const BookInfKeyword &lhs, const BookInfKeyword &rhs) {
        if (string(lhs.keyword) < string(rhs.keyword))return true;
        else if (string(lhs.keyword) > string(rhs.keyword))return false;
        else if (string(lhs.ISBN) < string(rhs.ISBN))return true;
        return false;
    }

    friend bool operator>(const BookInfKeyword &lhs, const BookInfKeyword &rhs) {
        if (string(lhs.keyword) > string(rhs.keyword))return true;
        else if (string(lhs.keyword) < string(rhs.keyword))return false;
        else if (string(lhs.ISBN) > string(rhs.ISBN))return true;
        return false;
    }

    friend bool operator>=(const BookInfKeyword &lhs, const BookInfKeyword &rhs) {
        if (string(lhs.keyword) > string(rhs.keyword))return true;
        else if (string(lhs.keyword) < string(rhs.keyword))return false;
        else if (string(lhs.ISBN) >= string(rhs.ISBN))return true;
        return false;
    }

    friend bool operator<=(const BookInfKeyword &lhs, const BookInfKeyword &rhs) {
        if (string(lhs.keyword) < string(rhs.keyword))return true;
        else if (string(lhs.keyword) > string(rhs.keyword))return false;
        else if (string(lhs.ISBN) <= string(rhs.ISBN))return true;
        return false;
    }
};

struct BookInfName {
    char name[60] = "";
    char ISBN[20] = "";

    void Initialize(string keyword_, string ISBN_) {
        strcpy(ISBN, ISBN_.c_str());
        strcpy(name, keyword_.c_str());
    }

    friend bool operator<(const BookInfName &lhs, const BookInfName &rhs) {
        if (string(lhs.name) < string(rhs.name))return true;
        else if (string(lhs.name) > string(rhs.name))return false;
        else if (string(lhs.ISBN) < string(rhs.ISBN))return true;
        return false;
    }

    friend bool operator>(const BookInfName &lhs, const BookInfName &rhs) {
        if (string(lhs.name) > string(rhs.name))return true;
        else if (string(lhs.name) < string(rhs.name))return false;
        else if (string(lhs.ISBN) > string(rhs.ISBN))return true;
        return false;
    }

    friend bool operator>=(const BookInfName &lhs, const BookInfName &rhs) {
        if (string(lhs.name) > string(rhs.name))return true;
        else if (string(lhs.name) < string(rhs.name))return false;
        else if (string(lhs.ISBN) >= string(rhs.ISBN))return true;
        return false;
    }

    friend bool operator<=(const BookInfName &lhs, const BookInfName &rhs) {
        if (string(lhs.name) < string(rhs.name))return true;
        else if (string(lhs.name) > string(rhs.name))return false;
        else if (string(lhs.ISBN) <= string(rhs.ISBN))return true;
        return false;
    }
};

template<class T>
struct IndexBase {
    int next = 0;
    int size = 0;
    T node[MAXSIZE];
};

template<class T>
class Ull {
private:
    IndexBase<T> indexbase;
    T database;
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

    static bool compare(const T &lhs, const T &rhs) {
        if (lhs < rhs)return true;
        return false;
    }

    void InsertValue(string index, T value) {
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
                file_catalogue.seekg(index_now * sizeof(IndexBase<T>) + sizeof(int), ios::beg);
                file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<T>));
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
        file_catalogue.seekg((index_now) * sizeof(IndexBase<T>) + sizeof(int), ios::beg);
        file_catalogue.write(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<T>));

    }

    T FindValue(const string index) {
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
                file_catalogue.seekg(sizeof(int) + index_now * sizeof(IndexBase<T>), ios::beg);
                file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<T>));
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
            return;//todo
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
            file_catalogue.seekg(sizeof(int) + index_now * sizeof(IndexBase<T>), ios::beg);
            file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<T>));
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
                file_catalogue.seekg(sizeof(int) + index_now * sizeof(IndexBase<T>), ios::beg);
                file_catalogue.write(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<T>));
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
        file_catalogue.write(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<T>));
    }

    void splitBlock(int &catalogue_rear) {
        IndexBase<T> new_index;
        new_index.next = indexbase.next;
        indexbase.next = catalogue_rear;
        for (int i = MAXSIZE / 2; i < MAXSIZE; i++) {
            new_index.node[i - (MAXSIZE / 2)] = indexbase.node[i];
        }
        new_index.size = MAXSIZE / 2;
        indexbase.size = MAXSIZE / 2;
        file_catalogue.seekg((catalogue_rear) * sizeof(IndexBase<T>) + sizeof(int), ios::beg);
        file_catalogue.write(reinterpret_cast<char *>(&new_index), sizeof(IndexBase<T>));
        catalogue_rear += 1;
        file_catalogue.seekg(0, ios::beg);
        file_catalogue.write(reinterpret_cast<char *>(&catalogue_rear), sizeof(int));
    }

};

#endif //BOOKSTORE_DATABASE_HPP
