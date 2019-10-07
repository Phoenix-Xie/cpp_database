#include "database.h"
int main(){
	
	//todo：做一个交互界面? 
    //windows 下中文输出需要切换字符集
    //system("chcp 65001"); //切换字符??
    system("cls");
	DataBase * d = DataBase::getInstance() ;
	ll opt = 1;
	string chooseTableName = "未选择";
	while(opt){
		system("cls");
		printf("\
已选中表：%s\n\
----------数据库操作----------\n\
0.退出\n\
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
100.测试\n\
101.验证存在\n", chooseTableName.data());
		scanf("%d", &opt);
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
		//操作界面 
		switch(opt){
			case 1:  //创建??
				
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
				printf("开始创建?......\n");
				d->createTable(name, col_name, col_size, isHash);
				printf("创建成功\n");
				break; 
			case 2:
				printf("输入希望删除的表?:");
				scanf("%s", s);
				name = s;
				printf("开始删除?......\n");
				d->deleteTable(name);
				printf("删除成功\n");
				break;
			case 3:
				printf("请输入选择的表??:");
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
			case 6: // ??
				if(d->getTableName() == ""){
					printf("请选中一张表\n");
					break;
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
			case 8:   //??
				if(d->getTableName() == ""){
					printf("请选中一张表\n");
					break;
				}
				printf("请输入字段名?");
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
			case 9: //??
				if(d->getTableName() == ""){
					printf("请选中一张表\n");
					break;
				}
				printf("请输入字段名?");
				scanf("%s", s);
				name = s;
				printf("请输入值：");
				scanf("%s", s);
				name2 = s;
				//获取数据 
				
				d->deleteData(name, name2);
				break;
			case 10: //清空??
				if(d->getTableName() == ""){
					printf("请选中一张表\n");
					break;
				}
				d->emptyTable();
				break;
			case 11: //??
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
} 