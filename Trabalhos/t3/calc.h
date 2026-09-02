// calc.h
// l226b - t3
// interface da calculadora

#ifndef CALC_H
#define CALC_H

#include "str.h"
#include "lista.h"

// Calcula o valor de expressão e retorna uma nova Str contendo o resultado.
// Em cado de erro, os primeiros caracteres da Str de retorno são "#ERRO ".
Str calculadora(Str expressão);

// Retorna uma nova Lista contendo substrings de txt.
// Uma substring inicia em um caractere diferente de espaço, tabulação,
//   fim de linha.
// Se a substring inicia por um dígito ou um ponto, contém os demais dígitos
//   ou pontos que seguem.
// Se a substring inicia por uma letra ou sublinhado ou `$`, contém os
//   demais letras, sublinhados, `$` ou dígitos que seguem.
// Se a substring inicia por outro caractere, contém somente esse caractere.
// Exemplos:
// " 9. 5" -> ["9." "5"]
// "92+a ba 3b3 ** *  " -> ["92" "+" "a" "ba" "3" "b3" "*" "*" "*"]
Lista tokeniza(Str txt);

#endif // CALC_H
