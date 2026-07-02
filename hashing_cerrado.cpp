#include "hashing_cerrado.h"




hashing_cerrado::hashing_cerrado(int cap) : capacidad(cap), cantidad(0), tabla(cap) {
    
};



hashing_cerrado::~hashing_cerrado() {
    
};

void hashing_cerrado::insertar(std::string clave) {
    

    if ((double)(cantidad + 1) / capacidad >= 0.70) {
        rehash();
    }

    int posicion = linear_probing(clave); 

    while (tabla[posicion].estado == OCUPADO) {
        
        if (tabla[posicion].clave == clave) {
            tabla[posicion].valor++;
            return;
        }
        
        posicion = (posicion + 1) % capacidad;
    }

    tabla[posicion].clave = clave;
    tabla[posicion].valor = 1;
    tabla[posicion].estado = OCUPADO;
    cantidad++;
}


int hashing_cerrado::linear_probing(std::string clave) {
    int valor = 0;
    for (char c : clave) {
        valor += c;
    }
    double multiplicacion = valor * A;
    double parte_fraccionaria = multiplicacion - (int)multiplicacion;

    return (int)(capacidad * parte_fraccionaria);
    
};

void hashing_cerrado::rehash() {

    int vieja_capacidad = capacidad;
    std::vector<Celda> tabla_vieja = tabla;


    capacidad = vieja_capacidad * 2; 
    

    tabla.assign(capacidad, Celda()); 
    cantidad = 0; 


    for (int i = 0; i < vieja_capacidad; i++) {
        
        if (tabla_vieja[i].estado == OCUPADO) {
            

            int posicion = linear_probing(tabla_vieja[i].clave);
            
            while (tabla[posicion].estado == OCUPADO) {
                posicion = (posicion + 1) % capacidad;
            }
            
            tabla[posicion] = tabla_vieja[i];
            cantidad++;
        }
    }
    
    std::cout << "[AVISO] Rehash ejecutado. Nueva capacidad: " << capacidad << std::endl;
}





void hashing_cerrado::imprimir_prueba(std::string nombre_tabla) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "REPORTE DE PRUEBA: " << nombre_tabla << std::endl;
    std::cout << "========================================" << std::endl;

    int vacios = 0;
    int ocupados = 0;
    int borrados = 0;
    int muestras_impresas = 0;

    for (int i = 0; i < capacidad; i++) {
        if (tabla[i].estado == VACIO) {
            vacios++;
        } else if (tabla[i].estado == BORRADO) {
            borrados++;
        } else if (tabla[i].estado == OCUPADO) {
            ocupados++;
            
            if (muestras_impresas < 5) {
                std::cout << "-> Indice de tabla [" << i << "] | Usuario: " << tabla[i].clave 
                          << " | Tweets: " << tabla[i].valor << std::endl;
                muestras_impresas++;
            }
        }
    }

    if (ocupados > 5) {
        std::cout << "    ... (hay " << (ocupados - 5) << " usuarios mas, omitidos para la prueba)" << std::endl;
    }

    std::cout << "\n--- ESTADISTICAS ---" << std::endl;
    std::cout << "* Capacidad total de la tabla: " << capacidad << std::endl;
    std::cout << "* Espacios ocupados (usuarios unicos): " << ocupados << std::endl;
    std::cout << "* Espacios vacios: " << vacios << std::endl;
    std::cout << "* Espacios borrados: " << borrados << std::endl;
    
    double factor_carga = ((double)ocupados / capacidad) * 100;
    std::cout << "* Factor de carga de la tabla: " << factor_carga << "%" << std::endl;
    std::cout << "========================================\n" << std::endl;
}