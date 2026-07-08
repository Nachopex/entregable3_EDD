/*
 * Implementacion de los metodos de la clase hashing_cerrado.
 */

#include "hashing_cerrado.h"
#include <cmath>

// Inicializa la tabla con la capacidad indicada, cantidad en 0 y reserva el espacio en el vector.
hashing_cerrado::hashing_cerrado(int cap, TipoProbing tipo, TipoClave tipo_clave) : tabla(1), capacidad(1), cantidad(0), tipo_probing(tipo), tipo_clave(tipo_clave){
    capacidad = siguientePotenciaDeDos(cap);
    tabla.assign(capacidad, Celda());
};

// Destructor vacio, la limpieza del contenedor std::vector se maneja automaticamente.
hashing_cerrado::~hashing_cerrado() {
    
};

// Inserta un elemento en la tabla o incrementa su conteo si ya existe.
// Resuelve las colisiones utilizando la tecnica de prueba lineal.
void hashing_cerrado::insertar(std::string clave) {
    
    // Si el factor de carga alcanza o supera el 70%, se redimensiona la tabla.
    if ((double)(cantidad + 1) / capacidad >= 0.70) {
        rehash();
    }

    // Se calcula la posicion inicial (intento 0) utilizando la capacidad
    // actual (post-rehash si lo hubo) y la estrategia de colisiones activa.
    int intento = 0;
    int posicion = obtenerPosicion(clave, intento);

    // Busca una celda disponible o la celda que ya contiene la clave.
    // Cada nuevo intento recalcula la posicion con la estrategia elegida,
    // para que linear, quadratic y double hashing realmente se diferencien.
    while (tabla[posicion].estado == OCUPADO) {
        if (tabla[posicion].clave == clave) {
            tabla[posicion].valor++;
            return;
        }

        intento++;
        posicion = obtenerPosicion(clave, intento);
    }

    // Se encontro una celda vacia, se asignan los datos del nuevo elemento.
    tabla[posicion].clave = clave;
    tabla[posicion].valor = 1;
    tabla[posicion].estado = OCUPADO;
    cantidad++;
}

// Calcula el indice base para una clave utilizando el metodo de la multiplicacion.
int hashing_cerrado::linear_probing(std::string clave, int intento) {
    // prueba lineal: h(k, i) = (h(k) + i) mod m. Avanza de a una posicion por cada
    //colision, recorriendo la tabla de forma circular y consecutiva.
    return (hashPrimario(clave) + intento) % capacidad;
};

// Calcula el indice base para una clave utilizando el metodo de la multiplicacion:
// h(k) = floor(m * (suma_ascii(k) * A mod 1)). Esta es la funcion hash primaria
// que utilizan las 3 estrategias de manejo de colisiones como punto de partida.
int hashing_cerrado::hashPrimarioUserId(std::string clave) {
    unsigned long long hash = 1469598103934665603ULL;
    for (unsigned char c : clave) {
        hash ^= c;
        hash *= 1099511628211ULL;
    }
    return (int)(hash % capacidad);
}

int hashing_cerrado::hashPrimarioScreenName(std::string clave) {
    unsigned long long hash = 5381ULL;
    for (unsigned char c : clave) {
        hash = ((hash << 5) + hash) + c;
    }
    return (int)(hash % capacidad);
}

int hashing_cerrado::hashPrimario(std::string clave) {
    if (tipo_clave == USER_ID) {
        return hashPrimarioUserId(clave);
    }
    return hashPrimarioScreenName(clave);
};

// Calcula una funcion hash secundaria, distinta e independiente de la primaria,
// utilizada exclusivamente por double hashing para definir el "salto" entre
// intentos.
// Como la capacidad de la tabla es siempre potencia de 2 (ver constructor y
// rehash()), basta con que el salto sea SIEMPRE IMPAR para garantizar que
// mcd(salto, capacidad) = 1. Cuando el salto y la capacidad son coprimos, la
// secuencia (h1(k) + i*salto) mod capacidad recorre las m celdas de la tabla
// sin repetir ninguna antes de completarla, evitando ciclos infinitos.
int hashing_cerrado::hashSecundarioUserId(std::string clave) {
    if (capacidad <= 1) {
        return 1;
    }

    unsigned long valor = 0;
    for (size_t i = 0; i < clave.size(); i++) {
        valor += (unsigned long)(clave[i]) * (i + 1) * 131;
    }
    return 1 + 2 * (int)(valor % (capacidad / 2));
}

int hashing_cerrado::hashSecundarioScreenName(std::string clave) {
    if (capacidad <= 1) {
        return 1;
    }

    unsigned long valor = 0;
    for (size_t i = 0; i < clave.size(); i++) {
        valor += (unsigned long)(clave[i]) * (i + 3) * 17;
    }
    return 1 + 2 * (int)(valor % (capacidad / 2));
}

int hashing_cerrado::hashSecundario(std::string clave) {
    if (tipo_clave == USER_ID) {
        return hashSecundarioUserId(clave);
    }
    return hashSecundarioScreenName(clave);
};

// Determina la posicion candidata a evaluar en el intento indicado, segun la
// estrategia de manejo de colisiones configurada para esta instancia.
int hashing_cerrado::obtenerPosicion(std::string clave, int intento) {
    switch (tipo_probing) {
        case QUADRATIC:
            return quadratic_probing(clave, intento);
        case DOUBLE:
            return double_hashing(clave, intento);
        case LINEAR:
            return linear_probing(clave, intento);
    }
    return linear_probing(clave, intento);
};

// Prueba cuadratica con formula triangular: h(k, i) = (h(k) + (i^2+i)/2) mod m,
// equivalente a usar c1=c2=0.5 en la formula general h(k,i)=h(k)+c1*i+c2*i^2.
// i^2+i es siempre par (producto de dos enteros consecutivos), por lo que la
// division entera es exacta y no se pierde precision.
//
// Se eligio especificamente esta variante porque, cuando la capacidad de la
// tabla es una potencia de 2 (ver constructor y rehash()), esta formula
// recorre exactamente las m celdas de la tabla sin repetir ninguna antes de
// completarla. Esto evita que la insercion cicle indefinidamente buscando
// una celda libre, algo que SI puede ocurrir con la formula clasica
// h(k)+i+i^2 cuando la capacidad no es un numero primo.
int hashing_cerrado::quadratic_probing(std::string clave, int intento) {
    long long paso = ((long long)intento * intento + intento) / 2;
    int posicion = (int)((hashPrimario(clave) + paso) % capacidad);
    return posicion;
};

// Doble hashing: h(k, i) = (h1(k) + i * h2(k)) mod m. Utiliza una segunda
// funcion hash (h2) para determinar el tamano del salto entre intentos, lo que
// elimina tanto el clustering primario como el secundario.
int hashing_cerrado::double_hashing(std::string clave, int intento) {
    int h1 = hashPrimario(clave);
    int h2 = hashSecundario(clave);
    return (h1 + intento * h2) % capacidad;
};

// Retorna la menor potencia de 2 mayor o igual a n (n >= 1).
int hashing_cerrado::siguientePotenciaDeDos(int n) {
    int potencia = 1;
    while (potencia < n) {
        potencia *= 2;
    }
    return potencia;
}

// Duplica el tamano de la tabla y reubica todos los elementos existentes 
// para mantener un rendimiento eficiente en las busquedas e inserciones.
void hashing_cerrado::rehash() {

    int vieja_capacidad = capacidad;
    std::vector<Celda> tabla_vieja = tabla;

    // Se duplica la capacidad total.
    capacidad = vieja_capacidad * 2; 
    
    // Se reinicia la tabla actual con la nueva capacidad y se llena de celdas vacias.
    tabla.assign(capacidad, Celda()); 
    cantidad = 0; 

    // Se recorre la tabla antigua para reinsertar los elementos validos en la nueva.
    for (int i = 0; i < vieja_capacidad; i++) {
        
        if (tabla_vieja[i].estado == OCUPADO) {
            
            /// Se recalcula la posicion en base a la nueva capacidad, utilizando
            // la misma estrategia de manejo de colisiones configurada para la tabla.
            int intento = 0;
            int posicion = obtenerPosicion(tabla_vieja[i].clave, intento);
            
            // Se resuelven las colisiones en la nueva tabla con la estrategia activa.
            while (tabla[posicion].estado == OCUPADO) {
                intento++;
                posicion = obtenerPosicion(tabla_vieja[i].clave, intento);
            }
            
            // Se traspasa el elemento y se actualiza el contador global.
            tabla[posicion] = tabla_vieja[i];
            cantidad++;
        }
    }
    
    std::cout << "[AVISO] Rehash ejecutado. Nueva capacidad: " << capacidad << std::endl;
}

// Imprime un reporte formateado con estadisticas de uso de la tabla 
// y una muestra pequena de los datos almacenados actualmente.
void hashing_cerrado::imprimir_prueba(std::string nombre_tabla) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "REPORTE DE PRUEBA: " << nombre_tabla << std::endl;
    std::cout << "========================================" << std::endl;

    int vacios = 0;
    int ocupados = 0;
    int borrados = 0;
    int muestras_impresas = 0;

    // Recorre la tabla completa para contabilizar el estado de cada celda.
    for (int i = 0; i < capacidad; i++) {
        if (tabla[i].estado == VACIO) {
            vacios++;
        } else if (tabla[i].estado == BORRADO) {
            borrados++;
        } else if (tabla[i].estado == OCUPADO) {
            ocupados++;
            
            // Imprime unicamente las primeras 5 muestras para no saturar la consola.
            if (muestras_impresas < 5) {
                std::cout << "-> Indice de tabla [" << i << "] | Usuario: " << tabla[i].clave 
                          << " | Tweets: " << tabla[i].valor << std::endl;
                muestras_impresas++;
            }
        }
    }

    // Indica de manera visual si existen mas datos que fueron omitidos en el muestreo.
    if (ocupados > 5) {
        std::cout << "    ... (hay " << (ocupados - 5) << " usuarios mas, omitidos para la prueba)" << std::endl;
    }

    // Despliega las metricas consolidadas y el estado general de la tabla.
    std::cout << "\n--- ESTADISTICAS ---" << std::endl;
    std::cout << "* Capacidad total de la tabla: " << capacidad << std::endl;
    std::cout << "* Espacios ocupados (usuarios unicos): " << ocupados << std::endl;
    std::cout << "* Espacios vacios: " << vacios << std::endl;
    std::cout << "* Espacios borrados: " << borrados << std::endl;
    
    // Calcula y muestra el factor de carga actual en formato de porcentaje.
    double factor_carga = ((double)ocupados / capacidad) * 100;
    std::cout << "* Factor de carga de la tabla: " << factor_carga << "%" << std::endl;
    std::cout << "========================================\n" << std::endl;
}

size_t hashing_cerrado::memoria_aproximada_bytes() {
    size_t memoria = sizeof(*this);
    memoria += tabla.capacity() * sizeof(Celda);
    return memoria;
}
