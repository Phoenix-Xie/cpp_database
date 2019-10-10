#include "database.h"
#include "statu.h"
#include "index.h"
#include <cstdio>
#include <string>
using namespace std;

DataBase::DataBase(){
	sta = Statu::getInstance();
	table_id = -1;
	index.clear();
	for(ll i = 0; i < sta->table_number; i++){
		index.push_back(new Index(i));
		index[i]->read();
	}
}

int DataBase::createTable(string name,const vector<string>& col_name,const vector<ll>& col_size, const vector<char> & isHash, const vector<char> & isUnique){
	
	ll code = sta->createTable(name, col_name, col_size, isHash, isUnique);
	if(code == 0){
		//创建该数据库文件
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
		index[sta->table_number-1]->create();
	}else{
		return code;
	}	 
}

int DataBase::check(){
	if(table_id == -1)
		return -1;
}
//暂时停止该功能
/*
void DataBase::showTables(){
	printf("show\n");
	FILE * fp = fopen(settings::table_settings_name.data(), "rb+");
	fseek(fp, 0, SEEK_SET);
	// 循环读取
	char valid;
	while((valid=fgetc(fp))!=EOF){
		printf("tell:%d\n", ftell(fp));
		//读取有效位
		printf("%c ", valid); 
		//读取表名 
		vector <char> s(this->table_name_max_len);
		fread(&s[0], sizeof(char), this->table_name_max_len, fp);
		printf("%s ", &s[0]); 
		//读取项数 
		ll len;
		fread(&len, sizeof(ll), 1, fp);
		printf("%d ", len);
		vector<char> col(this->col_name_max_len+10);
		for(ll j = 0; j < len; j++){
			fread(&col[0], sizeof(char), this->col_name_max_len, fp);
			printf("%s ", &col[0]);
		} 
		ll t;
		for(ll j = 0; j < len; j++){
			fread(&t, sizeof(ll), 1, fp);
			printf("%ld ", t);
		} 
		printf("\n"); 
	} 
	
	fclose(fp);
}
*/

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
	FILE * fp = fopen(sta->table_name[table_id].data(), "rb+");
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
	FILE * fp = fopen(sta->table_name[table_id].data(), "rb+");
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

int DataBase::query(string key, string value, vector<ll> & id, vector< vector<string> > & ans, string name){
	//判断是否选表

	ll checkCode = check();
	if(checkCode != 0){
		return checkCode;
	}
	if(name != "")
		this->chooseTable(name);
	name = sta->table_name[table_id];

	//获取参数长度
	ll idx, selfLen, preSeek, lastSeek, totalSeek;
	if(!this->getKeyLocation(key, idx, selfLen, preSeek, lastSeek, totalSeek)){
		//查无此字段
		return false;
	}
	//初始化参数
	id.resize(0); //返回id参数
	ll lineNum = 0; //记录当前行号，用作id
	ll maxLen = 0;
	for(ll i = 0; i < sta->table_col_num[table_id]; i++){
		maxLen = max(maxLen, sta->table_col_size[table_id][i]);
	}
	vector<char> s(maxLen+10);  //临时变量 
	string str;  //临时变量 
	char valid;
	
	vector <ll> & csize = sta->table_col_size[table_id];
	//打开文件查询
	FILE * fp = fopen(name.data(), "rb");
	
	//具有索引的查询
	if(sta->isHash[table_id][idx] == 'T'){
		list <ll> addr;
		index[table_id]->query(idx, value, addr);
		list<ll>::iterator itr;
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
					id.push_back(lineNum);
				}
			}
		}
	}else{ //遍历查询
		//跳过头部
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
	
	FILE * fp = fopen(sta->table_name[table_id].data(), "rb+");
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
/*
int DataBase::predictId(ll num, vector<ll> &id, string name){
	//判断是否选表
	if (name == "" && (sta->table_name == "未选择" || this->table_name == ""))
		return -2;
	if(name != "")
		this->chooseTable(name);
	name = this->table_name; 

	//获取参数长度
	ll totalSeek = this->table_col_pre_size[this->table_col_num] * sizeof(char);  //数据总长
	ll lineSeek = sizeof(char) + sizeof(ll) + totalSeek; //每行数据长度
	//初始化参数
	id.clear(); //返回id参数
	ll lineNum = 0; //记录当前行号，用作id
	ll nextLocation; //表头
	vector <ll> & csize = this->table_col_size;
	//打开文件查询
	FILE * fp = fopen(name.data(), "rb");
	//读取头部
	fread(&nextLocation, sizeof(ll), 1, fp);
	ll location;
	while(num > 0){
		num--;
		if(nextLocation == -1){
			fseek(fp, 0, SEEK_END);
			location = ftell(fp);
			ll idx = (location - sizeof(ll)) / lineSeek+1;
			id.push_back(idx);
			break;
		}else{
			//跳过表头和标志位
			fseek(fp, nextLocation+sizeof(ll)+sizeof(char), SEEK_SET);
			ll idx = (nextLocation)/lineSeek + 1;
			id.push_back(idx);
			fread(&nextLocation, sizeof(ll), 1, fp);
		}
	}
	ll n = id.size()-1;
	while(num > 0){
		num--;			
		id.push_back(id[n]+1);
		n++;
	}
	return  true;
}
*/

int DataBase::update(string key, string value, string key2, string value2, string name){
	//判断是否选表
	ll checkCode = check();
	if(checkCode != 0){
		return checkCode;
	}

	vector<ll> id(0);
	vector< vector<string> > ans;
	this->query(key, value, id, ans);
	ll n = id.size();
	ll len = sizeof(char) + sizeof(ll) + sta->table_col_pre_size[table_id][sta->table_col_num[table_id]]*sizeof(char);
	ll idx, selfLen, preSeek, lastSeek, totalSeek;
	if(!this->getKeyLocation(key2, idx, selfLen, preSeek, lastSeek, totalSeek)){
		//查无此字段
		return -1;
	}
	//进行文件操作
	FILE * fp = fopen(sta->table_name[table_id].data(), "rb+");
	for(int i = 0; i < n; i++){
		//跳过表头
		fseek(fp, sizeof(ll), SEEK_SET);
		//跳转所在行
		fseek(fp, (id[i]-1) * (totalSeek+sizeof(ll)+sizeof(char)), SEEK_CUR);
		//跳过有效位和链表位
		fseek(fp, sizeof(char) + sizeof(ll), SEEK_CUR);
		//跳到数据所在位置
		
		fseek(fp, preSeek, SEEK_CUR);
		//写入新数据
		
		fwrite(value2.data(), sizeof(char), selfLen, fp);
		//fflush(fp);
	}
	fclose(fp);
	return 0;
}

bool DataBase::getKeyLocation(string key, ll &idx, ll & selfLen, ll & preSeek, ll & lastSeek, ll & totalSeek){
	idx = -1;
	for(ll i = 0; i < sta->table_col_num[table_id]; i++){
		if(sta->table_col_name[table_id][i] == key){
			idx = i;
		}
	} 
	if(idx == -1) return false; //字段不存在
	//计算前后数据长度 
	selfLen = sta->table_col_size[table_id][idx];
	preSeek = (sta->table_col_pre_size[table_id][idx] - sta->table_col_pre_size[table_id][0])*sizeof(char);
	lastSeek = (sta->table_col_pre_size[table_id][sta->table_col_num[table_id]] - sta->table_col_pre_size[table_id][idx+1])*sizeof(char);
	totalSeek = sta->table_col_pre_size[table_id][sta->table_col_num[table_id]] * sizeof(char);  //数据总长
	return true;
}


int DataBase::clear(){
	return sta->clear();
}

int DataBase::clearTable(string name){
	ll checkCode = check();
	if(checkCode != 0)
		return checkCode;
	if(name == "") name = sta->table_name[table_id];
	ll nextLocation = -1;
	FILE * fp = fopen(name.data(), "w");
	fwrite(&nextLocation, sizeof(ll), 1, fp);
	fclose(fp);
	return 0;
}


