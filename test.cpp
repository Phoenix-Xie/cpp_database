#include <cstdio>
#include <iostream>
#include <vector>
#define ll long long
using namespace std;

int main(){
//    FILE * fp = fopen("table_settings_name.ini", "r");
//    rewind(fp);
//    char valid;
//    char s[1000];
//    fread(&valid, sizeof(char), 1, fp);
//    printf("%c ", valid);
//    fread(s, sizeof(char), 50, fp);
//    printf("%s\n", s);
//    for(int i = 0; i < 8; i++){
//    	fread(s, sizeof(char), 50, fp);
//    	printf("%s ", s);
//    }
//    fclose(fp);
//	FILE *fp = fopen("Êé", "rb+");
//	char s[1000];
//	char valid;
//	rewind(fp);
//	fseek(fp, 809, SEEK_SET);
//	fread(&s, sizeof(char), 1, fp);
//	printf("%c", valid); 
	vector< vector< vector<ll> > > pointBucket(0);
	vector< vector<ll> > test(0);
	vector<ll> a(0);
	a.push_back(1);
	a.push_back(2);
	test.push_back(a);
	pointBucket.push_back(test);
	for(int i = 0; i < pointBucket.size(); i++){
		for(int j = 0; j < pointBucket[0].size(); j++){
			for(int k = 0; k < pointBucket[0][0].size(); k++){
				printf("%lld\n", pointBucket[i][j][k]);
			}
		}
	}
}
