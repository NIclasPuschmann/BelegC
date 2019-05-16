#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#define MAX 50

//Funktion zum Bildschirm bereinigen
void screenwipe()
{
    for (int i = 0; i < 200; i++)
    {
        printf("\n\n");
    }
}

//7-stellig kundennummer
int generateCustnr, generateBnr;

//hilfsfels fuer Haeuserbelegung
int hausbelegung[10];

struct CUSTOMER
{
    char vname[MAX];
    char name[MAX];
    unsigned long int customerNr;
    
    
};
typedef struct CUSTOMER customer;

struct DATE
{
    int day;
    int month;
    int year;
};
typedef struct DATE date;


struct BOOKING
{
    unsigned long bookingNr;
    customer Kunde;
    date anreise;
    date abreise;
    struct BOOKING *next;
};
typedef struct BOOKING Buchung;

struct HOUSE
{
    int nr;
    char cat;
    Buchung *anfang;
};
typedef struct HOUSE house;

// Feld Ferienhaeuser
house allFhaeuser [3][10];


// wohin damit?
struct BOOKING *next   = NULL;
struct BOOKING *anfang = NULL;


// Schaltjahrueberpruefung
int SchaltJCheck(int year)
{
    int y = 0;
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        y = 1;
    else  y = 0;
    return y;
}



// Funktion zum Zaehlen von Tagen
int countDay (int tm_day, int tm_month, int tm_year)
{   int days = 0;
    int daysinMonth = 0;
    int countMonth = 1;
    int totalNum_days;
    
    while (countMonth <= tm_month)
    {
        if (countMonth == 2)
        {
            if (SchaltJCheck(tm_year) == 1)
            {
                days = (days + 29);
                countMonth = countMonth + 1;
                daysinMonth = 29;
            } else
            {
                days = (days + 28);
                countMonth = countMonth + 1;
                daysinMonth = 28;
            }
        } else
        {
            if (countMonth == 4 || countMonth == 6 || countMonth == 9 || countMonth == 11)
            {
                days = days + 30;
                countMonth = countMonth + 1;
                daysinMonth = 30;
            } else
            {
                days = days + 31;
                countMonth = countMonth + 1;
                daysinMonth = 31;
            }
        }
    }
    totalNum_days = days - daysinMonth + tm_day;
    return totalNum_days;
}


//Funktion zum Zaehlen der belegten Tage (Aufenthalt eines Gasts)
int ZaehlerAufenthalt(int dateAnf, int dateEnd)
{
    int period_of_stay;
    period_of_stay = (dateEnd - dateAnf);
    return period_of_stay;
}

// Gueltigkeitsueberpruefung Datum
int dateCheck(int d,int m,int y)
{
    if (d < 1 || d > 31 || m < 1 || m > 12 || y < 1583)
        return 0;
    
    if ((m == 4 || m == 6 || m == 9 || m == 11) && (d < 1 || d > 30))
        return 0;
    
    if (m == 2)
    {
        if (SchaltJCheck(y) && d > 29)
            return 0;
        if (!SchaltJCheck(y) && d > 28)
            return 0;
    }
    return 1;
}


// Vergleich Datum
int CompareDate (int d, int m,int y,int d2,int m2,int y2)
{
    if (d==d2 && m==m2 && y==y2)
    {
    return 1;
    }
    if (y2 > y)
    {
    return 2;
    }
    else
    {
        if (y2 == y && m2 > m)
        {
        return 2;
        }
    }
    if (y2 == y && m2 == m && d2 > d)
    {
    return 2;
    }
    if (y2 < y)
    {
    return 3;
    }
    else
    {
        if (y2 == y && m2 < m)
        {
        return 3;
        }
    }
    if (y2 == y && m2 == m && d2 < d)
    {
    return 3;
    }
    return 0;
}

void Buchung_anhaengen(unsigned long bookNr, unsigned long custNr, char *n, char *v, int anreiset, int anreisem, int anreisej,int abreiset, int abreisem, int abreisej, int l, int k)
{
    date anreise;
    anreise.day = anreiset;
    anreise.month = anreisem;
    anreise.year = anreisej;
    date abreise;
    abreise.day = abreiset;
    abreise.month = abreisem;
    abreise.year = abreisej;
    customer Kunde;
    Kunde.customerNr = custNr;
    Buchung *zeiger;
    
    if(allFhaeuser[k][l].anfang == NULL)
    {
        
        if((allFhaeuser[k][l].anfang = malloc(sizeof(struct BOOKING))) == NULL)
        {
            fprintf(stderr, "Kein Speicherplatz vorhanden\n");
            return;
        }
        strcpy(allFhaeuser[k][l].anfang->Kunde.name, n);
        strcpy(allFhaeuser[k][l].anfang->Kunde.vname, v);
        allFhaeuser[k][l].anfang->bookingNr=bookNr;
        allFhaeuser[k][l].anfang->Kunde.customerNr=custNr;
        allFhaeuser[k][l].anfang->anreise=anreise;
        allFhaeuser[k][l].anfang->abreise=abreise;
        allFhaeuser[k][l].anfang->next=NULL;
        printf("\nanhaengen success\n");
        
    } else
    {
        zeiger=allFhaeuser[k][l].anfang;
        while(zeiger->next != NULL)
            zeiger=zeiger->next;

        if((zeiger->next = malloc(sizeof(struct BOOKING))) == NULL)
            {
                fprintf(stderr,"Kein Speicherplatz für das letzte Element\n");
                return;
            }
        zeiger=zeiger->next; /* zeiger auf neuen Speicherplatz */
        strcpy(zeiger->Kunde.name,n);
        strcpy(zeiger->Kunde.vname,v);
        zeiger->bookingNr=bookNr;
        zeiger->Kunde.customerNr=custNr;
        zeiger->anreise=anreise;
        zeiger->abreise=abreise;
        zeiger->next=NULL;
    }
}


// Funktion zur Stornierung
void storno (int Buchung, int Custnr)
{
    struct BOOKING *zeiger, *zeiger1;
    zeiger=NULL;
    zeiger1=NULL;
    int i,j;
    bool abbruch=true;
    for(i=0; i<3; i++)
    {
        for (j=0; j<10; j++)
        {
            zeiger=allFhaeuser[i][j].anfang;
            if(allFhaeuser[i][j].anfang!=NULL)
            {
                if(zeiger->bookingNr==Buchung && zeiger->Kunde.customerNr==Custnr)
                    {
                        allFhaeuser[i][j].anfang=allFhaeuser[i][j].anfang->next;
                        abbruch=false;
                    } else
                {
                            while(zeiger != NULL && abbruch)
                    {
                                if(zeiger->bookingNr==Buchung && zeiger->Kunde.customerNr==Custnr)
                                {
                                    abbruch=false;
                                    zeiger1->next=zeiger->next;
                                } else
                        {
                            zeiger1=zeiger;
                            zeiger=zeiger->next;
                        }
                    }
                }
            }
        }
    }
    if (abbruch==true)
    {
        printf("\nWir haben keine Buchung unter der angegebenen Buchungsnummer finden koennen.\n");
    }
}



/* Funktion zum Ausgeben der Eingaben_Buchung/Anfrage */
void ausgabe_buchung(void) {
    struct BOOKING *zeiger = anfang;
    
    printf("||===================================================================||\n");
    printf("|%10cName%10c|Anreise|Abreise|Buchungsnummer|Kundennummer|\n",' ',' ');
    printf("||===================================================================||\n");
    
    while(zeiger != NULL) {
        printf("|%12s,%-12s| %02d.%02d.%04d|%02d.%02d.%04d|%ld|%ld\n",
               zeiger->Kunde.name,
               zeiger->Kunde.vname,
               zeiger->anreise.day,
               zeiger->anreise.month,
               zeiger->anreise.year,
               zeiger->abreise.day,
               zeiger->abreise.month,
               zeiger->abreise.year,
               zeiger->bookingNr,
               zeiger->Kunde.customerNr);
        printf("|---------------------------------------------------------|\n");
        zeiger=zeiger->next;
    }
}

/* Funktion zur Eingabe der Daten */
void eingabe_buchung(void) {
    char nam[MAX],vorn[MAX];
    int anreisetag, anreisemon, anreisejahr, abreiset, abreisem, abreisej;
    unsigned long bookNR, custNR;
    char *ptr;
    
    printf("Geben Sie ihren Namen ein.\n");
    printf("Name........................: ");
    fgets(nam, MAX, stdin);
    ptr = strrchr(nam, '\n');
    *ptr = '\0';
    printf("Geben Sie ihren Vornamen ein.\n");
    printf("Vorname.....................: ");
    fgets(vorn, MAX, stdin);
    ptr = strrchr(vorn, '\n');
    *ptr = '\0';
    
    printf("Geben Sie ihre Buchungsnummer ein.\n");
    printf("Buchungsnummer................: ");
    scanf("%ld",&bookNR);
    printf("Geben Sie ihre Kundennummer ein.\n");
    printf("Kundennummer................: ");
    scanf("%ld",&custNR);
    printf("Anreise...........(tt.mm.jjjj): ");
    scanf("%2d.%2d.%4d",&anreisetag,&anreisemon,&anreisejahr);
    printf("Abreise am..(tt.mm.jjjj): ");
    scanf("%2d.%2d.%4d",&abreiset,&abreisem,&abreisej);
    
//   getchar();
//    Buchung_anhaengen(bookNR, custNR, nam, vorn, anreisetag, anreisemon, anreisejahr, abreiset, abreisem, abreisej, anfang);
}

/*
 int main(void) {
    int wahl;
    int Buchung;
    
    do {
        printf("\n1 : Eingabe\n");
        printf("2 : Ausgabe\n");
        printf("3 : Buchung loeschen\n");
        printf("9 : Ende\n");
        printf("Ihre Wahl : ");
        scanf("%d",&wahl);
        getchar();
        switch(wahl) {
            case 1 : eingabe_buchung();
                break;
            case 2 : ausgabe_buchung();
                break;
            case 3 : printf("Nummer eingeben");
                scanf("%d", &Buchung);
                storno (Buchung);
                break;
            case 9 : break;
            default: printf("Falsche Eingabe!!!\n");
        }
    } while(wahl != 9);
    return EXIT_SUCCESS;
}
*/


date increaseDate(date toincrease)
{
    toincrease.day++;
    
    if (dateCheck(toincrease.day,toincrease.month, toincrease.year) == 1) {
        return toincrease;
    } else {
        toincrease.day=1;
        toincrease.month++;
        if(dateCheck(toincrease.day,toincrease.month, toincrease.year) == 1) {
            return toincrease;
        } else {
            toincrease.month=1;
            toincrease.year++;
            if (dateCheck(toincrease.day,toincrease.month, toincrease.year) == 1) {
                return toincrease;
            }
            
        }
        
    }
    date failure;
    failure.day=0;
    failure.month=0;
    failure.year=0;
    
    return failure;
}

//Funktion Hausbelegung
void Hausbelegung (int j, date anreise2, date abreise2)

{
    for(int k=0; k<10; k++)
    {
    
        hausbelegung[k]=1;
        Buchung *zeiger;
        zeiger=allFhaeuser[j][k].anfang;
        while(zeiger!=NULL)
        {   date anreise1 = zeiger->anreise;
            date abreise1 = zeiger->abreise;
            date iterator = anreise1;
            bool abbruch = false;
         while (CompareDate (iterator.day, iterator.month, iterator.year,abreise1.day, abreise1.month, abreise1.year) !=1 && abbruch==false)
            {   if ((CompareDate(iterator.day, iterator.month, iterator.year,anreise2.day, anreise2.month, anreise2.year)==2 || (CompareDate(iterator.day, iterator.month,   iterator.year,anreise2.day, anreise2.month, anreise2.year)==1 && CompareDate(iterator.day, iterator.month, iterator.year,abreise2.day, abreise2.month, abreise2.year)==2) || CompareDate(iterator.day, iterator.month, iterator.year,abreise2.day, abreise2.month, abreise2.year)==1))
                 {  abbruch = true;
                    hausbelegung[k]=0;
                 }
                 iterator=increaseDate(iterator);
            }
            zeiger = zeiger->next;
        }
    }
}



//Zufallszahlengenerator Entwurf fuer Kundennummer/ Buchungsnummer
unsigned long  NumGenerator ()
{
    long int variable;
    
//    srand(time(NULL));
    
    for(int i = 0; i < 10; i++)
        {
        variable = rand() %1000;
        }
    return 0;
}


// Compare Customer ueber Kundennummer

// EingabeZeitraumBuchung
void EingabeZeitraumBuchung()

{
 
    int dateAnf, dateEnd, period_of_stay;
    int d = 0,m = 0, y = 0;
    int d2 = 0, m2 = 0, y2 = 0;

    // Startdatum Buchung
    
    printf("\nGeben Sie nun ihr gewuenschtes Anreisedatum (Format: dd-mm-yyyy) ein.\n");
    scanf("%d.%d.%d", &d, &m, &y);
    
    dateAnf = dateCheck(d, m, y);
    if (dateAnf == 0)
    {
        printf("\nIhr angegebenes Datum ist leider ungueltig. Bitte versuchen Sie es erneut (Format: DD.MM.YY).\n");
        
    }   else { printf("\n Ihr angegebenes Datum ist gueltig.\n"); }
    
    
    
    // Enddatum Buchung
    printf("\nGeben Sie nun ihr gewuenschtes Abreisedatum  (Format: dd-mm-yyyy) ein.\n");
    scanf("%d.%d.%d", &d2, &m2, &y2);
    
    dateEnd = dateCheck(d2, m2, y2);
    if (dateEnd == 0)
    {
        printf("\nIhr angegebenes Datum ist leider ungueltig. Bitte versuchen Sie es erneut (Format: DD.MM.YY).\n");
        
    }
    else
    {
        printf("\nIhr Datum ist gueltig.\n");
    }
    
    period_of_stay = ZaehlerAufenthalt (countDay (d ,m ,y), countDay(d2, m2, y2));
    if (period_of_stay < 3)
    {
        printf("Sie muessen bei uns ein Ferienhaus mindestens 3 Tage mieten.\n");
    }
    else
        printf("Sie haben angefragt, ob das Ferienhaus vom %d.%d.%d. bis zum %d.%d.%d. fuer %d Tage frei ist.\n", d, m, y, d2, m2, y2, period_of_stay);
    
}
    


// Funktion eingabe_ausw() zur robusten Eingabe eines Zeichens von der Konsole
char eingabe_ausw()
{
    char zeile[20];
    fflush(stdin);
    fgets(zeile, 20, stdin);
    fflush(stdin);
    printf("%s\n", zeile);
    while (zeile[0] == ' ')
        strcpy(zeile, &zeile[1]);
    zeile[0] = tolower(zeile[0]);
    return zeile[0];
}
/*
struct CUSTOMER
{
    char vname[MAX];
    char name[MAX];
    unsigned long int customerNr;
    
    
};
typedef struct CUSTOMER customer;

struct DATE
{
    int day;
    int month;
    int year;
};
typedef struct DATE date;


struct BOOKING
{
    unsigned long bookingNr;
    customer Kunde;
    date anreise;
    date abreise;
    struct BOOKING *next;
};
typedef struct BOOKING Buchung;

struct HOUSE
{
    int nr;
    char cat;
    Buchung *anfang;
};
typedef struct HOUSE house;
 
 */

//initiliserung ferienhaus

void initilisierung ()
{
    Buchung *init=NULL;
    house a;
    house b;
    house c;
    a.anfang=init;
    b.anfang=init;
    c.anfang=init;
    a.nr = 1;
    b.nr = 11;
    c.nr = 21;
    a.cat = 'a';
    b.cat = 'b';
    c.cat = 'c';
    
    while(a.nr<=10)
    {
        allFhaeuser[0][a.nr-1]=a;
        a.nr++;
    }
    while(b.nr<=20)
    {
        allFhaeuser[1][b.nr-11]=b;
        b.nr++;
    }
    while(c.nr<=30)
    {
        allFhaeuser[2][c.nr-21]=c;
        c.nr++;
    }
    
    
}


//funktion anfrage 2
void anfrage2()
{
    int dateAnf, dateEnd, period_of_stay;
    char Ausw;
    int AuswHnr;
    int j = 0;
    int anreisetag, anreisemon, anreisejahr, abreiset, abreisem, abreisej;
    date anreise2, abreise2;
   bool dateChek;
    
    
    
    printf("\nFuer welches Startdatum wollen Sie freie Ferienhaeuser ueberpruefen?\n");
    printf("Geben Sie ihr gewuenschtes Anreisedatum ein.\n");
    printf("Anreise...........(tt.mm.jjjj): ");
    scanf("%2d.%2d.%4d",&anreisetag,&anreisemon,&anreisejahr);
    fflush(stdin);
    
    do {
        dateChek=true;
        dateAnf = dateCheck(anreisetag, anreisemon, anreisejahr);
        if (dateAnf == 0)
        {
            printf("\nIhr angegebenes Datum ist leider ungueltig. Bitte versuchen Sie es erneut (Format: dd-mm-yyyy).\n");
            
        }   else { printf("\nIhr angegebenes Datum ist gueltig.\n");
            dateChek=false;
            anreise2.day = anreisetag;
            anreise2.month = anreisemon;
            anreise2.year = anreisejahr;
        }
    } while (dateChek);
    
    printf("\nFuer welches Enddatum wollen Sie freie Ferienhaeuser ueberpruefen?\n");
    printf("\nGeben Sie nun ihr gewuenschtes Abreisedatum ein.\n");
    printf("Abreise...........(tt.mm.jjjj): ");
    scanf("%2d.%2d.%4d",&abreiset,&abreisem,&abreisej);
    fflush(stdin);
    
    do {
        dateChek=true;
        dateEnd = dateCheck(abreiset, abreisem, abreisej);
        if (dateEnd == 0)
        {
            printf("\nIhr angegebenes Datum ist leider ungueltig. Bitte versuchen Sie es erneut (Format: dd-mm-yyyy).\n");
            
        }
        else
        {
            printf("\nIhr Datum ist gueltig.\n");
            dateChek=false;
            abreise2.day = abreiset;
            abreise2.month = abreisem;
            abreise2.year = abreisej;
        }
    } while (dateChek);
    
    period_of_stay = ZaehlerAufenthalt (countDay (anreisetag, anreisemon, anreisejahr), countDay(abreiset, abreisem, abreisej));
    if (period_of_stay < 3)
    {
        printf("\nIhr angegebenes Datum ist leider ungueltig. Bitte versuchen Sie es erneut (Format: dd-mm-yyyy).\n");
        printf("Sie muessen bei uns ein Ferienhaus mindestens 3 Tage mieten, um es buchen zu koennen.\n");
    }
    else
        printf("Sie haben angefragt, ob das Ferienhaus vom %d.%d.%d. bis zum %d.%d.%d. fuer %d Tage frei ist.\n",  anreisetag, anreisemon, anreisejahr, abreiset, abreisem, abreisej, period_of_stay);
    
    
    
    
    printf("\n\tWelche Ferienhauskategorie ([A] [B] [C]) wollen Sie ueberpruefen?\n");
    printf("\n\t\tKategorie A: Ferienhaus in den Bergen\n");
    printf("\n\t\tKategorie B: Ferienhaus am Meer\n");
    printf("\n\t\tKategorie C: Ferienhaus auf dem Land\n");
    printf("\n\tDruecken Sie einen der Buchstaben, um sich die freien Haeuser der entsprechenden Kategorie anzeigen zu lassen.\n");
    
    // scanf eingabe char
    scanf("%1s", &Ausw);
    fflush(stdin);
    Ausw=tolower(Ausw);
    
   printf("\n\tWelches Haus wollen Sie ueberpruefen?\n");
   printf("\n\tKategorie A...Haeuser 1-10\n");
   printf("\n\tKategorie B...Haeuser 11-20\n");
   printf("\n\tKategorie C...Haeuser 21-30\n");
    
    // scanf eingabe hausnummer
    scanf("%d", &AuswHnr);
    fflush(stdin);
    
    
// berechnung hausnummer
    int i,l;
    for(i=0; i<3; i++)
    {
        for (j=0; j<10; j++)
        {
            if (allFhaeuser[i][j].nr==AuswHnr)
            {
                l=j;
            }
        }
    }
    
    
    
    switch (Ausw)
    {
        case 'a':
            j=0;
            printf("Sie haben Kategorie A ausgewaehlt\n");
            if(AuswHnr>10)
            {
            printf("Hausnummer passt nicht zur Kategorie\n");
            break;
            }
            printf("Auswahl war erfolgreich\n");
            Hausbelegung (j,anreise2, abreise2);
            if(hausbelegung[l]==1)
            {
                printf("Ihr ausgewaehltes Haus ist frei.\n");
            } else
            {    printf("Ihr ausgewaehltes Haus ist nicht frei.\n");
            }
            break;
        case 'b':
            j=1;
            printf("Sie haben Kategorie B ausgewaehlt\n");
            if(AuswHnr>20||AuswHnr<11)
            {
                printf("Hausnummer passt nicht zur Kategorie\n");
                break;
            }
            printf("Auswahl war erfolgreich\n");
            Hausbelegung (j,anreise2, abreise2);
            if(hausbelegung[l]==1)
            {
                printf("Ihr ausgewaehltes Haus ist frei.\n");
            } else
            {   printf("Ihr ausgewaehltes Haus ist nicht frei.\n");
            }
            break;
        case 'c':
            j=2;
            printf("Sie haben Kategorie C ausgewaehlt\n");
            if(AuswHnr<21)
            {
                printf("Hausnummer passt nicht zur Kategorie\n");
                break;
            }
            printf("Auswahl war erfolgreich\n");
            Hausbelegung (j,anreise2, abreise2);
            if(hausbelegung[l]==1)
            {
                printf("Ihr ausgewaehltes Haus ist frei.\n");
            } else
            {   printf("Ihr ausgewaehltes Haus ist nicht frei.\n");
            }
            break;
        default:
            printf("Ungueltige Eingabe\n");
    }

    
    
    
    
    
    
}



// Funktion Anfrage Ferienhaeuser
void anfrage()
{
    int dateAnf, dateEnd, period_of_stay;
    char Ausw;
    int i,j = 0;
    int anreisetag, anreisemon, anreisejahr, abreiset, abreisem, abreisej;
    date anreise2, abreise2;
    bool dateChek;
    int c=0; //zaehlvairable
    
 /*   char nam[MAX],vorn[MAX];
    int anreisetag, anreisemon, anreisejahr, abreiset, abreisem, abreisej;
    unsigned long bookNR, custNR;
    char *ptr;
  */
    
   
    printf("Waehlen Sie bitte einer der folgenden Optionen aus\n");
    printf("\nOption A: Anfrage, welche Haeuser in einem Zeitraum frei sind....");
    printf("\nOption B: Anfrage, ob ein bestimmtes Haus in einem Zeitraum frei ist....\n");
    
    // scanf eingabe char
    scanf("%1s", &Ausw);
    fflush(stdin);
    Ausw=tolower(Ausw);
    
    switch (Ausw)
    {
        case 'a':
            printf("Sie haben Option A ausgewaehlt\n");
            break;
        case 'b':
            printf("Sie haben Option B ausgewaehlt\n");
            //funktion anfrage 2
            anfrage2();
            return;
            break;
        default:
            printf("Ungueltige Eingabe\n");
    }
    

    
    printf("\nFuer welches Startdatum wollen Sie freie Ferienhaeuser ueberpruefen?\n");
    printf("Geben Sie ihr gewuenschtes Anreisedatum ein.\n");
    printf("Anreise...........(tt.mm.jjjj): ");
    scanf("%2d.%2d.%4d",&anreisetag,&anreisemon,&anreisejahr);
    fflush(stdin);
    
    do {
        dateChek=true;
        dateAnf = dateCheck(anreisetag, anreisemon, anreisejahr);
        if (dateAnf == 0)
        {
            printf("\nIhr angegebenes Datum ist leider ungueltig. Bitte versuchen Sie es erneut (Format: dd-mm-yyyy).\n");
            
        }   else { printf("\nIhr angegebenes Datum ist gueltig.\n");
            dateChek=false;
            anreise2.day = anreisetag;
            anreise2.month = anreisemon;
            anreise2.year = anreisejahr;
        }
    } while (dateChek);
    
   printf("\nFuer welches Enddatum wollen Sie freie Ferienhaeuser ueberpruefen?\n");
    printf("\nGeben Sie nun ihr gewuenschtes Abreisedatum ein.\n");
    printf("Abreise...........(tt.mm.jjjj): ");
    scanf("%2d.%2d.%4d",&abreiset,&abreisem,&abreisej);
    fflush(stdin);
    
    do {
        dateChek=true;
        dateEnd = dateCheck(abreiset, abreisem, abreisej);
        if (dateEnd == 0)
        {
            printf("\nIhr angegebenes Datum ist leider ungueltig. Bitte versuchen Sie es erneut (Format: dd-mm-yyyy).\n");
            
        }
        else
        {
            printf("\nIhr Datum ist gueltig.\n");
            dateChek=false;
            abreise2.day = abreiset;
            abreise2.month = abreisem;
            abreise2.year = abreisej;
        }
    } while (dateChek);
    
    period_of_stay = ZaehlerAufenthalt (countDay (anreisetag, anreisemon, anreisejahr), countDay(abreiset, abreisem, abreisej));
    if (period_of_stay < 3)
    {
        printf("\nIhr angegebenes Datum ist leider ungueltig. Bitte versuchen Sie es erneut (Format: dd-mm-yyyy).\n");
        printf("Sie muessen bei uns ein Ferienhaus mindestens 3 Tage mieten, um es buchen zu koennen.\n");
    }
    else
        printf("Sie haben angefragt, ob das Ferienhaus vom %d.%d.%d. bis zum %d.%d.%d. fuer %d Tage frei ist.\n",  anreisetag, anreisemon, anreisejahr, abreiset, abreisem, abreisej, period_of_stay);
    
  
    // =====================================================================================================================================================================
    
    printf("\n\tWelche Ferienhauskategorie ([A] [B] [C]) wollen Sie ueberpruefen?\n");
    printf("\n\t\tKategorie A: Ferienhaus in den Bergen\n");
    printf("\n\t\tKategorie B: Ferienhaus am Meer\n");
    printf("\n\t\tKategorie C: Ferienhaus auf dem Land\n");
    printf("\n\tDruecken Sie einen der Buchstaben, um sich die freien Haeuser der entsprechenden Kategorie anzeigen zu lassen.\n");
    
    
        
// scanf eingabe char
    scanf("%1s", &Ausw);
    fflush(stdin);
    Ausw=tolower(Ausw);
    
    
    switch (Ausw)
    {
        case 'a':
            printf("Sie haben Kategorie A ausgewaehlt\n");
            j=0;
            Hausbelegung (j,anreise2, abreise2);
            for (i = 0; i < 10; i++)
            { if (hausbelegung[i]==1)
                printf("Ferienhaus [Nr.A%d]: A%d\n", i+1, allFhaeuser [0][i].nr);
                c++;
            } printf("%d Haeuser der Kategorie sind frei\n", c);
            break;
        case 'b':
            printf("Sie haben Kategorie B ausgewaehlt\n");
            j=1;
            Hausbelegung (j,anreise2, abreise2);
            for (i = 0; i < 10; i++)
            { if (hausbelegung[i]==1)
                printf("Ferienhaus [Nr.B%d]: B%d\n", i + 1, allFhaeuser [1][i].nr);
                c++;
            } printf("%d Haeuser der Kategorie sind frei\n", c);
            break;
        case 'c':
            printf("Sie haben Kategorie C ausgewaehlt\n");
            j=2;
            Hausbelegung (j,anreise2, abreise2);
            for (i = 0; i < 10; i++)
            {
                if (hausbelegung[i]==1)
                    printf("Ferienhaus [Nr.C%d]: C%d\n", i + 1, allFhaeuser [2][i].nr);
                    c++;
            } printf("%d Haeuser der Kategorie sind frei\n", c);
            break;
        default:
            printf("Ungueltige Eingabe\n");
    }

}

 // ========================================================================================================================================================================


// Funktion Buchung Ferienhaeuser
void buchung()
{
    printf("Ausfuehrung Buchung\n");

    char nam[MAX],vorn[MAX];
    int anreisetag, anreisemon, anreisejahr, abreiset, abreisem, abreisej;
   // unsigned long bookNR, custNR;
    char *ptr;
    int dateAnf, dateEnd, period_of_stay;
    int FHaus;
    char AuswKat_Ferienh;
    int i,j;
    date anreise2, abreise2;
    bool dateChek;
    
    do{
    printf("Geben Sie ihr gewuenschtes Anreisedatum ein.\n");
    printf("Anreise...........(tt.mm.jjjj): ");
    scanf("%2d.%2d.%4d",&anreisetag,&anreisemon,&anreisejahr);
  
 
    dateChek=true;
    dateAnf = dateCheck(anreisetag, anreisemon, anreisejahr);
    if (dateAnf == 0)
    {
        printf("\nIhr angegebenes Datum ist leider ungueltig. Bitte versuchen Sie es erneut (Format: DD.MM.YY).\n");
        
    }   else { printf("\nIhr angegebenes Datum ist gueltig.\n");
        dateChek=false;
        anreise2.day = anreisetag;
        anreise2.month = anreisemon;
        anreise2.year = anreisejahr;
            }
    } while (dateChek);
    
    do {
    printf("\nGeben Sie nun ihr gewuenschtes Abreisedatum ein.\n");
    printf("Abreise...........(tt.mm.jjjj): ");
    scanf("%2d.%2d.%4d",&abreiset,&abreisem,&abreisej);
    

    dateChek=true;
    dateEnd = dateCheck(abreiset, abreisem, abreisej);
    if (dateEnd == 0)
    {
        printf("\nIhr angegebenes Datum ist leider ungueltig. Bitte versuchen Sie es erneut (Format: dd-mm-yyyy).\n");
        
    }
    else
    {
        printf("\nIhr Datum ist gueltig.\n");
        dateChek=false;
        abreise2.day = abreiset;
        abreise2.month = abreisem;
        abreise2.year = abreisej;
        }
    } while (dateChek);
    
    period_of_stay = ZaehlerAufenthalt (countDay (anreisetag ,anreisemon ,anreisejahr), countDay(abreiset, abreisem, abreisej));
    if (period_of_stay < 3)
    {
        printf("\nIhr angegebenes Datum ist leider ungueltig. Bitte versuchen Sie es erneut (Format: dd-mm-yyyy).\n");
        printf("Sie muessen bei uns ein Ferienhaus mindestens 3 Tage mieten.\n");
    }
    else
        printf("Sie haben angefragt, ob das Ferienhaus vom %d.%d.%d. bis zum %d.%d.%d. fuer %d Tage frei ist.\n", anreisetag, anreisemon, anreisejahr, abreiset, abreisem, abreisej, period_of_stay);
    
    
    printf("\nWelches Ferienhaus wollen Sie buchen?\n");
    printf("\nWaehlen Sie dazu zunaechst eine Kategorie ([A] [B] [C]) aus\n");
    
 
    // scanf eingabe char
    scanf("%1s", &AuswKat_Ferienh);
    fflush(stdin);
    AuswKat_Ferienh=tolower(AuswKat_Ferienh);
    
    
    switch (AuswKat_Ferienh)
    {
        case 'a':
            printf("Sie haben Kategorie A ausgewaehlt\n");
            j=0;
            Hausbelegung (j,anreise2, abreise2);
            for (i = 0; i < 10; i++)
            { if (hausbelegung[i]==1)
                printf("\nFerienhaus [Nr.A%d]: A%d\n", i+1, allFhaeuser [0][i].nr);
            }
                printf("\nBitte waehlen sie eines der oben genannten Ferienhaeuser aus: ");
                scanf("%d", &FHaus);
            if (0 < FHaus && FHaus <= 10)
                printf("\nSie haben folgendes Ferienhaus ausgwaehlt..\nA %d\n", allFhaeuser [0][FHaus-1].nr);
            else
            {
                printf("Auswahl ungueltig, Sie gelangen zurueck ins Hauptmenue... \n");
                break;
            }
            break;
        case 'b':
            printf("Sie haben Kategorie B ausgewaehlt\n");
            j=1;
            Hausbelegung (j,anreise2, abreise2);
            for (i = 0; i < 10; i++)
            { if (hausbelegung[i]==1)
             printf("\nFerienhaus [Nr.B%d]: B%d\n", i + 1, allFhaeuser [1][i].nr);
            }
                
            printf("\nBitte waehlen sie eines der oben genannten Ferienhaeuser aus: ");
            scanf("%d", &FHaus);
            if (10 < FHaus && FHaus <= 20)
                printf("\nSie haben folgendes Ferienhaus ausgwaehlt..\nB %d\n", allFhaeuser [1][FHaus-11].nr);
            else
            {
                printf("Auswahl ungueltig, Sie gelangen zurueck ins Hauptmenue... \n");
                break;
            }
            break;
        case 'c':
            printf("Sie haben Kategorie C ausgewaehlt\n");
            j=2;
            Hausbelegung (j,anreise2, abreise2);
            for (i = 0; i < 10; i++)
            {
                if (hausbelegung[i]==1)
                printf("\nFerienhaus [Nr.C%d]: C%d\n", i + 1, allFhaeuser [2][i].nr);
            }
            printf("\nBitte waehlen sie eines der oben genannten Ferienhaeuser aus: ");
            scanf("%d", &FHaus);
            if (20 < FHaus && FHaus <= 30)
                printf("\nSie haben folgendes Ferienhaus ausgwaehlt..\nC %d\n", allFhaeuser [2][FHaus-21].nr);
            else
            {
                printf("Auswahl ungueltig, Sie gelangen zurueck ins Hauptmenue... \n");
                break;
            }
            break;
        default:
            printf("Ungueltige Eingabe\n");
    }
    
    
    printf("\nGeben Sie ihren Namen ein.\n");
    printf("\nName........................: ");
    fpurge(stdin);
    fgets(nam, MAX, stdin);
    ptr = strrchr(nam, '\n');
    *ptr = '\0';
                       
    printf("\nGeben Sie ihren Vornamen ein.\n");
    printf("\nVorname.....................: ");
    fpurge(stdin);
    fgets(vorn, MAX, stdin);
    ptr = strrchr(vorn, '\n');
    *ptr = '\0';
    

    generateBnr++;
    generateCustnr++;
    
    printf("Das ist ihre Kundennummer:...%d\n",generateCustnr);
    printf("\nDas ist ihre Buchungsnummer:...%d\n",generateBnr);
    
    
    int k = 0,l = 0;
    
    for(i=0; i<3; i++)
    {
        for (j=0; j<10; j++)
        {
            if (allFhaeuser[i][j].nr==FHaus)
            {
                k=i;
                l=j;
            }
        }
    }
    Buchung_anhaengen(generateBnr, generateCustnr, nam, vorn, anreisetag,anreisemon,anreisejahr, abreiset,  abreisem, abreisej, k,l);
 
    printf("Ihre Buchung war erfolgreich");
}


// Funktion Stornierung Ferienhaeuser
void stornierung()
{
    int bookNR, custNR;
    
    printf("Ausfuehrung Storno\n");
    printf("Geben Sie ihre Buchungsnummer ein.\n");
    printf("Buchungsnummer................: ");
    scanf("%d",&bookNR);
    printf("Geben Sie ihre Kundennummer ein.\n");
    printf("Kundennummer................: ");
    scanf("%d",&custNR);

    storno(bookNR,custNR);
    
// void eingabe_buchung(void);
}


// Main-Funktion - Menue
int main()
{
    int ende = 0;
    char ausw;
    generateCustnr = 1000000;
    generateBnr = 0;
    initilisierung();
    
    
    do {
        printf("Auswahl ... [A]nfrage [B]uchung [S]torno [E]nde\n >");
        // scanf eingabe char
        scanf("%1s", &ausw);
        fflush(stdin);
        ausw=tolower(ausw);
        printf("Ihre Eingabe war %c\n", ausw);
        

        switch (ausw)
        {
            case 'a':
                anfrage();
                break;
            case 'b':
                buchung();
                break;
            case 's':
                stornierung();
                break;
            case 'e':
                ende = 1;
                break;
            default:
                printf("Ungueltige Eingabe\n");
        }

    } while (ende == 0);

    return 0;
}


/*


//dateiverarbeitung
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 30

struct student
{   int mnr;
    char name[LEN];
    char vname[LEN];
    char fach[LEN];
    int semester;
    char email[LEN];
};

// Funktion zum Vergleich von Variablen des Typs struct student,
// wird ueber Funktionszeiger an qsort() uebergeben
int vglfun(const void *p1, const void *p2)
{
    const struct student *s1 = p1;
    const struct student *s2 = p2;
    int r;
    
    // Rueckgaberegeln rc: <0 wenn s1 < s2
    //                rc:0  wenn s1 == s2
    //                rc: >0  wenn s1 > s2
    
    r = strcmp(s1->fach, s2->fach);
    if (r == 0)
    {
        return s1->semester - s2->semester;
    }
    else
        return r;
}

int main()
{
    const char eingabedatei[] = "C:\\Temp\\daten_eingabe.txt";
    const char ausgabedatei[] = "C:\\Temp\\daten_ausgabe.txt";
    
    struct student s;
    struct student *sfeld;
    int i, n;
    FILE *ein, *aus; // Dateihandle
    
    
    ein = fopen(eingabedatei,"rt");
    if (ein == NULL)
    {
        printf("Fehler beim Oeffnen der Datei %s, Programmende\n", eingabedatei);
        return EXIT_FAILURE;
    }
    
    n = 0;
    do {
        rz = fscanf(ein, "%d %s %s %s %d %s", &s.mnr, s.name, s.vname, s.fach, &s.semester ,s.email
                    );
        if (rz == 6)
        {
            printf("Gelesen: %d  %10s %10s  %15s %d  %s\n", s.mnr, s.name, s.vname, s.fach, s.semes
                   ter, s.email);
            n = n + 1;
        }
        else if (rz == EOF)
            printf("Rueckgabewert: %d (EOF)\n",rz);
        else
            printf("Unerwarteter Rueckgabewert: %d\n",rz);
        
    } while (rz == 6);
    
    // wiederholtes Lesen auf Feld variabler Laenge
    rewind(ein);
    sfeld = malloc(n* sizeof(struct student));
    if (sfeld==NULL)
    {
        printf("Fehler bei Speicherallokation\n");
        return EXIT_FAILURE;
    }
    
    for (i = 0; i < n; i = i + 1)
    {
        rz = fscanf(ein, "%d %s %s %s %d %s", &sfeld[i].mnr, sfeld[i].name, sfeld[i].vname, sfeld[i
                                                                                                  ].fach, &sfeld[i].semester, sfeld[i].email);
    }
    fclose(ein);
    
    // Sortieren, hier nach Studienfach und Semester
    qsort(sfeld, n, sizeof(struct student), vglfun);
    
    // Ausgabe auf Konsole und auf Datei
    aus = fopen(ausgabedatei, "wt");
    if (aus == NULL)
    {
        printf("Fehler beim Oeffnen der Datei %s, Programmende\n", eingabedatei);
        return EXIT_FAILURE;
    }
    for (i = 0; i < n; i = i + 1)
    {
        printf(" %d  %10s %10s  %15s %d  %s\n", sfeld[i].mnr, sfeld[i].name, sfeld[i].vname, sfeld[
                                                                                                   i].fach, sfeld[i].semester, sfeld[i].email);
        rz = fprintf(aus, " %d  %10s %10s  %15s %d  %s\n", sfeld[i].mnr, sfeld[i].name, sfeld[i].vn
                     ame, sfeld[i].fach, sfeld[i].semester, sfeld[i].email);
    }
    fclose(aus);
    
    // Freigabe des Felds variabler Laenge
    free(sfeld);
    
    return EXIT_SUCCESS;
}


*/
