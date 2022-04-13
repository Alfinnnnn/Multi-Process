#include <pthread.h>    /* mutex */
#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */

pthread_mutex_t lock;

int main(int argc, char **argv) {
    
    int i,j; //iterator
    
    // input jumlah proses berdasarkan masukkan user
    printf("Masukkan banyaknya proses fork yang diinginkan :\n");
    int jmlh_fork;
    scanf("%d", &jmlh_fork);
    
    // init mutex
	if (pthread_mutex_init(&lock, NULL) != 0) {
		printf("\n mutex init gagal\n");
		return 1;
	}

	// destroy mutex
	pthread_mutex_destroy(&lock);

    int pid;
    int fd[2];  // pipe

    // create pipe
    if (pipe(fd) < 0) {
        exit(1); //error
    }
    
    // looping jumlah proses
    for (i = 0; i < jmlh_fork; i++)
    {
        pid = fork();
    }

    switch (pid) {
    case 0: 
        printf("Proses Produser \n");
        int angka_produsen;

        /* tutup bagian input dari pipe */
        close(fd[0]);

        srand(getpid());  //melakukan random sesuai PID

        /* tulis ke pipe */
        for(j=0; j<jmlh_fork; j++){
	        pthread_mutex_lock(&lock); // lock
	   
            angka_produsen = rand() % 100;  // tulis data angka random ke pipe 
            write(fd[1], &angka_produsen, sizeof(angka_produsen)); // tulis ke pipe
            printf("- Produsen ke %d dengan (PID = %d) menghasilkan angka : %d \n", j+1, getpid(), angka_produsen);
            
            pthread_mutex_unlock(&lock); // unlock
            sleep(2);
        }
        break;

    default: 
        printf("Proses Consumer \n");
        int total_sum = 0;
        int angka_konsumen;
        
        /* tutup bagian output dari pipe */
        close(fd[1]);
        
        /* baca yang ditulis child dari pipe */
        for(j=0; j<jmlh_fork; j++){
            pthread_mutex_lock(&lock); // lock
            
            read(fd[0], &angka_konsumen, sizeof(angka_konsumen)); //membaca pipe
            printf("--- Konsumen ke %d dengan (PID = %d) mengambil angka : %d \n", j+1, getpid(), angka_konsumen);
            total_sum = total_sum + angka_konsumen; // jumlahkan angka
            
            pthread_mutex_unlock(&lock); // unlock
            sleep(2);
        }
        // tampilkan hasil penjumlahan konsumen
        printf("Total konsumsi dengan (PID = %d) adalah %d \n", getpid(), total_sum);
        break;

    case -1: 
        /* error */
        perror("fork");
        exit(1);
    }
    
    exit(0);
}