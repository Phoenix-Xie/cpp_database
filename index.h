#ifndef INDEX_H
#define INDEX_H
#define ll long long
#include <vector>
#include <string>
using namespace std;

class Index{
    private:
       //指针哈希木桶
	   //第一维：每个字段 第二维：每个桶 第三维：每个值
	   //hash映射存距离文件开头真实地址
	   vector< vector< vector<ll> > > bucket;

		static const ll HASHMOD = 1000;
	   	//是否开启hash，标志位
		vector<char> isHash;
		//是否已选中
		bool isChoose;
		//当前选中表名
		string nowName;

		/*************************************************
		Function: hash
		Description: hash映射函数
		Calls: // 被本函数调用的函数清单
		Input: 
			value: string 需要进行hash的值
		Output: 无
		Return: long long hash后的结果
		Optional: 
			mod: long long 哈希用的取模数值，即其hash后的范围，默认取自身HASHMOD
		Other: 进行该操作后会清空该表中现存所有数据索引
		*************************************************/ 
		ll hash(string value, ll mod = Index::HASHMOD);

		int read(string name, ll cnum);

		int save(string name);

    public:
		/*************************************************
		Function: Index
		Description: 构造函数
		Calls: // 被本函数调用的函数清单
		Input: 
		Output: 无
		Return: 一个index实例
		Other: 无
		*************************************************/ 
        Index();

		/*************************************************
		Function: create
		Description: 创建一个表的索引文件以及所需的初始化准备
		Calls: // 被本函数调用的函数清单
		Input: 
			name: string 创建索引的表的名称
			isHahs: const vector<char> & 是否进行hash的每一项的标志数组，注意此处T 为进行hash， F为不进行hash
		Output: 无
		Return: int 状态码
		Other: 进行该操作后会清空该表中现存所有数据索引,并且直接选中当前创建的表数据索引
		*************************************************/ 
        int create(string name, const vector<char> & isHash);

		/*************************************************
		Function: choose
		Description: 选取一个表的hash数据
		Calls: // 被本函数调用的函数清单
		Input: 
			name: string 选取的表的名称
		Output: 无
		Return: int 状态码
		Other: 无
		*************************************************/ 
        int choose(string name, ll cnum);

		int insert(const vector< vector<string> > & s, const vector <ll> & addr);

		int deleteData(const vector< vector<string> > &s, const vector <ll> & addr);

		int query(ll idx, string value, vector<ll> & addr);

};
#endif