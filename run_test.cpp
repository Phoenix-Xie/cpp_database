#include "catch.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "statu.h"
using namespace std;

int main(){
    printf("run start\n");
    Statu* statu = Statu::getInstance();
    // string name = "²âÊÔ±í1";
    // vector<string> col_name(3, "×Ö¶Î");
    // col_name[0] += "1";
    // col_name[1] += "2";
    // col_name[2] += "3";
    // printf("%s\n", col_name[2].data());
    // vector<ll> size(3, 100);
    // vector<char> isHash(3, 'F');
    // vector<char> isUnique(3, 'F');

    
    for(int i = 0; i < statu->table_number; i++){
        printf("%s\n", statu->table_name[i].data());
        for(int j = 0; j < statu->table_col_num[i]; j++){
            printf("%s\t", statu->table_col_name[i][j].data());
        }
        printf("\n");
    }
}


