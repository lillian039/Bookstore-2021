#include <iostream>
#include "parser.hpp"

int main() {
  /*  freopen("1.in","r",stdin);
    freopen("me.out","w",stdout);*/
    string commandLine;
    Command *user;
    while(true){
       try{
           getline(cin,commandLine);
           /*cout<<commandLine<<endl;*/
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
