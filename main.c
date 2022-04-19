#include <stdio.h>
#include <stdlib.h>
#include "dati e funzioni.h"

int main()
{
 system ("color 1f");
 utenti *AlberoUtenti=NULL,*utente;
 FILE *Futenti,*Fcapi;
 abbigliamento *capo=NULL;
 char  fileUt[]="utenti.txt",fileAb[]="capi.txt",nome[2],lettura[]="r+";
 int errore=0,scelta;
 AlberoUtenti=CopiaDaFileUtenti(&Futenti,fileUt,AlberoUtenti,&errore,lettura);
 utente=AlberoUtenti;
 capo= CopiaDaFileCapi(&Fcapi,fileAb,capo,&errore,lettura);
 Stampa_Utenti(AlberoUtenti,&errore);
 Stampa_Capi(capo,scelta);
 utente=LoginRegistrazione(&Futenti,&AlberoUtenti,&errore);
 int tmp=0;
 if (utente->admin==1){
 tmp=1;
     printf("Ciao %s, vuoi continuare come admin o come cliente?\nInserisci 1 per continuare come un admin o 0 per continuare come cliente",utente->nickname);
     scanf("%d",&tmp);
     if (tmp==1) {
         MenuAdmin (utente,&capo,&Fcapi,&errore);
     }
     if ((utente->admin!=1)||(tmp!=1)){
     }
 }
 else{}
 return 0;
}

