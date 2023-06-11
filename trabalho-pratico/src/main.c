#include <string.h>
#include <stdlib.h>
#include "catalogo.h"
#include "outputs.h"
#include "iterativo.h"





int main(int argc, char **argv)
{
    // Variaveis onde vamos guardar o caminho para os ficheiros csv e o catalogo de dados

    char *pasta=NULL;
    Catalogos c = NULL;
    if (argc == 3)      
    {
        // Ficheiro onde vai ser lido o input

        FILE *queries=NULL;
        FILE *output = NULL;

        // Entra no modo batch, input é o segundo argumento

        queries = fopen(argv[2],"r");
        pasta = argv[1];
        c = initCatalogos(pasta);

        // Argumento do caminho passado incorretamente

        if (c == NULL) return 1;

        char* line = NULL;
        ssize_t read = 0;
        size_t len =0;

        int  i = 0;
        while (((read = getline(&line, &len, queries)) != -1))
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
            
            // Passa a linha para a forma de uma querie e executa-a

            QUERIE q = parselinha(line);
            processalinha (q,output,c,true);
        }

        free (line);

        fclose(queries);
        if (c != NULL) libertaCatalogo (c);

    }

   else 
    {
        // Inicialização correspondente a deixar ser possível imprimir no ecrã wide chars, como por exemplo letras com acentos ortográficos (á , ã , í,....)
        setlocale(LC_ALL,"");

        // Inicializa o ecrã, nomeadamente as cores e possibilidade de ler do ecra Keys especiais
        initscr();
        cbreak();
        keypad(stdscr,TRUE);
        start_color();

        // Inicializa pares de cores para as janelas(1) e para o fundo(2)

        init_pair(1, COLOR_WHITE,COLOR_BLACK);
        init_pair(2, COLOR_WHITE,COLOR_BLUE);
        bkgd(COLOR_PAIR(2));

        // Obter a altura e largura da janela

        int height, length;
        getmaxyx(stdscr,height,length);
        
        // Obter as coordenadas centrais do ecra

        height /= 2;
        length /= 2;
        
        mvprintw(height,length -strlen("Introduza o caminho para os ficheiros csv:"),"Introduza o caminho para os ficheiros csv:");

        refresh();
        pasta = malloc(1000);
        
        scanw("%s",pasta);
        clear();
        char *loading = strdup("Os dados estão a ser carregados de momento, por favor aguarde.");        
        printCenter(stdscr,loading,height);

         
        refresh();

        // Enquanto o caminho passado pelo utilizador for incorreto, apresentar mensagem de erro e voltar a pedir input para o caminho dos ficheiros csv

        while ((c = initCatalogos(pasta)) == NULL)
        {   
            errorPath(length,height);
            mvprintw(height,length -strlen("Introduza o caminho para os ficheiros csv:"),"Introduza o caminho para os ficheiros csv:");
            scanw("%s",pasta);
            clear();
            mvprintw(height,length-(strlen(loading)/2),"%s",loading);
            refresh();         
        }
        
        
        free(loading);

        // Deixa de escrever no ecrã as keys pressionadas pelo utilizador

        noecho();

        // Limpa o ecra

        clear();

        bool running = true;

        // Corre o menu até o programa terminar

        MENU m = initMenu(length,height,c);
        while ( running ) running = runMenu(m);
        clear();

        printCenter(stdscr,"Fechando o programa, por favor aguarde",height);
        refresh();
        if (c != NULL) libertaCatalogo (c);

        // Fecha a janela

        endwin();
    }

    return 0;
}
