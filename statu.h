#ifndef STATU_H
#define STATU_H
#include <vector>
#include <string>
#define ll long long
using namespace std;

class Statu{
public:
    ll table_name_max_len;
	ll col_name_max_len;
	ll table_number; //当前表配置
	string table_name; //表名 
	ll table_col_num; //列数量  
	vector<string> table_col_name; //列名
	vector<ll> table_col_size; // 当前表每列长度
	vector<ll> table_col_pre_size; //每列表长前缀和 
	ll table_id;
    
};


#endif