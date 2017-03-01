
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "proc_queue.h"

FILE* check_file_status(FILE* f, char* name, char* mode)
{
    if(!(strcmp(mode, "w") == 0 ||
                strcmp(mode, "r") == 0))
    {
        return NULL;
    }
    f = fopen(name, mode);
    
    if(f != NULL) { return f; }
    
    fprintf(stderr, "fopen() failed\n");
    return NULL;
}

int close_file(FILE* f)
{
    fclose(f);
    return 1;
}

int parse_input(FILE* input, struct proc_queue* queue)
{
    int buf_size = 100;
    char buffer[buf_size];
    
    while(fgets(buffer, buf_size, (FILE*)input) != NULL)
    {
        if(buffer[0] == '#' ||
            isblank(buffer[0]) ||
            buffer[0] == '\n')
        {
            continue;
        }
        
        struct process temp;
        char* s = strtok(buffer, "|");
        if(s == NULL) { return -1; }
        temp._PID = s[0];
        
        s = strtok(NULL, "|");
        if(s == NULL) { return -1; }
        temp._t_arrival = atoi(s);
        
        s = strtok(NULL, "|");
        if(s == NULL) { return -1; }
        temp._t_burst = atoi(s);
        
        s = strtok(NULL, "|");
        if(s == NULL) { return -1; }
        temp._n_burst = atoi(s);
        
        s = strtok(NULL, "|");
        if(s == NULL) { return -1; }
        temp._t_io = atoi(s);
        
        /*
        printf("ID: %c, ARR: %d, BUR: %d, NBUR: %d, IO: %d\n", 
            temp._PID, 
            temp._t_arrival, 
            temp._t_burst, 
            temp._n_burst, 
            temp._t_io);
        */
        
        
        add_process(queue, &temp);
    }
    
    return 1;
}

void write_output(FILE* output)
{
    
}

/*For Queue Debugging*/
void print_queue(const struct proc_queue* queue)
{
    printf("Current processes in the queue:\n");
    int i;
    for(i = 0; i < queue->_size; i++)
    {
        printf("ID: %c, ARR: %d, BUR: %d, NBUR: %d, IO: %d\n",
                queue->_queue[i]._PID,
                queue->_queue[i]._t_arrival,
                queue->_queue[i]._t_burst,
                queue->_queue[i]._n_burst,
                queue->_queue[i]._t_io);
    }
}