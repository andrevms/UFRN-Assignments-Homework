#include <stdlib.h>
#include <stdio.h>

#define MAX_RAND 100;

int rows, columns;
char* fileName;
int** matrix;


void init_matrixRandom(){
    matrix = (int**)malloc(rows * sizeof(int*));

    for (size_t i = 0; i < rows; i++){
        matrix[i] = (int*) malloc (columns * sizeof(int));
        for (size_t y = 0; y < columns; y++)
            matrix[i][y] = rand() %MAX_RAND;
    }
}

void save_MatrixFile(){
    FILE *fp = fopen(fileName, "w");

    for (size_t i = 0; i < rows; i++)
        fwrite(matrix[i], sizeof(int), columns, fp );
    fclose(fp);
}

void read_MatrixFile(){
    int** m;
    m = (int**)malloc(rows * sizeof(int*));
    
    FILE *fp = fopen(fileName, "r");

    for (size_t i = 0; i < rows; i++){
        m[i] = (int*) malloc (columns * sizeof(int));
        fread(m[i], sizeof(int), columns, fp );
    
    }
    fclose(fp);

    printf("init read file\n");
    for (size_t i = 0; i < rows; i++){
        printf("\n");
        for (size_t y = 0; y < columns; y++)
            printf("%i ", m[i][y]);
    }
    printf("\n");
}


void print_rowsXcolumns(){
    printf("Rows = %i\n",rows);
    printf("Columns = %i\n", columns);
}
void print_matrix(){
    for (size_t i = 0; i < rows; i++){
        printf("\n");
        for (size_t y = 0; y < columns; y++)
            printf("%i ", matrix[i][y]);
    }
    printf("\n");
}

int main(int argc, char* argv[]){
    if (argc != 4)
    {
        printf("Number of arguments differ from 4\n");
        return -1;
    }
    
    rows = atoi(argv[1]);
    columns = atoi(argv[2]);
    fileName = argv[3];

    printf("Nome do arquivo gerado: %s\n", fileName);
    print_rowsXcolumns();
    init_matrixRandom();
    print_matrix();
    save_MatrixFile();
    //read_MatrixFile();

    return 0;
}

