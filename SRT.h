/*
 *SRT Simulation
 *
 */

void sim_SRT(const struct proc_queue* q, const unsigned int t_cs)
{
    /*data section*/
    unsigned int current_time = 0;
    struct proc_queue queue;
    
    queue_initialize(&queue);
    
    printf("time %dms: Simulator started for SRT %s\n", 
            current_time, queue_status(&queue));
    
    /*TODO: Implement Algo Here*/
    
    
    
    printf("time %dms: Simulator ended for SRT %s\n", 
            current_time, queue_status(&queue));
}