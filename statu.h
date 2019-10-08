#ifndef STATU_H
#define STATU_H
#include <vector>
#include <string>
#define ll long long
using namespace std;

//���Խ������������ֱ�Ӵ����ڴ�֮��
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
	Description: ���浱ǰ���б�Ļ�������
	Calls: // �����������õĺ����嵥
	Input: 
	Output: ��
	Return: int ״̬��
	Other: ��
	*************************************************/  
	int save();
	/*************************************************
	Function: read
	Description: ��ȡ��ǰ�������л�������
	Calls:
	Input: 
	Output: ��
	Return: int ״̬��
	Other: ��
	*************************************************/  
	int read();

	/*************************************************
	Function: isNameRepeat
	Description: �жϱ����Ƿ��ظ�
	Calls: // �����������õĺ����嵥
	Input: 
		name�� string �ж��õı���
	Output: ��
	Return: bool �Ƿ����ظ������ظ�Ϊtrue ����Ϊfalse
	Other: ��
	*************************************************/  
	bool isNameRepeat(string name);
public:
	ll table_number; //��ǰ������
	
	vector<string> table_name;
	vector<ll> table_col_num; 
	vector< vector<string> > table_col_name; //����
	vector< vector<ll> > table_col_size; // ÿ����ÿ�г���
	vector< vector<ll> > table_col_pre_size; //ÿ����ÿ�б�ǰ׺��
	vector< vector<char> > isHash;
	vector< vector<char> > isUnique;

	static Statu * getInstance();
	

	/*************************************************
	Function: createTable
	Description: ������
	Calls:
	Input: 
	Output: ��
	Return: int ״̬�� 0�ɹ� -1���Ѵ���
	Other: ��
	*************************************************/
	int createTable(string name, vector<string> col_name, vector<ll> size, vector<char> isHash, vector<char> isUnique);

	int getIdx(string name);

	int clearTables();
};


#endif