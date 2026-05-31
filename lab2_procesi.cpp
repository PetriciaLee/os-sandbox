/*
 * Zadatak 2d: Međusobno isključivanje Lamportovim algoritmom (procesi)
 * Kompajliranje: gcc -o lamport_procesi lamport_procesi.c
 * Pokretanje:    ./lamport_procesi 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>      /* fork(), getpid() */
#include <sys/wait.h>    /* wait() */
#include <sys/shm.h>     /* shmget(), shmat(), shmdt(), shmctl() */
#include <sys/ipc.h>     /* IPC_PRIVATE, IPC_RMID */


typedef struct {
    int n;          
    int *TRAZIM;    
    int *BROJ;       
} Zajednicko;

Zajednicko *Z = NULL;

int maks(int n) {
    int m = 0;
    for (int j = 0; j < n; j++)
        if (Z->BROJ[j] > m)
            m = Z->BROJ[j];
    return m;
}


void udi_u_kriticni_odsjecak(int i) {
    int j;
    int n = Z->n;

    Z->TRAZIM[i] = 1;
    Z->BROJ[i]   = maks(n) + 1;
    Z->TRAZIM[i] = 0;

    for (j = 0; j < n; j++) {
        if (j == i) continue;   

        while (Z->TRAZIM[j] != 0) ;  

        while (
            Z->BROJ[j] != 0 &&
            (
                Z->BROJ[j] < Z->BROJ[i] ||
                (Z->BROJ[j] == Z->BROJ[i] && j < i)
            )
        );  
    }
}

void izidi_iz_kriticnog_odsjecka(int i) {
    Z->BROJ[i] = 0;
}

void proces(int i) {
    int k, m;

    for (k = 1; k <= 5; k++) {

        udi_u_kriticni_odsjecak(i);

        for (m = 1; m <= 5; m++) {
            printf("Dretva: %d, K.O. br: %d (%d/5)\n", i + 1, k, m);
            fflush(stdout);  
            
        }
        
        izidi_iz_kriticnog_odsjecka(i);
    }
    exit(0);
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Upotreba: %s <broj_procesa>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    if (n < 2) {
        fprintf(stderr, "Broj procesa mora biti >= 2.\n");
        return 1;
    }

    size_t velicina = sizeof(Zajednicko) + 2 * n * sizeof(int);

    int shmid = shmget(IPC_PRIVATE, velicina, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        return 1;
    }

    void *blok = shmat(shmid, NULL, 0);
    if (blok == (void *)-1) {
        perror("shmat");
        return 1;
    }
    
    Z         = (Zajednicko *) blok;
    Z->n      = n;
    Z->TRAZIM = (int *)((char *)blok + sizeof(Zajednicko));
    Z->BROJ   = Z->TRAZIM + n;

    for (int j = 0; j < n; j++) {
        Z->TRAZIM[j] = 0;
        Z->BROJ[j]   = 0;
    }

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            return 1;
        }
        else if (pid == 0) {
        
            proces(i);   
        }
    }

    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    shmdt(blok);
    shmctl(shmid, IPC_RMID, NULL);

    printf("Svi procesi su završili.\n");
    return 0;
}