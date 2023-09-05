#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define LEN 18000
#define MAXAUTO 512
#define INT_MIN (-32768)

struct stazione{
    int distanza;
    int cars[MAXAUTO];
    int size;
    int maxCar;
    int ref;
    struct stazione *next;
    struct stazione *prev;
};


typedef struct stazione *autostrada;

autostrada last_staz = NULL;

autostrada map[LEN];

struct coda{
    autostrada station;
    struct coda *next;
};

typedef struct coda *queue;

queue front = NULL;
queue rear = NULL;

int hash(int distanza){
    return distanza/65536;
}

void inserisciMap(char comandi[]){

    char *temp = comandi+18;
    int station = strtol(temp, &temp, 10);
    int numauto = strtol(temp, &temp, 10);

    if(numauto > MAXAUTO){
        printf("non aggiunta\n");
        return;
    }

    autostrada elem = (autostrada) malloc(sizeof (struct stazione));
    elem->distanza = station;
    elem->next = NULL;
    elem->prev = NULL;
    elem->size = numauto;
    elem->maxCar = 0;

    int i;
    for(i = 0; i< numauto;i++){
        //printf("n.%d : %ld \n", i, autonomie[i]);
        elem->cars[i] = strtol(temp, &temp, 10);
        if(elem->cars[i] > elem->maxCar){
            elem->maxCar = elem->cars[i];
        }
    }

    // hashing
    int index = hash(elem->distanza);


    // controllo gli i primi elementi del bucket
    if(map[index] == NULL){

        map[index] = elem;
        int check = 0;

        if(index == 0) {
            int j;
            for(j = index+1;j<LEN;j++){
                // mi fermo al primo bucket non nullo
                if(map[j] != NULL){
                    map[index]->next = map[j];
                    // controllo se ha già un prev
                    if(map[j] != NULL){
                        map[j]->prev = map[index];
                    }
                    printf("aggiunta\n");

                    return;
                }
            }
        }

        // scorro avanti per cercare il next
        int j;
        for(j = index+1;j<LEN;j++){
            // mi fermo al primo bucket non nullo
            if(map[j] != NULL){
                map[index]->next = map[j];
                // controllo se ha già un prev
                if(map[j]->prev != NULL){
                    map[j]->prev->next = map[index];
                    map[index]->prev = map[j]->prev;

                }

                map[j]->prev = map[index];
                check = 1;
                break;
            }
        }

        // se non lo trova scorro per trovare il prev
        if(check == 0){
            for(j = index-1;j>=0;j--){
                if(map[j] != NULL){
                    // trovo l'ultimo elemento di quel bucket
                    autostrada p = map[j];
                    while(p->next != NULL && p->next->distanza <= station){
                        if(p->next->distanza == station){
                            printf("non aggiunta\n");
                            return;
                        }
                        p = p->next;
                    }

                    elem->prev = p;
                    if(p->next != NULL){
                        elem->next = p->next;
                        p->next->prev = elem;
                    }
                    p->next = elem;

                    break;
                }
            }
        }

        printf("aggiunta\n");
        return;
    }


    if(map[index]->distanza >= station){
        if(map[index]->distanza == station){
            printf("non aggiunta\n");
            return;
        }

        elem->next = map[index];
        elem->prev = map[index]->prev;
        if(map[index]->prev != NULL){
            map[index]->prev->next = elem;
        }
        map[index]->prev = elem;
        map[index] = elem;

        printf("aggiunta\n");
        return;
    }


    // inserisco nel bucket al posto corretto
    autostrada p = map[index];
    while(p->next != NULL && p->next->distanza <= station){
        if(p->next->distanza == station){
            printf("non aggiunta\n");
            free(elem);
            return;
        }
        p = p->next;
    }

    elem->prev = p;

    if(p->next != NULL){
        p->next->prev = elem;
        elem->next = p->next;
    }

    p->next = elem;


    printf("aggiunta\n");

}

void enqueue(autostrada station){
    queue elem = (queue) malloc(sizeof(struct coda));
    elem->station = station;
    elem->next = NULL;

    if(front == NULL && rear == NULL){
        front = elem;
        rear = elem;
        return;
    }

    rear->next = elem;
    rear = elem;
}

autostrada dequeue(){

    if (front == NULL) {
        return NULL;
    }

    queue temp = front;
    autostrada elem = temp->station;

    if(front == rear){
        front = rear = NULL;
    }else{
        front = front->next;
    }


    free(temp);
    return elem;
}

void freeQueue(){
    queue t = front;
    while(t != NULL){
        queue temp = t;
        t = t->next;
        free(temp);
    }

    front = NULL;
    rear = NULL;

}


void printPercorsoAvanti(int visited[]){
    int i;
    for(i = LEN-1; i>=0;i--){
        if(visited[i] != -1){
            if(i==0){
                printf("%d", visited[i]);
            }else{
                printf("%d ", visited[i]);
            }
        }
    }


    printf("\n");
}

void printPercorsoIndietro(autostrada s_p, autostrada s_a){
    autostrada p = s_p, t = s_p;

    while(p != s_a){
        printf("%d ", t->distanza);
        while(p->distanza != t->ref){
            p = p->prev;
        }
        t = p;
    }

    printf("%d\n", s_a->distanza);
}

int findMaxCar(autostrada p){
    int max = 0;
    for(int i = 0; i < p->size; i++){
        if(p->cars[i] > max){
            max = p->cars[i];
        }
    }

    return max;
}

void rimuoviStazione(char comandi[]){

    char *temp = comandi+19;
    int station = strtol(temp, &temp, 10);

    int index = hash(station);

    autostrada t = map[index];

    if(map[index+1] != NULL){
        autostrada s = map[index+1];
        while(t != NULL && s!=NULL){
            if(t->distanza == station){

                if(map[index]->distanza == station) {
                    if (t->next != NULL && hash(t->next->distanza) == index){
                        map[index] = t->next;
                    }else{
                        map[index] = NULL;
                    }
                }

                // check per memory leak
                if(t->next != NULL){
                    t->next->prev = t->prev;
                }

                // check per memory leak
                if(t->prev != NULL){
                    t->prev->next = t->next;
                }

                if(t == last_staz){
                    free(last_staz);
                }

                // libero la memoria
                free(t);
                printf("demolita\n");
                //printAutostrada();
                return;
            }

            if(s->distanza == station){

                if(map[index]->distanza == station) {
                    if (s->next != NULL && hash(s->next->distanza) == index){
                        map[index] = s->next;
                    }else{
                        map[index] = NULL;
                    }
                }

                // check per memory leak
                if(s->next != NULL){
                    s->next->prev = s->prev;
                }

                // check per memory leak
                if(s->prev != NULL){
                    s->prev->next = s->next;
                }

                if(s == last_staz){
                    last_staz = NULL;
                    free(last_staz);
                }

                // libero la memoria
                free(s);
                printf("demolita\n");
                //printAutostrada();
                return;
            }

            t = t->next;
            s = s->prev;
        }

        printf("non demolita\n");
        return;
    }


    while(t != NULL){
        if(t->distanza == station){

            if(map[index]->distanza == station) {
                if (t->next != NULL && hash(t->next->distanza) == index){
                    map[index] = t->next;
                }else{
                    map[index] = NULL;
                }
            }

            // check per memory leak
            if(t->next != NULL){
                t->next->prev = t->prev;
            }

            // check per memory leak
            if(t->prev != NULL){
                t->prev->next = t->next;
            }



            if(t == last_staz){
                free(last_staz);
            }

            // libero la memoria
            free(t);
            printf("demolita\n");
            //printAutostrada();
            return;
        }

        t = t->next;
    }


    printf("non demolita\n");

}



void aggiungi_auto(char comandi[]){

    char *temp = comandi+14;
    int station = strtol(temp, &temp, 10);
    int autonomia = strtol(temp, &temp, 10);

    int index = hash(station);
    autostrada p = map[index];


    if(map[index+1] != NULL){
        autostrada s = map[index+1];
        while(p != NULL && s!=NULL){
            if(p->distanza == station){
                p->cars[p->size++] = autonomia;
                if(autonomia > p->maxCar){
                    p->maxCar = autonomia;
                }

                printf("aggiunta\n");
                return;

            }

            if(s->distanza == station){
                s->cars[s->size++] = autonomia;
                if(autonomia > s->maxCar){
                    s->maxCar = autonomia;
                }

                printf("aggiunta\n");
                return;

            }

            p = p->next;
            s = s->prev;

        }

        printf("non aggiunta\n");
        return;

    }

    while(p != NULL){
        if(p->distanza == station){
            p->cars[p->size++] = autonomia;
            if(autonomia > p->maxCar){
                p->maxCar = autonomia;
            }

            printf("aggiunta\n");
            return;

        }

        p = p->next;

    }

    printf("non aggiunta\n");

}


void rottama_auto(char comandi[]){
    int i;

    char *temp = comandi+13;
    int stazione = strtol(temp, &temp, 10);
    int autonomia = strtol(temp, &temp, 10);

    int index = hash(stazione);
    autostrada p = map[index];

    if(map[index+1] != NULL){
        autostrada s = map[index+1];
        while(p != NULL && s!=NULL){

            if(p->distanza == stazione){
                for (i = 0; i < p->size; i++) {
                    if (p->cars[i] == autonomia) {
                        p->cars[i] = p->cars[--p->size];

                        if(p->maxCar == autonomia){
                            p->maxCar = findMaxCar(p);
                        }

                        printf("rottamata\n");
                        return;
                    }
                }

                // non ho trovato la macchina
                printf("non rottamata\n");
                return;
            }

            if(s->distanza == stazione){
                for (i = 0; i < s->size; i++) {
                    if (s->cars[i] == autonomia) {
                        s->cars[i] = s->cars[--s->size];

                        if(s->maxCar == autonomia){
                            s->maxCar = findMaxCar(s);
                        }

                        printf("rottamata\n");
                        return;
                    }
                }

                // non ho trovato la macchina
                printf("non rottamata\n");
                return;
            }

            p = p->next;
            s = s->prev;
        }

        printf("non rottamata\n");
        return;
    }


    while(p != NULL){

        if(p->distanza == stazione){
            for (i = 0; i < p->size; i++) {
                if (p->cars[i] == autonomia) {
                    p->cars[i] = p->cars[--p->size];

                    if(p->maxCar == autonomia){
                        p->maxCar = findMaxCar(p);
                    }

                    printf("rottamata\n");
                    return;
                }
            }

            // non ho trovato la macchina
            printf("non rottamata\n");
            return;
        }
        p = p->next;
    }


    // non ho trovato la stazione
    printf("non rottamata\n");

}


void pianificaPercorsoAvanti(int partenza, int arrivo){

    int index = hash(partenza);

    autostrada p = map[index];

    int visited[LEN];
    int i = 0;
    int minStazione;


    int maxCar;


    memset(visited,-1,sizeof(visited));

    // go to arrival
    while(p->distanza != arrivo){
        p = p->next;
    }

    visited[i++] = arrivo;


    while(p->distanza != partenza){

        // from zero again
        minStazione = -1;


        autostrada t = p;
        autostrada s = p;


        // scan lista completa fino a tornare al punto di partenza
        while(t->distanza != partenza){
            maxCar = t->prev->maxCar;
            // printf("%d %d\n", t->prev->distanza, maxCar);

            int dist = p->distanza - t->prev->distanza;

            if(maxCar >= dist){
                minStazione = t->prev->distanza;
                s = t->prev;
            }

            t = t->prev;
        }


        // 0 stazioni intermedie, nessun percorso
        if(minStazione == -1){
            printf("nessun percorso\n");
            return;
        }

        // aggiorno il percorso da stampare
        visited[i] = minStazione;
        i++;

        // printf("min staz: %d\n", minStazione);

        // mi piazzo alla tappa per ricominciare di nuovo da qua
        p = s;

    }

    // stampo il percorso
    printPercorsoAvanti(visited);

}


void pianificaPercorsoIndietro(int partenza, int arrivo) {
    // int visited[LEN];
    int index = hash(arrivo);
    autostrada t = map[index];

    // memset(visited,-1, sizeof(visited));
    // int i = 0;

    // t punta all'arrivo
    while(t->distanza != arrivo){
        t = t->next;
    }

    autostrada p = t;

    // p punta alla partenza
    while(p->distanza != partenza){
        p = p->next;
        p->ref = INT_MIN;
    }

    p->ref = INT_MIN;

    // mi salvo partenza per stampa
    autostrada s_p = p;

    // mi salvo arrivo per stampa
    autostrada s_a = t;

    // scan come pianificaAvanti
    while(t != s_p && s_p->ref == INT_MIN){
        p = t->next;
        while(p != s_p->next){
            int maxCar = p->maxCar;

            if(maxCar >= (p->distanza-t->distanza) && p->ref == INT_MIN){
                p->ref = t->distanza;
                enqueue(p);
            }

            p = p->next;
        }

        t = dequeue();

        if(t == NULL){
            printf("nessun percorso\n");
            return;
        }

    }


    if(s_p->ref == -1){
        printf("nessun percorso\n");
        return;
    }


    printPercorsoIndietro(s_p, s_a);

    // cleanup
    freeQueue();

}


void clean() {
    int i = 0;

    while(map[i] != NULL){
        i++;
    }

    autostrada p = map[i];
    while (p != NULL) {
        autostrada temp = p;
        p = p->next;
        free(temp);
    }
}

void init(){
    int i;
    for(i=0;i<LEN;i++){
        map[i] = (autostrada) malloc(sizeof(struct stazione));
        map[i] = NULL;
    }
}


int main() {


    char comandi[LEN];

    init();

    while(fgets(comandi, sizeof(comandi), stdin)){

        // add station
        if(comandi[9] == 's'){
            inserisciMap(comandi);
        }


        // demolish station
        if(comandi[0] == 'd'){
            rimuoviStazione(comandi);
        }


        // add car
        if(comandi[9] == 'a'){
            aggiungi_auto(comandi);
        }


        // demolish car
        if(comandi[0] == 'r'){
            rottama_auto(comandi);
        }


        // plan trip
        if(comandi[0] == 'p'){

            char *temp = comandi+19;
            int partenza = strtol(temp, &temp, 10);
            int arrivo = strtol(temp, &temp, 10);


            if(partenza < arrivo){
                pianificaPercorsoAvanti(partenza, arrivo);
                //printf("\n");
            }else{
                pianificaPercorsoIndietro(partenza, arrivo);
            }


        }



    }


    clean();

    return 0;
}