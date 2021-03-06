#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>

#include "../file_manager/manager.h"


// impar verde, par rojo (1,2 sem 1 ; 3,4 sem 2; 5,6 sem 3)
int fabrica;
int fabrica_pid;
int semaforo_1;
int semaforo_2;
int semaforo_3;
int tope_repartidores = 0;
int* arreglo_rep; 
int i = 0;
int array_colores[3] = {1,3,5};
int array_pid_sem[3]; 
int rep_finalizados = 0;
int procesos = 0;
int fab_termino = 0;
int indice_rep = 0;
char* input_name;
InputFile *data_in;



void handle_abort_sem(int sig, siginfo_t *siginfo, void *context)
{
  int senal = siginfo->si_value.sival_int;
  if (senal == 1){
    kill(array_pid_sem[0], SIGABRT);
    kill(array_pid_sem[1], SIGABRT);
    kill(array_pid_sem[2], SIGABRT);
  }
}

void handle_semaforo(int sig, siginfo_t *siginfo, void *context)
{
  int color_received = siginfo->si_value.sival_int;
  if (color_received == 1 || color_received == 2){
    array_colores[0] = color_received;
  }
  if (color_received == 3 || color_received == 4){
    array_colores[1] = color_received;
  }
  if (color_received == 5 || color_received == 6){
    array_colores[2] = color_received;
  }
  char* n_repartidores[] = {data_in->lines[1][1]};
  int int_n_repartidores;
  int_n_repartidores = atoi(*n_repartidores);
  
  for (int j = 0; j < int_n_repartidores; j++){
    send_signal_with_int(arreglo_rep[j], color_received);
  }
}
void handle_sigalrm_repartidores(int sig){
    char* tiempo_entre_rep[] = {data_in->lines[1][0]};
    int int_tiempo_entre_rep;
    int_tiempo_entre_rep = atoi(*tiempo_entre_rep);

    char* n_repartidores[] = {data_in->lines[1][1]};
    int int_n_repartidores;
    int_n_repartidores = atoi(*n_repartidores);

    // Volvemos a conectar la señal
    signal(SIGALRM, handle_sigalrm_repartidores);
    //Crear N repartidores cada x seg
    int repartidores = fork();
    tope_repartidores += 1;
    indice_rep += 1;
    if (repartidores == 0){
      
      char char_color_1[2];
      sprintf(char_color_1, "%i", array_colores[0]);

      char char_color_2[2];
     
      sprintf(char_color_2, "%i", array_colores[1]);
      
      char char_color_3[2];
      sprintf(char_color_3, "%i", array_colores[2]);

      char char_indice_rep[2];
      sprintf(char_indice_rep, "%i", indice_rep);
      execlp("./repartidor", char_color_1, char_color_2, char_color_3, char_indice_rep, input_name, NULL);

      exit(0);
    }

    arreglo_rep[i] = repartidores;
    i += 1;
    // Queremos solo los hijos de la fabrica, asi que borramos el resto creado por el loop
    if (int_n_repartidores > tope_repartidores){
      alarm(int_tiempo_entre_rep);
      
    }
  }

void handle_sigint_main(int sig)
{
  printf("Gracefully finishing\n");

  // Hacer sigabrt a fab y sem
  kill(fabrica_pid, SIGABRT); 
  kill(array_pid_sem[0], SIGABRT);
  kill(array_pid_sem[1], SIGABRT);
  kill(array_pid_sem[2], SIGABRT);

  free(arreglo_rep);
  input_file_destroy(data_in);
  // Terminamos el programa con exit code 0
  exit(0);
}

void handle_sigabrt_fabrica(int sig)
{ 
  char* numero_reps[] = {data_in->lines[1][1]};
  int int_numero_reps;
  int_numero_reps = atoi(*numero_reps);
  for (int i = 0; i < int_numero_reps; i++){
    kill(arreglo_rep[i], SIGABRT);
  }
  free(arreglo_rep); //Libero la memoria del arreglo de repartidores
  input_file_destroy(data_in); //Liberando memoria
  exit(0);
}


int main(int argc, char *argv[])
{
  input_name = argv[1];
  char *filename = input_name;
  data_in = read_file(filename);
  signal(SIGINT, handle_sigint_main);
  
  printf("I'm the DCCUBER process and my PID is: %i\n", getpid());

  

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
  char* tiempo_entre_rep[] = {data_in->lines[1][0]};
  char* n_repartidores[] = {data_in->lines[1][1]};
  char* delay_1[] = {data_in->lines[1][2]};
  char* delay_2[] = {data_in->lines[1][3]};
  char* delay_3[] = {data_in->lines[1][4]};
// Cambiamos a int el tiempo entre repartidores
  int int_tiempo_entre_rep;
  int_tiempo_entre_rep = atoi(*tiempo_entre_rep);

  int int_n_rep;
  int_n_rep = atoi(*n_repartidores);
  arreglo_rep = calloc(int_n_rep, sizeof(int));

  int fabrica = fork();
  if (fabrica == 0){
    signal(SIGINT, SIG_IGN);
    printf("estoy en la fabrica %i \n", getpid());
    //Conectamos sigabrt mandado por el proceso ppal con un handler
    signal(SIGABRT, handle_sigabrt_fabrica);
    //Conectar señal de cambio de semaforo con un handler
    connect_sigaction(SIGUSR1, handle_semaforo);
    //Conectamos SIGALRM a handle_sigalrm_repartidores
    signal(SIGALRM, handle_sigalrm_repartidores);
    //Crear N repartidores cada x seg
    alarm(int_tiempo_entre_rep);
    
    while (rep_finalizados < int_n_rep){
      int tiene_reps = wait(NULL);
      if (tiene_reps > 0){
        rep_finalizados += 1;
      }
    }
    
    free(arreglo_rep); //Libero la memoria del arreglo de repartidores
    input_file_destroy(data_in);
    send_signal_with_int(getppid(), 1);
    
    exit(0);
  } 

  char char_fabrica[5];
  sprintf(char_fabrica, "%i", fabrica);

  int semaforo_1 = fork();
  if (semaforo_1 == 0){
    execlp("./semaforo", "1", *delay_1, char_fabrica, NULL);
    exit(0);
  }
  
  int semaforo_2 = fork();
  if (semaforo_2 == 0){
    execlp("./semaforo", "2", *delay_2, char_fabrica, NULL);
    exit(0);
  }

  int semaforo_3 = fork(); 
  if (semaforo_3 == 0){
      execlp("./semaforo", "3", *delay_3, char_fabrica, NULL);
      exit(0);
    }

  array_pid_sem[0] = semaforo_1;
  array_pid_sem[1] = semaforo_2;
  array_pid_sem[2] = semaforo_3;
  fabrica_pid = fabrica;

  connect_sigaction(SIGUSR1, handle_abort_sem);
  
  while (procesos < 4){
      int n_proc = wait(NULL);
      if (n_proc > 0){
        procesos += 1;
      }
    }
  
  printf("Proceso principal ha terminado.\n");
  

  input_file_destroy(data_in);
  free(arreglo_rep);
  return 0;
  
} 

