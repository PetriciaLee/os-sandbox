#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <vector>

using namespace std;

struct MonitorBarijera {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int ukupno_dretava;
    int brojac;
    int faza; 

    void inicijaliziraj(int n) {
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
        ukupno_dretava = n;
        brojac = 0;
        faza = 0;
    }

    void unesi_i_ceka_barijeru(int dretva_id, int &broj) {
        pthread_mutex_lock(&mutex);

        cout << "Dretva " << dretva_id << ". unesite broj" << endl;
        cin >> broj;

        brojac++;
        if (brojac == ukupno_dretava) {
            brojac = 0;
            faza++;
            pthread_cond_broadcast(&cond);
        } else {
            int trenutna_faza = faza;
            while (trenutna_faza == faza) {
                pthread_cond_wait(&cond, &mutex);
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    void unisti() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }
};

MonitorBarijera barijera;
vector<int> uneseni_brojevi;
vector<int> dretve_ids;

void* dretva_funkcija(void* arg) {
    int id = *static_cast<int*>(arg);
    
    barijera.unesi_i_ceka_barijeru(id, uneseni_brojevi[id]);

    pthread_mutex_lock(&barijera.mutex);
    cout << "Dretva " << id << ". uneseni broj je " << uneseni_brojevi[id] << endl;
    pthread_mutex_unlock(&barijera.mutex);

    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Greška: Potrebno je zadati broj dretava kao argument!" << endl;
        cerr << "Primjer: " << argv[0] << " 6" << endl;
        return 1;
    }

    int N = atoi(argv[1]);
    if (N <= 0) {
        cerr << "Greška: Broj dretava mora biti veći od 0!" << endl;
        return 1;
    }

    cout << "Broj dretava=" << N << endl;

    barijera.inicijaliziraj(N);
    uneseni_brojevi.resize(N);
    dretve_ids.resize(N);

    vector<pthread_t> tids(N);

    for (int i = 0; i < N; i++) {
        dretve_ids[i] = i;
        if (pthread_create(&tids[i], nullptr, dretva_funkcija, &dretve_ids[i]) != 0) {
            cerr << "Greška pri stvaranju dretve " << i << endl;
            return 1;
        }
    }

    for (int i = 0; i < N; i++) {
        pthread_join(tids[i], nullptr);
    }

    barijera.unisti();

    return 0;
}
