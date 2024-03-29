#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct ingles{
    char *eng;
    struct ingles *prox;
};

typedef struct ingles ing;

struct arv{
    char *info1;
    char *info2;
    ing *l1,*l2;
    struct arv *esq,*cen,*dir;
    int qtd;
};

typedef struct arv arv;

struct capitulo{
    char *unidade;
    arv *arvore;
    struct capitulo *prox;
};

typedef struct capitulo cap;


int menu();
void mostraLista(ing *lista);
void insereLista(ing **palavra, char *p);
arv *criaNo(char *port,char *ingl,arv *esq,arv *cen,arv *dir);
arv *criaNoL(char *port,ing *ingl,arv *esq,arv *cen,arv *dir);
arv *quebraNo(arv **raiz,char *info,ing *ingl,char *promovePal,ing **promoveL,arv *sub);
arv *adcNO(arv *no,char *info,ing *lis,arv *filho);
int efolha(arv *no);
arv *insere(arv **raiz,char *info,char *ingl,char *promove,ing *promoveL,arv *pai);
void mostraInOrdem(arv *raiz);
void mostraPreOrdem(arv *raiz);
void Mostrar(cap *uni);
void lerArquivo(char *caminho, cap **uni);
void *inserirCapitulo(cap **uni, char *nome);
char *strcp(char *pala2);
char *forma(char *p);

int main(){
    arv *no;
    cap *uni = NULL;
    int choice;
    char procura[100];
    lerArquivo("Teste.txt", &uni);
    do{
        choice = menu();
        switch(choice){
        case 0:
            break;
        case 1:
            printf("Digite o nome da unidade:\n");
            scanf("%s", procura);
            //procuraUnidade(uni, procura);
            break;
        case 2:
            Mostrar(uni);
            break;
        case 3:
            printf("Digite o nome da palavra:\n");
            scanf("%s", procura);
            //removeNo(&uni->arvore, procura);
            break;
        default:
            printf("Informacao invalida!\n");
            break;
        }
    }while(choice != 0);
}

int menu(){
    int choice;
    printf("1-Mostrar Uma Unidade\n2-Mostrar Todas as Unidades\n3-Remover uma Palavra\n0-Sair\n");
    scanf("%d", &choice);
    return choice;
}

void Mostrar(cap *uni){
    if(uni!=NULL){
        Mostrar(uni->prox);
        printf("%s\n",uni->unidade);
        mostraInOrdem(uni->arvore);
    }
}

void mostraLista(ing *lista){
    if(lista != NULL){
        printf("%s", lista->eng);
        if(lista->prox != NULL){
            printf(", ");
        }else{
            printf(".");
        }
        mostraLista(lista->prox);
    }
}

void insereLista(ing **palavra, char *p){
    ing *new = (ing*) malloc(sizeof(ing));
    new->eng=(char*)malloc(sizeof(char)*100);
    strcpy(new->eng,p);
    new->prox = *palavra;
    *palavra = new;
}

arv *criaNo(char *port,char *ingl,arv *esq,arv *cen,arv *dir){
    arv *no=(arv*)malloc(sizeof(arv));
    no->info1=(char*)malloc(sizeof(char)*100);
    no->info2=(char*)malloc(sizeof(char)*100);
    strcpy(no->info1,port);
    no->l1=NULL;
    no->l2=NULL;
    insereLista(&(no->l1),ingl);
    no->qtd=1;
    no->esq=esq;
    no->cen=cen;
    no->dir=dir;
    return no;
}

arv *criaNoL(char *port,ing *ingl,arv *esq,arv *cen,arv *dir){
    arv *no=(arv*)malloc(sizeof(arv));
    no->info1=(char*)malloc(sizeof(char)*100);
    no->info2=(char*)malloc(sizeof(char)*100);
    strcpy(no->info1,port);
    no->l1=ingl;
    no->l2=NULL;
    no->qtd=1;
    no->esq=esq;
    no->cen=cen;
    no->dir=dir;
    return no;
}

arv *quebraNo(arv **raiz,char *info,ing *ingl,char *promovePal,ing **promoveL,arv *sub){
    arv *aux;
    if(strcmp(info,(*raiz)->info1)<0){
        strcpy(promovePal,(*raiz)->info1);
        *promoveL=(*raiz)->l1;
        (*raiz)->l1=ingl;
        strcpy((*raiz)->info1,info);
        (*raiz)->qtd-=1;
        aux=criaNoL((*raiz)->info2,(*raiz)->l2,(*raiz)->cen,(*raiz)->dir,NULL);
        (*raiz)->l2=NULL;
        (*raiz)->cen=sub;
        (*raiz)->dir=NULL;
    }else if(strcmp(info,(*raiz)->info2)<0){
        strcpy(promovePal,info);
        *promoveL=ingl;
        (*raiz)->qtd-=1;
        aux=criaNoL((*raiz)->info2,(*raiz)->l2,(*raiz)->cen,(*raiz)->dir,NULL);
        (*raiz)->l2=NULL;
        (*raiz)->cen=sub;
        (*raiz)->dir=NULL;
    }else{
       strcpy(promovePal,(*raiz)->info2);
        *promoveL=(*raiz)->l2;
        (*raiz)->qtd-=1;
        aux=criaNoL(info,ingl,(*raiz)->cen,(*raiz)->dir,NULL);
        (*raiz)->l2=NULL;
        (*raiz)->cen=sub;
        (*raiz)->dir=NULL;
    }
    return aux;
}

arv *adcNO(arv *no,char *info,ing *lis,arv *filho){
    if(strcmp(no->info1,info)<0){
        strcpy(no->info2,info);
        no->l2=lis;
        no->dir=filho;
    }else{
        strcpy(no->info2,no->info1);
        no->l2=no->l1;
        no->dir=no->cen;
        strcpy(no->info1,info);
        no->l1=lis;
        no->cen=filho;
    }
    no->qtd=2;
    return no;
}

int efolha(arv *no){
    int r=0;
    if((no)->esq == NULL && (no)->cen == NULL && (no)->dir == NULL){
        r = 1;
    }
    return r;
}

arv *insere(arv **raiz,char *info,char *ingl,char *promove,ing *promoveL,arv *pai){
    arv *aux = NULL;
    ing *aux2=NULL;
    if(promoveL==NULL){
        insereLista(&aux2,ingl);
    }    
    if(*raiz == NULL){
        *raiz=criaNo(info,ingl,NULL,NULL,NULL);
        aux=NULL;
    }else if(strcmp(info,(*raiz)->info1)==0){
        printf("%s\n",ingl);
        insereLista(&(*raiz)->l1,ingl);
    }else if((*raiz)->qtd == 2 && strcmp(info,(*raiz)->info2) == 0){
        insereLista(&(*raiz)->l2,ingl);
    }else if(efolha(*raiz)){
        if((*raiz)->qtd == 1){
            *raiz=adcNO(*raiz,info,aux2,NULL);
            aux=NULL;
        }else{
            aux=quebraNo(raiz,info,aux2,promove,&promoveL,NULL);
        }
    }else{
        if(strcmp(info,(*raiz)->info1)<0){
            aux=insere(&((*raiz)->esq),info,ingl,promove,promoveL,*raiz);
        }else if((*raiz)->qtd==1 || strcmp(info,(*raiz)->info2)<0){
            aux=insere(&((*raiz)->cen),info,ingl,promove,promoveL,*raiz);
        }else{
            aux=insere(&((*raiz)->dir),info,ingl,promove,promoveL,*raiz);
        }
    }
    if(aux!=NULL){
        if(pai==NULL){
            *raiz=criaNoL(promove,promoveL,*raiz,aux,NULL);
            aux=NULL;
        }else if(pai->qtd==1){
            pai=adcNO(pai,promove,promoveL,aux);
            aux=NULL;
        }else{
            aux=quebraNo(&pai,promove,promoveL,promove,&promoveL,aux);
        }
    }
    return aux;
}



void mostraInOrdem(arv *raiz){
    if(raiz!=NULL){
        mostraInOrdem(raiz->esq);
        printf("%s | ",raiz->info1);
        mostraLista(raiz->l1);
        printf("\n");
        mostraInOrdem(raiz->cen);
        if(raiz->qtd == 2){
            printf("%s | ",raiz->info2);
            mostraLista(raiz->l2);
            printf("\n");
            mostraInOrdem(raiz->dir);
        }
    }
}

void mostraPreOrdem(arv *raiz){
    if(raiz!=NULL){
        printf("<");
        printf("%s | ",raiz->info1);
        if(raiz->qtd == 2){
            printf("%s | ",raiz->info2);
            mostraPreOrdem(raiz->esq);
            mostraPreOrdem(raiz->cen);
            mostraPreOrdem(raiz->dir);
        }else{
            mostraPreOrdem(raiz->esq);
            mostraPreOrdem(raiz->cen);
        }
        printf(">");
    }
}

void *inserirCapitulo(cap **uni ,char *nome){
    cap *new = (cap*) malloc(sizeof(cap));
    new->unidade=(char*)malloc(sizeof(char)*100);
    strcpy(new->unidade, nome);
    new->arvore = NULL;
    new->prox = *uni;
    *uni = new; 
}

void lerArquivo(char *caminho, cap **uni){
    FILE *fptr;
    int i, j;
    char *palavras, *nome, *ingles, *promove;
    promove=(char*)malloc(sizeof(char)*100);
    palavras=(char*)malloc(sizeof(char)*100);
    nome=(char*)malloc(sizeof(char)*100);
    ingles=(char*)malloc(sizeof(char)*100);
    ing *proLista = NULL;
    fptr = fopen(caminho, "r");
    if( fptr == NULL ){
        printf("Erro na abertura de arquivo!\n");
        exit(1);        
    }else{
        printf("Arquivo aberto com sucesso!\n");
    }
    while (!feof(fptr)){
        fscanf(fptr, "%s", palavras);
        j = 0;
        if(palavras[0] == '%'){
            for(i=1; palavras[i] != '\0'; i++){
                nome[j] = palavras[i];
                j++;
            }
            nome[j] = '\0';
            inserirCapitulo(uni, nome);
        }else{
            for(i=0; palavras[i] != ':'; i++){
                ingles[j] = palavras[i];
                j++;
            }
            ingles[j] = '\0';
            j = 0;
            for(i=i+1; palavras[i] != '\0'; i++){
                nome[j] = palavras[i];
                j++;
                if(palavras[i] == ','){
                    nome[j-1] = '\0';
                    insere(&(*uni)->arvore, nome, ingles, promove, proLista, NULL);
                    proLista = NULL;
                    j = 0;
                }
            }
            nome[j] = '\0';
            insere(&(*uni)->arvore, nome, ingles, promove, proLista, NULL);
            proLista = NULL;
        }
    }
}
