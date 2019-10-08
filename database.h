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
/*
��д�淶��
1. �κ�size���� ָ���Ƕ��ٸ�char�� sizec �������ٸ� size* sizeof(char) 
2. ���д���Ĳ�����Ӧ�ÿ�������? 1�� ������ʹ���߼���
3. �кŴ�1��ʼ����

//�����뺬�壺
0.�ɹ�
-1. δ֪����ʧ��
-2. δѡ���?
-3. δ�ҵ�����
*/ 
	
private:
	ll table_name_max_len;
	ll col_name_max_len;
	ll table_number;
	static DataBase * DBinstance;

	//��ǰ������
	string table_name; //���� 
	ll table_col_num; //������  
	vector<string> table_col_name; //����
	vector<ll> table_col_size; // ��ǰ��ÿ�г���
	vector<ll> table_col_pre_size; //ÿ�б���ǰ׺�� 
	ll table_id;
	
	//ȫ������
	DataBase();
public:
	/*���ݿⱾ������*/

	/*************************************************
	Function: get
	Description: ѡȡһ������hash����
	Calls: // �����������õĺ����嵥
	Input: 
		name: string ѡȡ�ı�������
	Output: ��
	Return: int ״̬��
	Other: ��
	*************************************************/  
	static DataBase * getInstance();
	
	void closeDataBase();
	
	/*����������*/ 
	//todo�������ж� 
    //�������ݿ���?
	void createTable(string name, vector<string> col_name, vector<ll> col_size, vector<char> isHash);
	
	void showTables();
	
	int deleteTable(string name);

	//��ȡ���ݿ⵱ǰ����Ϣ
	string getTableName();

    ll getColNum();
	
	vector<string> getColName();

	/*���ݿ��������? -- ����*/
    //ѡ���? 
	string chooseTable(string name);

    //��
	int insert(const vector< vector<string> > & s, vector<ll> & id, string name = "");
	
	void showDatas(string name = "");
	
	//ɾ
	int deleteData(string key, string value, string name = "");

	// ��
	int query(string key, string value, vector<ll> & id, vector< vector<string> > & ans, string name = "");


	int queryById(vector<ll> &id, vector< vector<string> > & ans, ll id1 = 1, ll id2 = -1, string name = "");

	int predictId(ll num, vector<ll> &id, string name = "");

	//��
	int update(string key, string value, string key2, string value2, string name = "");
	
	//��ѯĳ���ֶ��������ݶε�λ��(��������־λ�Ϳ�������λ) idxΪ�����ֶ�λ�ã���0��ʼ����
	bool getKeyLocation(string key, ll &idx, ll & selfLen, ll & preSeek, ll & lastSeek, ll & totalSeek);

	//������ݿ�?
	void emptyDataBase();

	//������ݱ�?
	void emptyTable();


};

#endif