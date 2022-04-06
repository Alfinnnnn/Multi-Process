#include <sys/mman.h>   /* mmap()  */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>	    /* errno */
#include <time.h>       // use time.h header file to use time  

#define MSGSIZE 100

int main(int argc, char **argv) {
        int pid;
        int bebas= 0;
        printf("mulai \n");

        int size = MSGSIZE * sizeof(int);
        //buat memory map
        void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        int input1, input2;
        printf("Masukkan jumlah proses kelompok 1:\n");
        scanf("%d", &input1);
        printf("Masukkan jumlah proses kelompok 2:\n");
        scanf("%d", &input2);
        int num[input1];
        printf("\nMapped at %p\n\n", addr);
        int *shared = addr;
        switch (pid = fork()) {
        case 0:         /*  proses anak */
                printf("Proses anak \n");
                //isi data di memory map
                time_t t1; // declare time variable
                
                srand ((unsigned) time (&t1));   // pass the srand() parameter  
                for(int i=0;i<input1;i++){
                    shared[i] = rand() % 50 + 1;
                    printf("Proses anak sedang menulis : %d\n", shared[i]);
                }
                
                break;
        default:        /*  ortu */
                printf("Proses ortu\n");
                int pidWait,status;
                /* wait sampai child selesai */
                while (pidWait = wait(&status)) {   
                    if (pidWait == pid)  /* child sukses selesai*/
                        //cetak isi memory map
                        for (int i = 0; i<input2; i++) {
                          bebas += shared[i];
                          printf("(Ini Parent) Proses child menulis: %d \n", shared[i]);
                        }
                        printf("\nSum = %d\n", bebas);
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