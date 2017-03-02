/* This header contains functions for adding process to, and removing 
 * process from the process queue. Add any functions related to queue 
 * operation here.
 */
 


#define max(a,b) ((a) > (b) ? (a) : (b))

struct process
{
    char _PID;
    int _t_arrival;
    int _t_burst;
    int _n_burst;
    int _t_io;
};

struct cpu
{
    unsigned int _t_cs_remain;
    int start;
    int _t_remaining;
    struct process *_p;

};

int cpu_Initialize(struct cpu *cPU);//structure initializer
int cpu_AddProcess(struct cpu *cPU, struct process p, unsigned int cs_time);//copy a process into a CPU
void cpu_TimePass(struct cpu *cPU);//change of process behavior in cPU

int cpu_Initialize(struct cpu *cPU)
{
    cPU->_t_cs_remain = 0;
    cPU->_t_remaining = 0;
    cPU->_p = NULL;
    cPU->start = 0;
    return 1;
}

int cpu_AddProcess(struct cpu *cPU, struct process p, unsigned int cs_time)
{
    cPU->_p = &p;
    cPU->_t_cs_remain = cs_time;
    cPU->_t_remaining = cPU->_p->_t_burst;
    return 1;
}

void cpu_TimePass(struct cpu *cPU){
    if (cPU->_t_cs_remain == 0){
        cPU->start = 1;
        cPU->_t_remaining--;
    }
    cPU->_t_cs_remain = max(0, cPU->_t_cs_remain-1);
}


struct proc_queue
{
    unsigned int _size;
    unsigned int _cap;
    struct process* _queue;
};

/* Signatures */

/* Use this function to initialize or clear the process queue. 
 * It zeroes all variables in the queue, and makes the queue 
 * pointing to NULL
 */
int queue_initialize(struct proc_queue* queue);

/* Adding process into the queue, returns 1 upon success
 * Automatically resizes the queue if necessary
 */
int add_process(struct proc_queue* queue, const struct process* p);

/* Remove the process from the queue, returns 1 upon success
 * Automatically resizes the queue if necessary
 */
int remove_process(struct proc_queue* queue, const struct process* p);

/* Returns the queue status in the format of homework requirement
 * Use this to print out content in queue
 * Examples: 
 *      empty queue: [Q <empty>]
 *      non-empty queue: [Q A B C]
 */
char* queue_status(struct proc_queue* queue);


/* Implementation */
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