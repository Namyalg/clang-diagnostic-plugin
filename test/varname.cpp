#include<bits/stdc++.h>
#include<stdio.h>

using namespace std;
string getVarName(string var){
		int index = 0;
		string allow = "*abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789";
		while(index < var.length() && allow.find(var[index]) != string::npos){
			index += 1;
		}
		return var.substr(0, index);
}

std::string removePointer(std::string str)
	{
		str.erase(remove(str.begin(), str.end(), '*'), str.end());
		return str;
	}

int main(){

    cout <<  removePointer(getVarName("*p"));
    return 0;    
}