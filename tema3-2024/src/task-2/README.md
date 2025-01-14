## Task 2 - Divide et impera

În acest task veți implementa doi algoritmi bine cunoscuți (sperăm noi): *quick sort* și *binary search*.

*Note:* ambele implementări trebuie să fie recursive, NU iterative.

Pentru a rula toate testele, rulați executabilul generat fără niciun argument. Pentru a rula un singur test, apăsați unul dintre keyword-urile `qsort` și `bsearch` ca prim argument, urmat de numărul testului.
De exemplu:

```bash
# rulează toate testele
$ ./checker

# rulează un singur test
$ ./checker qsort 3
$ ./checker bsearch 10
```

---

### Exercițiul 1

Pentru acest exercițiu aveți de implementat funcția `quick_sort()` în fișierul `subtask_1.asm`.

Această funcție are semnătura de mai jos. În urma rulării ei, numerele stocate în `buff` vor fi sortate crescător. `start` și `end` reprezintă indexul primului, respectiv ultimului element (începând de la 0).

```c
void quick_sort(int32_t *buff, uint32_t start, uint32_t end);
```

---

### Exercițiul 2

În acest exercițiu va trebui sa implementați funcția `binary_search()` în fișierul `subtask_2.asm`.

Această funcție va returna poziția elementului `needle` in cadrul array-ului `buff`. Căutarea va avea loc între elementele indexate `start` și `end` (inclusiv). Dacă `needle` nu se găsește în `buff`, funcția va returna `-1`. Este garantat faptul că elementele din `buff` sunt sortate crescător.

Observați în cadrul semnăturii funcției atributul [fastcall](https://gcc.gnu.org/onlinedocs/gcc-4.7.0/gcc/Function-Attributes.html).
Acesta va modifica calling convention-ul utilizat in `check.c`. Țineți cont de acest lucru când implementați funcția (și când faceți apelurile recursive).

```c
int32_t __attribute__((fastcall))
binary_search(int32_t *buff, uint32_t needle, uint32_t start, uint32_t end);
```
