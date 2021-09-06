#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "../file_manager/manager.h"

int color_received;
int posicion = 0;
int array_colores[3];
int cont_turnos = 0;
int array_turnos[4];
int indice_rep;

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

void handle_sigabrt_rep(int sig)
{
  //printf("Abortando repartidores\n");
  //printf("indice del rep a escribir %i\n", indice_rep);
  char nombre_archivo[20];
  sprintf(nombre_archivo, "repartidor_%i.txt", indice_rep);
      
  FILE *output = fopen(nombre_archivo, "w");
    for (int i = 0; i < 4; i++){
      if (array_turnos[i] == 0){
        fprintf(output, "-1");
      }
      else{
        fprintf(output, "%i", array_turnos[i]);
      }
      // No agregamos el separador al último número
      if (i + 1 != 4)
          fprintf(output, ",");
    }

  // Se cierra el archivo (si no hay leak)
  fclose(output);
 
  exit(0);
}

int main(int argc, char const *argv[])
{
  signal(SIGINT, SIG_IGN);
  signal(SIGABRT, handle_sigabrt_rep);
  printf("I'm the REPARTIDOR process and my PID is: %i\n", getpid());
  
  //printf("tengo que completar %s repartidores\n", argv[3]);
  
  // Recibimos los estados por primera vez cuando se crea el repartidor
  int argv0;
  int argv1;
  int argv2;
  int argv3;
  argv0 = atoi(argv[0]);
  argv1 = atoi(argv[1]);
  argv2 = atoi(argv[2]);
  argv3 = atoi(argv[3]);
  array_colores[0] = argv0;
  array_colores[1] = argv1;
  array_colores[2] = argv2;
  indice_rep = argv3 - 1;
  // Hacemos la conexión para que reciba los cambios de estado del semáforo
  connect_sigaction(SIGUSR1, handle_color_repartidor);
  //printf("indice del rep a escribir %i\n", indice_rep);
  // Leemos la posicion_final
  char *filename = argv[4];
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

    //if ((array_colores[0] != 0) & (array_colores[1] != 0) & (array_colores[2] != 0)){
      //signal(SIGABRT, handle_sigabrt_rep);
      cont_turnos += 1;
      if (posicion == (int_pos_sem_1 - 1)){
        printf("entre al primer if\n");
        if (array_colores[0] == 1){
          posicion += 1;
          array_turnos[0] = cont_turnos;
        }
      }
      else if (posicion == (int_pos_sem_2 - 1)){
        printf("entre al segundo if\n");
        if (array_colores[1] == 3){
          posicion += 1;
          array_turnos[1] = cont_turnos;
        }
      }
      else if (posicion == (int_pos_sem_3 - 1)){
        printf("entre al tercer if\n");
        if (array_colores[2] == 5){
          posicion += 1;
          array_turnos[2] = cont_turnos;
        }
      }
      else {
        printf("entre al else\n");
        posicion += 1;
      }
      if (posicion == int_pos_final){
        printf("El repartidor %i llegó a la bodega \n", getpid());
        array_turnos[3] = cont_turnos;
        
        char nombre_archivo[20];
        sprintf(nombre_archivo, "repartidor_%i.txt", indice_rep);
      
        FILE *output = fopen(nombre_archivo, "w");
        for (int i = 0; i < 4; i++){
          fprintf(output, "%i", array_turnos[i]);
          // No agregamos el separador al último número
          if (i + 1 != 4)
            fprintf(output, ",");
          }

          // Se cierra el archivo (si no hay leak)
        fclose(output);
        input_file_destroy(data_in);
      }
      printf("Repartidor en main %i: Recibi %i, %i, %i \n", getpid(), array_colores[0], array_colores[1], array_colores[2]); 
      printf("estado actual semaforos antes de avanzar: %i, %i, %i \n", array_colores[0], array_colores[1], array_colores[2]);
      printf("posicion del repartidor %i: %i \n", getpid(), posicion);
      //printf("array turnos de %i: %i, %i, %i, %i \n",getpid(), array_turnos[0], array_turnos[1], array_turnos[2], array_turnos[3]);
      sleep(1);
    //}
  }
} 
  

