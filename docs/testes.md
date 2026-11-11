# Casos de teste

Ambiente-alvo: GCC C11, terminal UTF-8, arquivos em diretório de dados isolado. Os cenários abaixo devem ser executados com dados descartáveis. **Status desta revisão: não executados**, pois o GCC disponível (`C:\MinGW\bin\gcc.exe`) falhou ao iniciar com erro do Windows `Illegal System DLL Relocation`. Portanto nenhum caso é declarado aprovado.

| ID | Cenário | Resultado esperado | Status |
|---|---|---|---|
| CT01 | Compilar com `-std=c11 -Wall -Wextra -Wpedantic` | Executável gerado sem erros | Bloqueado pelo ambiente |
| CT02 | Cadastrar escola válida | Código atribuído e registro em CSV | Não executado |
| CT03 | Cadastrar escola com mesmo nome | Duplicidade recusada | Não executado |
| CT04 | Cadastrar estudante com escola existente | Vínculo e persistência | Não executado |
| CT05 | Cadastrar estudante com escola inexistente | Registro recusado | Não executado |
| CT06 | Registrar ação com data válida | Código atribuído com situação Planejada | Não executado |
| CT07 | Informar data ou opção inválida | Mensagem de erro, sem alteração | Não executado |
| CT08 | Pesquisar por código/tema/escola/ano/situação existente | Ação correspondente listada | Não executado |
| CT09 | Pesquisar por valor inexistente | Mensagem sem resultados | Não executado |
| CT10 | Atualizar ação existente | Situação escolhida salva | Não executado |
| CT11 | Atualizar código inexistente | Registro não encontrado; dados intactos | Não executado |
| CT12 | Preencher cada categoria até 100 e tentar o 101º | 101º recusado sem corrupção | Não executado |
| CT13 | Fechar e abrir novamente | CSV recarregados, códigos e situação preservados | Não executado |
| CT14 | Campo obrigatório vazio, texto com `;`, idade fora da faixa | Entrada rejeitada | Não executado |
| CT15 | Gerar resumo em base vazia e preenchida | Totais correspondem aos registros | Não executado |

## Roteiro manual

1. Compile conforme o README em ambiente com GCC funcional.
2. Use diretório de dados limpo e registre uma escola, um estudante e uma ação.
3. Repita o cadastro da mesma escola/estudante para observar duplicidade.
4. Pesquise pela ação cadastrada e por um termo inexistente.
5. Atualize a situação e confira o resumo.
6. Saia e execute novamente; pesquise a ação atualizada.
7. Tente datas, números e campos inválidos.
8. Para o limite, prepare uma base de testes descartável e registre 100 itens por categoria antes da tentativa 101.

## Capturas

Ainda não há capturas de tela de execução, porque o executável não pôde ser compilado no ambiente desta revisão. Após compilar, capture uma sessão real do terminal exibindo cadastro, pesquisa, atualização e resumo; não use dados pessoais reais de estudantes.
