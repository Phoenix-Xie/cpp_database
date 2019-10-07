#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <memory.h>
#include <vector>
#define ll long long
class DataBase{
/*
编写规范：
1. 任何size变量 指的是多少个char， sizec 代表多少个 size* sizeof(char) 
2. 所有传入的参数不应该考虑问题 1， 即避免使用者计算
3. 行号从1开始计数

//返回码含义：
0.成功
-1. 未知错误失败
-2. 未选择表
-3. 未找到数据
*/ 
	
private:
	ll table_name_max_len;
	ll col_name_max_len;
	ll table_number;
	static DataBase * DBinstance;

	//当前表配置
	string table_name; //表名 
	ll table_col_num; //列数量  
	vector<string> table_col_name; //列名
	vector<ll> table_col_size; // 当前表每列长度
	vector<ll> table_col_pre_size; //每列表长前缀和 
	ll table_id;
	
	//全局设置
	DataBase();
public:
	/*索引代码*/

	//创建与更新索引
	int refreshPointBucket();

	//保存索引
	int saveHash();

	//读取索引
	int readHash();

	/*数据库本身操作*/ 
	static DataBase * getInstance();
	
	void closeDataBase();
	
	/*表生成销毁*/ 
	//todo：重名判断 
    //创建数据库记录
	void createTable(string name, vector<string> col_name, vector<ll> col_size, vector<char> isHash);
	
	void showTables();
	
	int deleteTable(string name);

	//获取数据库当前表信息
	string getTableName();

    ll getColNum();
	
	vector<string> getColName();

	/*数据库基本操作 -- 单表*/
    //选择表 
	string chooseTable(string name);

    //增
	int insert(vector< vector<string> > s, vector<ll> & id, string name = "");
	
	void showDatas(string name = "");
	
	//删
	int deleteData(string key, string value, string name = "");

	// 查
	int query(string key, string value, vector<ll> & id, vector< vector<string> > & ans, string name = "");


	int queryById(vector<ll> &id, vector< vector<string> > & ans, ll id1 = 1, ll id2 = -1, string name = "");

	int predictId(ll num, vector<ll> &id, string name = "");

	//改
	int update(string key, string value, string key2, string value2, string name = "");
	
	//查询某个字段整行数据段的位置(不包含标志位和空闲链表位) idx为所在字段位置，以0开始计数
	bool getKeyLocation(string key, ll &idx, ll & selfLen, ll & preSeek, ll & lastSeek, ll & totalSeek);

	//清空数据库
	void emptyDataBase();

	//清空数据表
	void emptyTable();


};
//莫名其妙的静态私有变量初始化 
DataBase * DataBase::DBinstance = NULL;