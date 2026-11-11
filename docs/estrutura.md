# Estrutura do projeto

- `src/main.c`: inicialização da aplicação.
- `src/pse.h`: limites, estruturas e interface entre módulos.
- `src/menu.c`: leitura e validação de entradas, operações e apresentação no terminal.
- `src/dados.c`: criação da pasta de dados e persistência CSV.
- `docs/`: manual, casos de uso, testes e revisão da primeira entrega.
- `tests/`: reservado para testes automatizados futuros.
- `data/`: criado em tempo de execução; não versionado.

Cada tipo de registro possui capacidade máxima de 100. Os identificadores são incrementais e os estudantes/ações guardam o código da escola relacionada.
