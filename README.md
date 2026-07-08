# entregable3_EDD

compilar

```bash
 g++ -std=c++17 Lista_doblemente_enlazada.cpp hashing_cerrado.cpp hashing_abierto.cpp  main.cpp -o main
```

ejecutar

```bash
./main hashing_cerrado
```

o

```bash
./main hashing_abierto
```

o

```bash
./main unordered_map
```

benchmark experimental

```bash
./main benchmark
```

Ese modo genera `resultados_experimentos.csv` con mediciones por repeticion,
promedios y desviaciones estandar impresas en consola para comparar:

- `hashing_abierto`
- `hashing_cerrado_linear`
- `hashing_cerrado_quadratic`
- `hashing_cerrado_double`
- `unordered_map`
