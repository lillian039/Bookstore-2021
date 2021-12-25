#include <iostream>
#include "parser.hpp"
#include "statement.hpp"
#include "databaseBook.hpp"
#include "error.hpp"

int main() {
    freopen("1.in","r",stdin);
    freopen("me.out","w",stdout);
    string commandLine;
    Command *user;
    while(true){
       try{
           getline(cin,commandLine);
           user=getSigned();
           parseCommand(commandLine,user);
           delete user;
       }
       catch(MyError &ex){
           cout<<ex.what();
       }
    }
    return 0;
}
