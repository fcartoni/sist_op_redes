#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>

#include "../file_manager/manager.h"
//#include "../semaforo/main.c"
//#include "../repartidor/main.c"

int fabrica;
int semaforo_1;
int semaforo_2;
int semaforo_3;

int main(int argc, char const *argv[])
{
  printf("I'm the DCCUBER process and my PID is: %i\n", getpid());

  char *filename = "input.txt";
  InputFile *data_in = read_file(filename);

  printf("Leyendo el archivo %s...\n", filename);
  printf("- Lineas en archivo: %i\n", data_in->len);
  printf("- Contenido del archivo:\n");

  printf("\t- ");
  for (int i = 0; i < 4; i++)
  {
    printf("%s, ", data_in->lines[0][i]);
  }
  printf("\n");

  printf("\t- ");
  for (int i = 0; i < 5; i++)
  {
    printf("%s, ", data_in->lines[1][i]);
  }
  printf("\n");
  printf("Liberando memoria...\n");
  char* delay_1[] = {data_in->lines[1][2]};
  char* delay_2[] = {data_in->lines[1][3]};
  char* delay_3[] = {data_in->lines[1][4]};
  input_file_destroy(data_in);
  
  int fabrica = fork();
  if (fabrica == 0){
    //int pid_fabrica = getpid();
    //printf("I'm fabrica and my pid is int: %i\n", pid_fabrica);
    //crear N repartidores cada x seg
    //conectar señal de cambio de semaforo con un handler
    //wait a todos sus repartidores
    exit(0);
  }

  waitpid(fabrica,0,0); //quizás hay que cambiar el NULL
  
  //char pid_fabrica_char = pid_fabrica;
  //printf("I'm fabrica and my pid is char: %i\n", pid_fabrica_char);

  //logica main
  // wait fabrica
  char char_fabrica[5];
  sprintf(char_fabrica, "%i", fabrica);

  int semaforo_1 = fork();
  if (semaforo_1 == 0){
    execlp("./semaforo", "1", *delay_1, char_fabrica, NULL);
    printf("No se ejecutó semaforo 1\n");
    exit(0);
  }
  
  
  int semaforo_2 = fork();
  if (semaforo_2 == 0){
    execlp("./semaforo", "2", *delay_2, char_fabrica, NULL);
    printf("No se ejecutó semaforo 2\n");
    exit(0);
  }
  

  int semaforo_3 = fork(); 
  if (semaforo_3 == 0){
      execlp("./semaforo", "3", *delay_3, char_fabrica, NULL);
      printf("No se ejecutó semaforo 3\n");
      exit(0);
    }
  
  
  printf("Proceso principal ha terminado.\n");
  return 0;
}

