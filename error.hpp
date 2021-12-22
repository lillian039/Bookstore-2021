#ifndef BOOKSTORE_ERROR_HPP
#define BOOKSTORE_ERROR_HPP

#include <iostream>

class MyError : public std::exception {
private:
    const char *message;
public:
    explicit MyError() : message("Invalid\n") {};//用错误信息初始化

    const char *what() const noexcept override{
        return message;        //return: 错误信息字符串
    }
};

void PasswdInvaild(std::string passwd){
    for(int i=0;i<passwd.size();i++){
        if(!('a'<=passwd[i]&&passwd[i]<='z'||'A'<=passwd[i]&&passwd[i]<='Z'||passwd[i]=='_'))throw MyError();
    }
}

void BooknameInvalid(std::string bookname){
    for(int i=0;i<bookname.size();i++){
        if(bookname[i]=='"')throw MyError();
    }
}

vector<std::string> ProcessKeyword(std::string keyword){
    vector<std::string> keywords;
    std::string key="";
    for(int i=0;i<=keyword.size();i++){
        if(keyword[i]=='"')throw MyError();
        if(keyword[i]=='|'||i==keyword.size()){
            keywords.push_back(key);
            key="";
            continue;
        }
        key+=keyword[i];
    }
}

#endif //BOOKSTORE_ERROR_HPP
