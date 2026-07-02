#include <vector>
#include <utility>
#include <string>
#include <iostream>
//Para usar en lineal probing el metodo de la multiplicacion
const double A = 0.6180339887;

enum Estado { VACIO, OCUPADO, BORRADO };


struct Celda {
    public:
        std::string clave;
        int valor;
        Estado estado;
    
    Celda() : clave(""), valor(0), estado(VACIO) {}
};


class hashing_cerrado {
    private:
        std::vector<Celda> tabla;
        int capacidad;
        int cantidad;
    
    public:
    
        hashing_cerrado(int capacidad);
        ~hashing_cerrado();


        void insertar(std::string clave);

        int linear_probing(std::string clave);
        int quadratic_probing(std::string clave);
        int double_hashing(std::string clave);

        void imprimir_prueba(std::string nombre_tabla);

        void rehash();
};
