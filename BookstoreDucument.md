

# Bookstore开发文档

开发文档作者：胡珂雅 Lillian_

partner：郭一锦 FlowerInSpring

当前版本：1.3 （原1.2）

最新修改：2021.01.12

修改内容：

1.添加了一些声明，使对执行方的要求相对宽松，给执行方更多调整的空间

2.应执行方建议，修改了Statement类，防止解体。

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

### 三、各个类的接口及成员说明

执行方可以根据需要自行添加所需要的类，但不要修改文件数量和命名。

account类：每一个账户均实时存入 account.file 并更新accountBlock.file与accountIndex.file

```c++
struct AccountInf {
    int priority;
    char userId[31];
    char password[31];
    char userName[31];
    /*根据需要添加重载函数*/
};

class Account {
private:

    static AccountInf accountLog;//只记录最新登陆，进行操作的用户
    /*其他需要的私有成员变量*/
    
public:

   Account();
    //登录{0}
    void su(const std::string &_user_id, const std::string &_password);
    //登出{1}
    void logout();
    //注册{0}
    void Register(const std::string &_user_id, const std::string &_password, const std::string &_user_name);
    //修改密码{1}
    void passwd(const std::string &_user_id, const std::string &old_password,const std::string &new_password);
    //创建用户{3}
    void Useradd(const std::string &_user_id, const std::string &_password, const int &priority, const std::string &_user_name);
    //删除用户{7}
    void deleteAccount(const std::string &_user_id);
    
    /*根据需要添加函数*/
    
};
```

books类：每一个账户均实时存入 books.file 并更新booksBlock.file与booksIndex.file

```c++
struct BooksInf {
    char ISBN[21];
    char bookName[61];
    char author[61];
    char keyword[61];//记录整段keyword
    int quantity;
    double price;
    double totalCost = 0;//销售总金额
     /*根据需要添加重载函数*/
};

class Books {
private:
    
    BooksInf bookSelect;
    /*其他需要的私有成员变量*/
    
public:
    Books();
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

    /*根据需要添加函数*/
};
```

diary类：用SumIncome.file记录每笔交易额

```c++
struct SumIncome {
    int time;
    double in = 0;
    double out = 0;
     /*根据需要添加重载函数*/
};
```

用OperaRecord.file记录每次员工不同的操作记录 用OperaRecordIndex.file记录

对日志生成的格式 根据bonus需要自行设计，大致包含每笔操作，操作与金额等等

```c++
class Diary {
private:
    FileData<SumIncome> finance;
    SumIncome latest;//最近一次交易总额
    int Time;//交易总笔数
    std::fstream diary;//这是一个直接可读的文件,是所有人操作的记录,格式：权限 ID 操作 \n
public:
    Diary();
    ~Diary();
    //将某用户的某次操作记录写入文件
    void write(int &priority,std::string &name, std::string &content);
    //收入
    void buyBook(double cost);
    //支出
    void importBook(double total_cost);

    //{3}reportMyself
    void reportMyself(std::string &index);
    //{7}财务记录查询
    void showFinance(int time);
    //{7}生成财务记录报告
    void reportFinance();
    //{7}生成全体员工工作情况报告
    void reportEmployee();
    //{7}生成日志
    void LogRecord();

};
```

FileData类：存储每个类的不同类型不同的成员均开一个文件，存储该成员的所有数据

```c++
template<class T>//文件数据结构类接口 T可为Books SumIncome Account 中任意一个；也可以根据执行者需要修改
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

//该类比较简单，只包含一个对功能实现有意义的函数，执行方允许不使用类进行包装。
```

statement类：执行指令

该类用于包装三个系统，使main函数更加简洁。请小心处理各个文件中类和全局变量/静态变量的关系，防止解体。

```c++
class Statement{
private:
    Account accountSystem;
    Books bookSystem;
    Diary diarySystem;
    int wordNum = 0;//vector中非空字符数量
    std::vector<std::string> words;//储存每句语法被拆解出的单词
    /*可以根据需要添加私有成员*/
public:
    Statement();
    ~Statement();
    void separateCmd(std::string &cmd);
    /*根据需要自行添加成员函数*/
    void Quit();
    void Exit();
    void Su();
    void Logout();
    void Register();
    void Passwd();
    void Useradd();
    void Delete();
    void Show();
    void Buy();
    void Select();
    void Modify();
    void Import();
    void ReportMyself();
    void ShowFinance();
    void ReportFinance();
    void ReportEmployee();
    void log();
};
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

/*根据需要添加异常类*/
/*允许简化异常类使代码更简洁*/
```



### 四、文件读取说明

#### **（1）所需文件：**

允许执行方为便于实现对文件个数、用途等进行修改，总数不超过20即可。文件请自主命名。

以下描述提供参考。

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

