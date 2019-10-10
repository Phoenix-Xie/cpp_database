#include "catch.hpp"
#include "database.h"

bool contain(const vector< vector<string> > & ans, const vector<string> & ans_t){
    ll n = ans.size();
    ll m = ans_t.size();
    for(ll i = 0; i < n; i++){
        bool flag = true;
        for(ll j = 0; j < m; j++){
            if(ans[i][j] != ans_t[j]){
                flag = false;
                break;
            }
        }
        if(flag) return true;
    }
    return false;
}
TEST_CASE( "测试database", "[database]" ) {
    DataBase * da = new DataBase();
    da->clear();
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

    REQUIRE(da->createTable(name, col_name, size, isHash, isUnique) == 0);
    da->chooseTable(name);
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
    vector<ll> id;
    vector< vector<string> > ans;
    REQUIRE(da->insert(data, id) == 0);
    for(ll i = 0; i < m; i++){
        for(ll j = 0; j < n; j++){
            if(i == m-1)
                ll a = 0;
            REQUIRE(da->query(col_name[j],data[i][j], id, ans) == 0);
            REQUIRE(contain(ans, data[i]) == true);
        }
    }
    da->clear();
}