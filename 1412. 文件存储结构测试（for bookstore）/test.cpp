#include "file2.hpp"

int main() {
    freopen("test1.in", "r", stdin);
    freopen("test.out", "w", stdout);
    int n;//n commands
    cin >> n;
    string command;
    getline(cin, command);
    Ull file_data("database_index");
    for (int i = 0; i < n; i++) {
        getline(cin, command);
        vector<string> single_command;
        int len = command.size();
        string word = "";
        for (int i = 0; i <= len; i++) {
            if (command[i] == ' ' || command[i] == '\0') {
                single_command.push_back(word);
                word = "";
                continue;
            }
            word += command[i];
        }
        if (single_command[0] == "insert") {
            file_data.InsertValue(single_command[1], (atoi(single_command[2].c_str())));
         /* file_data.printindexbase();*/
        } else if (single_command[0] == "find") {
            file_data.FindValue(single_command[1]);
/*         file_data.printindexbase();*/
        } else if (single_command[0] == "delete") {
            file_data.DeleteValue(single_command[1], (atoi(single_command[2].c_str())));
     /*      file_data.printindexbase();*/
        }
    }
    return 0;
}//
// Created by lenovo on 2021/12/13.
//

