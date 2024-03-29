#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct No{
    int altura, enderecoInicio, enderecoFim;
    char status;
    struct No *esq, *dir;
};

typedef struct No No;

int menu();
No* criaNo(int *status, int enderecoInicio, int enderecoFim);
void rotacaoRR(No** raiz);
void insere(No** raiz, No* Novo, int fim);
void mostra(No* raiz);
int maior(int a, int b);
int eFolha(No *no);
int altNo(No* raiz);
int alocar(No** raiz, No** pai, int qtdBlocos, int *status, int fim);
int liberar(No** raiz, No** pai, int qtdBlocos, int* status, int fim);
int buscaAloca(No* raiz, int valor);
int buscaLibera(No* raiz, int valor);


int main(){
	No* raiz = NULL;
	int  status=0, choice, fim;
	clock_t tempoBuscaI, tempoBuscaF;
	clock_t tempoAlterarI, tempoAlterarF;
	float tempoBuscaDecorrido, tempoAlterarDecorrido;
	do{
		choice = menu();
		switch(choice){
			case 1:
				free(raiz);
				raiz = NULL;
                printf("Digite a quantidade de blocos em MBs: ");
				scanf("%d", &fim);
				printf("Insira como esta o primeiro bloco (1-Ocupado 0-Livre)\n");
				scanf("%d", &status);
				int auxinicio=0, auxfim=0, cont=0;
				while(auxfim < fim){
					printf("Insira o endereço incial: ");
					scanf("%d", &auxinicio);
					printf("Insira o endereço final: ");
					scanf("%d", &auxfim);
					insere(&raiz, criaNo(&status, auxinicio,auxfim), fim);
				}
				break;
			case 2:
				mostra(raiz);
				break;
			case 3:	
				printf("Digite a quantidade de blocos para alocar: ");
				int auxalocar;
				scanf("%d", &auxalocar);
				tempoBuscaI = clock();
				int procuraAloc = buscaAloca(raiz, auxalocar);
				tempoBuscaF = clock();
				tempoBuscaDecorrido = ((long double)(tempoBuscaF- tempoBuscaI)*1000.0/CLOCKS_PER_SEC);
				printf("Tempo de busca: %lf \n", tempoBuscaDecorrido);
				tempoAlterarI = clock();
				if( alocar(&raiz,NULL, auxalocar, &status, fim) == 0)
					printf("Espaço insuficiente.\n", procuraAloc);
				else
					printf("Alocação bem sucedida.\n", procuraAloc);	
				tempoAlterarF = clock();
				tempoAlterarDecorrido = ((long double)(tempoAlterarF- tempoAlterarI)*1000.0/CLOCKS_PER_SEC);
				printf("Tempo de alterar %lf \n", tempoBuscaDecorrido);
				break;
			case 4:
				printf("Digite a quantidade de blocos para liberar: ");
				int auxliberar;
				scanf("%d", &auxliberar);
				tempoBuscaI = clock();
				int procuraLib = buscaLibera(raiz, auxliberar);
				tempoBuscaF = clock();
				tempoBuscaDecorrido = ((long double)(tempoBuscaF- tempoBuscaI)*1000.0/CLOCKS_PER_SEC);
				printf("Tempo para buscar: %lf \n", tempoBuscaDecorrido);
				tempoAlterarI = clock();
				if( liberar(&raiz,NULL, auxliberar, &status, fim) == 0)
					printf("Espaço %d insuficiente.\n", procuraLib);
				else
					printf("Liberação %d bem sucedida.\n", procuraLib);
				tempoAlterarF = clock();
				tempoAlterarDecorrido = ((long double)(tempoAlterarF- tempoAlterarI)*1000.0/CLOCKS_PER_SEC);
				printf("Tempo para alterar: %lf \n", tempoBuscaDecorrido);
				break;
			case 0:
				break;
		}
	}while(choice != 0);
    return 0;
}

int menu(){
	int choice;
	printf("1-Criar Arvore\n2-Mostra Arvore\n3-Alocar No\n4-Liberar No\n0-Sair\n");
	scanf("%d", &choice);
	return choice;
}

No* criaNo(int *status, int enderecoInicio, int enderecoFim){
	No* no = (No*) malloc(sizeof(No));
    no->altura = 0;
	no->enderecoInicio = enderecoInicio;
	no->enderecoFim = enderecoFim;
    no->esq = NULL;
	no->dir = NULL;
	if( *status == 0)
		no->status = 'L';
	else
		no->status = 'O';
	*status = 1- (*status);
	return no;
}

int maior(int a, int b){
	 int mai;
    if(a > b){
        mai = a;
    }else{
        mai = b;
    }
    return mai;
}

int eFolha(No *no){
    int ret = 0;
    if((no->esq == NULL) && (no->dir == NULL)){
        ret = 1;
    }
    return ret;
}

int altNo(No* raiz){
    int alt;
    if(raiz == NULL){
        alt = -1;
    }else{
        alt = raiz->altura;
    }
    return alt;
}

void rotacaoRR(No** raiz){
	No *aux;
	aux = (*raiz)->dir;
	(*raiz)->dir = aux->esq;
	aux->esq = *raiz;
	*raiz = aux;
}

void insere(No** raiz, No* Novo, int fim){
	if(*raiz==NULL){
		*raiz = Novo;
	}else{
		if(fim >= Novo->enderecoInicio){
			if(Novo->enderecoInicio > (*raiz)->enderecoFim){
				insere(&(*raiz)->dir, Novo, fim);
				if( abs( altNo((*raiz)->esq)-altNo((*raiz)->dir)) == 2)
					rotacaoRR(raiz);
			}
		}
        (*raiz)->altura = maior(altNo((*raiz)->esq), altNo((*raiz)->dir)) + 1;
	}
}

void mostra(No* raiz){
	if( raiz != NULL){
		mostra(raiz->esq);
		printf("Status: %c Inicio: %d - Fim: %d\n",raiz->status, raiz->enderecoInicio, raiz->enderecoFim);
		mostra(raiz->dir);
	}
}

int alocar(No** raiz, No** pai, int qtdBlocos, int *status, int fim){
	int resul = 0;
	if( *raiz != NULL){
		resul = alocar(&(*raiz)->esq,raiz, qtdBlocos, status, fim);
		if(resul == 0){
			if((*raiz)->status == 'L'){
				int qtdLocal = (*raiz)->enderecoFim - (*raiz)->enderecoInicio + 1;
				if( pai == NULL && (*raiz)->esq == NULL && (*raiz)->dir == NULL){
					if(qtdLocal == qtdBlocos){
						(*raiz)->status = 'O';
                        *status = 1 - (*status);
					}else{
						int newfim = (*raiz)->enderecoFim - qtdBlocos;
						int auxfim = (*raiz)->enderecoFim;
						(*raiz)->enderecoFim = newfim;
						int newinicio = newfim+1;
						insere(raiz, criaNo(status, newinicio,auxfim), fim);
					}
					resul = 1;
				}else if(eFolha(*raiz)){
					if(qtdLocal == qtdBlocos){
						int newfim = (*pai)->enderecoFim + qtdBlocos;
						(*pai)->enderecoFim = newfim;
						free(*raiz);
						*raiz = NULL;
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						if((*pai)->esq == *raiz){
							int newfim = (*raiz)->enderecoFim - qtdBlocos;
							int newinicio = (*pai)->enderecoInicio - qtdBlocos;
							(*raiz)->enderecoFim = newfim;
							(*pai)->enderecoInicio = newinicio;
						}else{
							int newinicio = (*raiz)->enderecoInicio + qtdBlocos;
							int newfim = (*pai)->enderecoFim + qtdBlocos;
							(*raiz)->enderecoInicio = newinicio;
							(*pai)->enderecoFim = newfim;
						}
						resul = 1;
					}
				}else{
					No** paiMaisEsq = raiz;
					No** maisEsq = &((*raiz)->dir);
					while((*maisEsq)->esq != NULL){
						paiMaisEsq = maisEsq;
						maisEsq = &((*maisEsq)->esq) ;
					}
					if((*paiMaisEsq)->status == 'O'){
						maisEsq = paiMaisEsq;
					}		
					if(qtdLocal == qtdBlocos){
						No** paiMaisDir = raiz;
						No** maisDir = &((*raiz)->esq);
						while((*maisDir)->dir != NULL){
							paiMaisDir = maisDir;
							maisDir = &((*maisDir)->dir) ;
						}
						if((*paiMaisDir)->status == 'O'){
							maisDir = paiMaisDir;
						}
						int newinicio = (*maisDir)->enderecoInicio;
						int newfim = (*maisEsq)->enderecoFim;
						(*raiz)->enderecoInicio = newinicio;
						(*raiz)->enderecoFim = newfim;
						(*raiz)->status = 'O';
						free(*maisDir);
						free(*maisEsq);
						*maisDir = NULL;
						*maisEsq = NULL;	
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						int newfim = (*raiz)->enderecoFim - qtdBlocos;
						int newinicio = (*maisEsq)->enderecoInicio - qtdBlocos;
						(*raiz)->enderecoFim = newfim;
						(*maisEsq)->enderecoInicio = newinicio;
						resul = 1;					
					}else{
						resul = alocar(&(*raiz)->dir,raiz, qtdBlocos, status, fim);
					}	
				}
			}else{
				resul = alocar(&(*raiz)->dir,raiz, qtdBlocos, status, fim);
			}
		}
	}
	return resul;
}

int liberar(No** raiz, No** pai, int qtdBlocos, int* status, int fim){
	int resul = 0;
	if( *raiz != NULL){
		resul = liberar(&(*raiz)->esq,raiz, qtdBlocos, status, fim);
		if(resul == 0){
			if((*raiz)->status == 'O'){
				int qtdLocal = (*raiz)->enderecoFim - (*raiz)->enderecoInicio +1;
				if( pai == NULL && (*raiz)->esq == NULL && (*raiz)->dir == NULL){
					if(qtdLocal == qtdBlocos){
						(*raiz)->status = 'L';
						*status = 1 - (*status);
					}else{
						int newfim = (*raiz)->enderecoFim - qtdBlocos;
						int auxfim = (*raiz)->enderecoFim;
						(*raiz)->enderecoFim = newfim;
						int newinicio = newfim+1;
						insere(raiz, criaNo(status, newinicio,auxfim), fim);
					}
					resul = 1;
				}else if((*raiz)->dir == NULL){
					if(qtdLocal == qtdBlocos){
						int newfim = (*pai)->enderecoFim + qtdBlocos;
						(*pai)->enderecoFim = newfim;
						free(*raiz);
						*raiz = NULL;
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						if((*pai)->esq == *raiz){
							int newfim = (*raiz)->enderecoFim - qtdBlocos;
							int newinicio = (*pai)->enderecoInicio - qtdBlocos;
							(*raiz)->enderecoFim = newfim;
							(*pai)->enderecoInicio = newinicio;
						}else{
							int newinicio = (*raiz)->enderecoInicio + qtdBlocos;
							int newfim = (*pai)->enderecoFim + qtdBlocos;
							(*raiz)->enderecoInicio = newinicio;
							(*pai)->enderecoFim = newfim;
						}
						resul = 1;
					}
				}else{
					No** paiMaisEsq = raiz;
					No** maisEsq = &((*raiz)->dir);
					while((*maisEsq)->esq != NULL){
						paiMaisEsq = maisEsq;
						maisEsq = &((*maisEsq)->esq) ;
					}
					if((*paiMaisEsq)->status == 'L'){
						maisEsq = paiMaisEsq;
					}
					if(qtdLocal == qtdBlocos){
						No** paiMaisDir = raiz;
						No** maisDir = &((*raiz)->esq);
						while((*maisDir)->dir != NULL){
							paiMaisDir = maisDir;
							maisDir = &((*maisDir)->dir) ;
						}
						if((*paiMaisDir)->status == 'L'){
							maisDir = paiMaisDir;
						}
						int newinicio = (*maisDir)->enderecoInicio;
						int newfim = (*maisEsq)->enderecoFim;
						(*raiz)->enderecoInicio = newinicio;
						(*raiz)->enderecoFim = newfim;
						(*raiz)->status = 'L';
						free(*maisDir);
						free(*maisEsq);
						*maisDir = NULL;
						*maisEsq = NULL;	
						resul = 1;
					}else if(qtdLocal > qtdBlocos){
						int newfim = (*raiz)->enderecoFim - qtdBlocos;
						int newinicio = (*maisEsq)->enderecoInicio - qtdBlocos;
						(*raiz)->enderecoFim = newfim;
						(*maisEsq)->enderecoInicio = newinicio;
						resul = 1;
					}else{
						resul = liberar(&(*raiz)->dir,raiz, qtdBlocos, status, fim);
					}	
				}
			}else{
				resul = liberar(&(*raiz)->dir,raiz, qtdBlocos, status, fim);
			}
		}
	}
	return resul;
}

int buscaAloca(No* raiz, int valor){
	int procura = 0;
	if(raiz != NULL){
		procura = buscaAloca(raiz->esq, valor);
		if((raiz)->status == 'L'){
			int qtdLocal = (raiz)->enderecoFim - (raiz)->enderecoInicio +1;
			if(valor <= qtdLocal){
				procura = 1;
			}
		}
		if(procura==0){
			procura = buscaAloca(raiz->dir, valor);
		}
	}
	return procura;
}

int buscaLibera(No* raiz, int valor){
	int procura = 0;
	if(raiz != NULL){
		procura = buscaLibera(raiz->esq, valor);
		if((raiz)->status == 'L'){
			int qtdLocal = (raiz)->enderecoFim - (raiz)->enderecoInicio +1;
			if(valor <= qtdLocal){
				procura = 1;
			}
		}
		if(procura==0){
			procura = buscaLibera(raiz->dir, valor);
		}
	}
	return procura;
}