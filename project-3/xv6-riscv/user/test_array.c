#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(void)
{
    int size = 1024*15;
    int array[size];
    
    for (int i = 0; i < size; i++){
        array[i] = i;
    }

    for (int i = 0; i < size; i++){
        printf("%d\n", array[i]);
    }
    
    return 0;
}
