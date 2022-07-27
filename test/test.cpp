#include<cstdlib>

struct c{
	int c;
};

int main() {

	// 2 DECLARATIONS
	int* ptr = (int *)malloc(4);
	int *var = (int *)malloc(4);

	if(var != nullptr){
		*var = 10;
	}
	
	
	// ARRAY
	if(ptr != nullptr){
		for (int i = 0; i < 5; i++) {
    		ptr[i] = i * 2;
  		}	
	}

	// 2 ASSIGNMENTS
	char* p;

	// USER-DEFINED DATA TYPE structure
	struct c* m;

	p = (char *)malloc(2);
	
	if(p != nullptr){
		*p = '0';	
	}
	
	
	

	m = (struct c*)malloc(sizeof(struct c));
	if(m != nullptr){
		m->c = 0;		
	}
	
	return 0;
}
