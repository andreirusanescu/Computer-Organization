## Task 3 - Treyfer (25p)

Tot cutreierând pe la porțile diferitor animale, Suricata noastră a devenit îngrijorată că datele din valiza sa ar putea să fie furate în timp ce doarme.
Din această cauză, ea dorește să cripteze tot ce deține folosind un [block cipher](https://en.wikipedia.org/wiki/Block_cipher),
însă vrea ca acesta să fie cât mai simplu.

<div align="center">
    <img title="IDS" alt="IDS" src="../images/suricata_crypto.webp" width="300" height="300">
</div>

După mai multe nopți nedormite de gândire, aceasta se decide asupra cifrului [Treyfer](https://en.wikipedia.org/wiki/Treyfer), care pentru a cripta / decripta un bloc de dimensiune **block_size** de date funcționează conform algoritmului descris în continuare.

Pentru **criptare**, pornim cu textul de `block_size` bytes pe care dorim să-i criptăm și cu o cheie secretă de `block_size` bytes.

Vom pleca cu o variabilă de 1 byte `t`, ce reprezintă starea
criptării.
Inițial, aceasta este egală cu primul byte al textului de criptat.

La fiecare rundă, pentru byte-ul de pe poziția `i` din blocul de criptat:
1. Se adună la `t` byte-ul de pe poziția `i` din **cheia secretă**.
2. Se substituie `t` cu corespondentul acesteia într-un [S-Box](https://en.wikipedia.org/wiki/S-box), definit [în scheletul temei](https://gitlab.cs.pub.ro/iocla/tema2-2024-private/-/blob/master/src/task-3/treyfer.asm#L4). Practic, înlocuim variabila `t` cu `sbox[t]`.
3. Se adună la `t` **următorul** byte din bloc. Dacă vorbim de ultimul byte, atunci următorul byte va fi cel de pe poziția 0.
4. Variabila `t` suferă [o rotație](https://github.com/systems-cs-pub-ro/iocla/tree/master/laborator/content/reprezentare-numere#4-rota%C8%9Bii) la stânga cu 1 bit.
5. Byte-ul de pe poziția `(i + 1) % block_size` din bloc va fi actualizat cu valoarea variabilei `t`.

Pentru **decriptare**, vom parcurge blocul **în sens invers**  la fiecare rundă
și vom efectua următoarele operații:
1. Luăm byte-ul `i` din bloc și adunăm la valoarea sa byte-ul `i` din cheia secretă.
2. Aplicăm același `S-box` pe byte-ul nou format. Notăm acest rezultat cu `top`.
3. Luăm byte-ul următor din bloc (poziția `(i + 1) % block_size`) și îi aplicăm o rotire la dreapta cu 1 bit. Notăm acest rezultat cu `bottom`.
4.  Byte-ul de pe poziția `(i + 1) % block_size` din bloc va fi actualizat cu diferența `bottom - top`.

Pentru o înțelegere mai bună a procesului de criptare / decriptare, Zoly s-a gândit
la următorul exemplu ce ilustrează **o rundă** de criptare pentru blocuri de **2 bytes**
(procedeul este similar pentru dimensiunea reală a blocului din task, **8 bytes**):

**Criptarea** textului "mo" cu cheia "da" folosind **S-Box**-ul din scheletul temei:

**Starea inițială (0)**
```
text:                   m      o
ascii_text:             109    111
cheia:                  d      a
ascii_cheie:            100    97
stare(t) = text[0]:     109
```

**Pasul 1**:
1. Adunăm byte-ul corespunzător al cheii (`'d' = 100`): `t = t + key[0] = 109 + 100 = 209`
2. Facem substituția t = sbox[t], în cazul nostru `t = sbox[209] = 135`
3. Adunăm următorul byte din bloc (`'o'` = 111): `t = 135 + 111 = 246`
4. Aplicăm o rotație la stânga pe `t`: `t = 246 <<< 1 = 237`
5. Actualizăm byte-ul de pe poziția `(i + 1) % block_size = (0 + 1) % 2 = 1` cu valoarea lui `t`, ajungând la următoarea stare:
    ```
    ascii_text: 109 237
    stare (t): 237
    ```

**Pasul 2**:
1. Adunăm byte-ul corespunzător al cheii (`'a' = 97`): `t = t + key[1] = 237 + 97 = 78` (luând în considerare overflow-ul pe 1 byte)
2. Facem substituția `t = sbox[t]`, în cazul nostru `t = sbox[78] = 169`
3. Adunăm următorul byte din bloc (`109`): `t = 169 + 109 = 22` (după overflow)
4. Aplicăm o rotație la stânga pe `t`: `t = 22 <<< 1 = 44`
5. Actualizăm byte-ul de pe poziția `(i + 1) % block_size = (1 + 1) % 2 = 0` cu valoarea lui `t`, ajungând la următoarea stare:
    ```
    ascii_text_criptat: 44 237
    stare (t): 44
    ```

Pentru **decriptare**, vom porni de la starea tocmai criptată și vom folosi aceeași cheie secretă:
```
ascii_text_criptat:             44     237
ascii_cheie:                    100    97
```

**Pasul 1**:
1. Luăm byte-ul de pe ultima poziție (în cazul nostru 1) și adunăm byte-ul corespondent al cheii (`'a' = 97`) => `237 + 97 = 78`
2. Întrucât `sbox[78] = 169`, notăm `top = 169`
3. Rotim byte-ul următor din bloc (în cazul nostru poziția 0, deoarece suntem la ultimul byte și nu mai există un byte următor) la dreapta: `44 >>> 1 = 22`, deci `bottom = 22`
4. Calculăm rezultatul `bottom - top = 22 - 169 = 109` (după underflow) și actualizăm byte-ul de pe poziția `(i + 1) % block_size = (1 + 1) % 2 = 0`, ajungând la starea:
    ```
    ascii: 109 237
    ```

**Pasul 2**:
1. Luăm byte-ul de pe prima poziție și adunăm byte-ul corespondent al cheii (`'d' = 100`) => `109 + 100 = 209`
2. Întrucât `sbox[209] = 135`, notăm `top = 135`
3. Rotim byte-ul următor din bloc (adică byte-ul 1) la dreapta: `237 >>> 1 = 246`, deci `bottom = 246`
4. Calculăm rezultatul `bottom - top = 246 - 135 = 111` și actualizăm byte-ul de pe poziția 0, ajungând la starea inițială:
    ```
    ascii_text_decriptat:  109  111
    text_decriptat:         m    o
    ```

Sarcina voastră este să o ajutați pe Zoly să implementeze metode de criptare și
decriptare pentru acest cifru, în cazul cu `10` runde de criptare și dimensiunea blocului de `8` bytes.
Mai precis, va trebui să implementați funcțiile:

```c
void treyfer_crypt(uint8_t text[8], uint8_t key[8]);
void treyfer_dcrypt(uint8_t text[8], uint8_t key[8]);
```
Aceste funcții vor modifica **in-place** blocul de criptat / decriptat. Astfel,
veți modifica în mod direct memoria din array-ul `text` dat ca parametru cu rezultatul criptării /  decriptării.

---
