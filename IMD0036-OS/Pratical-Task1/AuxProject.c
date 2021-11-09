#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_RAND 10;


int* create_Random_array(int rows, int columns){
    int num_elements = rows * columns;
    int* array = (int*) malloc (num_elements * sizeof(int*));
    
    for (size_t i = 0; i < num_elements; i++){
            array[i] = rand() %MAX_RAND;
    }
    return array;
}

void save_txt_File(int* matrix, char* fileName, int rows, int columns){
    
    FILE *fp = fopen(fileName, "wt");
    fprintf(fp,"%i %i\n",rows, columns);

    int count = 0;
    for (int x = 0; x < rows; x++){
        for (int y = 0; y < columns; y++)
        {
            fprintf(fp, "C%i %i %i \n", x, y, matrix[count]);
            count++;
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
    
   
    int row = atoi(argv[1]);
    int column = atoi(argv[2]);
    int* m = create_Random_array(row, column);
    save_txt_File(m, "m1.txt", row, column);
    free(m);
    
    row = atoi(argv[3]);
    column = atoi(argv[4]);
    m = create_Random_array(row, column);
    save_txt_File(m, "m2.txt", row, column);
    free(m);

    return 0;
}

