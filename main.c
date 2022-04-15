#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dati e funzioni.h"

int main()
{
 system ("color 0a");
 utenti *utente;
 FILE *Futenti=NULL,*Fcapi;
 abbigliamento *capo;
 char fileUt[]="utenti.txt",fileAb[]="capi.txt";
 int *errore=0;
 utente=(utenti*)malloc(sizeof(utenti));
 utente=CopiaDaFileUtenti(Futenti,fileUt,utente,errore);
 return 0;
}

