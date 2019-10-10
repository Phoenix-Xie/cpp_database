#ifndef DATABASE_H
#define DATABASE_H
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <memory.h>
#include <vector>
#include "statu.h"
#include "index.h"
#define ll long long

using namespace std;
class DataBase{	
private:
	//��ǰ��id
	ll table_id;
	Statu * sta;
	vector<Index *> index;
	

	/*************************************************
	Function: check
	Description: ͨ��׼��״̬��麯��
	Return: ״̬�� int
	0 ��ȷ
	-1 δѡ��
	Other: 
	*************************************************/ 
	int check();
public:
	/*************************************************
	Function: DataBase
	Calls: Statu::getInstance;
	Description: ���캯��
	*************************************************/ 
	DataBase();
	
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
	Description: ����һ�ű��������ñ����������
	Calls: 
	Input: 
	Output: 
	Return: int ״̬�� 
	0 �����ɹ�
	-1
	-1 ���Ѵ���
		-200:
		 	-201: size��С����ȷ
			-202: isHash��С����ȷ
			-203: isUnique��С����ȷ
		-300
			-301�������ļ���ʧ��
	Other: 
	*************************************************/
	int createTable(string name,const vector<string>& col_name,const vector<ll>& col_size, const vector<char> & isHash, const vector<char> & isUnique);
	
	//void showTables();

	/*************************************************
	Function: getTableName
	Description: ��ȡ����
	Calls: 
	Input: 
	Output: 
	Return: string ��δѡ�б�Ϊ ""
	Other: 
	*************************************************/
	string getTableName();

    ll getColNum();
	
	vector<string> getColName();

	/*************************************************
	Function: chooseTable
	Description: ѡ���
	Calls: 
	Input: 
	Output: 
	Return: int ״̬�� 
	0 ѡ��
	-1 ������
	Other: 
	*************************************************/
	int chooseTable(string name);

    //��
	int insert(const vector< vector<string> > & s, vector<ll> & id, string name = "");
	
	void showDatas(string name = "");
	
	//ɾ
	int deleteData(string key, string value, string name = "");

	/*************************************************
	Function: query
	Description: ѡ���
	Calls: getKeyLocation
	Input: 
	Output: 
	Return: int ״̬�� 
	0 ѡ��
	-1 ������
	Other: 
	*************************************************/
	int query(string key, string value, vector<ll> & id, vector< vector<string> > & ans, string name = "");


	int queryById(vector<ll> &id, vector< vector<string> > & ans, ll id1 = 1, ll id2 = -1, string name = "");

	int predictId(ll num, vector<ll> &id, string name = "");

	//��
	int update(string key, string value, string key2, string value2, string name = "");
	
	//��ѯĳ���ֶ��������ݶε�λ��(��������־λ�Ϳ�������λ) idxΪ�����ֶ�λ�ã���0��ʼ����
	bool getKeyLocation(string key, ll &idx, ll & selfLen, ll & preSeek, ll & lastSeek, ll & totalSeek);

	//������ݿ�?
	int clear();

	//������ݱ�?
	int clearTable(string name="");


};

#endif