#ifndef DATE_H
#define DATE_H
typedef struct information{
	char user[40];
	char date[100];
	char session[200];
}information;

char *transformDate(char tab[]);
#endif