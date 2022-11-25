#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

int sem_id;

static void semafor_p(int);
static void semafor_v(int);

int main(int argc, char *argv[])
{
  FILE *fp = NULL;
  char buff[50];
  fp = fopen("wynik.txt", "w");

  if (fp == NULL)
  {
    perror("file open error");
    exit(EXIT_FAILURE);
  }

  sem_id = atoi(argv[1]);

  semafor_p(2);

  sprintf(buff, "Sekcja t11 procesu o PID: %d \n", getpid());
  printf(buff);
  fprintf(fp, buff);
  fflush(fp);
  sleep(1);

  sprintf(buff, "Sekcja t12 procesu o PID: %d \n", getpid());
  printf(buff);
  fprintf(fp, buff);
  fflush(fp);
  sleep(1);

  semafor_v(3);

  fclose(fp);
  return 0;
}

static void semafor_p(int i)
{
  struct sembuf bufor_sem;
  bufor_sem.sem_num = i;
  bufor_sem.sem_op = -1;
  bufor_sem.sem_flg = 0;

  int zmien_sem = semop(sem_id, &bufor_sem, 1);
  if (zmien_sem == -1)
  {
    if (errno == EINTR)
    {
      semafor_p(i);
    }
    else
    {
      perror("Nie moglem zamknac semafora.\n");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    printf("Semafor zostal zamkniety.\n");
  }
}

static void semafor_v(int i)
{
  struct sembuf bufor_sem;
  bufor_sem.sem_num = i;
  bufor_sem.sem_op = 1;
  bufor_sem.sem_flg = SEM_UNDO;
  int zmien_sem = semop(sem_id, &bufor_sem, 1);
  if (zmien_sem == -1)
  {
    perror("Nie moglem otworzyc semafora.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("Semafor zostal otwarty.\n");
  }
}
