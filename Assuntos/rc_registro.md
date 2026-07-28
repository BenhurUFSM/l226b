
## Registros (*struct*)

Um registro é uma variável que contém outras variáveis, chamadas 'campos'. Em geral se declara um tipo para o registro, usando `typedef`. Abaixo se cria o tipo `pessoa`, que é um registro contendo os campos `nome`, `idade` e `nota`, cada um de um tipo diferente. A variável `ana` é desse tipo, e a variável `p` é um ponteiro para uma variável desse tipo.
```c
   typedef struct {
       char nome[30];
       int idade;
       float nota;
   } pessoa;

   pessoa ana = { "Ana Maria", 19, 8.2 };
   //...
   ana.nota = (ana.nota + exame) / 2;
   incrementa_idade(&ana);
```
A última linha acima chama uma função passando como parâmetro uma referência para o registro. Essa função poderia ser escrita assim:
```c
   void incrementa_idade(pessoa *p)
   {
       p->idade += 1;   // ou (*p).idade += 1;
   }
```
Pode-se também passar um registro para uma função por valor, em vez de referência. Nesse caso, a função recebe uma cópia do registro.
```c
   //...
   incrementa_idade(ana);
   //...
   void incrementa_idade(pessoa p)
   {
       p.idade += 1;   // altera a idade de p, mas não de ana
   }
```

