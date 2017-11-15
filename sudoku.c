#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdbool.h>

//bit mask for checking if sudokuresults is valid 
enum { VALID_BITS = 0b1111111110 };
#define NUM_THREADS 27

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
//the sudoku grid is fractal two levels of nesting(2d Euclidean Space)
//this tells us we can use an offset to be able to 
//recursively solve the same problem 
int sectioncheck( Gamebox* gamebox ){
    int coloffset = gamebox->col * 3;
    int rowoffset = gamebox->row * 3;
    unsigned sudokuresults = 0;
    for( int i = 0 ; i < 3 ; i++ ){
        for(int j = 0 ; j < 3 ; j++){
            unsigned tmp = 1 << gamebox->sudokuboard[ i + rowoffset ][ j + coloffset ];
            sudokuresults |= tmp;
        }
    }
    if( sudokuresults == VALID_BITS ){
        return 1;
    }
    else return 0;
}

void* rowstartroutine( void* gamebox ){
    Gamebox* sudoku = (Gamebox*) gamebox;
    sudoku->result = rowcheck( sudoku);
    return NULL;
}

void* colstartroutine( void* gamebox ){
    Gamebox* sudoku = (Gamebox*) gamebox;
    sudoku->result = colcheck( sudoku);
    return NULL;
}

void* sectionstartroutine( void* gamebox ){
    Gamebox* sudoku = (Gamebox*) gamebox;
    sudoku->result = sectioncheck( sudoku);
    return NULL;
}

int main( int argc, char *argv[]){
   
    Gamebox gamebox;
    gamebox = bldsudokuboard( gamebox);
    printsudokuboard( gamebox);
    pthread_t tid[NUM_THREADS]; 
    int results[NUM_THREADS];
    int t_count = 0;
    bool validsudoku = true;
    //TODO change to NUM_THREADS* sizeof 
    Gamebox** t_boxes = malloc( NUM_THREADS * sizeof(Gamebox*));  

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

    for( int i = 0 ; i < 3 ; i++ ){
        for( int j = 0 ; j < 3 ; j++){
            Gamebox* threadboard = (Gamebox*) malloc( sizeof( Gamebox));
            threadboard->row = i;
            threadboard->col = j;
            threadboard->sudokuboard = gamebox.sudokuboard;
            t_boxes[t_count] = threadboard;
            pthread_create( &tid[t_count], NULL, sectionstartroutine, (void*) t_boxes[t_count]);
            t_count++;
        }
    }
//TODO add wtf you are if invalid  

    for( int i = 0 ; i < 9 ; i++ ){
        pthread_join( tid[i], NULL );
        if( t_boxes[i]->result != 1){
            printf("Not valid sudoku row %d failed!\n", t_boxes[i]->row );
            validsudoku = false;
        }
    }  
   
    for( int i = 9 ; i < 18 ; i++ ){
        
    pthread_join( tid[i], NULL );
        if( t_boxes[i]->result != 1){
            printf("Not valid sudoku col %d failed!\n", t_boxes[i]->col );
            validsudoku = false;
        }
    }
    int grid = 0; 
    for( int i = 18 ; i < 27 ; i++ ){
        pthread_join( tid[i], NULL );
        if( t_boxes[i]->result != 1){
            printf("Not valid sudoku gridrow %d gridcol %d failed!\n", t_boxes[i]->row, t_boxes[i]->col );
            validsudoku = false;
        }
    }
    if( validsudoku){
        puts("valid sudoku!");
    }
    
    return 0;
}

/*
if( rowcheck( gamebox , 1)){
        puts("valid row");
    }
    else puts("not valid row");
*/