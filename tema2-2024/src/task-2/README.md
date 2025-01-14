## Task 2 - Requests

Pentru a intra în tribul lui, ratonul inginer îi cere lui Zoly să implementeze un sistem de login.

<div align="center">
    <img title="IDS" alt="IDS" src="../images/racoon.webp" width="300" height="300">
</div>

Se dau structurile simplificate ale unui login request:

```c
    struct creds {
        unsigned short passkey;
        char username[51];
    };

    struct request {
        unsigned char admin;
        unsigned char prio;
        struct creds login_creds;
    };
```

### Exercițiul 1

Pentru această parte a task-ului, aveți de implementat funcția `sort_requests()` în fișierul *subtask1.asm*.
Această funcție va simula sortarea tuturor request-urilor de login.

Pentru a ințelege mai bine cum funcționează un login request, vom explica mai jos ce înseamnă fiecare field al structurilor:

- `admin` ne spune dacă request-ul e făcut de un admin
- `prio` reprezintă prioritatea pe care o are un request
- `creds` reprezintă o structură de tip creds
- `passkey` reprezintă passkey-ul necesar ca login-ul să fie realizat
- `username` reprezintă un string de identificare unic pentru fiecare request

Pentru a sorta request-urile, stabilim următoarele reguli:

- Request-urile trebuie să fie sortate astfel încât request-urile făcute de admini să fie primele.
- Request-urile trebuie sortate după prioritate; prioritatea reprezentată de un număr mai mic e mai "mare".
- Request-urile cu aceeași prioritate trebuie apoi sortate alfabetic dupa username.

Sortarea se va face **in place**, adică vectorul `requests` prezentat mai jos va trebui, în urma apelului funcției, să fie sortat.

Antetul funcției este:

```c
void sort_requests(struct request *requests, int len);
```

Semnificația argumentelor este:

- **requests:** adresa de început a vectorului de request-uri
- **len:** numărul de request-uri

**Atenție!** Nu puteți folosi funcții externe pentru a sorta vectorul sau pentru a compara username-urile.


### Exercițiul 2

În continuarea exercițiului 1, acum trebuie să implementați funcția `check_passkeys()` în fișierul *subtask2.asm*.
Această funcție va verifica dacă passkey-ul din interiorul request-ului este unul corect, care nu e asociat unui hacker.

Suricata Zoly s-a prins de faptul că există și niște hackeri care vor să spargă sistemul, dar și de metoda prin care vor să păcălească login-ul.
Aceștia setează mereu primul bit și ultimul bit din `passkey`, iar pentru cei 14
biți rămași fac următoarele:

- pentru cei mai puțin semnificativi 7 biți: număr par de biți de `1`
- pentru cei mai semnificativi 7 biți: număr impar de biți de `1`

**Exemplu:**
```
    1000 1110 0110 0001 => hacker
```

Va trebui să puneți valorile obținute în vectorul `connected[]` prezentat mai jos.
Veți pune `0` pentru request-urile care nu sunt făcute de hackeri, `1` pentru cele făcute de hackeri.

Antetul funcției este:
```c
void check_passkeys(struct request *requests, int len, char *connected);
```

Semnificația argumentelor este:

- **requests:** adresa de început a vectorului de request-uri
- **len:** numărul de request-uri
- **connected:** adresa de început a vectorului pentru conexiuni

**Se garantează că toate valorile rămân în limitele tipurilor de date din definiția structurilor.**

#### **Observație**

Pentru exercițiul 2 se va folosi același vector folosit și la exercițiul 1. Nu puteți face
exercițiul 2 fără să rezolvați exercițiul 1, deoarece ordinea de parcurgere a request-urilor
trebuie să fie cea sortată.

---
