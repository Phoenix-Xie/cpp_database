#include "database.h"
#include "statu.h"
#include <string>
using namespace std;

//莫名其妙的静态私有变量初始化 
DataBase * DataBase::DBinstance = NULL;

DataBase::DataBase(){
	//全局设置 
	this->table_name_max_len = 50;
	this->col_name_max_len = 50;
	this->table_number = 0; 
	FILE * fp = fopen(settings::settings_name.data(), "r");
	//读取已有表数 
	if(fread(&this->table_number, sizeof(ll), 1, fp) == 0){
		this->table_number = 0;
	}
	//设置表名长度
	//设置 
	fclose(fp);
	//表设置 
	fp = fopen(settings::table_settings_name.data(), "a");
	fclose(fp);
}

DataBase * DataBase::getInstance(){
	if(DBinstance == NULL){
		DBinstance = new DataBase(); 
	}
	return DBinstance;
}

void DataBase::closeDataBase(){
	FILE * fp = fopen("settings", "w");
	fwrite(&this->table_number, sizeof(ll), 1, fp);
	fclose(fp);
}

void DataBase::createTable(string name, vector<string> col_name, vector<ll> col_size, vector <char> isHash){
	
	//打开配置文件 
	vector<char> s ;
	FILE * fp = fopen(settings::table_settings_name.data(), "rb+");
	fseek(fp, 0, SEEK_END);
	char valid = 'T';
	//写入有效标志位
	// printf("%d\n", fwrite(&valid, sizeof(char), 1, fp)); 
	// fwrite(&valid, sizeof(char), 1, fp); 
	fwrite(&valid, sizeof(char), 1, fp); 
	//写入文件名 
	fwrite(name.data(), sizeof(char), this->table_name_max_len, fp);
	//写入列数 
	ll len = col_size.size();
	fwrite(&len, sizeof(ll), 1, fp);
	//写入每一列名字 
	for(ll i = 0; i < len; i++){
		fwrite(col_name[i].data(), sizeof(char), this->col_name_max_len, fp);
	}

	//写入每一列长度 
	
	for(ll i = 0; i < len; i++){
		fwrite(&col_size[i], sizeof(ll), 1, fp);
	}
	fclose(fp);



	//创建该数据库文件
	fp = fopen(name.data(), "w");
	
	rewind(fp);
	//写入表头
	ll nextLocation = -1;
	fwrite(&nextLocation, sizeof(ll), 1, fp);
	fclose(fp); 
	this->table_number++;
}

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

int DataBase::deleteTable(string name){
	FILE * fp = fopen(settings::settings_name.data(), "rb+");
	fseek(fp, 0, SEEK_SET);
	char valid; 
	vector <char> s(this->table_name_max_len);
	ll num; 
	while((valid = fgetc(fp)) != EOF){
		printf("tell:%d\n", ftell(fp));
		if(valid == 'T'){
			
			fread(&s[0], sizeof(char), this->table_name_max_len, fp);
			string ss(&s[0]);
			printf("检测到 %s\n", &s[0]);
			if(ss == name){
				printf("进入删除\n"); 
				valid = 'F';
				fseek(fp, -((this->table_name_max_len+1) * sizeof(char)), SEEK_CUR);
				fwrite(&valid, sizeof(char), 1, fp);
				break;
			}
			fread(&num, sizeof(ll), 1, fp);
			fseek(fp, num * (this->col_name_max_len) * sizeof(char), SEEK_CUR);
			fseek(fp, num * sizeof(ll), SEEK_CUR);
		}
		else{
			fseek(fp, this->table_name_max_len * sizeof(char), SEEK_CUR);
			fread(&num, sizeof(ll), 1, fp);
			fseek(fp, num * (this->col_name_max_len) * sizeof(char), SEEK_CUR);
			fseek(fp, num * sizeof(ll), SEEK_CUR);
		}
	}
	//fflush(fp);
	fclose(fp);
}

string DataBase::getTableName(){
	return this->table_name;
}

ll DataBase::getColNum(){
	return this->table_col_num;
}

vector<string> DataBase::getColName(){
	return this->table_col_name;
}

string DataBase::chooseTable(string name){
	bool getFlag = false; //是否读取到标志 
	FILE * fp = fopen(settings::table_settings_name.data(), "rb");
	fseek(fp, 0, SEEK_SET);
	//获取全局变量
	ll clen = this->col_name_max_len;
	//设定临时变量 
	char valid; 
	vector <char> s(this->table_name_max_len);
	ll num; 
	ll lineNum = 0; //当前表所在行号
	while((valid = fgetc(fp)) != EOF){
		// printf("choose:%d\n", ftell(fp));
		//无论有效都应该计算行号
		lineNum++;
		// 判断该行是否有效 
		if(valid == 'T'){
			fread(&s[0], sizeof(char), this->table_name_max_len,fp);
			string ss(&s[0]);
			//判断是否是相同名字 
			if(ss == name){
				printf("找到相关数据表\n");
				this->table_id = lineNum;
				this->table_name = name;
				//读取列数 
				fread(&num, sizeof(ll), 1, fp);
				this->table_col_num = num; 
				//设置引用
				vector<string> & cname = this->table_col_name;
				vector<ll> & csize = this->table_col_size;
				vector<ll> & csizep = this->table_col_pre_size;
				//临时变量
				vector<char> temp_c(clen); 
				//初始化向量 
				cname.resize(this->table_col_num);
				csize.resize(this->table_col_num);
				csizep.resize(this->table_col_num+1); 
				//读取每一列名字 
				for(ll i = 0; i < num; i++){
					fread(&temp_c[0], sizeof(char), clen, fp);
					cname[i] = &temp_c[0];
				}
				//读取每一列的宽度 
				csizep[0] = 0;
				for(ll i = 0; i < num; i++){
					fread(&csize[i], sizeof(ll), 1, fp);
					//printf("csize:%d\n", csize[i]);
					csizep[i+1] = csizep[i] + csize[i];
				}
				getFlag = true; 
				break; 
			}
			//不同名字情况，跳过 
			else{
				ll num;
				fread(&num, sizeof(ll), 1, fp);
				//跳过所有列名 和 列长度 
				fseek(fp, num * sizeof(char) * clen, SEEK_CUR);
				fseek(fp, num * sizeof(ll), SEEK_CUR);
			} 
		}
		else{
			ll num;
			fseek(fp, sizeof(char) * this->table_name_max_len, SEEK_CUR);
			fread(&num, sizeof(ll), 1, fp);
			fseek(fp, num * sizeof(char) * clen, SEEK_CUR);
			fseek(fp, num * sizeof(ll), SEEK_CUR);
		} 
		//todo：该行无效情况未处理
		//todo：未找到情况未处理 
	}
	fclose(fp); 
	if(getFlag){
		printf("%s", this->table_name.data());
		return name;
	}
	else{
		return "未进入";
	}
}

int DataBase::insert(const vector< vector<string> > & s, vector<ll> & id, string name){
	//判断是否选表
	if (name == "" && (this->table_name == "未选择" || this->table_name == ""))
		return -2;
	//todo：多存一个行数 
	printf("start insert\n");

	//初始化参数
	id.clear();

	//用到变量定义
	ll cnum = this->table_col_num; //项数
	char valid = 'T';
	vector <ll> & csize = this->table_col_size;
	vector <ll> & cpsize = this->table_col_pre_size;
	ll totalSeek = this->table_col_pre_size[this->table_col_num] * sizeof(char);  //数据总长
	ll lineSeek = sizeof(char) + sizeof(ll) + totalSeek; //每行数据长度

	//打开文件
	FILE * fp = fopen(this->table_name.data(), "rb+");
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

			ll m = this->table_col_num;
			
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
			ll m = this->table_col_num;
			for(ll j = 0;  j < m; j++){
				fwrite(s[i][j].data(), sizeof(char), csize[j], fp);
			}  
		}

		//插入结束，根据fp位置计算id
		ll fpLocation = ftell(fp);
		id.push_back((fpLocation-sizeof(ll))/lineSeek);
	}
	//返回文件头
	rewind(fp);
	//将最后一次获得的next值写入头部
	fwrite(&nextLocation, sizeof(ll), 1, fp);
	fclose(fp);

	// ll cnum = this->table_col_num;
	// vector <ll> &csize = this->table_col_size;
	// FILE * fp = fopen(this->table_name.data() , "a");
	// // printf("name:%s\n", this->table_name.data());
	// ll line_num = s.size();
	// char valid = 'T';
	// for(ll i = 0; i < line_num; i++){
	// 	//写入数据有效位 
	// 	fwrite(&valid, sizeof(char), 1, fp);
	// 	for(ll j = 0; j < cnum; j++){
	// 		printf("%d %d %s %d\n", i, j, s[i][j].data(), csize[j]);
	// 		fwrite(s[i][j].data(), sizeof(char), csize[j], fp);
	// 	}
	// }
	// fclose(fp);
} 

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

int DataBase::deleteData(string key, string value, string name){
	//判断是否选表
	if (name == "" && (this->table_name == "未选择" || this->table_name == ""))
		return -2;
	//查出id
	vector<ll> id(0);
	vector<vector<string> > ans;
	this->query(key, value, id, ans);
	//初始化变量
	ll n = id.size();
	ll len = sizeof(char) + sizeof(ll) + this->table_col_pre_size[this->table_col_num]*sizeof(char);
	char valid = 'F';
	ll nextLocation = 0;
	ll headLocation = 0;
	//打开文件
	FILE * fp = fopen(this->table_name.data(), "rb+");
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
	if (name == "" && (this->table_name == "未选择" || this->table_name == ""))
		return -2;
	if(name != "")
		this->chooseTable(name);
	name = this->table_name; 

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
	for(ll i = 0; i < this->table_col_num; i++){
		maxLen = max(maxLen, this->table_col_size[i]);
	}
	vector<char> s(maxLen+10);  //临时变量 
	string str;  //临时变量 
	
	
	vector <ll> & csize = this->table_col_size;
	//打开文件查询
	FILE * fp = fopen(name.data(), "rb");
	//跳过头部
	fseek(fp, sizeof(ll), SEEK_SET);
	char valid;
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
				vector<string> line(this->table_col_num); //一行结果 
				for(ll i = 0; i < this->table_col_num; i++){
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
	return true;
}

int DataBase::queryById(vector<ll> &id, vector< vector<string> > & ans, ll id1, ll id2, string name){
	//判断是否选表
	if (name == "" && (this->table_name == "未选择" || this->table_name == ""))
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
	ll totalSeek = this->table_col_pre_size[this->table_col_num] * sizeof(char);  //数据总长
	ll lineSeek = sizeof(char) + sizeof(ll) + totalSeek; //每行数据长度
	//确定最长字段长度
	// todo: 判断是否解决id超过当前行数,以及id合法性
	ll maxLen = 0;
	for(ll i = 0; i < this->table_col_num; i++){
		maxLen = max(maxLen, this->table_col_size[i]);
	}
	vector <char> s(maxLen);
	string str; 
	
	FILE * fp = fopen(this->table_name.data(), "rb+");
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
			vector<string> tempStr(this->table_col_num);
			for(ll i = 0; i < this->table_col_num; i++){
				fread(&s[0], sizeof(char), this->table_col_size[i], fp);
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

int DataBase::predictId(ll num, vector<ll> &id, string name){
	//判断是否选表
	if (name == "" && (this->table_name == "未选择" || this->table_name == ""))
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

int DataBase::update(string key, string value, string key2, string value2, string name){
	//判断是否选表
	if (name == "" && (this->table_name == "未选择" || this->table_name == ""))
		return -2;
	vector<ll> id(0);
	vector< vector<string> > ans;
	this->query(key, value, id, ans);
	ll n = id.size();
	ll len = sizeof(char) + sizeof(ll) + this->table_col_pre_size[this->table_col_num]*sizeof(char);
	ll idx, selfLen, preSeek, lastSeek, totalSeek;
	if(!this->getKeyLocation(key2, idx, selfLen, preSeek, lastSeek, totalSeek)){
		//查无此字段
		return -1;
	}
	//进行文件操作
	FILE * fp = fopen(this->table_name.data(), "rb+");
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
	for(ll i = 0; i < this->table_col_num; i++){
		if(this->table_col_name[i] == key){
			idx = i;
		}
	} 
	if(idx == -1) return false; //字段不存在
	//计算前后数据长度 
	selfLen = this->table_col_size[idx];
	preSeek = (this->table_col_pre_size[idx] - this->table_col_pre_size[0])*sizeof(char);
	lastSeek = (this->table_col_pre_size[this->table_col_num] - this->table_col_pre_size[idx+1])*sizeof(char);
	totalSeek = this->table_col_pre_size[this->table_col_num] * sizeof(char);  //数据总长
	return true;
}

void DataBase::emptyDataBase(){
	FILE * fp = fopen(settings::settings_name.data(), "wb");
	fclose(fp);
	fp = fopen(settings::table_settings_name.data(), "wb");
	fclose(fp);
}

void DataBase::emptyTable(){
	ll nextLocation = -1;
	FILE * fp = fopen(this->table_name.data(), "w");
	fwrite(&nextLocation, sizeof(ll), 1, fp);
	fclose(fp);
}


