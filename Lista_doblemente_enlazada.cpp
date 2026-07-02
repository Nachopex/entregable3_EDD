/*
 * Implementacion de los metodos de la clase Nodo y Lista_doblemente_enlazada.
 */

#include <iostream>
#include "Lista_doblemente_enlazada.h"
#include <iostream>

// Inicializa el nodo con una clave, establece el conteo en 1 y los punteros a nulo.
Nodo::Nodo(std::string c) {
    clave = c;
    cantidad_tweets = 1;
    siguiente = nullptr;
    anterior = nullptr; 
}

// Inicializa la lista vacia estableciendo punteros a nulo y el contador de elementos a 0.
Lista_doblemente_enlazada::Lista_doblemente_enlazada() {
    cabeza = nullptr;
    cola = nullptr;
    elementos = 0;
}

// Libera secuencialmente la memoria dinamica reservada por cada nodo al destruir la lista.
Lista_doblemente_enlazada::~Lista_doblemente_enlazada() {
    Nodo* temporal;
    while(cabeza != nullptr) {
        temporal = cabeza;
        cabeza = cabeza->siguiente;
        delete temporal;
    }
}

// Verifica si la clave ya esta en la lista. Si existe, incrementa su frecuencia.
// Si no existe, crea un nuevo nodo y lo enlaza al final de la estructura.
void Lista_doblemente_enlazada::insertar_o_sumar(std::string clave) {
    Nodo* temporal = cabeza;
    
    // Recorre la lista buscando coincidencias
    while(temporal != nullptr) {
        if(temporal->clave == clave) {
            temporal->cantidad_tweets++;
            return; // Finaliza si se encontro y actualizo
        }
        temporal = temporal->siguiente;
    }
    
    // Si la clave no existe, se procede a crear un nuevo nodo
    Nodo* nodo_nuevo = new Nodo(clave);
    
    // Si la lista esta vacia, el nuevo nodo es tanto cabeza como cola
    if (isEmpty()) {
        cabeza = nodo_nuevo;
        cola = nodo_nuevo;
    } else {
        // Enlaza el nuevo nodo al final de la lista
        cola->siguiente = nodo_nuevo;
        nodo_nuevo->anterior = cola;
        cola = nodo_nuevo; 
    }
    elementos++;
}

// Realiza una busqueda lineal en la lista para retornar el conteo asociado a una clave.
int Lista_doblemente_enlazada::buscar_cantidad(std::string clave) {
    Nodo* temporal = cabeza;
    while(temporal != nullptr) {
        if(temporal->clave == clave) {
            return temporal->cantidad_tweets;
        }
        temporal = temporal->siguiente;
    }
    return 0; // Retorna 0 si la clave no esta presente
}

// Devuelve el numero total de elementos distintos almacenados en la lista.
int Lista_doblemente_enlazada::size() {
    return elementos;
}

// Evalua si la lista carece de elementos comprobando el estado del puntero cabeza.
bool Lista_doblemente_enlazada::isEmpty() {
    return cabeza == nullptr; 
}

// Itera sobre los primeros elementos de la lista para imprimirlos.
// Se utiliza para fines de demostracion limitando la salida por consola.
void Lista_doblemente_enlazada::imprimir_muestra(int max_elementos) {
    Nodo* temporal = cabeza;
    int contador = 0;
    
    while(temporal != nullptr && contador < max_elementos) {
        std::cout << "    - Clave: " << temporal->clave 
                  << " | Tweets publicados: " << temporal->cantidad_tweets << std::endl;
        temporal = temporal->siguiente;
        contador++;
    }
    
    // Muestra una indicacion si la lista contiene mas elementos que el maximo solicitado
    if (temporal != nullptr) {
        std::cout << "    ... (hay mas elementos, pero se omiten para la prueba)" << std::endl;
    }
}