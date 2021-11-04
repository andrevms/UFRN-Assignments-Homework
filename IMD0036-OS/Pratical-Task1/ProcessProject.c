#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int** matrix1, **matrix2;
int* rows, *columns;


int** load_txt_Matrix(char* fileName, int index){
    FILE *fp = fopen(fileName, "r");
    fscanf(fp,"%i", &rows[index]);
    fscanf(fp,"%i",&columns[index]);
    
    //printf("\nrow %i\n",rows[index]);
    //printf("column %i\n",columns[index]);
        
    int** matrix = (int**)malloc(rows[index] * sizeof(int*));
    for (size_t i = 0; i < rows[index]; i++){
        matrix[i] = (int*) malloc (columns[index] * sizeof(int));
        for (size_t y = 0; y < columns[index]; y++)
        {
            fscanf(fp,"%i", &matrix[i][y]);
            //printf("c%li%li = %i\n",i,y, matrix[i][y]);
        }
    }
    fclose(fp);

    return matrix;
}
int main(int argc, char* argv[]){
    
    rows = (int*)malloc((2) * sizeof(int));
    columns = (int*)malloc((2) * sizeof(int));

    char* fileName = argv[1];
    matrix1 = load_txt_Matrix(fileName, 0);

    fileName = argv[2];
    matrix2 = load_txt_Matrix(fileName, 1);

    int num_Process = atoi(argv[3]);
    printf("Number of process  : %i", num_Process);

    pid_t pid;
    pid = fork();

    if(pid < 0) { /*ocorrência de erro*/
        fprintf(stderr, "Ciração Falhou");
        exit(-1);
    }
    else if (pid == 0) { /*processo filho*/
        printf("Executando o filho (PID=%d), cujo pai tem PID=%d/n", getpid(), getppid());
    }
    else {  /*processo pai*/
        printf("Processo Pai finalizou");
    }

    clock_t t;
    t = clock();
    
    //prod_matrix();
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    return 0;
}