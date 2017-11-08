#include<stdio.h>
#include<string.h>
#include<stdlib.h>




int main(){
    int** iarr = malloc(sizeof(int*)*4);
    int* tmp = malloc(sizeof(int*)*16);
    for( int i = 0; i < 4 ; i++ ){
        iarr[i] = tmp + i * 4;
    }
    

    return 0;
}