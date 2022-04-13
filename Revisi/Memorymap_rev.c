#include <sys/mman.h>   /* mmap() */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /* wait */
#include <errno.h>      /* errno */
#include <pthread.h>    /* mutex */

pthread_mutex_t lock;

int main(int argc, char **argv) {
  
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

    // inisialisasi 
    int pid;    
    int size = jmlh_fork * sizeof(int);
    
    // buat memory map
    void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *shared = addr;
    
    // inisialisasi iterator memory map
    int *idx = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *idx = 0;

    // Looping untuk forking process
    for (int i = 0; i<jmlh_fork; i++)
    {
        pid = fork();
    }

    switch (pid) {
    case 0:         
        printf("Proses Producer \n");
        
        srand(getpid()); //angka random berdasarkan pid

        /* lakukan perulangan elemen random sesuai banyaknya inputan user */
        for (int j = 0; j < jmlh_fork; j++)
        {
            pthread_mutex_lock(&lock); // lock
            
            shared[*idx] = rand() % 100; // tulis data angka random ke map
            printf("- Produsen ke %d dengan (PID  = %d) menghasilkan angka : %d\n", j+1, getpid(), shared[*idx]);
            *idx += 1; //buffer produsen bertambah
            
            pthread_mutex_unlock(&lock); // unlock
            sleep(2);
        }
        break;
        
    default:        
        printf("Proses Consumer \n");
        int pidWait, status;
        int total_sum = 0;

        // menunggu proses produsen selesai 
        while (pidWait = wait(&status)) {   
          
            // jika produsen telah selsai
            if (pidWait == pid)  

                //cetak isi memory map dan lakukan penjumlahan hasil produsen sebanyak proses inputan
                for (int k = 0; k<jmlh_fork; k++) {
                    pthread_mutex_lock(&lock); // lock
                    
                    printf("--- Konsumen ke %d dengan (PID = %d) mengambil angka : %d \n", k+1, getpid(), shared[*idx-1]);
                    total_sum = total_sum + shared[*idx-1]; // jumlahkan angka yang telah di random dari produsen
                    *idx -= 1; //buffer konsumen berkurang
                    
                    pthread_mutex_unlock(&lock); // unlock
                    sleep(2);
                }
                break; //keluar dari loop wait

            if ((pidWait == -1) && (errno != EINTR)) {
                /* ada error */
                perror("waitpid");
                exit(1);
            }
        }
        // tampilkan hasil penjumlahan
        printf("Total konsumsi dengan (PID = %d) adalah %d \n", getpid(), total_sum);
        break;

    case -1:        
        /* error */
        perror("fork");
        exit(1);
    }
    exit(0);

    return 0;
}