# Structuri de Date - Tema 1

## Concepte

### Clasa `Racer`

- Folosesc skiplist-ul ca structura in care stochez clasamentul, avand drept
  date instante ale clasei `Racer`.
- Clasa `Racer` stocheaza urmatoarele date:
  - pozitia curenta in clasament (_currRank)
  - pozitia in ultimul clasament afisat (_prevRank)
  - numarul de puncte acumulate (_currPoints)
  - daca i s-a actualizat vreodata pozitia (_prevUpdated)
    - necesara pentru printarea diferentei de clasament
- Pentru a putea fi implementat skiplist-ul, clasa `Racer` implementeaza
  operatori de comparare:
  - Operatorul `<` este satisfacut astfel:
    - la puncte diferite: cel cu mai putine puncte va fi inaintea celui
      cu mai multe puncte
    - la puncte egale: cel cu ID-ul mai mare va fi inaintea celui
      cu ID-ul mai mic
- Pozitia in clasament este data de pozitia jucatorului in skiplist, deoarece
  lista este mereu ordonata conform regulii de mai sus.
- Intrucat ordonarea skiplist-ului se face dupa 2 chei (puncte si ID),
  creez un vector care memoreaza punctajul fiecarui ID (jucator): `racerScores`
  Este necesar pentru cautarea in skiplist.
- Campul `Racer::_currRank` este actualizat dupa citirea unei noi curse si
  refacerea listei prin actualizarea punctajului. Este setat in functie de
  pozitia din skiplist (functie: `redoRankings`)
- Metodele `getMin` si `getMax` returneaza cate un obiect `Racer` ce poate fi
  considerat de skiplist ca `-inf` sau `+inf` (capetele listei)
- Metoda `updatePrevRank` este apelata dupa fiecare printare a clasamentului
  pentru a salva pozitia curenta intrucat va fi nevoie de ea la printarea
  urmatorului clasament (diferenta de pozitie)
- Asadar, printarea skiplist-ului in ordine inversa (de la tail spre head) va
  rezulta in printarea clasamentului in ordine corecta.

### Structura `SkipList`

- Skiplist-ul este implementat conform informatiilor din cerinta.
- Fiecare nod din lista are 4 pointeri ce pointeaza la nodurile vecine:
  - Nodurile de tip `-inf` (`_head` si cele de mai sus) nu au legatura `_prev`
  - Nodurile de tip `+inf` (`_tail` si cele de mai sus) nu au legatura `_next`
  - In rest, toate celelalte noduri (adica cele introduse cu `insert`)
    au garantat noduri in stanga si dreapta (important la `find` si `insert`)
- Nodul `_head` pointeaza la cel mai de sus-stanga nod.
- Nodul `_tail` pointeaza la cel mai de sus-dreapta nod.
- Nodurile `_bottomHead` si `_bottomTail` pointeaza la nodurile de pe nivelele
  cel mai de jos, sub `_head`, respectiv `_tail`: ajuta la iterare.
- La initializare, in lista se va crea 1 nivel si vor fi create 2 noduri:
  `_head` (`_bottomHead`) si `_tail` (`_bottomTail`).
- La final, se sterg toate nodurile din lista fara a se reincerca refacearea
  legaturilor stanga-dreapta.
- Metoda `find` va cauta un nod in lista:
  - Necesita ca tipul de date T al listei sa fie ordonabil, adica sa
    implementeze operatorii <, <=, ==
  - Se putea relaxa aceasta regula intrucat toti ceilalti operatori de
    comparatie pot fi implementati doar prin < (sau >), dar asta ar fi
    complicat codul pentru implementarea skiplist-ului.
    Ex: `(a == b) = !(a < b || b < a)`
  - Daca exista: se va returna un pointer la nodul cautat de pe nivelul
    cel mai de jos.
  - Daca nu exista: se va returna un pointer la nodul de pe nivelul cel mai de
    jos dupa care poate fi adaugat nodul cautat astfel incat
    lista sa ramana ordonata. (de ajutor la `insert`).
- Metoda `insert` va insera un nod in lista:
  - Se cauta pozita unde acesta va fi adaugat prin apelarea metodei `find`.
  - Daca `find` arata ca nodul exista deja in lista, atunci se va opri
    executarea functiei si se va returna `nullptr`
    - nu se permite ca doua noduri diferite sa contina aceleasi date
  - Se creaza un nod, se fac legaturile corecte si se adauga dupa nodul
    returnat de `find`.
  - Cat timp `_randEngine` returneaza un numar impar se vor adauga noduri
    deasupra celui precedent (eventual se vor crea mai multe nivele daca
    acestea nu exista deja)
  - Se returneaza:
    - `nullptr` daca nu s-a putut adauga nodul (deja exista).
    - pointer la nodul cel mai de jos adaugat.
- Metoda `remove` sterge un nod din lista:
  - se sterge nodul curent si toate nodurile de deasupra acestuia;
  - se refac legaturile stanga-dreapta

### Structura `ResizableArray`

- Este o implementare modificata a ArrayList-ului din laboratul 2.

## Probleme intalnite

- Gasirea unui scop pentru a folosi skiplist in tema actuala.
- Am avut de ales intre a folosi skiplist-ul pentru a retine:
  - clasamentul (cheie: punctaj si ID)
    - avantaje:
      - parcurgerea skiplist-ului imi va genera clasamentul cerut
    - dezavantaje:
      - dupa fiecare modificare de punctaj trebuie sa verific daca nu trebuie
        refacute legaturile deoarece e posibil ca o coloana de noduri sa fie
        mutata in stanga/dreapta. Pentru a scapa cu mai putin cod, sterg
        si readaug nodul cu punctajul modificat. (TIMP)
      - trebuie sa retin un vector cu punctajele fiecarui ID pentru a putea
        cauta in skiplist deoarece am nevoie de ambele chei. (MEMORIE)
  - doar jucatorii (cheie: ID)
    - avantaje:
      - ?
    - dezavantaje:
      - ar fi un double linkedlist pe mai multe nivele
      - un arraylist ar necesita mai putina memorie si ar avea si random access
      - ordonarea pentru afisarea clasamentului ar fi greu de facut

- Nu vad la ce ar putea fi folosit skiplist-ul in contextul actual
  in detrimentul unui simplu ArrayList.
