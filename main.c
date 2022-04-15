#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dati e funzioni.h"

int main()
{
 system ("color 0a");
 utenti *utente=NULL;
 FILE *Futenti,*Fcapi;
 abbigliamento *capo=NULL;
 char fileUt[]="utentae.txt",fileAb[]="capi.txt";
 int errore=0;
 utente=CopiaDaFileUtenti(&Futenti,fileUt,utente,&errore);
 return 0;
}

