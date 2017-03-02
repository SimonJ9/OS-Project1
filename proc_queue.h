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


/*structure for recording process behavior in CPU*/
struct cpu
{
    int _t_cs1_remain;
    int _t_cs2_remain;
    int start;
    int _t_remaining;
    struct process *_p;

};


int cpu_Initialize(struct cpu *cPU);//structure initializer
int cpu_AddProcess(struct cpu *cPU, struct process p, unsigned int cs_time);//copy a process into a CPU
void cpu_TimePass(struct cpu *cPU);//change of process behavior in cPU

int cpu_Initialize(struct cpu *cPU)
{
    cPU->_t_cs1_remain = 0;
    cPU->_t_cs2_remain = 0;
    cPU->_t_remaining = 0;
    cPU->_p = NULL;
    cPU->start = 0;
    return 1;
}

int cpu_AddProcess(struct cpu *cPU, struct process p, unsigned int cs_time)
{
    cPU->_p = &p;
    cPU->_t_cs1_remain = cs_time;
    cPU->_t_cs2_remain = cs_time;
    cPU->_t_remaining = cPU->_p->_t_burst;
    return 1;
}

void cpu_TimePass(struct cpu *cPU){
    if (cPU->_t_remaining <= 0){
        cPU->_t_cs2_remain = max(0, cPU->_t_cs2_remain-1);
        printf("Process %c away from CPU\n", cPU->_p->_PID);
    }
    if (cPU->_t_cs1_remain == 0){
        cPU->start = 1;
        cPU->_t_remaining = cPU->_t_remaining-1;
        #ifdef DEBUG_MDOE
            printf("Process %c away from CPU\n", cPU->_p->_PID);
        #endif
    }
    cPU->_t_cs1_remain = max(0, cPU->_t_cs1_remain-1);
    
    //printf("{Process: %c has time remaining: %d, context switch time remain: %d\n", cPU->_p->_PID, cPU->_t_remaining, cPU->_t_cs_remain);
}


/*structure recordng certain process in I/O*/
struct iotime
{
    int io_remaining;
    const struct process* _p;
};


int iotime_Initialize(struct iotime* ioTime){
    ioTime->io_remaining = 0;
    ioTime->_p = NULL;
    return 1;
}

int iotime_addProcess(struct iotime* ioTime, const struct process* p){
    ioTime->_p = p;
    ioTime->io_remaining = p->_t_io+1;
    return 1;
}

/*structure recording lists of preocess in I/O*/
struct iolist
{
    unsigned int _size;
    unsigned int _cap;
    struct iotime* _io;
};

int io_Initialize(struct iolist* ioList){
    ioList->_size = 0;
    ioList->_cap = 0;
    ioList->_io = NULL;
    return 1;
    
}


int io_addPorcess(struct iolist* ioList, const struct process* p){
    struct iotime ioTime;
    //printf("process %c has io remaining: %d", p->_PID, p->_t_io);
    iotime_Initialize(&ioTime);
    iotime_addProcess(&ioTime, p);
    if(ioList->_size > ioList->_cap)
    {
        printf("fail1\n");
        return -1;
    }
    
    if(ioList->_io == NULL)
    {
        if(ioList->_size != 0 || ioList->_cap != 0)
        {
            return -1;
        }
        ioList->_cap = 1;
        ioList->_size = 1;
        ioList->_io = (struct iotime*)malloc(sizeof(struct iotime));
        ioList->_io[0] = ioTime;
        return 1;
    }
    
    if(ioList->_size >= ioList->_cap)
    {
        ioList->_cap *= 2;
        ioList->_io = (struct iotime*)realloc(ioList->_io, 
                        ioList->_cap * sizeof(struct iotime));
    }
    
    ioList->_io[ioList->_size] = ioTime;
    ioList->_size++;
    return 1;
}

int io_RemoveProcess(struct iolist* ioList, const struct process* p)
{
    int i, j;
    for(i = 0; i < ioList->_size; i++)
    {
        if(ioList->_io[i]._p->_PID == p->_PID)
        {
            for(j = i; j < ioList->_size-1; j++)
            {
                ioList->_io[j] = ioList->_io[j+1];
            }
            ioList->_size--;
            if(ioList->_size <= ioList->_cap/2)
            {
                ioList->_cap /= 2;
                ioList->_io = (struct iotime*)realloc(ioList->_io, 
                                ioList->_cap * sizeof(struct iotime));
            }
            return 1;
        }
    }
    
    return -1;
}


/*one call of the function reduces 1ms of all process in I/O*/
int io_TimePass(struct iolist* ioList){
    if (ioList->_io != NULL){
        int i;
        for (i = 0; i < ioList->_size; i++){
            ioList->_io[i].io_remaining -= 1;
        }
    }
    return 1;
    
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
        printf("fail1\n");
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
        queue->_cap = queue->_cap*2;
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
                queue->_cap = queue->_cap/2;
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