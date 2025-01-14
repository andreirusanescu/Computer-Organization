## Bonus - x64 assmebly

Acum ca Zoly și-a scris sistemul de detecție a intruziunilor și își poate apăra toți prietenii pe care și i-a facut în tema 2, se poate întoarce la marea ei pasiune: programarea funcțională.

Totodată, spre deosebire de echipa de PCLP2, ea a înțeles că în prezent se folosesc sisteme pe 64 de biți, iar cele pe 32 de biți sunt foarte rare. Astfel, ea dorește să implementeze funcțiile `map` și `reduce` în assembly pe 64 de biți si folosim și numere pe 64 de biți. Stiți de la [tema 1](https://gitlab.cs.pub.ro/iocla/tema-1-2024) ce sunt fiecare.

## Map

Antet map:

```c
void map(int64_t *destination_array, int64_t *source_array, int64_t array_size, int64_t(*f)(int64_t));
```

Antet functie ce poate fi folosita pentru map:

```c
int64_t map_func1(int64_t curr_elem);
```

Pseudocod map:

```text
map(dst, src, n, to_apply):
    for i de la 0 la n:
        dst[i] = to_apply(src[i])
```

## Reduce

Antet reduce:

```c
int64_t reduce(int64_t *destination_array, int64_t *source_array, int64_t array_size, int64_t accumulator_initial_value, int64_t(*f)(int64_t, int64_t));
```

Antet functie ce poate fi folosita pentru reduce:

```c
int64_t reduce_func1(int64_t accumulator, int64_t current_elem);
```

Pseudocod reduce:

```text
reduce(src, n, accumulator_initial, to_apply):
    acc = accumulator_initial
    for i de la 0 la n:
        acc = to_apply(acc, src[i])
    return acc
```
