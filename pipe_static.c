#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <string.h>

#define MSGSIZE 5

int main(int argc, char **argv) {

    //variabel int for randoming
    int num1, num2, num3, num4, num5;
    char inbuf[MSGSIZE]; //buffer
    int pid;
    int fd[2]; //pipe
  
    //create pipe
    if (pipe(fd) < 0) 
    {
        /* error */
        exit(1);
    }
  
    printf("===============\n");
    printf("Memulai proses \n");
    printf("===============\n");

    switch (pid = fork()) 
    {
          case 0:
                /* fork returns 0 ke proses anak */
                printf("Proses anak\n");
          
                // randoming int
                srand (getpid());   // pass the srand() parameter  
                num1 = 0;
                num1 = rand() % 100 + 1;
                num2 = 0;
                num2 = rand() % 100 + 1;
                num3 = 0;
                num3 = rand() % 100 + 1;
                num4 = 0;
                num4 = rand() % 100 + 1;
                num5 = 0;
                num5 = rand() % 100 + 1;
          
                int arrIntChild[MSGSIZE] = {num1,num2,num3,num4,num5};
                
                printf("(Ini Child) Proses anak sedang menulis : %d\n", num1);
                printf("(Ini Child) Proses anak sedang menulis : %d\n", num2);
                printf("(Ini Child) Proses anak sedang menulis : %d\n", num3);
                printf("(Ini Child) Proses anak sedang menulis : %d\n", num4);
                printf("(Ini Child) Proses anak sedang menulis : %d\n", num5);
            
                /* tutup bagian input dari pipe */
                close(fd[0]);
          
                // tulis ke pipe
                write(fd[1], arrIntChild, sizeof(arrIntChild));

                break;
          default:        
                /* fork returns pid ke proses ortu */
                printf("Proses ortu\n");
                int arrIntParent[MSGSIZE];
                int sum = 0;
          
                // baca yang ditulis child dari pipe
                
                /* tutup bagian output dari pipe */
                close(fd[1]);
    
                /* menampilkan proses random */
                read(fd[0], arrIntParent, sizeof(arrIntParent));
                for (int i = 0; i<MSGSIZE; i++) 
                {
                    printf("- (Ini Parent) Proses anak ke %d sedang menulis : %d \n", i+1, arrIntParent[i]);
                    sum += arrIntParent[i];
                }
                printf("Sum = %d\n", sum);
                break;
          
        case -1:        
                /* error */
                perror("fork");
                exit(1);
    }
    exit(0);
}