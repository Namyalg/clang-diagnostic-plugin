#include<cstdlib>

int main() {
	char* p;
	int a;
	int* ptr = (int *)malloc(10);
	p = (char *)malloc(10);
	
	if(ptr != nullptr){
		
	}
	
	for (int i = 0; i < 5; i++) {
    		ptr[i] = i * 2 + 1;
  		}
	return 0;
}
