/*
 *FCFS Simulation
 *
 */

void sim_FCFS(const struct proc_queue* q, const unsigned int t_cs)
{
    /*data section*/
    unsigned int current_time = 0;
    struct proc_queue queue;
    
    queue_initialize(&queue);
    
    printf("time %dms: Simulator started for FCFS %s\n", 
            current_time, queue_status(&queue));
    
    /*TODO: Implement Algo Here*/
    
    
    /*test*/

    
    /*c9test*/

    printf("time %dms: Simulator ended for FCFS %s\n", 
            current_time, queue_status(&queue));
}