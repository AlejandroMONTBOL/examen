#include <iostream>
#include <cstdlib>
#include <mutex>
#include <thread>
#include <chrono>
#include <semaphore.h>
#define MAX_COMIENDO 3
#define NUM_HAMSTERS 5
using namespace std;
using namespace chrono;

mutex mtx;
sem_t sem;

class Hamster {
public:
        string nombre;
        Hamster(string nom)
        {
                this->nombre = nom;
        }
        ~Hamster()
        {

        }

};
class Rueda {
public:
        Hamster* h;
        Rueda() {
                h = NULL;
        }
        ~Rueda()
        {

        }

};
Rueda rueda;
int comiendo = 0;
void comer(Hamster& h) {
        mtx.lock();
        while (comiendo == MAX_COMIENDO) {
                mtx.unlock();
                sem_wait(&sem);
                mtx.lock();
        }
        comiendo++;
        printf("Hamsters comiendo: %d. \n", comiendo);
        mtx.unlock();
	printf("%s esta comiendo.\n", h.nombre.c_str());
        this_thread::sleep_for(seconds(2));
        mtx.lock();
	printf("%s ya no esta comiendo.\n", h.nombre.c_str());
	comiendo--;
	printf("Hamsters comiendo: %d. \n", comiendo);
        sem_post(&sem);
        mtx.unlock();
}
void correr(Hamster& h) {

        mtx.lock();
        while (rueda.h != NULL) {
                mtx.unlock();
                sem_wait(&sem);
                mtx.lock();
        }
        rueda.h = &h;
        printf("%s esta corriendo.\n", h.nombre.c_str());
        this_thread::sleep_for(seconds(4));
        rueda.h = NULL;
	printf("%s ya no esta corriendo.\n", h.nombre.c_str());
	printf("rueda vacia.\n");
        sem_post(&sem);
        mtx.unlock();

}
void hiloHamster(Hamster& h) {
        while (true) {
                comer(h);
                correr(h);
        }

}
int main() {
        thread* threads = new thread[NUM_HAMSTERS];
        sem_init(&sem, 0, 0);
        Hamster h("h1");
        Hamster h1("h2");
        Hamster h2("h3");
        Hamster h3("h4");
        Hamster h4("h5");
        threads[0] = thread(hiloHamster, ref(h));
        threads[1] = thread(hiloHamster, ref(h1));
        threads[2] = thread(hiloHamster, ref(h2));
        threads[3] = thread(hiloHamster, ref(h3));
        threads[4] = thread(hiloHamster, ref(h4));
        for (int i = 0; i < NUM_HAMSTERS; i++)
        {
                threads[i].join();
        }
        sem_destroy(&sem);
        delete[] threads;
        return EXIT_SUCCESS;
}
