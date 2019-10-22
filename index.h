#ifndef INDEX_H
#define INDEX_H
#define ll long long
#include <vector>
#include <string>
#include <list>
#include "statu.h"
using namespace std;

//单表index
class Index{
    private:
       //指针哈希木桶
	   //第一维：每个字段 第二维：每个桶 第三维：每个值
	   //hash映射存距离文件开头真实地址
	   vector< vector< list<ll> > > bucket;

		static const ll HASHMOD = 1000;
		Statu * sta;
		ll sidx;

		//todo: 设置一个 isread位置保证该index是否read或者create过

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

		/*************************************************
		Function: check
		Description: 公用条件检测函数
		Calls: // 被本函数调用的函数清单
		Return: int 状态码
		-1 该对象还未read或create 处于空状态
		*************************************************/ 
		int check();

		/*************************************************
		Function: bucketInit
		Description: 按照isHash需要初始化bucket变量
		Calls: // 被本函数调用的函数清单
		Return: int 状态码
		0 成功
		*************************************************/ 
		int bucketInit();

    public:
		
		/*************************************************
		Function: Index
		Description: 构造函数
		Calls: // 被本函数调用的函数清单
		Input: 
			id: 创建索引的表
		Output: 无
		Return: 一个index实例
		Other: 此处未进行id对应表是否存在的判断，应注意
		*************************************************/ 
        Index(ll id);

		/*************************************************
		Function: createFile
		Description: 创建一个表的索引文件以及所需的初始化准备
		Calls: // 被本函数调用的函数清单
		Input: 
			name: string 创建索引的表的名称
			isHahs: const vector<char> & 是否进行hash的每一项的标志数组，注意此处T 为进行hash， F为不进行hash
		Output: 无
		Return: int 状态码
		0:成功
		Other: 进行该操作后会清空该表中现存所有数据索引,并且直接选中当前创建的表数据索引
		*************************************************/ 
        int createFile();

		int read();

		//int save();
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

		/*************************************************
		Function: delete
		Description: 删除二维数据表中对应的数据地址
		Calls: // 被本函数调用的函数清单
		Input: 
			name: string 选取的表的名称
		Output: 无
		Return: int 状态码
		Other: 默认每个有效地址唯一
		*************************************************/
		int deleteData(const vector< vector<string> > &s, const vector <ll> & addr);

		/*************************************************
		Function: delete8
		Description: 指定字段的指定值可能存储位置的地址的链表桶
		Calls: // 被本函数调用的函数清单
		Input: 
			idx: ll 字段位置
			value: string 值
			addr: ll 地址链表
		Output: 无
		Return: int 状态码
		Other: 
		*************************************************/
		int query(ll idx, string value, list<ll> & addr);

		int clear();

};
#endif