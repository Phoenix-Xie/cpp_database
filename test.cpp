#include "catch.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "statu.h"
using namespace std;



TEST_CASE( "statu is work", "[statu]" ) {
	//测试是否会因为外部vector改变而改变

	Statu * statu = Statu::getInstance();
	SECTION("清空数据"){
		FILE* fp = fopen(settings::table_settings_name.data(), "w");
		fclose(fp);
	}

	SECTION("创建第一张表"){
		string name = "测试表1";
		vector<string> col_name(3, "字段");
		col_name[0] += "1";
		col_name[1] += "2";
		col_name[2] += "3";
		REQUIRE(col_name[2] == "字段3");
		vector<ll> size(3, 100);
		vector<char> isHash(3, 'F');
		vector<char> isUnique(3, 'F');

		statu->createTable(name, col_name, size, isHash, isUnique);
		string str = "测试表1";
		REQUIRE(statu->table_name[0] == str);
		REQUIRE(statu->table_col_num[0] == 3);
		
		REQUIRE(statu->table_col_name[0][0] == "字段1");
		REQUIRE(statu->table_col_name[0][1] == "字段2");
		REQUIRE(statu->table_col_name[0][2] == "字段3");

		
	}
	SECTION("第二次打开"){
		string str = "测试表1";
		REQUIRE(statu->table_name[0] == str);
		REQUIRE(statu->table_col_num[0] == 3);
		
		REQUIRE(statu->table_col_name[0][0] == "字段1");
		REQUIRE(statu->table_col_name[0][1] == "字段2");
		REQUIRE(statu->table_col_name[0][2] == "字段3");
	}

	SECTION("改了改vector看是否影响"){
		string name = "测试表2";
		vector<string> col_name(3, "字段");
		col_name[0] += "4";
		col_name[1] += "5";
		col_name[2] += "6";
		vector<ll> size(3, 100);
		vector<char> isHash(3, 'F');
		vector<char> isUnique(3, 'F');

		statu->createTable(name, col_name, size, isHash, isUnique);
		
		col_name[0] = "字段10";
		string str = "测试表1";
		REQUIRE(statu->table_name[0] == str);
		REQUIRE(statu->table_col_num[0] == 3);
		
		REQUIRE(statu->table_col_name[0][0] == "字段1");
		REQUIRE(statu->table_col_name[0][1] == "字段2");
		REQUIRE(statu->table_col_name[0][2] == "字段3");
	}
	// SECTION("first"){
	// 	REQUIRE( Factorial(0) == 1);
	// }
	// SECTION("second"){
	// 	REQUIRE( Factorial(1) == 1 );
	// 	REQUIRE( Factorial(2) == 2 );
	// 	REQUIRE( Factorial(3) == 6 );
	// 	REQUIRE( Factorial(10) == 3628800 );
	// 	REQUIRE( Factorial(0) == 1);
	// }
	// SECTION("third"){
	// 	REQUIRE( Factorial(0) == 1);
	// }
}

