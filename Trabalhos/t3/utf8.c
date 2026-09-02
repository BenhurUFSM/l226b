#include "utf8.h"

#include <stdbool.h>
#include <stdlib.h>

int u8_nbytes_no_unichar_que_comeca_com(byte b1)
{
  if (b1 <= 0b01111111) return 1;
  if (b1 >= 0b11000000 && b1 <= 0b11011111) return 2;
  if (b1 >= 0b11100000 && b1 <= 0b11101111) return 3;
  if (b1 >= 0b11110000 && b1 <= 0b11110111) return 4;
  return -1;
}

// b é um byte de continuação de uma codificação utf8?
static bool u8_é_byte_de_continuacao(byte b)
{
  return (b & 0b11000000) == 0b10000000;
}

// adiciona os 6 bits do byte do continuação cont em *uni
// retorna false se cont não for válido
static bool u8_adiciona_bits_de_continuacao(unichar *uni, byte cont)
{
  if (!u8_é_byte_de_continuacao(cont)) return false;
  byte bits = cont & 0b00111111;
  *uni = ((*uni) << 6) | bits;
  return true;
}

int u8_unichar_nos_bytes(int maxn, byte s[maxn], unichar *puni)
{
  byte primeiro_byte = s[0];
  int nbytes = u8_nbytes_no_unichar_que_comeca_com(primeiro_byte);
  if (nbytes < 1 || nbytes > maxn || nbytes > 4) return -1;

  // monta o código uni à partir dos bytes utf8
  //   máscara binária com os bits válidos para o 1o byte de cada tamanho
  byte mascara[] = { 0b01111111, 0b00011111, 0b00001111, 0b00000111, };
  unichar uni = primeiro_byte & mascara[nbytes - 1];
  int nbytes_adicionados = 1;
  while (nbytes > nbytes_adicionados) {
    if (!u8_adiciona_bits_de_continuacao(&uni, s[nbytes_adicionados])) return -1;
    nbytes_adicionados++;
  }

  // verifica se o código é válido
  if (u8_converte_pra_utf8(uni, NULL) != nbytes) return -1;

  if (puni != NULL) *puni = uni;
  return nbytes;
}

int u8_conta_unichar_nos_bytes(int nbytes, byte s[nbytes])
{
  int num_uni = 0;
  int ind_bytes = 0;
  while (nbytes > 0) {
    int nb1 = u8_unichar_nos_bytes(nbytes, &s[ind_bytes], NULL);
    if (nb1 < 1) return -1;
    num_uni++;
    ind_bytes += nb1;
    nbytes -= nb1;
  }
  return num_uni;
}

byte *u8_avanca_unichar(byte *ptr, int n)
{
  byte *p = ptr;
  for (int i = 0; i < n; i++) {
    int nb1 = u8_nbytes_no_unichar_que_comeca_com(*p);
    if (nb1 < 1) return NULL;
    p += nb1;
  }
  return p;
}

// retorna o número de bytes necessários para codificar uni em utf8
static int u8_nbytes_para_unichar(unichar uni)
{
  if (uni > 0x10FFFF) return -1;
  if (uni >= 0xD800 && uni <= 0xDFFF) return -1;
  if (uni <= 0b0000000001111111) return 1;
  if (uni <= 0b0000011111111111) return 2;
  if (uni <= 0b1111111111111111) return 3;
  return 4;
}

int u8_converte_pra_utf8(unichar uni, byte *s)
{
  int nbytes = u8_nbytes_para_unichar(uni);
  if (nbytes == -1 || s == NULL) return nbytes;

  switch (nbytes) {
    case 1:
      s[0] = uni;
      break;
    case 2:
      s[0] = 0b11000000 | ((uni >>  6) & 0b00011111);
      s[1] = 0b10000000 | ( uni        & 0b00111111);
      break;
    case 3:
      s[0] = 0b11100000 | ((uni >> 12) & 0b00001111);
      s[1] = 0b10000000 | ((uni >>  6) & 0b00111111);
      s[2] = 0b10000000 | ( uni        & 0b00111111);
      break;
    case 4:
      s[0] = 0b11110000 | ((uni >> 18) & 0b00000111);
      s[1] = 0b10000000 | ((uni >> 12) & 0b00111111);
      s[2] = 0b10000000 | ((uni >>  6) & 0b00111111);
      s[3] = 0b10000000 | ( uni        & 0b00111111);
      break;
  }
  return nbytes;
}
