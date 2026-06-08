#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <signal.h>

#define PUN    0  
#define PISI   1  
#define PRAZAN 2 

struct dijeljeno {
    int ULAZ;
    int IZLAZ;
    int UKUPNO;
    int M[5];
};

int ShmId, SemId;
struct dijeljeno *Zajednicko = NULL;

struct sembuf smanji = {0, -1, 0};
struct sembuf povecaj = {0, 1, 0};

void oslobodi_resurse(int sig) {
    if (Zajednicko != NULL && Zajednicko != (void *)-1) {
        shmdt(Zajednicko);
    }
    if (ShmId >= 0) {
        shmctl(ShmId, IPC_RMID, NULL);
    }
    if (SemId >= 0) {
        semctl(SemId, 0, IPC_RMID, 0);
    }
    if (sig != 0) {
        exit(1);
    }
}
void Proizvodjac(int id, int n) {
    for (int i = 0; i < n; i++) {
        int slucajni_broj = (rand() % 1000) + 1;

        smanji.sem_num = PUN; 
        semop(SemId, &smanji, 1);
        
        smanji.sem_num = PISI; 
        semop(SemId, &smanji, 1);

        Zajednicko->M[Zajednicko->ULAZ] = slucajni_broj;
        printf("Proizvodac %d salje \"%d\"\n", id, slucajni_broj);
        fflush(stdout);
        
        Zajednicko->ULAZ = (Zajednicko->ULAZ + 1) % 5;
        
        usleep(100000); 

        povecaj.sem_num = PISI; 
        semop(SemId, &povecaj, 1);
        
        povecaj.sem_num = PRAZAN; 
        semop(SemId, &povecaj, 1);
    }

    printf("Proizvodac %d zavrsio sa slanjem\n", id);
    fflush(stdout);
    exit(0);
}
void Potrosac() {
    int zbroj = 0;
    int ukupno = Zajednicko->UKUPNO;

    for (int i = 0; i < ukupno; i++) {
        smanji.sem_num = PRAZAN; 
        semop(SemId, &smanji, 1);

        int uzeti_broj = Zajednicko->M[Zajednicko->IZLAZ];
        zbroj += uzeti_broj;
        printf("Potrosac prima %d\n", uzeti_broj);
        fflush(stdout);

        Zajednicko->IZLAZ = (Zajednicko->IZLAZ + 1) % 5;
        
        usleep(150000);

        povecaj.sem_num = PUN; 
        semop(SemId, &povecaj, 1);
    }
    printf("Potrosac - zbroj primljenih brojeva=%d\n", zbroj);
    fflush(stdout);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Koristenje: %s <broj_proizvodjaca> <broj_brojeva>\n", argv[0]);
        return 1;
    }
    int broj_proizvodjaca = atoi(argv[1]);
    int broj_brojeva = atoi(argv[2]);

    if (broj_proizvodjaca <= 0 || broj_brojeva <= 0) {
        fprintf(stderr, "Argumenti moraju biti veci od 0.\n");
        return 1;
    }
    
    struct sigaction sa;
    sa.sa_handler = oslobodi_resurse;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    ShmId = shmget(IPC_PRIVATE, sizeof(struct dijeljeno), 0600);
    if (ShmId < 0) {
        perror("shmget");
        return 1;
    }
    Zajednicko = (struct dijeljeno *)shmat(ShmId, NULL, 0);
    Zajednicko->ULAZ = 0;
    Zajednicko->IZLAZ = 0;
    Zajednicko->UKUPNO = broj_proizvodjaca * broj_brojeva;

    SemId = semget(IPC_PRIVATE, 3, 0600);
    if (SemId < 0) {
        perror("semget");
        shmctl(ShmId, IPC_RMID, NULL);
        return 1;
    }
    semctl(SemId, PUN, SETVAL, 5);     
    semctl(SemId, PISI, SETVAL, 1);    
    semctl(SemId, PRAZAN, SETVAL, 0);  

    if (fork() == 0) {
        Potrosac();
    }
    for (int i = 0; i < broj_proizvodjaca; i++) {
        if (fork() == 0) {
            srand(getpid()); 
            Proizvodjac(i + 1, broj_brojeva);
        }
    }
    for (int i = 0; i < broj_proizvodjaca + 1; i++) {
        wait(NULL);
    }
    oslobodi_resurse(0);
    return 0;
}
