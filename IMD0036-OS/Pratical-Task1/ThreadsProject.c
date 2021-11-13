#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <pthread.h>


int*** matrix;
int* rows, *columns;
char** fileName;
int num_elements;

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
            //printf("C%i %i %i\n", aux1, aux2, m[i][y]);

            count++;
        }
    }

    fclose(fp);
    return m;
}

void* prod_matrix(void *tid){

    int elements = num_elements * (int) (size_t) tid;
    int end_elements = elements + num_elements;
    int max = rows[0]* columns[1];
    
    if (end_elements > max)
    {
        end_elements -= (end_elements - max);
    }

    printf("%s %i %i\n", fileName[(int) (size_t) tid], elements, end_elements);

    clock_t t;
    t = clock();
    for (size_t i = elements; i < end_elements; i++)
    {
        int x = i/rows[0];
        int y = i%columns[1];
        int soma = 0;
        for (size_t j = 0; j < rows[0]; j++)
        {
            soma+= matrix[0][x][j] * matrix[1][j][y];
        }
        matrix[2][x][y] = soma;
    }
    t = clock() - t;

    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("tempo [%f]\n", time_taken); 

    FILE *fp = fopen(fileName[(int) (size_t) tid], "wt");
    fprintf(fp,"%i %i\n", rows[0], columns[1]);

    for (int i = elements; i < end_elements; i++)
        {
            int x = i/rows[0];
            int y = i%columns[1];
            fprintf(fp, "C%i %i %i \n", x, y, matrix[2][x][y]);
        }
        
    fprintf(fp, "%f", time_taken);
    fclose(fp);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    
    matrix = (int***) malloc ((argc) * sizeof(int**));
    rows = (int*) malloc ((argc) * sizeof(int));
    columns = (int*) malloc ((argc) * sizeof(int));

    for (size_t i = 1; i < argc-1; i++)
    {
        matrix[i-1] = load_txt_Matrix(argv[i], i-1);
    }

    matrix[2] = (int**) malloc (rows[0] * sizeof(int*));
    for (size_t i = 0; i < rows[0]; i++)
    {
        matrix[2][i] = (int*) malloc (columns[1]*sizeof(int));
    }

    num_elements = atoi(argv[3]);

    int process = (rows[0] * columns[1])/num_elements;
    if((rows[0] * columns[1])%num_elements != 0){
        process++;
    }

    printf("Number of process  : %i\n", process);

    fileName = (char**) malloc (process * sizeof(char*));
    for (size_t i = 0; i < process; i++)
    {
        fileName[i] = (char*) malloc (100 * sizeof(char*));
        sprintf(fileName[i], "Teste-Thread-%02ld.txt", i);
        //printf("%s\n",fileName[i]);
    }

    void* thread_return;
    pthread_t* thread;
    thread = (pthread_t*) malloc (process * sizeof(pthread_t));
    int* status;
    status = (int*) malloc (process * sizeof(int));
    
    int* count;
    count = (int*) malloc (process * sizeof(int));
    
    for (int i = 0; i < process; i++)
    {
        count[i] = i;
    }

    for (int i = 0; i < process; i++)
    {
       status[i] = pthread_create(&thread[i], NULL, prod_matrix, (void*)(size_t)count[i]);
    
        if (status[i] !=0)
        {
            printf("Error %d\n", status[i]);
            return 1;
        }
        pthread_join(thread[i], &thread_return);
    }

    printf("Processo Pai finalizou\n");
    return 0;
}