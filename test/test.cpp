#include <windows.h> 
#include<iostream>
#include <cstdio>
using namespace std;

int main(){
	string dataFolder = "test/";
	if (CreateDirectoryA(dataFolder.data(), NULL) ||
    ERROR_ALREADY_EXISTS == GetLastError()){
    	cout << "Yes" << endl; 
	}else{
		cout << "No" << endl;
	}
	string name = "data";
	FILE *fp = fopen( (dataFolder+name).data(), "w");
	if(fp == NULL){
		cout << "No" << endl;
	}
	fclose(fp);
	DeleteFileA( (dataFolder+name).data());
	RemoveDirectoryA(dataFolder.data());
}
