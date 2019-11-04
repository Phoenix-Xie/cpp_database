/*
 * @Date: 2019-10-06 19:21:17
 * @LastEditors: Phoenix
 * @LastEditTime: 2019-10-29 10:54:09
 */

#ifndef DATABASE_H
#define DATABASE_H
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <memory.h>
#include <vector>
#include "code/statu.h"
#include "code/index.h"
#define ll long long

using namespace std;
class DataBase{	
private:
	//当前表id
	ll table_id;
	Statu * sta;
	vector<Index *> index;
	

	/*************************************************
	Function: check
	Description: 通用准备状态检查函数
	Return: 状态码 int
	0 正确
	-1 未选表
	Other: 
	*************************************************/ 
	int check();

	/*************************************************
	Function: getKeyLocation
	Description: 获取对应字段所处表的位置（第几个字段）以及前后数据信息
	Input: 
		key: string 所查询的字段名
	Output: 
		idx: & ll 字段计数位置，从0开始计数第几个字段
		selfLen: &ll 自身长度
		preSeek: &ll 该字段前面部分数据长度（不包括空闲链表位和有效位）
		lastSeek: &ll 该字段后面部分数据长度
		totalSeek: && 该字段总数据长度（不包括空闲链表位和有效位）
	Return: bool 是否成功 
	Other: 
	*************************************************/ 
	bool getKeyLocation(string key, ll &idx, ll & selfLen, ll & preSeek, ll & lastSeek, ll & totalSeek);
	/**
  * @Date: 2019-10-29 10:34:44
  * @describe: 无
  * @input: 无
  * @output: 无
  * @return: 无
  */
	int traverseQuery(const string & key, const string & value, vector<ll> & id, vector< vector<string> > & ans, FILE * fp, bool isFoozy = false);
	/**
  * @description: 
  * @param {type} 
  * @return: 
  */
	int indexQuery(const string & key, const string & value, vector<ll> & id, vector< vector<string> > & ans, FILE * fp);
public:
	/*************************************************
	Function: DataBase
	Calls: Statu::getInstance;
	Description: 构造函数
	*************************************************/ 
	DataBase();
	
	~DataBase();
	/*************************************************
	Function: createTable
	Description: 创建一张表，并建立该表的数据内容
	Calls: 
	Input: 
	Output: 
	Return: int 状态码 
	0 创建成功
	-1
	-1 表已存在
		-200:
		 	-201: size大小不正确
			-202: isHash大小不正确
			-203: isUnique大小不正确
		-300
			-301：数据文件打开失败
	Other: 
	*************************************************/
	int createTable(string name,const vector<string>& col_name,const vector<ll>& col_size, const vector<char> & isHash, const vector<char> & isUnique = vector<char>(0));
	
	//void showTables();
	
	/*************************************************
	Function: chooseTable
	Description: 选择表
	Calls: 
	Input: 
	Output: 
	Return: int 状态码 
	0 选中
	-1 表不存在
	Other: 
	*************************************************/
	int chooseTable(string name);

    /*************************************************
	Function: insert
	Description: 插入数据
	Input: 
		s: 二维数组，插入的数据
		id: 插入后的结果id
	Optional:
		name: 更新表名
	Return: int 状态码 
	0 插入成功
	-1 表不存在
	Other: 
	*************************************************/
	int insert(const vector< vector<string> > & s, vector<ll> & id, string name = "");
	
	// void showDatas(string name = "");
	
	//删
	int deleteData(string key, string value, string name = "");

	/*************************************************
	Function: query
	Description: 选择表
	Calls: getKeyLocation
	Input: 
	Output: 
	Return: int 状态码 
	0 选中
	-1 表不存在
	Other: 
	*************************************************/
	int query(string key, string value, vector<ll> & id, vector< vector<string> > & ans, bool isFuzzy = false, string name = "");


	int queryById(vector<ll> &id, vector< vector<string> > & ans, ll id1 = 1, ll id2 = -1, string name = "");


	/*************************************************
	Function: update
	Description: 更新数据
	Calls: query
	Input: 
		key: 查询用字段名
		value： 查询字段对应值
		key2: 更改用字段名
		value2： 更新用的字段对应的新值
	Optional:
		name: 执行的对应的表名，默认执行当前选中表
	Return: int 状态码 
	0 选中
	-1 表不存在
	Other: 
	*************************************************/
	int update(string key, string value, string key2, string value2, string name = "");
	
	

	/*************************************************
	Function: clear
	Description: 清空数据库
	Input: 
	Return: int 状态码 
	0 清空成功 
	*************************************************/
	int clear();

	/*************************************************
	Function: clear
	Description: 清空一张表内所有数据
	Optional: 
		name: string 选中的表名，默认当前选中表名
	Return: int 状态码 
	0 清空成功
	-1 未选中表
	*************************************************/
	int clearTable(string name="");


	bool isChoose();
	//get方法集合
	string getTableName();

    ll getColNum();
	
	vector<string> getColName();

	//测试用方法
	void showTables();
};

#endif