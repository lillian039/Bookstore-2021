#include <iostream>
#include "parser.hpp"

int main() {
/*    freopen("3.txt","r",stdin);
    freopen("me.out","a",stdout);*/
    string commandLine;
    Command *user;
    while(true){
       try{
           getline(cin,commandLine);
           if (cin.eof())std::exit(0);
           user=getSigned();
           parseCommand(commandLine,user);
           delete user;
       }
       catch(MyError &ex){
           std::cout<<ex.what();
       }
    }
    return 0;
}
