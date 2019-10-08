#ifndef DATABASE_H
#define DATABASE_H
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <memory.h>
#include <vector>
#define ll long long

using namespace std;
class DataBase{	
private:
	//当前表id
	ll table_id;
	Statu * statu;
	/*************************************************
	Function: DataBase
	Calls: Statu::getInstance;
	Description: 构造函数
	*************************************************/ 
	DataBase();
public:
	/*************************************************
	Function: get
	Description: 
	Calls: 
	Input: 
	Output: 
	Return: 
	Other: 
	*************************************************/ 
	
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
	int createTable(string name,const vector<string>& col_name,const vector<ll>& col_size, const vector<char> & isHash, const vector<char> & isUnique);
	
	void showTables();

	/*************************************************
	Function: getTableName
	Description: 获取表名
	Calls: 
	Input: 
	Output: 
	Return: string 若未选中表为 ""
	Other: 
	*************************************************/
	string getTableName();

    ll getColNum();
	
	vector<string> getColName();

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

    //锟斤拷
	int insert(const vector< vector<string> > & s, vector<ll> & id, string name = "");
	
	void showDatas(string name = "");
	
	//删
	int deleteData(string key, string value, string name = "");

	// 锟斤拷
	int query(string key, string value, vector<ll> & id, vector< vector<string> > & ans, string name = "");


	int queryById(vector<ll> &id, vector< vector<string> > & ans, ll id1 = 1, ll id2 = -1, string name = "");

	int predictId(ll num, vector<ll> &id, string name = "");

	//锟斤拷
	int update(string key, string value, string key2, string value2, string name = "");
	
	//锟斤拷询某锟斤拷锟街讹拷锟斤拷锟斤拷锟斤拷锟捷段碉拷位锟斤拷(锟斤拷锟斤拷锟斤拷锟斤拷志位锟酵匡拷锟斤拷锟斤拷锟斤拷位) idx为锟斤拷锟斤拷锟街讹拷位锟矫ｏ拷锟斤拷0锟斤拷始锟斤拷锟斤拷
	bool getKeyLocation(string key, ll &idx, ll & selfLen, ll & preSeek, ll & lastSeek, ll & totalSeek);

	//锟斤拷锟斤拷锟斤拷菘锟?
	void emptyDataBase();

	//锟斤拷锟斤拷锟斤拷荼锟?
	void emptyTable();


};

#endif