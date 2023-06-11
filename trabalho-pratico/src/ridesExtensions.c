#include "ridesExtensions.h"
#include "auxiliares.h"




// Querie 7


    // Estrutura auxiliar para guardar a informacao dum driver

typedef struct driverScoreCityAux
{
    int id;                         // Id do driver
    int NumberTrips;                // Numero de viagens numa cidade
    double scoreDriver;             // Score total do driver numa cidade
} DriverScoreCityAux;

    // Estrutura usada para passar dados para a funcao iterator que vai ordenar os arrays de drivers por classificacao

typedef struct argQuerie7
{
    GHashTable *cities;             // Hash das cidades em que a cada cidade corresponde um array de drivers ainda nao ordenado
    int nDrivers;                   // Numero total de drivers
} argQuerie7;

    // Estrutura final para guardar a informacao de um driver

typedef struct driversCity
{
    int id;                         // Id do driver
    double class;                   // Classificacao media do driver numa cidade
} DriversCity;

    // Estrutura final

struct ordScoreDriversByCity
{
    GHashTable *cities;             // Hash das cidades em que a cada cidade corresponde um array de drivers ordenado por ordem decrescente de classificao
};


    // Libertam a memoria alocada para a estrutura da querie 7

static void destroyOrdScoreDriversByCity (gpointer key, gpointer value, gpointer data)
{
    free (value);
    free (key);
}

void libertaOrdScoreDriversByCity (OrdScoreDriversByCity ordScoreDriversByCity)
{
    g_hash_table_foreach (ordScoreDriversByCity->cities, destroyOrdScoreDriversByCity, NULL);
    g_hash_table_destroy (ordScoreDriversByCity->cities);
    free (ordScoreDriversByCity);
}

    // Funcao que determina que o array de drivers vai ser ordenado por ordem decrescente de classificacao

static int comparatorDrivers (const void *p, const void *q)
{
    DriverScoreCityAux l = *(DriverScoreCityAux*) p;
    DriverScoreCityAux r = *(DriverScoreCityAux*) q;

    if (l.id==-1)
    {
        return 1;
    }
    else if (r.id==-1)
    {
        return -1;
    }
    else if (l.scoreDriver>r.scoreDriver)
    {
        return -1;
    }
    else if (l.scoreDriver<r.scoreDriver)
    {
        return 1;
    }
    else
    {
        if (l.id>r.id)
        {
            return -1;
        }
        return 1;
    }
}

    // Itera sobre todas as keys da hashTable das cidades, substituindo o array auxiliar desordenado e com informacao desnecessaria,
    // por um array ordenado por ordem decrescente de classificacao e apenas com apenas os dados necessários

static void iterator (gpointer key, gpointer value, gpointer arg)
{
    DriverScoreCityAux *scoresDrivers = (DriverScoreCityAux*) value;
    GHashTable *hashCities = ((argQuerie7*) arg)->cities;
    int nDrivers = ((argQuerie7*) arg)->nDrivers;
    int activeDrivers = 0;

    for (int i = 0;i<nDrivers;i++)
    {
        if (scoresDrivers[i].id!=-1)
        {
            scoresDrivers[i].scoreDriver /= (double) scoresDrivers[i].NumberTrips;
            activeDrivers++;
        }
    }
    
    int size = sizeof(*scoresDrivers)*nDrivers / sizeof(scoresDrivers[0]);

    qsort ((void*)scoresDrivers, size, sizeof(scoresDrivers[0]), comparatorDrivers);

    DriversCity *driversCity = malloc (activeDrivers * sizeof(DriversCity));

    for (int i = 0, position = 0;i<nDrivers;i++)
    {
        if (scoresDrivers[i].id!=-1)
        {
            driversCity[position].id = scoresDrivers[i].id;
            driversCity[position].class = scoresDrivers[i].scoreDriver;
            position++;
        }
    }

    free (scoresDrivers);

    g_hash_table_replace (hashCities, key, driversCity);
}

    // Funcao que percorre o array das rides e cria uma hashTable em que as keys são as cidades e a cada cidade está associado
    // um array de drivers ordenado por ordem decrescente de classificacao

OrdScoreDriversByCity hashScoreDriversbyCity (Drivers arrayD, Trips arrayT, int nDrivers, int nRides)
{
    GHashTable *hashCities = g_hash_table_new (g_str_hash, g_str_equal);

    char *city;
    int id;
    double scoreDriver;

    for (int i = 1;i<=nRides;i++)
    {
        if (verifyTripNULL(arrayT, i))
        {
            city = getCity (arrayT, i);
            id = getDriverT (arrayT, i);
            scoreDriver = getClassTD (arrayT, i);
            DriverScoreCityAux *driversCity;

            if (getArrDriversStatus (arrayD, id))
            {
                if ((driversCity = g_hash_table_lookup (hashCities, city))==NULL)
                {
                    driversCity = malloc (nDrivers * sizeof(DriverScoreCityAux));

                    for (int j = 0;j<nDrivers;j++)
                    {
                        driversCity[j].id = -1;
                        driversCity[j].NumberTrips = 0;
                        driversCity[j].scoreDriver = 0;
                    }

                    g_hash_table_insert (hashCities, city, driversCity);
                }
                else
                {
                    free (city);
                }

                driversCity[id-1].id = id;
                driversCity[id-1].NumberTrips++;
                driversCity[id-1].scoreDriver += scoreDriver;
            }
            else
            {
                free (city);
            }
        }
    }

    argQuerie7 *arg = malloc (sizeof(argQuerie7));
    arg->cities = hashCities;
    arg->nDrivers = nDrivers;

    g_hash_table_foreach (hashCities, iterator, arg);

    OrdScoreDriversByCity ordScoreDriversByCity = malloc(sizeof(OrdScoreDriversByCity));
    ordScoreDriversByCity->cities = hashCities;

    free (arg);

    return ordScoreDriversByCity;
}

    // Funcao que quando chamada a querie 7, percorre o array ordenado de drivers e imprime os N primeiros num ficheiro

void printTopDriversByCity (OrdScoreDriversByCity ordScoreDriversByCity, Drivers arrayrivers, void *output, int N, char *city ,bool batchMode)
{

    DriversCity *driversCity = g_hash_table_lookup (ordScoreDriversByCity->cities, city);

    if (driversCity)
    {
        if (batchMode)
        {
            for (int i = 0;i<N;i++)
            {
                int id = driversCity[i].id;
                char *name = getArrDriversName (arrayrivers, id);
                double scoreDriver = driversCity[i].class;

                fprintf (output, "%012d;%s;%.3f\n", id, name, scoreDriver);

                free (name);
            }
        }
        else
        {
            WINDOW *w =output;
            clearBox(w);
            int yStart,maximoLinhas,divisoes;
            
            maximoLinhas = w->_maxy-1; // deixar 2 espaços para a caixa da janlea (y =0 e y =w ->maxy)
            
            divisoes = N/maximoLinhas;
            
            if (N % maximoLinhas != 0) divisoes++;

            
            char *strOutput = malloc (w->_maxx);

            /*
                se houver mais que 1 janela cheia, vamos ter de separar o output em janelas
                alternando entre estas usando as setas -> <- e 'a' e 'd' o espaço ou o enter terminam o programa

            */ 
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
                    int id = driversCity[i].id;
                    char *name = getArrDriversName (arrayrivers, id);
                    double scoreDriver = driversCity[i].class;

                    sprintf (strOutput, "%012d;%s;%.3f", id, name, scoreDriver);
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
    else if (!batchMode)
    {
        wgetch(output);
    }
}





// Querie 8


    // Estrutura auxiliar para guardar a informacao de um driver e user com o mesmo sexo

typedef struct sameGender
{
    int idTrip;                                 // Id da viagem
    int idDriver;                               // Id do driver
    char *usernameUser;                         // Username do user
    unsigned short int DriverCreationDate;      // Data da criacao da conta do driver
    unsigned short int UserCreationDate;        // Data da criacao da conta do user
} SameGender;

    // Estrutura auxiliar para guardar um array de drivers e users do mesmo sexo

typedef struct array
{
    int sp;                                     // Numero de elementos no array
    int size;                                   // Espaco alocado para o array
    struct sameGender **array;                  // Array de elementos do mesmo sexo
} Array;

    // Estrutura final para guardar a informacao de um driver e user com o mesmo sexo

typedef struct arrayOrdGender
{
    unsigned short int DriverCreationDate;      // Data da criacao da conta do driver
    unsigned short int UserCreationDate;        // Data da criacao da conta do user
    int idDriver;                               // Id do driver
    char *usernameUser;                         // Username do user
} *ArrayOrdGender;

    // Estrutura final

struct driversUsersSameGender
{
    int sizeArrayMales;                         // Numero de elementos no array do sexo masculino
    int sizeArrayFemales;                       // Numero de elementos no array do sexo feminino
    struct arrayOrdGender *males;               // Array de elementos do sexo masculino
    struct arrayOrdGender *females;             // Array de elementos do sexo feminino
};


    // Liberta a memoria alocada para a estrutura da querie 8

void libertaDriversUsersSameGender (DriversUsersSameGender driversUsersSameGender)
{
    int sizeArrayMales = driversUsersSameGender->sizeArrayMales;
    int sizeArrayFemales = driversUsersSameGender->sizeArrayFemales;

    for (int i = 0;i<sizeArrayMales;i++) free (driversUsersSameGender->males[i].usernameUser);
    for (int i = 0;i<sizeArrayFemales;i++) free (driversUsersSameGender->females[i].usernameUser);
    free (driversUsersSameGender->females);
    free (driversUsersSameGender->males);
    free (driversUsersSameGender);
}

    // Inicializa um array de pointers para estruturas que guardam informacoes de drivers e users do mesmo sexo

static Array *initArrayGenders ()
{
    Array *array = malloc (sizeof(Array));
    array->sp = 0;
    array->size = 4;
    array->array = malloc (4 * sizeof(SameGender*));

    for (int i = 0;i<4;i++)
    {
        array->array[i] = malloc (sizeof(SameGender));
    }

    return array;
}

    // Funcao que determina que o array de elementos do mesmo sexo vai ser ordenado por ordem crescente de data de criacao da conta,
    // estando nas primeiras posicoes elementos com data de criacao mais antiga

static int comparatorSameGender (const void *p, const void *q)
{
    SameGender l = **(SameGender**) p;
    SameGender r = **(SameGender**) q;

    if (l.DriverCreationDate<r.DriverCreationDate)
    {
        return -1;
    }
    else if (l.DriverCreationDate>r.DriverCreationDate)
    {
        return 1;
    }
    else
    {
        if (l.UserCreationDate<r.UserCreationDate)
        {
            return -1;
        }
        else if (l.UserCreationDate>r.UserCreationDate)
        {
            return 1;
        }
        else
        {
            if (l.idTrip<r.idTrip)
            {
                return -1;
            }
            return 1;
        }
    }
}

    // Funcao que copia apenas a informacao necessaria de um array para outro, que ocupa apenas o espaço de memória necessario

static ArrayOrdGender copyIdUsername (Array *sameGender)
{
    int nSameGender = sameGender->sp;
    int size = sameGender->size;

    ArrayOrdGender arrayOrdGender = malloc (nSameGender * sizeof(struct arrayOrdGender));

    int i;

    for (i = 0;i<nSameGender;i++)
    {
        arrayOrdGender[i].idDriver = sameGender->array[i]->idDriver;
        arrayOrdGender[i].usernameUser = sameGender->array[i]->usernameUser;
        arrayOrdGender[i].DriverCreationDate = sameGender->array[i]->DriverCreationDate;
        arrayOrdGender[i].UserCreationDate = sameGender->array[i]->UserCreationDate;

        free (sameGender->array[i]);
    }

    for (;i<size;i++)
    {
        free (sameGender->array[i]);
    }

    free (sameGender->array);
    free (sameGender);

    return arrayOrdGender;
}

    // Funcao que percorre o array das rides e cria uma estrutura com dois arrays, um correspondente apenas a elementos
    // do sexo masculino e outro apenas a elementos do sexo feminino, em que para pertencer ao array é necessário numa
    // viagem o driver e user serem do mesmo genero. Este array é ainda ordenado por ordem crescente de data de criacao
    // da conta, estando nas primeiras posicoes elementos com data de criacao mais antiga

DriversUsersSameGender arrayDriversUsersSameGender (Drivers arrayD, Trips arrayT, Users users, int nRides)
{
    Array *males = initArrayGenders ();
    Array *females = initArrayGenders ();

    char genderDriver;
    char genderUser;
    int idDriver;
    char *usernameUser;

    for (int i = 0;i<nRides;i++)
    {
        if (verifyTripNULL(arrayT, i+1))
        {
            genderDriver = getGenderDriverTrips (arrayT, arrayD, i+1);
            genderUser = getGenderUserTrips (arrayT, users, i+1);
            idDriver = getDriverT (arrayT, i+1);
            usernameUser = getUserT (arrayT, i+1);

            if (genderDriver=='M' && genderUser=='M' && getArrDriversStatus (arrayD, idDriver) && getStatusUsers (users, usernameUser))
            {
                if (males->sp==males->size)
                {
                    males->size *= 2;
                    males->array = realloc (males->array, males->size * sizeof(SameGender*));

                    for (int i = males->size/2;i<males->size;i++)
                    {
                        males->array[i] = malloc (sizeof(SameGender));
                    }
                }

                males->array[males->sp]->idDriver = idDriver;
                males->array[males->sp]->usernameUser = usernameUser;
                males->array[males->sp]->DriverCreationDate = getArrDriversCreationDate (arrayD, idDriver);
                males->array[males->sp]->UserCreationDate = getCreationDateUsers (users, usernameUser);;
                males->array[males->sp]->idTrip = i+1;
                (males->sp)++;
            }
            else if (genderDriver=='F' && genderUser=='F' && getArrDriversStatus (arrayD, idDriver) && getStatusUsers (users, usernameUser))
            {
                if (females->sp==females->size)
                {
                    females->size *= 2;
                    females->array = realloc (females->array, females->size * sizeof(SameGender*));

                    for (int i = females->size/2;i<females->size;i++)
                    {
                        females->array[i] = malloc (sizeof(SameGender));
                    }
                }

                females->array[females->sp]->idDriver = idDriver;
                females->array[females->sp]->usernameUser = usernameUser;
                females->array[females->sp]->DriverCreationDate = getArrDriversCreationDate (arrayD, idDriver);
                females->array[females->sp]->UserCreationDate = getCreationDateUsers (users, usernameUser);
                females->array[females->sp]->idTrip = i+1;
                (females->sp)++;
            }
            else
            {
                free (usernameUser);
            }
        }
    }

    int sizeMales = sizeof(*(males->array))*males->sp / sizeof(males->array[0]);
    int sizeFemales = sizeof(*(females->array))*females->sp / sizeof(females->array[0]);

    qsort ((void*)males->array, sizeMales, sizeof(males->array[0]), comparatorSameGender);
    qsort ((void*)females->array, sizeFemales, sizeof(females->array[0]), comparatorSameGender);

    ArrayOrdGender arrayOrdGenderMales = copyIdUsername (males);
    ArrayOrdGender arrayOrdGenderFemales = copyIdUsername (females);

    DriversUsersSameGender driversUsersSameGender = malloc (sizeof(struct driversUsersSameGender));
    driversUsersSameGender->sizeArrayMales = sizeMales;
    driversUsersSameGender->males = arrayOrdGenderMales;
    driversUsersSameGender->sizeArrayFemales = sizeFemales;
    driversUsersSameGender->females = arrayOrdGenderFemales;

    return driversUsersSameGender;
}

    // Funcao que quando chamada a querie 8, percorre o array ordenado de elementos do mesmo sexo e imprime todos os elementos com ano de
    // criacao inferior ao recebido como argumento num ficheiro

void printArrayOrdGender (void *output, DriversUsersSameGender driversUsersSameGender, Drivers drivers, Users users, char gender, int year ,bool batchMode)
{
    unsigned short int limitDate;

    char dateAux[29];

    sprintf (dateAux, "09/10/%d", 2022-year);
    limitDate = pushData1 (dateAux);

    ArrayOrdGender arrayOrdGender;

    if (gender=='M')
    {
        arrayOrdGender = driversUsersSameGender->males;
    }
    else
    {
        arrayOrdGender = driversUsersSameGender->females;
    }

    // criar um array dinamico que guarda os indices das viagens validas para a querie

    int *results= NULL,numberResults,size;
    numberResults = size =0;

    if (!batchMode)
    {


        size = 10;
        results = malloc(size*sizeof(int));

    }  

    for (int i = 0;arrayOrdGender[i].DriverCreationDate<=limitDate;i++)
    {
        if (arrayOrdGender[i].UserCreationDate<=limitDate)
        {
            int idDriver = arrayOrdGender[i].idDriver;
            char *username = arrayOrdGender[i].usernameUser;
            char *nameDriver = getArrDriversName (drivers, idDriver);
            char *nameUser = getNameUsers (users, username);

            // Se tivermos no modo batch, vamos apenas escrever o output no ficheiro, caso contrário vamos ter de adicionar o indice,
            // ao array dos resultados

            if (batchMode)    fprintf (output, "%012d;%s;%s;%s\n", idDriver, nameDriver, username, nameUser);
            else
            {
                if (numberResults == size)
                {
                    size *=2;
                    results = realloc(results,size*sizeof(int));
                }
                results[numberResults] = i;//
                numberResults++;


            }
            free (nameDriver);
            free (nameUser);
        }
    }

    if(!batchMode)
    {
        WINDOW *w =output;
        clearBox(w);
        int yStart,maximoLinhas,divisoes;

        // Deixar 2 espaços para a caixa da janlea (y = 0 e y = w->maxy)
        
        maximoLinhas = w->_maxy-1;
        
        divisoes = numberResults/maximoLinhas;
        
        if (numberResults % maximoLinhas != 0) divisoes++;

        char *strAux = malloc(w->_maxx);

        // Se houver mais que 1 janela cheia, vamos ter de separar o output em janelas
        // alternando entre estas usando as setas -> <- e 'a' e 'd' o espaço ou o enter terminam o programa

        bool loop = true;
        int pagI =0,index;
        while (loop)
        {
            int i = pagI * maximoLinhas,fim;
            if (pagI == divisoes-1) 
            {
                yStart  = (w->_maxy - numberResults +i+1) /2;
                fim = numberResults;
            }
            else 
            {
                yStart = 1;
                fim =i+maximoLinhas;
            }


            mvwprintw(w,1,1,"%d -> %d",i+1,fim);
            while (i<numberResults && yStart<=maximoLinhas)
            {
                
                index = results[i];
                int idDriver = arrayOrdGender[index].idDriver;
                char *username = arrayOrdGender[index].usernameUser;
                char *nameDriver = getArrDriversName (drivers, idDriver);
                char *nameUser = getNameUsers (users, username);

                sprintf (strAux, "%012d;%s;%s;%s", idDriver, nameDriver, username, nameUser);

                free(nameDriver);
                free(nameUser);

                // Ecrever no ficheiro se estiver no modo batch, se tiver no modo iterativo escrever na terminal
                
                printCenter(w,strAux,yStart);
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

        

        free(strAux);
        free(results);
    }
}





// Estrutura para as queries 4, 5, 6 e 9


    // Estrutura auxiliar para guardar a informacao duma viagem

typedef struct idsDatesAux
{
    int id;                             // Id da viagem
    unsigned short int date;            // Data da viagem
} IdsDatesAux;

    // Estrutura auxiliar para guardar a informacao de um array de viagens

typedef struct cityTripsAux
{
    int sp;                             // Numero de elementos no array
    int size;                           // Espaco alocado para o array
    struct idsDatesAux *idsDates;       // Array de viagens
} CityTripsAux;

    // Estrutura final para guardar um array de viagens ordenado da viagem vais antiga para a mais recente

typedef struct cityTrips
{
    int sp;                             // Numero de elementos no array
    int *ids;                           // Array de ids de viagens ordenado da viagem mais antiga para a mais recente
} CityTrips;

    // Estrutura final

struct cityOrdByDate
{
    GHashTable *cities;                 // Hash das cidades em que a cada cidade corresponde um array de viagens ordenado da viagem vais antiga para a mais recente
};

    // Libertam a memoria alocada para a estrutura da querie 4, 5, 6 e 9

static void destroyCityOrdByDate (gpointer key, gpointer value, gpointer data)
{
    CityTrips *ordCitiesByDate = (CityTrips*) value;
    free (ordCitiesByDate->ids);
    free (ordCitiesByDate);
    free (key);
}

void libertaCityOrdByDate (CityOrdByDate ordCitiesByDate)
{
    g_hash_table_foreach (ordCitiesByDate->cities, destroyCityOrdByDate, NULL);
    g_hash_table_destroy (ordCitiesByDate->cities);
    free (ordCitiesByDate);
}

    // Inicializa um array de estruturas que guardam informacoes de viagens

static CityTripsAux *inicializeCityTrips ()
{
    CityTripsAux *cityTrips = malloc(sizeof(CityTripsAux));
    cityTrips->sp = 0;
    cityTrips->size = 20;
    cityTrips->idsDates = malloc (20 * sizeof(IdsDatesAux));

    return cityTrips;
}

    // Preenche o array auxiliar de viagens

static void insertIdsCity (CityTripsAux *cityTrips, int id, unsigned short int date)
{
    if (cityTrips->sp==cityTrips->size)
    {
        cityTrips->size *= 2;
        cityTrips->idsDates = realloc (cityTrips->idsDates, cityTrips->size * sizeof(IdsDatesAux));
    }

    cityTrips->idsDates[cityTrips->sp].id = id;
    cityTrips->idsDates[cityTrips->sp].date = date;
    (cityTrips->sp)++;
}

    // Funcao que determina que o array de viagens vai ser ordenado da viagem vais antiga para a mais recente

static int comparatorDatesTrips (const void *p, const void *q)
{
    IdsDatesAux l = *(IdsDatesAux*) p;
    IdsDatesAux r = *(IdsDatesAux*) q;

    if (l.date<r.date)
    {
        return -1;
    }
    return 1;
}

    // Funcao que copia apenas a informacao necessaria de um array para outro, que ocupa apenas o espaço de memória necessario

static CityTrips *copyToArrayIds (CityTripsAux *cityTripsAux, int numberIds)
{
    CityTrips *cityTrips = malloc (sizeof(CityTrips));
    cityTrips->ids = malloc (numberIds * sizeof(int));
    cityTrips->sp = numberIds;

    int i;

    for (i = 0;i<numberIds;i++)
    {
        cityTrips->ids[i] = cityTripsAux->idsDates[i].id;
    }

    return cityTrips;
}

    // Itera sobre todas as keys da hashTable das cidades, substituindo o array auxiliar desordenado e com informacao desnecessaria,
    // por um array ordenado da viagem vais antiga para a mais recente e apenas com apenas os dados necessários

static void iteratorTripsByCity (gpointer key, gpointer value, gpointer hashCityTrips)
{
    CityTripsAux *cityTripsAux = (CityTripsAux*) value;
    int sp = ((CityTripsAux*) value)->sp;
    IdsDatesAux *tripsCity = ((CityTripsAux*) value)->idsDates;
    
    int size = sizeof(*(tripsCity)) * sp / sizeof(tripsCity[0]);

    qsort ((void*)tripsCity, size, sizeof(tripsCity[0]), comparatorDatesTrips);

    CityTrips *cityTrips = copyToArrayIds (cityTripsAux, sp);

    free (cityTripsAux->idsDates);
    free (cityTripsAux);

    g_hash_table_replace (hashCityTrips, key, cityTrips);
}

    // Funcao que percorre o array das rides e cria uma hashTable em que as keys são as cidades e a cada cidade está associado
    // um array de ids de viagens ordenado da viagem vais antiga para a mais recente

CityOrdByDate ordCitiesByDate (Trips arrayT, int nTrips)
{
    GHashTable *hashCityTrips = g_hash_table_new (g_str_hash, g_str_equal);

    char *city;
    unsigned short int date;

    for (int i = 0;i<nTrips;i++)
    {
        if (verifyTripNULL(arrayT, i+1))
        {
            city = getCity (arrayT, i+1);
            date = getDateArrTrips (arrayT, i+1);
            CityTripsAux *cityTrips;

            if ((cityTrips = g_hash_table_lookup (hashCityTrips, city))==NULL)
            {
                cityTrips = inicializeCityTrips ();
                g_hash_table_insert (hashCityTrips, city, cityTrips);
            }
            else
            {
                free (city);
            }

            insertIdsCity (cityTrips, i+1, date);
        }
    }

    g_hash_table_foreach (hashCityTrips, iteratorTripsByCity, hashCityTrips);

    CityOrdByDate cityOrdByDate = malloc (sizeof(struct cityOrdByDate));
    cityOrdByDate->cities = hashCityTrips;
    
    return cityOrdByDate;
}





// Querie 4


    // Funcao que quando chamada a querie 4, percorre o array todo de viagens de uma cidade ordenado da viagem mais antiga para a mais recente e
    // imprime o preço médio das viagens nessa cidade num ficheiro

void printAveragePriceCity (Trips rides_arr, Drivers drivers_arr, CityOrdByDate cityOrdByDate, char *city_name, void *output ,bool batchMode)
{
    int trip = 0, distance, trip_id;
    double price = 0;
    char class;

    CityTrips *city = g_hash_table_lookup (cityOrdByDate->cities, city_name);

    if (city!=NULL)
    {
        int length = city->sp;
        while(trip<length)
        {
            trip_id = city->ids[trip];
            distance = getDistTrips (rides_arr,trip_id);
            class = getArrDriversClass (drivers_arr,getDriverT(rides_arr,trip_id));
            price += preco (distance,class);
            trip++;
        }
        double media = price / (double) length;

        if (batchMode)fprintf(output, "%.3lf\n",media);
        else 
        {
            WINDOW *w = output;
            char *straux =malloc (w->_maxx);
            sprintf(straux, "%.3lf",media);
 
            printCenter(w,straux,w->_maxy/2);
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

            free(straux);
        }


    }
}



// Queire 5


    // Funcao que quando chamada a querie 5, percorre os array de viagens de todas as cidades ordenados da viagem mais antiga para a mais recente e
    // imprime o preço médio das viagens num determinado intervalo de tempo num ficheiro

void printAveragePriceByDate (Trips rides_arr, Drivers drivers_arr, CityOrdByDate cityOrdByDate, unsigned short int s_date, unsigned short int b_date, void *output ,bool batchMode)
{
    int i = 0, number = 0, length, trip_id, distance;
    double price = 0;
    char class;

    // Passa todas as keys da hashtable para um array

    gpointer *keys_arr = g_hash_table_get_keys_as_array(cityOrdByDate->cities,NULL);
    CityTrips *city;

    while(keys_arr[i]!=NULL)
    {
        city = g_hash_table_lookup (cityOrdByDate->cities, keys_arr[i]);
        length = city->sp;
        int *ids = city->ids;

        // Faz uma especie de binary search para encontrar a posicao com a data de comeco do intervalo

        int indiceStartDate;
        int indiceLimitDate;
        int stop = 0;

        int auxMax = length-1, auxMin = 0, middle = 0;

        if (s_date<=getDateArrTrips(rides_arr,ids[length-1]) && b_date>=getDateArrTrips(rides_arr,ids[0]))
        {
            while (auxMin<=auxMax && !stop)
            {
                middle = (auxMin+auxMax) / 2;

                if (getDateArrTrips(rides_arr,ids[middle]) == s_date) stop++;
                else if (getDateArrTrips(rides_arr,ids[middle]) < s_date)
                {
                    auxMin = middle+1;
                }
                else auxMax = middle-1;
            }

            indiceStartDate = middle;

            // Verifica se a posicao realmente corresponde a uma viagem na data desejada e,
            // se corresponder à data desejada, percorre o array até encontrar a primeira posicao com a mesma data.
            // Caso não seja a data pretendida, procura a primeira posicao da data mais proxima da data pretendida, que exista e
            // que seja superior à data pretendida.

            if (getDateArrTrips(rides_arr,ids[middle])==s_date)
            {
                for (int j = 1;middle-j>0 && getDateArrTrips(rides_arr,ids[middle-j])==s_date;indiceStartDate--,j++);
            }
            else if (getDateArrTrips(rides_arr,ids[middle])>s_date)
            {
                for (int j = 1;middle-j>0 && getDateArrTrips(rides_arr,ids[middle-j])>s_date;indiceStartDate--,j++);
            }
            else
            {
                for (int j = 1;middle+j<length && getDateArrTrips(rides_arr,ids[middle+j])<s_date;indiceStartDate++,j++);
                indiceStartDate++;
            }

            // Faz uma especie de binary search para encontrar a posicao com a data em que termina o intervalo

            auxMax = length-1, auxMin = indiceStartDate, stop = 0;

            while (auxMin<=auxMax && !stop)
            {
                middle = (auxMin+auxMax) / 2;

                if (getDateArrTrips(rides_arr,ids[middle]) == b_date) stop++;
                else if (getDateArrTrips(rides_arr,ids[middle]) < b_date)
                {
                    auxMin = middle+1;
                }
                else auxMax = middle-1;
            }

            indiceLimitDate = middle;

            // Verifica se a posicao realmente corresponde a uma viagem na data desejada e,
            // se corresponder à data desejada, percorre o array até encontrar a ultima posicao com a mesma data.
            // Caso não seja a data pretendida, procura a ultima posicao da data mais proxima da data pretendida, que exista e
            // que seja inferior à data pretendida.

            if (getDateArrTrips(rides_arr,ids[middle])==b_date)
            {
                for (int j = 1;middle+j<length && getDateArrTrips(rides_arr,ids[middle+j])==b_date;indiceLimitDate++,j++);
            }
            else if (getDateArrTrips(rides_arr,ids[middle])>b_date)
            {
                for (int j = 1;middle-j>0 && getDateArrTrips(rides_arr,ids[middle-j])>b_date;indiceLimitDate--,j++);
                indiceLimitDate--;
            }
            else
            {
                for (int j = 1;middle+j<length && getDateArrTrips(rides_arr,ids[middle+j])<b_date;indiceLimitDate++,j++);
            }

            for (int j = indiceStartDate;j<=indiceLimitDate;j++)
            {
                trip_id = city->ids[j];
                distance = getDistTrips (rides_arr,trip_id);
                class = getArrDriversClass (drivers_arr,getDriverT(rides_arr,trip_id));
                price += preco (distance,class);
                number++;
            }
        }

        i++;
    }

    free (keys_arr);

    if(number!=0)
    {
        double media = price / (double) number;
        if  (batchMode) fprintf (output, "%.3lf\n", media);
        else 
        {
            WINDOW *w = output;
            char *straux =malloc (w->_maxx);
            sprintf(straux, "%.3lf",media);
 
            printCenter(w,straux,w->_maxy/2);
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

            free(straux);
        }
    }
    else if (!batchMode)
    {
        wgetch(output);
    }
}



// Querie 6


    // Funcao que quando chamada a querie 6, percorre o array de viagens de uma cidade ordenado da viagem mais antiga para a mais recente e
    // imprime a distancia media percorrida num determinado intervalo de tempo nessa cidade num ficheiro

void printAverageDistanceByDate (Trips rides_arr, CityOrdByDate cityOrdByDate, char *city_name, unsigned short int s_date, unsigned short int b_date, void *output ,bool batchMode)
{
    double media, distance = 0;
    int number = 0, trip_id;

    CityTrips *city = g_hash_table_lookup (cityOrdByDate->cities, city_name);

    if (city!=NULL)
    {
        int length = city->sp;
        int *ids = city->ids;

        // Faz uma especie de binary search para encontrar a posicao com a data de comeco do intervalo

        int indiceStartDate;
        int indiceLimitDate;
        int stop = 0;

        int auxMax = length-1, auxMin = 0, middle = 0;

        if (s_date<=getDateArrTrips(rides_arr,ids[length-1]) && b_date>=getDateArrTrips(rides_arr,ids[0]))
        {
            while (auxMin<=auxMax && !stop)
            {
                middle = (auxMin+auxMax) / 2;

                if (getDateArrTrips(rides_arr,ids[middle]) == s_date) stop++;
                else if (getDateArrTrips(rides_arr,ids[middle]) < s_date)
                {
                    auxMin = middle+1;
                }
                else auxMax = middle-1;
            }

            indiceStartDate = middle;

            // Verifica se a posicao realmente corresponde a uma viagem na data desejada e,
            // se corresponder à data desejada, percorre o array até encontrar a primeira posicao com a mesma data.
            // Caso não seja a data pretendida, procura a primeira posicao da data mais proxima da data pretendida, que exista e
            // que seja superior à data pretendida.

            if (getDateArrTrips(rides_arr,ids[middle])==s_date)
            {
                for (int j = 1;middle-j>0 && getDateArrTrips(rides_arr,ids[middle-j])==s_date;indiceStartDate--,j++);
            }
            else if (getDateArrTrips(rides_arr,ids[middle])>s_date)
            {
                for (int j = 1;middle-j>0 && getDateArrTrips(rides_arr,ids[middle-j])>s_date;indiceStartDate--,j++);
            }
            else
            {
                for (int j = 1;middle+j<length && getDateArrTrips(rides_arr,ids[middle+j])<s_date;indiceStartDate++,j++);
                indiceStartDate++;
            }

            // Faz uma especie de binary search para encontrar a posicao com a data em que termina o intervalo

            auxMax = length-1, auxMin = indiceStartDate, stop = 0;

            while (auxMin<=auxMax && !stop)
            {
                middle = (auxMin+auxMax) / 2;

                if (getDateArrTrips(rides_arr,ids[middle]) == b_date) stop++;
                else if (getDateArrTrips(rides_arr,ids[middle]) < b_date)
                {
                    auxMin = middle+1;
                }
                else auxMax = middle-1;
            }

            indiceLimitDate = middle;

            // Verifica se a posicao realmente corresponde a uma viagem na data desejada e,
            // se corresponder à data desejada, percorre o array até encontrar a ultima posicao com a mesma data.
            // Caso não seja a data pretendida, procura a ultima posicao da data mais proxima da data pretendida, que exista e
            // que seja inferior à data pretendida.

            if (getDateArrTrips(rides_arr,ids[middle])==b_date)
            {
                for (int j = 1;middle+j<length && getDateArrTrips(rides_arr,ids[middle+j])==b_date;indiceLimitDate++,j++);
            }
            else if (getDateArrTrips(rides_arr,ids[middle])>b_date)
            {
                for (int j = 1;middle-j>0 && getDateArrTrips(rides_arr,ids[middle-j])>b_date;indiceLimitDate--,j++);
                indiceLimitDate--;
            }
            else
            {
                for (int j = 1;middle+j<length && getDateArrTrips(rides_arr,ids[middle+j])<b_date;indiceLimitDate++,j++);
            }

            for (int j = indiceStartDate;j<=indiceLimitDate;j++)
            {
                trip_id = city->ids[j];
                distance += getDistTrips (rides_arr,trip_id);
                number++;
            }
        }

        if(number!=0)
        {
            media = distance / (double) number;
            if  (batchMode) fprintf (output, "%.3lf\n", media);
            else 
            {
                WINDOW *w = output;
                char *straux =malloc (w->_maxx);
                sprintf(straux, "%.3lf",media);

                printCenter(w,straux,w->_maxy/2);
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

                free(straux);
            }
        }
        else if (!batchMode)
        {
            wgetch(output);
        }
        
    }
}





// Querie 9


    // Estrutura auxiliar para guardar a informacao duma viagem

typedef struct distanceTips
{
    int distance;                                           // Distancia percorrida
    int id;                                                 // Id da viagem
    unsigned short int date;                                // Data da viagem
} DistanceTips;

    // Estrutura auxiliar para guardar um array de viagens

typedef struct arrayDistanceTips
{
    int sp;                                                 // Numero de elementos no array
    int size;                                               // Espaco alocado para o array
    struct distanceTips *distanceTips;                      // Array de viagens
} ArrayDistanceTips;

    // Estrutura usada para passar dados para a funcao iteratorDistanceTips que vai ordenar o array de viagens por ordem decrescente
    // de distancias percorridas e em caso de empate a aparecer primeiro as viagens mais recentes

typedef struct arg
{
    Trips arrayT;                                           // Array das viagens
    struct arrayDistanceTips *arrayDistanceTips;            // Array auxiliar das viagens
    unsigned short int startDate;                           // Data de inicio do intervalo
    unsigned short int limitDate;                           // Data em que termina o intervalo
} Arg;

    // Inicializa um array de estruturas que guardam informacoes de viagens

static ArrayDistanceTips *inicializeArrayDistanceTips ()
{
    ArrayDistanceTips* arrayDistanceTips = malloc (sizeof(ArrayDistanceTips));
    arrayDistanceTips->sp = 0;
    arrayDistanceTips->size = 4;
    arrayDistanceTips->distanceTips = malloc (4 * sizeof(DistanceTips));

    return arrayDistanceTips;
}

    // Funcao que determina que o array de viagens vai ser ordenado por ordem decrescente de distancias percorridas e
    // em caso de empate a aparecer primeiro as viagens mais recentes

static int comparatorDistancesDates (const void *p, const void *q)
{
    DistanceTips l = *(DistanceTips*) p;
    DistanceTips r = *(DistanceTips*) q;

    if (l.distance>r.distance)
    {
        return -1;
    }
    else if (l.distance<r.distance)
    {
        return 1;
    }
    else
    {
        
        if (l.date>r.date)
        {
            return -1;
        }
        else if (l.date<r.date)
        {
            return 1;
        }
        else
        {
            if (l.id>r.id)
            {
                return -1;
            }
            return 1;
        }
    }
}

    // Itera sobre todas as keys da hashTable das cidades, preenchendo o array auxiliar de viagens com todas as viagens no intervalo
    // dado como argumento

static void iteratorDistanceTips (gpointer key, gpointer value, gpointer hashCityTrips)
{
    Arg *arg = (Arg*) hashCityTrips;
    unsigned short int startDate = ((Arg*) hashCityTrips)->startDate;
    unsigned short int limitDate = ((Arg*) hashCityTrips)->limitDate;
    CityTrips *cityTrips = (CityTrips*) value;
    int sp = cityTrips->sp;
    int *ids = cityTrips->ids;

    // Faz uma especie de binary search para encontrar a posicao com a data de comeco do intervalo

    int indiceStartDate;
    int indiceLimitDate;
    int stop = 0;

    int auxMax = sp-1, auxMin = 0, middle = 0;

    if (startDate<=getDateArrTrips(arg->arrayT,ids[sp-1]) && limitDate>=getDateArrTrips(arg->arrayT,ids[0]))
    {
        while (auxMin<=auxMax && !stop)
        {
            middle = (auxMin+auxMax) / 2;

            if (getDateArrTrips(arg->arrayT,ids[middle]) == startDate) stop++;
            else if (getDateArrTrips(arg->arrayT,ids[middle]) < startDate)
            {
                auxMin = middle+1;
            }
            else auxMax = middle-1;
        }

        indiceStartDate = middle;

        // Verifica se a posicao realmente corresponde a uma viagem na data desejada e,
        // se corresponder à data desejada, percorre o array até encontrar a primeira posicao com a mesma data.
        // Caso não seja a data pretendida, procura a primeira posicao da data mais proxima da data pretendida, que exista e
        // que seja superior à data pretendida.

        if (getDateArrTrips(arg->arrayT,ids[middle])==startDate)
        {
            for (int j = 1;middle-j>0 && getDateArrTrips(arg->arrayT,ids[middle-j])==startDate;indiceStartDate--,j++);
        }
        else if (getDateArrTrips(arg->arrayT,ids[middle])>startDate)
        {
            for (int j = 1;middle-j>0 && getDateArrTrips(arg->arrayT,ids[middle-j])>startDate;indiceStartDate--,j++);
        }
        else
        {
            for (int j = 1;middle+j<sp && getDateArrTrips(arg->arrayT,ids[middle+j])<startDate;indiceStartDate++,j++);
            indiceStartDate++;
        }

        // Faz uma especie de binary search para encontrar a posicao com a data em que termina o intervalo

        auxMax = sp-1, auxMin = indiceStartDate, stop = 0;

        while (auxMin<=auxMax && !stop)
        {
            middle = (auxMin+auxMax) / 2;

            if (getDateArrTrips(arg->arrayT,ids[middle]) == limitDate) stop++;
            else if (getDateArrTrips(arg->arrayT,ids[middle]) < limitDate)
            {
                auxMin = middle+1;
            }
            else auxMax = middle-1;
        }

        indiceLimitDate = middle;

        // Verifica se a posicao realmente corresponde a uma viagem na data desejada e,
        // se corresponder à data desejada, percorre o array até encontrar a ultima posicao com a mesma data.
        // Caso não seja a data pretendida, procura a ultima posicao da data mais proxima da data pretendida, que exista e
        // que seja inferior à data pretendida.

        if (getDateArrTrips(arg->arrayT,ids[middle])==limitDate)
        {
            for (int j = 1;middle+j<sp && getDateArrTrips(arg->arrayT,ids[middle+j])==limitDate;indiceLimitDate++,j++);
        }
        else if (getDateArrTrips(arg->arrayT,ids[middle])>limitDate)
        {
            for (int j = 1;middle-j>0 && getDateArrTrips(arg->arrayT,ids[middle-j])>limitDate;indiceLimitDate--,j++);
            indiceLimitDate--;
        }
        else
        {
            for (int j = 1;middle+j<sp && getDateArrTrips(arg->arrayT,ids[middle+j])<limitDate;indiceLimitDate++,j++);
        }

        for (int i = indiceStartDate;i<=indiceLimitDate;i++)
        {
            if (getArrTripsTipVerify(arg->arrayT,ids[i]))
            {
                if (arg->arrayDistanceTips->sp==arg->arrayDistanceTips->size)
                {
                    arg->arrayDistanceTips->size *= 2;
                    arg->arrayDistanceTips->distanceTips = realloc (arg->arrayDistanceTips->distanceTips, arg->arrayDistanceTips->size * sizeof(DistanceTips));
                }

                arg->arrayDistanceTips->distanceTips[arg->arrayDistanceTips->sp].id = ids[i];
                arg->arrayDistanceTips->distanceTips[arg->arrayDistanceTips->sp].date = getDateArrTrips(arg->arrayT,ids[i]);
                arg->arrayDistanceTips->distanceTips[arg->arrayDistanceTips->sp].distance = getDistTrips (arg->arrayT,ids[i]);
                (arg->arrayDistanceTips->sp)++;
            }
        }
    }
}

    // Funcao que quando chamada a querie 9, cria um array com todas as viagens realizadas no intervalo dado como argumento, ordena esse
    // array por ordem decrescente de distancias percorridas e em caso de empate a aparecer primeiro as viagens mais recentes e
    // imprime o array inteiro da primeira posicao até à ultima num ficheiro

void printfTipsOrsByDistance (CityOrdByDate cityOrdByDate, Trips arrayT, unsigned short int startDate, unsigned short int limitDate, void *output ,bool batchMode)
{
    
    ArrayDistanceTips* arrayDistanceTips = inicializeArrayDistanceTips ();

    Arg *arg = malloc (sizeof(Arg));
    arg->startDate = startDate;
    arg->limitDate = limitDate;
    arg->arrayT = arrayT;
    arg->arrayDistanceTips = arrayDistanceTips;

    g_hash_table_foreach (cityOrdByDate->cities, iteratorDistanceTips, arg);

    int size = size =arg->arrayDistanceTips->sp;

    qsort ((void*)arg->arrayDistanceTips->distanceTips, size, sizeof(arg->arrayDistanceTips->distanceTips[0]), comparatorDistancesDates);

    if (batchMode)
    {
        for (int i = 0;i<arg->arrayDistanceTips->sp;i++)
        {
            int idRide = arg->arrayDistanceTips->distanceTips[i].id;
            char *date = decodeData (arg->arrayDistanceTips->distanceTips[i].date);
            char *city = getCity (arrayT,idRide);
            double tip = getArrTripsTip (arrayT,idRide);
            int distance = arg->arrayDistanceTips->distanceTips[i].distance;

            fprintf (output, "%012d;%s;%d;%s;%.3f\n", idRide, date, distance, city, tip);

            free (date);
            free (city);
        }
    }
    else
    {
        WINDOW *w =output;
        clearBox(w);
        int yStart,maximoLinhas,divisoes;
        
        maximoLinhas = w->_maxy-1; // deixar 2 espaços para a caixa da janlea (y =0 e y =w ->maxy)
        
        divisoes = size/maximoLinhas;
        
        if (size % maximoLinhas != 0) divisoes++;

        
        char *strOutput = malloc (w->_maxx);

        /*
            se houver mais que 1 janela cheia, vamos ter de separar o output em janelas
            alternando entre estas usando as setas -> <- e 'a' e 'd' o espaço ou o enter terminam o programa

        */ 
        bool loop = true;
        int pagI =0;
        while (loop)
        {
            int i = pagI * maximoLinhas,fim;
            if (pagI == divisoes-1) 
            {
                yStart  = (w->_maxy - size +i+1) /2;
                fim = size;
            }
            else 
            {
                yStart = 1;
                fim =i+maximoLinhas;
            }


            mvwprintw(w,1,1,"%d -> %d",i+1,fim);
            while (i<size && yStart<=maximoLinhas)
            {

                int idRide = arg->arrayDistanceTips->distanceTips[i].id;
                char *date = decodeData (arg->arrayDistanceTips->distanceTips[i].date);
                char *city = getCity (arrayT,idRide);
                double tip = getArrTripsTip (arrayT,idRide);
                int distance = arg->arrayDistanceTips->distanceTips[i].distance;

                sprintf (strOutput, "%012d;%s;%d;%s;%.3f", idRide, date, distance, city, tip);

                free (date);
                free (city);

                // Ecrever no ficheiro se estiver no modo batch, se tiver no modo iterativo escrever na terminal
                printCenter(w,strOutput,yStart);
  
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

    free (arg->arrayDistanceTips->distanceTips);
    free (arg->arrayDistanceTips);
    free (arg);
}
