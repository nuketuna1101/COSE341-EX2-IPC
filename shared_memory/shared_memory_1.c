#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define KEY 1234

int main(){
    int shmid;
    int *num;
    void *memory_segment=NULL;
    // shared memory 생성, 아이디 생성 - 실패 시 오류 리턴
    if ((shmid = shmget(KEY, sizeof(int), IPC_CREAT|0666)) == -1) return -1;

    printf("shmid : %d\n", shmid);
    // shared memory를 가리키도록 attach
    if ((memory_segment = shmat(shmid, NULL, 0)) == (void*)-1) return -1;
    //
    num = (int*)memory_segment;
    (*num)++;
    printf("num : %d\n", (*num));

    return 0;
}