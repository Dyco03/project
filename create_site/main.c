#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *fix_slash(char *prototype,int size);

int main() {
    char test[900];
    char server_name[50], server_alias[50], document_root[100], directory[100];
    int exec_cgi = 0, indexes = 0;
    char *get = getenv("QUERY_STRING");

    sscanf(get, "%[^=]=%[^&]&%[^=]=%[^&]&%[^=]=%[^&]&%[^=]=%[^&\n]", test, server_name, test, server_alias, test, document_root, test, directory);
    //we did condition if the options is activated or no
    if(strstr(get,"execCGI")){
        exec_cgi = 1;
    }
    if(strstr(get,"indexes")){
        indexes = 1;
    }

    // Open a temporary file for write information
    FILE *tempFile = fopen("/tmp/temporary_file", "w+");
    if (tempFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier temporaire");
        return EXIT_FAILURE;
    }
    // here we'll fixed the slash(/) problem
    strcpy(document_root,fix_slash(document_root,sizeof(document_root)));
    strcpy(directory,fix_slash(directory,sizeof(directory)));
    // We write information in temporary file
    fprintf(tempFile, "<VirtualHost *:80>\n\n");
    fprintf(tempFile, "\tServerName %s\n", server_name);
    server_name[strcspn(server_name,".")] = '\0';
    fprintf(tempFile, "\tServerAlias %s\n", server_alias);
    fprintf(tempFile, "\tDocumentRoot %s\n\n", document_root);
    fprintf(tempFile, "\t<Directory %s>", directory);
    if(exec_cgi || indexes){
        fprintf(tempFile, "\n\t\tOptions");

        if(indexes){
            fprintf(tempFile, " +Indexes");
        }
        if(exec_cgi){
            fprintf(tempFile, " +ExecCGI\n\t\tAddHandler cgi-script .cgi");
        }        
    }
    fprintf(tempFile, "\n\t\tRequire all granted\n");
    fprintf(tempFile, "\t</Directory>\n\n");
    fprintf(tempFile, "</VirtualHost>");

    // Close the temporary file
    fclose(tempFile);

    // We renamme and use sudo for move the file in apache2 directory
    char command[200];
    snprintf(command, sizeof(command), "sudo mv /tmp/temporary_file /etc/apache2/sites-available/%s.conf", server_name);

    // execute the popen command
    FILE *move = popen(command, "r");
    if (move == NULL) {
        perror("Erreur lors de l'exécution de la commande sudo");
        return EXIT_FAILURE;
    }
    fclose(move);

    // create the DNS
    char dns_command[200];
    snprintf(dns_command,sizeof(dns_command), "echo \"127.0.1.1    %s.com\" | sudo tee -a /home/dyco/miasa",server_name);
    FILE *hosts = popen(dns_command,"r");
    if (hosts == NULL){
        perror("Erreur de l'assignation du dns");
        return EXIT_FAILURE;
    }
    fclose(hosts);

    // site activation
    char activation_com[200];
    snprintf(activation_com,sizeof(activation_com), "sudo a2ensite %s", server_name);
    FILE *site_activation = popen(activation_com,"r");
    if (site_activation == NULL){
        perror("Erreur d'activation du site");
        return EXIT_FAILURE;
    }


    printf("Content-type: text/html\n\n");
    printf("<html>");
    printf("<head>");
    printf("</meta charset = UTF-8>");
    printf("<title> test </title>");
    printf("</head>");
    printf("<body>");
    printf("%s<br/>", get);
    printf("%s  %s  %s  %s %d %d", server_name, server_alias, document_root, directory,exec_cgi,indexes);
    printf("</body>");
    printf("</html>");

    return 0;
}



char *fix_slash(char *prototype,int size){
    char *release = (char*)malloc(size);
    int k = 0;
    for(int i = 0 ; i < strlen(prototype) ; i++,k++){
        if(prototype[i] == '%'){
            release[k] = '/';
            i = i+2;
        }
        else{
            release[k] = prototype[i];
        }

    }
    return release;
}
