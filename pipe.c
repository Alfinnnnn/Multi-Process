#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <string.h>
#include <time.h> // use time.h header file to use time  

#define MSGSIZE 3

int main(int argc, char **argv) {

    //variabel int for randoming
    int num1, num2, num3;
  
    time_t t1;                        // declare time variable  
    srand ( (unsigned) time (&t1));   // pass the srand() parameter  
  
    num1 = 0;
    num1 = rand() % 100 + 1;
    num2 = 0;
    num2 = rand() % 100 + 1;
    num3 = 0;
    num3 = rand() % 100 + 1;
    
    char inbuf[MSGSIZE]; //buffer
    int pid;
    int fd[2]; //pipe
  
    //create pipe
    if (pipe(fd) < 0) {
        /* error */
        exit(1);
    }
  
    printf("===============\n");
    printf("Memulai proses \n");
    printf("===============\n");
    switch (pid = fork()) {
    case 0:         
            /* fork returns 0 ke proses anak */
            printf("Proses anak \n");
      
            // randoming int
            int arrIntChild[MSGSIZE] = {num1,num2,num3};

            /* tutup bagian input dari pipe */
            close(fd[0]);
      
            // tulis ke pipe
            write(fd[1], arrIntChild, sizeof(arrIntChild));

            break;
    default:        
            /* fork returns pid ke proses ortu */
            printf("Proses ortu\n\n");
            int arrIntParent[MSGSIZE];
            int bebas = 0;
      
            // baca yang ditulis child dari pipe
            
            /* tutup bagian output dari pipe */
            close(fd[1]);

            /* menampilkan proses random */
            read(fd[0], arrIntParent, sizeof(arrIntParent));
            for (int i = 0; i<MSGSIZE; i++) {
                printf("- Proses child mengambil angka random : %d \n", arrIntParent[i]);
                bebas += arrIntParent[i];
            }
            printf("\nPenjumlahan = %d", bebas);

            break;
    case -1:        
            /* error */
            perror("fork");
            exit(1);
    }
    exit(0);
}