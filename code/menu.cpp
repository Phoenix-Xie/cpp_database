#include "../database.h"
#include <time.h>
#include <string>
#include <windows.h>
#include "cmdui.h"
using namespace std;

int main(){
	
	//todo：做一个交互界面? 
    //windows 下中文输出需要切换字符集
    //system("chcp 65001"); //切换字符??
    system("cls");
	DataBase* d = new DataBase();
	ll opt = 1;
	string chooseTableName = "未选择";
	
	while(opt){
		chooseTableName = d->getTableName();
		if(chooseTableName == "") chooseTableName = "未选择";
		system("cls");
		printf("\
已选中表：%s\n\
----------数据库操作----------\n\
0.退出\n\
1.新增表\n\
*2.删除表\n\
3.选择表\n\
4.显示所有表\n\
5.清空数据库\n\
----------单表操作----------\n\
6.插入数据\n\
*7.显示当前表中数据\n\
8.根据具体名字查询值（where）\n\
9.删除\n\
10.清空当前表\n\
11.更新数据\n\
12.根据id范围选择数据\n\
13.测试插入数据\n\
----------其他额外操作-------\n\
*100.测试\n\
*101.验证存在\n", chooseTableName.data());
		//变量定义 
		vector <string> col_name;
		vector <ll> col_size;
		vector <ll> id(0);
		vector <char> isHash;
		vector < vector<string> > insertData(0);
		vector < vector<string> > queryData;
		char s[1000];
		wchar_t sw[1000];
		string name, name2, str, str2;
		char temp_c;
		ll num, num2;
		int code;
		CmdUI cmdui;
		//操作界面 
		
		//蓝色输入
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | BACKGROUND_INTENSITY|BACKGROUND_GREEN);
		scanf("%d", &opt);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
		clock_t optStartTime = clock();
		if(opt ==  1){  //创建??
			printf("输入表名:");
			scanf("%s", s);
			name = s;
			
			printf("输入列项?:");
			scanf("%lld", &num); 
			
			col_name.resize(num);
			col_size.resize(num);
			isHash.clear();
			isHash.resize(num);
			printf("请输入各项名字\n");  
			for(ll i = 0; i < num; i++){
				
				scanf("%s", s);
				col_name[i] = s;
				
			}
			printf("请输入各项最大长度\n");
			for(ll i = 0; i < num; i++){
				
				scanf("%lld", &col_size[i]);
			}
			printf("输入对于每个字段是否建立hash索引，建立输入T，不建立输入F\n");
			for(ll i = 0; i < num; i++){
				ll hashTemp; 
				scanf("%s", s);
				isHash[i] = s[0]; 
			}
			optStartTime = clock();
			printf("开始创建......\n");
			d->createTable(name, col_name, col_size, isHash);
			printf("创建成功\n");
		} else if(opt == 2){
			// printf("输入希望删除的表?:");
			// scanf("%s", s);
			// name = s;
			// printf("开始删除?......\n");
			// d->deleteTable(name);
			// printf("删除成功\n");
		}else if(opt == 3){
			printf("请输入选择的表:");
			scanf("%s", s);
			name = s;
			optStartTime = clock();
			code = d->chooseTable(name);
			if(code == -1){
				printf("不存在该表\n");
			}else{
				printf("选中\n");
			}
		}else if(opt == 4){
				d->showTables();
		}else if(opt == 5){
				d->clear();
		}else if(opt == 6){
			if(!d->isChoose()){
				printf("请选中一张表\n");
				getchar();
				getchar();
				continue;
			}
			printf("输入希望输入的数据行数?:");
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
			optStartTime = clock();
			code = d->insert(insertData, id);
			if(code == -1){
				printf("未选中表\n");
				continue;
			}
			printf("插入元素id如下\n");

			num = id.size();
			vector< vector<string> > temp(num);
			for(int i = 0; i < num; i++){
				temp[i].push_back(to_string(id[i]));
			}
			cmdui.drawTable(temp);
		}else if(opt == 7){
			 	// if(d->getTableName() == ""){
				// 	printf("请选中一张表\n");
				// 	break;
				// }
				// d->showDatas();
		}else if(opt == 8){
			if(!d->isChoose()){
				printf("请选中一张表\n");
				getchar();
				getchar();
				continue;
			}
			printf("请输入字段名?");
			scanf("%s", s);
			name = s;
			printf("请输入值：");
			scanf("%s", s);
			name2 = s; 
			//获取列名 
			col_name = d->getColName();
			// printf("id\t");
			// for(ll i = 0; i < col_name.size(); i++){
			// 	printf("%s\t", col_name[i].data());
			// }
			// printf("\n"); 
			// //获取数据 
			optStartTime = clock();
			d->query(name, name2, id, queryData);
			// for(ll i = 0; i < queryData.size(); i++){
			// 	printf("%lld\t", id[i]);
			// 	for(ll j = 0; j < queryData[0]. size(); j++){
			// 		printf("%s\t", queryData[i][j].data());
			// 	}
			// 	printf("\n");
			// }
			cmdui.drawTable(queryData, col_name);
		}else if(opt == 9){
			if(!d->isChoose()){
				printf("请选中一张表\n");
				getchar();
				getchar();
				continue;
			}
			printf("请输入字段名?");
			scanf("%s", s);
			name = s;
			printf("请输入值：");
			scanf("%s", s);
			name2 = s;
			//获取数据 
			optStartTime = clock();
			d->deleteData(name, name2);
			break;
		}else if(opt == 10){
			if(!d->isChoose()){
				printf("请选中一张表\n");
				getchar();
				getchar();
				continue;
			}
			d->clearTable();
		}else if(opt == 11){
			if(!d->isChoose()){
				printf("请选中一张表\n");
				getchar();
				getchar();
				continue;
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
			optStartTime = clock();
			d->update(name, name2, str, str2);
		}else if(opt == 12){
			if(!d->isChoose()){
				printf("请选中一张表\n");
				getchar();
				getchar();
				continue;
			}
			printf("请输入起始id\n");
			scanf("%lld", &num);
			printf("请输入终止id, 若输入-1则为默认值\n");
			scanf("%lld", &num2);
			//获取列名 
			col_name = d->getColName(); 
			optStartTime = clock();
			d->queryById(id, queryData, num, num2);
			CmdUI::drawTable(queryData, col_name);
		}else if(opt == 13){
			d->clear();
			//参数设置
			const ll insertNumOnce = 100;
			//变量准备
			string name;
			vector<string> col_name;
			vector<ll> size;
			vector<char> isHash;
			vector<char> isUnique;
			vector< vector< string> > data;
			vector<ll> addr; addr.clear();
			col_name.clear();
			size.clear();
			isHash.clear();
			isUnique.clear();
			data.clear();
			//数据读取
			vector<char> str(10000);
			ll n, m;
			FILE* fp = fopen("test_data.txt", "r");
			rewind(fp);
			fscanf(fp, "%s %lld", &str[0], &n);
			name = &str[0];
			col_name.resize(n);
			for(ll i = 0; i < n; i++){
				fscanf(fp, "%s", &str[0]);
				col_name[i] = &str[0];
			}

			size.resize(n);
			for(ll i = 0; i < n; i++){
				fscanf(fp, "%lld", &size[i]);
			}

			isHash.resize(n);
			for(ll i = 0; i < n; i++){
				fscanf(fp, "%s", &str[0]);
				isHash[i] = str[0];
			}

			isUnique.resize(n);
			for(ll i = 0; i < n; i++){
				fscanf(fp, "%s", &str[0]);
				isUnique[i] = str[0];
			}

			d->createTable(name, col_name, size, isHash, isUnique);
			d->chooseTable(name);
			fscanf(fp, "%lld", &m);
			data.resize(insertNumOnce);
			optStartTime = clock();
			for(ll I = 0; I < m/insertNumOnce; I++){
				for(ll i = 0; i < insertNumOnce; i++){
					data[i].resize(n);
					for(ll j = 0; j < n; j++){
						fscanf(fp, "%s", &str[0]);
						data[i][j] = &str[0];
					}
					addr.push_back(i);
				}
				d->insert(data, id);
			}
			for(ll i = 0; i < m % insertNumOnce; i++){
				data[i].resize(n);
				for(ll j = 0; j < n; j++){
					fscanf(fp, "%s", &str[0]);
					data[i][j] = &str[0];
				}
				addr.push_back(i);
			}
			d->insert(data, id);
			fclose(fp);
		}
		clock_t optEndTime = clock();
		cout << endl << endl;
		for(int i = 0; i < 30; i++) cout << "=";
		cout << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);//设置三色相加
		cout << "本次操作执行耗时 ";

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		
		double optDuringTime = optEndTime - optStartTime;
		cout << optDuringTime/1000;

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);//设置三色相加
		cout << " 秒" << endl;
		
		for(int i = 0; i < 30; i++) cout << "=";
		cout << endl;
		cout << "按任意键继续" << endl;
		getchar();
		getchar();
	} 
} 