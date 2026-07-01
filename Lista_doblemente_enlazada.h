#ifndef LISTA_DOBLEMENTE_ENLAZADA_H
#define LISTA_DOBLEMENTE_ENLAZADA_H
#include <string>


class Nodo {
public:
    std::string clave;     
    int cantidad_tweets;    
    Nodo* siguiente;
    Nodo* anterior; 

    
    Nodo(std::string c);
};


class Lista_doblemente_enlazada {
private:
    Nodo* cabeza; 
    Nodo* cola;   
    int elementos;

public:
    Lista_doblemente_enlazada();
    ~Lista_doblemente_enlazada();


    void insertar_o_sumar(std::string clave);


    bool isEmpty();
    int size();
    int buscar_cantidad(std::string clave);

    void imprimir_muestra(int max_elementos);
};

#endif