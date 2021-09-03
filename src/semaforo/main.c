#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "../file_manager/manager.h"

int color_1 = 1;
int color_2 = 3;
int color_3 = 5;
int cambios = 0;
int id_sem;


void handle_sigabrt_sem(int sig)
{
  //printf("Abortando semaforos\n");
  //printf("el semaforo %c cambio %i veces \n", id_sem, cambios);
  //Escribir archivo
  char nombre_archivo[20];
  sprintf(nombre_archivo, "semaforo_%c.txt", id_sem);
      
  FILE *output = fopen(nombre_archivo, "w");
    fprintf(output, "%i", cambios);
  // Se cierra el archivo (si no hay leak)
  fclose(output);
  exit(0);
}

int main(int argc, char const *argv[])
{
  signal(SIGINT, SIG_IGN);
  signal(SIGABRT, handle_sigabrt_sem);
  printf("I'm the SEMAFORO process and my PID is: %i\n", getpid());
  // verde -> positivo
  // rojo -> negativo
  int delay;
  int fabrica_id;
  delay = atoi(argv[1]);
  fabrica_id = atoi(argv[2]);
  id_sem = *argv[0];
  while (!0){
    //signal(SIGABRT, handle_sigabrt_sem);
    sleep(delay);
    if (*argv[0] == '1'){
      if (color_1 == 1){
        color_1 += 1;
      }
      else {
        color_1 -= 1;
      }
      cambios += 1;
      //printf("color:%i \n" , color_1);
      send_signal_with_int(fabrica_id, color_1);
    } 
    else if (*argv[0] == '2'){
      if (color_2 == 3){
        color_2 += 1;
      }
      else {
        color_2 -= 1;
      }
      cambios += 1;
      //printf("color:%i \n" , color_2);
      send_signal_with_int(fabrica_id, color_2);
    }
    else if (*argv[0] == '3'){
      if (color_3 == 5){
        color_3 += 1;
      }
      else {
        color_3 -= 1;
      }
      cambios += 1;
      //printf("color:%i \n" , color_3);
      send_signal_with_int(fabrica_id, color_3);
    }
    
 }
  //printf("I'm the SEMAFORO process and my PID is: %i\n", getpid());
  //printf("id:%s \n" , argv[0]);
  //printf("delay:%s \n" , argv[1]);
  //printf("parent:%s \n" , argv[2]);
}
