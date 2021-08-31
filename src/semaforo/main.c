#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../file_manager/manager.h"

int color_1 = 1;
int color_2 = 2;
int color_3 = 3;

int main(int argc, char const *argv[])
{
  // verde -> positivo
  // rojo -> negativo
  int delay;
  int fabrica_id;
  delay = atoi(argv[1]);
  fabrica_id = atoi(argv[2]);
  while (!0){
    sleep(delay);
    if (*argv[0] == '1'){
      color_1 = color_1 * -1;
      printf("color:%i \n" , color_1);
      send_signal_with_int(fabrica_id, color_1);
    } 
    else if (*argv[0] == '2'){
      color_2 = color_2 * -1;
      printf("color:%i \n" , color_2);
      send_signal_with_int(fabrica_id, color_2);
    }
    else if (*argv[0] == '3'){
      color_3 = color_3 * -1;
      printf("color:%i \n" , color_3);
      send_signal_with_int(fabrica_id, color_3);
    }
    
 }
  
  printf("I'm the SEMAFORO process and my PID is: %i\n", getpid());
  printf("id:%s \n" , argv[0]);
  printf("delay:%s \n" , argv[1]);
  printf("parent:%s \n" , argv[2]);
}
