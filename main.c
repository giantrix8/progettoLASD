#include <stdio.h>
#include <stdlib.h>
#include "dati e funzioni.h"

int main() {
    system("color 0e");
    utenti *AlberoUtenti = NULL, *utente;
    FILE *Futenti, *Fcapi, *Fattesa;
    abbigliamento *capo = NULL;
    attesa *Attesa = NULL;
    char fileUt[] = "utenti.txt", fileAb[] = "capi.txt", lettura[] = "r+", fileAttesa[] = "attesa.txt";
    int errore = 0;
    AlberoUtenti = CopiaDaFileUtenti(&Futenti, fileUt, AlberoUtenti, &errore, lettura);
    utente = AlberoUtenti;
    capo = CopiaDaFileCapi(&Fcapi, fileAb, capo, &errore, lettura);
    Attesa = CopiaDaFileAttesa(&Fattesa, fileAttesa, Attesa, &errore, lettura);
    utente = LoginRegistrazione(&Futenti, &AlberoUtenti, &errore);
    int tmp = 0;
    if (utente->admin == 1) {
        tmp = 1;
        printf("Ciao %s, vuoi continuare come admin o come cliente?\nInserisci 1 per continuare come un admin o 0 per continuare come cliente",utente->nickname);
        scanf("%d", &tmp);
        if (tmp == 1) {
            MenuAdmin(utente, &capo, &errore);
        }
    }
    if ((utente->admin != 1) || (tmp != 1)) {
        MenuUtente(&utente, &capo, &errore, &Attesa);
    }
    //Stampa_Utenti(AlberoUtenti);
    //Stampa_Capi(capo);
    ScriviSuFileUtente(&Futenti,AlberoUtenti,fileUt);
    ScriviSuFileCapi (&Fcapi,capo,fileAb);
    ScriviSuFileAttesa(&Fattesa, Attesa ,fileAttesa);
    AlberoUtenti=DeallocaUtenti(AlberoUtenti);
    AlberoUtenti=utente=NULL;
    capo= DeallocaCapi(capo);
    capo=NULL;
    system("PAUSE");
}

