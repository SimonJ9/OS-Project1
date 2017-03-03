/*
 *FCFS Simulation
 *
 */

void sim_FCFS(const struct proc_queue* q, const unsigned int t_cs)
{
    /*data section*/
    unsigned int current_time = 0;
    struct proc_queue queue;
    
    struct process* p;
    
    struct iolist ioqueue;
    
    queue_initialize(&queue);
    
    io_Initialize(&ioqueue);
    printf("time %dms: Simulator started for FCFS %s\n", 
            current_time, queue_status(&queue));
    
    /*TODO: Implement Algo Here*/
    
    int stillworking = 4;
    int cs_time = 0;
    int addq = 0;
    struct cpu CPU;
    cpu_Initialize(&CPU);
    
    while (stillworking > 0){
        int i;
        

        
        for (i = 0; i < q->_size; i++){
            if (current_time == q->_queue[i]._t_arrival){
               addq = add_process(&queue,(const struct process *) &(q->_queue[i]));
               printf("time %dms: Process %c arrived and added to ready queue %s\n", current_time, q->_queue[i]._PID, queue_status(&queue));
            }
        }
        
        if ((&CPU)->_p != NULL && (&CPU)->_t_remaining == 0){
    		cs_time = t_cs/2;
    		p = (&CPU)->_p;
    	    p->_n_burst = p->_n_burst-1;
    		if (p->_n_burst == 0){//terminate process
    			printf("time %dms: Process %c terminated %s", current_time, (&CPU)->_p->_PID, queue_status(&queue));
    			stillworking--;
    		}
    		else{ //kick into I/O
    			printf("time %dms: Process %c completed a (&CPU) burst; %d bursts to go %s\n", current_time, p->_PID, p->_n_burst, queue_status((&queue)));
    			printf("time %dms: Process %c switching out of (&CPU); will block on I/O until time %dms %s\n", current_time, p->_PID, current_time+p->_t_io+cs_time, queue_status((&queue)));
    		}

        }
        if (addq == 0){
            printf("never add\n");
        }
        
        if ((&CPU)->_p ==  NULL && (&queue)->_queue != NULL){
            
            
            int addcpu = cpu_AddProcess((&CPU), (&queue)->_queue[0], t_cs/2);
            #ifdef DEBUG_MODE
                printf("add process %c to cpu\n", (&CPU)->_p->_PID);
            #endif
            if (addcpu != 1){
                fprintf(stderr, "ERROR: Process failed in CPU\n");
            }
            int remove = remove_process(&queue, (&CPU)->_p);
            if (remove < 0){
                fprintf(stderr, "ERROR: remove not smooth\n");
            }
        }
        
        if ((&CPU)->_t_cs1_remain == 0 && (&CPU)->start == 0){
            printf("time %dms: Process %c started using the CPU %s\n", current_time, (&CPU)->_p->_PID, queue_status(&queue));
        }
        
        cpu_TimePass((&CPU));
        
        io_TimePass(&ioqueue);
        
        
		if ((&CPU)->_p != NULL && (&CPU)->_t_cs2_remain == 0 ){
		    if ((&CPU)->_p->_n_burst != 0){
    		    struct process p;
    		    p = *((&CPU)->_p);
    		    //printf("Process %c has io remaining: %d", p->_PID, p->_t_io);
    		    int add = io_addPorcess(&ioqueue, &p);
    		    printf("add process %c to I/O\n", p._PID);
    		    if (add < 0){
    		       fprintf(stderr, "ERROR: Process %c add to I/O failed\n", p._PID);
    		    }
		    }
		    cpu_Initialize(&CPU);
		    printf("at %dms, CPU reinitialized\n", current_time);
		}
		
        //printf("a%db", (&ioqueue)->_io[i].io_remaining);
        for (i = 0; i < (&ioqueue)->_size; i++){
            //printf("a%db", (&ioqueue)->_io[i].io_remaining);
            if ((&ioqueue)->_io[i].io_remaining == 0){
                printf("I/O end\n");
                struct process p;
                p = *((&ioqueue)->_io[i]._p);
                int remove = io_RemoveProcess(&ioqueue, &p);
                if (remove < 0){
                    fprintf(stderr, "ERROR: remove failed\n");
                }
                else{
                    printf("remove successfulof process %c\n", p._PID);
                }
                int add = add_process(&queue, &p);
                if (add < 0){
                    fprintf(stderr, "ERROR: add failed\n");
                }else{
                    printf("add successof process %c\n", p._PID);
                }
                printf("time %dms: Process %c completed I/O; added to ready queue %s\n", current_time, p._PID, queue_status(&queue));
            }
        }
        
        
        
        current_time++;
        //stillworking--;
    }
    

    /*test*/

    
    /*c9test*/

    printf("time %dms: Simulator ended for FCFS %s\n", 
            current_time, queue_status(&queue));
}