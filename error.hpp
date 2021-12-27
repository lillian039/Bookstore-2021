#ifndef BOOKSTORE_ERROR_HPP
#define BOOKSTORE_ERROR_HPP

#include <iostream>
#include <vector>
#include <string>

class MyError : public std::exception {
private:
    const char *message;
public:
    explicit MyError() : message("Invalid\n") {};//用错误信息初始化

    const char *what() const noexcept override {
        return message;        //return: 错误信息字符串
    }
};

void PasswdInvaild(std::string &passwd) {
    if (passwd.size() > 30)throw MyError();
    for (int i = 0; i < passwd.size(); i++) {
        if (!('0' <= passwd[i] && passwd[i] <= '9' || 'a' <= passwd[i] && passwd[i] <= 'z' ||
              'A' <= passwd[i] && passwd[i] <= 'Z' || passwd[i] == '_'))
            throw MyError();
    }
}

void BooknameInvalid(std::string &bookname) {
    if (bookname.size() > 60)throw MyError();
    for (int i = 0; i < bookname.size(); i++) {
        if (bookname[i] == '"')throw MyError();
    }
}

std::vector<std::string> ProcessKeyword(const std::string &keyword) {
    if (keyword.size() > 60)throw MyError();
    std::vector<std::string> keywords;
    std::string key = "";
    for (int i = 0; i <= keyword.size(); i++) {
        if (keyword[i] == '"')throw MyError();
        if (keyword[i] == '|' || i == keyword.size()) {
            for (int k = 0; k < keywords.size(); k++) {
                if (keywords[k] == key)throw MyError();
            }
            keywords.push_back(key);
            key = "";
            continue;
        }
        key += keyword[i];
    }
    return keywords;
}

void IntInvalid(std::string &num) {
    if (num.size() > 10)throw MyError();
    for (int i = 0; i < num.size(); i++) {
        if (num[i] < '0' || num[i] > '9')throw MyError();
    }
}

void PriorityInvalid(std::string &num) {
    if (num.size() != 1)throw MyError();
    if (num[0] != '1' && num[0] != '3')throw MyError();
}

void FloatInvalid(std::string &num) {
    int sum = 0;
    bool flag = false;
    for (int i = 0; i < num.size(); i++) {
        if ((num[i] < '0' || num[i] > '9') && num[i] != '.')throw MyError();
        if (num[i] == '.' && flag)throw MyError();
        if (num[i] == '.') flag = true;
    }
}

//修改图书modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
std::vector<std::string> ParseModify(std::vector<std::string> &cmd) {
    std::vector<std::string> splitwords;
    std::string word = "";//解析语法quq
    for (int i = 1; i < cmd.size(); i++) {
        for (int j = 0; j <= cmd[i].size(); j++) {
            if (j == cmd[i].size() || cmd[i][j] == '=') {
                splitwords.push_back(word);
                word = "";
                continue;
            }
            if (cmd[i][j] == '"')continue;
            word += cmd[i][j];
        }
    }
    bool states[6] = {false};
    for (int i = 0; i < splitwords.size(); i += 2) {
        if (splitwords[i] == "-name" && !states[0])states[0] = true;
        else if (splitwords[i] == "-author" && !states[1])states[1] = true;
        else if (splitwords[i] == "-keyword" && !states[2])states[2] = true;
        else if (splitwords[i] == "-ISBN" && !states[3])states[3] = true;
        else if (splitwords[i] == "-price" && !states[4])states[4] = true;
        else if (splitwords[i] == "-quantity" && !states[5])states[5] = true;
        else throw MyError();
    }
    return splitwords;
}

#endif //BOOKSTORE_ERROR_HPP
