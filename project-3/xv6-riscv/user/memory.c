#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int g = 0;

void foo();

int main(int argc, char** argv) {
    int a = 0;
    printf("main:  %p\n", main);
    printf("foo:   %p\n", foo);
    printf("gbl:   %p\n", &g);
    printf("heap:  %p\n", malloc(sizeof(int)));
    printf("argv:  %p\n", argv);
    printf("arg0:  %p\n", argv[0]);
    printf("arg1:  %p\n", argv[1]);
    printf("arg2:  %p\n", argv[2]);
    printf("arg3:  %p\n", argv[3]);
    printf("stack: %p\n", &a);
    return 0;
}

void foo() { }