#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *fix_slash(char *prototype,int size);
int main(){
    char test[100] = "%2Fdyco%2Fhome%2Fdyco.com";
    printf("%s et %ld\n",test,sizeof(test));
    strcpy(test,fix_slash(test,sizeof(test)));
    printf("%s",test);

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
