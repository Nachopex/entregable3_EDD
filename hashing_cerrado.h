#ifndef HASHING_CERRADO_H
#define HASHING_CERRADO_H

/*
 * Archivo de cabecera para la clase hashing_cerrado.
 * Define las estructuras de datos y la interfaz para una tabla hash 
 * con manejo de colisiones mediante direccionamiento abierto.
 */

#include <vector>
#include <utility>
#include <string>
#include <iostream>

/* * Constante utilizada en el metodo de la multiplicacion para la funcion hash.
 * Representa la fraccion de la proporcion para una distribucion uniforme.
 */
const double A = 0.6180339887;

/*
 * Enumeracion para rastrear el estado de cada celda en la tabla hash.
 */
enum Estado { VACIO, OCUPADO, BORRADO };

/*
 * Estructura que representa cada espacio dentro de la tabla hash.
 * Almacena la clave, el valor asociado y su estado actual.
 */
struct Celda {
    public:
        std::string clave;
        int valor;
        Estado estado;
    
    // Constructor por defecto que inicializa la celda como vacia
    Celda() : clave(""), valor(0), estado(VACIO) {}
};

/*
 * Clase principal que implementa la tabla hash de direccionamiento abierto.
 */
class hashing_cerrado {
    private:
        std::vector<Celda> tabla; // Contenedor principal de la tabla
        int capacidad;            // Tamano total reservado para la tabla
        int cantidad;             // Numero actual de elementos insertados
    
    public:
    
        // Constructor que inicializa la tabla con una capacidad inicial
        hashing_cerrado(int capacidad);
        
        // Destructor
        ~hashing_cerrado();

        // Inserta una nueva clave o incrementa su valor si ya existe
        void insertar(std::string clave);

        // Funciones de calculo de indices y resolucion de colisiones
        int linear_probing(std::string clave);
        int quadratic_probing(std::string clave);
        int double_hashing(std::string clave);

        // Muestra por consola un reporte del estado actual de la tabla
        void imprimir_prueba(std::string nombre_tabla);

        // Redimensiona la tabla cuando el factor de carga supera el limite establecido
        void rehash();
};
#endif