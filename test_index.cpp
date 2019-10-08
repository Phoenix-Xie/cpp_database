#include "catch.hpp"
#include "statu.h"
#include "index.h"

bool contain(ll a, list<ll> b){
    list<ll>::iterator itr = b.begin();
    while(itr != b.end()){
        if(*itr == a)
            return true;
    }
    return false;
}
TEST_CASE( "测试index", "[index]" ) {
    SECTION("测试"){
        Statu * statu = Statu::getInstance();
        statu->clear();
        //创建两张表
        string name = "测试表1";
        vector<string> col_name(3, "字段");
		col_name[0] += "1";
		col_name[1] += "2";
		col_name[2] += "3";
		vector<ll> size(3, 100);
		vector<char> isHash(3, 'F');
		vector<char> isUnique(3, 'F');
		REQUIRE(statu->createTable(name, col_name, size, isHash, isUnique) == 0);
        name = "测试表2";
		col_name.resize(4, "字段");
		col_name[0] += "4";
		col_name[1] += "5";
		col_name[2] += "6";
		col_name[3] += "7";
		size.resize(4, 100);
		isHash.resize(4, 'F');
		isUnique.resize(4, 'F');
		REQUIRE(statu->createTable(name, col_name, size, isHash, isUnique) == 0);
		
        //创建两个index
        Index * index[2];
        for(int i = 0; i < 2; i++){
            index[i] = new Index(i);
            REQUIRE(index[i]->create() == 0);
        }
        vector< vector<string> > s;
        s.clear();
        vector<string> line(3, "test");
        s.push_back(line);
        vector<ll> addr(1,1);
        REQUIRE(index[0]->insert(s, addr) == 0);
        list<ll> addr_l;
        REQUIRE(index[0]->query(0, "test", addr_l) == 0);
        REQUIRE(contain(1, addr_l) == true);
    }
}