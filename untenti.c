#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strutture.h"
#include <unistd.h>
static utenti* InserisciUtente (char *nick, char *password, float saldo, int *errore){ //funziona FUNZIONA
    utenti *tmp;
    tmp=(utenti*)malloc(sizeof(utenti));
    if (tmp==NULL)
    {
        printf ("non %c stato possibile allocare la memoria\n", e_accentata);
        *errore=-2;
        return NULL;
    }
    tmp->nickname=calloc(dim1,sizeof(char));
    tmp->password=calloc(dim1,sizeof(char));
    strcpy(tmp->nickname,nick);
    strcpy(tmp->password,password);
    tmp->saldo=saldo;
    tmp->dx=tmp->sx=NULL;
    *errore=0;
    return tmp;
}
static utenti* RiempiAlberoUtente(char *nick,char *pass, utenti* radice,float saldo, int *errore) {
    if (radice == NULL) {
        radice = InserisciUtente(nick, pass, saldo, errore);
        return radice;
    } else {
        if (strcmp(radice->nickname, nick) > 0) {
            radice->sx = RiempiAlberoUtente(nick, pass, radice->sx, saldo, errore);
        } else {
            radice->dx = RiempiAlberoUtente(nick, pass, radice->dx, saldo, errore);
        }
        return radice;
    }
}
static int ControlloUtente(char *nick, utenti *utente, char *password,short login){
    int controllo=0;
    if (utente == NULL) {
        return 1;
    }
    controllo = strcmp(utente->nickname,nick);
    if(controllo==0){
        if(login==0) {
            if (strcmp(utente->password,password)!=0) {
                return 2;
            }
        }
    }
    else if (controllo > 0) {
        if(login==1) {
            controllo = ControlloUtente(nick, utente->sx, NULL, 1);
        }
        else{
            controllo = ControlloUtente(nick, utente->sx,password,0);
        }
    }
    else {
        if(login==1) {
            controllo = ControlloUtente(nick, utente->dx, NULL, 1);
        }
        else{
            controllo = ControlloUtente(nick, utente->dx, password, 0);
        }
    }
    return controllo;
}//ritorna 1 se l'utente non è presente, 0 altrimenti, 2 se la password è errata
utenti *TrovaUtente(char *nick,utenti *utente){
    int controllo;
    if (utente == NULL) {
        return NULL;
    }
    controllo = strcmp(utente->nickname,nick);
    if(controllo==0){
        return utente;
    }
    else if (controllo > 0) {
        utente = TrovaUtente(nick, utente->sx);
    }
    else{
        utente = TrovaUtente(nick, utente->dx);
    }
    return utente;
}
utenti *LoginRegistrazione(utenti **radice,int *errore){
    char nick[dim1],password[dim1],ps[dim1];
    int scelta, controllo;
    float saldo;
    printf ("\n\nVuoi Registrarti o effettuare il Login? digita 1 per registrarti o 0 per effettuare il Login->");
    do{
        scanf("%d",&scelta);
        if ((scelta!=1)&&(scelta!=0)){
            printf ("\n!il numero inserito %c sbagliato, puoi inserire solo 1 oppure 0!",e_accentata);
        }
    }while ((scelta!=1)&&(scelta!=0));
    if (scelta==1){
        printf("\ninserisci il NickName-->");
        scanf("%s",nick);
        scelta= ControlloUtente(nick,*radice,NULL,1);
        if (scelta==0){
            printf ("il nickname %c gi%c presente nel nostro sito;\n",e_accentata, a_accentata);
            sleep(3);
            *radice=LoginRegistrazione(radice,errore);
        }
        else {
            do {
                controllo=0;
                printf("\nInserisci la password-->");
                scanf("%s", password);
                printf("\nInseriscila di nuovo--->");
                scanf("%s", ps);
                if (strcmp(password,ps) != 0) {
                    printf("\nLe due password non coincidono riprova.");
                    controllo = 1;
                }
            } while (controllo == 1);
            printf("\nDi quanto %c il tuo saldo iniziale?",e_accentata);
            scanf("%f",&saldo);
            *radice=RiempiAlberoUtente(nick,password, *radice, saldo,errore);
        }
    }
    else{
        do{
            printf("\ninserisci il NickName-->");
            if (nick==100){
                *errore=100;
            }
            scanf("%s", nick);
            do {
                printf("\nInserisci la password-->");
                scanf("%s", password);
                controllo = ControlloUtente(nick, *radice, password, 0);
                if (controllo == 2) {
                    printf("\nLa password inserita %c sbagliata:", e_accentata);
                }
            }while(controllo==2);
            if (controllo == 1) {
                printf("\nL'utente non %c presente:", e_accentata);
                printf("Digita 1 se vuoi riprovare il Login, 2 se vuoi Registrarti");
                scanf("%d", &controllo);
                if(controllo==2){
                    *radice= LoginRegistrazione(radice,errore);
                    controllo=0;
                }
            }
        }while(controllo!=0);
    }
    utenti* tmp=TrovaUtente(nick,*radice);
    return tmp;
}//BISOGNA IMPLEMENTARE IL CASO IN CUI L'UTENTE è UN'ADMIN