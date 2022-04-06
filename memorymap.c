#include <sys/mman.h>   /* mmap()  */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>	    /* errno */

#define MSGSIZE 3

int main(int argc, char **argv) {
        int pid;
        int bebas= 0;
        printf("mulai \n");

        int size = MSGSIZE * sizeof(int);
        //buat memory map
        void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        printf("Mapped at %p\n", addr);
        int *shared = addr;
        switch (pid = fork()) {
        case 0:         /*  proses anak */
                printf("Proses anak \n");
                //isi data di memory map
                 time_t t1; // declare time variable
                  srand((unsigned) time(&t1));
                shared[0] = rand() % 50 + 1;
                shared[1] = rand() % 50 + 1;
                shared[2] = rand() % 50 + 1;
                break;
        default:        /*  ortu */
                printf("Proses ortu\n");
                int pidWait,status;
                /* wait sampai child selesai */
                while (pidWait = wait(&status)) {   
                    if (pidWait == pid)  /* child sukses selesai*/
                        //cetak isi memory map
                        for (int i = 0; i<MSGSIZE; i++) {
                            bebas += shared[i];
                          printf("ini parent, proses child menulis: %d \n", shared[i]);
                          printf("sum = %d\n", bebas);
                        }
                        break; //keluar dari loop wait
                    if ((pidWait == -1) && (errno != EINTR)) {
                        /* ada error*/
                        perror("waitpid");
                        exit(1);
                    }
                }
                break;
        case -1:        /* error */
                perror("fork");
                exit(1);
        }
        exit(0);
}
