#include "fileData.hpp"
//块状链表的尝试
int main() {
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
    int n;//n commands
    cin >> n;
    string command;
    getline(cin, command);
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
            fileData file_data;
            file_data.InsertValue(single_command[1], (atoi(single_command[2].c_str())), "database_index", "database");
        } else if (single_command[0] == "find") {
            fileData file_data;
            file_data.FindValue(single_command[1], "database_index", "database");
        } else if (single_command[0] == "delete") {
            fileData file_data;
            file_data.DeleteValue(single_command[1], (atoi(single_command[2].c_str())), "database_index", "database");
        }
    }
    return 0;
}

