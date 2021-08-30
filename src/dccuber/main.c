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
  input_file_destroy(data_in);
  
  int fabrica = fork();
  if (fabrica == 0){
    //crear N repartidores cada x seg
    //conectar señal de cambio de semaforo con un handler
    //wait a todos sus repartidores
    //exit
  }
  int pid_fabrica = getpid();
  printf("I'm fabrica and my pid is int: %i\n", pid_fabrica);
  char pid_fabrica_char = pid_fabrica;
  printf("I'm fabrica and my pid is char: %i\n", pid_fabrica_char);

  //logica main
  // wait fabrica

  int semaforo_1 = fork();
  if (semaforo_1 == 0){
    execlp("./semaforo", "1", "7", "3", NULL);
    printf("No se ejecutó semaforo 1\n");
    exit(0);
  }
  
  int semaforo_2 = fork();
  if (semaforo_2 == 0){
    execlp("./semaforo", "2", "8", "3", NULL);
    printf("No se ejecutó semaforo 1\n");
    exit(0);
  }

  int semaforo_3 = fork(); 
  if (semaforo_3 == 0){
      execlp("./semaforo", "3", "9", "3", NULL);
      printf("No se ejecutó semaforo 1\n");
      exit(0);
    }
  
 

  wait(NULL); //quizás hay que cambiar el NULL
  printf("Proceso principal ha terminado.\n");
  return 0;
}

