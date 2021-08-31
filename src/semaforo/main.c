#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  printf("I'm the SEMAFORO process and my PID is: %i\n", getpid());
  printf("id:%s \n" , argv[0]);
  printf("delay:%s \n" , argv[1]);
  printf("parent:%s \n" , argv[2]);
}
