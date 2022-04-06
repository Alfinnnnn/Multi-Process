#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <string.h>
#include <time.h>       // use time.h header file to use time  

#define MSGSIZE 100

int main(int argc, char **argv) {

    //variabel int for randoming
    int bebas = 0;
    time_t t1; // declare time variable  

    char inbuf[MSGSIZE]; //buffer
    int pid;
    int fd[2]; //pipe
  
    //create pipe
    if (pipe(fd) < 0) {
        /* error */
        exit(1);
    }
  
    int input1, input2;
    printf("Masukkan jumlah proses kelompok 1 (child): \n");
    scanf("%d", &input1);
    printf("Masukkan jumlah proses kelompok 2 (parent): \n");
    scanf("%d", &input2);
  
    int num[input1];
    printf("===============\n");
    printf("Memulai proses \n");
    printf("===============\n");
  
        switch (pid = fork()) {
        case 0:         
                /* fork returns 0 ke proses anak */
                printf("Proses anak \n");
  
                srand ((unsigned) time (&t1));   // pass the srand() parameter  
          
                int arrIntChild[MSGSIZE];
                for(int i=0;i<input1;i++){

                    num[i] = rand() %50 + 1;
                    arrIntChild[i] = num[i];
                    printf("Proses anak sedang menulis : %d\n", arrIntChild[i]);
                }
                
                /* tutup bagian input dari pipe */
                close(fd[0]);
          
                // tulis ke pipe
                write(fd[1], arrIntChild, sizeof(arrIntChild));
    
                break;
        default:        
                /* fork returns pid ke proses ortu */
                printf("Proses ortu\n");
                int arrIntParent[MSGSIZE];
                // baca yang ditulis child dari pipe
                
                /* tutup bagian output dari pipe */
                close(fd[1]);
    
                /* menampilkan proses random */
                read(fd[0], arrIntParent, sizeof(arrIntParent));
                for (int i = 0; i<input2; i++) {
                    printf("- Proses child ke %d mengambil sedang menulis : %d \n", i+1, arrIntParent[i]);
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