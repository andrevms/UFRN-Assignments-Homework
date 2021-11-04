#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<time.h>

#define MAX_RAND 1000;


int** create_Random_Matrix(int rows, int columns){
    
    int** matrix = (int**)malloc(rows * sizeof(int*));

    for (size_t i = 0; i < rows; i++){
        matrix[i] = (int*) malloc (columns * sizeof(int));
        for (size_t y = 0; y < columns; y++)
            matrix[i][y] = rand() %MAX_RAND;
    }

    return matrix;
}

void save_binary_MatrixFile(int** matrix , char* fileName, int rows, int columns){
    FILE *fp = fopen(fileName, "w");
    int* pt = (int*)malloc(2 * sizeof(int));
    pt[0] = rows;
    pt[1] = columns;

    fwrite(pt, sizeof(int), 2, fp);
    for (size_t i = 0; i < rows; i++)
        fwrite(matrix[i], sizeof(int), columns, fp );
    fclose(fp);
}

void save_txt_MatrixFile(int** matrix, char* fileName, int rows, int columns){
    
    FILE *fp = fopen(fileName, "wt");
    fprintf(fp,"%i ",rows);
    fprintf(fp,"%i\n",columns);

    for (size_t i = 0; i < rows; i++){
        for (size_t y = 0; y < columns; y++)
        {
            fprintf(fp, "%i", matrix[i][y]);

            if(y == columns-1){
                fprintf(fp, "\n");
            }
            else{
                fprintf(fp, " ");
            }
        }
    }
        
    fclose(fp);
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    
    if (argc < 2)
    {
        printf("Number of arguments differ from pair\n");
        return -1;
    }
    
    for (int i = 1, count = 0; i < argc; i+= 2, count++)
    {
        int row = atoi(argv[i]);
        int column = atoi(argv[i+1]);
        int** m = create_Random_Matrix(row, column);
        char num = '0' + count;
        
        /*
        char* fileName = (char*)malloc (2* sizeof(char));
        fileName[0]= 'm';
        fileName[1]= num;
        save_binary_MatrixFile(m, fileName, row, column);
        */
       
        char* fileName = (char*)malloc (5* sizeof(char));
        fileName[0]= 'm';
        fileName[1]= num;
        fileName[2]= '.';
        fileName[3]= 't';
        fileName[4]= 'x';
        fileName[5]= 't';
        save_txt_MatrixFile(m, fileName, row, column);
    }
    
    return 0;
}

