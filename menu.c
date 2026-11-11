#include "pse.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_ENTRADA 512

typedef enum {
    BUSCAR_CODIGO = 1,
    BUSCAR_ESCOLA,
    BUSCAR_TEMA,
    BUSCAR_ANO,
    BUSCAR_STATUS,
    LISTAR_TODAS
} FiltroPesquisa;

static int ler_texto(const char *mensagem, char *destino, size_t capacidade)
{
    char entrada[TAM_ENTRADA];
    size_t tamanho;

    printf("%s", mensagem);
    if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
        return 0;
    }

    if (strchr(entrada, '\n') == NULL) {
        int caractere;
        while ((caractere = getchar()) != '\n' && caractere != EOF) {
        }
    }

    entrada[strcspn(entrada, "\r\n")] = '\0';
    tamanho = strlen(entrada);

    while (tamanho > 0 && (entrada[tamanho - 1] == ' ' || entrada[tamanho - 1] == '\t')) {
        entrada[--tamanho] = '\0';
    }

    while (*entrada == ' ' || *entrada == '\t') {
        memmove(entrada, entrada + 1, strlen(entrada));
    }

    tamanho = strlen(entrada);
    if (tamanho == 0 || tamanho >= capacidade || strchr(entrada, ';') != NULL) {
        puts("Entrada vazia, longa demais ou com caractere reservado.");
        return 0;
    }

    memcpy(destino, entrada, tamanho + 1);
    return 1;
}

static int ler_inteiro(const char *mensagem, int minimo, int maximo, int *valor)
{
    char entrada[64];
    char *fim;
    long numero;

    if (!ler_texto(mensagem, entrada, sizeof(entrada))) {
        return 0;
    }

    numero = strtol(entrada, &fim, 10);
    if (*fim != '\0' || numero < minimo || numero > maximo) {
        printf("Informe um número entre %d e %d.\n", minimo, maximo);
        return 0;
    }

    *valor = (int)numero;
    return 1;
}

static int comparar_sem_diferenciar_caixa(const char *primeiro, const char *segundo)
{
    while (*primeiro != '\0' && *segundo != '\0') {
        if (tolower((unsigned char)*primeiro) != tolower((unsigned char)*segundo)) {
            return 0;
        }
        primeiro++;
        segundo++;
    }

    return *primeiro == '\0' && *segundo == '\0';
}

static int contem_sem_diferenciar_caixa(const char *texto, const char *busca)
{
    size_t tamanho_texto = strlen(texto);
    size_t tamanho_busca = strlen(busca);
    size_t inicio;

    if (tamanho_busca > tamanho_texto) {
        return 0;
    }

    for (inicio = 0; inicio + tamanho_busca <= tamanho_texto; inicio++) {
        size_t i;
        for (i = 0; i < tamanho_busca; i++) {
            if (tolower((unsigned char)texto[inicio + i])
                != tolower((unsigned char)busca[i])) {
                break;
            }
        }
        if (i == tamanho_busca) {
            return 1;
        }
    }

    return 0;
}

static int indice_escola(const Sistema *sistema, int id_escola)
{
    size_t i;
    for (i = 0; i < sistema->quantidade_escolas; i++) {
        if (sistema->escolas[i].id == id_escola) {
            return (int)i;
        }
    }
    return -1;
}

static int indice_acao(const Sistema *sistema, int id_acao)
{
    size_t i;
    for (i = 0; i < sistema->quantidade_acoes; i++) {
        if (sistema->acoes[i].id == id_acao) {
            return (int)i;
        }
    }
    return -1;
}

static void listar_escolas(const Sistema *sistema)
{
    size_t i;

    puts("\nEscolas cadastradas:");
    for (i = 0; i < sistema->quantidade_escolas; i++) {
        const Escola *escola = &sistema->escolas[i];
        printf("  %d - %s (%s)\n", escola->id, escola->nome, escola->municipio);
    }
}

static void cadastrar_escola(Sistema *sistema)
{
    char nome[TAM_NOME];
    char municipio[TAM_NOME];
    size_t i;
    Escola *escola;

    if (sistema->quantidade_escolas == LIMITE_REGISTROS) {
        puts("Limite de escolas atingido.");
        return;
    }
    if (!ler_texto("Nome da escola: ", nome, sizeof(nome))
        || !ler_texto("Município: ", municipio, sizeof(municipio))) {
        return;
    }

    for (i = 0; i < sistema->quantidade_escolas; i++) {
        if (comparar_sem_diferenciar_caixa(sistema->escolas[i].nome, nome)) {
            puts("Já existe escola com esse nome.");
            return;
        }
    }

    escola = &sistema->escolas[sistema->quantidade_escolas++];
    escola->id = sistema->proximo_id_escola++;
    strcpy(escola->nome, nome);
    strcpy(escola->municipio, municipio);

    if (salvar_dados(sistema)) {
        printf("Escola cadastrada com código %d.\n", escola->id);
    } else {
        puts("Falha ao salvar os dados.");
    }
}

static void cadastrar_estudante(Sistema *sistema)
{
    char nome[TAM_NOME];
    int id_escola;
    int idade;
    size_t i;
    Estudante *estudante;

    if (sistema->quantidade_estudantes == LIMITE_REGISTROS) {
        puts("Limite de estudantes atingido.");
        return;
    }
    if (sistema->quantidade_escolas == 0) {
        puts("Cadastre uma escola antes.");
        return;
    }

    listar_escolas(sistema);
    if (!ler_inteiro("Código da escola: ", 1, 999999, &id_escola)) {
        return;
    }
    if (indice_escola(sistema, id_escola) < 0) {
        puts("Escola inexistente.");
        return;
    }
    if (!ler_texto("Nome do estudante: ", nome, sizeof(nome))
        || !ler_inteiro("Idade (4 a 25): ", 4, 25, &idade)) {
        return;
    }

    for (i = 0; i < sistema->quantidade_estudantes; i++) {
        const Estudante *atual = &sistema->estudantes[i];
        if (atual->escola_id == id_escola
            && comparar_sem_diferenciar_caixa(atual->nome, nome)) {
            puts("Estudante já cadastrado nessa escola.");
            return;
        }
    }

    estudante = &sistema->estudantes[sistema->quantidade_estudantes++];
    estudante->id = sistema->proximo_id_estudante++;
    estudante->escola_id = id_escola;
    estudante->idade = idade;
    strcpy(estudante->nome, nome);

    if (salvar_dados(sistema)) {
        printf("Estudante cadastrado com código %d.\n", estudante->id);
    } else {
        puts("Falha ao salvar os dados.");
    }
}

static int data_valida(const char *data)
{
    int ano;
    int mes;
    int dia;
    size_t i;

    if (strlen(data) != 10 || data[4] != '-' || data[7] != '-') {
        return 0;
    }
    for (i = 0; i < 10; i++) {
        if (i != 4 && i != 7 && !isdigit((unsigned char)data[i])) {
            return 0;
        }
    }

    sscanf(data, "%4d-%2d-%2d", &ano, &mes, &dia);
    return ano >= 2000 && ano <= 2099 && mes >= 1 && mes <= 12 && dia >= 1 && dia <= 31;
}

static void registrar_acao(Sistema *sistema)
{
    char tema[TAM_TEMA];
    char data[TAM_DATA];
    char responsavel[TAM_RESPONSAVEL];
    int id_escola;
    Acao *acao;

    if (sistema->quantidade_acoes == LIMITE_REGISTROS) {
        puts("Limite de ações atingido.");
        return;
    }
    if (sistema->quantidade_escolas == 0) {
        puts("Cadastre uma escola antes.");
        return;
    }

    listar_escolas(sistema);
    if (!ler_inteiro("Código da escola: ", 1, 999999, &id_escola)) {
        return;
    }
    if (indice_escola(sistema, id_escola) < 0) {
        puts("Escola inexistente.");
        return;
    }
    if (!ler_texto("Tema da ação: ", tema, sizeof(tema))
        || !ler_texto("Data (AAAA-MM-DD): ", data, sizeof(data))) {
        return;
    }
    if (!data_valida(data)) {
        puts("Data inválida. Use AAAA-MM-DD.");
        return;
    }
    if (!ler_texto("Responsável: ", responsavel, sizeof(responsavel))) {
        return;
    }

    acao = &sistema->acoes[sistema->quantidade_acoes++];
    acao->id = sistema->proximo_id_acao++;
    acao->escola_id = id_escola;
    strcpy(acao->tema, tema);
    strcpy(acao->data, data);
    strcpy(acao->responsavel, responsavel);
    strcpy(acao->status, "Planejada");

    if (salvar_dados(sistema)) {
        printf("Ação registrada com código %d (Planejada).\n", acao->id);
    } else {
        puts("Falha ao salvar os dados.");
    }
}

static void imprimir_acao(const Sistema *sistema, const Acao *acao)
{
    int posicao_escola = indice_escola(sistema, acao->escola_id);
    const char *nome_escola = posicao_escola >= 0
        ? sistema->escolas[posicao_escola].nome
        : "(escola removida)";

    printf("#%d | %s | %s | %s | %s | Escola: %s\n",
           acao->id, acao->tema, acao->data, acao->responsavel,
           acao->status, nome_escola);
}

static int acao_corresponde(const Sistema *sistema, const Acao *acao,
                            FiltroPesquisa filtro, const char *termo)
{
    int posicao_escola;
    char id_texto[20];

    switch (filtro) {
    case BUSCAR_CODIGO:
        snprintf(id_texto, sizeof(id_texto), "%d", acao->id);
        return strcmp(id_texto, termo) == 0;
    case BUSCAR_ESCOLA:
        posicao_escola = indice_escola(sistema, acao->escola_id);
        return posicao_escola >= 0
            && contem_sem_diferenciar_caixa(sistema->escolas[posicao_escola].nome, termo);
    case BUSCAR_TEMA:
        return contem_sem_diferenciar_caixa(acao->tema, termo);
    case BUSCAR_ANO:
        return strlen(termo) == 4 && strncmp(acao->data, termo, 4) == 0;
    case BUSCAR_STATUS:
        return contem_sem_diferenciar_caixa(acao->status, termo);
    case LISTAR_TODAS:
        return 1;
    }

    return 0;
}

static void pesquisar_acoes(const Sistema *sistema)
{
    int opcao;
    char termo[120] = "";
    int encontradas = 0;
    size_t i;

    puts("Pesquisar ações:");
    puts("1 Código | 2 Escola | 3 Tema | 4 Ano | 5 Situação | 6 Listar todas");
    if (!ler_inteiro("Opção: ", BUSCAR_CODIGO, LISTAR_TODAS, &opcao)) {
        return;
    }
    if (opcao != LISTAR_TODAS && !ler_texto("Termo de pesquisa: ", termo, sizeof(termo))) {
        return;
    }

    for (i = 0; i < sistema->quantidade_acoes; i++) {
        const Acao *acao = &sistema->acoes[i];
        if (acao_corresponde(sistema, acao, (FiltroPesquisa)opcao, termo)) {
            imprimir_acao(sistema, acao);
            encontradas++;
        }
    }

    if (encontradas == 0) {
        puts("Nenhuma ação encontrada.");
    }
}

static void atualizar_situacao(Sistema *sistema)
{
    static const char *situacoes[] = {
        "Planejada", "Em andamento", "Concluída", "Cancelada"
    };
    int id_acao;
    int opcao;
    int posicao_acao;

    if (!ler_inteiro("Código da ação: ", 1, 999999, &id_acao)) {
        return;
    }
    posicao_acao = indice_acao(sistema, id_acao);
    if (posicao_acao < 0) {
        puts("Ação não encontrada.");
        return;
    }

    printf("Situação atual: %s\n", sistema->acoes[posicao_acao].status);
    puts("1 Planejada | 2 Em andamento | 3 Concluída | 4 Cancelada");
    if (!ler_inteiro("Nova situação: ", 1, 4, &opcao)) {
        return;
    }

    strcpy(sistema->acoes[posicao_acao].status, situacoes[opcao - 1]);
    if (salvar_dados(sistema)) {
        puts("Situação atualizada e salva.");
    } else {
        puts("Falha ao salvar os dados.");
    }
}

static void gerar_resumo(const Sistema *sistema)
{
    static const char *situacoes[] = {
        "Planejada", "Em andamento", "Concluída", "Cancelada"
    };
    int totais_por_situacao[4] = {0, 0, 0, 0};
    size_t i;

    for (i = 0; i < sistema->quantidade_acoes; i++) {
        size_t j;
        for (j = 0; j < 4; j++) {
            if (strcmp(sistema->acoes[i].status, situacoes[j]) == 0) {
                totais_por_situacao[j]++;
                break;
            }
        }
    }

    printf("\nResumo: %zu escolas, %zu estudantes, %zu ações\n",
           sistema->quantidade_escolas, sistema->quantidade_estudantes,
           sistema->quantidade_acoes);
    for (i = 0; i < 4; i++) {
        printf("  %-15s %d\n", situacoes[i], totais_por_situacao[i]);
    }

    for (i = 0; i < sistema->quantidade_escolas; i++) {
        size_t j;
        int total_acoes = 0;
        for (j = 0; j < sistema->quantidade_acoes; j++) {
            if (sistema->acoes[j].escola_id == sistema->escolas[i].id) {
                total_acoes++;
            }
        }
        printf("  Escola %s: %d ação(ões)\n", sistema->escolas[i].nome, total_acoes);
    }
}

static void exibir_menu(void)
{
    puts("\n=== PSE Gestão Escolar ===");
    puts("1 Cadastrar escola");
    puts("2 Cadastrar estudante");
    puts("3 Registrar ação do PSE");
    puts("4 Pesquisar ações");
    puts("5 Atualizar situação");
    puts("6 Gerar resumo");
    puts("0 Sair");
}

void executar_menu(Sistema *sistema)
{
    char entrada[32];
    char *fim;
    long opcao;

    for (;;) {
        exibir_menu();
        if (!ler_texto("Opção: ", entrada, sizeof(entrada))) {
            continue;
        }

        opcao = strtol(entrada, &fim, 10);
        if (*fim != '\0') {
            puts("Opção inválida.");
            continue;
        }

        switch (opcao) {
        case 1:
            cadastrar_escola(sistema);
            break;
        case 2:
            cadastrar_estudante(sistema);
            break;
        case 3:
            registrar_acao(sistema);
            break;
        case 4:
            pesquisar_acoes(sistema);
            break;
        case 5:
            atualizar_situacao(sistema);
            break;
        case 6:
            gerar_resumo(sistema);
            break;
        case 0:
            puts("Dados mantidos em data/. Até logo.");
            return;
        default:
            puts("Opção inválida.");
            break;
        }
    }
}
