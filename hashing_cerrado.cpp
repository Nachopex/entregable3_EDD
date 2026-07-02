/*
 * Implementacion de los metodos de la clase hashing_cerrado.
 */

#include "hashing_cerrado.h"

// Inicializa la tabla con la capacidad indicada, cantidad en 0 y reserva el espacio en el vector.
hashing_cerrado::hashing_cerrado(int cap) : capacidad(cap), cantidad(0), tabla(cap) {
    
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

    // Se calcula la posicion inicial utilizando la capacidad actual (post-rehash si lo hubo).
    int posicion = linear_probing(clave); 

    // Busca una celda disponible o la celda que ya contiene la clave.
    while (tabla[posicion].estado == OCUPADO) {
        
        // Si la clave ya existe, solo incrementa su valor (conteo) y finaliza.
        if (tabla[posicion].clave == clave) {
            tabla[posicion].valor++;
            return;
        }
        
        // Prueba lineal: avanza a la siguiente posicion de manera circular.
        posicion = (posicion + 1) % capacidad;
    }

    // Se encontro una celda vacia, se asignan los datos del nuevo elemento.
    tabla[posicion].clave = clave;
    tabla[posicion].valor = 1;
    tabla[posicion].estado = OCUPADO;
    cantidad++;
}

// Calcula el indice base para una clave utilizando el metodo de la multiplicacion.
int hashing_cerrado::linear_probing(std::string clave) {
    int valor = 0;
    
    // Suma los valores ASCII de todos los caracteres de la clave.
    for (char c : clave) {
        valor += c;
    }
    
    // Aplicacion del metodo de multiplicacion: h(k) = m * (k*A mod 1).
    double multiplicacion = valor * A;
    double parte_fraccionaria = multiplicacion - (int)multiplicacion;

    return (int)(capacidad * parte_fraccionaria);
    
};

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
            
            // Se recalcula la posicion en base a la nueva capacidad.
            int posicion = linear_probing(tabla_vieja[i].clave);
            
            // Se resuelven las colisiones en la nueva tabla mediante prueba lineal.
            while (tabla[posicion].estado == OCUPADO) {
                posicion = (posicion + 1) % capacidad;
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