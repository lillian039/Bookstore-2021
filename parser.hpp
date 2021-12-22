#ifndef BOOKSTORE_PARSER_HPP
#define BOOKSTORE_PARSER_HPP

#include <iostream>
#include "database.hpp"
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
            if (i == commandLine.size() - 1)break;
            words.push_back(word);
            word = commandLine[i];
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
    for (int i = 0; i < words.size(); i++) {
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

}

Command *getSigned() {//读取登录栈
    UserInf usernow;
    fstream logfile;
    logfile.open("logfile");
    if (!logfile) {
        logfile.out;
        logfile.close();
        logfile.open("logfile");
    }
    int n;
    Command *p;
    logfile.read(reinterpret_cast<char *>(&n), sizeof(int));
    logfile.seekg(sizeof(int) + (n - 1) * sizeof(UserInf));
    logfile.read(reinterpret_cast<char *>(&usernow), sizeof(UserInf));
    if (usernow.value.Priority == 0)p = new Visitor;
    if (usernow.value.Priority == 1)p = new Customer;
    if (usernow.value.Priority == 3)p = new Staff;
    if (usernow.value.Priority == 7)p = new Shopkeeper;
    return p;
}

#endif //BOOKSTORE_PARSER_HPP
