#include "pse.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

static void montar_caminho(char *destino, size_t tamanho, const char *nome_arquivo)
{
    snprintf(destino, tamanho, "data/%s.csv", nome_arquivo);
}

static int criar_diretorio_dados(void)
{
#ifdef _WIN32
    return _mkdir("data") == 0 || errno == EEXIST;
#else
    return mkdir("data", 0777) == 0 || errno == EEXIST;
#endif
}

static int salvar_escolas(const Sistema *sistema)
{
    char caminho[64];
    FILE *arquivo;
    size_t i;

    montar_caminho(caminho, sizeof(caminho), "escolas");
    arquivo = fopen(caminho, "w");
    if (arquivo == NULL) {
        return 0;
    }

    for (i = 0; i < sistema->quantidade_escolas; i++) {
        const Escola *escola = &sistema->escolas[i];
        fprintf(arquivo, "%d;%s;%s\n", escola->id, escola->nome, escola->municipio);
    }

    return fclose(arquivo) == 0;
}

static int salvar_estudantes(const Sistema *sistema)
{
    char caminho[64];
    FILE *arquivo;
    size_t i;

    montar_caminho(caminho, sizeof(caminho), "estudantes");
    arquivo = fopen(caminho, "w");
    if (arquivo == NULL) {
        return 0;
    }

    for (i = 0; i < sistema->quantidade_estudantes; i++) {
        const Estudante *estudante = &sistema->estudantes[i];
        fprintf(arquivo, "%d;%d;%s;%d\n", estudante->id, estudante->escola_id,
                estudante->nome, estudante->idade);
    }

    return fclose(arquivo) == 0;
}

static int salvar_acoes(const Sistema *sistema)
{
    char caminho[64];
    FILE *arquivo;
    size_t i;

    montar_caminho(caminho, sizeof(caminho), "acoes");
    arquivo = fopen(caminho, "w");
    if (arquivo == NULL) {
        return 0;
    }

    for (i = 0; i < sistema->quantidade_acoes; i++) {
        const Acao *acao = &sistema->acoes[i];
        fprintf(arquivo, "%d;%d;%s;%s;%s;%s\n", acao->id, acao->escola_id,
                acao->tema, acao->data, acao->responsavel, acao->status);
    }

    return fclose(arquivo) == 0;
}

int salvar_dados(const Sistema *sistema)
{
    if (!criar_diretorio_dados()) {
        return 0;
    }

    return salvar_escolas(sistema)
        && salvar_estudantes(sistema)
        && salvar_acoes(sistema);
}

static void carregar_escolas(Sistema *sistema)
{
    char caminho[64];
    char linha[512];
    FILE *arquivo;

    montar_caminho(caminho, sizeof(caminho), "escolas");
    arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL
           && sistema->quantidade_escolas < LIMITE_REGISTROS) {
        Escola escola;
        if (sscanf(linha, "%d;%99[^;];%99[^\r\n]", &escola.id,
                   escola.nome, escola.municipio) == 3) {
            sistema->escolas[sistema->quantidade_escolas++] = escola;
            if (escola.id >= sistema->proximo_id_escola) {
                sistema->proximo_id_escola = escola.id + 1;
            }
        }
    }

    fclose(arquivo);
}

static void carregar_estudantes(Sistema *sistema)
{
    char caminho[64];
    char linha[512];
    FILE *arquivo;

    montar_caminho(caminho, sizeof(caminho), "estudantes");
    arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL
           && sistema->quantidade_estudantes < LIMITE_REGISTROS) {
        Estudante estudante;
        if (sscanf(linha, "%d;%d;%99[^;];%d", &estudante.id,
                   &estudante.escola_id, estudante.nome, &estudante.idade) == 4) {
            sistema->estudantes[sistema->quantidade_estudantes++] = estudante;
            if (estudante.id >= sistema->proximo_id_estudante) {
                sistema->proximo_id_estudante = estudante.id + 1;
            }
        }
    }

    fclose(arquivo);
}

static void carregar_acoes(Sistema *sistema)
{
    char caminho[64];
    char linha[512];
    FILE *arquivo;

    montar_caminho(caminho, sizeof(caminho), "acoes");
    arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL
           && sistema->quantidade_acoes < LIMITE_REGISTROS) {
        Acao acao;
        if (sscanf(linha, "%d;%d;%99[^;];%10[^;];%99[^;];%19[^\r\n]",
                   &acao.id, &acao.escola_id, acao.tema, acao.data,
                   acao.responsavel, acao.status) == 6) {
            sistema->acoes[sistema->quantidade_acoes++] = acao;
            if (acao.id >= sistema->proximo_id_acao) {
                sistema->proximo_id_acao = acao.id + 1;
            }
        }
    }

    fclose(arquivo);
}

int carregar_dados(Sistema *sistema)
{
    memset(sistema, 0, sizeof(*sistema));
    sistema->proximo_id_escola = 1;
    sistema->proximo_id_estudante = 1;
    sistema->proximo_id_acao = 1;

    carregar_escolas(sistema);
    carregar_estudantes(sistema);
    carregar_acoes(sistema);
    return 1;
}
