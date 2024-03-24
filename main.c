
/*
ftok //convert a pathname and a project identifier to a System V
shmget //allocates a System V shared memory segment

shmat //XSI shared memory attach operation
msgget //get a System V message queue identifier
semget //get a System V semaphore set identifier

 */



#include <sys/types.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdio.h>
//#include "semun.h"

#include <mqueue.h>

int	main(int argc, char **argv){
	key_t key = ftok("/tmp", 'a');
	if (key == -1)
		perror("FTOK");

	int perm = S_IRUSR | S_IWUSR;
	int nbr_sem = 1;

	printf("%u\n", key);

	int sem_id = semget(key, nbr_sem, IPC_CREAT | IPC_EXCL | perm);

	if (sem_id != -1){
		union semun arg;

		arg.val = 1;
		semctl(sem_id, 0, SETVAL, arg);
		printf("SETVAL\n");

		struct sembuf sop;

		sop.sem_num = 0; // semophore
		sop.sem_op = 0; //wait to equal 0
		sop.sem_flg = 0;

		semop(sem_id, &sop, 1);

	}
	else{
		if (errno != EEXIST)
			perror("semget1");
		sem_id = semget(key, nbr_sem, perm);
		if (sem_id == -1)
			perror("semget2");

		struct sembuf sop;

		sop.sem_num = 0; // semophore
		sop.sem_op = -1; //add 1
		sop.sem_flg = 0;

		semop(sem_id, &sop, 1);
	}


	return (0);
}