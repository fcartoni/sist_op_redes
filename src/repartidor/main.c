#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "../file_manager/manager.h"

int color_received;
int posicion;
int array_colores[3];

void handle_color_repartidor(int sig, siginfo_t *siginfo, void *context)
{
  
  color_received = siginfo->si_value.sival_int;
  printf("Repartidor %i: Recibi %i\n", getpid(), color_received);
  if (color_received == 1 || color_received == -1){
    array_colores[0] = color_received;
  }
  if (color_received == 2 || color_received == -2){
    array_colores[1] = color_received;
  }
  if (color_received == 3 || color_received == -3){
    array_colores[2] = color_received;
  }
}

int main(int argc, char const *argv[])
{
  printf("I'm the REPARTIDOR process and my PID is: %i\n", getpid());
  while (!0){
    connect_sigaction(SIGUSR1, handle_color_repartidor);
    sleep(3);
    printf("el main de repartidor id %i recibi del excev %s, %s, %s \n",getpid(), argv[0], argv[1], argv[2]);
    if ((array_colores[0] != 0) & (array_colores[1] != 0) & (array_colores[2] != 0)){
      printf("Repartidor en main %i: Recibi %i, %i, %i \n", getpid(), array_colores[0], array_colores[1], array_colores[2]);
      // cada 1 segundo que el repartidos vea si puede avanzar y avance y si no espere otro segundo, 
      // tiene que revisar 1 antes de cada semaforo si esta en verde
    }
  
  }
  
  //printf("Repartidor en main %i: Recibi %i\n", getpid(), color_received);

}
