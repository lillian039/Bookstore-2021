#ifndef BOOKSTORE_STATEMENT_HPP
#define BOOKSTORE_STATEMENT_HPP

#include "database.hpp"
#include "error.hpp"

//todo unchanged number
//abstract Command
class Command {
protected:
    fstream fileUserInf;//账户系统
    fstream fileBookISBN;//ISBN->information
    fstream fileBookAuthor;//author->ISBN
    fstream fileBookKeyword;//keyword->ISBN
    fstream fileBookName;//name->ISBN
    fstream logStack;//登录栈
    fstream stuffReport;//员工工作记录
    fstream financeRecord;//财务记录
    fstream fileBackup;//文件备份
public:

    Command() {
        //todo 打开文件
        fileUserInf.open("UserInf");
        fileBookISBN.open("BookISBN");
        fileBookAuthor.open("BookAuthor");
        fileBookKeyword.open("BookKeyword");
        fileBookName.open("BookName");
        logStack.open("logStack");
        stuffReport.open("stuffReport");
        financeRecord.open("financeRecord");
        fileBackup.open("fileBackup");
    }

    virtual ~Command() {
        //todo 关闭文件
        fileUserInf.close();
        fileBookISBN.close();
        fileBookAuthor.close();
        fileBookKeyword.close();
        fileBookName.close();
        logStack.close();
        stuffReport.close();
        financeRecord.close();
        fileBackup.close();
    }

    virtual void exit() = 0;

    virtual void Login(std::vector<std::string> &) { throw MyError(); }

    virtual void Register(std::vector<std::string> &) { throw MyError(); }

    //注销账户logout
    virtual void Logout(std::vector<std::string> &) { throw MyError(); }

    //修改密码passwd [User-ID] ([Old-Password])? [New-Password]
    virtual void Passwd(std::vector<std::string> &) { throw MyError(); }

    //检索图书show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]")?
    virtual void Show(std::vector<std::string> &) { throw MyError(); }

    //购买图书buy [ISBN] [Quantity]
    virtual void Buy(std::vector<std::string> &) { throw MyError(); }


    //选择图书select [ISBN]
    virtual void Select(std::vector<std::string> &) { throw MyError(); }

    //修改图书modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
    virtual void Modify(std::vector<std::string> &) { throw MyError(); }

    //图书进货import [Quantity] [Total-Cost]
    virtual void Import(std::vector<std::string> &) { throw MyError(); }

    //*生成员工操作记录report myself
    virtual void Report(std::vector<std::string> &) { throw MyError(); }

    //$创建账户useradd [User-ID] [Password] [Priority] [User-Name]
    virtual void Useradd(vector<std::string> &) { throw MyError(); }

    //删除账户delete [User-ID]
    virtual void Delete(vector<std::string> &) { throw MyError(); }

    virtual void showFinance(vector<std::string> &) { throw MyError(); }

    //*生成财务记录报告report finance
    virtual void reportFinance(vector<std::string> &) { throw MyError(); }

    //*生成全体员工工作情况报告report employee
    virtual void reportEmployee(vector<std::string> &) { throw MyError(); }

    /*对于该函数的实现有文件输出格式要求，详见四、3中关于文件的说明*/
    virtual void Log(vector<std::string> &) { throw MyError(); }


};


class Visitor : public Command {
public:
    void exit() override {
        std::exit(0);
    }

    //登录账户su [User-ID] ([Password])?
    void Login(std::vector<std::string> &words) override {
        if (words.size() > 3 || words[1].size() > 30 || words[2].size() > 30)throw MyError();
        string userid = words[1];
        string passwd = words[2];
        Ull<UserInf> findUser("UserInformation");
        UserInf userInf;
        userInf = findUser.FindValue(userid);
        if (userInf.value.password == passwd) {
            //todo 登录栈读写
        } else throw (MyError());
    }

    //注册账户register [User-ID] [Password] [User-Name]
    void Register(std::vector<std::string> &words) override {
        if (words.size() > 4 || words[1].size() > 30 || words[2].size() > 30 || words[3].size() > 30)throw MyError();
        UserInfValue userInfValue(words[2], words[3], 1);
        UserInf userInf(words[1], userInfValue);
        //todo 进行一个文件的写
        //todo 判断userid是否存在
    }


};

class Customer : public Visitor {
public:
    //注销账户logout
    void Logout(std::vector<std::string> &words) override {
        //todo 进行登录栈文件读写
        //要不再开一个登录栈类吧不然好麻烦啊quq
    }

    //修改密码passwd [User-ID] ([Old-Password])? [New-Password]
    void Passwd(std::vector<std::string> &words) override {
        if (words.size() > 4 || words[1].size() > 30 || words[2].size() > 30 || words[3].size() > 30)throw MyError();
        Ull<UserInf> findUser("UserInformation");
        UserInf userInf;
        std::string userid = words[1];
        std::string oldpasswd = words[2];
        std::string newpasswd = words[3];
        PasswdInvaild(oldpasswd);
        PasswdInvaild(newpasswd);
        userInf = findUser.FindValue(userid);
        if (userInf.value.password == oldpasswd) {
            //todo 登录栈读写
        } else throw (MyError());
    }

    //检索图书show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]")?
    void Show(std::vector<std::string> &words) override {
        if (words.size() > 2)throw MyError();
        std::string search = words[1];
        words.pop_back();
        std::string word = "";
        for (int i = 0; i <= search.size(); i++) {
            if (search[i] == '=' || i == search.size()) {
                words.push_back(word);
                word = "";
            }
            word += search[i];
        }
        if (words[1] == "-ISBN") {

        } else {
            BooknameInvalid(words[2]);
            if (words[1] == "-name") {

            } else if (words[1] == "-author") {

            } else if (words[1] == "-keyword") {

            } else throw MyError();
        }
    }

    //购买图书buy [ISBN] [Quantity]
    void Buy(std::vector<std::string> &words) override {
        if (words.size() > 3 || words[2] > "2147483647")throw MyError();
        BooknameInvalid(words[1]);
        //todo 书籍文件读写
        //todo 财务文件读写

    }


};

class Staff : public Customer {
public:
    //$登录账户su [User-ID] ([Password])?
    //如果当前账户权限等级高于登录账户则可以省略密码
    void Login(std::vector<std::string> &words) override {
        PasswdInvaild(words[2]);
        //todo 文件读写 判断priority
        //todo 文件读写 登录栈
    }

    //创建账户useradd [User-ID] [Password] [Priority] [User-Name]
    //-如果待创建账户的权限等级大于等于当前账户权限等级则操作失败
    void Useradd(std::vector<std::string> &words) override {
        PasswdInvaild(words[1]);
        PasswdInvaild(words[2]);
        if (words[3] < "0" || words[3] > "9")throw MyError();
        //todo 判断priority
        //todo 文件读写
    }

    //选择图书select [ISBN]
    void Select(std::vector<std::string> &words) override {
        if (words.size() > 2)throw MyError();
        //todo 文件读写
    }

    //修改图书modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
    void Modify(std::vector<std::string> &words) override {
        if (words.size() > 2)throw MyError();
        std::string search = words[1];
        words.pop_back();
        std::string word;
        for (int i = 0; i <= search.size(); i++) {
            if (search[i] == '=' || i == search.size()) {
                words.push_back(word);
                word = "";
            }
            word += search[i];
        }
        if (words[1] == "-ISBN") {

        } else {
            BooknameInvalid(words[2]);
            if (words[1] == "-name") {

            } else if (words[1] == "-author") {

            } else if (words[1] == "-keyword") {

            } else throw MyError();
        }
    }

    //图书进货import [Quantity] [Total-Cost]
    void Import(std::vector<std::string> &words) override {
        if (words.size() > 3)throw MyError();
        //todo 书籍文件读写
    }

    //*生成员工操作记录report myself
    void Report(std::vector<std::string> &words) override {
        if (words.size() > 2 || words[1] != "myself")throw MyError();
        //todo 员工工作记录文件读写
    }


};

class Shopkeeper : public Staff {
public:
    //**注册账户 : 店主账户初始化，不进行注册
    //&注销账户logout
    //$修改密码passwd [User-ID] ([Old-Password])? [New-Password]
    //priority=7 可省略old-password
    void Passwd(vector<std::string> &words) override {
        //todo 可省略密码 其余同上
    }


    //删除账户delete [User-ID]
    void Delete(vector<std::string> &words) override {
        //todo 文件内容删除
    }

    //&检索图书show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]")?
    //&购买图书buy [ISBN] [Quantity]
    //&选择图书select [ISBN]
    //&修改图书modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
    //&图书进货import [Quantity] [Total-Cost]

    //*&生成员工操作记录report myself
    //财务记录查询show finance ([Time])?
    void showFinance(vector<std::string> &words) override {

    }

    //*生成财务记录报告report finance
    void reportFinance(vector<std::string> &words) override {
        if(words.size()>2)throw MyError();
    }

    //*生成全体员工工作情况报告report employee
    void reportEmployee(vector<std::string> &words)override{
        if(words.size()>2)throw MyError();
    }
    //*生成日志log
    /*对于该函数的实现有文件输出格式要求，详见四、3中关于文件的说明*/
    void Log(vector<std::string> &words)override{
        if(words.size()>2)throw MyError();
    }


};

#endif //BOOKSTORE_STATEMENT_HPP
