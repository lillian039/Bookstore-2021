#ifndef BOOKSTORE_PARSER_HPP
#define BOOKSTORE_PARSER_HPP

#include <iostream>
#include "databaseBook.hpp"
#include "databaseAccount.hpp"
#include "databaseDiary.hpp"
#include "error.hpp"
#include "statement.hpp"

//解构输入的命令行
vector<string> parser(std::string commandLine) {
    vector<string> words;
    string word = "";
    for (int i = 0; i < commandLine.size(); i++) {
        if (commandLine[i] == ' ') {
            i++;
            while (commandLine[i] == ' ' && i < commandLine.size())i++;
            if (i == commandLine.size() - 1&&commandLine[i]==' ')break;
            words.push_back(word);
            word = commandLine[i];
            if(i==commandLine.size()-1)break;
            continue;
        }
        word += commandLine[i];
    }
    words.push_back(word);
    return words;
}

void parseCommand(std::string commandLine, Command *cmd) {
    vector<string> words;
    words = parser(commandLine);
    if (words[0] == "")return;
    if ((words[0] == "quit" || words[0] == "quit") && words.size() == 1) cmd->exit();
    else if (words[0] == "show" && words[1] == "finance")cmd->showFinance(words);
    else if (words[0] == "report" && words[1] == "employee")cmd->reportEmployee(words);
    else if (words[0] == "report" && words[1] == "finance")cmd->reportFinance(words);
    else if (words[0] == "report" && words[1] == "myself")cmd->Report(words);
    else if (words[0] == "su") cmd->Login(words);
    else if (words[0] == "logout")cmd->Logout(words);
    else if (words[0] == "register")cmd->Register(words);
    else if (words[0] == "passwd")cmd->Passwd(words);
    else if (words[0] == "useradd")cmd->Useradd(words);
    else if (words[0] == "delete")cmd->Delete(words);
    else if (words[0] == "show")cmd->Show(words);
    else if (words[0] == "buy")cmd->Buy(words);
    else if (words[0] == "select")cmd->Select(words);
    else if (words[0] == "modify")cmd->Modify(words);
    else if (words[0] == "import")cmd->Import(words);
    else if (words[0] == "log")cmd->Log(words);
    else if (words[0] == "")return;
    else throw (MyError());


}

Command *getSigned() {//读取登录栈
    LogStack usernow;
    Command *p;
    if (onlineusers.empty()) {
        p = new Visitor;
        return p;
    }
    usernow = onlineusers.back();
    if (usernow.userInf.value.Priority == 1)p = new Customer;
    if (usernow.userInf.value.Priority == 3)p = new Staff;
    if (usernow.userInf.value.Priority == 7)p = new Shopkeeper;
    return p;
}

#endif //BOOKSTORE_PARSER_HPP
