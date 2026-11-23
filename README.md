# Sistema de Gerenciamento de Senhas de Atendimento Clínico

**Aluno:** Bernardo Kopp Pinheiro  
**RGM:** 47294264  
**Curso:** Análise e Desenvolvimento de Sistemas  
**Matéria:** Programação em C  

## Objetivo do Sistema
Aplicação desenvolvida em linguagem C para automatizar a triagem, classificação de prioridades e organização de filas de atendimento em uma clínica de especialidades médicas.

## Funcionalidades
- **Registro e Triagem de Pacientes:** Coleta dados do paciente (gênero, idade, gestante, lactante, necessidade especial e obesidade) com validação de entradas.
- **Classificação Automática:**
  - **Urgente (`U001`, `U002`...):** Pacientes com idade superior a 80 anos.
  - **Preferencial (`P001`, `P002`...):** Gestantes, lactantes, pessoas com necessidades especiais, pessoas obesas, mulheres com 60 anos ou mais e homens com 65 anos ou mais.
  - **Normal (`N001`, `N002`...):** Demais pacientes.
- **Gerenciamento de Filas (FIFO + Prioridade):** Atendimento na ordem **Urgente > Preferencial > Normal**, respeitando a ordem de chegada (FIFO) dentro de cada categoria.
- **Controle de Atendimento:** Consulta da próxima senha, chamada de paciente, bloqueio de chamadas simultâneas enquanto há atendimento ativo e finalização de atendimento.
- **Painel Textual em Tempo Real:** Exibe a senha em atendimento, categoria, quantidade de pacientes em espera por categoria e a fila ordenada das próximas senhas.

## Funções Implementadas
- `limparEntrada` / `lerNumero` / `lerSexo`: Leitura e validação das entradas do usuário no terminal.
- `definirPrioridade`: Classifica o paciente em urgente (`3`), preferencial (`2`) ou normal (`1`).
- `montarCodigo`: Gera o código sequencial da senha conforme a categoria (`U`, `P` ou `N`).
- `inserirNaFila` / `retirarDaFila` / `verProximo`: Operações de fila circular (FIFO) para cada categoria.
- `cadastrar`: Coleta os dados do paciente, define a prioridade, gera a senha e insere na fila correspondente.
- `consultarFila` / `chamarProximo` / `encerrarAtendimento`: Gerenciam a consulta, chamada respeitando prioridade e encerramento do atendimento.
- `textoPrioridade` / `mostrarPainel` / `mostrarMenu`: Exibição das informações no painel e do menu de opções.

## Procedimentos de Compilação e Execução

### Pré-requisitos
- Compilador C (`gcc`, `clang` ou equivalente).

### Compilação
```bash
gcc -Wall -Wextra -std=c11 main.c -o clinica

### Execução
Linux / macOS:
./clinica

Windows:
clinica.exe

### Exemplo de Utilização

Cadastro de pacientes:

**Paciente 1:** Masculino, 30 anos, sem condições especiais → Gera senha N001 **(Normal)**

**Paciente 2:** Feminino, 28 anos, gestante → Gera senha P001 **(Preferencial)**

**Paciente 3:** Masculino, 85 anos → Gera senha U001 **(Urgente)**

Painel Exibido antes da chamada:

==========================================
          PAINEL DE ATENDIMENTO
==========================================
Em atendimento: NENHUMA
Aguardando -> Urgente: 1 | Preferencial: 1 | Normal: 1
Proximas senhas: U001 P001 N001 
==========================================

Ordem de chamada:

Ao escolher a opção 3 **(Chamar próximo)**, o sistema chama U001 **(Urgente)** primeiro, depois P001 **(Preferencial)** ao finalizar **(4)**, e por fim N001 **(Normal)**.
