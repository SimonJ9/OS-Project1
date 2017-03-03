/*
 *RR Simulation
 *
 */

void sim_RR(const struct proc_queue* q, 
            const unsigned int t_cs, 
            const unsigned int t_slice)
{
    /*data section*/
    unsigned int current_time = 0;
    unsigned int id = 0;            //Read from queue to ready queue
    struct proc_queue ready_queue;
    
    queue_initialize(&ready_queue);
    
    printf("time %dms: Simulator started for RR %s\n", 
            current_time, queue_status(&queue));
    
    /*TODO: Implement Algo Here*/
    
    /*cpu started working. time increasing in each loop*/
    while(true)
    {
        /*add process to ready queue upon arrival*/
        if(q->_queue[id]._t_arrival == current_time)
        {
            add_process(&ready_queue, &(q->_queue[id]));
        }
        
        
        current_time++;
    }
    
    
    printf("time %dms: Simulator ended for RR %s\n", 
            current_time, queue_status(&queue));
}