#include "../database.h"
#include "statu.h"
#include "index.h"
#include <cstdio>
#include <string>
#include <windows.h>
#include "cmdui.h"
using namespace std;

DataBase::DataBase(){
	sta = Statu::getInstance();
	table_id = -1;
	index.clear();
	for(ll i = 0; i < sta->table_number; i++){
		index.push_back(new Index(i));
	}
	//创建文件夹
	// std::wstring stemp = std::wstring(settings::dataFolder.begin(), settings::dataFolder.end());
	// LPCWSTR sw = stemp.c_str();
	if (CreateDirectory(settings::dataFolder.c_str(), NULL) ||
    ERROR_ALREADY_EXISTS == GetLastError()){
    	//cout << "Yes" << endl;
		//todo: 文件夹存在与不存在错误处理 
	}

	//对数据文件的表检查是否存在，不存在则创建
	for(ll i = 0; i < sta->table_number; i++){
		FILE * fp = fopen( (settings::dataFolder + sta->table_name[i]).data() , "rb");
		if(fp == NULL){
			fp = fopen((settings::dataFolder + sta->table_name[i]).data(), "wb");
			fclose(fp);
		}
	}
}

DataBase::~DataBase(){
	ll n = index.size();
	for(ll i = 0; i < n; i++){
		delete index[i];
	}
	index.clear();
}

int DataBase::createTable(string name,const vector<string>& col_name,const vector<ll>& col_size, const vector<char> & isHash, const vector<char> & isUnique){
	
	ll code = sta->createTable(name, col_name, col_size, isHash, isUnique);
	if(code == 0){
		//创建该数据库文件
		name = settings::dataFolder+name;
		FILE * fp = fopen(name.data(), "w");
		//数据文件创建失败
		if(fp == NULL) 
			return -301;
		rewind(fp);
		//写入表头
		ll nextLocation = -1;
		fwrite(&nextLocation, sizeof(ll), 1, fp);
		fclose(fp);
		index.push_back(new Index(sta->table_number-1));
		return 0;
	}else{
		return code;
	}	 
}

int DataBase::check(){
	if(table_id == -1)
		return -1;
	return 0;
}

void DataBase::showTables(){
	// printf("show\n");
	// printf("表名，字段，字段长度，是否建立hash索引\n");
	// for(int i = 0; i < sta->table_number; i++){
	// 	printf("表名：%s 字段：", sta->table_name[i].data());
	// 	for(int j = 0; j < sta->table_col_num[i]; j++){
	// 		printf("%s %lld %c ", sta->table_col_name[i][j].data(), sta->table_col_size[i][j], sta->isHash[i][j]);
	// 	}
	// 	printf("\n");
	// }
	vector< vector<string> > data;
	
	for(ll i = 0; i < sta->table_number; i++){
		vector<string> t[3];
		data.clear();
		t[0].clear();
		t[0].push_back("表名："+sta->table_name[i]);
		for(ll j = 0; j < sta->table_col_num[i]; j++){
			t[0].push_back(sta->table_col_name[i][j]);
		}
		data.push_back(t[0]);

		t[1].clear();
		t[1].push_back("数据大小");
		for(ll j = 0; j < sta->table_col_num[i]; j++){
			t[1].push_back(to_string(sta->table_col_size[i][j]));
		}
		data.push_back(t[1]);

		t[2].clear();
		t[2].push_back("索引");
		for(ll j = 0; j < sta->table_col_num[i]; j++){
			char chart[2];
			chart[0] = sta->isHash[i][j];
			t[2].push_back(chart);
		}
		data.push_back(t[2]);
		CmdUI::drawTable(data);
	}
}


bool DataBase::isChoose(){
	if(table_id == -1){
		return false;
	}
	return true;
}

string DataBase::getTableName(){
	if(table_id == -1)
		return "";
	return sta->table_name[table_id];
}

ll DataBase::getColNum(){
	if(table_id == -1) return 0;
	return sta->table_col_num[table_id];
}

vector<string> DataBase::getColName(){
	if(table_id == -1) return vector<string>(0);
	return sta->table_col_name[table_id];
}

int DataBase::chooseTable(string name){
	ll code = sta->getIdx(name);
	if(code != -1){
		table_id = code;
		return 0;
	}else{
		return code;
	}
}



int DataBase::insert(const vector< vector<string> > & s, vector<ll> & id, string name){
	
	ll checkCode = check();
	if(checkCode != 0){
		return checkCode;
	}
	
	//初始化参数
	id.clear();

	//用到变量定义
	ll cnum = sta->table_col_num[table_id]; //项数
	char valid = 'T';
	vector <ll> & csize = sta->table_col_size[table_id];
	vector <ll> & cpsize = sta->table_col_pre_size[table_id];
	ll totalSeek = sta->table_col_pre_size[table_id][sta->table_col_num[table_id]] * sizeof(char);  //数据总长
	ll lineSeek = sizeof(char) + sizeof(ll) + totalSeek; //每行数据长度
	vector<ll> addr; addr.clear();
	//打开文件
	FILE * fp = fopen( (settings::dataFolder + sta->table_name[table_id]).data(), "rb+");
	rewind(fp);

	//获取表头
	ll nextLocation;
	fread(&nextLocation, sizeof(ll), 1, fp);
	ll n = s.size();
	for(int i = 0; i < n; i++){
		//每次读取表头
		if(nextLocation == -1){
			//进入文件末尾
			fseek(fp, 0, SEEK_END);
			//写入标志位
			fwrite(&valid, sizeof(char), 1, fp);
			
			//写入空闲表位
			nextLocation = -1;
			fwrite(&nextLocation, sizeof(ll), 1, fp);

			ll m = sta->table_col_num[table_id];
			
			for(ll j = 0;  j < m; j++){
				fwrite(s[i][j].data(), sizeof(char), csize[j], fp);
			}
		

		} else{ //有空闲行
			//从开始跳转空闲行
			int temp;
			fseek(fp, nextLocation+sizeof(ll), SEEK_SET);
			// temp = ftell(fp);
			// printf("%d\n", temp);
			//写入标志位
			fwrite(&valid, sizeof(char), 1, fp);
			fflush(fp);
			// temp = ftell(fp);
			// printf("%d\n", temp);
			//读取空闲行next值(链表操作)
			//同时读写要重定位，否则出错
			// fseek(fp, ftell(fp), SEEK_CUR);
			fread(&nextLocation, sizeof(ll), 1, fp);
			// temp = ftell(fp);
			// printf("%d\n", temp);
			// fseek(fp, ftell(fp)-sizeof(ll), SEEK_CUR);
			// temp = ftell(fp);
			// printf("%d\n", temp);
			ll tempNum = -1;
			fseek(fp, -sizeof(ll), SEEK_CUR);
			fwrite(&tempNum, sizeof(ll), 1, fp);	
			//写入数据
			ll m = sta->table_col_num[table_id];
			for(ll j = 0;  j < m; j++){
				fwrite(s[i][j].data(), sizeof(char), csize[j], fp);
			}  
		}

		//插入结束，根据fp位置计算id
		ll fpLocation = ftell(fp);
		id.push_back((fpLocation-sizeof(ll))/lineSeek);
		//存储开始位置
		addr.push_back(fpLocation-lineSeek);
	}
	//返回文件头
	rewind(fp);
	//将最后一次获得的next值写入头部
	fwrite(&nextLocation, sizeof(ll), 1, fp);
	fclose(fp);
	index[table_id]->insert(s, addr);
	return 0;
} 

/*
void DataBase::showDatas(string name){
	if(name != "")
		this->chooseTable(name); 
	name = this->table_name; 

	//变量定义
	char valid;
	ll nextLocation;
	ll idx = 0;
	ll totalLen = sizeof(char) + sizeof(ll) + this->table_col_pre_size[this->table_col_num]*sizeof(char);

	FILE * fp = fopen(this->table_name.data(), "rb+");
	rewind(fp);
	fread(&nextLocation, sizeof(ll), 1, fp);
	
	printf("表头：%lld  id:%lld\n", nextLocation, nextLocation/totalLen);
	

	//打印表头
	printf("有效位\t");
	printf("id\t");
	printf("nextLocation\t");
	for(ll i = 0; i < this->table_col_num; i++){
		printf("%s\t", this->table_col_name[i].data());
	} 
	printf("\n");
	//确定最长字段长度
	ll maxLen = 0;
	for(ll i = 0; i < this->table_col_num; i++){
		maxLen = max(maxLen, this->table_col_size[i]);
	}
	vector <char> s(maxLen);
	string str; 
	//读取并显示每一行 
	while((valid=fgetc(fp))!=EOF){
		idx++;
		//printf("tell:%d\n", ftell(fp));
		//读取有效位
		printf("%c\t", valid); 
		printf("%lld\t", idx);
		//读取链表位
		fread(&nextLocation, sizeof(ll), 1, fp);
		printf("%lld nextId:%lld\t", nextLocation, nextLocation/totalLen);
		//读取整行每一位数据并打印 
		for(ll i = 0; i < this->table_col_num; i++){
			fread(&s[0], sizeof(char), this->table_col_size[i], fp);
			
			printf("%s\t",  &s[0]);
		}  
		printf("\n");
	} 
	
	fclose(fp);
} 
*/

int DataBase::deleteData(string key, string value, string name){
	ll checkCode = check();
	if(checkCode != 0){
		return checkCode;
	}

	//查出id
	vector<ll> id(0);
	vector<vector<string> > ans;
	this->query(key, value, id, ans);
	//初始化变量
	ll n = id.size();
	ll len = sizeof(char) + sizeof(ll) + sta->table_col_pre_size[table_id][sta->table_col_num[table_id]]*sizeof(char);
	char valid = 'F';
	ll nextLocation = 0;
	ll headLocation = 0;
	//打开文件
	FILE * fp = fopen((settings::dataFolder + sta->table_name[table_id]).data(), "rb+");
	fseek(fp, 0, SEEK_SET);
	fread(&headLocation, sizeof(ll), 1, fp);
	for(ll i = 0; i < n; i++){
		printf("tell%lld id:%lld\n", headLocation, id[i]);
		//从开头跳转到指定位置
		fseek(fp, len*(id[i]-1)+sizeof(ll), SEEK_SET);
		fwrite(&valid, sizeof(char), 1, fp);
		fwrite(&headLocation, sizeof(ll),1, fp);
		headLocation = (id[i]-1)*len;
	}
	rewind(fp);
	//最后将最后一项通过文件头连接
	fwrite(&headLocation, sizeof(ll), 1, fp);
	fclose(fp);
}

int DataBase::query(string key, string value, vector<ll> & id, vector< vector<string> > & ans, bool isFuzzy, string name){
	//判断是否选表

	ll checkCode = check();
	if(checkCode != 0){
		return checkCode;
	}
	if(name != "")
		this->chooseTable(name);
	name = sta->table_name[table_id];

	//打开文件
	FILE * fp = fopen( (settings::dataFolder+name).data(), "rb");
	
	//具有索引的查询
	ll idx = sta->fieldToidxMap["key"];
	if(isFuzzy == true){
		return traverseQuery(key, value, id, ans, fp, isFuzzy);
	}
	else if(sta->isHash[table_id][idx] == 'T'){
		return indexQuery(key, value, id, ans, fp);
	}else{ 
		return traverseQuery(key, value, id, ans, fp, isFuzzy);
	}
	return 0;
}

int DataBase::queryById(vector<ll> &id, vector< vector<string> > & ans, ll id1, ll id2, string name){
	//判断是否选表
	if (name == "" && (sta->table_name[table_id] == "未选择" || sta->table_name[table_id] == ""))
		return -2;
	//参数处理
	ans.clear();
	id.clear();
	if(id2 < 0){
		id2 = 0x3f3f3f3f3f3f3f3f;
	}
	if(id1 > id2)
		throw "id范围错误，id1应小于id2";
	//变量定义
	char valid; //有效位
	ll nextLocation; //空闲链表位
	ll idx = 0; //id记录器
	ll totalSeek = sta->table_col_pre_size[table_id][sta->table_col_num[table_id]] * sizeof(char);  //数据总长
	ll lineSeek = sizeof(char) + sizeof(ll) + totalSeek; //每行数据长度
	//确定最长字段长度
	// todo: 判断是否解决id超过当前行数,以及id合法性
	ll maxLen = 0;
	for(ll i = 0; i < sta->table_col_num[table_id]; i++){
		maxLen = max(maxLen, sta->table_col_size[table_id][i]);
	}
	vector <char> s(maxLen);
	string str; 
	
	FILE * fp = fopen( (settings::dataFolder + sta->table_name[table_id]).data(), "rb");
	//跳过表头
	fseek(fp, sizeof(ll), SEEK_SET);
	//跳过前面几行
	fseek(fp, (id1-1)*lineSeek, SEEK_CUR);
	idx = id1;
	//读取并显示每一行 
	while( ((valid=fgetc(fp))!=EOF) && idx <= id2){
		if(valid == 'T'){
			//跳过表头
			fseek(fp, sizeof(ll), SEEK_CUR);
			id.push_back(idx);
			vector<string> tempStr(sta->table_col_num[table_id]);
			for(ll i = 0; i < sta->table_col_num[table_id]; i++){
				fread(&s[0], sizeof(char), sta->table_col_size[table_id][i], fp);
				tempStr[i] = &s[0];
			}  
			ans.push_back(tempStr);
		}
		else{
			fseek(fp, lineSeek-sizeof(char), SEEK_CUR);
		}
		idx++;
	}
	fclose(fp);
}

int DataBase::update(string key, string value, string key2, string value2, string name){
	//判断是否选表
	ll checkCode = check();
	if(checkCode != 0){
		return checkCode;
	}

	vector<ll> id;
	vector< vector<string> > ans;
	this->query(key, value, id, ans, false, name);
	ll n = id.size();
	ll len = sizeof(char) + sizeof(ll) + sta->table_col_pre_size[table_id][sta->table_col_num[table_id]]*sizeof(char);
	ll idx, selfLen, preSeek, lastSeek, totalSeek;
	if(!this->getKeyLocation(key2, idx, selfLen, preSeek, lastSeek, totalSeek)){
		//查无此字段
		return -1;
	}
	//进行文件操作
	FILE * fp = fopen( (settings::dataFolder + sta->table_name[table_id]).data(), "rb+");
	for(int i = 0; i < n; i++){
		//跳过表头
		fseek(fp, sizeof(ll), SEEK_SET);
		//跳转所在行
		fseek(fp, (id[i]-1) * (totalSeek+sizeof(ll)+sizeof(char)), SEEK_CUR);
		//获取当前地址
		ll addr = ftell(fp);
		//跳过有效位和链表位
		fseek(fp, sizeof(char) + sizeof(ll), SEEK_CUR);
		//跳到数据所在位置
		
		fseek(fp, preSeek, SEEK_CUR);
		//写入新数据
		
		fwrite(value2.data(), sizeof(char), selfLen, fp);
		//fflush(fp);
		//带有索引则更新索引
		if(sta->isHash[table_id][idx] == 'T'){
			index[table_id]->update(idx, value, value2, addr);
		}
	}
	fclose(fp);
	return 0;
}

bool DataBase::getKeyLocation(string key, ll &idx, ll & selfLen, ll & preSeek, ll & lastSeek, ll & totalSeek){
	if(sta->fieldToidxMap[table_id].count(key) > 0){
		idx = sta->fieldToidxMap[table_id][key];
	}
	else return false; //字段不存在
	//计算前后数据长度 
	selfLen = sta->table_col_size[table_id][idx];
	preSeek = (sta->table_col_pre_size[table_id][idx] - sta->table_col_pre_size[table_id][0])*sizeof(char);
	lastSeek = (sta->table_col_pre_size[table_id][sta->table_col_num[table_id]] - sta->table_col_pre_size[table_id][idx+1])*sizeof(char);
	totalSeek = sta->table_col_pre_size[table_id][sta->table_col_num[table_id]] * sizeof(char);  //数据总长
	return true;
}

int DataBase::clear(){
	//todo: 清空过程错误捕捉
	ll num = sta->table_name.size();
	for(ll i = 0; i < num; i++){
		index[i]->clear();
		DeleteFileA((settings::dataFolder+sta->table_name[i]).data());
	}
	sta->clear();
	return 0;
}

int DataBase::clearTable(string name){
	ll checkCode = check();
	if(checkCode != 0)
		return checkCode;
	if(name == "") name = sta->table_name[table_id];
	ll nextLocation = -1;
	FILE * fp = fopen((settings::dataFolder + name).data(), "w");
	fwrite(&nextLocation, sizeof(ll), 1, fp);
	fclose(fp);

	//清空索引
	index[table_id]->clear();
	return 0;
}

int DataBase::traverseQuery(const string & key, const string & value, vector<ll> & id, vector< vector<string> > & ans, FILE * fp, bool isFuzzy){
	id.clear();
	ans.clear();
	//获取参数长度
	ll idx, selfLen, preSeek, lastSeek, totalSeek;
	if(!this->getKeyLocation(key, idx, selfLen, preSeek, lastSeek, totalSeek)){
		//查无此字段
		return -1;
	}
	//整行数据长度
	ll lineSeek = totalSeek + sizeof(ll) + sizeof(char);
	//初始化参数
	ll lineNum = 0; //记录当前行号，用作id
	const ll & maxLen = sta->maxLen[table_id];
	//临时变量 
	vector<char> s(maxLen+10);  
	string str; 
	char valid;
	
	vector <ll> & csize = sta->table_col_size[table_id];
	
	rewind(fp);
	fseek(fp, sizeof(ll), SEEK_SET);
	while((valid=fgetc(fp))!=EOF){
		lineNum++;
		//该行数据有效 
		if(valid == 'T'){
			//跳过链表位
			fseek(fp, sizeof(ll), SEEK_CUR);
			//跳过前面数据块
			fseek(fp, preSeek, SEEK_CUR);
			//读取数据并放入string
			fread(&s[0], sizeof(char), selfLen, fp);
			str = &s[0];
			// 数据正确
			if( (!isFuzzy && str == value) || (isFuzzy &&  (str.find(value) != str.npos)) ){
				//找到向前跳跃 
				fseek(fp, -(preSeek + selfLen*sizeof(char)), SEEK_CUR);
				//读取整行数据
				vector<string> line(sta->table_col_num[table_id]); //一行结果 
				for(ll i = 0; i < sta->table_col_num[table_id]; i++){
					fread(&s[0], sizeof(char), csize[i], fp);
					line[i] = &s[0];
				}
				//vector的拷贝是深拷贝 
				ans.push_back(line); 
				id.push_back(lineNum);
			}
			else{
				//跳过后面部分
				fseek(fp, lastSeek, SEEK_CUR);
			}
		}
		else{
			fseek(fp, sizeof(ll), SEEK_CUR);
			fseek(fp, totalSeek, SEEK_CUR);
		}
	}
	return 0;
}

int DataBase::indexQuery(const string & key, const string & value, vector<ll> & id, vector< vector<string> > & ans, FILE * fp){
	id.clear();
	ans.clear();
	//获取参数长度
	ll idx, selfLen, preSeek, lastSeek, totalSeek;
	if(!this->getKeyLocation(key, idx, selfLen, preSeek, lastSeek, totalSeek)){
		//查无此字段
		return -1;
	}
	//整行数据长度
	ll lineSeek = totalSeek + sizeof(ll) + sizeof(char);
	//初始化参数
	ll lineNum = 0; //记录当前行号，用作id
	const ll & maxLen = sta->maxLen[table_id];
	//临时变量 
	vector<char> s(maxLen+10);  
	string str; 
	char valid;
	
	vector <ll> & csize = sta->table_col_size[table_id];
	list <ll> addr;
	index[table_id]->query(idx, value, addr);
	list<ll>::iterator itr;
	int  t = addr.size();
	for(itr = addr.begin(); itr != addr.end(); itr++){
		fseek(fp, *itr, SEEK_SET);
		valid = fgetc(fp);
		if(valid == 'T'){
			fseek(fp, sizeof(ll), SEEK_CUR);
			//跳过前面数据块
			fseek(fp, preSeek, SEEK_CUR);
			//读取数据并放入string
			fread(&s[0], sizeof(char), selfLen, fp);
			str = &s[0];
			// 数据正确
			if(str == value){
				// printf("%s数据相同", str.data());
				//找到向前跳跃 
				fseek(fp, -(preSeek + selfLen*sizeof(char)), SEEK_CUR);
				//读取整行数据
				vector<string> line(sta->table_col_num[table_id]); //一行结果 
				
				for(ll i = 0; i < sta->table_col_num[table_id]; i++){
					fread(&s[0], sizeof(char), csize[i], fp);
					line[i] = &s[0];
				}
				//vector的拷贝是深拷贝 
				ans.push_back(line); 
				id.push_back((*itr - sizeof(ll))/lineSeek + 1);
			}
		}
	}
}