#ifndef STATU_H
#define STATU_H
#include <vector>
#include <string>
#define ll long long
using namespace std;

//可以将表的数据内容直接存在内存之中
namespace settings{
	static const string table_settings_name = "table_settings";
	static const string settings_name = "settings";
	static const ll table_name_max_len = 100;
	static const ll col_name_max_len = 100;
}

class Statu{
private:
	
	static Statu * instance;
	Statu();

	/*************************************************
	Function: save
	Description: 保存当前所有表的基本数据
	Calls: // 被本函数调用的函数清单
	Input: 
	Output: 无
	Return: int 状态码
	Other: 无
	*************************************************/  
	int save();
	/*************************************************
	Function: read
	Description: 读取当前表中所有基本数据
	Calls:
	Input: 
	Output: 无
	Return: int 状态码
	Other: 无
	*************************************************/  
	int read();

	/*************************************************
	Function: isNameRepeat
	Description: 判断表名是否重复
	Calls: // 被本函数调用的函数清单
	Input: 
		name： string 判断用的表名
	Output: 无
	Return: bool 是否有重复，有重复为true 否则为false
	Other: 无
	*************************************************/  
	bool isNameRepeat(string name);
public:
	ll table_number; //当前表配置
	
	vector<string> table_name;
	vector<ll> table_col_num; 
	vector< vector<string> > table_col_name; //列名
	vector< vector<ll> > table_col_size; // 每个表每列长度
	vector< vector<ll> > table_col_pre_size; //每个表每列表长前缀和
	vector< vector<char> > isHash;
	vector< vector<char> > isUnique;

	static Statu * getInstance();
	

	/*************************************************
	Function: createTable
	Description: 创建表
	Calls:
	Input: 
	Output: 无
	Return: int 状态码 0成功 -1表已存在
	Other: 无
	*************************************************/
	int createTable(string name, vector<string> col_name, vector<ll> size, vector<char> isHash, vector<char> isUnique);

	int getIdx(string name);

	int clearTables();
};


#endif