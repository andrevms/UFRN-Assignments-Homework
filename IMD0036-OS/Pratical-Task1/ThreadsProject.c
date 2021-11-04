#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int* rows;
int* columns;
int*** matrix;

int** load_txt_Matrix(char* fileName, int index){
    FILE *fp = fopen(fileName, "r");
    fscanf(fp,"%i", &rows[index]);
    fscanf(fp,"%i",&columns[index]);
    
  //  printf("\nrow %i\n",rows[index]);
  //  printf("column %i\n",columns[index]);
        
    int** matrix = (int**)malloc(rows[index] * sizeof(int*));
    for (size_t i = 0; i < rows[index]; i++){
        matrix[i] = (int*) malloc (columns[index] * sizeof(int));
        for (size_t y = 0; y < columns[index]; y++)
        {
            fscanf(fp,"%i", &matrix[i][y]);
    //        printf("c%li%li = %i\n",i,y, matrix[i][y]);
        }
    }
    fclose(fp);

    return matrix;
}


int main(int argc, char* argv[]){
    if (argc != 4)
    {
        printf("Number of arguments differ from 3\n");
        return -1;
    }
    
    matrix = (int***)malloc((argc) * sizeof(int**));
    rows = (int*)malloc((argc) * sizeof(int));
    columns = (int*)malloc((argc) * sizeof(int));

    for (size_t i = 1; i < argc; i++)
    {
        char* fileName = argv[i];
        matrix[i-1] = load_txt_Matrix(fileName, i-1);
    }

    rows[2]= rows[0];
    columns[2]= columns[1];

    matrix[2] = (int**)malloc(rows[0] * sizeof(int*));
    for (size_t i = 0; i < rows[0]; i++){
        matrix[2][i] = (int*) malloc (columns[1] * sizeof(int));
    }

    clock_t t;
    t = clock();
    
    prod_matrix();
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    save_txt_MatrixFile(matrix[2],"m3.txt", rows[2], columns[2], time_taken);
    return 0;
}