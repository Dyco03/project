#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
char *transformDate(char tab[]){
    
    int annee = 2023;
    char *mois = malloc(100);
    mois = strcpy(mois,strtok(tab," "));
    char *jour = malloc(20);
    jour = strcpy(jour,strtok(NULL," "));
    char *date = malloc(1000);
    char *Andro[] = {"Sabotsy","Alahady","Alatsinainy","Talata","Alarobia","Alakamisy","Zoma"};
    char *MoisParDef[] = {"Mar","Avr","Mai","Jun","Jui","Aug","Sep","Oct","Nov","Dec","Jan","Fev"};
    char *Volana[] = {"Martsa","Aprily","May","Jona","Jolay","Aogostra","Septambra","Oktobra","Novambra","Desambra","Janoary","Febroary"};
    int Jour = atoi(jour);
    int m = 0;
    char Mois[100];
    int Annee = annee;
    char andro[100];
    int n=0;

    //récupération de l'année
    for(int i = 0; i < 12; i++){
        if(!(strcmp(MoisParDef[i],mois))){
            m = i+3;
            strcpy(Mois,Volana[i]);
        }
    }
    n = (Jour + (26*(m + 1))/10 + (Annee%100) + ((Annee%100)/4) + ((Annee/100)/4) + ((Annee/100)*5))%7;
    strcpy(andro,Andro[n]);
    sprintf(date,"%s %d %s %d",andro,Jour,Mois,Annee);
    return date;
}