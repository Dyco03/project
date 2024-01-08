#ifndef DATE_H
#define DATE_H
typedef struct information{
	char ip_client[40];
	char methode[10];
	char date[100];
	char referent[200];
	char url[200];
}information;

char *transformDate(char tab[]);
#endif