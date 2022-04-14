#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define e_accentata 138
#define a_accentata 160
#define dim1 20
#define Ntaglie 5

typedef struct abbigliamento {
	char nome[dim1];
	float prezzo;
	int taglie[Ntaglie];
	struct Albero_Abbigliamento* dx;
	struct Albero_Abbigliamento* sx;
}abbigliamento;

typedef struct utenti {
	char nickname[dim1];
	char password[dim1];
	float saldo;
	short admin; //quando admin vale 1 allora l'utente è un admin
	struct albero_Utenti* sx;
	struct albero_Utenti* dx;
}utenti;

utenti *CopiaDaFileUtenti (FILE *Futente, char*NomeFile, utenti *radice, int *errore);
abbigliamento *CopiaDaFileCapi(FILE *F1,char *NomeFile,abbigliamento*radice, int *errore);
//utenti *registrazione(char *nick, char *password, utenti *utente,int admin);
void gestisciErrore (int *errore);



