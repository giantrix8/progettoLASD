#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dati e funzioni.h"

//rewind(fp);
static int controllo_registrazione(char *nick, utenti *utente){
	if (utente==NULL)
	{
		return 0;
	}
	int controllo;
	controllo=strcmp(nick,utente->nickname);
	if (controllo==0)
	{
		return 1;
	}
	else
	{
		if (controllo>0)
		{
			return controllo_registrazione(nick, utente->dx);
		}
		else 
		{
			return controllo_registrazione(nick, utente->dx);
		}
	}
}
static utenti* InserisciUtente (char *nick, char *password, short admin,float saldo, int *errore){
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
	tmp->admin=admin;
	tmp->saldo=saldo;
	tmp->dx=tmp->sx=NULL;
	*errore=0;
	return tmp;
}
static abbigliamento* InserisciCapo(char *nome, int disponbilita[], float prezzo, int* errore) {
	abbigliamento *tmp;
	tmp=(abbigliamento*)malloc(sizeof(abbigliamento));
	if (tmp==NULL)
	{
		printf ("non %c stato possibile allocare la memoria\n", e_accentata);
		*errore=-2;
		return NULL;
	}
	tmp->nome=calloc(dim2,sizeof(char));
	strcpy(tmp->nome,nome);
	tmp->prezzo=prezzo;
	for (int i=0;i<Ntaglie;i++){
		tmp->taglie[i]= disponbilita[i];
	}
	tmp->dx=tmp->sx=NULL;
	*errore=0;
	return tmp;
}
static utenti* RiempiAlberoUtente(char *nick,char *pass,short admin, utenti* radice,float saldo, int *errore){
	if(radice == NULL) {
		radice = InserisciUtente(nick,pass,admin,saldo,errore);
		return radice;
	} // radice = riempialvero(..., null, ...)
	else {
		if(strcmp(radice->nickname,nick)<0) {
			radice->sx = RiempiAlberoUtente(nick,pass,admin,radice->sx,saldo,errore);
		}
		else if(strcmp(radice->nickname,nick)>0) {
			radice->dx =  RiempiAlberoUtente(nick,pass,admin,radice->dx,saldo,errore);
		}
	}
}
static abbigliamento* RiempiAlberoAbbigliamento(char *nome,float prezzo,int *disponibilita, abbigliamento* radice, int *errore){
	if(radice == NULL) {
		radice = InserisciCapo(nome,disponibilita,prezzo,errore);
		return radice;
	}
	else {
		if(strcmp(radice->nome,nome)<0) {
			radice->sx = RiempiAlberoAbbigliamento(nome,prezzo,disponibilita,radice->sx,errore);
		}
		else if(strcmp(radice->nome,nome)>0) {
			radice->dx = RiempiAlberoAbbigliamento(nome,prezzo,disponibilita,radice->dx,errore);
		}
	}
}
 static FILE *apri_file(FILE *f1, int *errore, char *NomeFile){ //errore=-1 se non apre il file FUNZIONA
	*errore=0;
	f1=fopen(NomeFile, "r");
	if (f1==NULL)
 	{
 		*errore=-1;
         printf("il file non %c stato aperto",e_accentata);
        //gestisciErrore(errore);
	}
}
utenti *CopiaDaFileUtenti (FILE *F1, char *NomeFile, utenti *radice,int *errore){
	F1=apri_file (F1,errore,NomeFile);
	char nome[dim1], pass[dim1];
	int admin;
	float saldo;
	while(fscanf(F1,"%s%s%d%f",nome,pass,&admin,&saldo)>=4)
	{
        //printf("%s -- %s -- %d -- %6.2f\n",nome , pass, admin,saldo); FUNZIONA
		//radice=RiempiAlberoUtente(nome,pass,admin,radice,saldo,errore);
	}
	return radice;
}
abbigliamento *CopiaDaFileCapi(FILE *F1,char *NomeFile,abbigliamento*radice, int *errore ){
	char *nome;
	nome=calloc(dim1,sizeof(char));
	int disponibilita[Ntaglie];
	float prezzo;
	apri_file (F1,errore,NomeFile);
	if (*errore==0){
		while(!feof(F1))
		{
			fscanf(F1,"%s%f",nome,&prezzo);
			for (int i = 0; i < Ntaglie; i++) {
				fscanf(F1,"%d",disponibilita[i]);
			}
			radice=RiempiAlberoAbbigliamento(nome,prezzo,disponibilita,radice, errore);
		}
        free(nome);
		return radice;
	}
	else{
        free(nome);
		return NULL;
	}
}
void gestisciErrore (int *errore){
	int scelta;
	switch (*errore)
	{
		case 0: //nessun errore
		break;
		
		case -1: //file non aperto
		system ("cls");
		printf ("Errore di apertura del file per la lettura dei dati.\n");
		break;	
		
		case 1: //errore registrazione
		do{	
			system ("cls");
			printf ("\n il nickname inserito %c gi%c presente:\n",e_accentata,a_accentata);
			printf ("inserisci 1 se vuoi provare il login, 0 se vuoi riprovare la registrazione");
			scanf ("%d", &scelta);
		}while ((scelta!=0)&&(scelta!=1));
		*errore=scelta;
		break;
	}
}
/*utenti* registrazione(char *nick, char *password, utenti *utente, int *errore){
	*errore=0;
	if (controllo_registrazione(nick,utente)==1){
		*errore= 1;
		gestisciErrore(errore);
		if(*errore==0){
			printf ("inserisci il nuovo nickname: ");
			scanf ("%s",nick);
			printf ("inserisci la password: ");
			scanf ("%s",password);
			utente=registrazione(nick,password,utente,errore);
		}
		else if (*errore==1){
			tryLogin(nick,password,utenti,errore);
		}
	}
	else
	{
		utente=InserisciUtente (nick, password, utente);
	}
	return utente;
}
*/
