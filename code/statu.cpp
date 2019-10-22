#include "statu.h"
#include <windows.h>

using namespace std;

Statu* Statu::instance = NULL;

Statu::Statu(){
    read();
}

Statu* Statu::getInstance(){
    if(instance == NULL){
        instance = new Statu();
    }
    //printf("instance finnal\n");
    return instance;
}

bool Statu::isNameRepeat(string name){
    for(ll i = 0; i < table_number; i++){
        if(name == table_name[i])
            return true;
    }
    return false;
}

int Statu::save(){
    FILE * fp = fopen( (settings::dataFolder + settings::table_settings_name).data(), "w");
    //printf("save\n");
    char valid = 'T';
    fwrite(&valid, sizeof(char), 1, fp);
    fwrite(&table_number, sizeof(ll), 1, fp);
    for(ll i = 0; i < table_number; i++){
        fwrite(table_name[i].data(), sizeof(char), settings::table_name_max_len, fp);
        fwrite(&table_col_num[i], sizeof(ll), 1, fp);
        for(ll j = 0; j < table_col_num[i]; j++){
            fwrite(table_col_name[i][j].data(), sizeof(char), settings::col_name_max_len, fp);
            string temp = table_col_name[i][j];
            fwrite(&table_col_size[i][j], sizeof(ll), 1, fp);
            fwrite(&isHash[i][j], sizeof(char), 1, fp);
            fwrite(&isUnique[i][j], sizeof(char), 1, fp);
        }
    }
    fclose(fp);
    //printf("save final\n");
    return 0;
}

int Statu::read(){
    
    FILE * fp = fopen((settings::dataFolder + settings::table_settings_name).data(), "r");
    //printf("read\n");
    //文件不存在
    if(fp == NULL){
        table_number = 0;
        table_name.clear();
        table_col_num.clear();
        table_col_name.clear();
        table_col_size.clear();
        table_col_pre_size.clear();
        return 0;
    }
    //文件存在但无内容
    char valid;
    valid = fgetc(fp);
    if(valid == EOF){
        table_number = 0;
        table_name.clear();
        table_col_num.clear();
        table_col_name.clear();
        table_col_size.clear();
        table_col_pre_size.clear();
        return 0;
    }

    fread(&table_number, sizeof(ll), 1, fp);

    vector<char> s(settings::table_name_max_len);
    vector<char> ss(settings::col_name_max_len);
    //清空变量
    table_name.resize(table_number);
    table_col_num.resize(table_number);
    table_col_name.resize(table_number);
    table_col_size.resize(table_number);
    table_col_pre_size.resize(table_number);
    isHash.resize(table_number);
    isUnique.resize(table_number);

    for(ll i = 0; i < table_number; i++){
        fread(&s[0], sizeof(char), settings::table_name_max_len, fp);
        table_name[i] = &s[0];
        fread(&table_col_num[i], sizeof(ll), 1, fp);
        table_col_name[i].resize(table_col_num[i]);
        table_col_size[i].resize(table_col_num[i]);
        table_col_pre_size[i].resize(table_col_num[i]+1);
        isHash[i].resize(table_col_num[i]);
        isUnique[i].resize(table_col_num[i]);

        table_col_pre_size[i][0] = 0;
        for(ll j = 0; j < table_col_num[i]; j++){    
            fread(&ss[0], sizeof(char), settings::col_name_max_len, fp);
            
            table_col_name[i][j] = &ss[0];
            fread(&table_col_size[i][j], sizeof(ll), 1, fp);
            //维护前缀和
            table_col_pre_size[i][j+1] = table_col_pre_size[i][j] + table_col_size[i][j];

            fread(&isHash[i][j], sizeof(char), 1, fp);
            fread(&isUnique[i][j], sizeof(char), 1, fp);
        }
    }
    fclose(fp);
    return 0;
}

int Statu::createTable(string name, vector<string> col_name, vector<ll> size, vector<char> isHash, vector<char> isUnique){

    
    //若名字重复
    if(isNameRepeat(name)){
        return -1;
    }


    ll num = col_name.size();

    //默认参数设置
    if(isUnique.size() == 0){
        isUnique = vector<char>(num, 'F');
    }

    //检查长度
    if(num != size.size()){
        return -201;
    }
    if(num != isHash.size()){
        return -202;
    }
    
    if(num != isUnique.size()){
        return -203;
    }
    //新增一张表设置
    
    table_number++;
    this->table_name.push_back(name);
    this->table_col_name.push_back(col_name);
    //printf("%s\n", col_name[2].data());
    //printf("%s\n", table_col_name[0][2].data());
    this->table_col_size.push_back(size);
    
    this->table_col_num.push_back(num);

    vector<ll> pre_size(size.size()+1);
    pre_size[0] = 0;
    ll n = pre_size.size();
    for(ll i = 1; i < n; i++){
        pre_size[i] = pre_size[i-1] + size[i-1];
    }
    this->table_col_pre_size.push_back(pre_size);
    this->isHash.push_back(isHash);
    this->isUnique.push_back(isUnique);
    save();
    return 0;
}

int Statu::getIdx(string name){
    for(ll i = 0; i < table_number; i++){
        if(name == table_name[i]){
            return i;
        }
    }
    return -1;
}

int Statu::clear(){
    //由于为状态类，因此应注意此处应该将所有自身变量清空
    table_number = 0;
    table_name.clear();
    table_col_num.clear();
    table_col_name.clear();
    table_col_size.clear();
    table_col_pre_size.clear();
    isHash.clear();
    isUnique.clear();
    FILE * fp = fopen((settings::dataFolder + settings::table_settings_name).data(), "w");
    fclose(fp);
    //删除文件
    DeleteFileA((settings::dataFolder + settings::table_settings_name).data());
    return 0;
}