#ifndef PSE_H
#define PSE_H

#include <stddef.h>

#define LIMITE_REGISTROS 100
#define TAM_NOME 100
#define TAM_TEMA 100
#define TAM_DATA 11
#define TAM_RESPONSAVEL 100
#define TAM_STATUS 20

typedef struct {
    int id;
    char nome[TAM_NOME];
    char municipio[TAM_NOME];
} Escola;

typedef struct {
    int id;
    int escola_id;
    char nome[TAM_NOME];
    int idade;
} Estudante;

typedef struct {
    int id;
    int escola_id;
    char tema[TAM_TEMA];
    char data[TAM_DATA];
    char responsavel[TAM_RESPONSAVEL];
    char status[TAM_STATUS];
} Acao;

typedef struct {
    Escola escolas[LIMITE_REGISTROS];
    Estudante estudantes[LIMITE_REGISTROS];
    Acao acoes[LIMITE_REGISTROS];
    size_t quantidade_escolas;
    size_t quantidade_estudantes;
    size_t quantidade_acoes;
    int proximo_id_escola;
    int proximo_id_estudante;
    int proximo_id_acao;
} Sistema;

int carregar_dados(Sistema *sistema);
int salvar_dados(const Sistema *sistema);
void executar_menu(Sistema *sistema);

#endif
