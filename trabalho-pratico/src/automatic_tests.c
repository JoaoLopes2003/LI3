#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "catalogo.h"
#include "outputs.h"





// Funcoes responsaveis por realizar os testes automaticos


    // Estruturas auxiliares

typedef struct testes_aux
{
    short int number_input;
    double time;
    int lines;
    bool valid;                     // True = sem erros False = tem erros
    char *errors_info;              // string que guarda informaçao sobre os erros
} Testes_aux;

typedef struct testes
{
    int sp;
    int size;
    int total_lines;
    double media_pesada;
    Testes_aux **arr;
} Testes;
 
    // Inicializa a estrutura

void init_garray (Testes **arr,int len)
{
    int i = 0;

    while (i<len)
    {
        arr[i] = malloc(sizeof(Testes));
        arr[i]->sp = 0;
        arr[i]->size = 4;
        arr[i]->total_lines=0;
        arr[i]->media_pesada=0;
        arr[i]->arr = malloc(arr[i]->size*sizeof(Testes_aux));
        i++;
    }
}

    // Liberta a estrutura

void free_arr (Testes **arr, int len)
{
    int i = 0, j = 0;

    while (i<len)
    {
        while (j<arr[i]->sp)
        {
            free(arr[i]->arr[j]->errors_info);
            j++;
        }
        free(arr[i]->arr);
        i++;
    }
}

    // Funcao que imprime os resultados e do erros conforme o esperado

void print_results (Testes **arr, int len)
{
    int i = 0, j = 0;
    int valid = 0, invalid = 0;
    Testes_aux *test = NULL;

    FILE *errors = fopen("Errors_File.txt","w");

    while (i<len)
    {
        fprintf(errors,"Querie %d total lines: %d -> Average: %lf seconds\n",i+1,arr[i]->total_lines,arr[i]->media_pesada);
        while (j<arr[i]->sp)
        {
            test = arr[i]->arr[j];

            // Se nao houver erros, da print apenas da informaçao dos segundos respetivos à querie

            if (test->valid==TRUE)
            {
                fprintf (errors,"Querie %d -> input number %d -> %f seconds\n",i+1,test->number_input,test->time);
                valid++;
            }
            j++;
        }
        fprintf(errors," \n");
        j = 0;
        i++;
    }
    fprintf (errors,"\n- ERRORS -\n\n\n");
    i = 0;
    while (i<len)
    {
        while (j<arr[i]->sp)
        {
            test = arr[i]->arr[j];

            // No caso de haver erros, informa tambem qual o tipo e, se possivel, a linha do mesmo

            if (test->valid==FALSE)
            {
                fprintf (errors,"Querie %d -> input number %d -> %f seconds -> %s",i+1,test->number_input,test->time,test->errors_info);
                invalid++;
            }
            j++;
        }
        if(test->valid==FALSE) fprintf(errors,"\n");
        j = 0;
        i++;
    }
    fprintf (errors,"\n\nValid queries: %d\nInvalid queries: %d",valid,invalid);
}

    // Compara ficheiros e diz o numero da linha que esta errada

int compare_files (FILE *file1, FILE *file2, char *string,Testes_aux *media_line)
{
    char ch1 = getc(file1);
    char ch2 = getc(file2);
    int error = 0, line = 1;

    while(ch1!=EOF)
    {
        if(ch1=='\n') media_line->lines++;
        ch1 = getc (file1);
    }

    // Testa se ha algum file vazio. Neste caso, alerta para o caso do file ter informacao mas ser suposto estar vazio
    rewind(file1);

    ch1 = getc(file1);
    if(ch1!=EOF && ch2==EOF)
    {
        error++;
        sprintf (string,"The file should be empty.\n");
        return error;
    }

    // Testa se ha algum file vazio. Neste caso, alerta para o caso do file estar vazio mas ser suposto ter informacao

    if((ch1==EOF && ch2!=EOF))
    {
        error++;
        sprintf (string,"The file shouldn't be emtpy.\n");
        return error;
    }

    // No caso de nenhum dos dois estar vazio, percorre todas as linhas do ficheiro

    while (ch1 != EOF && ch2 != EOF)    //EOF = end of file
    {
        if (ch1 == '\n' && ch2 == '\n')
        { 
            line++;
        }

        // Caso hajam chars diferentes, é enunciada a linha onde isso aconteceu

        if (ch1 != ch2)
        {
            error++;
            sprintf (string,"First error in line %d.\n",line);
            return error;
        }
        ch1 = getc (file1);
        ch2 = getc (file2);
    }
    return error;
}

void store_results (double time,int input, int querie, Testes **arr,char *outputs)
{
    // Sao obtidos os caminhos para poder comparar cada output

    char *resultados_name = malloc(sizeof(char)*40);
    char *output_name = malloc(sizeof(char)*40);
    sprintf (resultados_name ,"Resultados/command%d_output.txt",input);
    sprintf (output_name ,"%scommand%d_output.txt",outputs,input);

    FILE *resultados = fopen(resultados_name,"r");
    FILE *output = fopen(output_name,"r");

    // Acrescentamos informação pertinente à estrutura

    Testes_aux *new = malloc(sizeof(Testes_aux));
    char *errors_info = malloc(40*sizeof(char));

    new->number_input = input;
    new->time = time;
    new->valid = NULL;
    new->lines=0;

    if(compare_files(resultados,output,errors_info,new) == 0)
    {
        if(new->time>10)
        {
            // No caso do tempo exceder o suposto, é dada como querie invalida com erro de "tempo excedido"

            new->errors_info = "Time exceeded\n";
            new->valid = FALSE;
        }
        else 
        {
            new->errors_info = NULL;
            new->valid = TRUE;
        }
    }
    else
    {
        new->valid = FALSE;
        new->errors_info = strdup (errors_info);
    }

    // Guardamos a informaçao da estrutura em cima num array em que o index é o numero da querie -1

    if(arr[querie-1]->sp==arr[querie-1]->size)
    {
        arr[querie-1]->size *= 2;
        arr[querie-1]->arr = realloc(arr[querie-1]->arr,arr[querie-1]->size*sizeof(Testes_aux));
    }
    arr[querie-1]->total_lines+=new->lines;
    arr[querie-1]->arr[arr[querie-1]->sp] = new;
    arr[querie-1]->sp++;
    fclose (resultados);
    fclose (output);
    free (errors_info);
    free (resultados_name);
    free (output_name);
}

// Funcao igual à funçao main do projeto, funçao onde tudo acontece, com o acrescimo dos testes

int make_querie (int argc, char *q, char *outputs, FILE *input, FILE *output, Testes **arr)
{
    // Cria a variavel start e end que contam o tempo de cada querie

    clock_t start, end;
    double real_time;
    Catalogos c = NULL;

    if (argc == 4)      
    {   
        c = initCatalogos(q);

        // Argumento do caminho passado incorretamente

        if (c == NULL) return 1;

        char* line = NULL;
        ssize_t read =0;
        size_t len =0;

        int  i = 0;
        while (((read = getline(&line, &len, input)) != -1))
        {
            i++;

            // Cria um ficheiro de output na pasta de resultados, correspondendo à linha do input

            char *nomeFicheiro = malloc(40);
            sprintf(nomeFicheiro ,"Resultados/command%d_output.txt",i);
            output = fopen (nomeFicheiro,"w");
            free(nomeFicheiro);

            // Tira o '\n' da linha

            int t = strlen(line);        
            if (line[t-1] == '\n') line[t-1] = '\0';
            
            // Unico local onde a funçao main realmente difere
            // Passa a linha para a forma de uma querie e executa-a

            QUERIE q = parselinha(line);
            start = clock();
            int querie_num = get_querie_num(q);
            processalinha (q,output,c,true);
            end = clock();

            // Calculamos o tempo real para depois poder guardar na informaçao de cada querie

            real_time = ((double) (end - start))/CLOCKS_PER_SEC;
            store_results (real_time,i,querie_num,arr,outputs);
        }
        fclose(input);
    }

    if (c != NULL) libertaCatalogo (c);
    
    return 0;
}

void media_pesada(Testes **array,int len)
{
    int i=0,j=0;
    while(i<len)
    {
        while(j<array[i]->sp)
        {
            array[i]->media_pesada+=((double) array[i]->arr[j]->lines/(double) array[i]->total_lines)*(array[i]->arr[j]->time);
            j++;
        }
        j=0;
        i++;
    }
}

int main(int argc, char **argv)
{
    // Ficheiro onde vai ser lido o input

    FILE *queries = fopen(argv[3],"r");
    FILE *output = NULL;
    char *pasta = argv[1];
    char *outputs=argv[2];
    Testes **arr = malloc(9*sizeof(Testes));
    init_garray (arr,9);    
    make_querie (argc,pasta,outputs,queries,output,arr);
    media_pesada(arr,9);
    print_results (arr,9);
    fclose (queries);
    free_arr (arr,9);
    free (arr);
    return 0;
}
