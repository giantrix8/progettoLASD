#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dati e funzioni.h"
#include <unistd.h>

//rewind(fp);
static void Logo(){
    system("CLS");
    printf ("Benvenuto %c\n\t\t     %c%c%c%c%c%c%c%c%c%c%c%c",faccina,218,196,196,196,196,196,196,196,196,196,196,191);
    printf ("\nQuesta %c la boutique:%c %c LASD %c %c", e_accentata,179, 4, 4,179);
    printf ("\n\t %c           %c%c%c%c%c%c%c%c%c%c%c%c\n",32,192,196,196,196,196,196,196,196,196,196,196,217);
   }//FUNZIONA
static FILE *apri_file(FILE **f1, int *errore, char *NomeFile,char *modalita){ //errore=-1 se non apre il file FUNZIONA
    *errore=0;
    *f1=fopen(NomeFile, modalita);
    if (*f1==NULL)
    {
        *errore=-1;
        printf("il file non %c stato aperto",e_accentata);
        //gestisciErrore(errore);
    }
}//FUNZIONA
static void ScriviSuFileUtente(FILE **F1, utenti *radice){
    if (F1!=NULL) {
        if (radice != NULL) {
            //printf("%s\t%s\t%d\t%6.2f\n",radice->nickname,radice->password,radice->admin,radice->saldo);
            fprintf(*F1, "%s\t%s\t%d\t%6.2f\n", radice->nickname, radice->password, radice->admin, radice->saldo);
        } else {
            printf("\nNon %c stato passato nessun elemento", e_accentata);
        }
    }
}//FUNZIONA
static void ScriviSuFileCapi (FILE **F1, abbigliamento *radice, char *nomeFile){
    int errore=0;
    static eseguito=0;
    if (eseguito==0) {
        fclose(*F1);
        free(*F1);
        *F1 = apri_file(*F1, &errore, nomeFile, "w");
        eseguito=1;
    }
    if (errore==0) {
        if (radice != NULL) {
            //printf("%s\t%s\t%d\t%6.2f\n",radice->nickname,radice->password,radice->admin,radice->saldo);
            fprintf(*F1, "%d\t%s\t%s\t%d\t%6.2f\n", radice->id, radice->nome, radice->marca, radice->prezzo);
            for (int i = 0; i < Ntaglie; i++) {
                fprintf(*F1, "%d ", radice->taglie[i]);
            }
            fprintf(F1, "\n");
            ScriviSuFileCapi (F1,radice->sx,nomeFile);
            ScriviSuFileCapi (F1,radice->dx,nomeFile);
        }
    }
    else {
        printf("\nNon %c stato passato nessun elemento", e_accentata);
    }
}//FUNZIONA
static int ControlloUtente(char *nick, utenti *utente, char *password,short login){
    int controllo=0,Pcontrollo=2;
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
}//ritorna 1 se l'utente non è presente, 0 altrimenti
static utenti* InserisciUtente (char *nick, char *password, short admin,float saldo, int *errore){ //funziona FUNZIONA
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
}//FUNZIONA
static utenti* RiempiAlberoUtente(char *nick,char *pass,short admin, utenti* radice,float saldo, int *errore){ //FUNZIONA
    if(radice == NULL) {
        radice = InserisciUtente(nick,pass,admin,saldo,errore);
        //printf ("%s\n",radice->nickname);
        return radice;
    }
    else {
        if(strcmp(radice->nickname,nick)>0) {
            //printf ("scendo a sx %s\n", radice->nickname);
            radice->sx= RiempiAlberoUtente(nick,pass,admin,radice->sx,saldo,errore);
        }
        else {
            //printf ("scendo a dx %s\n", radice->nickname);
            radice->dx =  RiempiAlberoUtente(nick,pass,admin,radice->dx,saldo,errore);
        }
        return radice;
    }
} //FUNZIONA
static abbigliamento* InserisciCapo(int id, char *nome,char *marca, int disponbilita[], float prezzo, int *errore) {
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
    tmp->marca=calloc(dim2,sizeof(char));
    strcpy(tmp->marca,marca);
    tmp->id=id;
	tmp->prezzo=prezzo;
    printf ("\t%6.2f",prezzo);
    int i;
	for (i=0;i<Ntaglie;i++){
		tmp->taglie[i]= disponbilita[i];
	}
	tmp->dx=tmp->sx=NULL;
	*errore=0;
	return tmp;
}//FUNZIONA
static abbigliamento* RiempiAlberoAbbigliamento(int id,char *nome,char *marca,float prezzo,int disponibilita[], abbigliamento *radice, int *errore){
	if(radice == NULL) {
		radice = InserisciCapo(id,nome,marca,disponibilita,prezzo,errore);
        return radice;
	}
	else {
		if(radice->id<id) {
            //printf ("\nsx\n");
			radice->sx = RiempiAlberoAbbigliamento(id,nome,marca,prezzo,disponibilita,radice->sx,errore);
		}
		else {
            //printf ("\ndx\n");
			radice->dx = RiempiAlberoAbbigliamento(id,nome,marca,prezzo,disponibilita,radice->dx,errore);
		}
	}
    return radice;
}//FUNZIONA
static void *CercaCapo(abbigliamento *radice,int id, int *errore)
{
    if(radice== NULL) {
        *errore=1;
    }
    else {
        if ( radice->id==id) {
            *errore=0;
        }
        else if(radice->id<id) {
            radice=CercaCapo(radice->sx,id,errore);
        }
        else {
            radice=(radice->dx,id,errore);
        }
    }
    return radice;
}//FUNZIONA
static void ModificaDisponibilita (abbigliamento **capi,int *errore) {
    int scelta, id;
    printf("\nInserisci l'ID dell'articolo da aggiornare--->");
    scanf("%d", &id);
    char taglia[2];
    abbigliamento *tmp;
    tmp = *capi;
    tmp = CercaCapo(tmp, id, errore);
    if (*errore == 0) {
        do {
            printf("\nInserire la taglia della quale si vuole modificare la disponibilit%c;\nLe taglie vanno dalla XS alla XL\n----->",
                   a_accentata);
            scanf("%s", taglia);
            if ((strcmp(taglia, "XS") == 0) || (strcmp(taglia, "xs") == 0)) {
                printf("\nLe taglie XS dell'artcolo ID: %d  NOME: %s MARCA: %s sono:%d", tmp->id, tmp->nome, tmp->marca,tmp->taglie[0]);
                printf("\nInserisci la nuova quantit%c--->", a_accentata);
                scanf("%d", &tmp->taglie[0]);
            } else if ((strcmp(taglia, "S") == 0) || (strcmp(taglia, "s") == 0)) {
                printf("\nLe taglie S dell'artcolo ID: %d  NOME: %s MARCA: %s sono:%d", tmp->id, tmp->nome, tmp->marca,tmp->taglie[1]);
                printf("\nInserisci la nuova quantit%c--->", a_accentata);
                scanf("%d", &tmp->taglie[1]);
            } else if ((strcmp(taglia, "M") == 0) || (strcmp(taglia, "m") == 0)) {
                printf("\nLe taglie M dell'artcolo ID: %d  NOME: %s MARCA: %s sono:%d", tmp->id, tmp->nome, tmp->marca,tmp->taglie[2]);
                printf("\nInserisci la nuova quantit%c--->", a_accentata);
                scanf("%d", &tmp->taglie[2]);
            } else if ((strcmp(taglia, "L") == 0) || (strcmp(taglia, "l") == 0)) {
                printf("\nLe taglie L dell'artcolo ID: %d  NOME: %s MARCA: %s sono:%d", tmp->id, tmp->nome, tmp->marca,tmp->taglie[3]);
                printf("\nInserisci la nuova quantit%c--->", a_accentata);
                scanf("%d", &tmp->taglie[3]);
            } else if ((strcmp(taglia, "XL") == 0) || (strcmp(taglia, "xl") == 0)) {
                printf("\nLe taglie XL dell'artcolo ID: %d  NOME: %s MARCA: %s sono:%d", tmp->id, tmp->nome, tmp->marca, tmp->taglie[4]);
                printf("\nInserisci la nuova quantit%c--->", a_accentata);
                scanf("%d", &tmp->taglie[4]);
            } else {
                printf("\nLa taglia inserita non %c giusta", e_accentata);
            }
            printf("\nInserisci 0 se vuoi modificare un'altra taglia 1 altrimenti--->");
            scanf("%d", &scelta);
        } while (scelta == 0);
    } else {
        printf("\nL'ID %d non corrisponde a nessun articolo", id);
    }
    printf("\nInserisci 1 se vuoi modificare la taglia di un altro articolo");
    scanf("%d", &scelta);
    if (scelta == 1) {
        ModificaDisponibilita(capi, errore);
    }
}//FUNZIONA
//UTENTI----------------------------------------------------------------------------------------------
utenti *CopiaDaFileUtenti (FILE **F1, char *NomeFile, utenti *radice,int *errore, char *modalita){ //FUNZIONA
	*F1=apri_file (F1,errore,NomeFile, modalita);
	char nome[dim1], pass[dim1];
	int admin;
	float saldo;
    if (*errore==0) {
        while (fscanf(*F1, "%s%s%d%f", nome, pass, &admin, &saldo) == 4) {
            //printf("%s -- %s -- %d -- %6.2f\n",nome , pass, admin,saldo); //FUNZIONA
            radice = RiempiAlberoUtente(nome, pass, admin, radice, saldo, errore);
        }
    }
	return radice;
}//funziona
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
}//funziona
utenti *LoginRegistrazione(FILE **F1,utenti **radice,int *errore){
    char nick[dim1],password[dim1],ps[dim1];
    int scelta, controllo;
    short admin;
    float saldo;
    Logo();
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
            *radice=LoginRegistrazione(F1,radice,errore);
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
            printf("\nInserisci 1 se sei un admin, 0 altrimenti-->");
            scanf("%d", &admin);
            printf("\nDi quanto %c il tuo saldo iniziale?",e_accentata);
            scanf("%f",&saldo);
            *radice=RiempiAlberoUtente(nick,password,admin, *radice, saldo,errore);
        }
    }
    else{
        do{
            printf("\ninserisci il NickName-->");
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
                    *radice= LoginRegistrazione(F1,radice,errore);
                    controllo=0;
                }
            }
        }while(controllo!=0);
    }
    utenti* tmp=TrovaUtente(nick,*radice);
    if (scelta==1){
        ScriviSuFileUtente(F1,tmp);
    }
    return tmp;
}//FUNZIONA
void Stampa_Utenti(utenti *radice, int scelta){ //FUNZIONA
    if(radice!=NULL) {
        if (scelta==0) {
            Stampa_Utenti(radice->sx, scelta);
            printf("%s\t", radice->nickname);
            if (radice->admin == 1) {
                printf("Admin\t");
            } else { printf("Utente\t"); }
            printf("%6.2f%c\n", radice->saldo,dollaro);
            Stampa_Utenti(radice->dx, scelta);
        }
        else{
            Stampa_Utenti(radice->dx, scelta);
            printf("%s\t", radice->nickname);
            if (radice->admin == 1) {
                printf("Admin\t");
            } else { printf("Utente\t"); }
            printf("%6.2f%c\n", radice->saldo,dollaro);
            Stampa_Utenti(radice->sx, scelta);
        }
    }
}//funziona
//ABBIGLIAMENTO---------------------------------------------------------------------------------------------
abbigliamento *CopiaDaFileCapi(FILE **F1,char *NomeFile,abbigliamento*radice, int *errore, char*modalita ) {
    *F1 = apri_file(F1, errore, NomeFile, modalita);
    char *nome, *marca;
    nome = calloc(dim1, sizeof(char));
    marca = calloc(dim1, sizeof(char));
    int id, disponibilita[Ntaglie];
    float prezzo;
    if (*errore == 0) {
        while (!feof(*F1)) {
            fscanf(*F1, "%s%s%d%f", nome, marca, &id, &prezzo);
            for (int i = 0; i < Ntaglie; i++) {
                fscanf(*F1, "%d", &disponibilita[i]);
            }
            radice = RiempiAlberoAbbigliamento(id, nome, marca, prezzo, disponibilita, radice, errore);
        }
        free(nome);
        free(marca);
        return radice;
    }//funziona
}//FUNZIONA
void MenuAdmin (utenti *utente,abbigliamento **capi,FILE **Fcapi, int *errore){
    int scelta,opzione,id,taglie[Ntaglie];
    char nome[dim2],marca[dim2];
    float prezzo;
    Logo();
    printf("Benevnuto %s, sei nel sistema degli admin\nCosa vuoi fare?\n", utente->nickname);
    printf("\nInserisci 1 per aggiungere un nuovo prodotto:\n");
    printf("Inserisci 2 per aggiornare la disponibilit%c dei prodotti\n",a_accentata);
    printf("Inserisci 0 per concludere le operazioni\n--->");
    scanf("%d", &scelta);
    if (scelta == 1) {
        printf("Inserisci:\n");
        printf("NOME------>");
        scanf("%s",nome);
        printf("\nMARCA----->");
        scanf("%s",marca);
        printf("\nID-------->");
        scanf("%d",&id);
        printf("\nPREZZO---->");
        scanf("%f",&prezzo);
        printf("\nTAGLIE: XS | S | M | L | XL\n");
        int i;
        for (i=0;i<Ntaglie;i++){
             scanf("%d",&taglie[i]);
        }
        do {
            opzione=0;
            *capi = RiempiAlberoAbbigliamento(id, nome, marca,prezzo,taglie,*capi,errore);
            if (*errore == -2) {
                printf("\nInserisci 1 per riprovare ");
                scanf("%d", &opzione);
            }
            else if(*errore==0){
                printf("\nCapo salvato correttamente");
                opzione=0;
            }
        }while(opzione!=0);
    }
    else if (scelta == 2){
            ModificaDisponibilita(capi,errore);
        }
    else if (scelta==0){
        return;
    }
        MenuAdmin(utente,capi,Fcapi,errore);
}//FUNZIONA
void Stampa_Capi(abbigliamento *radice, int scelta){
    if(radice!=NULL) {
        if (scelta==0) {
            Stampa_Capi(radice->sx, scelta);
            printf("ID=%d\t",radice->id);
            printf("NOME=%s\t", radice->nome);
            printf("MARCA=%s\t",radice->marca);
            printf("PREZZO:%6.2f%c\n", radice->prezzo,dollaro);
            Stampa_Capi(radice->dx, scelta);
        }
        else{
            Stampa_Capi(radice->dx, scelta);
            printf("ID=%d\t",radice->id);
            printf("NOME=%s\t", radice->nome);
            printf("MARCA=%s\t",radice->marca);
            printf("PREZZO:%6.2f%c\n", radice->prezzo,dollaro);
            Stampa_Capi(radice->sx, scelta);
        }
    }
}//funziona