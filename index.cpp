#include "index.h"
#include <cstdio>

ll Index::hash(string value, ll mod){
    ll len = value.length();
    ll ans = 0;
    for(int i = 0; i < len; i++){
        ans = ( ans + (value[i]>0?value[i]:-value[i])  * 31)%mod;
    }
    return ans;
} 

Index::Index(){
    isChoose = false;
    isHash.clear();
    bucket.clear();
    nowName = "";
}

int Index::read(string name, ll cnum){
    name += "_hash";
    isHash.resize(cnum);
    FILE * fp = fopen("name", "r");
    rewind(fp);
    for(ll i = 0; i < cnum; i++){
        fread(&isHash[i], sizeof(char), 1, fp);
    }
    ll n;
    bucket.resize(cnum);
    for(ll field = 0; field < cnum; field++){
        //若该字段有建立索引则读取
        if(isHash[field] == 'T'){
            bucket[field].resize(HASHMOD);
            for(ll i = 0; i < HASHMOD; i++){

                fread(&n, sizeof(ll), 1, fp);
                for(ll j = 0; j < n; j++){
                    fread(&bucket[field][i][j], sizeof(ll), 1, fp);
                }
            }
        }
    }
    fclose(fp);
}

int Index::save(string name){
    name += "_hash";
    ll cnum = isHash.size();
    FILE * fp = fopen("name", "w");
    rewind(fp);
    for(ll i = 0; i < cnum; i++){
        fwrite(&isHash[i], sizeof(char), 1, fp);
    }
    ll n;
    for(ll field = 0; field < cnum; field++){
        if(isHash[field] == 'T'){
            for(ll i = 0; i < HASHMOD; i++){
                n = bucket[field][i].size();

                fwrite(&n, sizeof(ll), 1, fp);
                for(ll j = 0; j < n; j++){
                    fwrite(&bucket[field][i][j], sizeof(ll), 1, fp);
                }
            }
        }
    }
    fclose(fp);
}

int Index::create(string name, const vector<char> & isHash){
    ll n = isHash.size();
    //写入该表hash设置
    bucket.resize(n);
    this->isHash = isHash;
    for(ll i = 0; i < n; i++){
        if(isHash[i] == 'T'){
            bucket[i].resize(HASHMOD);
            for(ll j = 0; j < HASHMOD; j++){
                bucket[i][j].resize(0);
            }
        }
    }
    save(name);
    isChoose = true;
    nowName = name;
}

int Index::choose(string name, ll cnum){
    if(isChoose){
        save(nowName);
    }
    read(name, cnum);
    isChoose = true;
    nowName = name;
}

int Index::insert(const vector< vector<string> > & s, const vector <ll> & addr){
    ll n = s.size();
    //无输入数据
    if(n == 0) return 0;

    ll m = s.size();
    ll hashCode;
    for(ll i = 0; i < n; i++){
        for(ll j = 0; j < m; j++){
            if(isHash[j] == 'T'){
                hashCode = hash(s[i][j]);
                bucket[j][hashCode].push_back(addr[i]);
            }
        }
    }
}

int Index::deleteData(const vector< vector<string> > &s, const vector <ll> & addr){
    ll n = s.size();
    if(n == 0) return 0;
    ll m = s[0].size();
    ll hashCode, t, k;
    for(ll i = 0; i < n; i++){
        for(ll j = 0; j < m; j++){
            if(isHash[j] == 'T'){
                hashCode = hash(s[i][j]);
                t = bucket[j][hashCode].size();
                for(k = 0; k < t; k++){
                    if(bucket[j][hashCode][k] == addr[i]){
                        break;
                    }
                }
                vector<ll>::iterator itr = bucket[j][hashCode].begin();
                bucket[j][hashCode].erase(itr+k);
            }
        }
    }
}

int Index::query(ll idx, string value, vector<ll> & addr){
    ll hashCode = hash(value);
    addr = bucket[idx][hashCode];
}