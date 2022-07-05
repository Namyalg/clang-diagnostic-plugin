#include<cstdlib>

int main() {
	char* p;
	int a;
	int* ptr = (int *)malloc(10);
	p = (char *)malloc(10);
	if(p != nullptr){
		*p = 'a';	
	}
	return 0;
}
