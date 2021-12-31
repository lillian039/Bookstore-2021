#ifndef BOOKSTORE_DATABASEBOOK_HPP
#define BOOKSTORE_DATABASEBOOK_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include "error.hpp"
#include <iomanip>

#define MAXSIZE 100

class AllBook;

//第一个索引 ISBN->map
struct BookInfISBN {
    char index[21];//ISBN
    int value;

    void Initialize(std::string index_, int value_) {
        strcpy(index, index_.c_str());
        value = value_;
    }

    friend bool operator<(const BookInfISBN &lhs, const BookInfISBN &rhs) {
        return std::string(lhs.index) < std::string(rhs.index);
    }

    friend bool operator>(const BookInfISBN &lhs, const BookInfISBN &rhs) {
        return std::string(lhs.index) > std::string(rhs.index);
    }

    friend bool operator>=(const BookInfISBN &lhs, const BookInfISBN &rhs) {
        return std::string(lhs.index) >= std::string(rhs.index);
    }

    friend bool operator<=(const BookInfISBN &lhs, const BookInfISBN &rhs) {
        return std::string(lhs.index) <= std::string(rhs.index);
    }

};

//第二个索引 author->map
//name->map
//keyword->map
struct BookInfoIndex {
    char index[61];
    int value;

    void Initialize(std::string index_, int value_) {
        strcpy(index, index_.c_str());
        value = value_;
    }

    friend bool operator<(const BookInfoIndex &lhs, const BookInfoIndex &rhs) {
        if (std::string(lhs.index) < std::string(rhs.index))return true;
        if (std::string(lhs.index) > std::string(rhs.index))return false;
        if (lhs.value < rhs.value)return true;
        return false;
    }

    friend bool operator>(const BookInfoIndex &lhs, const BookInfoIndex &rhs) {
        if (std::string(lhs.index) > std::string(rhs.index))return true;
        if (std::string(lhs.index) < std::string(rhs.index))return false;
        if (lhs.value > rhs.value)return true;
        return false;
    }

    bool operator<=(const BookInfoIndex &rhs) {
        if (std::string(index) < std::string(rhs.index))return true;
        if (std::string(index) == std::string(rhs.index) && value <= rhs.value)return true;
        return false;
    }

    bool operator>=(const BookInfoIndex &rhs) {
        if (std::string(index) > std::string(rhs.index))return true;
        if (std::string(index) == std::string(rhs.index) && value >= rhs.value)return true;
        return false;
    }

    bool operator>(const BookInfoIndex &rhs) {
        if (std::string(index) > std::string(rhs.index))return true;
        if (std::string(index) < std::string(rhs.index))return false;
        if (value > rhs.value)return true;
        return false;
    }
};

/*图书信息*/
struct BookInf {
    char ISBN[21] = "";
    char book_name[61] = "";
    char author[61] = "";
    char keyword[61] = "";
    int quantity = 0;
    double price = 0;


    void Initialize(std::string book_name_, std::string author_, std::string keyword_, std::string price) {
        strcpy(book_name, book_name_.c_str());
        strcpy(author, author_.c_str());
        strcpy(keyword, keyword_.c_str());
        price = stod(price);
    }

    friend std::ostream &operator<<(std::ostream &os, const BookInf &inf) {
        os << inf.ISBN << '\t' << inf.book_name << '\t' << inf.author << '\t'
           << inf.keyword << '\t';
        os << std::fixed << std::setprecision(2) << inf.price << '\t';
        os << inf.quantity << '\n';
        return os;
    }

    friend bool operator<(BookInf &lhs, BookInf &rhs) {
        return std::string(lhs.ISBN) < std::string(rhs.ISBN);
    }
};

//总数据库
class AllBook {
private:
    BookInf books;
    std::string filename;
    std::fstream bookfile;
    int n;
public:
    AllBook() {
        bookfile.open("allbooks");
        if (!bookfile) {
            bookfile.open("allbooks", std::ios_base::out);
            n = 0;
            bookfile.close();
            bookfile.open("allbooks");
            bookfile.write(reinterpret_cast<char *>(&n), sizeof(n));
        }
    }

    ~AllBook() {
        bookfile.close();
    }

    BookInf findInf(int &num) {
        bookfile.seekg((num - 1) * sizeof(BookInf) + sizeof(int));
        bookfile.read(reinterpret_cast<char *>(&books), sizeof(BookInf));
        return books;
    }

    int InsertInf(BookInf &books_) {
        bookfile.seekg(0);
        bookfile.read(reinterpret_cast<char *>(&n), sizeof(int));
        n += 1;
        bookfile.seekg(0);
        bookfile.write(reinterpret_cast<char *>(&n), sizeof(int));
        bookfile.seekg((n - 1) * sizeof(BookInf) + sizeof(int));
        bookfile.write(reinterpret_cast<char *>(&books_), sizeof(BookInf));
        return n;
    }

    void CoverInf(BookInf &books_, int index) {
        bookfile.seekg((index - 1) * sizeof(BookInf) + sizeof(int));
        bookfile.write(reinterpret_cast<char *>(&books_), sizeof(BookInf));
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
    std::fstream file_catalogue;
    std::string catalogue_name;
public:
    Ull(std::string catalogue_name_) {
        catalogue_name = catalogue_name_;
        file_catalogue.open(catalogue_name);
    }

    Ull() = default;

    void Initailize(std::string catalogue_name_) {
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

    void InsertValue(T &database_) {
        file_catalogue.seekg(0, std::ios::beg);
        if (!file_catalogue)InitializeFirstOpen();//
        database = database_;
        int catalogue_rear;//共有几个块
        int index_now = 0;//当前指针位置
        file_catalogue.read(reinterpret_cast<char *>(&catalogue_rear), sizeof(int));
        if (catalogue_rear == 0) {
            InitializeNoneBlock(catalogue_rear);
            return;
        } else {
            file_catalogue.seekg(sizeof(int), std::ios::beg);
            indexbase.next = 1;
            while (indexbase.next != 0) {
                file_catalogue.seekg(index_now * sizeof(IndexBase<T>) + sizeof(int), std::ios::beg);
                file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<T>));
                if (database <= indexbase.node[indexbase.size - 1] ||
                    indexbase.next == 0) {
                    indexbase.node[indexbase.size] = database;
                    long long n;
                    n = (std::upper_bound(indexbase.node, indexbase.node + indexbase.size, database, compare) -
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
        file_catalogue.seekg((index_now) * sizeof(IndexBase<T>) + sizeof(int), std::ios::beg);
        file_catalogue.write(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<T>));

    }

    std::vector<int> FindValue(T &database_) {
        database = database_;
        file_catalogue.seekg(0, std::ios::beg);
        std::vector<int> value_;
        if (!file_catalogue) {
            return value_;
        }
        int catalogue_rear;
        int index_now = 0;
        file_catalogue.read(reinterpret_cast<char *>(&catalogue_rear), sizeof(int));
        if (catalogue_rear == 0) {
            return value_;
        } else {
            indexbase.next = 1;//enter loop
            bool flag = false;
            while (indexbase.next != 0) {
                file_catalogue.seekg(sizeof(int) + index_now * sizeof(IndexBase<T>), std::ios::beg);
                file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<T>));
                if (std::string(indexbase.node[0].index) <= std::string(database_.index) &&
                    std::string(database_.index) <= std::string(indexbase.node[indexbase.size - 1].index)) {
                    long long n;
                    n = (std::lower_bound(indexbase.node, indexbase.node + indexbase.size, database, compare) -
                         indexbase.node);
                    for (int i = n; i < indexbase.size; i++) {
                        if (std::string(indexbase.node[i].index) != std::string(database_.index))break;
                        flag = true;
                        value_.push_back(indexbase.node[i].value);
                    }
                    if (std::string(indexbase.node[indexbase.size - 1].index) != database_.index)break;
                }
                index_now = indexbase.next;
            }
            if (!flag) {
                return value_;
            }
            return value_;
        }
    }

    void DeleteValue(T &database_) {
        file_catalogue.seekg(0, std::ios::beg);
        if (!file_catalogue) {
            return;
        }
        int catalogue_rear;
        file_catalogue.read(reinterpret_cast<char *>(&catalogue_rear), sizeof(int));
        if (catalogue_rear == 0) {
            return;
        }
        database = database_;
        indexbase.next = 1;//enter loop
        int index_now = 0;
        while (indexbase.next != 0) {
            file_catalogue.seekg(sizeof(int) + index_now * sizeof(IndexBase<T>), std::ios::beg);
            file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<T>));
            if (indexbase.size == 0) {
                index_now = indexbase.next;
                continue;
            }
            if (std::string(database_.index) < std::string(indexbase.node[0].index))return;
            if (indexbase.node[0] <= database && database <= indexbase.node[indexbase.size - 1]) {
                long long n;
                n = (std::lower_bound(indexbase.node, indexbase.node + indexbase.size, database, compare) -
                     indexbase.node);
                if (std::string(indexbase.node[n].index) != std::string(database_.index) ||
                    indexbase.node[n].value != database_.value)
                    return;
                for (int j = n; j < indexbase.size - 1; j++) {
                    indexbase.node[j] = indexbase.node[j + 1];
                }
                indexbase.size--;
                file_catalogue.seekg(sizeof(int) + index_now * sizeof(IndexBase<T>), std::ios::beg);
                file_catalogue.write(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<T>));
                return;
            }
            index_now = indexbase.next;
        }
    }

    void InitializeFirstOpen() {
        file_catalogue.open(catalogue_name, std::ios_base::out);
        int rear = 0;
        file_catalogue.write(reinterpret_cast<char *>(&rear), sizeof(int));
        file_catalogue.close();
        file_catalogue.open(catalogue_name);
    }

    void InitializeNoneBlock(int &catalogue_rear) {
        indexbase.next = 0;
        indexbase.size = 1;
        file_catalogue.seekg(0, std::ios::beg);
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
        file_catalogue.seekg((catalogue_rear) * sizeof(IndexBase<T>) + sizeof(int), std::ios::beg);
        file_catalogue.write(reinterpret_cast<char *>(&new_index), sizeof(IndexBase<T>));
        catalogue_rear += 1;
        file_catalogue.seekg(0, std::ios::beg);
        file_catalogue.write(reinterpret_cast<char *>(&catalogue_rear), sizeof(int));
    }

    void printindexbase() {
        file_catalogue.seekg(0, std::ios::beg);
        if (!file_catalogue)return;
        int n;
        file_catalogue.read(reinterpret_cast<char *>(&n), sizeof(int));
        AllBook allBook;
        int location;
        while (true) {
            file_catalogue.read(reinterpret_cast<char *>(&indexbase), sizeof(IndexBase<T>));
            for (int i = 0; i < indexbase.size; i++) {
                location = indexbase.node[i].value;
                std::cout << allBook.findInf(location);
            }
            if (indexbase.next == 0)break;
            file_catalogue.seekg(sizeof(int) + indexbase.next * sizeof(IndexBase<T>), std::ios::beg);
        }
    }


};


#endif //BOOKSTORE_DATABASEBOOK_HPP
