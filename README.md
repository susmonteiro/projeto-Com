# Projeto de Compiladores
### Ver [**Enunciado**](https://web.tecnico.ulisboa.pt/~david.matos/w/pt/index.php/Compiladores/Projecto_de_Compiladores/Projecto_2020-2021/Manual_de_Refer%C3%AAncia_da_Linguagem_FIR) e [**Avaliação**](https://web.tecnico.ulisboa.pt/~david.matos/w/pt/index.php/Compiladores/Projecto_de_Compiladores)

---
## Indice
- [Projeto de Compiladores](#projeto-de-compiladores)
    - [Ver **Enunciado** e [**Avaliação**](https://web.tecnico.ulisboa.pt/~david.matos/w/pt/index.php/Compiladores/Projecto_de_Compiladores)](#ver-enunciado-e-avaliação)
  - [Indice](#indice)
  - [Linguagem FIR](#linguagem-fir)
    - [Tipos de Dados](#tipos-de-dados)
  - [- **ponteiros** - endereços, 4 bytes](#--ponteiros---endereços-4-bytes)
    - [Manipulação de Nomes](#manipulação-de-nomes)
    - [Convenções Lexicais](#convenções-lexicais)
      - [**Caracteres brancos**](#caracteres-brancos)
      - [**Comentários**](#comentários)
      - [**Palavras chave**](#palavras-chave)
      - [**Tipos**](#tipos)
      - [**Delimitadores e terminadores**](#delimitadores-e-terminadores)
      - [**Identificadores (nomes)**](#identificadores-nomes)
    - [Estrutura/Sintaxe](#estruturasintaxe)
    - [Especificação das Funções](#especificação-das-funções)
    - [Semântica das Instruções](#semântica-das-instruções)
    - [Semântica das Expressões](#semântica-das-expressões)
  - [Entrega Inicial - dia 2021/03/22 17:00](#entrega-inicial---dia-20210322-1700)
  - [Commitar com CVS](#commitar-com-cvs)
  - [Hints](#hints)

---
---
## Linguagem FIR
FIR é uma linguagem imperativa e é caracterizada por: [tipos de dados](#tipos-de-dados); [manipulação de nomes](#manipulação-de-nomes); [convenções lexicais](#convenções-lexicais); [estrutura/sintaxe](#estrutura/sintaxe); [especificação das funções](#especificação-das-funções); [semântica das instruções](#semântica-das-instruções); [semântica das expressões](#semântica-das-expressões).

No final apresentam-se alguns [exemplos](#exemplos)

---
### Tipos de Dados
Existem 4 tipos de dados, com alinhamento em memória sempre de 32 bits
- **inteiros** - complemento para 2, 4 bytes
- **reais** - vírgula flutuante, 8 bytes
- **cadeiras de caracteres** - vetores de caracteres terminados por ASCII NULL (0x00, '\0' em C)
- **ponteiros** - endereços, 4 bytes
---
### Manipulação de Nomes
Correspondem a variáveis e funções

O espaço de nomes global é único (um nome não pode ser reutilizado, mesmo que em contexto diferente). Os identificadores são visíveis desde a declaração até ao fim da função (ou do ficheiro, se forem globais). Redeclarações locais encobrem as globais. Não é possível importar ou definir símbolos globais nos contextos das funções.


---
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

### Estrutura/Sintaxe
### Especificação das Funções
### Semântica das Instruções
### Semântica das Expressões


## Entrega Inicial - dia 2021/03/22 17:00
Nesta fase, além da estrutura básica do compilador, todas as classes dos nós da linguagem, assim como os esqueletos dos "visitors" (xml_writer, postfix_writer, etc.) devem estar implementados. Não é ainda necessário ter implementado nenhum código de análise lexical, sintáctica ou semântica.

> ver pauta de avaliação, ainda não disponível (?)

## Commitar com CVS
A informação do CVS foi retirada deste repositório para evitar conflitos.

Para commitar com o CVS:
1. Fazer novamente checkout do repositório para outra diretoria;
2. Copiar o conteúdo deste repositório para cima dessa diretoria, mergindo as subpastas;
3. Commitar a partir dessa diretoria.

## Hints
- CDK16 da wiki não está 100% correta
- não mexer nos if-then-else que estão no CVS porque "não há outra forma de fazer"
- usar os últimos 4 ficheiros para instalar a máquina virtual (básico para correr o compilador, mas não necessário)
