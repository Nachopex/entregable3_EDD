/*
 * Archivo de cabecera para la clase hashing_abierto.
 * Define la estructura para una tabla hash que resuelve las colisiones
 * empleando el encadenamiento mediante listas doblemente enlazadas.
 */

#ifndef HASHING_ABIERTO_H
#define HASHING_ABIERTO_H

#include <vector>
#include <string>
#include "Lista_doblemente_enlazada.h"
#include "tipos_clave.h"

/*
 * Clase principal que implementa el hashing abierto.
 * Cada indice de la tabla hash (vector) contiene una lista doblemente enlazada.
 */
class hashing_abierto {
    private:
        std::vector<Lista_doblemente_enlazada> tabla; // Vector contenedor de listas
        int capacidad;                                // Capacidad fija de la tabla
        TipoClave tipo_clave;                         // Tipo de clave que maneja la tabla
        
        // Genera un indice numerico a partir de una cadena de texto
        int funcionHash(std::string clave);
        int funcionHashUserId(std::string clave);
        int funcionHashScreenName(std::string clave);

    public:
        // Inicializa la tabla estableciendo su tamano inicial
        hashing_abierto(int cap, TipoClave tipo);
        ~hashing_abierto();

        // Inserta o actualiza un elemento utilizando la funcion hash para encontrar la cubeta
        void insertar(std::string clave);
        
        // Muestra un reporte estadistico sobre la distribucion de elementos en la tabla
        void imprimir_prueba(std::string nombre_tabla);

        // Entrega una estimacion del uso de memoria de la estructura en bytes
        size_t memoria_aproximada_bytes();
};

#endif
