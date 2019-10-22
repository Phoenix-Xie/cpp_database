#include "catch.hpp"
#include "statu.h"
#include "index.h"
#include <map>

bool contain(ll a, list<ll> b){
    list<ll>::iterator itr = b.begin();
    while(itr != b.end()){
        if(*itr == a)
            return true;
        itr++;
    }
    return false;
}

bool isSame(list <ll> a){
    map<int, bool> b;
    list<ll>::iterator itr = a.begin();
    for(itr = a.begin(); itr != a.end(); itr++){
        b[*itr] = false;
    }
    for(itr = a.begin(); itr != a.end(); itr++){
        if(b[*itr]){
            return true;
        }
        b[*itr] = true;
    }
    return false;
}
TEST_CASE( "测试index", "[index]" ) {
    SECTION("创建表并测试插入查询,存储"){
        Statu * statu = Statu::getInstance();
        statu->clear();
        //创建两张表
        string name = "测试表1";
        vector<string> col_name(3, "字段");
		col_name[0] += "1";
		col_name[1] += "2";
		col_name[2] += "3";
		vector<ll> size(3, 100);
		vector<char> isHash(3, 'T');
		vector<char> isUnique(3, 'F');
		REQUIRE(statu->createTable(name, col_name, size, isHash, isUnique) == 0);
        name = "测试表2";
		col_name.resize(4, "字段");
		col_name[0] += "4";
		col_name[1] += "5";
		col_name[2] += "6";
		col_name[3] += "7";
		size.resize(4, 100);
		isHash.resize(4, 'T');
		isUnique.resize(4, 'F');
		REQUIRE(statu->createTable(name, col_name, size, isHash, isUnique) == 0);
		
        //创建两个index
        Index * index[2];
        for(int i = 0; i < 2; i++){
            index[i] = new Index(i);
            index[i]->clear();
        }
        //清空index数据
        vector< vector<string> > s;
        s.clear();
        vector<string> line(3, "test");
        s.push_back(line);
        vector<ll> addr(1,1);
        REQUIRE(index[0]->insert(s, addr) == 0);

        //查询
        list<ll> addr_l;
        REQUIRE(index[0]->query(0, "test", addr_l) == 0);
        //包含所需结果
        REQUIRE(contain(1, addr_l) == true);
        //地址唯一
        REQUIRE(isSame(addr_l) == false);
    }
    SECTION("测试读取"){
        Index * index[2];
        for(int i = 0; i < 2; i++){
            index[i] = new Index(i);
            REQUIRE(index[i]->read() == 0);
        }
        list<ll> addr_l;
        REQUIRE(index[0]->query(0, "test", addr_l) == 0);
        REQUIRE(contain(1, addr_l) == true);
    }

    SECTION("大规模单表随机数据测试 插入 查询 保存"){
        Statu * sta = Statu::getInstance();
        sta->clear();
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
        //临时变量
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

        REQUIRE(sta->createTable(name, col_name, size, isHash, isUnique) == 0);
        Index * index = new Index(0);

        fscanf(fp, "%lld", &m);
        data.resize(m);
        for(ll i = 0; i < m; i++){
            data[i].resize(n);
            for(ll j = 0; j < n; j++){
                fscanf(fp, "%s", &str[0]);
                data[i][j] = &str[0];
            }
            addr.push_back(i);
        }
        fclose(fp);
        REQUIRE(index->insert(data, addr) == 0);

        list<ll> addr_l;
        for(ll i = 0; i < m; i++){
            for(ll j = 0; j < n; j++){
                REQUIRE(index->query(j, data[i][j], addr_l) == 0);
                REQUIRE(contain(i, addr_l) == true);
            }
        }
        // REQUIRE(index->save() == 0);
        // REQUIRE(index->save() == 0);
        
    }

    SECTION("大规模数据读取,查询"){
        Statu * sta = Statu::getInstance();
        vector< vector<string> > data; data.clear();
        FILE* fp = fopen("test_data_delete.txt", "r");
        rewind(fp);
        char s[1000];
        fscanf(fp, "%s", s);
        ll id = sta->getIdx(s);
        Index * index = new Index(id);
        REQUIRE(index->read() == 0);

        ll n, t;
        fscanf(fp, "%lld", &n);
        data.resize(n);
        for(ll i = 0; i < n; i++){
            data[i].resize(sta->table_col_num[id]);
            for(ll j = 0; j < sta->table_col_num[id]; j++){
                fscanf(fp, "%s", s);
                data[i][j] = s;
            }
        }
        vector<ll> addr(n);
        for(ll i = 0; i < n; i++){
            fscanf(fp, "%lld", &addr[i]);
        }
        REQUIRE(index->deleteData(data, addr) == 0);
        list<ll> addr_l;
        
        for(ll i = 0; i < n; i++){
            for(ll j = 0; j < sta->table_col_num[id]; j++){
                REQUIRE(index->query(j, data[i][j], addr_l) == 0);
                REQUIRE(contain(addr[i], addr_l) == false);
            }
        }

        fclose(fp);
    }
}