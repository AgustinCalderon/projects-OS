#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void){
  for (int i = 0; i < 100; i++)
  {
    sleep(1);
  }
  
  printf("Hello World\n");
  return 0;
}




