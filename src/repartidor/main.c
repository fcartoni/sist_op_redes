#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "../file_manager/manager.h"

int color_received;
int posicion = 0;
int array_colores[3];

void handle_color_repartidor(int sig, siginfo_t *siginfo, void *context)
{
  
  color_received = siginfo->si_value.sival_int;
  printf("Repartidor %i: Recibi %i\n", getpid(), color_received);
  if (color_received == 1 || color_received == 2){
    array_colores[0] = color_received;
  }
  if (color_received == 3 || color_received == 4){
    array_colores[1] = color_received;
  }
  if (color_received == 5 || color_received == 6){
    array_colores[2] = color_received;
  }
}

int main(int argc, char const *argv[])
{
  printf("I'm the REPARTIDOR process and my PID is: %i\n", getpid());
  // sacar el while y recibir la primera vez y dps las señales
  
  // Recibimos los estados por primera vez cuando se crea el repartidor
  int argv0;
  int argv1;
  int argv2;
  argv0 = atoi(argv[0]);
  argv1 = atoi(argv[1]);
  argv2 = atoi(argv[2]);
  array_colores[0] = argv0;
  array_colores[1] = argv1;
  array_colores[2] = argv2;

  // Hacemos la conexión para que reciba los cambios de estado del semáforo
  connect_sigaction(SIGUSR1, handle_color_repartidor);

  // Leemos la posicion_final
  char *filename = "input.txt";
  InputFile *data_in = read_file(filename);

  char* pos_final[] = {data_in->lines[0][3]};
  int int_pos_final;
  int_pos_final = atoi(*pos_final);
  // pos_sem_1
  char* pos_sem_1[] = {data_in->lines[0][0]};
  int int_pos_sem_1;
  int_pos_sem_1 = atoi(*pos_sem_1);
  //pos_sem_2
  char* pos_sem_2[] = {data_in->lines[0][1]};
  int int_pos_sem_2;
  int_pos_sem_2 = atoi(*pos_sem_2);
  //pos_sem_3
  char* pos_sem_3[] = {data_in->lines[0][2]};
  int int_pos_sem_3;
  int_pos_sem_3 = atoi(*pos_sem_3);

  // While de posiciones
  while (posicion < int_pos_final){
    if ((array_colores[0] != 0) & (array_colores[1] != 0) & (array_colores[2] != 0)){
      if (posicion == (int_pos_sem_1 - 1)){
        printf("entre al primer if");
        if (array_colores[0] == 1){
          posicion += 1;
        }
      }
      else if (posicion == (int_pos_sem_2 - 1)){
        printf("entre al segundo if");
        if (array_colores[1] == 3){
          posicion += 1;
        }
      }
      else if (posicion == (int_pos_sem_3 - 1)){
        printf("entre al tercer if");
        if (array_colores[2] == 5){
          posicion += 1;
        }
      }
      else {
        printf("entre al else");
        posicion += 1;
      }
        //printf("Repartidor en main %i: Recibi %i, %i, %i \n", getpid(), array_colores[0], array_colores[1], array_colores[2]); 
      printf("estado actual semaforos antes de avanzar: %i, %i, %i \n", array_colores[0], array_colores[1], array_colores[2]);
      printf("posicion del repartidor %i: %i \n", getpid(), posicion);
      sleep(1);
    }
  }
} // parentesis main
  //while (!0){
  //  connect_sigaction(SIGUSR1, handle_color_repartidor);
  //  sleep(3);
    //printf("el main de repartidor id %i recibi del excev %s, %s, %s \n",getpid(), argv[0], argv[1], argv[2]);
  //  if ((array_colores[0] != 0) & (array_colores[1] != 0) & (array_colores[2] != 0)){
  //    printf("Repartidor en main %i: Recibi %i, %i, %i \n", getpid(), array_colores[0], array_colores[1], array_colores[2]);
      // cada 1 segundo que el repartidos vea si puede avanzar y avance y si no espere otro segundo, 
      // tiene que revisar 1 antes de cada semaforo si esta en verde
    //}
  

