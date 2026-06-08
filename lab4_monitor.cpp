#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <csignal>

using namespace std;

const int LINUX = 0;
const int MICROSOFT = 1;

int N;            
int broj_svake_vrste; 


pthread_mutex_t monitor_latch; 
pthread_cond_t red_linux;      
pthread_cond_t red_microsoft;  

int u_restoranu = 0;       
int vrsta_u_restoranu = -1;
int cekaju_L = 0;          
int cekaju_M = 0;          

int propusteno_L = 0;
int propusteno_M = 0;

void ispis_stanja(int id, int vrsta, string akcija) {
    string ispis_L = "";
    for (int i = 0; i < broj_svake_vrste; i++) {
        if (i < cekaju_L) ispis_L += "L";
        else ispis_L += "-";
    }

    string ispis_M = "";
    for (int i = 0; i < broj_svake_vrste; i++) {
        if (i < cekaju_M) ispis_M += "M";
        else ispis_M += "-";
    }

    string ispis_restoran = "";
    if (vrsta_u_restoranu != -1) {
        char oznaka = (vrsta_u_restoranu == LINUX) ? 'L' : 'M';
        for (int i = 0; i < u_restoranu; i++) {
            ispis_restoran += oznaka;
        }
    }

    char trenutni_p = (vrsta == LINUX) ? 'L' : 'M';
    
    cout << "Red Linux:" << ispis_L << " Red Microsoft:" << ispis_M 
         << " Restoran:" << ispis_restoran << " --> " << trenutni_p << " " << akcija << endl;
}

void udji(int vrsta, int id) {
    pthread_mutex_lock(&monitor_latch);

    if (vrsta == LINUX) {
        cekaju_L++;
        while (vrsta_u_restoranu == MICROSOFT || (cekaju_M > 0 && propusteno_L >= N)) {
            ispis_stanja(id, vrsta, "u red čekanja");
            pthread_cond_wait(&red_linux, &monitor_latch);
        }
        cekaju_L--;
        u_restoranu++;
        vrsta_u_restoranu = LINUX;
        propusteno_L++;
        propusteno_M = 0; 
        ispis_stanja(id, vrsta, "u restoran");
    } 
    else { 
        cekaju_M++;
        
        while (vrsta_u_restoranu == LINUX || (cekaju_L > 0 && propusteno_M >= N)) {
            ispis_stanja(id, vrsta, "u red čekanja");
            pthread_cond_wait(&red_microsoft, &monitor_latch);
        }
        cekaju_M--;
        u_restoranu++;
        vrsta_u_restoranu = MICROSOFT;
        propusteno_M++;
        propusteno_L = 0; 
        ispis_stanja(id, vrsta, "u restoran");
    }

    pthread_mutex_unlock(&monitor_latch);
}

void izadji(int vrsta, int id) {
    pthread_mutex_lock(&monitor_latch);

    u_restoranu--;
    ispis_stanja(id, vrsta, "iz restorana");

    if (u_restoranu == 0) {
        vrsta_u_restoranu = -1; 

        if (vrsta == LINUX) {
            if (cekaju_M > 0 && (propusteno_L >= N || cekaju_L == 0)) {
                pthread_cond_broadcast(&red_microsoft);
            } else if (cekaju_L > 0) {
                pthread_cond_broadcast(&red_linux);
            } else if (cekaju_M > 0) {
                pthread_cond_broadcast(&red_microsoft);
            }
        } 
        else { 
            if (cekaju_L > 0 && (propusteno_M >= N || cekaju_M == 0)) {
                pthread_cond_broadcast(&red_linux);
            } else if (cekaju_M > 0) {
                pthread_cond_broadcast(&red_microsoft);
            } else if (cekaju_L > 0) {
                pthread_cond_broadcast(&red_linux);
            }
        }
    }

    pthread_mutex_unlock(&monitor_latch);
}

struct dretva_arg {
    int id;
    int vrsta;
};

void* programer(void* arg) {
    dretva_arg* p_info = (dretva_arg*)arg;
    int vrsta = p_info->vrsta;
    int id = p_info->id;

    usleep(rand() % 100 * 1000); 

   
    udji(vrsta, id);

    usleep(rand() % 100 * 1000); 

    izadji(vrsta, id);

    delete p_info; 
    return NULL;
}

void unisti_monitore() {
    pthread_mutex_destroy(&monitor_latch);
    pthread_cond_destroy(&red_linux);
    pthread_cond_destroy(&red_microsoft);
}

void ukloni_na_signal(int sig) {
    unisti_monitore();
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Pogreška! Ispravan unos: " << argv[0] << " <N_propustanje> <broj_programera_svake_vrste>" << endl;
        return 1;
    }

    N = atoi(argv[1]);
    broj_svake_vrste = atoi(argv[2]);

    srand(time(NULL));
    signal(SIGINT, ukloni_na_signal);

    pthread_mutex_init(&monitor_latch, NULL);
    pthread_cond_init(&red_linux, NULL);
    pthread_cond_init(&red_microsoft, NULL);

    int ukupno_dretvi = broj_svake_vrste * 2;
    vector<pthread_t> dretve(ukupno_dretvi);

    int idx = 0;
    for (int i = 0; i < broj_svake_vrste; i++) {
        dretva_arg* argL = new dretva_arg{i, LINUX};
        pthread_create(&dretve[idx++], NULL, programer, (void*)argL);

        dretva_arg* argM = new dretva_arg{i, MICROSOFT};
        pthread_create(&dretve[idx++], NULL, programer, (void*)argM);
    }

    for (int i = 0; i < ukupno_dretvi; i++) {
        pthread_join(dretve[i], NULL);
    }

    unisti_monitore();

    return 0;
}
