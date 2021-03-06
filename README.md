# Projeto de Compiladores
### Ver [**Enunciado**](https://web.tecnico.ulisboa.pt/~david.matos/w/pt/index.php/Compiladores/Projecto_de_Compiladores/Projecto_2020-2021/Manual_de_Refer%C3%AAncia_da_Linguagem_FIR) e [**Avaliação**](https://web.tecnico.ulisboa.pt/~david.matos/w/pt/index.php/Compiladores/Projecto_de_Compiladores)

---
## Indice
- [Projeto de Compiladores](#projeto-de-compiladores)
    - [Ver **Enunciado** e [**Avaliação**](https://web.tecnico.ulisboa.pt/~david.matos/w/pt/index.php/Compiladores/Projecto_de_Compiladores)](#ver-enunciado-e-avaliação)
  - [Indice](#indice)
  - [Linguagem FIR](#linguagem-fir)
    - [Tipos de Dados](#tipos-de-dados)
    - [Manipulação de Nomes](#manipulação-de-nomes)
    - [Convenções Lexicais](#convenções-lexicais)
      - [**Caracteres brancos**](#caracteres-brancos)
      - [**Comentários**](#comentários)
      - [**Palavras chave**](#palavras-chave)
      - [**Tipos**](#tipos)
      - [**Delimitadores e terminadores**](#delimitadores-e-terminadores)
      - [**Identificadores (nomes)**](#identificadores-nomes)
      - [**Literais**](#literais)
    - [Estrutura/Sintaxe = Gramática](#estruturasintaxe--gramática)
      - [**Declaração de Variáveis**](#declaração-de-variáveis)
      - [**Símbolos Globais**](#símbolos-globais)
    - [Especificação das Funções](#especificação-das-funções)
      - [**Invocação**](#invocação)
      - [**Corpo**](#corpo)
      - [**Função Principal e Execução de Programas**](#função-principal-e-execução-de-programas)
    - [Semântica das Instruções](#semântica-das-instruções)
      - [**Blocos**](#blocos)
      - [**Instrução _leave_**](#instrução-leave)
      - [**Instrução _restart_**](#instrução-restart)
      - [**Instruções de Impressão**](#instruções-de-impressão)
    - [Semântica das Expressões](#semântica-das-expressões)
      - [**Expressões primitivas**](#expressões-primitivas)
      - [**Expressões resultantes de avaliação de operadores**](#expressões-resultantes-de-avaliação-de-operadores)
    - [Exemplos](#exemplos)
  - [Como compilar:](#como-compilar)
  - [Entrega Inicial - dia 2021/03/22 17:00](#entrega-inicial---dia-20210322-1700)
  - [Commitar com CVS](#commitar-com-cvs)
  - [ToDo](#todo)
  - [- olhar para os compiladores ***Simple*** e ***Og***](#--olhar-para-os-compiladores-simple-e-og)
  - [Nodes](#nodes)
    - [Nodes do CKS:](#nodes-do-cks)
    - [Nodes ToDo](#nodes-todo)
  - [Hints](#hints)
<<<<<<< HEAD
  - [Nodes](#nodes)
    - [Nodes do CKS:](#nodes-do-cks)
    - [Nodes ToDo](#nodes-todo)
=======
>>>>>>> 77e90c3ac72c307b0af07eb3086717cb7e876019
  - [Hints](#hints)

---
## Linguagem FIR
FIR é uma linguagem imperativa e é caracterizada por: [tipos de dados](#tipos-de-dados); [manipulação de nomes](#manipulação-de-nomes); [convenções lexicais](#convenções-lexicais); [estrutura/sintaxe](#estrutura/sintaxe-=-gramática); [especificação das funções](#especificação-das-funções); [semântica das instruções](#semântica-das-instruções); [semântica das expressões](#semântica-das-expressões).

No final apresentam-se alguns [exemplos](#exemplos)

---
### Tipos de Dados
Existem 4 tipos de dados, com alinhamento em memória sempre de 32 bits
- **inteiros** - complemento para 2, 4 bytes
- **reais** - vírgula flutuante, 8 bytes
- **cadeiras de caracteres** - vetores de caracteres terminados por ASCII NULL (0x00, '\0' em C)
- **ponteiros** - endereços, 4 bytes
### Manipulação de Nomes
Correspondem a variáveis e funções

O espaço de nomes global é único (um nome não pode ser reutilizado, mesmo que em contexto diferente). Os identificadores são visíveis desde a declaração até ao fim da função (ou do ficheiro, se forem globais). Redeclarações locais encobrem as globais. Não é possível importar ou definir símbolos globais nos contextos das funções.

### Convenções Lexicais
#### **Caracteres brancos**
São considerados separadores e não representam nenhum elemento lexical: mudança de linha ASCII LF (0x0A, '\n' em C), recuo do carreto ASCII CR (0x0D, '\r' em C), espaço ASCII SP (0x20) e tabulação horizontal ASCII HT (0x09, '\t' em C).

#### **Comentários**
- explicativos -- começam com !! e acabam no fim da linha; e
- operacionais -- começam com (* e terminam com *), não podendo estar aninhados.

#### **Palavras chave**
As seguintes palavras estão reservadas:
- int float string void sizeof null
- while do finally leave restart return if then else write writeln

O identificador fir, embora não reservado, quando refere uma função, corresponde à função principal, devendo ser declarado público.

#### **Tipos**
- `int` - inteiro
- `float` - real
- `string` - cadeia de caracteres
- `<` e `>` - ponteiros

#### **Delimitadores e terminadores**
Os seguintes elementos lexicais são delimitadores/terminadores: `,` (vírgula), `;` (ponto e vírgula), e `(` e `)` (delimitadores de expressões).

#### **Identificadores (nomes)**
Iniciados sempre por uma letra

Podem ser seguidos por um 0 (zero), mais letras, dígitos ou `_` (sublinhado)

#### **Literais**
- *inteiros*: um literal inteiro é um número não negativo. Uma constante inteira pode, contudo, ser negativa: números negativos são construídos pela aplicação do operador de negação aritmética unária (-) a um literal positivo
  - Literais inteiros **decimais** são constituídos por sequências de 1 (um) ou mais dígitos de 0 a 9, em que o primeiro dígito não é 0 (zero), excepto no caso do número 0 (zero). Neste caso, é composto apenas pelo dígito 0 (zero) (em qualquer base).
  - Literais inteiros em **octal** começam sempre pelo dígito 0 (zero), sendo seguido de um ou mais dígitos de 0 a 7 (note-se que 09 é um literal octal inválido). Exemplo: 007. Se não for possível representar o literal inteiro na máquina, devido a um overflow, deverá ser gerado um erro lexical.
- *reais em vírgula flutuante*: Os literais reais positivos são expressos tal como em C (apenas é suportada a base 10). Não existem literais negativos (números negativos resultam da aplicação da operação de negação unária). Um literal sem . (ponto decimal) nem parte exponencial é do tipo inteiro. Se não for possível representar o literal real na máquina, devido a um overflow, deverá ser gerado um erro lexical.
- *cadeias de caracteres*:

  As cadeias de caracteres são delimitadas por plicas (') e podem conter quaisquer caracteres, excepto ASCII NULL (0x00 ou \0 em C). Nas cadeias, os delimitadores de comentários não têm significado especial. Se for escrito um literal que contenha o carácter nulo (\~0 em FIR), então a cadeia termina nessa posição. Exemplo: 'ab\~0xy' tem o mesmo significado que 'ab'.

  É possível designar caracteres por sequências especiais (iniciadas por \~), especialmente úteis quando não existe representação gráfica directa. As sequências especiais correspondem aos caracteres ASCII LF, CR e HT (\n, \r e \t, respectivamente, em C e ~n, ~r, ~t, respectivamente, em FIR), plica (\~'), til (\~\~), ou a quaisquer outros especificados através de 1 ou 2 digitos hexadecimais (e.g. \~0a ou apenas \~a se o carácter seguinte não representar um dígito hexadecimal).

  Elementos lexicais distintos que representem duas ou mais cadeias consecutivas são representadas na linguagem como uma única cadeia que resulta da concatenação.

  Exemplos:

  'ab' 'cd' é o mesmo que 'abcd'.
  'ab' (* comentário com 'cadeia de caracteres falsa' *) 'cd' é o mesmo que 'abcd'.

- *ponteiros*: o único literal admissível para ponteiros é indicado pela palavra reservada `null`, indicando o ponteiro nulo

### Estrutura/Sintaxe = Gramática
Ver [Gramática](https://web.tecnico.ulisboa.pt/~david.matos/w/pt/index.php/Compiladores/Projecto_de_Compiladores/Projecto_2020-2021/Manual_de_Refer%C3%AAncia_da_Linguagem_FIR#Gram.C3.A1tica)

#### **Declaração de Variáveis**
- Inteiro: `int i`
- Real: `float f`
- Cadeia de caracteres: `string s`
- Ponteiro para inteiro: `<int> p1` (equivalente a int* em C)
- Ponteiro para real: `<float> p2` (equivalente a double* em C)
- Ponteiro para cadeia de caracteres: `<string> p3` (equivalente a char** em C)
- Ponteiro para ponteiro para inteiro: `<<int>> p4` (equivalente a int** em C)

#### **Símbolos Globais**
O símbolo `*` permite declarar um identificador como público, tornando-o acessível a partir de outros módulos.

O símbolo `?` (opcional para funções) permite declarar num módulo entidades definidas em outros módulos.

### Especificação das Funções
As funções são sempre designadas por identificadores constantes precedidos do tipo de dados devolvido pela função. Se a função não devolver um valor, é declarada como tendo tipo ***void*** para o indicar.

As funções que recebam argumentos devem indicá-los no cabeçalho. Funções sem argumentos definem um cabeçalho vazio. Não é possível aplicar os qualificadores de exportação/importação `*` ou `?` às declarações dos argumentos de uma função. Não é possível especificar valores iniciais (valores por omissão) para argumentos de funções.

Função sem corpo: tipificar um identificador exterior ou para pré-declarar uma função

#### **Invocação**
A função chamadora coloca os argumentos na pilha e é responsável pela sua remoção, após o retorno da chamada. Assim, os parâmetros actuais devem ser colocados na pilha pela ordem inversa da sua declaração (i.e., são avaliados da direita para a esquerda antes da invocação da função e o resultado passado por cópia/valor). O endereço de retorno é colocado no topo da pilha pela chamada à função.

#### **Corpo**
- **Prólogo** (indicado com `@`): sempre executado no início e as variáveis declaradas aqui podem ser usadas nas outras partes. As variáveis declaradas fora do prólogo são visíveis apenas no bloco em que a declaração ocorre
- **Bloco Principal**
- **Epílogo** (indicado com `>>`): é executado no fim da função, mesmo na presença de instrução de retorno 

> Não é possível aplicar os qualificadores de exportação `*` ou de importação `?` em declarações dentro do corpo de uma função

Se existir um valor declarado por omissão para o retorno da função (indicado pela notação `->` seguindo a assinatura da função), então deve ser utilizado se não for especificado outro. A especificação do valor de retorno por omissão é obrigatoriamente um literal do tipo indicado. Uma função cujo retorno seja inteiro ou ponteiro retorna `0` (zero) ou `null` por omissão. Nos outros casos, é indeterminado

> Uma instrução return, em qualquer parte que não o epílogo, causa a interrupção imediata dessa parte da função e a execução da epílogo (se existir). A instrução return, no epílogo, causa a interrupção da própria função, assim como o retorno do seu valor de retorno actual ao chamador.

#### **Função Principal e Execução de Programas**
Um programa inicia-se com a invocação da função fir (sem argumentos). Os argumentos com que o programa foi chamado podem ser obtidos através das seguintes funções:

- int `argc()` (devolve o número de argumentos);
- string `argv(int n)` (devolve o n-ésimo argumento como uma cadeia de caracteres) (n>0); e
- string `envp(int n)` (devolve a n-ésima variável de ambiente como uma cadeia de caracteres)

Valor de retorno da função principal:
- 0 (zero): execução sem erros;
- 1 (um): argumentos inválidos (em número ou valor);
- 2 (dois): erro de execução.
- \>128 (superior a 128): significa que terminou com um sinal 


### Semântica das Instruções

#### **Blocos**
Cada bloco tem uma zona de declaração de variáveis locais e uma zona de instruções (ambas facultativas). Não é possível declarar ou definir funções dentro dos blocos.

As variáveis só são visíveis dentro desse bloco, exceto aquelas definidas no **prólogo**. Podem ser passadas como argumentos para outras funções. 

#### **Instrução _leave_**
A instrução `leave` termina o ciclo mais interior em que a instrução se encontrar, tal como a instrução break em C. Esta instrução só pode existir dentro de um ciclo (não pode ser usada na parte que segue a palavra **finally**), sendo a última instrução do seu bloco. Se for usada com um literal inteiro, termina o ciclo correspondente à ordem indicada pelo inteiro (1 é o mais interno, 2 é o seguinte, etc.).

#### **Instrução _restart_**
A instrução `restart` reinicia o ciclo mais interior em que a instrução se encontrar, tal como a instrução continue em C. Esta instrução só pode existir dentro de um ciclo (não pode ser usada na parte que segue a palavra **finally**), sendo a última instrução do seu bloco. Se for usada com um literal inteiro, reinicia o ciclo correspondente à ordem indicada pelo inteiro (1 é o mais interno, 2 é o seguinte, etc.).

#### **Instruções de Impressão**
Usa-se as palavras `write` e `writeln`

Quando existe mais de uma expressão, as várias expressões são apresentadas sem separação. Valores numéricos (inteiros ou reais) são impressos em decimal. As cadeias de caracteres são impressas na codificação nativa. Ponteiros não podem ser impressos.

### Semântica das Expressões
Ver [Tabela](https://web.tecnico.ulisboa.pt/~david.matos/w/pt/index.php/Compiladores/Projecto_de_Compiladores/Projecto_2020-2021/Manual_de_Refer%C3%AAncia_da_Linguagem_FIR#Express.C3.B5es_primitivas)

#### **Expressões primitivas**
- identificadores: 
  Um identificador é uma expressão se tiver sido declarado. Um identificador pode denotar uma variável.

  Um identificador é o caso mais simples de um left-value, ou seja, uma entidade que pode ser utilizada no lado esquerdo (left) de uma atribuição.

- leitura: 
  A operação de leitura de um valor inteiro ou real pode ser efectuado pela expressão indicada pelo símbolo @, que devolve o valor lido, de acordo com o tipo esperado (inteiro ou real). Caso se use como argumento dos operadores de impressão ou noutras situações que permitam vários tipos (write ou writeln), deve ser lido um inteiro.

  Exemplos: `a = @` (leitura para a), `f(@)` (leitura para argumento de função), `write @` (leitura e impressão).

#### **Expressões resultantes de avaliação de operadores**
- indexação de ponteiros:
  A indexação de ponteiros devolve o valor de uma posição de memória indicada por um ponteiro. Consiste de uma expressão ponteiro seguida do índice entre parênteses rectos. O resultado de uma indexação de ponteiros é um left-value.

  Exemplo (acesso à posição 0 da zona de memória indicada por p): `p[0]`

- identidade e simétrico:
  Os operadores identidade `+` e simétrico `-` aplicam-se a inteiros e reais. Têm o mesmo significado que em C

- reserva de memória:
  A expressão reserva de memória devolve o ponteiro que aponta para a zona de memória, na pilha da função actual, contendo espaço suficiente para o número de objectos indicados pelo seu argumento inteiro.

  Exemplo (reserva vector com 5 reais, apontados por p): `<float>p = [5]`

- expressão de indicação de posição:
  O operador sufixo `?` aplica-se a left-values, retornando o endereço (com o tipo ponteiro) correspondente.

  Exemplo (indica o endereço de a): `a?`

- expressão de dimensão:
  O operador `sizeof` aplica-se a expressões, retornando a dimensão correspondente em bytes.

  Exemplo: `sizeof(a)` (dimensão de a).

### Exemplos
Definição do programa fatorial

Em C:
```C
int *factorial(int n) {
  if n > 1 then factorial = n * factorial(n-1); else factorial = 1;
}
```

Utilização desta função em fir:
```
!! external builtin functions
int ?argc()
string ?argv(int n)
int ?atoi(string s)

!! external user functions
int ?factorial(int n)

!! the main function
int *fir() -> 0
@ { int f = 1; }
{
  writeln 'Teste para a função factorial';
  if argc() == 2 then f = atoi(argv(1));
  writeln f, '! = ', factorial(f);
}
>> { (* nothing to do, really *) }
```

Mais [testes](https://web.tecnico.ulisboa.pt/~david.matos/w/pt/index.php/Compiladores/Projecto_de_Compiladores/Testes_Autom%C3%A1ticos_2020-2021)

Aqui está o acesso aos testes diários

> Tentar colocar sempre o código no CVS (a compilar!) porque todos os dias são realizados testes diários!

---
## Como compilar:
```
fir --target asm factorial.fir
fir --target asm main.fir
yasm -felf32 factorial.asm
yasm -felf32 main.asm
ld -melf_i386 -o main factorial.o main.o -lrts
```

---
## Entrega Inicial - dia 2021/03/22 17:00
Nesta fase, além da estrutura básica do compilador, todas as classes dos nós da linguagem, assim como os esqueletos dos "visitors" (xml_writer, postfix_writer, etc.) devem estar implementados. Não é ainda necessário ter implementado nenhum código de análise lexical, sintáctica ou semântica.

> ver pauta de avaliação, ainda não disponível (?)

---
## Commitar com CVS
A informação do CVS foi retirada deste repositório para evitar conflitos.

Para commitar com o CVS:
1. Fazer novamente checkout do repositório para outra diretoria;
2. Copiar o conteúdo deste repositório para cima dessa diretoria, mergindo as subpastas;
3. Remover README.md;
4. Commitar a partir dessa diretoria.

---
## ToDo
- compilar o simple e olhar para os nodes
- olhar para a pauta para ver o que é suposto ter feito
- [fases de desenvolvimento](https://web.tecnico.ulisboa.pt/~david.matos/w/pt/index.php/Compiladores/Projecto_de_Compiladores/Fases_Desenvolvimento) -> uma fase por entrega?
- olhar para os [compiladores](https://web.tecnico.ulisboa.pt/~david.matos/w/pt/index.php/Compiladores/Projecto_de_Compiladores/Compiladores_Exemplo) ***Simple*** e ***Og***
---

## Nodes
### Nodes do CKS:

```
basic_node
├── data_node
├── nil_node
├── sequence_node
└── typed_node
    ├── expression_node
    │   ├── assignment_node
    │   ├── binary_operation_node
    │   │   ├── add_node
    │   │   ├── and_node
    │   │   ├── div_node
    │   │   ├── eq_node
    │   │   ├── ge_node
    │   │   ├── gt_node
    │   │   ├── le_node
    │   │   ├── lt_node
    │   │   ├── mod_node
    │   │   ├── mul_node
    │   │   ├── ne_node
    │   │   ├── or_node
    │   │   └── sub_node
    │   ├── literal_node
    │   │   ├── double_node
    │   │   ├── integer_node
    │   │   └── string_node
    │   ├── rvalue_node
    │   └── unary_operation_node
    │       ├── neg_node
    │       └── not_node
    └── lvalue_node
        └── variable_node
```

Para cada nó:
- ficheiro .h
  - ifndefs
  - decidir os objetos que tem
- fir_parser.y
  - token
  - stmt
- xml_writer.cpp
- type_checker.cpp
- postfix_
- type_checker.cpp
- postfix_writer.cpp




---
## Hints
- CDK16 da wiki não está 100% correta
- não mexer nos if-then-else que estão no CVS porque "não há outra forma de fazer"
- usar os últimos 4 ficheiros para instalar a máquina virtual (básico para correr o compilador, mas não necessário)
- ver vídeo da aula #2
