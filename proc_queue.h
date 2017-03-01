/*
 *This header contains functions for adding process to, and removing 
 *process from the process queue
 */

struct process
{
    char _PID;
    int _t_arrival;
    int _t_burst;
    int _n_burst;
    int _t_io;
};

struct proc_queue
{
    unsigned int _size;
    unsigned int _cap;
    struct process* _queue;
};

int queue_initialize(struct proc_queue* queue)
{
    queue->_size = 0;
    queue->_cap = 0;
    queue->_queue = NULL;
    return 1;
}

int add_process(struct proc_queue* queue, const struct process* p)
{
    if(queue->_size > queue->_cap)
    {
        return -1;
    }
    
    if(queue->_queue == NULL)
    {
        if(queue->_size != 0 || queue->_cap != 0)
        {
            return -1;
        }
        queue->_cap = 1;
        queue->_size = 1;
        queue->_queue = (struct process*)malloc(sizeof(struct process));
        queue->_queue[0] = *p;
        
        return 1;
    }
    
    if(queue->_size >= queue->_cap)
    {
        queue->_cap *= 2;
        queue->_queue = (struct process*)realloc(queue->_queue, 
                        queue->_cap * sizeof(struct process));
    }
    
    queue->_queue[queue->_size] = *p;
    queue->_size++;
    return 1;
}

int remove_process(struct proc_queue* queue, const struct process* p)
{
    int i, j;
    for(i = 0; i < queue->_size; i++)
    {
        if(queue->_queue[i]._PID == p->_PID)
        {
            for(j = i; j < queue->_size-1; j++)
            {
                queue->_queue[j] = queue->_queue[j+1];
            }
            queue->_size--;
            if(queue->_size <= queue->_cap/2)
            {
                queue->_cap /= 2;
                queue->_queue = (struct process*)realloc(queue->_queue, 
                                queue->_cap * sizeof(struct process));
            }
            return 1;
        }
    }
    
    return -1;
}

/*Use this to print queue status*/
char* queue_status(struct proc_queue* queue)
{
    char* result = (char*)malloc(2 * sizeof(char));
    result = strcpy(result, "[Q");
    int i;
    if(queue->_size == 0)
    {
        result = (char*)realloc(result, 11 * sizeof(char));
        strcat(result, " <empty>]");
        return result;
    }
    result = (char*)realloc(result, (2*(queue->_size)+3)*sizeof(char));
    for(i = 0; i < queue->_size; i++)
    {
        strcat(result, " ");
        strcat(result, &(queue->_queue[i]._PID));
    }
    strcat(result, "]");
    return result;
}