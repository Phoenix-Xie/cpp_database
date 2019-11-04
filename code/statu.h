#ifndef STATU_H
#define STATU_H
#include <vector>
#include <string>
#include "settings.cpp"
#define ll long long
using namespace std;

//可以将表的数据内容直接存在内存之中


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
	Return: int 状态码 0成功
		-1 表已存在
		-200:
		 	-201: size大小不正确
			-202: isHash大小不正确
			-203: isUnique大小不正确
	Other: 无
	*************************************************/
	int createTable(string name, vector<string> col_name, vector<ll> size, vector<char> isHash, vector<char> isUnique);

	/*************************************************
	Function: getIdx
	Description: 获得表所在位置
	Calls:
	Input: 
	Output: 无
	Return: int 表所在位置，若为
		-1 则表不存在
	Other: 无
	*************************************************/
	int getIdx(string name);

	/*************************************************
	Function: clear
	Description: 清空所有数据
	Calls:
	Input: 
	Output: 无
	Return: 状态码
	Other: 无
	*************************************************/
	int clear();
};


#endif