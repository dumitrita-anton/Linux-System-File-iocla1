# Linux-System-File-iocla1
Anton Dumitrita 324CBa

Implemenatrea contine pe langa functiile deja definite in
scheletul de cod oferit, functii ajutatoare definite in 
prealabil care sa faciliteze scrierea codului necesar 
rularii aplicatiei.

Tema incepe cu functiile de alocare pentru structura
de fisiere si structura de directoare(alocDir si alocFile) ce primesc
ca parametru numele directorului(pe care il si atribuie campului name) si aloca memorie pentru fiecare camp
al structurii in parte.

In implementarea functiei touch m-am folosit la inceput de functia searchFile
care returneaza 0 daca fisierul cu numele name exista deja in directorul 
parint parent si 1 daca nu exista. Similar si pentru functia searchDir pe 
care am folosit-o mai departe in functia mkdir. In functia touch, daca
lista de fisiere a directorului parinte este goala, alocam memorie si legam celula
la inceputul listei, altfel mergem la sfarsitul listei si adaugam acolo celula de 
fisier noua. In acelasi mod este implemenatata si functia mkdir, cu diferenta ca adaugam celula in lista de 
directoare.

Functia ls parcurge lista de directoare, respectiv de fisiere din 
directorul parinte si afiseaza numele tuturor acestora

Functia rm sterge fiserul cu numele name din lista de fisiere a directorului 
parent. Mai intai in functie se cauta fisierul cu numele name in lista de directoare, daca nu se gaseste se afiseaza mesaj.
Daca in schimb fileCrt nu este NULL(nu am ajuns la sfarsitul listei cu cautarea) rupem legaturile si eliberam memoria. Pe acelasi
principiu functioneaza si rmdir, cu diferenta ca trebuie sterse si fisierele din directorul pe care il eliminam daca Dir->head_children_files != NULL

In functia cd am parcurs cu un pointer p lista de directoare a directorului curent pentru a gasi directorul cu numele name. Daca gasim directorul cautat directorul 
curent(target) devine directorul la care pointeaza p.

In functia pwd am salvat calea de la directorul curent(target) in ordine inversa, de la target la directorul radacina home, urmand ca apoi sa salvez 
in stringul path continutul vectorului de la ultimul element pana la primul.

Functia tree am implementat-o recursiv. In ajutorul afisarii spatiilor de dinaintea numelor am folosit functia printSpace.

Functia stop elibereaza toata memoria folosita de structuri.








