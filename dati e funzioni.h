#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define e_accentata 138 
#define a_accentata 160
#define u_accentata 151
#define ltg 3
#define faccina 1
#define fiamma 5
#define dim1 20	//grandezza dei campi nickname e password nella struct utenti
#define dim2 20 //grandezza del campo nome nella struct abbigliamento
#define Ntaglie 5
#define dollaro 36

typedef struct abbigliamento {
	char 	*nome;
    char    *marca;
    int     id;
	float 	prezzo;
	int 	taglie[Ntaglie];//nel vettore saranno presenti le dimensioni delle varie tagli in ordine crescente
	struct 	Albero_Abbigliamento* dx;
	struct 	Albero_Abbigliamento* sx;
}abbigliamento;

typedef struct utenti {
	char 	*nickname;
	char 	*password;
	float 	saldo;
	short 	admin; //quando admin vale 1 allora l'utente è un admin
	struct 	albero_Utenti* sx;
	struct 	albero_Utenti* dx;
}utenti;

typedef struct UtAttesa{ //sarà una lista FIFO
    char *nickname;
    struct UtAttesa *next;
}UtAttesa;

typedef struct attesa{ //albero di attesa che fa corrispondere ad ogni articolo una lista di attesa
    int id;
    struct UtAttesa *lista;
    struct attesa *dx;
    struct attesa *sx;
}attesa;

utenti *CopiaDaFileUtenti (FILE **Futente, char*NomeFile, utenti *radice, int *errore, char *modalita);
abbigliamento *CopiaDaFileCapi(FILE **F1,char *NomeFile,abbigliamento*radice, int *errore, char *modalita);
void Stampa_Utenti(utenti *utente);
void Stampa_Capi(abbigliamento *radice);
utenti *LoginRegistrazione(FILE **F1,utenti **radice,int *errore);
utenti *TrovaUtente(char *nick,utenti *utente);
void MenuAdmin (utenti *utente,abbigliamento **capi, int*errore);
void MenuUtente (utenti **utente, abbigliamento **capi,int *errore);
