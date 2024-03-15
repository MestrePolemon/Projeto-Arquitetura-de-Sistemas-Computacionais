#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int indice(char *registrador) // Função que retorna o índice do registrador, feito para poder armazenar um resultado para cada registrador
{
    char registradores[32][6] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
                                 "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
                                 "$t8", "$t9", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5",
                                 "$s6", "$s7", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"}; // Vetor com os registradores
    int i;

    for (i = 0; i < 32; i++) // Percorre o vetor de registradores
    {
        if (strcmp(registrador, registradores[i]) == 0) // Compara a string registrador com cada registrador do vetor
        {
            return i;
        }
    }

    printf("Registrador nao encontrado\n"); // Caso o registrador não seja encontrado
    return -1;
}

void transformaBinario(int valor, char *binario) // Função que transforma o valor em binário
{
    for (int i = 31; i >= 0; --i)
    {
        binario[i] = (valor % 2) + '0'; // Pega o resto da divisão por 2
        valor /= 2;                     // Divide o valor por 2
    }
    binario[32] = '\0'; // Adiciona o caractere nulo no final do vetor
}

int transformaInteiro(char *binario) // Função que transforma o valor binário em inteiro
{
    int soma = 0;
    int tamanho = strlen(binario); // Pega o tamanho do vetor
    for (int i = 0; i < tamanho; i++)
    {
        soma += (binario[i] - '0') * pow(2, tamanho - i - 1); // Transforma o valor binário em inteiro
    }
    return soma;
}

int ULA(int codigo, int valor1, int valor2) // Função que simula a ULA
{
    char RSBinario[33], RTBinario[33];
    char RDBinario[33] = {0}; // Vetores para armazenar os valores binários
    int RD;
    switch (codigo) // Switch case para cada instrução
    {

    case 0: // soma
        return valor1 + valor2;
        break;

    case 1: // subtração
        return valor1 - valor2;
        break;

    case 2: // and

        transformaBinario(valor1, RSBinario); // Função que transforma o valor em binário
        transformaBinario(valor2, RTBinario);

        for (int i = 0; i < 32; i++)
        {
            if (RSBinario[i] == '1' && RTBinario[i] == '1')
            {
                RDBinario[i] = '1';
            }
            else
            {
                RDBinario[i] = '0';
            }
        }
        RDBinario[32] = '\0'; // Adiciona o caractere nulo no final do vetor

        RD = transformaInteiro(RDBinario); // Função que transforma o valor binário em inteiro

        return RD; // Retorna o valor inteiro
        break;

    case 3: // or

        transformaBinario(valor1, RSBinario); // Função que transforma o valor em binário
        transformaBinario(valor2, RTBinario);

        for (int i = 0; i < 32; i++)
        {
            if (RSBinario[i] == '1' || RTBinario[i] == '1')
            {
                RDBinario[i] = '1';
            }
            else
            {
                RDBinario[i] = '0';
            }
        }
        RDBinario[32] = '\0'; // Adiciona o caractere nulo no final do vetor

        RD = transformaInteiro(RDBinario);

        return RD;
        break;

    case 4: // beq
        if (valor1 == valor2)
        {
            return 1;
        }
        else
        {
            return 0;
        }
        break;

    case 5: // bne
        if (valor1 != valor2)
        {
            return 1;
        }
        else
        {
            return 0;
        }
        break;

    case 6: // mul
        return valor1 * valor2;
        break;

    case 7: // div
        return valor1 / valor2;
        break;
    }
}

int memoria(int codigo, int valor1, int valor2)
{
    int mem[1000];
    switch (codigo)
    {
    case 0: // lw
        valor1 = mem[valor1 + valor2];
        return valor1;

    case 1: // sw
        mem[valor1 + valor2] = valor1;
        return 0;
    }
}

int main()
{
    int valores[32] = {0}; // Vetor para armazenar os valores dos registradores
    int decimais[32] = {0, 1, 2, 3, 4, 5, 6, 7,
                        8, 9, 10, 11, 12, 13, 14, 15,
                        24, 25, 16, 17, 18, 19, 20, 21,
                        22, 23, 26, 27, 28, 29, 30, 31}; // Vetor com os valores decimais dos registradores
    char comando[100];
    int PC = 0;

    do // Loop para o programa rodar até o usuário digitar exit
    {
        fflush(stdin); // Limpa o buffer do teclado
        printf("\n EXIT para sair\n");
        printf("Insira o comando (add $t0, $s1, $s2): ");
        fgets(comando, 100, stdin); // Use fgets para ler a linha inteira, incluindo espaços

        if (strcmp(comando, "exit\n") == 0 || strcmp(comando, "EXIT\n") == 0) // Verifica se o usuário digitou exit
        {
            break;
        }

        printf("*********************Passo 1*********************\n");
        printf("Armazenar o endereco da instrucao na memoria RAM\n");
        printf("**************************************************\n");

        char *aux = strtok(comando, " "); // strtok com " " para tratar espaços

        printf("\n*********************Passo 2*********************\n");
        printf("Fetch\n");
        printf("Busca a instrucao na memoria e armazena no PC (contador de Programa)\n");
        printf("**************************************************\n");
        // No caso era para o PC armazenar o endereço da próxima instrução, mas como não temos memória, o PC vai trabalhar com valores fixos
        printf("O valor de PC e: %d\n", PC);

        if (aux != NULL && strcmp(aux, "LW") == 0 || aux != NULL && strcmp(aux, "lw") == 0)
        {

            printf("\n*********************Passo 3*********************\n");
            printf("Decode\n");
            printf("Decodifica a instrucao\n");
            printf("**************************************************\n");

            char *RD, *offset, *RS; // Use ponteiros para guardar os registradores

            RD = strtok(NULL, " ,"); // strtok com " ," para tratar vírgulas e espaços
            offset = strtok(NULL, " (");
            RS = strtok(NULL, ")");

            int indiceRD = indice(RD); // Use a função indice para obter o índice do registrador
            int indiceRS = indice(RS);

            if (indiceRD == -1 || indiceRS == -1)
            { // Verifica se algum registrador não foi encontrado
                return 0;
            }

            printf("insira o valor de %s: ", RS);
            scanf("%d", &valores[indiceRS]);

            int valorOffset = atoi(offset); // Transforma o offset em inteiro

            printf("\n*********************Passo 4*********************\n");
            printf("Execute\n");
            printf("Carregara a word na posicao de memoria (RS + offset)\n");
            printf("**************************************************\n");

            memoria(0, valorOffset, valores[indiceRS]);

            printf("\n*********************Passo 5*********************\n");
            printf("Armazena a word no registrador\n");
            printf("**************************************************\n");

            printf("Valor de %s atualizado: %d\n", RD, valores[indiceRD]);

            printf("\n*********************Passo 6*********************\n");
            printf("Incrementa o PC\n");
            printf("**************************************************\n");

            PC += 4; // Incrementa o PC em 4 para apontar para a próxima instrução

            printf("O valor de PC e: %d\n", PC);
        }

        if (aux != NULL && strcmp(aux, "SW") == 0 || aux != NULL && strcmp(aux, "sw") == 0)
        {

            printf("\n*********************Passo 3*********************\n");
            printf("Decode\n");
            printf("Decodifica a instrucao\n");
            printf("**************************************************\n");

            char *RD, *offset, *RS; // Use ponteiros para guardar os registradores

            RD = strtok(NULL, " ,"); // strtok com " ," para tratar vírgulas e espaços
            offset = strtok(NULL, " (");
            RS = strtok(NULL, ")");

            int indiceRD = indice(RD); // Use a função indice para obter o índice do registrador
            int indiceRS = indice(RS);

            if (indiceRD == -1 || indiceRS == -1)
            { // Verifica se algum registrador não foi encontrado
                return 0;
            }

            printf("insira o valor de %s: ", RS);
            scanf("%d", &valores[indiceRS]);

            printf("insira o valor de %s: ", RD);
            scanf("%d", &valores[indiceRD]);

            int valorOffset = atoi(offset); // Transforma o offset em inteiro

            printf("\n*********************Passo 4*********************\n");
            printf("Execute\n");
            printf("Armazenara a word na posicao de memoria (RS + offset)\n");
            printf("**************************************************\n");

            memoria(1, valorOffset, valores[indiceRS]);

            printf("\n*********************Passo 5*********************\n");
            printf("Incrementa o PC\n");
            printf("**************************************************\n");

            PC += 4; // Incrementa o PC em 4 para apontar para a próxima instrução

            printf("O valor de PC e: %d\n", PC);
        }

        if (aux != NULL && strcmp(aux, "ADD") == 0 || aux != NULL && strcmp(aux, "add") == 0)
        { // Use strcmp para comparar strings

            printf("\n*********************Passo 3*********************\n");
            printf("Decode\n");
            printf("Decodifica a instrucao\n");
            printf("**************************************************\n");

            char *RD, *RS, *RT; // Use ponteiros para guardar os registradores

            RD = strtok(NULL, " ,"); // strtok com " ," para tratar vírgulas e espaços
            RS = strtok(NULL, " ,");
            RT = strtok(NULL, " ,");

            RT[strcspn(RT, "\n")] = 0; // Remove o \n do final da string

            printf("Valores decimais:\n");
            printf("RD: %d\n", decimais[indice(RD)]); // Use a função indice para obter o índice do registrador
            printf("RS: %d\n", decimais[indice(RS)]);
            printf("RT: %d\n", decimais[indice(RT)]);
            printf("\n");

            int indiceRD = indice(RD); // Use a função indice para obter o índice do registrador
            int indiceRS = indice(RS);
            int indiceRT = indice(RT);

            if (indiceRD == -1 || indiceRS == -1 || indiceRT == -1)
            { // Verifica se algum registrador não foi encontrado
                return 0;
            }

            printf("insira o valor de %s: ", RS);
            scanf("%d", &valores[indiceRS]);

            printf("Insira o valor de %s: ", RT);
            scanf("%d", &valores[indiceRT]);

            printf("\n*********************Passo 4*********************\n");
            printf("Execute\n");
            printf("A ULA realiza a instrucao\n");
            printf("**************************************************\n");

            valores[indiceRD] = ULA(0, valores[indiceRS], valores[indiceRT]);

            printf("\n*********************Passo 5*********************\n");
            printf("Armazena o resultado no registrador\n");
            printf("**************************************************\n");

            printf("Valor de %s atualizado: %d\n", RD, valores[indiceRD]);

            printf("\n*********************Passo 6*********************\n");
            printf("Incrementa o PC\n");
            printf("**************************************************\n");

            PC += 4; // Incrementa o PC em 4 para apontar para a próxima instrução

            printf("O valor de PC e: %d\n", PC);
        }

        if (aux != NULL && strcmp(aux, "SUB") == 0 || aux != NULL && strcmp(aux, "sub") == 0)
        {
            printf("\n*********************Passo 3*********************\n");
            printf("Decode\n");
            printf("Decodifica a instrucao\n");
            printf("**************************************************\n");

            char *RD, *RS, *RT; // Use ponteiros para guardar os registradores

            RD = strtok(NULL, " ,"); // strtok com " ," para tratar vírgulas e espaços
            RS = strtok(NULL, " ,");
            RT = strtok(NULL, " ,");

            RT[strcspn(RT, "\n")] = 0; // Remove o \n do final da string

            printf("Valores decimais:\n");
            printf("RD: %d\n", decimais[indice(RD)]); // Use a função indice para obter o índice do registrador
            printf("RS: %d\n", decimais[indice(RS)]);
            printf("RT: %d\n", decimais[indice(RT)]);
            printf("\n");

            int indiceRD = indice(RD); // Use a função indice para obter o índice do registrador
            int indiceRS = indice(RS);
            int indiceRT = indice(RT);

            if (indiceRD == -1 || indiceRS == -1 || indiceRT == -1)
            { // Verifica se algum registrador não foi encontrado
                return 0;
            }

            printf("insira o valor de %s: ", RS);
            scanf("%d", &valores[indiceRS]);

            printf("Insira o valor de %s: ", RT);
            scanf("%d", &valores[indiceRT]);

            printf("\n*********************Passo 4*********************\n");
            printf("Execute\n");
            printf("A ULA realiza a instrucao\n");
            printf("**************************************************\n");

            valores[indiceRD] = ULA(1, valores[indiceRS], valores[indiceRT]);

            printf("\n*********************Passo 5*********************\n");
            printf("Armazena o resultado no registrador\n");
            printf("**************************************************\n");

            printf("Valor de %s atualizado: %d\n", RD, valores[indiceRD]);

            printf("\n*********************Passo 6*********************\n");
            printf("Incrementa o PC\n");
            printf("**************************************************\n");

            PC += 4; // Incrementa o PC em 4 para apontar para a próxima instrução

            printf("O valor de PC e: %d\n", PC);
        }

        if (aux != NULL && strcmp(aux, "AND") == 0 || aux != NULL && strcmp(aux, "and") == 0)
        {
            printf("\n*********************Passo 3*********************\n");
            printf("Decode\n");
            printf("Decodifica a instrucao\n");
            printf("**************************************************\n");

            char *RD, *RS, *RT; // Use ponteiros para guardar os registradores

            RD = strtok(NULL, " ,"); // strtok com " ," para tratar vírgulas e espaços
            RS = strtok(NULL, " ,");
            RT = strtok(NULL, " ,");

            RT[strcspn(RT, "\n")] = 0; // Remove o \n do final da string

            printf("Valores decimais:\n");
            printf("RD: %d\n", decimais[indice(RD)]); // Use a função indice para obter o índice do registrador
            printf("RS: %d\n", decimais[indice(RS)]);
            printf("RT: %d\n", decimais[indice(RT)]);
            printf("\n");

            int indiceRD = indice(RD); // Use a função indice para obter o índice do registrador
            int indiceRS = indice(RS);
            int indiceRT = indice(RT);

            if (indiceRD == -1 || indiceRS == -1 || indiceRT == -1)
            { // Verifica se algum registrador não foi encontrado
                return 0;
            }

            printf("insira o valor de %s: ", RS);
            scanf("%d", &valores[indiceRS]);

            printf("Insira o valor de %s: ", RT);
            scanf("%d", &valores[indiceRT]);

            printf("\n*********************Passo 4*********************\n");
            printf("Execute\n");
            printf("A ULA realiza a instrucao\n");
            printf("**************************************************\n");

            valores[indiceRD] = ULA(2, valores[indiceRS], valores[indiceRT]);

            printf("\n*********************Passo 5*********************\n");
            printf("Armazena o resultado no registrador\n");
            printf("**************************************************\n");

            printf("Valor de %s atualizado: %d\n", RD, valores[indiceRD]);

            printf("\n*********************Passo 6*********************\n");
            printf("Incrementa o PC\n");
            printf("**************************************************\n");

            PC += 4; // Incrementa o PC em 4 para apontar para a próxima instrução

            printf("O valor de PC e: %d\n", PC);
        }

        if (aux != NULL && strcmp(aux, "OR") == 0 || aux != NULL && strcmp(aux, "or") == 0)
        {
            printf("\n*********************Passo 3*********************\n");
            printf("Decode\n");
            printf("Decodifica a instrucao\n");
            printf("**************************************************\n");

            char *RD, *RS, *RT; // Use ponteiros para guardar os registradores

            RD = strtok(NULL, " ,"); // strtok com " ," para tratar vírgulas e espaços
            RS = strtok(NULL, " ,");
            RT = strtok(NULL, " ,");

            RT[strcspn(RT, "\n")] = 0; // Remove o \n do final da string

            printf("Valores decimais:\n");
            printf("RD: %d\n", decimais[indice(RD)]); // Use a função indice para obter o índice do registrador
            printf("RS: %d\n", decimais[indice(RS)]);
            printf("RT: %d\n", decimais[indice(RT)]);
            printf("\n");

            int indiceRD = indice(RD); // Use a função indice para obter o índice do registrador
            int indiceRS = indice(RS);
            int indiceRT = indice(RT);

            if (indiceRD == -1 || indiceRS == -1 || indiceRT == -1)
            { // Verifica se algum registrador não foi encontrado
                return 0;
            }

            printf("insira o valor de %s: ", RS);
            scanf("%d", &valores[indiceRS]);

            printf("Insira o valor de %s: ", RT);
            scanf("%d", &valores[indiceRT]);

            printf("\n*********************Passo 4*********************\n");
            printf("Execute\n");
            printf("A ULA realiza a instrucao\n");
            printf("**************************************************\n");

            valores[indiceRD] = ULA(3, valores[indiceRS], valores[indiceRT]);

            printf("\n*********************Passo 5*********************\n");
            printf("Armazena o resultado no registrador\n");
            printf("**************************************************\n");

            printf("Valor de %s atualizado: %d\n", RD, valores[indiceRD]);

            printf("\n*********************Passo 6*********************\n");
            printf("Incrementa o PC\n");
            printf("**************************************************\n");

            PC += 4; // Incrementa o PC em 4 para apontar para a próxima instrução

            printf("O valor de PC e: %d\n", PC);
        }

        if (aux != NULL && strcmp(aux, "beq") == 0 || aux != NULL && strcmp(aux, "BEQ") == 0)
        {
            printf("\n*********************Passo 3*********************\n");
            printf("Decode\n");
            printf("Decodifica a instrucao\n");
            printf("**************************************************\n");

            int valorLabel;
            char *RS, *RT, *label; // Use ponteiros para guardar os registradores

            RS = strtok(NULL, " ,"); // strtok com " ," para tratar vírgulas e espaços
            RT = strtok(NULL, " ,");
            label = strtok(NULL, " ,");

            printf("Valores decimais:\n");
            printf("RT: %d\n", decimais[indice(RT)]); // Use a função indice para obter o índice do registrador
            printf("RS: %d\n", decimais[indice(RS)]);
            printf("\n");

            int indiceRS = indice(RS); // Use a função indice para obter o índice do registrador
            int indiceRT = indice(RT);

            if (indiceRS == -1 || indiceRT == -1)
            { // Verifica se algum registrador não foi encontrado
                return 0;
            }

            printf("insira o valor de %s: ", RT);
            scanf("%d", &valores[indiceRS]);

            printf("Insira o valor de %s: ", RS);
            scanf("%d", &valores[indiceRT]);

            printf("Insira um valor para %s: ", label); // Para fins de funcionamento
            scanf("%d", &valorLabel);

            printf("\n*********************Passo 4*********************\n");
            printf("Execute\n");
            printf("A ULA realiza a instrucao\n");
            printf("**************************************************\n");

            int resultado = ULA(4, valores[indiceRS], valores[indiceRT]);

            printf("\n*********************Passo 6*********************\n");
            printf("Incrementa o PC\n");
            printf("**************************************************\n");

            if (resultado == 1)
            {
                printf("O valor de %s e %s sao iguais\n", RS, RT);
                printf("O PC sera atualizado para o endereco de %s\n", label);
                PC = valorLabel + 4;
            }
            else
            {
                printf("O valor de %s e %s sao diferentes\n", RS, RT);
                printf("O PC sera incrementado em 4\n");
                PC += 4;
            }

            printf("O valor de PC e: %d\n", PC);
        }

        if (aux != NULL && strcmp(aux, "bne") == 0 || aux != NULL && strcmp(aux, "BNE") == 0)
        {
            printf("\n*********************Passo 3*********************\n");
            printf("Decode\n");
            printf("Decodifica a instrucao\n");
            printf("**************************************************\n");

            int valorLabel;
            char *RT, *RS, *label; // Use ponteiros para guardar os registradores

            RT = strtok(NULL, " ,"); // strtok com " ," para tratar vírgulas e espaços
            RS = strtok(NULL, " ,");
            label = strtok(NULL, " ,");

            printf("Valores decimais:\n");
            printf("RT: %d\n", decimais[indice(RT)]); // Use a função indice para obter o índice do registrador
            printf("RS: %d\n", decimais[indice(RS)]);
            printf("\n");

            int indiceRS = indice(RS); // Use a função indice para obter o índice do registrador
            int indiceRT = indice(RT);

            if (indiceRS == -1 || indiceRT == -1)
            { // Verifica se algum registrador não foi encontrado
                return 0;
            }

            printf("insira o valor de %s: ", RT);
            scanf("%d", &valores[indiceRS]);

            printf("Insira o valor de %s: ", RS);
            scanf("%d", &valores[indiceRT]);

            printf("Insira um valor para %s: ", label); // Para fins de funcionamento
            scanf("%d", &valorLabel);

            printf("\n*********************Passo 4*********************\n");
            printf("Execute\n");
            printf("A ULA realiza a instrucao\n");
            printf("**************************************************\n");

            int resultado = ULA(5, valores[indiceRS], valores[indiceRT]);

            printf("\n*********************Passo 6*********************\n");
            printf("Incrementa o PC\n");
            printf("**************************************************\n");

            if (resultado == 1)
            {
                printf("O valor de %s e %s sao diferentes\n", RS, RT);
                printf("O PC sera atualizado para o endereco de %s\n", label);
                PC = valorLabel + 4;
            }
            else
            {
                printf("O valor de %s e %s sao iguais\n", RS, RT);
                printf("O PC sera incrementado em 4\n");
                PC += 4;
            }

            printf("O valor de PC e: %d\n", PC);
        }

        if (aux != NULL && strcmp(aux, "ADDI") == 0 || aux != NULL && strcmp(aux, "addi") == 0)
        {
            printf("\n*********************Passo 3*********************\n");
            printf("Decode\n");
            printf("Decodifica a instrucao\n");
            printf("**************************************************\n");

            char *RT, *RS, *imediato; // Use ponteiros para guardar os registradores

            RT = strtok(NULL, " ,"); // strtok com " ," para tratar vírgulas e espaços
            RS = strtok(NULL, " ,");
            imediato = strtok(NULL, " ,");

            imediato[strcspn(imediato, "\n")] = 0; // Remove o \n do final da string

            printf("Valores decimais:\n");
            printf("RT: %d\n", decimais[indice(RT)]); // Use a função indice para obter o índice do registrador
            printf("RS: %d\n", decimais[indice(RS)]);
            printf("Imediato: %d\n", atoi(imediato));
            printf("\n");

            int indiceRS = indice(RS); // Use a função indice para obter o índice do registrador
            int indiceRT = indice(RT);

            printf("insira o valor de %s: ", RS);
            scanf("%d", &valores[indiceRS]);

            printf("\n*********************Passo 4*********************\n");
            printf("Execute\n");
            printf("A ULA realiza a instrucao\n");
            printf("**************************************************\n");

            valores[indiceRT] = ULA(0, valores[indiceRS], atoi(imediato));

            printf("\n*********************Passo 5*********************\n");
            printf("Armazena o resultado no registrador\n");
            printf("**************************************************\n");

            printf("Valor de %s atualizado: %d\n", RT, valores[indiceRT]);

            printf("\n*********************Passo 6*********************\n");
            printf("Incrementa o PC\n");
            printf("**************************************************\n");

            PC += 4; // Incrementa o PC em 4 para apontar para a próxima instrução

            printf("O valor de PC e: %d\n", PC);
        }

        if (aux != NULL && strcmp(aux, "mul") == 0 || aux != NULL && strcmp(aux, "MUL") == 0)
        {
            printf("\n*********************Passo 3*********************\n");
            printf("Decode\n");
            printf("Decodifica a instrucao\n");
            printf("**************************************************\n");

            char *RD, *RS, *RT; // Use ponteiros para guardar os registradores

            RD = strtok(NULL, " ,"); // strtok com " ," para tratar vírgulas e espaços
            RS = strtok(NULL, " ,");
            RT = strtok(NULL, " ,");

            RT[strcspn(RT, "\n")] = 0; // Remove o \n do final da string

            printf("Valores decimais:\n");
            printf("RD: %d\n", decimais[indice(RD)]); // Use a função indice para obter o índice do registrador
            printf("RS: %d\n", decimais[indice(RS)]);
            printf("RT: %d\n", decimais[indice(RT)]);
            printf("\n");

            int indiceRD = indice(RD); // Use a função indice para obter o índice do registrador
            int indiceRS = indice(RS);
            int indiceRT = indice(RT);

            if (indiceRD == -1 || indiceRS == -1 || indiceRT == -1)
            { // Verifica se algum registrador não foi encontrado
                return 0;
            }

            printf("insira o valor de %s: ", RS);
            scanf("%d", &valores[indiceRS]);

            printf("Insira o valor de %s: ", RT);
            scanf("%d", &valores[indiceRT]);

            printf("\n*********************Passo 4*********************\n");
            printf("Execute\n");
            printf("A ULA realiza a instrucao\n");
            printf("**************************************************\n");

            valores[indiceRD] = ULA(6, valores[indiceRS], valores[indiceRT]);

            printf("\n*********************Passo 5*********************\n");
            printf("Armazena o resultado no registrador\n");
            printf("**************************************************\n");

            printf("Valor de %s atualizado: %d\n", RD, valores[indiceRD]);

            printf("\n*********************Passo 6*********************\n");
            printf("Incrementa o PC\n");
            printf("**************************************************\n");

            PC += 4; // Incrementa o PC em 4 para apontar para a próxima instrução

            printf("O valor de PC e: %d\n", PC);
        }

        if (aux != NULL && strcmp(aux, "div") == 0 || aux != NULL && strcmp(aux, "DIV") == 0)
        {
            printf("\n*********************Passo 3*********************\n");
            printf("Decode\n");
            printf("Decodifica a instrucao\n");
            printf("**************************************************\n");

            char *RD, *RS, *RT; // Use ponteiros para guardar os registradores

            RD = strtok(NULL, " ,"); // strtok com " ," para tratar vírgulas e espaços
            RS = strtok(NULL, " ,");
            RT = strtok(NULL, " ,");

            RT[strcspn(RT, "\n")] = 0; // Remove o \n do final da string

            printf("Valores decimais:\n");
            printf("RD: %d\n", decimais[indice(RD)]); // Use a função indice para obter o índice do registrador
            printf("RS: %d\n", decimais[indice(RS)]);
            printf("RT: %d\n", decimais[indice(RT)]);
            printf("\n");

            int indiceRD = indice(RD); // Use a função indice para obter o índice do registrador
            int indiceRS = indice(RS);
            int indiceRT = indice(RT);

            if (indiceRD == -1 || indiceRS == -1 || indiceRT == -1)
            { // Verifica se algum registrador não foi encontrado
                return 0;
            }

            printf("insira o valor de %s: ", RS);
            scanf("%d", &valores[indiceRS]);

            printf("Insira o valor de %s: ", RT);
            scanf("%d", &valores[indiceRT]);

            printf("\n*********************Passo 4*********************\n");
            printf("Execute\n");
            printf("A ULA realiza a instrucao\n");
            printf("**************************************************\n");

            valores[indiceRD] = ULA(7, valores[indiceRS], valores[indiceRT]);

            printf("\n*********************Passo 5*********************\n");
            printf("Armazena o resultado no registrador\n");
            printf("**************************************************\n");

            printf("Valor de %s atualizado: %d\n", RD, valores[indiceRD]);

            printf("\n*********************Passo 6*********************\n");
            printf("Incrementa o PC\n");
            printf("**************************************************\n");

            PC += 4; // Incrementa o PC em 4 para apontar para a próxima instrução

            printf("O valor de PC e: %d\n", PC);
        }

        if (aux != NULL && strcmp(aux, "j") == 0 || aux != NULL && strcmp(aux, "J") == 0)
        {
            printf("\n*********************Passo 3*********************\n");
            printf("Decode\n");
            printf("Decodifica a instrucao\n");
            printf("**************************************************\n");

            int valorLabel;
            char *label; // Use ponteiros para guardar os registradores

            label = strtok(NULL, " ,"); // strtok com " ," para tratar vírgulas e espaços

            printf("Insira um valor para %s: ", label); // Para fins de funcionamento
            scanf("%d", &valorLabel);

            printf("\n*********************Passo 4*********************\n");
            printf("Execute\n");
            printf("Nao sera usada a ULA\n");
            printf("Neste caso o PC sera atualizado para o endereco da label");
            printf("**************************************************\n");

            printf("O PC sera atualizado para o endereco de %s\n", label);
            PC = valorLabel + 4;

            printf("O valor de PC e: %d\n", PC);
        }
    } while (strcmp(comando, "exit\n") != 0 || strcmp(comando, "EXIT\n") != 0);

    printf("\n*********************Passo 7*********************\n");
    printf("Para o funcionamento\n");
    printf("**************************************************\n");

    return 0;
}