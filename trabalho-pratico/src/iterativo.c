#include "iterativo.h"





// Modo Iterativo


#define pressAnything "Pressiona uma key para continuar."
#define NumberOptions 10

    // Descrições das opções do menu

char *MenuOptions[] ={
    "1: Listar o resumo do perfil de um condutor ou utilizador ativo, através do seu id ou username, repetivamente.",
    "2: Listar os N condutores com maior avaliação media.",
    "3: Listar os N utilizadores com maior distancia percorrida.",
    "4: Preço médio (sem considerar gorjetas) das viagens numa determinada cidade.",
    "5: Preço médio (sem considerar gorjetas) das viagens num determinado intrevalo de tempo",
    "6: Distância média percorrida numa cidade, num determinado intrevalo de tempo.",
    "7: Ranking de N condutores numa determinada cidade(apenas considerando viagens realizadas nesta),ordenado pela avaliação média do condutor.",
    "8: Listar todas as viagens nas quais o utilizador e o condutor são do género passado como parâmetro, e têm perfis com X ou mais anos.",
    "9: Listar as viagens nas quais o passageiro deu gorjeta, num determinado intrevalo de tempo.",
    "Terminar Programa"
};

    // Estrutura dos menus

struct menu
{
    WINDOW *win;
    int option;
    void * data;
};

    // Cria uma janela com as dimensões passadas como argumento e establece as coordenadas iniciais de forma a que
    // esta janela esteja centrada em xCenter e yCenter

WINDOW *newWindow (int height, int length, int xCenter, int yCenter)
{
    int xStart = xCenter-(length/2);
    int yStart = yCenter-(height/2);
    WINDOW *new = newwin(height,length,yStart,xStart);
    box(new,0,0);

    return new;
}

    // Função que expõe uma mensagem de erro para o ecrã quando o utilizador passa um caminho errado para os ficheiros de dados
    // e de seguida volta a limpar o ecrã quando o utilizador pressiona alguma key

void errorPath (int xCenter,int yCenter)
{
    clear();

    int height = 5, length = 40;

    WINDOW *messageError = newWindow (height ,length,xCenter,yCenter);

    // Cria uma janela centrada no ecrã

    refresh();

    // Desenha uma caixa na janela

    box(messageError,0,0);

    wbkgd(messageError,COLOR_PAIR(1));
    
    char *error = strdup("!ERRO!");
    char *probl = strdup("Caminho para ficheiros csv incorreto.");
    mvwprintw(messageError,1,(length-strlen(error))/2,"%s",error);
    mvwprintw(messageError,2,(length-strlen(probl))/2,"%s",probl);
    mvwprintw(messageError,3,(length-strlen(pressAnything))/2,pressAnything);
    free(error);
    free (probl);
    wrefresh(messageError);
    //refresh();
    getch();
    clear();
}

    // Função que cria o menu com largura 160 e uma altura sufeciente para caberem todas as opções espaçadas por uma linha só.
    // Establece a cor da janela como fundo preto e carateres brancos e cria uma caixa à volta da janela

MENU initMenu (int xCenter,int yCenter,void * c)
{
    MENU m = malloc(sizeof(struct menu));
    m->data = c;
    int height = NumberOptions *2 +1;
    int width = 160;
    m->win = newWindow(height,width,xCenter,yCenter);

    refresh();
    m->option = 0;

    wbkgd(m->win,COLOR_PAIR(1));

    // Torna possivel ler teclas especiais como as setas do computador

    keypad(m->win,TRUE);
    cbreak();

    desenharMenu(m);

    return m;
}

    // Função que trata de executar as funções correspondentes as teclas pressionadas pelo utilizador no menu, retorna true se o programa
    // não tiver acabado, caso contrario false. Isto serve para quebrar o ciclo na função main e terminar o programa principal

bool runMenu (MENU m)
{
    bool notOver = true;
    int key = wgetch(m->win);

     switch (key)
    {
        case KEY_UP:
        case 'W' :
        case 'w' :
            keyUPMenu(m);
            break;
        case KEY_DOWN:
        case 'S' :
        case 's' :
            keyDownMenu(m);
            break;
        case KEY_ENTER :
        case 10:
        case KEY_BACKSPACE :
            notOver = pressEnter(m);
            break;
        default:
            break;
    }

    return notOver;
}

    // Função responsável por desenhar o menu, esta função da print no ecra das opções centradas, estando estas espaçadas por uma linha,
    // quando a opção selecionada vai ser impressa, invertemos as cores de fundo e letra, para ser fácil detetar que esta é a opção selecionada

void desenharMenu (MENU m)
{
    WINDOW *w = m->win;
    noecho();
    //box(m->win,0,0);

    int i;
    char *str;
    for (i =0; i<NumberOptions; i++)
    {
        str = MenuOptions[i];
        if (i == m->option)
        {
            wattron(w,A_REVERSE);
            mvwprintw(w,i*2 +1,(w->_maxx-strlen(str)) /2,"%s",str);
            wattroff(w,A_REVERSE);
        }
        else
        {
            mvwprintw(w,i*2 +1,(w->_maxx-strlen(str)) /2,"%s",str);
        }
    }

    wrefresh(w);
}

    // No menu principal, quando o utilizador se move para a opção mais acima ou mais abaixo,
    // atualizamos a opção escolhida e voltamos a desenhar o menu. Caso o utilizador esteja na
    // primeira opção e tentar mudar para a opção mais acima, ele muda para a ultima opção e vice-versa

void keyUPMenu (MENU m)
{
    // No menu principal, quando o utilizador tenta se mover para a opção mais acima

    WINDOW *w = m->win;


    if (m->option == 0) // Estava na primeira opção, então vai passar para a ultima
        m->option = 9;
    else m->option--;

    desenharMenu(m);


    wrefresh(w);
}



void keyDownMenu (MENU m)
{
    
    WINDOW *w = m->win;

    if (m->option == 9) // estava na ultima opção, então vai passar para a primeira
        m->option = 0;
    else m->option++;
    desenharMenu(m);



    wrefresh(w);
}





// Funcoes instrucoesQuerieN


    /*
        As funções de instrucoesQuerieN seguem todas a mesma estrutura mas ajustada as necessidades de cada uma.

        Começamos por dar print da discrição da querie centrada no topo da janela e do tipo de argumentos que o utilizador vai ter de introduzir. 
        Ativamos o echo para escrever no ecra o input introduzido pelo utilizador.
        Quando o utilizador escreve um argumento da forma errada, quer por não inserir uma data no formato válido, não inserir um número natural da forma válido,etc,
        escrevemos no ecrã uma mensagem de erro e limpamos o input do utilizador, voltamos a pedir o input até este introduzi-lo numa forma válida.

        Criamos uma string com a forma do input "N [ARGS]" na qual N representa o número da querie a ser executada e [ARGS] os argumentos passados, separados com um ' '

        De seguida limpamos a janela e executamos a querie com o input correto.
    */

void instrucoesQuerie1 (MENU m)
{
    WINDOW *w = m->win;

    int meio =w->_maxx / 2;
    char *str = MenuOptions[0];
    printCenter(w,str,2);

    echo();
    
    str = strdup("Id(Condutor) ou Username(Utilizador): ");

    mvwprintw(w,7,meio-strlen(str),"%s",str);

    char *resultado = malloc(w->_maxx/2 +2);
    wrefresh(w);
    wscanw(w,"%s",resultado);
    char *aux =strdup(resultado);

    sprintf(resultado,"1 %s",aux);
    QUERIE q = parselinha(resultado);

    free(aux);
    free(str);
    free(resultado);

    noecho();
    processalinha(q,w,m->data,false);
    
    clearBox(w);
}

void instrucoesQuerie2 (MENU m)
{
    WINDOW *w = m->win;
    int meio =w->_maxx / 2;
    char *str = MenuOptions[1];
    printCenter(w,str,2);
    echo();
    
    str = strdup("Número de condutores: ");

    mvwprintw(w,7,meio-strlen(str),"%s",str);

    char *resultado = malloc(w->_maxx/2 +2);
    wrefresh(w);
    wscanw(w,"%s",resultado);
    int res = intMaiorQue0(resultado);
    while (res == 0)
    {
        mvwprintw(w,8,meio,"Formato errado, por favor inserir numero maior que 0");
        cleanLine(w,7,meio);
        wmove(w,7,meio);
        wrefresh(w);
        wscanw(w,"%s",resultado);
        res = intMaiorQue0(resultado);
    }
    

    sprintf(resultado,"2 %d",res);
    QUERIE q = parselinha(resultado);
    
    
    free(str);
    free(resultado);

    noecho();
    processalinha(q,w,m->data,false);
    
    clearBox(w);
}

void instrucoesQuerie3 (MENU m)
{
    WINDOW *w = m->win;
    int meio =w->_maxx / 2;
    char *str = MenuOptions[2];
    printCenter(w,str,2);
    echo();
    
    str = strdup("Número de utilizadores: ");

    mvwprintw(w,7,meio-strlen(str),"%s",str);

    char *resultado = malloc(w->_maxx/2 +2);
    wrefresh(w);
    wscanw(w,"%s",resultado);
    int res = intMaiorQue0(resultado);
    while (res == 0)
    {
        mvwprintw(w,8,meio,"Formato errado, por favor inserir numero maior que 0");
        cleanLine(w,7,meio);
        wmove(w,7,meio);
        wrefresh(w);
        wscanw(w,"%s",resultado);
        res = intMaiorQue0(resultado);
    }
    

    sprintf(resultado,"3 %d",res);
    QUERIE q = parselinha(resultado);

    free(resultado);
    free(str);

    noecho();
    processalinha(q,w,m->data,false);
    
    clearBox(w);
}


void instrucoesQuerie4 (MENU m)
{
    WINDOW *w = m->win;

    int meio =w->_maxx / 2;
    char *str = MenuOptions[3];
    printCenter(w,str,2);

    echo();
    
    str = strdup("Nome da Cidade: ");

    mvwprintw(w,7,meio-strlen(str),"%s",str);

    char *resultado = malloc(w->_maxx/2 +2);
    wrefresh(w);
    wscanw(w,"%s",resultado);
    char *aux =strdup(resultado);

    sprintf(resultado,"4 %s",aux);
    QUERIE q = parselinha(resultado);

    free(aux);
    free(str);
    free(resultado);

    noecho();
    clearBox(w);
    processalinha(q,w,m->data,false);
    
    clearBox(w);
}

void instrucoesQuerie5 (MENU m)
{
    WINDOW *w = m->win;

    int meio =w->_maxx / 2;
    char *str = MenuOptions[4];
    printCenter(w,str,2);

    echo();


    
    str = strdup("Data inicial: ");
    mvwprintw(w,7,meio-strlen(str),"%s",str);
    free (str);
    str = strdup("Data final: ");
    mvwprintw(w,10,meio-strlen(str),"%s",str);

    int y= 7;
    char *resultado = malloc(w->_maxx/2 +2);
    wrefresh(w);
    int day,month,year;

    mvwscanw(w,y,meio,"%s",resultado);
    while (!checkData(resultado,&day,&month,&year))
    {
        mvwprintw(w,y+1,meio,"Formato errado, por favor inserir data na forma DD/MM/AAAA");
        cleanLine(w,y,meio);
        wrefresh(w);
        mvwscanw(w,y,meio,"%s",resultado);
    }
    
    char *aux1 =strdup(resultado);

    y= 10;
    mvwscanw(w,y,meio,"%s",resultado);
    while (!checkData(resultado,&day,&month,&year))
    {
        mvwprintw(w,y+1,meio,"Formato errado, por favor inserir data na forma DD/MM/AAAA");
        cleanLine(w,y,meio);
        wrefresh(w);
        mvwscanw(w,y,meio,"%s",resultado);
    }    

    char *aux2 =strdup(resultado);
    
    sprintf(resultado,"5 %s %s",aux1,aux2);
    QUERIE q = parselinha(resultado);

    free(str);
    free(aux1);
    free(aux2);
    free(resultado);

    noecho();
    clearBox(w);
    processalinha(q,w,m->data,false);
    
    clearBox(w);
}


void instrucoesQuerie6 (MENU m)
{
    WINDOW *w = m->win;

    int meio =w->_maxx / 2;
    char *str = MenuOptions[5];
    printCenter(w,str,2);

    echo();


    str = strdup("Nome da Cidade: ");
    mvwprintw(w,7,meio-strlen(str),"%s",str);
    free(str);

    str = strdup("Data inicial: ");
    mvwprintw(w,10,meio-strlen(str),"%s",str);
    free (str);

    str = strdup("Data final: ");
    mvwprintw(w,13,meio-strlen(str),"%s",str);
        
    char *resultado = malloc(w->_maxx/2 +2);

    mvwscanw(w,7,meio,"%s",resultado);
    char *aux1 =strdup(resultado);

    int y= 10;
    wrefresh(w);
    
    
    int day,month,year;
    mvwscanw(w,y,meio,"%s",resultado);
    while (!checkData(resultado,&day,&month,&year))
    {
        mvwprintw(w,y+1,meio,"Formato errado, por favor inserir data na forma DD/MM/AAAA");
        cleanLine(w,y,meio);
        wrefresh(w);
        mvwscanw(w,y,meio,"%s",resultado);
    }
    
    char *aux2 =strdup(resultado);

    y= 13;
    mvwscanw(w,y,meio,"%s",resultado);
    while (!checkData(resultado,&day,&month,&year))
    {
        mvwprintw(w,y+1,meio,"Formato errado, por favor inserir data na forma DD/MM/AAAA");
        cleanLine(w,y,meio);
        wrefresh(w);
        mvwscanw(w,y,meio,"%s",resultado);
    }    

    char *aux3 =strdup(resultado);
    
    sprintf(resultado,"6 %s %s %s",aux1,aux2,aux3);
    QUERIE q = parselinha(resultado);

    free(str);
    free(aux1);
    free(aux2);
    free(aux3);
    free(resultado);

    noecho();
    clearBox(w);
    processalinha(q,w,m->data,false);
    
    clearBox(w);
}


void instrucoesQuerie7 (MENU m)
{
    WINDOW *w = m->win;

    int meio =w->_maxx / 2;
    char *str = MenuOptions[6];
    printCenter(w,str,2);

    echo();
    str =strdup("Número de condutores: ");
    mvwprintw(w,7,meio-strlen(str),"%s",str);

    free(str);

    str = strdup("Nome da Cidade: ");
    mvwprintw(w,10,meio-strlen(str),"%s",str);

    char *resultado = malloc(w->_maxx/2 +2);
    wrefresh(w);
    
    mvwscanw(w,7,meio,"%s",resultado);
    int res = intMaiorQue0(resultado);
    while (res == 0)
    {
        mvwprintw(w,8,meio,"Formato errado, por favor inserir numero maior que 0");
        cleanLine(w,7,meio);
        wrefresh(w);
        mvwscanw(w,7,meio,"%s",resultado);
        res = intMaiorQue0(resultado);
    }

    char *aux1 =strdup(resultado);

    mvwscanw(w,10,meio,"%s",resultado);

    char *aux2 = strdup(resultado);

    sprintf(resultado,"7 %s %s",aux1,aux2);
    QUERIE q = parselinha(resultado);

    free(aux1);
    free(aux2);
    free(str);
    free(resultado);

    noecho();
    clearBox(w);
    processalinha(q,w,m->data,false);
    
    clearBox(w);
}


void instrucoesQuerie8 (MENU m)
{
    WINDOW *w = m->win;

    int meio =w->_maxx / 2;
    char *str = MenuOptions[7];
    printCenter(w,str,2);

    echo();
    str =strdup("Género do condutor e utilizador: ");
    mvwprintw(w,7,meio-strlen(str),"%s",str);

    free(str);

    str = strdup("Idade mínima dos perfis: ");
    mvwprintw(w,10,meio-strlen(str),"%s",str);

    char genero,*resultado = malloc(w->_maxx/2 +2);
    //int genero
    wrefresh(w);

    genero = mvwgetch(w,7,meio);
    while (genero != 'M' && genero !=  'F')
    {
        mvwprintw(w,8,meio,"Formato errado,'F' -feminino ,'M' -> masculino");
        cleanLine(w,7,meio);
        wrefresh(w);
        genero = mvwgetch(w,7,meio);
    }


    mvwscanw(w,10,meio,"%s",resultado);
    int res = intMaiorQue0(resultado);
    while (res == 0)
    {
        mvwprintw(w,11,meio,"Formato errado, por favor inserir idade maior que 0");
        cleanLine(w,10,meio);
        wrefresh(w);
        mvwscanw(w,10,meio,"%s",resultado);
        res = intMaiorQue0(resultado);
    }

    char *aux = strdup(resultado);

    sprintf(resultado,"8 %c %s",genero,aux);
    QUERIE q = parselinha(resultado);

    free(aux);
    free(str);
    free(resultado);

    noecho();
    clearBox(w);
    processalinha(q,w,m->data,false);
    
    clearBox(w);
}


void instrucoesQuerie9 (MENU m)
{
    WINDOW *w = m->win;

    int meio =w->_maxx / 2;
    char *str = MenuOptions[8];
    printCenter(w,str,2);

    echo();


    
    str = strdup("Data inicial: ");
    mvwprintw(w,7,meio-strlen(str),"%s",str);
    free (str);
    str = strdup("Data final: ");
    mvwprintw(w,10,meio-strlen(str),"%s",str);

    int y= 7;
    char *resultado = malloc(w->_maxx/2 +2);
    wrefresh(w);
    int day,month,year;

    mvwscanw(w,y,meio,"%s",resultado);
    while (!checkData(resultado,&day,&month,&year))
    {
        mvwprintw(w,y+1,meio,"Formato errado, por favor inserir data na forma DD/MM/AAAA");
        cleanLine(w,y,meio);
        wrefresh(w);
        mvwscanw(w,y,meio,"%s",resultado);
    }
    
    char *aux1 =strdup(resultado);

    y= 10;
    mvwscanw(w,y,meio,"%s",resultado);
    while (!checkData(resultado,&day,&month,&year))
    {
        mvwprintw(w,y+1,meio,"Formato errado, por favor inserir data na forma DD/MM/AAAA");
        cleanLine(w,y,meio);
        wrefresh(w);
        mvwscanw(w,y,meio,"%s",resultado);
    }    

    char *aux2 =strdup(resultado);
    
    sprintf(resultado,"9 %s %s",aux1,aux2);
    QUERIE q = parselinha(resultado);

    free(str);
    free(aux1);
    free(aux2);
    free(resultado);

    noecho();
    clearBox(w);
    processalinha(q,w,m->data,false);
    
    clearBox(w);
}


static void (*interpertator[]) (MENU) = 
{instrucoesQuerie1,instrucoesQuerie2,instrucoesQuerie3,instrucoesQuerie4,
instrucoesQuerie5, instrucoesQuerie6, instrucoesQuerie7, instrucoesQuerie8 , instrucoesQuerie9};

bool pressEnter (MENU m)
{
    // se a opção selecionada for a de fechar o programa, vamos limpar o ecra e retornar false de forma a sabermos que encontramos nos no fim

    if (m->option == 9) 
    {   
        // limpar a janela
         
       clear();
       return false;
    }
    else
    {
        clearBox(m->win);
        (*interpertator[m->option]) (m); 
        
        // Limpar o menu e abrir submenu correpondente à querie selecionada

       desenharMenu(m);
    }
    return true;
}
