/* This header contains functions for adding process to, and removing 
 * process from the process queue. Add any functions related to queue 
 * operation here.
 */
 

#define max(a,b) ((a) > (b) ? (a) : (b))
#define buf_size 255;

struct process
{
    char _PID;
    int _t_arrival;
    int _t_burst;
    int _n_burst;
    int _t_io;
    int io_countdown;
    int burst_countdown;
    int switch_in_countdown;
    int switch_out_countdown;
};


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
void remove_first(struct proc_queue* queue);

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
    unsigned int i, j;
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

void remove_first(struct proc_queue* queue)
{
    if(queue == NULL)
    {
        return;
    }
	if (queue->_size == 0)
	{
		return;
	}
    struct process tmp = queue->_queue[0];
    remove_process(queue, &tmp);
}

/*Use this to print queue status*/
char* queue_status(struct proc_queue* queue)
{
	char* result = (char*)malloc(255);
    result = strcpy(result, "[Q");
    unsigned int i;
	unsigned int id = 0;
    if(queue->_size == 0)
    {
        strcat(result, " <empty>]");
        return result;
    }
	id = 3;
    for(i = 0; i < queue->_size; i++)
    {
        strcat(result, " ");
		result[id] = queue->_queue[i]._PID;
		result[id + 1] = '\0';
		id += 2;
    }
    strcat(result, "]");
    return result;
}