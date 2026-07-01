#include <iostream>
#include "Lista_doblemente_enlazada.h"


Nodo::Nodo(std::string c) {
    clave = c;
    cantidad_tweets = 1;
    siguiente = nullptr;
    anterior = nullptr; 
}


Lista_doblemente_enlazada::Lista_doblemente_enlazada() {
    cabeza = nullptr;
    cola = nullptr;
    elementos = 0;
}

Lista_doblemente_enlazada::~Lista_doblemente_enlazada() {
    Nodo* temporal;
    while(cabeza != nullptr) {
        temporal = cabeza;
        cabeza = cabeza->siguiente;
        delete temporal;
    }
}


void Lista_doblemente_enlazada::insertar_o_sumar(std::string clave) {
    Nodo* temporal = cabeza;
    
    while(temporal != nullptr) {
        if(temporal->clave == clave) {
            temporal->cantidad_tweets++;
            return;
        }
        temporal = temporal->siguiente;
    }
    
    Nodo* nodo_nuevo = new Nodo(clave);
    
    if (isEmpty()) {
        cabeza = nodo_nuevo;
        cola = nodo_nuevo;
    } else {
        cola->siguiente = nodo_nuevo;
        nodo_nuevo->anterior = cola;
        cola = nodo_nuevo; 
    }
    elementos++;
}

int Lista_doblemente_enlazada::buscar_cantidad(std::string clave) {
    Nodo* temporal = cabeza;
    while(temporal != nullptr) {
        if(temporal->clave == clave) {
            return temporal->cantidad_tweets;
        }
        temporal = temporal->siguiente;
    }
    return 0;
}

int Lista_doblemente_enlazada::size() {
    return elementos;
}

bool Lista_doblemente_enlazada::isEmpty() {
    return cabeza == nullptr; 
}

void Lista_doblemente_enlazada::imprimir_muestra(int max_elementos) {
    Nodo* temporal = cabeza;
    int contador = 0;
    
    while(temporal != nullptr && contador < max_elementos) {
        std::cout << "    - Clave: " << temporal->clave 
                  << " | Tweets publicados: " << temporal->cantidad_tweets << std::endl;
        temporal = temporal->siguiente;
        contador++;
    }
    
    if (temporal != nullptr) {
        std::cout << "    ... (hay mas elementos, pero se omiten para la prueba)" << std::endl;
    }
}