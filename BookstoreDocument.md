

# Bookstore开发文档

开发文档作者：胡珂雅 Lillian_

partner：郭一锦 FlowerInSpring

当前版本：1.2

注意，该版本为执行者提出的修改意愿，尊重开发者意愿，该版本的最终形式以开发者提供的开发文档为准。



修改意愿说明：

1.账户系统和图书系统类中的私有变量均封装成结构体

2.删除了blockList.hpp文件，并大面积修改FileData文件

3.parser类重写，将其转变为只实现解析输入命令行，生成执行类的类，将其解析后生成的单词储存到Statement及其派生类中。

4.修改了Statement基类中的共有成员(一个vector型容器)

5.异常类中，重写what函数时，返回类型由string修改为char*（与exception类一致）

6.文件命名大小写略有修改

7.所有给出的代码中完善名字空间std

8.几个无关实现，可以忽略的代码格式的修改，如换行空行问题

3.几个无关实现，可以忽略的小错误



### 一、程序功能概述

实现一个图书管理系统，包括

#### （1）账户系统

账户分为四个权限：游客{0}，顾客{1}，店员{3}，店主{7}

存储账户信息：[User-ID] [Password] [User-Name] [Priority]

可支持的操作如下：

■ 权限{0}：登录账户  注册账户

■ 权限{1}：注销账户  修改密码

■ 权限{3}：创建账户

■ 权限{7}：删除账户

#### （2）图书系统

存储图书信息：[ISBN] [Book-Name] [Author] [Keyword] [Quantity] [Price] [Total-Cost]

可支持操作如下：

■ 权限{1}：购买图书 检索图书

■ 权限{3}：选择图书 修改图书信息 图书进货

#### （3）日志系统

存储日志信息：[Time]

可支持操作如下：

■ 权限{3}：生成员工操作记录

■ 权限{7}：财务记录查询 生成财务记录报告 生成全体员工工作记录报告 生成日志指令

### 二、主体逻辑说明

1.头文件说明

（1）main.cpp：不断获取输入的指令

（2）parser.hpp：解析语法

（3）statement.hpp：处理解析后的语法

（4）account.hpp books.hpp diary.hpp：定义账户系统、图书系统、日志系统类，实现类的功能

（5）FileData.hpp：用文件存储不同类的数据

（6）error.hpp：捕获并处理各种异常

2.登录栈说明：用一个文件记录登录中的所有账号，新登录的账号放在文件尾，从文件尾读取当前为登录的状态的账号。

使用栈

### 三、各个类的接口及成员说明

account类：每一个账户均实时存入 account.file 并更新accountBlock.file与accountIndex.file

```c++
struct AccountInf {
    int priority;
    char userId[30];
    char password[30];
    char userName[30];
};

class Account {
private:

    AccountInf account;

public:

    Account() = default;
    //注册{0}
    void Register(const char* _user_id, const char* _password, const char* _user_name);
    //登录{0}
    void su(const char* _user_id, const std::string _password);
    //修改密码{1}
    void passwd(const char* old_password,const char* new_password);
    //登出{1}
    void logout();
    //创建用户{3}
    void createAccount(const char* _user_id, const char*_password, const int priority, const char*_user_name);
    //删除用户{7}
    void deleteAccount(const char* _user_id);
};
```

books类：每一个账户均实时存入 books.file 并更新booksBlock.file与booksIndex.file

```c++
struct BooksInf {
    char ISBN[20];
    char bookName[60];
    char author[60];
    char keyword[60];//记录整段keyword
    int quantity;
    double price;
    double totalCost = 0;//销售总金额
};

class Books {
private:
    BooksInf books;
public:
    Books() = default;
    //以ISBN字典升序依次输出满足要求的图书信息{1}
    void show(char* cmd);
    //购买指定数量的图书{1}
    void buy(const char* _ISBN, const int _quantity);
    //以当前账户图书选中{3}
    void select(const char* _ISBN);
    //修改图书信息{3}
    void modify(char* cmd);
    //图书进货
    void import(const char* _ISBN, const int _total_cost);

};
```

diary类：用SumIncome.file记录每笔交易额

```c++
class SumIncome {
private:
    int time;
    double income;
public:
    SumIncome()=default;
    //记录新一笔交易
    void newDeal(double _income);
};
```

用OperaRecord.file记录每次员工不同的操作记录 用OperaRecordIndex.file记录

对日志生成的格式 根据bonus需要自行设计，大致包含每笔操作，操作与金额等等

```c++
class OperaRecord {
private:
    char user_id[30];
    char cmd[1024];
    int index;//操作记录所在位置
public:
    OperaRecord() = default;
    //记录不同员工的单次操作记录
    void newCmd(char *_cmd);
};
```

FileData类：存储每个类的不同类型不同的成员均开一个文件，存储该成员的所有数据

```c++
template<class T>//文件数据结构类接口 T可为Books SumIncome Account OperaRecord中任意一个
class FileData {
    
private:
    
    struct Node {
        int num;
        int next;
        T array[MAXSIZE];
    };
    int head = -1;//用于记录头节点在文件中的位置
    std::fstream file;
    std::string fileName;
    int sizeofT = sizeof(T);//每个数据的长度
    int sizeofNod = sizeof(Node);
    
public:
    FileData() = default;
    FileData(const std::string &fileName_) : fileName(fileName_) {};
    //初始化该文件
    void initialise(std::string FN);
    //获得该数据的位置（依靠index文件类）
    int getIndex(const T info);
    //删除某数据
    void deleteInfo(std::string index);
    //修改某数据
    void modifyInfo(std::string index);
    //添加某数据
    void addInfo(const T info);
    //返回某位置的数据
    T readInfo(const int index);

    //合并块
    void Merge();
    //分块
    void Split();
    //删除某个块
    void deleteBlock();
};
```

Parser类：拆分指令 解析语法 返回不同的执行类

```c++
class parser{
private:

    void separateCmd(std::string &cmd);

public:

    parser()=default;

    ~parser();

    Statement parserWord(std::string &cmd);//生成所执行的语法类型

};
```

statement类：执行指令

```c++
class Statement{
public:Statement();
   std::vector<std::string> words;//储存每句语法被拆解出的单词
    Statement();
    virtual ~Statement();
    virtual void execute();//每个派生类中均需要单独实现execute函数，功能用当前priority区分
};
class Quit:public Statement{};
class Exit:public Statement{};
class Su:public Statement{};
class Logout:public Statement{};
class Register:public Statement{};
class Passwd:public Statement{};
class Useradd:public Statement{};
class Delete:public Statement{};
class Show:public Statement{};
class Buy:public Statement{};
class Select:public Statement{};
class Modify:public Statement{};
class Import:public Statement{};
class ReportMyself:public Statement{};
class ShowFinance:public Statement{};
class ReportFinance:public Statement{};
class ReportEmployee:public Statement{};
class log:public Statement{};
```

error类：抛出异常

实现者根据可能会发生的异常，进行basic异常类的派生实现

```c++
class BasicException: public std::exception {
protected:
    const string message;
public:
    //错误信息初始化
    explicit BasicException(const std::string _message) : message(_message) {};
    virtual const char* what() const;
};
```

### 四、文件读取说明

#### **（1）所需文件：**

**■ class account：**

索引目录文件、索引文件、主体信息共三个文件

登录战状态记录：一个文件，记录登录中的账号

**■ class books：**

索引目录文件、索引文件、主题信息共三个文件

**■ class diary：**

SumIncome：由一个文件实现，线性存储每笔交易额

OperaRecord：由一个文件实现，记录店员id与操作记录

#### **（2）块状链表说明：**

■ 每次修改某值，将数据加到空位置中，若无空位置，则加到文件尾部，将地址index添加到某区块中去，需要重新刷新该区块的index，以字典序排好

■ 若需要并块，将两块地址合在一起排列，复制进其中一块，另一块标空备用，并更新地址索引目录

■ 若需要分块，将地址块直接从中间分开，一块留在原地，另一块移至空位，若无空位，移至尾部，并更新地址索引目录

■ 预留块链大小为160，超出160组则分块，单组小于80且相邻两组之和小于160则并块

### 五、其他补充说明

（1）本文档未考虑bonus。

（2）本文档中的文件名、函数名、变量名等任何命名，在保证可读性的前提下可以根据个人偏好自行修改。

（3）实现过程中，在合理范围内可以自行增加全局变量、全局函数、类的私有成员等，实现功能后与开发者统一说明即可。

（4）执行方若有任何疑问或质疑可随时提出。

