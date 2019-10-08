#include "catch.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "statu.h"
using namespace std;



TEST_CASE( "statu is work", "[statu]" ) {
	//�����Ƿ����Ϊ�ⲿvector�ı���ı�

	Statu * statu = Statu::getInstance();
	SECTION("�������"){
		FILE* fp = fopen(settings::table_settings_name.data(), "w");
		fclose(fp);
	}

	SECTION("������һ�ű�"){
		string name = "���Ա�1";
		vector<string> col_name(3, "�ֶ�");
		col_name[0] += "1";
		col_name[1] += "2";
		col_name[2] += "3";
		REQUIRE(col_name[2] == "�ֶ�3");
		vector<ll> size(3, 100);
		vector<char> isHash(3, 'F');
		vector<char> isUnique(3, 'F');

		statu->createTable(name, col_name, size, isHash, isUnique);
		string str = "���Ա�1";
		REQUIRE(statu->table_name[0] == str);
		REQUIRE(statu->table_col_num[0] == 3);
		
		REQUIRE(statu->table_col_name[0][0] == "�ֶ�1");
		REQUIRE(statu->table_col_name[0][1] == "�ֶ�2");
		REQUIRE(statu->table_col_name[0][2] == "�ֶ�3");

		
	}
	SECTION("�ڶ��δ�"){
		string str = "���Ա�1";
		REQUIRE(statu->table_name[0] == str);
		REQUIRE(statu->table_col_num[0] == 3);
		
		REQUIRE(statu->table_col_name[0][0] == "�ֶ�1");
		REQUIRE(statu->table_col_name[0][1] == "�ֶ�2");
		REQUIRE(statu->table_col_name[0][2] == "�ֶ�3");
	}

	SECTION("���˸�vector���Ƿ�Ӱ��"){
		string name = "���Ա�2";
		vector<string> col_name(3, "�ֶ�");
		col_name[0] += "4";
		col_name[1] += "5";
		col_name[2] += "6";
		vector<ll> size(3, 100);
		vector<char> isHash(3, 'F');
		vector<char> isUnique(3, 'F');

		statu->createTable(name, col_name, size, isHash, isUnique);
		
		col_name[0] = "�ֶ�10";
		string str = "���Ա�1";
		REQUIRE(statu->table_name[0] == str);
		REQUIRE(statu->table_col_num[0] == 3);
		
		REQUIRE(statu->table_col_name[0][0] == "�ֶ�1");
		REQUIRE(statu->table_col_name[0][1] == "�ֶ�2");
		REQUIRE(statu->table_col_name[0][2] == "�ֶ�3");
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

