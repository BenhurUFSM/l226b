#ifndef _STR_H_
#define _STR_H_

#include "utf8.h"
#include <stdbool.h>

// string (Str)
//
// TAD que implementa strings (cadeias de caracteres) codificadas em utf8
//
// o tipo do TAD é "Str", um ponteiro para o tipo opaco "struct str", definido
//   em str.c
// as operações desse TAD serão prefixadas com "s_"

// Em unicode, cada caractere tem um código (chamado de codepoint) entre 0 e
//   0x10FFFF (exceto entre 0xD800 e 0xDFFF).
// Uma string (deste TAD) é um sequência de tais caracteres, codificados em
//   UTF8, em que cada codepoint é codificado usando entre 1 e 4 bytes.
// Uma string pode conter um número qualquer de caracteres (inclusive nenhum),
//   limitado à capacidade da memória.
// Uma string pode conter quaisquer códigos válidos, **inclusive 0**.

// declarações {{{1

// Str é o tipo de dados para nossas strings
typedef struct str *Str;
// Str_c é uma string não alterável
//   usado para declarar parâmetros que a função não tem o direito de alterar
typedef struct str const *Str_c;

// operações {{{1

// Nas funções abaixo, o argumento `pos` refere-se à posição de um
//   caractere (e não de um byte) em uma string. Esse argumento deve ser
//   interpretado da seguinte forma:
//   - se ele for 0 representa a posição do primeiro caractere da string;
//     se for 1 a do segundo etc
//   - se ele for -1, representa a posição logo após o último caractere da string;
//     se for -2, a posição do último caractere etc

// operações de criação e destruição {{{1

// Aloca, inicializa e retorna uma string que contém uma cópia da
//   string C que inicia em strC.
// A string C deve estar codificada em UTF8, e é delimitada pelo
//   caractere \0 (que não faz parte da string).
// Retorna uma string vazia se strC não contiver UTF8 válido ou se for NULL.
Str s_cria(char const *strC);

// Destrói a string s, liberando a memória que ocupa.
// Essa string não deve ser utilizada após essa chamada.
void s_destroi(Str s);

// Retorna uma nova string, que contém uma substring de s.
// A substring é como descrito em s_substring
// Pode ser implementada assim:
//   Str nova = s_cria("");
//   s_substring(nova, s, pos, tam);
//   return nova;
Str s_cria_substring(Str_c s, int pos, int tam);

// Retorna uma nova string, que contém uma cópia de s.
// Pode ser implementada assim:
//   return s_cria_substring(s, 0, -1);
Str s_cria_cópia(Str_c s);

// Retorna uma nova string com o conteúdo do arquivo chamado nome.
// Retorna uma string vazia em caso de erro.
Str s_cria_de_arquivo(char *nome);

// operações de acesso {{{1

// Retorna o tamanho da string s.
// O tamanho é o número de caracteres, não número de bytes.
int s_tam(Str_c s);

// Retorna uma cópia compatível com string C da string em s, em uma nova
//   memória alocada com malloc de um byte a mais que o número de bytes da
//   string.
// É responsabilidade de quem chama esta função liberar essa memória.
char *s_strc(Str_c s);

// Retorna o código unicode do caractere na posição pos de s.
// A posição pos deve ser interpretada como descrito na seção de
//   declarações deste arquivo.
// Retorna UNI_INV se pos estiver fora dos limites de s.
unichar s_ch(Str_c s, int pos);


// operações de busca e comparação {{{1

// Retorna true se s e sb forem iguais, false caso sejam diferentes.
// Se não forem do mesmo tamanho, são diferentes.
bool s_igual(Str_c s, Str_c sb);

// Retorna a primeira posição em s, não antes de pos, onde tem
//   algum caractere pertencente a sb.
// Retorna -1 se não encontrar.
// A posição pos deve ser interpretada como descrito na seção de
//   declarações deste arquivo.
int s_busca_c(Str_c s, int pos, Str_c sb);

// Retorna a primeira posição em s, não antes de pos, onde tem
//   algum caractere não pertencente a sb.
// Retorna -1 se não encontrar.
// A posição pos deve ser interpretada como descrito na seção de
//   declarações deste arquivo.
int s_busca_nc(Str_c s, int pos, Str_c sb);

// Retorna a última posição em s, anterior a pos, onde tem
//   algum caractere pertencente a sb.
// Retorna -1 se não encontrar.
// A posição pos deve ser interpretada como descrito na seção de
//   declarações deste arquivo.
int s_busca_rc(Str_c s, int pos, Str_c sb);

// Retorna a última posição em s, anterior a pos, onde tem
//   algum caractere não pertencente a sb.
// Retorna -1 se não encontrar.
// A posição pos deve ser interpretada como descrito na seção de
//   declarações deste arquivo.
int s_busca_rnc(Str_c s, int pos, Str_c sb);

// Retorna a primeira posição em s, não antes de pos, onde tem
//   a substring buscada.
// Retorna -1 se não encontrar.
// A posição pos deve ser interpretada como descrito na seção de
//   declarações deste arquivo.
// A string vazia é encontrada em todo lugar 
//   (se buscada for vazia, retorna o valor corrigido de pos).
int s_busca_s(Str_c s, int pos, Str_c buscada);


// operações de alteração {{{1

// Substitui a substring de s que inicia no caractere na posição pos e 
//   vai até o caractere na posição pos+tam-1 pelo conteúdo de sb.
// A posição pos deve ser interpretada como descrito na seção de
//   declarações deste arquivo.
// Caso a posição inicial seja antes do início de s, deve
//   ser corrigida para o início; caso a posição final seja após
//   o final de s, deve ser corrigida para o final.
// Caso tam seja negativo, deve ser interpretado como todos os caracteres
//   desde pos até o final de s.
// Se sb for NULL, deve ser considerada como se fosse uma string vazia.
// Exemplo, se s for "abácaxi":
//   pos=5, tam=2, sb="te", s deve ser transformada em "abácate"
//   7,0,"." -> "abácaxi."
//   -1,5,"." -> "abácaxi."
//   9,0,"." -> "abácaxi."
//   9,0,"" -> "abácaxi"
//   -4,0,"123" -> "abác123axi"
//   -4,1,"123" -> "abác123xi"
//   0,1,"123" -> "123bácaxi"
//   -9,1,"123" -> "123abácaxi"
//   2,200,"" -> "ab"
void s_substitui(Str s, int pos, int tam, Str_c sb);

// Altera s para conter a substring de sb que inicia no caractere na
//   posição pos e vai até o caractere na posição pos+tam-1.
// A definição da substring é como na função s_substitui.
// Por exemplo, se sb contém "bárcó":
//   pos=2,  tam=2  s="rc"
//   pos=2,  tam=4  s="rcó"
//   pos=2,  tam=-1 s="rcó"
//   pos=-3, tam=1  s="c"
//   pos=-7, tam=3  s="bá"
//   pos=10, tam=5  s=""
void s_substring(Str s, Str_c sb, int pos, int tam);

// Altera s para ser uma cópia de sb.
// É equivalente a s_substring(s, sb, 0, -1) (e pode ser implementada assim).
void s_copia(Str s, Str_c sb);

// Insere a string sb na posição pos da string s.
// A posição pos deve ser interpretada como descrito na seção de
//   declarações deste arquivo.
// Equivalente a s_substitui(s, pos, 0, sb);
void s_insere(Str s, int pos, Str_c sb);

// Insere o caractere c na posição pos da string s.
// A posição pos deve ser interpretada como descrito na seção de
//   declarações deste arquivo.
void s_insere_c(Str s, int pos, unichar c);

// Adiciona ao final da string s o conteúdo da string sb.
// Equivalente a s_substitui(s, -1, 0, sb);
void s_anexa(Str s, Str_c sb);

// Adiciona ao final da string s o caractere c.
// Equivalente a s_insere_c(s, -1, c);
void s_anexa_c(Str s, unichar c);

// Remove a substring de tamanho tam iniciando em pos em s.
// pos e tam devem ser interpretados como em s_subst
// Equivalente a s_substitui(s, pos, tam, NULL);
void s_remove(Str s, int pos, int tam);

// Remove eventuais caracteres pertencentes a sobras,
//   do início e do final de s.
// apara("teste 1", " .") -> "teste 1"
// apara("... teste 2. .", " .") -> "teste 2"
void s_apara(Str s, Str_c sobras);

// operações de E/S {{{1

// imprime a string em s na saída padrão
void s_imprime(Str_c s);

// grava o conteúdo de s em um arquivo chamado nome
void s_grava_arquivo(Str_c s, char *nome);


#endif // _STR_H_
// vim: foldmethod=marker shiftwidth=2

