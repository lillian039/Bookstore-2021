#ifndef BOOKSTORE_STATEMENT_HPP
#define BOOKSTORE_STATEMENT_HPP

#include "databaseBook.hpp"
#include "databaseDiary.hpp"
#include "databaseAccount.hpp"
#include "error.hpp"
#include <cstdio>
#include <iomanip>

//todo 全局变量选中图书
//todo 登录栈 全局队列

std::vector<LogStack> onlineusers;

class Command {
public:

    Command() {}

    virtual ~Command() {}


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
        /*   //todo 最后记得要删掉！我只是不想手动删文件quq
           remove("accountfile");
           remove("allbooks");
           remove("AuthorCatalogue");
           remove("ISBNCatalogue");
           remove("KeywordCatalogue");
           remove("NameCatalogue");
           remove("FinanceFile");*/
        std::exit(0);
    }

    //登录账户su [User-ID] ([Password])?
    void Login(std::vector<std::string> &words) override {
        if (words.size() != 3 || words[1].size() > 30 || words[2].size() > 30)throw MyError();
        string userid = words[1];
        string passwd = words[2];
        AccountInf accountInf("accountfile");
        UserInf userInf;
        userInf = accountInf.FindValue(userid);
        if (string(userInf.index) == "null")throw MyError();
        if (std::string(userInf.value.password) == passwd) {
            LogStack logstack;
            logstack.userInf = userInf;
            onlineusers.push_back(logstack);
        } else throw (MyError());
    }

    //注册账户register [User-ID] [Password] [User-Name]
    void Register(std::vector<std::string> &words) override {
        if (words.size() != 4 || words[1].size() > 30 || words[2].size() > 30 || words[3].size() > 30)throw MyError();
        UserInfValue userInfValue(words[2], words[3], 1);
        UserInf userInf(words[1], userInfValue);
        AccountInf accountInf("accountfile");
        UserInf userInf_pre = accountInf.FindValue(words[1]);
        if (std::string(userInf_pre.index) != "null")throw MyError();
        accountInf.InsertValue(userInf);
    }


};

class Customer : public Visitor {
public:
    //注销账户logout
    void Logout(std::vector<std::string> &words) override {
        if (onlineusers.empty())throw MyError();
        onlineusers.pop_back();
        int n=onlineusers.size();
        if (!onlineusers.empty() && onlineusers[n - 1].select) {
            AllBook allBook;
            onlineusers[n-1].bookInf=allBook.findInf(onlineusers[n-1].index);
        }
    }

    //修改密码passwd [User-ID] ([Old-Password])? [New-Password]
    void Passwd(std::vector<std::string> &words) override {
        if (words.size() != 4 || words[1].size() > 30 || words[2].size() > 30 || words[3].size() > 30)throw MyError();
        AccountInf accountInf("accountfile");
        UserInf userInf;
        std::string userid = words[1];
        std::string oldpasswd = words[2];
        std::string newpasswd = words[3];
        PasswdInvaild(oldpasswd);
        PasswdInvaild(newpasswd);
        userInf = accountInf.FindValue(userid);
        if (std::string(userInf.index) == "null")throw MyError();
        if (string(userInf.value.password) == oldpasswd) {
            strcpy(userInf.value.password, newpasswd.c_str());
            accountInf.DeleteValue(userid);
            accountInf.InsertValue(userInf);
        } else throw MyError();
    }

    //检索图书show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]")?
    void Show(std::vector<std::string> &words) override {
        if (words.size() == 1) {
            Ull<BookInfISBN> ull("ISBNCatalogue");
            ull.printindexbase();
            return;
        }
        if (words.size() != 2)throw MyError();
        std::string search = words[1];
        words.pop_back();
        std::string word = "";
        for (int i = 0; i <= search.size(); i++) {
            if (search[i] == '=' || i == search.size()) {
                words.push_back(word);
                word = "";
                continue;
            }
            if (search[i] == '"')continue;
            word += search[i];
        }
        BookInfISBN bookInfIsbn;
        BookInfoIndex bookInfoIndex;
        Ull<BookInfoIndex> ull;
        AllBook books;
        vector<int> index;
        if (words[1] == "-ISBN") {
            bookInfIsbn.Initialize(words[2], 0);
            Ull<BookInfISBN> ull("ISBNCatalogue");
            index = ull.FindValue(bookInfIsbn);
            if (index.size() == 0) {
                std::cout << '\n';
                return;
            }
            BookInf book;
            book = books.findInf(index[0]);
            cout << book;
            return;
        } else {
            BooknameInvalid(words[2]);
            if (words[1] == "-name") {
                bookInfoIndex.Initialize(words[2], 0);
                ull.Initailize("NameCatalogue");

            } else if (words[1] == "-author") {
                bookInfoIndex.Initialize(words[2], 0);
                ull.Initailize("AuthorCatalogue");

            } else if (words[1] == "-keyword") {
                bookInfoIndex.Initialize(words[2], 0);
                ull.Initailize("KeywordCatalogue");
            } else throw MyError();
            index = ull.FindValue(bookInfoIndex);
            if (index.size() == 0) {
                std::cout << '\n';
                return;
            }
            std::vector<BookInf> bookss;
            for (int i = 0; i < index.size(); i++) {
                bookss.push_back(books.findInf(index[i]));
            }
            sort(bookss.begin(), bookss.end());
            for (int i = 0; i < bookss.size(); i++) {
                cout << bookss[i];
            }
        }
    }

    //购买图书buy [ISBN] [Quantity]
    void Buy(std::vector<std::string> &words) override {
        //异常判断
        if (words.size() != 3)throw MyError();
        BooknameInvalid(words[1]);
        IntInvalid(words[2]);
        //书籍文件读写
        BookInfISBN bookInfIsbn;
        bookInfIsbn.Initialize(words[1], 0);
        Ull<BookInfISBN> ull("ISBNCatalogue");
        vector<int> index;
        index = ull.FindValue(bookInfIsbn);
        if (index.size() == 0)throw MyError();
        BookInf book;
        AllBook books;
        book = books.findInf(index[0]);
        int quantity = atoi(words[2].c_str());
        if (book.quantity < quantity)throw MyError();
        book.quantity -= quantity;
        std::cout << long(book.price) * quantity / 100 << "." << setw(2) << setfill('0') << long(book.price) * quantity % 100
                  << std::endl;
        books.CoverInf(book, index[0]);
        //财务文件读写
        int deal = book.price * quantity;
        FinanceRecord financeRecord;
        financeRecord.addNewFiance(deal);
    }


};

class Staff : public Customer {
public:
    //$登录账户su [User-ID] ([Password])?
    //如果当前账户权限等级高于登录账户则可以省略密码
    void Login(std::vector<std::string> &words) override {
        if (words.size() != 3 && words.size() != 2)throw MyError();
        if (words.size() == 3)PasswdInvaild(words[2]);
        UserInf userInf;
        AccountInf accountInf("accountfile");
        userInf = accountInf.FindValue(words[1]);
        LogStack logstack;
        logstack.userInf = userInf;
        if (userInf.value.Priority < 3) {
            onlineusers.push_back(logstack);
            return;
        }
        if (string(userInf.value.password) == words[2]) onlineusers.push_back(logstack);
        else throw MyError();
    }

    //创建账户useradd [User-ID] [Password] [Priority] [User-Name]
    //-如果待创建账户的权限等级大于等于当前账户权限等级则操作失败
    void Useradd(std::vector<std::string> &words) override {
        if (words.size() != 5)throw MyError();
        PasswdInvaild(words[1]);
        PasswdInvaild(words[2]);
        PriorityInvalid(words[3]);
        std::string userid = words[1];
        std::string passwd = words[2];
        int priority = atoi(words[3].c_str());
        std::string username = words[4];
        UserInfValue userInfValue(passwd, username, priority);
        UserInf userInf(userid, userInfValue);
        AccountInf accountInf("accountfile");
        UserInf tmp = accountInf.FindValue(userid);
        if (std::string(tmp.index) != "null")throw MyError();
        accountInf.InsertValue(userInf);
        //todo 在文件读写中直接判断userid是否存在
    }

    //选择图书select [ISBN]
    //以**当前账户**选中指定图书（账户登出后无需保存选中图书情况）
    //- 没有符合条件的图书则创建仅拥有 `[ISBN]` 信息的新图书
    //- 退出系统视为取消选中图书
    //- 新的账户登入 默认为无选中图书状态 但是登出后原先的账户依然保持选中状态 仅能选中一本书
    //文件读写？
    void Select(std::vector<std::string> &words) override {
        if (words.size() != 2 || onlineusers.empty())throw MyError();
        BookInfISBN bookInfIsbn;
        BookInf bookInf;
        if (words[1].size() > 20)throw MyError();
        bookInfIsbn.Initialize(words[1], 0);
        Ull<BookInfISBN> ull("ISBNCatalogue");
        std::string ISBN = words[1];
        vector<int> index;
        index = ull.FindValue(bookInfIsbn);
        AllBook allBook;
        LogStack logStack;
        logStack = onlineusers.back();
        onlineusers.pop_back();
        int num;
        if (index.size() == 0) {
            strcpy(bookInf.ISBN, ISBN.c_str());
            num = allBook.InsertInf(bookInf);//创建图书
            bookInfIsbn.value = num;
            ull.InsertValue(bookInfIsbn);
        } else {
            bookInf = allBook.findInf(index[0]);//找到图书
            num = index[0];
            for (int i = 0; i < 3; i++) {
                logStack.exist[i] = true;
            }
        }
        logStack.index = num;
        logStack.select = true;
        logStack.bookInf = bookInf;
        onlineusers.push_back(logStack);
    }

    //修改图书modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
    void Modify(std::vector<std::string> &words) override {
        if (onlineusers.empty())throw MyError();
        LogStack logStack;
        AllBook allBook;
        logStack = onlineusers.back();
        if (!logStack.select)throw MyError();//登录栈是否有选中图书判断
        //处理语法结构
        std::vector<std::string> infom = ParseModify(words);
        if (infom.empty() % 2 != 0)throw MyError();
        for (int i = 0; i < infom.size(); i += 2) {
            if (infom[i] == "-name") {
                BooknameInvalid(infom[i + 1]);
                BookInfoIndex bookInfoIndex;
                bookInfoIndex.Initialize(std::string(logStack.bookInf.book_name), logStack.index);
                Ull<BookInfoIndex> ull("NameCatalogue");
                ull.DeleteValue(bookInfoIndex);
                strcpy(logStack.bookInf.book_name, infom[i + 1].c_str());
                strcpy(bookInfoIndex.index, infom[i + 1].c_str());
                ull.InsertValue(bookInfoIndex);
            } else if (infom[i] == "-author") {
                BooknameInvalid(infom[i + 1]);
                BookInfoIndex bookInfoIndex;
                bookInfoIndex.Initialize(std::string(logStack.bookInf.author), logStack.index);
                Ull<BookInfoIndex> ull("AuthorCatalogue");
                ull.DeleteValue(bookInfoIndex);
                strcpy(logStack.bookInf.author, infom[i + 1].c_str());
                strcpy(bookInfoIndex.index, infom[i + 1].c_str());
                ull.InsertValue(bookInfoIndex);
            } else if (infom[i] == "-keyword") {
                //todo keyword split
                std::vector<std::string> new_keywords;
                new_keywords = ProcessKeyword(infom[i + 1]);
                BooknameInvalid(infom[i + 1]);
                Ull<BookInfoIndex> ull("KeywordCatalogue");
                BookInfoIndex bookInfoIndex;
                for (int j = 0; j < new_keywords.size(); j++) {
                    bookInfoIndex.Initialize(new_keywords[j], logStack.index);
                    ull.InsertValue(bookInfoIndex);
                }
                if (logStack.exist[2]) {
                    std::vector<std::string> old_keywords;
                    std::string keyword_ = std::string(logStack.bookInf.keyword);
                    old_keywords = ProcessKeyword(keyword_);
                    for (int j = 0; j < old_keywords.size(); j++) {
                        bookInfoIndex.Initialize(old_keywords[j], logStack.index);
                        ull.DeleteValue(bookInfoIndex);
                    }
                }
                onlineusers[onlineusers.size() - 1].exist[2] = true;
                strcpy(logStack.bookInf.keyword, infom[i + 1].c_str());
            } else if (infom[i] == "-ISBN") {
                BookInfISBN bookInfIsbn;//修改索引ISBN
                BookInfISBN tmp;
                tmp.Initialize(infom[i + 1], 0);
                bookInfIsbn.Initialize(std::string(logStack.bookInf.ISBN), logStack.index);
                Ull<BookInfISBN> ull("ISBNCatalogue");
                if (!ull.FindValue(tmp).empty())throw MyError();
                ull.DeleteValue(bookInfIsbn);
                strcpy(logStack.bookInf.ISBN, infom[i + 1].c_str());
                strcpy(bookInfIsbn.index, infom[i + 1].c_str());
                ull.InsertValue(bookInfIsbn);
            } else if (infom[i] == "-price") {
                logStack.bookInf.price = FloatInvalid(infom[i + 1]);
            } else if (infom[i] == "-quantity") {
                IntInvalid(infom[i + 1]);
                logStack.bookInf.quantity = atoi(infom[i + 1].c_str());
            }
        }
        allBook.CoverInf(logStack.bookInf, logStack.index);//修改后的图书信息
        onlineusers[onlineusers.size() - 1].bookInf = logStack.bookInf;
    }

//图书进货import [Quantity] [Total-Cost]
    void Import(std::vector<std::string> &words) override {
        if (words.size() != 3)throw MyError();
        LogStack logStack;
        logStack = onlineusers.back();
        if (!logStack.select)throw MyError();
        AllBook allBook;
        BookInf bookInf;
        IntInvalid(words[1]);
        bookInf = allBook.findInf(logStack.index);
        int import_quantity = atoi(words[1].c_str());
        logStack.bookInf.quantity += import_quantity;
        allBook.CoverInf(logStack.bookInf, logStack.index);
        FinanceRecord financeRecord;
        int totalcost = FloatInvalid(words[2]) * (-1);//添加财务信息
        financeRecord.addNewFiance(totalcost);
        onlineusers[onlineusers.size() - 1].bookInf = logStack.bookInf;
    }

//*生成员工操作记录report myself
    void Report(std::vector<std::string> &words) override {
        if (words.size() > 2 || words[1] != "myself")throw MyError();
        std::cout << "I haven't complete report\n";
//todo 重用ull与database（1024）
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
        if (words.size() != 3 && words.size() != 4)throw MyError();
        std::string newpasswd;
        if (words.size() == 4)newpasswd = words[3];
        else if (words.size() == 3)newpasswd = words[2];
        else throw MyError();
        if (words[1].size() > 30)throw MyError();
        AccountInf accountInf("accountfile");
        UserInf userInf;
        userInf = accountInf.FindValue(words[1]);
        strcpy(userInf.value.password, newpasswd.c_str());
        accountInf.DeleteValue(words[1]);
        accountInf.InsertValue(userInf);
    }


    //删除账户delete [User-ID]
    void Delete(vector<std::string> &words) override {
        if (words.size() != 2)throw MyError();
        if (words[1].size() > 30)throw MyError();
        for (int i = 0; i < onlineusers.size(); i++) {
            if (std::string(onlineusers[i].userInf.index) == words[1])throw MyError();
        }
        AccountInf accountInf("accountfile");
        accountInf.DeleteValue(words[1]);
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
        int time;
        int *finance;
        int sum_plus = 0;
        int sum_minus = 0;
        FinanceRecord financeRecord;
        if (words.size() == 3) {
            IntInvalid(words[2]);
            time = atoi(words[2].c_str());
            if (time == 0) {
                std::cout << '\n';
                return;
            }
        } else if (words.size() == 2) {
            time = financeRecord.totalTime();
        } else throw MyError();
        finance = financeRecord.totalFinance(time);
        for (int i = 0; i < time; i++) {
            if (finance[i] < 0)sum_minus += finance[i];
            else if (finance[i] > 0)sum_plus += (finance[i]);
        }
        std::cout << "+ " << sum_plus / 100 << "." << setw(2) << setfill('0') << sum_plus % 100;
        std::cout << " - " << (-1) * sum_minus / 100 << "." << setw(2) << setfill('0') << (-1) * sum_minus % 100
                  << '\n';
        delete[]finance;
    }

    //*生成财务记录报告report finance
    void reportFinance(vector<std::string> &words) override {
        if (words.size() > 2)throw MyError();
    }

    //*生成全体员工工作情况报告report employee
    void reportEmployee(vector<std::string> &words) override {
        if (words.size() > 2)throw MyError();
    }
    //*生成日志log
    /*对于该函数的实现有文件输出格式要求，详见四、3中关于文件的说明*/
    void Log(vector<std::string> &words) override {
        if (words.size() > 2)throw MyError();
    }


};

#endif //BOOKSTORE_STATEMENT_HPP
