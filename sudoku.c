#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define DIM9 9


struct Gamebox {
    int row;
    int col;
    int** sudokuboard;
};

struct Gamebox bldsudokuboard( struct Gamebox gamebox ){
    gamebox.sudokuboard = malloc( sizeof(int*) * 9 ); 
    int* tmp = malloc( sizeof(int) * 81 );
    for( int i = 0 ; i < 9 ; i++){
        gamebox.sudokuboard[i] = tmp + i * 9;
    }
    FILE* file;
    file = fopen( "test.txt", "r");
    for( int i = 0 ; i < 9 ; i++ ){
        for( int j = 0 ;j < 9 ; j++){
            fscanf( file, "%d", &gamebox.sudokuboard[i][j]);
            }    
    }
    fclose( file);
    return gamebox;
}


void printsudokuboard( struct Gamebox gamebox ){
    for( int i = 0 ; i < 9 ; i++ ){
        for( int j = 0 ; j < 9 ; j++ ){
            printf( "%d", gamebox.sudokuboard[i][j]);
        }
        puts("");
    }
}



 
int main( int argc, char *argv[]){
   
    struct Gamebox gamebox;
    gamebox = bldsudokuboard(  gamebox);
    printsudokuboard( gamebox);

    
    return 0;
}
