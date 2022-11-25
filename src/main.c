#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>

int sem_create(key_t key, int n);
void sem_remove(int id);
void sem_set_default(int id);

int main(int argc, char *argv[])
{
  key_t key = 2115;
  int sem_count = 5;
  int status, child_pid;
  char *parsed_sem_id = NULL;

  char path[] = "./";
  char programs[][3] = {"p1", "p2", "p3"};

  int sem_id = sem_create(key, sem_count);
  sem_set_default(sem_id);

  int len = snprintf(NULL, 0, "%d", sem_id);
  parsed_sem_id = malloc((len + 1) * sizeof(char));

  int p_sem_id = snprintf(parsed_sem_id, len + 1, "%d", sem_id);
  parsed_sem_id[len] = '\0';

  if (p_sem_id == -1)
  {
    perror("parsing sem id failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < 3; i++)
  {
    switch (fork())
    {
    case -1:
      perror("creating child process error\n");
      exit(EXIT_FAILURE);
      break;
    case 0:
      child_pid = execl(strcat(path, programs[i]), programs[i], parsed_sem_id, (char *)NULL);

      if (child_pid == -1)
      {
        perror("exec child program error");
        exit(EXIT_FAILURE);
      }
      break;
    default:
      break;
    }
  }

  for (int i = 0; i < 3; i++)
  {
    int child_pid = wait(&status);

    if (child_pid == -1)
    {
      perror("wait for child error\n");
      exit(EXIT_FAILURE);
    }

    printf("Proces o PID: %d zakonczony z statusem %d\n", child_pid, status);
  }

  free(parsed_sem_id);
  parsed_sem_id = NULL;
  sem_remove(sem_id);

  return 0;
}

void sem_set_default(int id)
{
  int ustaw_sem = semctl(id, 0, SETVAL, 0); // sem_num is ignored
  if (ustaw_sem == -1)
  {
    perror("Nie mozna ustawic semafora.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("Semafor zostal ustawiony.\n");
  }
}

int sem_create(key_t key, int n)
{
  int sem_id = semget(key, n, 0600 | IPC_CREAT);

  if (sem_id == -1)
  {
    perror("Nie moglem utworzyc nowego semafora.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("Semafor zostal utworzony sem_id: %d\n", sem_id);
  }

  return sem_id;
}

void sem_remove(int id)
{
  int sem = semctl(id, 0, IPC_RMID); // sem_num is ignored
  if (sem == -1)
  {
    printf("Nie mozna usunac semafora.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("Semafor zostal usuniety status: %d\n", sem);
  }
}