#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>

#define MAX_TEST 1000000

int main(){
  int i = 1;
  siginfo_t childState;
  for( ;i <= MAX_TEST;i++){
    int pid = fork();
    if(pid==0){
      printf("\n***********************%d******************************\n",i);
      execl("caltrain","caltrain",0);
      exit(0);
    }
    waitid(P_PID,pid,&childState,WEXITED);
    //sleep(1);
  }
}
