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
static void Scrivi (FILE **F1,char *NomeFile){
    fclose(*F1);
    char modalita[]="w";
    int errore=0;
    *F1=apri_file(F1,&errore,NomeFile,modalita);
    if(errore==-1){
        exit(1);
    }
}
static void ScriviSuFileUt(FILE **F1,utenti *radice) {
    if (radice==NULL){return;}
    //printf("%s\t%s\t%d\t%6.2f\n", radice->nickname, radice->password, radice->admin, radice->saldo);
    fprintf(*F1, "%s\t%s\t%d\t%6.2f\n", radice->nickname, radice->password, radice->admin, radice->saldo);
    ScriviSuFileUt(F1,radice->sx);
    ScriviSuFileUt(F1,radice->dx);
} //funziona
static void ScriviSuFileLista(FILE **F1,UtAttesa *lista){
    if (lista==NULL){
        return;
    }
    else{
        fprintf(*F1,"%s\t",lista->nickname);
        ScriviSuFileLista(F1,lista->next);
    }
}
static void ScriviSuFileAtt(FILE **F1,attesa *radice) {
    if (radice==NULL){return;}
    //printf("%s\t%s\t%d\t%6.2f\n", radice->nickname, radice->password, radice->admin, radice->saldo);
    for (int i = 0; i < Ntaglie; ++i) {
        if(radice->listaAttesa[i].next!=NULL){
            fprintf(*F1, "%d\t%d\t",radice->id,i);
            ScriviSuFileLista(F1,radice->listaAttesa[i].next);
            fprintf(*F1,"0\n");
        }
        ScriviSuFileAtt(F1,radice->sx);
        ScriviSuFileAtt(F1,radice->dx);
    }
} //funziona
void ScriviSuFileAttesa(FILE **F1, attesa *radice,char *NomeFile){
    Scrivi(F1,NomeFile);
    if (radice == NULL) {
        printf("\nNon %c stato passato nessun elemento", e_accentata);
    }
    else {
        ScriviSuFileAtt(F1,radice);
    }
    fclose(*F1);
}
void ScriviSuFileUtente(FILE **F1, utenti *radice,char *NomeFile){
    Scrivi(F1,NomeFile);
        if (radice == NULL) {
            printf("\nNon %c stato passato nessun elemento", e_accentata);
        }
        else {
            ScriviSuFileUt(F1,radice);
        }
    fclose(*F1);
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
static void RicaricaSaldo(utenti **utente){
    float ricarica;
    utenti *radice;
    radice=*utente;
    do {
        printf("\nDi quanto vuoi ricaricare il tuo saldo?--->");
        scanf("%f", &ricarica);
        if (ricarica < 0) {
            printf("\nPuoi caricare solo quantit%c positive", a_accentata);
            printf("\nDigita -1 per ricaricare oppure 0 per tornare al men%c", u_accentata);
            scanf("%f", &ricarica);
        }
    } while (ricarica < 0);
    radice->saldo = radice->saldo + ricarica;
}//funziona
static void ScriviSuFileAb(FILE **F1,abbigliamento *radice) {
    if (radice==NULL){return;}
    //printf("%s\t%s\t%d\t%6.2f\n", radice->nome, radice->marca, radice->id, radice->prezzo);
    fprintf(*F1, "\n%s\t%s\t%d\t%6.2f\t", radice->nome, radice->marca, radice->id, radice->prezzo);
    for (int i=0;i<Ntaglie;i++){
        fprintf(*F1,"%d ",radice->taglie[i]);
    }
    ScriviSuFileAb(F1,radice->sx);
    ScriviSuFileAb(F1,radice->dx);
} //funziona
void ScriviSuFileCapi (FILE **F1, abbigliamento *radice, char *NomeFile){
    fclose(*F1);
    char modalita[]="w";
    int errore=0;
    *F1=apri_file(F1,&errore,NomeFile,modalita);
    if (radice == NULL) {
        printf("\nNon %c stato passato nessun elemento", e_accentata);
    }
    else {
        ScriviSuFileAb(F1,radice);
    }
    fclose(*F1);
}//FUNZIONA
//-----------------------------------------------------------------------------------------------------
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
static abbigliamento *CercaCapo(abbigliamento *radice,int id, int *errore){
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
            radice=CercaCapo(radice->dx,id,errore);
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
            printf("\nInserisci 0 se vuoi modificarne un'altra taglia 1 altrimenti--->");
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
//ATTESA----------------------------------------------------------------------------------------------
static UtAttesa *InserisciUtenteAttesa (UtAttesa *testa,char nick[],int *errore,int taglia){
        //printf ("hgjk");
        if(strcmp(nick,"0")==0){
            *errore=0;
        }
        else if (testa == NULL) {
            testa = (UtAttesa *) malloc(sizeof(UtAttesa));
            if (testa == NULL) {
                printf("non %c stato possibile allocare la memoria\n", e_accentata);
                *errore = -2;
                return NULL;
            }
            testa->taglia=taglia;
            testa->nickname= calloc(dim1,sizeof(char));
            strcpy(testa->nickname,nick);
            //printf(" %s--%s \n",nick,testa->nickname);
            testa->next = NULL;
        }
        else {
            testa->next = InserisciUtenteAttesa(testa->next, nick, errore,taglia);
        }
    return testa;
}//FUNZIONA
static attesa *CreaNodoAttesa (int id, int taglia, int *errore,char *nickname, FILE **F1){
    attesa *tmp;
    tmp=(attesa *)malloc(sizeof(attesa));
    *errore=1;
    if (tmp==NULL)
    {
        printf ("non %c stato possibile allocare la memoria\n", e_accentata);
        *errore=-2;
        return NULL;
    }
    tmp->id=id;
    tmp->listaAttesa[taglia].next=NULL;
        char nick[dim1];
        while (*errore != 0) {
            fscanf(*F1, "%s", nick);
            tmp->listaAttesa[taglia].next = InserisciUtenteAttesa(tmp->listaAttesa[taglia].next, nick, errore,taglia);
            if (*errore == -2) {
                return NULL;
            }
    }
    tmp->dx=tmp->sx=NULL;
    return tmp;
}//FUNZIONA
static attesa *InserimentoAttesa(attesa *radice,int id, int taglia,int *errore,FILE **F1){
    if(radice == NULL) {
        radice = CreaNodoAttesa(id,taglia,errore,NULL,F1);
        return radice;
    }
    else {
        if(radice->id<id) {
            //printf ("\nsx\n");
            radice->sx = InserimentoAttesa(radice->sx,id,taglia,errore,F1);
        }
        else if (radice->id>id){
            //printf ("\ndx\n");
            radice->dx = InserimentoAttesa(radice->dx,id,taglia,errore,F1);
        }
    }
    return radice;
} //FUNZIONA
static attesa *cercaNodoA (attesa *radice,int id){
    if(radice == NULL) {
        return NULL;
    }
    else if (radice->id==id) {
        //printf("radice %d ",radice->id);
        return radice;
    }
    else{
            if (radice->id < id) {
                //printf ("\nsx\n");
                radice = cercaNodoA(radice->sx, id);
            }
            else{
                //printf ("\ndx\n");
                radice = cercaNodoA(radice->dx, id);
            }
    }
    return radice;
}//FUNZIONA
static int Conta(UtAttesa *testa){
    if (testa==0){
        return 0;
    }
    else {
        return 1+Conta(testa->next);
    }
}//funziona
static attesa *ContaAttesa(attesa *radice,int id, int taglia, int *conteggio){
    *conteggio=0;
    radice=cercaNodoA(radice,id);
    //printf("radice %d ",radice->id);
    if (radice!=NULL) {
        *conteggio=Conta(radice->listaAttesa[taglia].next);
    }
    return radice;
}//FUNZIONA
attesa *CopiaDaFileAttesa(FILE **F1,char *NomeFile,attesa *radice,int *errore,char *modalita){
    *F1 = apri_file(F1, errore, NomeFile, modalita);
    if (*errore == 0) {
        int id, taglia;
        while (!feof(*F1)) {
            fscanf(*F1, "%d%d", &id, &taglia);
            radice = InserimentoAttesa(radice, id, taglia, errore, F1);
        }
    }
    return radice;
} //FUNZIONA//
attesa *DeallocaAttesa (attesa *radice){
    if(radice!=NULL) {
        if((radice->sx==NULL) && (radice->dx==NULL)){
            free(radice);
        }
        DeallocaUtenti(radice->sx);
        DeallocaUtenti(radice->dx);
    }
    return radice;
}
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
    return tmp;
}//FUNZIONA
void Stampa_Utenti(utenti *radice){ //FUNZIONA
    if(radice!=NULL) {
            Stampa_Utenti(radice->sx);
            printf("%s\t", radice->nickname);
            if (radice->admin == 1) {
                printf("Admin\t");
            } else { printf("Utente\t"); }
            printf("%6.2f%c\n", radice->saldo,dollaro);
            Stampa_Utenti(radice->dx);
    }
}//funziona
utenti *DeallocaUtenti (utenti *radice){
    if(radice!=NULL) {
        if((radice->sx==NULL) && (radice->dx==NULL)){
            free(radice);
        }
        DeallocaUtenti(radice->sx);
        DeallocaUtenti(radice->dx);
    }
    return radice;
}
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
void Stampa_Capi(abbigliamento *capo){
    abbigliamento *radice;
    radice=capo;
    if(radice!=NULL) {
            Stampa_Capi(radice->sx);
            printf("ID=%d\t",radice->id);
            printf("NOME=%s\t", radice->nome);
            printf("MARCA=%s\t",radice->marca);
            printf("PREZZO:%6.2f%c\n", radice->prezzo,dollaro);
            Stampa_Capi(radice->dx);
    }
}//funziona
static UtAttesa *InserisciAttesa(UtAttesa *lista, char *nickname){
    if(lista==NULL){
        lista=(UtAttesa*)malloc(sizeof (UtAttesa));
        lista->next=NULL;
        lista->nickname=nickname;
    }
    else{
        lista->next=InserisciAttesa(lista->next ,nickname);
    }
    return lista;
}
static void Transazione(utenti **utente,abbigliamento**vestito,int taglia_indice, attesa **Attesa, int id){
    attesa *tmp;
    tmp=*Attesa;
    abbigliamento *articolo;
    articolo=*vestito;
    utenti *cliente;
    cliente=*utente;
    int conto;
    tmp=ContaAttesa(tmp, id, taglia_indice,&conto);
    if (articolo->taglie[taglia_indice] < 0) {
        printf("\nOps sembra che la taglia dell'articolo selezionato sia esaurita.");
        printf("\nVerrai inserito nella lista d'attesa");
        tmp->listaAttesa[taglia_indice].next = InserisciAttesa(tmp->listaAttesa[taglia_indice].next,cliente->nickname);
    }
    else if (conto >= articolo->taglie[taglia_indice]) {
        printf("\nMi dispiace ma ci sono altre persone in coda, verrai inserito anche tu");
        tmp->listaAttesa[taglia_indice].next  = InserisciAttesa(tmp->listaAttesa[taglia_indice].next ,cliente->nickname);
    }
    else {
        cliente->saldo = cliente->saldo - articolo->prezzo;
        articolo->taglie[taglia_indice] = articolo->taglie[taglia_indice] - 1;
        printf("\nLa transazione %c stata eseguita correttamente%c", e_accentata, faccina);
    }
}
static void Acquista(utenti **utente,abbigliamento **capi, int *errore, attesa **ListaAttesa){
    int id,taglia_indice,scelta;
    char taglia[ltg];
    attesa **tmp;
    tmp=*ListaAttesa;
    abbigliamento *articolo;
    utenti *cliente;
    cliente=*utente;
    articolo=*capi;
    printf("\nQuale articolo vuoi acquistare? Inserisci l'ID --->");
    scanf("%d",&id);
    articolo=CercaCapo(articolo,id,errore);
    if (cliente->saldo>=articolo->prezzo){
        do {
            printf("\nInserisci la taglia tra XS|S|M|L|XL");
            scanf("%s", taglia);
            if ((strcmp(taglia, "XS") == 0) || (strcmp(taglia, "xs") == 0)) {
                taglia_indice = 0;
            } else if ((strcmp(taglia, "S") == 0) || (strcmp(taglia, "s") == 0)) {
                taglia_indice = 1;
            } else if ((strcmp(taglia, "M") == 0) || (strcmp(taglia, "m") == 0)) {
                taglia_indice = 2;
            } else if ((strcmp(taglia, "L") == 0) || (strcmp(taglia, "l") == 0)) {
                taglia_indice = 3;
            } else if ((strcmp(taglia, "XL") == 0) || (strcmp(taglia, "xl") == 0)) {
                taglia_indice = 4;
            } else {
                printf("\nLa taglia inserita non %c giusta", e_accentata);
                taglia_indice = -1;
            }
        } while (taglia_indice==-1);
        printf("\nHai selezionato l'articolo:\t");
        printf("\nNome:%s  Modello:%s  Prezzo:%6.2f  Taglia: %s",articolo->nome,articolo->marca,articolo->prezzo,taglia);
        printf ("\nInserire 1 se desideri proseguire l'aqcuisto, 0 altrimenti");
        scanf("%d",&scelta);
        if (scelta==1) {
                Transazione(&cliente,&articolo,taglia_indice,ListaAttesa,id);
            }
    }
    else{
        printf ("\nDevi effettuare una ricarica di almeno%6.2f%c",articolo->prezzo-cliente->saldo,dollaro);
        RicaricaSaldo(&cliente);
        if (cliente->saldo>=articolo->prezzo)
        {
            Transazione(&cliente,&articolo,taglia_indice,ListaAttesa,id);
        }
        else{
            printf("\nLa ricarica non %c stata sufficiente ti mancano%6.2f%c",e_accentata,articolo->prezzo-cliente->saldo,dollaro);
            sleep(2);
        }
    }
}
abbigliamento *DeallocaCapi (abbigliamento *radice){
    if(radice!=NULL) {
        if((radice->sx==NULL) && (radice->dx==NULL)){
            free(radice);
        }
        DeallocaCapi(radice->sx);
        DeallocaCapi(radice->dx);
    }
    return radice;
}
//Funzioni principali del main----------------------------------------------------------
void MenuAdmin (utenti *utente,abbigliamento **capi, int *errore){
    int scelta,opzione,id,taglie[Ntaglie];
    char nome[dim2],marca[dim2];
    float prezzo;
    //Logo();
    printf("Benevnuto %s, sei nel sistema degli admin\nCosa vuoi fare?\n", utente->nickname);
    printf("\nInserisci 0 per concludere le operazioni\n");
    printf("Inserisci 1 per aggiungere un nuovo prodotto:\n");
    printf("Inserisci 2 per aggiornare la disponibilit%c dei prodotti\n",a_accentata);
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
        printf("\nInserisci la quantit%c delle taglie nel seguente ordine:XS | S | M | L | XL\n",a_accentata);
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
    MenuAdmin(utente,capi,errore);
}//FUNZIONA
void MenuUtente (utenti **utente, abbigliamento **capi, int *errore, attesa **ListaAttesa) {
    //Logo();
    char password[dim2];
    int lettera, scelta, controllo;
    utenti *tmp;
    tmp = *utente;
    printf("\nCiao %s\nSALDO: %6.2f%c\nQuali operazioni vuoi effettuare?", tmp->nickname, tmp->saldo, dollaro);
    printf("\nDigita 0 per ricaricare il tuo saldo");
    printf("\nDigita 1 per svuotare e prelevare il tuo saldo");
    printf("\nDigita 2 per Acquistare");
    printf("\nDigita 3 per fare il Logout--->");
    scanf("%d", &scelta);
    switch (scelta) {
        case 0: {
            RicaricaSaldo(&tmp);
            break;
        }
        case 1: {
            printf("\nSei sicuro di voler prelevare tutto il tuo conto? digita 0 per confermare, 1 altrimenti");
            scanf("%d", &lettera);
            if (lettera == 0) {
                do {
                    printf("\nInserisci la tua password--->");
                    scanf("%s", password);
                    if (controllo = strcmp(tmp->password, password) != 0) {
                        printf("\nLa password non corrisponde\nDigita -1 se vuoi tornare al men%c o 1 se vuoi riprovare ",
                               u_accentata);
                        scanf("%d", &scelta);
                        if (scelta == -1) {
                            MenuUtente(utente, capi, errore, ListaAttesa);
                            return;
                        }
                    }
                } while (controllo != 0);
                printf("\nHai ritirato %6.2f%c", tmp->saldo, dollaro);
                tmp->saldo = 0;
            }
            break;
        }
        case 2: {
            //Logo();
            //printf("\nCiao %c\nSALDO: %6.2f%c\n",tmp->nickname,tmp->saldo,dollaro);
            printf("\nGli articoli in vendita sono:\n");
            Stampa_Capi(*capi);
            Acquista(utente, capi, errore, ListaAttesa);
            break;
        }
        case 3: {
            *errore=5;
            return;
        }
    }
    MenuUtente(utente, capi, errore, ListaAttesa);
}//FUNZIONA
