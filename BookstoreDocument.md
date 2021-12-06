

# Bookstore开发文档

开发文档作者：胡珂雅 Lillian_

partner：郭一锦 FlowerInSpring

当前版本：1.0

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

（1）main：不断获取输入的指令

（2）parser.hpp：解析语法

（3）Statement.hpp：处理解析后的语法

（4）account.hpp books.hpp diary.hpp：定义账户系统、图书系统、日志系统类，实现类的功能

（5）MemoryRiver.hpp：用文件存储不同类的数据

（6）BlockLink.hpp：利用块状链表实现文件中数据位置的分块存储

（7）error.hpp：捕获并处理各种异常

### 三、各个类的接口及成员说明

account类：每一个账户均实时存入 account.file 并更新accountBlock.file与accountIndex.file

```c++
class Account {
private:
    int priority;
    char user_id[30];
    char password[30];
    char user_name[30];

public:
    Account() = default;
    //注册{0}
    void Register(const char* &_user_id, const char* &_password, const char* &_user_name);
    //登录{0}
    void su(const char* &_user_id, const std::string &_password);
    //修改密码{1}
    void passwd(const char* old_password,const char* new_password);
    //登出{1}
    void logout();
    //创建用户{3}
    void createAccount(const char* &_user_id, const char*&_password, const int priority, const char*&_user_name);
    //删除用户{7}
    void deleteAccount(const char* &_user_id);
};
```

books类：每一个账户均实时存入 books.file 并更新booksBlock.file与booksIndex.file

```c++
class Books {
private:
    char ISBN[20];
    char bookName[60];
    char Author[60];
    vector <string> keyword;//每个关键词占长度占60
    int quantity;
    double price;
    double TotalCost = 0;
public:
    Books() = default;
    //以ISBN字典升序依次输出满足要求的图书信息{1}
    void show(char* &cmd);
    //购买指定数量的图书{1}
    void buy(const char* &_ISBN, const int _quantity);
	//以当前账户图书选中{3}
    void select(const char* &_ISBN);
	//修改图书信息{3}
    void modify(char* &cmd);
	//图书进货
    void import(const char* &_ISBN, const int _total_cost);

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

MemoryRiver类：存储每个类的不同类型不同的成员均开一个文件，存储该成员的所有数据

```c++
template<class T>//存储类型为T的数据
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);//每个数据的长度
    int sum=0;//当前最大的坐标
    vector<int>emptySpace//当前空闲坐标
public:
    MemoryRiver() = default;
    MemoryRiver(const string &file_name) : file_name(file_name) {};
    //初始化该文件
    void initialise(string FN);
    //获得该数据的位置（依靠index文件类）
    int getIndex(const T info);
    //删除某数据
    void deleteInfo(const T info);
    //修改某数据
    void modifyInfo(const T info);
    //添加某数据
    void addInfo(const T info);
    //返回某位置的数据
    T readInfo(const int index){};
```

BlockLink类：两个文件 一个文件记录块的范围与另一个文件该块的起始地址 另一个文件记录该范围内按字典序排列的数据的地址

```c++
struct node {
    char *head[30];
    char *rear[30];//首尾的user_id，按id字典序大小排好
    int size;//块的大小
    int indexHead;//首为坐标集合的坐标值
    int len=sizeof(char)*60+ sizeof(int)*3;//该语句的长度
    vector <char*>eachPlace;
};
```

```c++
class Index {
private:
    node index;
public:
    int findIndex(const std::string &id) {}//找到下标
    //合并块
    void Merge(){}
    //判断是否需要合并块
    void ifMerge() {}
    //分块
    void Split() {}
    //判断是否需要分块
    void ifSplit() {}
    //添加某条索引
    void addIndex(){}
    //删除某条索引
    void deleteIndex(){}
    //删除某个块
    void deleteBlock(){}
};
```

Parser类：拆分指令 解析语法 返回不同的执行类

```c++
class parser{
private:vector<string>words;
public:
    parser()=default;
    ~parser();
    void separateCmd(string &cmd);
    statement* parserWord(){return new statement();};
};
```

statement类：执行指令

```c++
class Statement{
public:Statement();
    virtual ~Statement();
    virtual void execute(string &cmd);
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

```c++
class BasicException {
protected:
    const string message;
public:
    //错误信息初始化
    explicit BasicException(const string _message) : message(_message) {};
    virtual const string what() const;
};
```

### 四、文件读取说明

#### **（1）所需文件：**

**■ class account：**

索引目录文件、索引文件、主题信息：priority，user_id，password，user_name 每个各三个文件

登陆状态记录：一个文件，记录登陆中的账号

**■ class books：**

索引目录文件、索引文件、主题信息：priority，user_id，password，user_name 每个各三个文件

 ISBN，bookName，Author，quantity，price，TotalCost 每个各三个文件

keyword用一个keyword对应地址的文件与一个查找keyword的索引实现

**■ class diary：**

SumIncome：由一个文件实现，线性存储每笔交易额

OperaRecord：由一个文件实现，记录店员id与操作记录

#### **（2）分块链表说明：**

■ 每次修改某值，将数据加到空位置中，若无空位置，则加到文件尾部，将地址index添加到某区块中去，需要重新刷新该区块的index，以字典序排好

■ 若需要并块，将两块地址合在一起排列，复制进其中一块，另一块标空备用，并更新地址索引目录

■ 若需要分块，将地址块直接从中间分开，一块留在原地，另一块移至空位，若无空位，移至尾部，并更新地址索引目录

■ 预留块链大小为160，超出160组则分块，单组小于80且相邻两组之和小于160则并块

### 五、其他补充说明

（1）本文档未考虑bonus。

（2）本文档中的文件名、函数名、变量名等任何命名，在保证可读性的前提下可以根据个人偏好自行修改。

（3）实现过程中，在合理范围内可以自行增加全局变量、全局函数、类的私有成员等，实现功能后与开发者统一说明即可。

（4）执行方若有任何疑问或质疑可随时提出。
