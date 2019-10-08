#ifndef INDEX_H
#define INDEX_H
#define ll long long
#include <vector>
#include <string>
using namespace std;

class Index{
    private:
       //ָ���ϣľͰ
	   //��һά��ÿ���ֶ� �ڶ�ά��ÿ��Ͱ ����ά��ÿ��ֵ
	   //hashӳ�������ļ���ͷ��ʵ��ַ
	   vector< vector< vector<ll> > > bucket;

		static const ll HASHMOD = 1000;
	   	//�Ƿ���hash����־λ
		vector<char> isHash;
		//�Ƿ���ѡ��
		bool isChoose;
		//��ǰѡ�б���
		string nowName;

		/*************************************************
		Function: hash
		Description: hashӳ�亯��
		Calls: // �����������õĺ����嵥
		Input: 
			value: string ��Ҫ����hash��ֵ
		Output: ��
		Return: long long hash��Ľ��
		Optional: 
			mod: long long ��ϣ�õ�ȡģ��ֵ������hash��ķ�Χ��Ĭ��ȡ����HASHMOD
		Other: ���иò��������ոñ����ִ�������������
		*************************************************/ 
		ll hash(string value, ll mod = Index::HASHMOD);

		int read(string name, ll cnum);

		int save(string name);

    public:
		/*************************************************
		Function: Index
		Description: ���캯��
		Calls: // �����������õĺ����嵥
		Input: 
		Output: ��
		Return: һ��indexʵ��
		Other: ��
		*************************************************/ 
        Index();

		/*************************************************
		Function: create
		Description: ����һ����������ļ��Լ�����ĳ�ʼ��׼��
		Calls: // �����������õĺ����嵥
		Input: 
			name: string ���������ı������
			isHahs: const vector<char> & �Ƿ����hash��ÿһ��ı�־���飬ע��˴�T Ϊ����hash�� FΪ������hash
		Output: ��
		Return: int ״̬��
		Other: ���иò��������ոñ����ִ�������������,����ֱ��ѡ�е�ǰ�����ı���������
		*************************************************/ 
        int create(string name, const vector<char> & isHash);

		/*************************************************
		Function: choose
		Description: ѡȡһ�����hash����
		Calls: // �����������õĺ����嵥
		Input: 
			name: string ѡȡ�ı������
		Output: ��
		Return: int ״̬��
		Other: ��
		*************************************************/ 
        int choose(string name, ll cnum);

		int insert(const vector< vector<string> > & s, const vector <ll> & addr);

		int deleteData(const vector< vector<string> > &s, const vector <ll> & addr);

		int query(ll idx, string value, vector<ll> & addr);

};
#endif