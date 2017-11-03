#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>

typedef enum{
    row, col,
    square
}type;

typedef struct {
    int row;
    int col;
    int result;
    int** sudokuboard;
}Gamebox;

Gamebox bldsudokuboard(  Gamebox gamebox ){
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


void printsudokuboard( Gamebox gamebox ){
    for( int i = 0 ; i < 9 ; i++ ){
        for( int j = 0 ; j < 9 ; j++ ){
            printf( "%d", gamebox.sudokuboard[i][j]);
        }
        puts("");
    }
}

//rowid should be 0 1 ... 8
int rowcheck(  Gamebox gamebox , int rowid ){
    unsigned sudokuresults = 0;
    unsigned validresults = 0;
    for( int i = 1 ; i <= 9 ; i++ ){
        unsigned tmp = 1 << i; 
        validresults |= tmp; 
    }
    for( int i = 0 ; i < 9 ; i++ ){
        printf( "%d", gamebox.sudokuboard[rowid][i]);
        unsigned tmp = 1 << gamebox.sudokuboard[rowid][i];
        sudokuresults |= tmp; 
    }
    if( ( sudokuresults & validresults) == validresults ){
    return 1;
    }
    else return 0; 
}

void* startroutine( void* gamebox){
    Gamebox* soduku = (Gamebox*) gamebox;
    soduku->result = rowcheck( *soduku, soduku->row);
    return NULL;
}
 
int main( int argc, char *argv[]){
   
    Gamebox gamebox;
    gamebox = bldsudokuboard( gamebox);
    pthread_t tid[9]; 
    int results[9];
    Gamebox** t_boxes = malloc( 9 * sizeof(Gamebox*));   

    for( int i = 0 ; i < 9 ; i++ ){
        Gamebox* gamebox = malloc( sizeof(Gamebox));
        gamebox->row = i;
        t_boxes[i] = gamebox;
        pthread_create( &tid[i], NULL, startroutine, (void*) &t_boxes[i]);
    }
    for( int i = 0 ; i < 9 ; i++ ){
        pthread_join( tid[i], NULL );
        results[i] = t_boxes[i]->result;
    }
    for( int i = 0 ; i < 9 ; i++ ){
        if( results[i] != 1 ){
            puts("not a valid sudoku!");
            return 1;
        }
    }
    puts("valid sudoku!");
    return 0;
}

/*
if( rowcheck( gamebox , 1)){
        puts("valid row");
    }
    else puts("not valid row");
*/