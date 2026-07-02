/*
 * Archivo de cabecera para la implementacion de una Lista Doblemente Enlazada.
 * Esta estructura de datos se utiliza para manejar las colisiones en la tabla hash 
 * de direccionamiento abierto (hashing abierto) mediante el metodo de encadenamiento.
 */

#ifndef LISTA_DOBLEMENTE_ENLAZADA_H
#define LISTA_DOBLEMENTE_ENLAZADA_H
#include <string>

/*
 * Clase que representa un nodo individual dentro de la lista doblemente enlazada.
 * Cada nodo almacena una clave unica (identificador o nombre de usuario) y 
 * la frecuencia de aparicion de dicha clave.
 */
class Nodo {
public:
    std::string clave;     
    int cantidad_tweets;    
    Nodo* siguiente;
    Nodo* anterior; 

    // Constructor que inicializa el nodo con una clave especifica
    Nodo(std::string c);
};

/*
 * Clase que gestiona la coleccion de nodos mediante enlaces dobles.
 * Permite insercion, actualizacion de contadores y consultas sobre los elementos.
 */
class Lista_doblemente_enlazada {
private:
    Nodo* cabeza; 
    Nodo* cola;   
    int elementos; // Mantiene el registro del numero de nodos en la lista

public:
    // Constructor por defecto
    Lista_doblemente_enlazada();
    
    // Destructor para liberar la memoria de todos los nodos
    ~Lista_doblemente_enlazada();

    // Inserta un nuevo nodo al final de la lista, o incrementa el contador 
    // si la clave ya existe dentro de la lista.
    void insertar_o_sumar(std::string clave);

    // Retorna true si la lista no contiene elementos
    bool isEmpty();
    
    // Retorna la cantidad actual de nodos en la lista
    int size();
    
    // Busca una clave y retorna su frecuencia, devuelve 0 si no se encuentra
    int buscar_cantidad(std::string clave);

    // Muestra por consola un subconjunto de los elementos contenidos en la lista
    void imprimir_muestra(int max_elementos);
};

#endif