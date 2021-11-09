#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int* rows;
int* columns;
int*** matrix;

int** load_txt_Matrix(char* fileName, int index){
    FILE *fp = fopen(fileName, "r");
    fscanf(fp,"%i %i\n", &rows[index], &columns[index]);
    //printf("%i %i\n", rows[index], columns[index]);

    int count = 0;
    int aux1 , aux2;
    int** m = (int**) malloc( rows[index] * sizeof(int*));    

    for (int i = 0; i < rows[index]; i++){
        m[i] = (int*) malloc( columns[index] * sizeof(int));  
        for (int y = 0; y < columns[index]; y++)
        {
            fscanf(fp,"C%i %i %i\n", &aux1, &aux2, &m[i][y]);
            count++;
        }
    }

    fclose(fp);
    return m;
}

void save_txt_File( char* fileName, double time){
    
    FILE *fp = fopen(fileName, "wt");
    fprintf(fp,"%i %i\n", rows[2], columns[2]);

    for (int x = 0; x < rows[2]; x++){
        for (int y = 0; y < columns[2]; y++)
        {
            fprintf(fp, "C%i %i %i \n", x, y, matrix[2][x][y]);
        }
    }  

    fprintf(fp, "%f", time);
    fclose(fp);
}

int rowXcolumn(int row, int column){
    int soma = 0;
    for(size_t i = 0; i < rows[0]; i++)
    {
        soma += matrix[0][row][i] * matrix[1][i][column];
    }
    return soma;
}

void prod_matrix(){
 
    int count = 0;
    for (size_t i = 0; i < rows[2]; i++)
    {
        for (size_t y = 0; y < columns[2]; y++)
        {
            matrix[2][i][y] = rowXcolumn(i, y);
        }
    }

}

int main(int argc, char* argv[]){
    if (argc != 3)
    {
        printf("Number of arguments differ from 3\n");
        return -1;
    }
    
    matrix = (int***) malloc ((argc) * sizeof(int**));
    rows = (int*) malloc ((argc) * sizeof(int));
    columns = (int*) malloc ((argc) * sizeof(int));

    for (size_t i = 1; i < argc; i++)
    {
        matrix[i-1] = load_txt_Matrix(argv[i], i-1);
    }

    matrix[2] = (int**) malloc( (rows[0]) * sizeof(int*));
    for (size_t i = 0; i < rows[0]; i++)
    {
        matrix[2][i] = (int*) malloc( (columns[1]) * sizeof(int));
    }
    
    rows[2] = rows[0];
    columns[2] = columns[1];
    
    clock_t t;
    t = clock();
    prod_matrix();
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("tempo [%f]\n", time_taken);

    save_txt_File("m3.txt", time_taken);
    return 0;
}

