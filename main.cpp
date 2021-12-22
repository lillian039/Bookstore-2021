#include <iostream>
#include "parser.hpp"
#include "statement.hpp"
#include "database.hpp"
#include "error.hpp"

int main() {
    string commandLine;
    Command *user;
    while(true){
       try{
           getline(cin,commandLine);
           user=getSigned();
           parseCommand(commandLine,user);
       }
       catch(MyError &ex){
           cout<<ex.what()<<endl;
       }
    }
    return 0;
}
