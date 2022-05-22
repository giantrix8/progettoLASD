#define LenC 20
#define e_accentata 138
#define a_accentata 160
#define u_accentata 151
#define dollaro 36
#define dim1 20
#define CodAdmin 100


typedef struct  AlberoCitta{
    struct Citta *citta;
    struct AlberoCitta *dx;
    struct AlberoCitta *sx;
}AlberoCitta;

typedef struct Citta {
    char nome[LenC];
    short aereo;
    short treno;
    struct ListaNext *ListaAereo;
    struct ListaNext *ListaTreno;
}Citta;
typedef struct ListaNext{
    char citta[LenC];
    struct ListaNext *next;
}ListaNext;

typedef struct utenti {
    char 	*nickname;
    char 	*password;
    float 	saldo;
    struct 	albero_Utenti* sx;
    struct 	albero_Utenti* dx;
}utenti;

utenti *LoginRegistrazione(utenti **radice,int *errore);