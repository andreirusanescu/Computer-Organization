## Task 3 - Depth first search

Pentru ca Zoly să verifice că drumul pe care e transmis un pachet este legitim, se va realiza o cautare in topologia rețelei. Protocolul de comunicație garantează trimiterea pachetelor folosind algoritmul de căutare în adâncime. Astfel, știind sursa pachetului, putem găsi calea legitimă către toate celelalte gazde din rețea.

În acest task veți implementa algoritmul cunoscut de la SDA de cautare pe grafuri, depth first search.

### Algoritmul dfs

Precum sunteți deja familiari, depth first search implică cautare în adâncime pe grafuri, pornind de la un nod sursă dat. Algoritmul marchează nodul inițial ca fiind visitat și se aplică recursiv pe toți vecini lui. Algoritmul va continua, evitând nodurile marcate ca vizitate.

Luăm graful următor ca exemplu:

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/2/23/Directed_graph_no_background.svg/1280px-Directed_graph_no_background.svg.png" alt="err" width="300"/>

Dacă nodul sursă este 1, atunci o posibilă cautare dfs ar fi urmatoarea:

- Nodul 1 este vizitat.
- Nodul 2 este vizitat.
- Nodul 2 nu are vecini, ne întoarcem la nodul 1.
- Nodul 3 este vizitat.
- Nodul 2 a fost deja vizitat, vizitam nodul 4.
- Nodul 3 a fost deja vizitat, iar nodul 4 nu mai are vecini, deci ne întoarcem la nodul 3.
- Nodul 3 nu mai are vecini, ne întoarcem la nodul 1.
- Nodul 1 nu mai are vecini, se încheie căutarea.

Dfs-ul poate avea mai multe căutari valide, în funcție de ordinea în care alegem să vizităm vecinii. În cazul nostru, celelalte posibilități de căutare ar fi {1, 3, 2, 4} și {1, 3, 4, 2}.

### API

În cadrul task-ului, nodurile sunt reprezentate sub forma unui id de tip `uint32_t` (unsigned int).
Pentru a obține vecinii unui nod, se apelează funcția `expand` cu următoarea semnătură:

```c
neighbours_t expand(uint32_t node);
```

Aceasta returnează **adresa** unei structuri care conține vecinii nodului în felul următor:

```x86asm
struc neighbours_t
    .num_neighs resd 1  ; Numărul de vecini.
    .neighs resd 1      ; Adresa vectorul cu `num_neighs` vecini.
endstruc
```

### Cerința

Vi se cere să implementați **recursiv** funcția `dfs()` din fișierul `dfs.asm`, care are următoarea semnătură:

```c
void dfs(uint32_t node, neighbours_t *(*expand)(uint32_t node));
```

Aceasta primește ca parametru nodul sursă și adresa funcției `expand` și se cere să implementați algoritmul de dfs și să printați nodurile în momentul în care le viziați.

Printarea se va realiza prin apelara funcției `printf`.

### Depunctari

| Descriere | Depunctare |
| --- | --- |
| Folosire macro PRINTF32 | -10p |
| Soluție nerecursivă | -20p |
