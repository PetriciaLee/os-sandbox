// vježba 1: Signali i prekidi


#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define N 8 

int OZNAKA_CEKANJA[N + 1];
int PRIORITET[N + 1];
int TEKUCI_PRIORITET = 0;

void ispisi_vrijeme() {
    time_t t;
    time(&t);
    printf("Prekidna rutina pozvana u: %s", ctime(&t));
}

void obradba_prekida(int i) {
    printf("Poceo obradbu prekida %d\n", i);
    for (int s = 1; s <= 5; s++) {
        sleep(1); 
        printf("Obradba prekida %d: %d/5\n", i, s);
    }
    printf("Zavrsio obradbu prekida %d\n", i);
}

void prekidna_rutina(int sig) {
    int i, x;
    
    
    sighold(SIGINT);
    
    ispisi_vrijeme();
    printf("Upisi razinu prekida (1 do %d): ", N);
    scanf("%d", &i);
    
    OZNAKA_CEKANJA[i] = 1;

    do {
        x = 0;
        for (int j = N; j > TEKUCI_PRIORITET; j--) {
            if (OZNAKA_CEKANJA[j] != 0) {
                x = j;
                break;
            }
        }

        if (x > 0) {
            OZNAKA_CEKANJA[x] = 0;
            PRIORITET[x] = TEKUCI_PRIORITET;
            TEKUCI_PRIORITET = x;

            sigrelse(SIGINT); /
            obradba_prekida(x);
            sighold(SIGINT); 

            TEKUCI_PRIORITET = PRIORITET[x];
        }
    } while (x > 0);

    sigrelse(SIGINT);
}

int main() {
    
    sigset(SIGINT, prekidna_rutina);

    printf("Poceo osnovni program\n");
    for (int i = 1; i <= 15; i++) {
        sleep(1);
        printf("Glavni program: %d/15\n", i);
    }
    printf("Zavrsio osnovni program\n");

    return 0;
}