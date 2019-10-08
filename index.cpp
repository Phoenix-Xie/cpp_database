#include "index.h"
#include <cstdio>
#include <list>

using namespace std;

ll Index::hash(string value, ll mod){
    ll len = value.length();
    ll ans = 0;
    for(int i = 0; i < len; i++){
        ans = ( ans + (value[i]>0?value[i]:-value[i])  * 31)%mod;
    }
    return ans;
} 

Index::Index(ll id){
    sta = Statu::getInstance();
    bucket.clear();
    sidx = id;
}

int Index::read(){
    string name =  sta->table_name[sidx] + "_hash";
    FILE * fp = fopen(name.data(), "r");
    rewind(fp);

    ll cnum = sta->table_col_num[sidx];
    ll n, t;
    bucket.resize(cnum);
    for(ll field = 0; field < cnum; field++){
        if(sta->isHash[sidx][field] == 'T'){
            bucket[field].resize(HASHMOD);
            for(ll i = 0; i < HASHMOD; i++){
                fread(&n, sizeof(ll), 1, fp);
                bucket[field][i].clear();
                for(ll j = 0; j < n; j++){
                    fread(&t, sizeof(ll), 1, fp);
                    bucket[field][i].push_front(t);
                }
            }
        }else{ //该字段不进行hash
            bucket[field].resize(0);
            bucket[field].clear();
        }
    }
    fclose(fp);
    return 0;
}

int Index::save(){
    string name =  sta->table_name[sidx] + "_hash";
    FILE * fp = fopen(name.data(), "w");
    rewind(fp);

    ll cnum = sta->table_col_num[sidx];
    ll n;
    for(ll field = 0; field < cnum; field++){
        if(sta->isHash[sidx][field] == 'T'){
            for(ll i = 0; i < HASHMOD; i++){
                //当前桶中指针数量
                n = bucket[field][i].size();
                fwrite(&n, sizeof(ll), 1, fp);
                //写入链表中每一项
                list<ll>::iterator itr = bucket[field][i].begin();
                while(itr != bucket[field][i].end()){
                    fwrite(&(*itr), sizeof(ll), 1, fp);
                }
            }
        }
    }
    fclose(fp);
    return 0;
}

int Index::create(){
    ll cnum = sta->table_col_num[sidx];
    ll n;
    bucket.resize(cnum);
    for(ll field = 0; field < cnum; field++){
        if(sta->isHash[sidx][field] == 'T'){
            bucket[field].resize(HASHMOD);
            for(ll i = 0; i < HASHMOD; i++){
                bucket[field][i].clear();
            }
        }else{ //该字段不进行hash
            bucket[field].clear();
        }
    } 
    return 0;
}


int Index::insert(const vector< vector<string> > & s, const vector <ll> & addr){
    ll n = s.size();
    //数据为空
    if(n == 0) return 0;

    ll cnum = sta->table_col_num[sidx];
    ll hashCode;
    for(ll i = 0; i < n; i++){
        for(ll j = 0; j < cnum; j++){
            if(sta->isHash[sidx][j] == 'T'){
                hashCode = hash(s[i][j]);
                bucket[j][hashCode].push_front(addr[i]);
            }
        }
    }
    return 0;
}

int Index::deleteData(const vector< vector<string> > &s, const vector <ll> & addr){
    ll n = s.size();
    if(n == 0) return 0;
    ll cnum = sta->table_col_num[sidx];
    ll hashCode, t, k;

    for(ll i = 0; i < n; i++){
        for(ll j = 0; j < cnum; j++){
            if(sta->isHash[sidx][j] == 'T'){
                hashCode = hash(s[i][j]);
                //此处默认每个有效地址唯一
                bucket[i][j].remove(addr[i]);
            }
        }
    }
    return 0;
}

int Index::query(ll idx, string value, list<ll> & addr){
    ll hashCode = hash(value);
    addr = bucket[idx][hashCode];
}