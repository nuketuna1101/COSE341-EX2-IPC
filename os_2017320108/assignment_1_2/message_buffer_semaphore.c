#include "message_buffer_semaphore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

int shmid;
void *memory_segment=NULL;

int semid;
union semun sem_union;

void init_sem() {
    /*---------------------------------------*/
    /* TODO 1 : init semaphore               */

    if ((semid = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
        // try as a client
        if ((semid = semget(SEM_KEY, 0, 0)) == -1) {
            printf("semget failed :: ERROR!\n");
            return -1;
        }
    }
    else {
        sem_union.val = 1;
        semctl(semid, 0, SETVAL, sem_union);
    }

    /* TODO 1 : END                          */
    /*---------------------------------------*/
    printf("init semid : %d\n", semid);
}

void destroy_sem() {
    /*---------------------------------------*/
    /* TODO 2 : destroy semaphore            */

    if (semctl(semid, 0, IPC_RMID) == -1) {
        printf("semget failed :: ABNORMAL TERMINATION!\n");
        return -1;
    }

    /* TODO 2 : END                          */
    /*---------------------------------------*/
    printf("destroy semid : %d\n", semid);
}

void s_wait() {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = SEM_UNDO;

    if (semop(semid, &buf, 1) == -1) {
        printf("<s_wait> semop error!\n");
	return;
    }
}

void s_quit() {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;

    if (semop(semid, &buf, 1) == -1) {
        printf("<s_quit> semop error!\n");
	return;
    }
}


/*---------------------------------------------*/
/* TODO 3 : use s_quit() and s_wait()          */
/* (1) clone from assignment 1-1               */
/* (2) use semaphore                           */
/* (3) report explanation (ex. 2020020000.pdf) */

int init_buffer(MessageBuffer** buffer) {
    if ((shmid = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0666)) == -1) {
        printf("shmget failed :: NOT ABLE TO GET SHARED MEMORY\n");
        return -1;
    }
    if ((memory_segment = shmat(shmid, NULL, 0)) == (void*)-1) {
        printf("shmat failed :: NOT ABLE TO ATTACH SHARED MEMORY\n");
        return -1;
    }

    *buffer = (MessageBuffer*)memory_segment;
    (*buffer)->is_empty = 1;

    printf("init buffer\n");
    return 0;
}


int attach_buffer(MessageBuffer** buffer) {
    if ((shmid = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0666)) == -1) {
        printf("shmget failed :: NOT ABLE TO GET SHARED MEMORY\n");
        return -1;
    }
    if ((memory_segment = shmat(shmid, NULL, 0)) == (void*)-1) {
        printf("shmat failed :: NOT ABLE TO ATTACH SHARED MEMORY\n");
        return -1;
    }

    *buffer = (MessageBuffer*)memory_segment;
    (*buffer)->is_empty = 0;

    printf("attach buffer\n");
    printf("\n");
    return 0;
}


int detach_buffer() {
    if (shmdt(memory_segment) == -1) {
        printf("shmdt error!\n\n");
        return -1;
    }

    printf("detach buffer\n\n");
    return 0;
}


int destroy_buffer() {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        printf("shmctl error!\n\n");
        return -1;
    }

    printf("destroy shared_memory\n\n");
    return 0;
}


int produce(MessageBuffer** buffer, int sender_id, int data, int account_id) {
    s_wait();

    (*buffer)->messages[(*buffer)->account_id].sender_id = sender_id;
    (*buffer)->messages[(*buffer)->account_id].data = data;

    printf("produce message\n");

    s_quit();
    return 0;
}


int consume(MessageBuffer** buffer, Message** message) {
    s_wait();

    if ((*buffer)->is_empty) {
        s_quit();
        return -1;
    }

    *message = &(*buffer)->messages[(*buffer)->account_id];

    if ((*message).data > (*buffer)->messages[(*buffer)->account_id].data) {
        printf("ERROR :: NOT SUFFICIENT BALANCE\n");
        s_quit();
        return -1;
    }

    (*message).data += (*buffer)->messages[(*buffer)->account_id].data;

    s_quit();
    return 0;
}

/* TODO 3 : END                                */
/*---------------------------------------------*/
