#include <iostream>
#include "parser.hpp"

int main() {
    string commandLine;
    Command *user;
    while (true) {
        try {
            getline(cin, commandLine);
            if (cin.eof())std::exit(0);
            user = getSigned(commandLine);
            parseCommand(commandLine, user);
            delete user;
        }
        catch (MyError &ex) {
            std::cout << ex.what();
        }
    }
}
