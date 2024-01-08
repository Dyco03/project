#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "date.h"
int main(){
	//déclaration
	FILE *fichier;
	fichier=fopen("/var/log/apache2/access.log","r");
	information *info;
	char test[1000];
	int ligne=0;// nombre de ligne de auth.log
	int idx=0;// c'est l'index totale de notre structure information
	int valeur_auth=0; // variable qui va contenir la réponse de notre authentification
	int limitation=0; //on met notre choix de limitation grâce à la variable limitation
	int aide_limite=0;//nous permet de récuperer le nombre de page qu'on pourras afficher
	int i=0; // c'est l'index de chaque structure information
	int choix=0;
	char *var_name; var_name=(char*)malloc(100);
	char *var_value; var_value=(char*)malloc(100);
	//reglage
	int cinq=0;// pour afficher cinq par cinq
	char *get=getenv("QUERY_STRING");
	char *cookie = getenv("HTTP_COOKIE");


	if(get !=NULL && strstr(get,"limit")){
		sscanf(get,"limit=%d",&limitation);

	}
	//recherche
	else if(get !=NULL && strstr(get,"find")){
		sscanf(get,"%[^=]=%[^\n]",var_name,var_value);
		choix=1;
	}
	else{
		limitation=0;
	}
	//pour récuperer le cookie d'authentification
	if(cookie!=NULL && strlen(cookie)>0){
		char temp[20];
		sscanf(cookie,"session=%[^;]",temp);
		valeur_auth=atoi(temp);

	}

	if(fichier==NULL){
		printf("Erreur de flux");
		exit(1);
	}
	
	while(fgets(test,200,fichier)){
		ligne++;
	}
	
	info=(information*)malloc(ligne*sizeof(information));
	rewind(fichier);
	while(fgets(test,1000,fichier)){
		if(strstr(test,"http")){
			
			char test2[1000]; //pour prendre les chaines que l'on ne veut pas
			char date[100];//on le concatenera 
			sscanf(test,"%[^ ]%[^[][%[^ ]%[^\"]\"%[^ ] %[^ ]%[^\"]\"%[^\"]\"%[^\"]%[^ \n]"
			,info[idx].ip_client,test2,info[idx].date,test2,info[idx].methode,info[idx].url,test2,test2,info[idx].referent,test2);
			idx++;
			i++;
		}
	}
	
	
	idx--;//pour corriger idx
	//
	if(valeur_auth && choix==0){
		printf("content-type:text/html\n\n");
		
		printf("<HTML>");
			printf("<HEAD>");
				printf("<META CHARSET='UTF-8'/>");
				printf("<TITLE>Utilisateur</TITLE>");
				printf("<LINK REL='stylesheet' HREF='auth.css'/>");
			printf("</HEAD>");
			
			printf("<BODY>");
				printf("<DIV CLASS='DIV1'>");
					printf("<FORM METHOD='GET' ACTION='limitation_auth.cgi'>");
						printf("<LABEL FOR='find'>Nom d'utilisateur:</LABEL>");
						printf("<INPUT TYPE='text' NAME='find' PLACEHOLDER='ex:root'>");
						printf("<INPUT TYPE='submit' VALUE='rechercher'>");
					printf("</FORM>");	
					printf("<h1>/var/log/apache2/access.log</h1>");
					printf("<TABLE STYLE='border:2px solid black'>");
						printf("<TR>");
							printf("<TH>IP_CLIENT</TH>  <TH>DATE</TH> <TH>METHODE</TH> <TH>URL DEMANDÉ</TH> <TH>REFERENT</TH>");
						printf("</TR>");
						for(int i=0 ;i<=idx;i++){
							if(i>limitation && i<=limitation+10){
							printf("<TR>");
								printf("<TD>%s</TD>  <TD>%s</TD>  <TD>%s</TD> <TD> %s </TD> <TD> %s </TD>"
										,info[i].ip_client,info[i].date,info[i].methode,info[i].url,info[i].referent);
							printf("</TR>");
							}
						}			
					printf("</TABLE>");
				printf("</DIV>");	
				printf("<BR/><BR/>");
				printf("<SPAN CLASS='ligne'>Ligne:</SPAN>");
		aide_limite = limitation;
		// condition pour le prévious 
		if(aide_limite >=10){
			printf("<a href='?limit=%d' CLASS='prevnext'>prev</a>", aide_limite-10);

		}
		// condition pour le next
		if( aide_limite < idx - (idx % 10) ){
			printf("<a href='?limit=%d' CLASS='prevnext'>next</a>", aide_limite+10);
		}
		
		
		//condition pour eviter le surdébordement 
		if ( idx>=50 && aide_limite >= idx - 40 - (idx % 10)) {
			aide_limite = idx - 40 - (idx % 10);	
		}
		//mettre le choix de l'utilisateur dans aide_limite
		while (aide_limite <= idx && (idx > 0 && cinq < 5)) {
			printf("<a href='?limit=%d'>%d+</a>", aide_limite, aide_limite);
			aide_limite += 10;
			cinq++;
		}
		//deconnexion
		printf("</br></br>");
		printf("<A HREF='traitement_auth.cgi?deconnexion=oui' CLASS='deconnexion'>Déconnexion</A>");
		
			printf("</BODY>");	
		printf("</HTML>");
	}
	else if( valeur_auth && choix==1 ){
		printf("content-type:text/html\n\n");
	
		printf("<HTML>");
			printf("<HEAD>");
				printf("<META CHARSET='UTF-8'/>");
				printf("<TITLE>Utilisateur</TITLE>");
				printf("<LINK REL='stylesheet' HREF='auth.css'/>");
			printf("</HEAD>");
			
			printf("<BODY>");
				printf("<DIV CLASS='DIV1'>");
					printf("<FORM METHOD='GET' ACTION='limitation_auth.cgi'>");
						printf("<LABEL FOR='find'>Nom d'utilisateur:</LABEL>");
						printf("<INPUT TYPE='text' NAME='find' PLACEHOLDER='ex:root'>");
						printf("<INPUT TYPE='submit' VALUE='rechercher'>");
					printf("</FORM>");
					printf("<A HREF='limitation_auth.cgi' CLASS='retour'>retour</A>");
					printf("<h1>/var/log/apache2/access.log</h1>");
					printf("<TABLE STYLE='border:2px solid black'>");
						printf("<TR>");
							printf("<TH>IP_CLIENT</TH>  <TH>DATE</TH>  <TH>URL DEMANDÉ</TH> <TH> REFERENT </TH>");
						printf("</TR>");
						for(int i=idx;i>=0;i--){
								if(strstr(info[i].ip_client,var_value)){
									
								printf("<TR>");
									printf("<TD>%s</TD>  <TD>%s</TD>  <TD>%s</TD> <TD> %s </TD> <TD> %s </TD>"
											,info[i].ip_client,info[i].date,info[i].methode,info[i].url,info[i].referent);
								printf("</TR>");
							}
							
						}		
					printf("</TABLE>");
				printf("</DIV>");
				printf("<A HREF='traitement_auth.cgi?deconnexion=oui' CLASS='deconnexion'>Déconnexion</A>");	
			printf("</BODY>");	
		
		printf("</HTML>");
		}
	else{
		printf("Location: accueil.html\r\n\r\n");
	}

	return 0;
}
