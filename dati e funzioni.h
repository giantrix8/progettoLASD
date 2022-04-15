#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define e_accentata 138 
#define a_accentata 160
#define dim1 20	//grandezza dei campi nickname e password nella struct utenti
#define dim2 50 //grandezza del campo nome nella struct abbigliamento
#define Ntaglie 5

typedef struct abbigliamento {
	char 	*nome;
	float 	prezzo;
	int 	taglie[Ntaglie];//nel vettore saranno presenti le dimensioni delle varie tagli in ordine crescente
	struct 	Albero_Abbigliamento* dx;
	struct 	Albero_Abbigliamento* sx;
}abbigliamento;

typedef struct utenti {
	char 	*nickname;
	char 	*password;
	float 	saldo;
	short 	admin; //quando admin vale 1 allora l'utente � un admin
	struct 	albero_Utenti* sx;
	struct 	albero_Utenti* dx;
}utenti;

utenti *CopiaDaFileUtenti (FILE *Futente, char*NomeFile, utenti *radice, int *errore);
abbigliamento *CopiaDaFileCapi(FILE *F1,char *NomeFile,abbigliamento*radice, int *errore);
//utenti *registrazione(char *nick, char *password, utenti *utente,int admin);
void gestisciErrore (int *errore);

