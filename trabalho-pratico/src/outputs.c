#include "outputs.h"
#include "auxiliares.h"





// Queries


/*
    Nestas queries, separamos um bocado as funcionalidades, caso o programa seja executado no modo batch , o argumento bool passado vai ser true caso contrário false,
    desta forma, sabemos sempre de que forma vamos interpretar os resultados. No modo batch o void pointer passado corresponde a um ficheiro(FILE *), 
    enquanto que no modo iterativo corresponde a uma janela (WINDOW *)

    No caso modo batch, as coisas são mais simples, todo o output gerado vai ser logo impresso no ficheiro.
    
    Por outro lado, no modo batch temos 2 situações: output que só pode ter uma linha ou output com número variavel de linhas. No primeiro caso, 
    escrevemos o resultado no centro da janela e esperamos apenas o user pressionar Enter ou a spaceBar para terminar a querie e voltar ao menu.
    Quando o output têm várias linhas, separamos este por páginas. Começamos por determinar o número de páginas existentes, através do número de linhas disponíveis para escrita
    e o número de linhas de output, dividindo o segundo pelo primeiro, e posteriormente caso o segundo não seja um múltiplo do primeiro, incrementando o valor.
    Para navegar por páginas, caso existam mais que 1, utilizamos as setas ou A e D para avançar ou recuar, sendo que quando o utilizador pressionar enter ou backSpace voltamos ao menu principal.
*/

    // Querie 1

static void querie1 (char **args, void *output, Catalogos c, bool batchMode)
{
    // Vai buscar o array dos drivers e das viagens e a hashTable dos users ao catálogo

    Drivers arrDrivers = getArrDrivers (c);
    Users users = getHashUsers (c);
    Trips arrTrips = getArrTrips (c);

    // Vai buscar o argumento da querie

    char *argumento = args[0];

    long int idDriver = 0;

    char *name;
    char gender;
    unsigned short int age;
    int numTrips;
    double classification, totalSpentEarned;

    if (sscanf(argumento,"%ld",&idDriver) == 1)
    {
        if (idDriver>0 && idDriver<=numberDrivers(c) && existeDriver(arrDrivers,idDriver) && getArrDriversStatus (arrDrivers, idDriver))
        {
            name = getArrDriversName (arrDrivers, idDriver);
            gender =  getArrDriversGender (arrDrivers, idDriver);
            age = getArrDriversAge (arrDrivers, idDriver);
            numTrips = getArrDriversNumberTrips (arrDrivers, idDriver);
            classification = classificationDrivers (arrTrips, arrDrivers, idDriver);
            totalSpentEarned = totalSpentDrivers (arrTrips, arrDrivers, idDriver);

            // Escrever no ficheiro se estiver no modo batch, se tiver no modo iterativo escrever na terminal

            if  (batchMode) fprintf(output, "%s;%c;%hu;%.3f;%d;%.3f\n", name,gender,age,classification,numTrips,totalSpentEarned);
            else 
            {  
                WINDOW *w =output;
                clearBox(w);
                char *strMax = malloc (w->_maxx);
                sprintf(strMax,"%s;%c;%hu;%.3f;%d;%.3f", name,gender,age,classification,numTrips,totalSpentEarned);
                
                printCenter(w,strMax,w->_maxy/2);
                wrefresh(w);
                
                int key;
                bool loop = true;
                while (loop)
                {
                    key = wgetch(w);
                    switch (key)
                    {
                    case KEY_BACKSPACE:
                    case 10:
                        loop = false;
                    default :
                        break;
                    }
                
                }

                
                free(strMax);
            }

            free (name);
        }
    }
    else if (getStatusUsers (users, argumento))
    {
        char *username = argumento;

        name = getNameUsers (users, username);
        gender =  getGenderUsers (users, username);
        age = getAgeUsers (users, username);
        numTrips = getNumberTripsUsers (users, username);
        classification = classificationUsers (arrTrips, users, username);
        totalSpentEarned = totalSpentUsers (arrTrips, users, arrDrivers, username);

        // Escrever no ficheiro se estiver no modo batch, se tiver no modo iterativo escrever na terminal

       if  (batchMode) fprintf(output, "%s;%c;%hu;%.3f;%d;%.3f\n", name,gender,age,classification,numTrips,totalSpentEarned);
            else 
            {  
                WINDOW *w =output;
                clearBox(w);
                char *strMax = malloc (w->_maxx);
                sprintf(strMax,"%s;%c;%hu;%.3f;%d;%.3f", name,gender,age,classification,numTrips,totalSpentEarned);
                
                printCenter(w,strMax,w->_maxy/2);
                wrefresh(w);
                
                int key;
                bool loop = true;
                while (loop)
                {
                    key = wgetch(w);
                    switch ((key))
                    {
                    case 10:
                    case KEY_BACKSPACE:
                        loop = false;
                    default :
                        break;
                    }
                
                }
                

                
                free(strMax);
            }

            free (name);
    }
}

    // Querie 2

static void querie2 (char **args, void *output, Catalogos c, bool batchMode)
{
    Drivers arrDrivers = getArrDrivers (c);

    // Vai buscar o argumento da querie

    int N = atoi (args[0]);

    // Vai buscar o array dos drivers ordenado por classificação ao catálogo

    DriversClass arrDClass = getArrDClass (c);

    // Escrever no ficheiro se estiver no modo batch, se tiver no modo iterativo escrever na terminal

    if (batchMode)
    {
        for (int i = 0; i<N; i++)
        {
            int id = getIdArrDClass(arrDClass, i);
            char *name = getArrDriversName (arrDrivers, id);
            double classification = getClassArrDClass (arrDClass, i);

            fprintf(output, "%012d;%s;%.3f\n", id, name, classification);
            free (name);
        }
    }
    else
    {
        WINDOW *w =output;
        clearBox(w);
        int yStart,maximoLinhas,divisoes;

        // deixar 2 espaços para a caixa da janlea (y = 0 e y = w->maxy)
        
        maximoLinhas = w->_maxy-1;
        
        divisoes = N/maximoLinhas;
        
        if (N % maximoLinhas != 0) divisoes++;

        
        char *strOutput = malloc (w->_maxx);

        // Se houver mais que 1 janela cheia, vamos ter de separar o output em janelas
        // alternando entre estas usando as setas -> <- e 'a' e 'd' o espaço ou o enter terminam o programa

        bool loop = true;
        int pagI =0;
        while (loop)
        {
            int i = pagI * maximoLinhas,fim;
            if (pagI == divisoes-1) 
            {
                yStart  = (w->_maxy - N +i+1) /2;
                fim = N;
            }
            else 
            {
                yStart = 1;
                fim =i+maximoLinhas;
            }


            mvwprintw(w,1,1,"%d -> %d",i+1,fim);
            while (i<N && yStart<=maximoLinhas)
            {
                int id = getIdArrDClass(arrDClass, i);
                char *name = getArrDriversName (arrDrivers, id);
                double classification = getClassArrDClass (arrDClass, i);

                sprintf(strOutput, "%012d;%s;%.3f", id, name, classification);
                printCenter(w,strOutput,yStart);
                free (name);

                i++;
                yStart++;
            }
            
            int key = wgetch(w);
            switch (key)
            {
                case KEY_RIGHT:
                case 'D' :
                case 'd' :
                    if (pagI != divisoes-1)
                    { 
                        pagI++;
                        clearBox(w);
                    }
                    break;
                case KEY_LEFT:
                case 'A' :
                case 'a' :
                    if (pagI)
                    {
                        pagI--;
                        clearBox(w);
                    }
                    break;
                case 10:
                case KEY_BACKSPACE:
                    loop = false;
                default :
                    break;
            }
        }
        free(strOutput);
    }
            
}

    // Querie 3

static void querie3 (char **args, void *output, Catalogos c, bool batchMode)
{
    // Vai buscar o array dos users ordenado por classificacao e a hash table dos users ao catálogo

    Users users = getHashUsers (c);
    UsersDist arrUDist = getArrUDist (c);

    // Vai buscar o argumento da querie

    int N = atoi (args[0]);

    // Escrever no ficheiro se estiver no modo batch, se tiver no modo iterativo escrever na terminal

    if (batchMode)
    {
        for (int i = 0; i<N; i++)
        {
            char *username = getUsernameArrUDist (arrUDist, i);
            char *name = getNameUsers (users, username);
            int distance = getDistArrUDist (arrUDist, i);

            fprintf(output, "%s;%s;%d\n", username, name, distance);

            free (username);
            free (name);
        }
    }
    else
    {
        WINDOW *w =output;
        clearBox(w);
        int yStart,maximoLinhas,divisoes;

        // Deixar 2 espaços para a caixa da janlea (y = 0 e y = w->maxy)
        
        maximoLinhas = w->_maxy-1;
        
        divisoes = N/maximoLinhas;
        
        if (N % maximoLinhas != 0) divisoes++;

        
        char *strOutput = malloc (w->_maxx);

        // Se houver mais que 1 janela cheia, vamos ter de separar o output em janelas
        // alternando entre estas usando as setas -> <- e 'a' e 'd' o espaço ou o enter terminam o programa

        bool loop = true;
        int pagI =0;
        while (loop)
        {
            int i = pagI * maximoLinhas,fim;
            if (pagI == divisoes-1) 
            {
                yStart  = (w->_maxy - N +i+1) /2;
                fim = N;
            }
            else 
            {
                yStart = 1;
                fim =i+maximoLinhas;
            }


            mvwprintw(w,1,1,"%d -> %d",i+1,fim);
            while (i<N && yStart<=maximoLinhas)
            {

                char *username = getUsernameArrUDist (arrUDist, i);
                char *name = getNameUsers (users, username);
                int distance = getDistArrUDist (arrUDist, i);

                sprintf(strOutput, "%s;%s;%d", username, name, distance);
                printCenter(w,strOutput,yStart);
                free (name);

                i++;
                yStart++;
            }
            
            int key = wgetch(w);
            switch (key)
            {
                case KEY_RIGHT:
                case 'D' :
                case 'd' :
                    if (pagI != divisoes-1)
                    { 
                        pagI++;
                        clearBox(w);
                    }
                    break;
                case KEY_LEFT:
                case 'A' :
                case 'a' :
                    if (pagI)
                    {
                        pagI--;
                        clearBox(w);
                    }
                    break;
                case 10:
                case KEY_BACKSPACE:
                    loop = false;
                default :
                    break;
            }
        }
        free(strOutput);
    }

}

    // Querie 4

static void querie4 (char **args, void *output, Catalogos c, bool batchMode)
{
    // Vai buscar o array dos drivers e das viagens e a hashTable das cidades, que a cada cidade correspode um array de ids de viagens
    // ordenado da viagem vais antiga para a mais recente, ao catalogo

    Trips rides_arr = getArrTrips (c);
    Drivers drivers_arr = getArrDrivers (c);
    CityOrdByDate cityOrdByDate = getCityOrdByDate (c);

    // Vai buscar o argumento da querie

    char *city_name = args[0];

    // Imprime o resultado
    
    printAveragePriceCity (rides_arr, drivers_arr, cityOrdByDate, city_name, output, batchMode);
}

    // Querie 5

static void querie5 (char **args, void *output, Catalogos c, bool batchMode)
{
    // Vai buscar o array dos drivers e das viagens e a hashTable das cidades, que a cada cidade correspode um array de ids de viagens
    // ordenado da viagem vais antiga para a mais recente, ao catalogo

    Trips rides_arr = getArrTrips (c);
    Drivers drivers_arr = getArrDrivers (c);
    CityOrdByDate cityOrdByDate = getCityOrdByDate (c);

    // Vai buscar os argumentos da querie

    unsigned short int s_date = pushData1 (args[0]), b_date = pushData1 (args[1]);

    // Imprime o resultado

    printAveragePriceByDate (rides_arr, drivers_arr, cityOrdByDate, s_date, b_date, output, batchMode);
}

    // Querie 6

static void querie6 (char **args, void *output, Catalogos c, bool batchMode)
{
    // Vai buscar o array das viagens e a hashTable das cidades, que a cada cidade correspode um array de ids de viagens
    // ordenado da viagem vais antiga para a mais recente, ao catalogo

    Trips rides_arr = getArrTrips (c);
    CityOrdByDate cityOrdByDate = getCityOrdByDate (c);

    // Vai buscar os argumentos da querie

    char *city_name = args[0];
    unsigned short int s_date = pushData1 (args[1]), b_date = pushData1 (args[2]);

    // Imprime o resultado

    printAverageDistanceByDate (rides_arr, cityOrdByDate, city_name, s_date, b_date, output, batchMode);
}

    // Querie 7

static void querie7 (char **args, void *output, Catalogos c, bool batchMode)
{
    // Vai buscar o array dos drivers e a hashTable das cidades, que a cada cidade está associado
    // um array de drivers ordenado por ordem decrescente de classificacao, ao catalogo

    Drivers arrayDrivers = getArrDrivers (c);
    OrdScoreDriversByCity ordScoreDriversByCity = getOrdScoreDriversByCity (c);

    // Vai buscar os argumentos da querie

    int N = atoi (args[0]);
    char *city = args[1];

    // Imprime o resultado

    printTopDriversByCity (ordScoreDriversByCity, arrayDrivers, output, N, city, batchMode);
}

    // Querie 8

static void querie8 (char **args, void *output, Catalogos c, bool batchMode)
{
    // Vai buscar o array dos drivers, a hashTable dos users e a estrutura que guarda dois arrays,
    // um apenas com as viagens em que o driver e o user são do sexo masculino e outro em que são do
    // sexo feminino, sendo ambos os arrays ordenados por ordem crescente de data de criacao, ao catalogo

    DriversUsersSameGender driversUsersSameGender = getDriversUsersSameGender (c);
    Drivers drivers = getArrDrivers (c);
    Users users = getHashUsers (c);

    // Vai buscar os argumentos da querie

    char gender = args[0][0];
    int year = atoi (args[1]);

    // Imprime o resultado

    printArrayOrdGender (output, driversUsersSameGender, drivers, users, gender, year, batchMode);
}

// Querie 9

static void querie9 (char **args, void *output, Catalogos c, bool batchMode)
{
    // Vai buscar o array das viagens e a hashTable das cidades, que a cada cidade correspode um array de ids de viagens
    // ordenado da viagem vais antiga para a mais recente, ao catalogo

    CityOrdByDate cityOrdByDate = getCityOrdByDate (c);
    Trips arrayT = getArrTrips (c);

    // Vai buscar os argumentos da querie

    unsigned short int startDate = pushData1 (args[0]);
    unsigned short int limitDate = pushData1 (args[1]);

    // Imprime o resultado

    printfTipsOrsByDistance (cityOrdByDate, arrayT, startDate, limitDate, output, batchMode);
}





// Funcoes ressponsaveis por executar a querie correta


    // Estrutura que guarda a linha de input

struct query{
    short int num;          // numero da querie a ser executada
    char **args;            // argumentos
};

    // Array de funções das queries a serem executadas, ordenadas pelo seu número correspondente

static void (*queriesArr[]) (char **,void *,Catalogos,bool) = {querie1, querie2, querie3, querie4, querie5, querie6, querie7, querie8, querie9};

    // Executa a querie pedida

void processalinha (QUERIE querie, void *fileOrWindow,Catalogos c,bool batchMode)
{
    // Calclua o indice da querie no array de funções

    int op = querie->num-1;

    // Executa a querie pedida, passando como argumentos, os argumentos da querie, o ficheiro onde vai ser efetuada a escrita,
    // o catalogo e se nos encontramos no modo batch

    (*queriesArr[op]) (querie->args,fileOrWindow,c,batchMode);

    // Fecha o ficheiro de escrita dos resultados

    if (batchMode) fclose(fileOrWindow);

    // Liberta o espaco alocado para a querie

    libertaQuerie (querie);
}

    // Processa uma linha de input, organizando os dados

QUERIE parselinha(char *line)
{
    QUERIE linha = malloc (sizeof(struct query));

    // Reserva espaço para um maximo de 3 argumentos numa querie
    
    char **argumentos = malloc (3*sizeof(char*));

    int x;
    char *prov;

    prov = strtok(line," ");

    // O primeiro elemento corrsponde ao número da querie a ser executada, introduzir este na querie

    sscanf(prov,"%d",&x);
    linha->num = x;

    // Faz parsing dos argumentos da querie a ser executada

    int i = 0;
    prov = strtok(NULL," ");
    while (prov != NULL)
    {
        argumentos[i] = strdup(prov);
        prov = strtok(NULL," ");
        i++;
    }
    if (i<3) argumentos[i] = NULL;
    linha->args = argumentos;
    return linha;
}

    // Função que liberta o espaço alocado para uma querie

void libertaQuerie (QUERIE querie)
{
    int i = 0;
    while (i<3 && querie->args[i] != NULL)
    {
        free(querie->args[i]);
        i++;
    }
    free(querie->args);
    free(querie);
}

    // Devolve o numero da querie

short int get_querie_num(struct query *l)
{
    return l->num;
}
