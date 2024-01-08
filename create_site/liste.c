#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct liste{
    char enable[50];
    char disable[50];
    char site[50];

}liste;
int main(){
    FILE *pipe = popen("sudo ls /etc/apache2/sites-available","r");
    int choice = 1; //this is the choice of user 0=liste of site 1=liste of enabled site 2=liste of disabled site
    int number_choice = 0; //this variable help us to choose the number we're going to use
    int number = 0; // we'll used this variable to know the number of site
    int number_enabled = 0;
    int number_disabled = 0;
    char test[200];

    while(fgets(test,200,pipe)){
        number++;
    }
    number--;
    liste lst[number];
    pclose(pipe); //we close the pipe to do the rewind because we can't do rewind with popen
    
    //site processing(traitement)
    pipe = popen("sudo ls /etc/apache2/sites-available","r");
    for(int i = 0 ; fgets(test,200,pipe) ; i++){
        sscanf(test, "%[^\n]",lst[i].site);
        lst[i].site[strcspn(lst[i].site,".")] = '\0';
    }
    pclose(pipe);

    //enabled site processing
    FILE *en_pipe = popen("sudo ls /etc/apache2/sites-enabled","r");
    for(int i = 0 ; fgets(test,200,en_pipe) ; i++){
        sscanf(test, "%[^\n]", lst[i].enable);
        lst[i].enable[strcspn(lst[i].enable,".")] = '\0';
        number_enabled++;
    }
    number_enabled--;
    pclose(en_pipe);

    //site disabled processing
    for(int i = 0; i <= number; i++){
        int is_enabled = 0; //we use this variable to separate the enabled site from disabled site

        for(int j = 0; j <= number_enabled; j++){

            if( strstr(lst[i].site,lst[j].enable) ){ //we are looking for the site wich is not in enabled
                is_enabled = 1;
				break;
            }
        }

        if(!is_enabled){
            strcpy(lst[number_disabled].disable,lst[i].site);
            number_disabled++;
        }

    }
    number_disabled--;
  
    //choice of display
    char title_choice[3][20] = {"disponible","activé","désactivé"};
    switch (choice)
    {
    case 0:
        number_choice = number;
        break;
    case 1:
        number_choice = number_enabled;
        break;
    case 2:
        number_choice = number_disabled;
        break;
    default:
        break;
    }

	
    printf("content-type:text/html\n\n");

    printf("<html>");

        printf("<head>");
            printf("<meta charset=UTF-8>");
            printf("<title>liste</title>");
        printf("</head>");

        printf("<body>");
        //display of site
            printf("<h2>site %s:</h2>",title_choice[choice]);
            printf("<p><ul>");
            for(int i=0 ; i<=number_choice ; i++){

                switch (choice)
                {
                case 0:
                    printf("<li>%s et %d</li>",lst[i].site,i);
                    break;
                case 1:
                    printf("<li>%s et %d</li>",lst[i].enable,i);
                    break;
                case 2:
                    printf("<li>%s et %d</li>",lst[i].disable,i);
                default:
                    break;
                }

            }
            printf("</ul></p>");

        printf("</body>"); 

    return 0;
}
