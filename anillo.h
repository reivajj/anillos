#ifndef ANILLO_H_
#define ANILLO_H_

#include <iostream>
#include <cassert>
using namespace std;

//se puede asumir que el tipo T tiene constructor por copia
//y operator==
//no se puede asumir que el tipo T tenga operator=
template<typename T>
class Anillo {
public:

	/*
	 * constructor por defecto, crea un anillo
	 */
	Anillo();

	/*
	 * constructor por copia, una vez copiado, ambos anillos deben ser
	 * independientes, es decir, cuando se borre uno no debe morir el otro.
	 */
	Anillo(const Anillo<T>& a);

	/*
	 * Destructor. Acordarse de liberar toda la memoria!
	 */
	~Anillo();

	/*
	 * Devuelve true si los anillos son iguales
	 */
    bool operator==(const Anillo<T>& otro) const;


	/*
	 * Dice si el anillo es vac�o.
	 */
	bool esVacio() const;

	/*
	 * Dice el tamanio del anillo
	 */
	int tamanio() const;

	/*
	 * Devuelve el pr�ximo elemento seg�n el orden del anillo.
	 * El anillo debe rotar. Con lo cual sucesivas llamadas a esta funci�n
	 * retornan distintos valores.
	 *
	 * El valor retornado pasa a ser el anterior.
	 *
	 * PRE: no es vac�o el anillo.
	 */
	const T& siguiente();

	/*
	 * Agrega el elemento al anillo. Recordar que el �ltimo agregado es el
	 * pr�ximo en el orden
	 */
	void agregar(const T& elem);

	/*
	 * Elimina el elemento indicado del anillo.
	 */
	void eliminar(const T& elem);

	/*
	 * Indica si hubo anterior elemento elegido
	 * y si �ste pertenece todav�a al anillo.
	 */
	bool huboAnterior() const;

	/*
	 * Indica cu�l es el elemento elegido la �ltima vez.
	 */
	const T& anterior() const;

	/*
	 * Vuelve hacia atr�s un elemento.
	 * El anterior, en caso de existir, no debe alterarse.
	 */
	void retroceder();

	/*
	 * debe mostrar el anillo en el stream (y retornar el mismo).
	 * Anillo vacio: []
	 * Anillo con 2 elemento (e1 es el pr�ximo a aparecer en siguiente): [e1, e2]
	 * Anillo con 2 elemento (e2 es el pr�ximo a aparecer en siguiente y e1 es el anterior): [e2, e1*]
	 *
	 */
	ostream& mostrarAnillo(ostream& os) const;

private:
	//No se puede modificar esta funcion.
	Anillo<T>& operator=(const Anillo<T>& otro) {
		assert(false);
		return *this;
	}

	//Aca va la implementacion del nodo.
	struct Nodo {
        Nodo(const T& d) : dato(d), anterior(NULL), siguiente(NULL) {};
        T dato;
        Nodo* anterior;
        Nodo* siguiente;
	};
    Nodo* primero_;
    int tamanio_;
    Nodo* anterior_;
    Nodo* buscarAnterior(const T& elem);
};


//==================================================================================================
template <typename T>
Anillo<T>::Anillo()
        : primero_(NULL), tamanio_(0), anterior_(NULL) {}
//==================================================================================================
template <typename T>
Anillo<T>::Anillo(const Anillo<T>& a) : primero_(NULL), tamanio_(0), anterior_(a.anterior_)
    {

        Nodo* aux = a.primero_;
        Anillo<T> temp;
        int i = 0;
        while(i < a.tamanio_){
            temp.agregar(aux->dato);
            aux = aux->siguiente;
            i++;
        }
        int tam = a.tamanio_;
       // Nodo* auxT = temp.primero_;
        while(tam > 0){
            //this->agregar(auxT->dato);
            //auxT = auxT->siguiente;
            //tam--;
            agregar(temp.primero_->dato);
            temp.eliminar(temp.primero_->dato);
            tam--;
        }
    }
//==================================================================================================
template <typename T>
Anillo<T>::~Anillo(){
    //delete anterior_;
    Nodo* aux = primero_;
    if(primero_ == NULL) {
        delete primero_;
        return;
    }
    int i = 0;
    int tamOriginal = tamanio();
    while(i < tamOriginal && aux != NULL){
        Nodo* aux1 = aux;
        aux = aux->siguiente;
        i++;
        eliminar(aux1->dato);
    }
    tamanio_ = 0;
    primero_ = NULL;
}
//==================================================================================================
template <typename T>
bool Anillo<T>::esVacio() const{
    return primero_ == NULL;
}
//==================================================================================================
template <typename T>
int Anillo<T>::tamanio() const{
    return tamanio_;
}
//==================================================================================================
template <typename T>
const T& Anillo<T>::siguiente(){
    Nodo* aux = primero_;
    primero_ = primero_->siguiente;
    anterior_ = aux;
    return aux->dato;
}
//==================================================================================================
template <typename T>
void Anillo<T>::agregar(const T& elem){
    Nodo* nuevo = new Nodo(elem);
    Nodo* aux = primero_;

    if(aux == NULL) {
        primero_ = nuevo;
        primero_->siguiente = nuevo;
        primero_->anterior = nuevo;
        tamanio_++;
        return;
    }
    //si el anillo es vacio solo tendra a nuevo
    if(aux->anterior==NULL){
        //si solo tiene un elem, los encadeno
        nuevo->anterior = aux;
        nuevo->siguiente = aux;
        aux->siguiente = nuevo;
        aux->anterior = nuevo;
        primero_ = nuevo;
        tamanio_++;
        return;
    }
    else{
        nuevo->siguiente = aux;
        Nodo* iter = aux;
        while(iter->siguiente != aux){
            iter = iter->siguiente;
        }
        nuevo->anterior = iter;
        iter->siguiente = nuevo;
        aux->anterior = nuevo;
        primero_ = nuevo;
        tamanio_++;
        return;
    }
}
//==================================================================================================
template <typename T>
void Anillo<T>::eliminar(const T& elem){
    bool eliminado = false;
    Nodo* aux = primero_;
    if(aux == NULL){
        return;
    }
    if(tamanio_ == 1){
        if(aux->dato == elem){
            if(aux == anterior_) anterior_ = NULL;
            delete aux;
            aux = NULL;
            eliminado = true;
            primero_ = NULL;
            tamanio_--;
        }
        return;
    }
    else{
        Nodo* anteriorAlTarget = buscarAnterior(elem);
        if(anteriorAlTarget == NULL) return;
        Nodo* siguienteAlTarget = anteriorAlTarget->siguiente->siguiente;
        Nodo* target = anteriorAlTarget->siguiente;
        if(target == primero_) primero_ = siguienteAlTarget;
        if(target == anterior_) anterior_ = NULL;
        delete target;
        target = NULL;
        eliminado = true;
        anteriorAlTarget->siguiente = siguienteAlTarget;
        siguienteAlTarget->anterior = anteriorAlTarget;
    }
    if(eliminado) tamanio_--;
}

//==================================================================================================
template <typename T>
bool Anillo<T>::huboAnterior() const{
    if(anterior_ == NULL) return false;
    else return true;
    
}

template <typename T>
const T& Anillo<T>:: anterior() const{
    return anterior_;
}

template <typename T>
void Anillo<T>::retroceder(){
    Nodo* aux = primero_;
    if(aux->anterior == NULL) return;
    primero_ = aux->anterior;
}

template <typename T>
ostream& Anillo<T>::mostrarAnillo(ostream& os) const{
    if(tamanio_==0) os << "[]";
    else{
        os << "[";
        //cout << "[\n" ;
        Nodo* aux = primero_;
        int i = 0;
        while(i < tamanio_){
            if(anterior_ != NULL && anterior_->dato == aux->dato){
                os << aux->dato << "*";
                //cout << "*]\n" ;
            }
            else{
                os << aux->dato;
                //cout << aux->dato << "\n" ;
            }
            if(i+1 == tamanio_){
                    os << "]";
                //cout << "]\n";
            }
            else{
                os << ", ";
                //cout << ", \n";
            }
            aux = aux->siguiente;
            i++;
        }

    }
    return os;
}

template <typename T>
bool Anillo<T>::operator==(const Anillo<T>& otro) const{
    bool iguales = true;
    if(tamanio_ != otro.tamanio()) return false;

    Nodo* aux = primero_;
    Nodo* aux1 = otro.primero_;

    for(int i = 0; i < tamanio_; aux->siguiente){
        if(aux != aux1) return false;
        aux1 = aux1->siguiente;
    }
}

template<class T>
ostream& operator<<(ostream& out, const Anillo<T>& a) {
	return a.mostrarAnillo(out);
}


//AUX
template<typename T>
typename Anillo<T>::Nodo* Anillo<T>::buscarAnterior(const T& elem){
    if(primero_ == NULL) return NULL;
    Nodo* aux = primero_;
    if(aux->dato == elem){
        return aux->anterior;
    }
    else{
        aux = aux->siguiente;
    }
    T& datoAux = aux->dato;
    while(!(aux->siguiente->dato == datoAux)){
        //si no entra al while quiere decir que ya mire todos los elems.
        if(aux->dato == elem) return aux->anterior;
        aux = aux->siguiente;
    }
    return NULL;
    //devuelvo NULL si el elem no esta
}

#endif //ANILLO_H_
