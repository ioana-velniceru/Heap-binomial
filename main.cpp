#include <iostream>
#include <fstream>
#include <list>

using namespace std;

const int INF = 2000000001;
const int NMAX = 101;

ifstream fi("mergeheap.in");
ofstream fo("mergeheap.out");

struct nod{
    int valoare, grad;
    nod *fiu, *frate, *tata;
};

nod* creare_nod(int val){
    nod* temp = new nod;
    temp -> valoare = val;
    temp -> grad = 0;
    temp -> fiu = temp -> frate = temp -> tata = NULL;
    return temp;
}

class heap_binomial{
    list<nod*> lista_heap;
    list<nod*> :: iterator gasire_radacina(){
        list<nod*> :: iterator it, it_min;
        nod* elmin = creare_nod(INF);
        for (it = lista_heap.begin(); it != lista_heap.end(); ++it){
            if ((*it) -> valoare < elmin -> valoare){
                elmin = *it;
                it_min = it;
            }
        }
        return it_min;
    }
    void stergere_radacina(nod* rad, heap_binomial& heap){
        if (rad -> grad == 0){
            delete rad;
            return;
        }
        nod* temp = rad;
        rad -> fiu -> tata = NULL;
        heap.lista_heap.push_front(rad -> fiu);
        rad = rad -> fiu;
        while (rad -> frate){
            rad -> frate -> tata = NULL;
            heap.lista_heap.push_front(rad -> frate);
            rad = rad -> frate;
        }
        delete temp;
    }
    void merge_heapuri(nod* heap1, nod* heap2){
        if (heap1 -> valoare > heap2 -> valoare){
            swap(*heap1, *heap2);
        }
        heap2 -> frate = heap1 -> fiu;
        heap2 -> tata = heap1;
        heap1 -> fiu = heap2;
        heap1 -> grad++;
    }
    void refacere_heap(){
        if (lista_heap.size() <= 1){
            return;
        }
        list<nod*> :: iterator anterior;
        list<nod*> :: iterator curent;
        list<nod*> :: iterator urmator;
        list<nod*> :: iterator temp;
        anterior = curent = lista_heap.begin();
        curent++;
        urmator = curent;
        urmator++;
        while (curent != lista_heap.end()){
            while ((urmator == lista_heap.end() || (*urmator) -> grad > (*curent) -> grad) && curent != lista_heap.end() && (*anterior) -> grad == (*curent) -> grad){
                merge_heapuri(*curent, *anterior);
                temp = anterior;
                if (anterior == lista_heap.begin()){
                    anterior++;
                    curent++;
                    if (urmator != lista_heap.end()){
                        urmator++;
                    }
                } else {
                    anterior--;
                }
                lista_heap.erase(temp);
            }
            anterior++;
            if (curent != lista_heap.end()){
                curent++;
            }
            if (urmator != lista_heap.end()){
                urmator++;
            }
        }
    }
public:
    int top(){
        return (*gasire_radacina()) -> valoare;
    }
    void push(int val){
        nod *nou_nod = creare_nod(val);
        lista_heap.push_front(nou_nod);
        refacere_heap();
    }
    void reuniune_heapuri(heap_binomial& heap2){
        list<nod*> :: iterator it1 = this -> lista_heap.begin();
        list<nod*> :: iterator it2 = heap2.lista_heap.begin();
        list<nod*> heap_final;
        while (it1 != this -> lista_heap.end() && it2 != heap2.lista_heap.end()){
            if ((*it1) -> grad <= (*it2) -> grad){
                heap_final.push_back(*it1);
                it1++;
            } else {
                heap_final.push_back(*it2);
                it2++;
            }
        }
        while (it1 != this -> lista_heap.end()){
            heap_final.push_back(*it1);
            it1++;
        }
        while (it2 != heap2.lista_heap.end()){
            heap_final.push_back(*it2);
            it2++;
        }
        heap2.lista_heap.clear();
        this -> lista_heap = heap_final;
        refacere_heap();
    }
    void pop(){
        list<nod*> :: iterator rad = gasire_radacina();
        heap_binomial nou_heap;
        stergere_radacina((*rad), nou_heap);
        lista_heap.erase(rad);
        reuniune_heapuri(nou_heap);
    }
};

int nr_mult, nr_op;
heap_binomial Heap[NMAX];

int main()
{
    fi >> nr_mult >> nr_op;
    int op, mult, val, heap1, heap2, nr;
    for (int i = 0; i < nr_op; ++i){
        fi >> op;
        if (op == 1){
            fi >> mult >> val;
            Heap[mult].push(val);
        } else if (op == 2){
            fi >> mult;
            fo << Heap[mult].top() << '\n';
            Heap[mult].pop();
        } else if (op == 3){
            fi >> heap1 >> heap2;
            Heap[heap1].reuniune_heapuri(Heap[heap2]);
        } else if (op == 4){
            //putem transforma in functia construieste_heap(int mult, int nr)
            fi >> mult >> nr;
            for (int i = 0; i < nr; i++){
                fi >> val;
                Heap[mult].push(val);
            }
        }
    }
    return 0;
}
