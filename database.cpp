#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <memory.h>
#define ll long long
using namespace std;

namespace settings{
	string settings_name = "settings.ini";
	string table_settings_name = "table_settings_name.ini";
	string point_bucket_name = "point_bucket_name";
}


class DataBase{
/*
编写规范：
1. 任何size变量 指的是多少个char， sizec 代表多少个 size* sizeof(char) 
2. 所有传入的参数不应该考虑问题 1， 即避免使用者计算
3. 行号从1开始计数

//返回码含义：
0.成功
-1. 未知错误失败
-2. 未选择表
-3. 未找到数据
*/ 
	
private:
	ll table_name_max_len;
	ll col_name_max_len;
	ll table_number;
	static DataBase * DBinstance;
	//当前表配置
	string table_name; //表名 
	ll table_col_num; //列数量  
	vector<string> table_col_name; //列名
	vector<ll> table_col_size; // 当前表每列长度
	vector<ll> table_col_pre_size; //每列表长前缀和 
	ll table_id;
	//todo：做一个表长度前缀和数组
	//todo：做一个内存池，栈式，用磁盘-缓存形式 

	//指针哈希木桶	
	//第一维：每个表 第二维：每个字段 第三维：每个桶
	vector< vector< vector<ll> > > pointBucket;
	//是否开启hash，标志位
	vector< vector<bool> > isHash();
	ll hash(string value, ll mod = 10000){
		ll len = value.length();
		ll ans = 0;
		for(int i = 0; i < len; i++){
			ans += (value[i] * 31)%mod;
		}
		return ans;
	}

	//全局设置
	DataBase(){
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
public:
	/*索引代码*/
	void savePointBucket(){
		vector< vector< vector<ll> > >& a = this->pointBucket;
		FILE * fp = fopen(settings::point_bucket_name.data(), "w");
		
		ll n = a.size();
		fwrite(&n, sizeof(ll), 1, fp);
		for(ll i = 0; i < n; i++){
			ll m = a[i].size();
			fwrite(&m, sizeof(ll), 1, fp);
			for(ll j = 0; j < m; j++){
				ll nm = a[i][j].size();
				fwrite(&nm, sizeof(ll), 1, fp);
				for(ll k = 0; k < nm; k++){
					
				}
			}
		}
	}

	/*数据库本身操作*/ 
	static DataBase * getInstance(){
		if(DBinstance == NULL){
			DBinstance = new DataBase(); 
		}
		return DBinstance;
	}
	
	void closeDataBase(){
		FILE * fp = fopen("settings", "w");
		fwrite(&this->table_number, sizeof(ll), 1, fp);
		fclose(fp);
	}
	
	/*表生成销毁*/ 
	//todo：重名判断 
    //创建数据库记录
	void createTable(string name, vector<string> col_name, vector<ll> col_size){
		
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

		//生成索引位置
		vector< vector<ll> > tableBucket(0);
		this->pointBucket.push_back(tableBucket);

		//创建该数据库文件
		fp = fopen(name.data(), "w");
		
        rewind(fp);
        //写入表头
        ll nextLocation = -1;
        fwrite(&nextLocation, sizeof(ll), 1, fp);
        fclose(fp); 
		this->table_number++;
	}
	
	void showTables(){
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
	
	int deleteTable(string name){
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

	//获取数据库当前表信息
	string getTableName(){
		return this->table_name;
	}

    ll getColNum(){
		return this->table_col_num;
	}
	
	vector<string> getColName(){
		return this->table_col_name;
	}

	//单表操作	
    //选择表 
	string chooseTable(string name){
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

    //增
	int insert(vector< vector<string> > s, vector<ll> & id, string name = ""){
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
	
	void showDatas(string name = ""){
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
	
	//删
	int deleteData(string key, string value, string name = ""){
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

	// 查
	int query(string key, string value, vector<ll> & id, vector< vector<string> > & ans, string name = ""){
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

	int queryById(vector<ll> &id, vector< vector<string> > & ans, ll id1 = 1, ll id2 = -1, string name = ""){
		//判断是否选表
		if (name == "" && (this->table_name == "未选择" || this->table_name == ""))
			return -2;
		//参数处理
		ans.clear();
		id.clear();
		if(id2 == -1){
			id2 = 0x3f3f3f3f3f3f3f3f;
		}
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

	int predictId(ll num, vector<ll> &id, string name = ""){
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

	//改
	int update(string key, string value, string key2, string value2, string name = ""){
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
	
	//查询某个字段整行数据段的位置(不包含标志位和空闲链表位)
	bool getKeyLocation(string key, ll &idx, ll & selfLen, ll & preSeek, ll & lastSeek, ll & totalSeek){
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

	//清空数据库
	void emptyDataBase(){
		FILE * fp = fopen(settings::settings_name.data(), "wb");
		fclose(fp);
		fp = fopen(settings::table_settings_name.data(), "wb");
		fclose(fp);
	}

	//清空数据表
	void emptyTable(){
		ll nextLocation = -1;
		FILE * fp = fopen(this->table_name.data(), "w");
		fwrite(&nextLocation, sizeof(ll), 1, fp);
		fclose(fp);
	}


};
//莫名其妙的静态私有变量初始化 
DataBase * DataBase::DBinstance = NULL;

int main(){
	//todo：做一个交互界面 
    //windows 下中文输出需要切换字符集
    //system("chcp 65001"); //切换字符集
    system("cls");
	DataBase * d = DataBase::getInstance() ;
	ll opt = 1;
	string chooseTableName = "未选择";
	while(opt){
		system("cls");
		printf("\
已选中表：%s\n\
----------数据库操作----------\n\
0.退出 \n\
1.新增表\n\
2.删除表\n\
3.选择表\n\
4.显示所有表\n\
5.清空数据库\n\
----------单表操作----------\n\
6.插入数据\n\
7.显示当前表中数据\n\
8.根据具体名字查询值（where）\n\
9.删除\n\
10.清空当前表\n\
11.更新数据\n\
12.根据id范围选择数据\n\
13.获取预测id\n\
----------其他额外操作-------\n\
100.测试\n", chooseTableName.data());
		scanf("%d", &opt);
		//变量定义 
		vector <string> col_name;
		vector <ll> col_size;
		vector <ll> id(0);
		vector < vector<string> > insertData(0);
		vector < vector<string> > queryData;
		char s[1000];
		wchar_t sw[1000];
		string name, name2, str, str2;
		ll num, num2;
		//操作界面 
		switch(opt){
			case 1:
				
				printf("输入表名:");
				scanf("%s", s);
				name = s;
				
				printf("输入列项数:");
				scanf("%d", &num); 
				
				col_name.resize(num);
				col_size.resize(num);
				printf("请输入各项名字\n");  
				for(ll i = 0; i < num; i++){
					
					scanf("%s", s);
					col_name[i] = s;
					
				}
				printf("请输入各项最大长度\n");
				for(ll i = 0; i < num; i++){
					
					scanf("%d", &col_size[i]);
				}
				printf("开始创建......\n");
				d->createTable(name, col_name, col_size);
				printf("创建成功\n");
				break; 
			case 2:
				printf("输入希望删除的表名:");
				scanf("%s", s);
				name = s;
				printf("开始删除......\n");
				d->deleteTable(name);
				printf("删除成功\n");
				break;
			case 3:
				printf("请输入选择的表名:");
				scanf("%s", s);
				name = s;
				chooseTableName = d->chooseTable(name);
				break;
			case 4:
				d->showTables();
				break;
			case 5:
				d->emptyDataBase();
				break;
			case 6:
				if(d->getTableName() == ""){
					printf("请选中一张表\n");
					break;
				}
				printf("输入希望输入的数据行数:");
				scanf("%lld", &num);
				num2 = d->getColNum();
				
				for(ll i = 0; i < num; i++){
					printf("输入第 %lld 行数据的 %lld 项数据\n", i+1, num2);
					vector<string> line(num2);
					for(ll j = 0; j < num2; j++){
						scanf("%s", s);
						line[j] = s;
						//printf("%s\n", s);
					}
					insertData.push_back(line);
				} 
				d->insert(insertData, id);
				printf("插入元素id如下\n");
				for(ll i = 0; i < id.size(); i++){
					printf("%lld\n", id[i]);
				}
				break; 
			case 7:
			 	if(d->getTableName() == ""){
					printf("请选中一张表\n");
					break;
				}
				d->showDatas();
				break;
			case 8:   //查
				if(d->getTableName() == ""){
					printf("请选中一张表\n");
					break;
				}
				printf("请输入字段名：");
				scanf("%s", s);
				name = s;
				printf("请输入值：");
				scanf("%s", s);
				name2 = s; 
				//获取列名 
				col_name = d->getColName();
				printf("id\t");
				for(ll i = 0; i < col_name.size(); i++){
					printf("%s\t", col_name[i].data());
				}
				printf("\n"); 
				//获取数据 
				
				d->query(name, name2, id, queryData);
				for(ll i = 0; i < queryData.size(); i++){
					printf("%lld\t", id[i]);
					for(ll j = 0; j < queryData[0]. size(); j++){
						printf("%s\t", queryData[i][j].data());
					}
					printf("\n");
				}
				break;
			case 9: //删
				if(d->getTableName() == ""){
					printf("请选中一张表\n");
					break;
				}
				printf("请输入字段名：");
				scanf("%s", s);
				name = s;
				printf("请输入值：");
				scanf("%s", s);
				name2 = s;
				//获取数据 
				
				d->deleteData(name, name2);
				break;
			case 10: //清空表
				if(d->getTableName() == ""){
					printf("请选中一张表\n");
					break;
				}
				d->emptyTable();
				break;
			case 11: //改
				if(d->getTableName() == ""){
					printf("请选中一张表\n");
					break;
				}
				printf("请输入查询字段名\n");
				scanf("%s", s);
				name = s;
				printf("请输入查询值\n");
				scanf("%s", s);
				name2 = s;
				printf("请输入修改字段名\n");
				scanf("%s", s);
				str = s;
				printf("请输入修改后值\n");
				scanf("%s", s);
				str2 = s;
				d->update(name, name2, str, str2);
				break;
			case 12:  //查id
				if(d->getTableName() == ""){
					printf("请选中一张表\n");
					break;
				}
				printf("请输入起始id\n");
				scanf("%lld", &num);
				printf("请输入终止id, 若输入-1则为默认值\n");
				scanf("%lld", &num2);
				//获取列名 
				col_name = d->getColName();
				printf("id\t");
				for(ll i = 0; i < col_name.size(); i++){
					printf("%s\t", col_name[i].data());
				}
				printf("\n"); 
				d->queryById(id, queryData, num, num2);
				for(ll i = 0; i < queryData.size(); i++){
					printf("%lld\t", id[i]);
					for(ll j = 0; j < queryData[0]. size(); j++){
						printf("%s\t", queryData[i][j].data());
					}
					printf("\n");
				}
				break;
			case 13: //预测
				if(d->getTableName() == ""){
					printf("请选中一张表\n");
					break;
				}
				printf("输入数量\n");
				scanf("%lld", &num);
				d->predictId(num, id);
				num = id.size();
				for(int i = 0; i < num; i++){
					printf("%lld\n", id[i]);
				}
				break;
			case 100:
				memset(s, 0, sizeof(s));
				scanf("%s", s);
				str = s;
				scanf("%s", s);
				name = s;
				if(name == str)
					printf("YES");
				else
					printf("No");
				
				for(int i = 0; i < 10; i++){
					printf("%x ", s[i]);
				}
				printf("\n");
				s[2] = 0;
				printf("%s\n", s);
				
				break;
			
				
		}
		getchar();
		getchar();
	} 
	d->closeDataBase();
//	vector<string> s(2);
//	s[0] = "123";
//	s[1] = "456";
//	vector<ll> t(2);
//	t[0] = 1;
//	t[1] = 2;
//	d->createTable("testTable", s, t);
//	d->createTable("testTable2", s, t);
//	d->deleteTable("testTable2");
//	d->showColumLine("testTable");
//	d->closeDataBase(); 
//	d->chooseTable("tantan");
//	d->showTables(); 
//	vector<vector<string> > s(1);
//	vector<string> t(2);
//	t[0] = "1";
//	t[1] = "2";
//	s[0] = t;
//	d->insert(s);
} 