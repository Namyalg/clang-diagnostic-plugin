#include<cstdlib>

struct user_defined_struct{
	int a;
    int b;
};

int main(){
    struct user_defined_struct* m;
	
	m = (struct user_defined_struct*)malloc(sizeof(struct user_defined_struct));
	m->a = 0;

    // if(m != nullptr){	
    //     m->b = 34;
	// }
    
    return 0;
}