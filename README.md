# AEP 2 Semestre
Sistema de apoio ao gerenciamento das ações do Programa Saúde na Escola (PSE)

Identificação dos estudantes:
- Pedro Henrique Graf Vieira - RA:26002857-2
-  Murilo Ambrosio Stegani - RA:26007986-2
-  Gustavo Henrique de Lima Costa - RA:26001173-2
# PSE Gestão Escolar

Aplicação de console em C para apoiar o cadastro administrativo de escolas, estudantes e ações do Programa Saúde na Escola. O sistema armazena somente dados básicos de identificação e acompanhamento; não substitui sistemas oficiais e não mantém prontuários clínicos.

## Requisitos

- GCC compatível com C11 (por exemplo, MinGW-w64 no Windows ou GCC no Linux).
- Terminal com suporte a UTF-8 recomendado.
- Nenhuma biblioteca externa.

## Compilar e executar

Na pasta raiz do projeto:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic -finput-charset=UTF-8 -fexec-charset=UTF-8 -O2 src/main.c src/menu.c src/dados.c -o pse
./pse
```

No Windows, use `pse.exe` no lugar de `pse` para a saída e execute `pse.exe`.

Os arquivos de persistência são criados em `data/` no diretório de execução: `escolas.csv`, `estudantes.csv` e `acoes.csv`. Faça backup dessa pasta para preservar os registros. Cada categoria aceita até 100 registros.

## Funcionalidades

1. Cadastrar escola, com nome e município.
2. Cadastrar estudante e vinculá-lo a uma escola existente.
3. Registrar ação com escola, tema, data, responsável e situação inicial Planejada.
4. Pesquisar ações por código, nome da escola, tema, ano ou situação; também é possível listar todas.
5. Atualizar situação para Planejada, Em andamento, Concluída ou Cancelada.
6. Consultar resumo de escolas, estudantes, ações e quantitativos por situação e escola.

Validações incluem campos obrigatórios, opção e faixa numérica, escola existente, data no formato AAAA-MM-DD, caracteres delimitadores, duplicidades simples e capacidade máxima. Limites e instruções detalhadas estão em `docs/manual.md`.

## Documentação e testes

- `docs/casos_de_uso.md`: atores, objetivos, pré/pós-condições e fluxos.
- `docs/manual.md`: guia de operação e exemplos.
- `docs/testes.md`: casos previstos e situação da execução nesta entrega.

## Limitações conhecidas

Aplicação local de terminal, com limite fixo por categoria e sem autenticação, edição/exclusão de escola/estudante, filtros por intervalo de datas ou exportação formatada. A busca por período aceita o ano. Os dados são texto simples e devem ser protegidos no computador em que o programa for executado.

