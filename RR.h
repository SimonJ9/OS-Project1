/*
 *RR Simulation
 *
 */

void sim_RR(const struct proc_queue* q)
{
    /*data section*/
    unsigned int current_time = 0;
    struct proc_queue queue;
    
    queue_initialize(&queue);
    
    printf("time %dms: Simulator started for RR %s\n", 
            current_time, queue_status(&queue));
    
    /*TODO: Implement Algo Here*/
    
    
    
    printf("time %dms: Simulator ended for RR %s\n", 
            current_time, queue_status(&queue));
}