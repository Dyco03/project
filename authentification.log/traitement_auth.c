#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct txt{ //on stockera ici les données de authentification.txt
   char pseudo[50];
   char mdp[200];
}txt;
int main() {
    FILE *fichier;
    fichier=fopen("authentification.txt","r");
    char *donnee = getenv("QUERY_STRING");
    char pseudo[50];
    char mdp[200];
    int i=0;
    int ligne; //nombre de ligne d'authentification.txt
    char test[1000]; //pour y mettre temporairement chaque ligne d'authentification.txt
    char test2[200]; //pour récuperer temporairement les données
    txt *text;
        sscanf(donnee,"pseudo=%[^&]&mdp=%s",pseudo,mdp);
    while( fgets(test,sizeof(test),fichier) ){
        ligne++;
    }
    text=(txt*)malloc(ligne*sizeof(txt)); // allocation de text grâce aux nombre de ligne
    //récuperation des données dans authentification.txt
    rewind(fichier);
    while( fgets(test,sizeof(test),fichier) ){
        sscanf(test,"%[^=]=%[^;]%[^=]=%[^\n]",test2,text[i].pseudo,test2,text[i].mdp);
        i++;
    }
    //récupération des données dans getenv
    sscanf(donnee,"%[^=]=%[^&]&%[^=]=%[^\n]",test2,pseudo,test2,mdp);
    for(int i=0 ; i<ligne ; i++){
        if( strcmp(pseudo,text[i].pseudo)==0 && strcmp(mdp,text[i].mdp)==0){
            printf("Set-Cookie: session=1; Path='/home/dyco/dyco.com/cgi/limitation/auth.cgi'; Max-Age=4000\r\n");
            printf("Location: auth.cgi?valeur=1\r\n\r\n");
        }
    }
    //deconnexion de limitation_auth.c
    if(strstr(donnee,"deconnexion")){
        printf("Set-Cookie: session=; Path='/home/dyco/dyco.com/cgi/limitation/auth.cgi'; Max-Age=0\r\n");
    }
    printf("Location: accueil.html\r\n\r\n");// le code n'arrive jusque là que si aucun authentification n'est réussie
    
    
    return 0;
}