#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>

#include "../file_manager/manager.h"
//#include "../semaforo/main.c"
//#include "../repartidor/main.c"


// impar verde, par rojo (1,2 sem 1 ; 3,4 sem 2; 5,6 sem 3)
int fabrica;
int semaforo_1;
int semaforo_2;
int semaforo_3;
int tope_repartidores = 0;
int arreglo_rep[100]; //ver como hacerlo para que no sea 100 y sean bien los repartidores
int i = 0;
int array_colores[3];

void handle_color_repartidor(int sig, siginfo_t *siginfo, void *context)
{
  int color_received = siginfo->si_value.sival_int;
  printf("Repartidor: Recibi %i\n", color_received);
}

void handle_semaforo(int sig, siginfo_t *siginfo, void *context)
{
  printf("Caught signal %d\n", sig);
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
  printf("Fabrica: Recibi %i\n", color_received);
  //numbers[current_index++] = color_received;
  char *filename = "input.txt";
  InputFile *data_in = read_file(filename);
  char* n_repartidores[] = {data_in->lines[1][1]};
  int int_n_repartidores;
  int_n_repartidores = atoi(*n_repartidores);
  printf("ids de repartidores %i, %i, %i \n ", arreglo_rep[0], arreglo_rep[1], arreglo_rep[2]);
  for (int j = 0; j < int_n_repartidores; j++){
    //printf("arreglo_rep[j]: %i \n", arreglo_rep[j]);
    send_signal_with_int(arreglo_rep[j], color_received);
  }
  // hacer un for con send adentro, para cada pid de repartidor
}
void handle_sigalrm_repartidores(int sig){
    char *filename = "input.txt";
    InputFile *data_in = read_file(filename);
    char* tiempo_entre_rep[] = {data_in->lines[1][0]};
    int int_tiempo_entre_rep;
    int_tiempo_entre_rep = atoi(*tiempo_entre_rep);

    char* n_repartidores[] = {data_in->lines[1][1]};
    int int_n_repartidores;
    int_n_repartidores = atoi(*n_repartidores);


    // Volvemos a conectar la señal
    signal(SIGALRM, handle_sigalrm_repartidores);
    //Crear N repartidores cada x seg
    printf("repartidores %i\n", tope_repartidores);
    int repartidores = fork();
    tope_repartidores += 1;
    // Hacemos exit para que no se haga un for con el fork()
    if (repartidores == 0){
      printf("COLOR a mandar en el excec %i, %i, %i \n",array_colores[0], array_colores[1], array_colores[2]);
      
      char char_color_1[2];
      sprintf(char_color_1, "%i", array_colores[0]);

      char char_color_2[2];
      sprintf(char_color_2, "%i", array_colores[1]);
      
      char char_color_3[2];
      sprintf(char_color_3, "%i", array_colores[2]);

      execlp("./repartidor", char_color_1, char_color_2, char_color_3, NULL);

      exit(0);
    }
    arreglo_rep[i] = repartidores;
    i += 1;
    // Queremos solo los hijos de la fabrica, asi que borramos el resto creado por el loop
    if (int_n_repartidores > tope_repartidores){
      alarm(int_tiempo_entre_rep);
      
    }
    // Ver si funciona
    waitpid(repartidores, 0, 0);

  }


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
  char* tiempo_entre_rep[] = {data_in->lines[1][0]};
  //char* n_repartidores[] = {data_in->lines[1][1]};
  char* delay_1[] = {data_in->lines[1][2]};
  char* delay_2[] = {data_in->lines[1][3]};
  char* delay_3[] = {data_in->lines[1][4]};
// Cambiamos a int el tiempo entre repartidores
  int int_tiempo_entre_rep;
  int_tiempo_entre_rep = atoi(*tiempo_entre_rep);
  printf("tiempo entre rep %i \n", int_tiempo_entre_rep);

  int fabrica = fork();
  if (fabrica == 0){
    printf("estoy en la fabrica %i \n", getpid());
    //Conectar señal de cambio de semaforo con un handler
    connect_sigaction(SIGUSR1, handle_semaforo);
    //Conectamos SIGALRM a handle_sigalrm_repartidores
    signal(SIGALRM, handle_sigalrm_repartidores);
    //Crear N repartidores cada x seg
    alarm(int_tiempo_entre_rep);
    while (!0) {
      pause();
    }
    exit(0);
  } 

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
  waitpid(semaforo_1, 0, 0);
  waitpid(semaforo_2, 0, 0);
  waitpid(semaforo_3, 0, 0);
  waitpid(fabrica,0,0); 
  printf("Proceso principal ha terminado.\n");
  

  input_file_destroy(data_in);
  return 0;
  
}

