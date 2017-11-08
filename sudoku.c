#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>

//bit mask for checking if sudokuresults is valid 
enum { VALID_BITS = 0b1111111110 };


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
int rowcheck(  Gamebox* gamebox ){
    int rowid = gamebox->row;
    unsigned sudokuresults = 0;

    for( int i = 0 ; i < 9 ; i++ ){
        unsigned tmp = 1 << gamebox->sudokuboard[rowid][i];
        sudokuresults |= tmp; 
    }
    if( sudokuresults == VALID_BITS ){
        return 1;
    }
    else return 0; 
}

int colcheck(  Gamebox* gamebox ){
    int colid = gamebox->col;
    unsigned sudokuresults = 0;

    for( int i = 0 ; i < 9 ; i++ ){
        unsigned tmp = 1 << gamebox->sudokuboard[i][colid];
        sudokuresults |= tmp; 
    }
    if( sudokuresults == VALID_BITS ){
        return 1;
    }
    else return 0; 
}

void* rowstartroutine( void* gamebox){
    Gamebox* sudoku = (Gamebox*) gamebox;
    sudoku->result = rowcheck( sudoku);
    return NULL;
}

void* colstartroutine( void* gamebox){
    Gamebox* sudoku = (Gamebox*) gamebox;
    sudoku->result = colcheck( sudoku);
    return NULL;
}

int main( int argc, char *argv[]){
   
    Gamebox gamebox;
    gamebox = bldsudokuboard( gamebox);
    printsudokuboard( gamebox);
    pthread_t tid[18]; 
    int results[18];
    int t_count = 0;
    Gamebox** t_boxes = malloc( 18 * sizeof(Gamebox*));  

    for( int i = 0 ; i < 9 ; i++ ){
        Gamebox* threadboard = (Gamebox*) malloc( sizeof( Gamebox));
        threadboard->row = i;
        threadboard->sudokuboard = gamebox.sudokuboard;
        t_boxes[t_count] = threadboard;
        pthread_create( &tid[t_count], NULL, rowstartroutine, (void*) t_boxes[t_count]);
        t_count++;
    }
    for( int i = 0 ; i < 9 ; i++ ){
        Gamebox* threadboard = (Gamebox*) malloc( sizeof( Gamebox));
        threadboard->col = i;
        threadboard->sudokuboard = gamebox.sudokuboard;
        t_boxes[t_count] = threadboard;
        pthread_create( &tid[t_count], NULL, colstartroutine, (void*) t_boxes[t_count]);
        t_count++;
    }

    for( int i = 0 ; i < 18 ; i++ ){
        pthread_join( tid[i], NULL );
        results[i] = t_boxes[i]->result;
        printf("result %d: %d ", i , t_boxes[i]->result);
        puts(" ");
    }
    for( int i = 0 ; i < 18 ; i++ ){
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