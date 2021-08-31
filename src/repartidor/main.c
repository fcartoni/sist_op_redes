#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "../file_manager/manager.h"

void handle_color_repartidor(int sig, siginfo_t *siginfo, void *context)
{
  printf("entre al handle del cambio de color de los repartidores\n");
  int color_received = siginfo->si_value.sival_int;
  printf("Repartidor %i: Recibi %i\n", getpid(), color_received);
}

int main(int argc, char const *argv[])
{
  connect_sigaction(SIGUSR1, handle_color_repartidor);
  printf("I'm the REPARTIDOR process and my PID is: %i\n", getpid());
  while (!0);
}
