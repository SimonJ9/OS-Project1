/*
 *SRT Simulation
 *
 */
int check_preempt(const struct process* a, const struct process* b){
    int ca = a->_t_burst;
    int cb = b->_t_burst;
    if (a->burst_countdown > 0) ca = a->burst_countdown;
    if (b->burst_countdown > 0) cb = b->burst_countdown;
    return (ca-cb);
}

int compare(const void* a, const void* b){
    const struct process* ia = (struct process *)a;
    const struct process* ib = (struct process *)b;
    return check_preempt(ia,ib);
}



void sim_SRT(const struct proc_queue* q, const unsigned int t_cs)
{
    /*data section*/
    unsigned int current_time = 0;
    
    struct proc_queue rqueue; /* ready queue */
    struct proc_queue ioqueue; /* io queue */
    struct proc_queue bqueue; /* bursting queue */
    
    queue_initialize(&rqueue);
    queue_initialize(&ioqueue);
    queue_initialize(&bqueue);
    
    
    printf("time %dms: Simulator started for SRT %s\n", current_time, queue_status(&rqueue));
    fflush(stdout);
    
    /*TODO: Implement Algo Here*/
    int stillworking = q->_size;
    int switchin = 0;
    int switchout = 0;
    
    int cs = 0; /* context switches */
    int pp = 0; /* preemptions */
    float cpu = 0; /* CPU burst time */
    int burst_num = 0;
    float wait = 0; /* wait time */
    float turnaround = 0; /* turnaround time */
    
    
    int addq = 0;
    
    while(stillworking)
    {
        wait += (&rqueue)->_size;
        
        int i;
        
        if ((&bqueue)->_queue != NULL){

            /* preempt occur */
            if ((&rqueue)->_queue != NULL && ((&bqueue)->_queue[0].burst_countdown > (&rqueue)->_queue[0]._t_burst || 
                    ((&rqueue)->_queue[0].burst_countdown > 0 && (&bqueue)->_queue[0].burst_countdown > (&rqueue)->_queue[0].burst_countdown))){
                
                /* switchout time set */
                if (switchout == 0) {
                    switchout = 1;
                    cs++;
                    (&bqueue)->_queue[0].switch_out_countdown = t_cs/2-1;
                }/* switchout time finish */
                else if ((&bqueue)->_queue[0].switch_out_countdown != 0){
                    (&bqueue)->_queue[0].switch_out_countdown--;
                }
                
                if (switchout == 1 && (&bqueue)->_queue[0].switch_out_countdown == 0){
                    (&bqueue)->_queue[0].burst_countdown++;
                    
                    switchout = 0;
                    addq = add_process((&rqueue),(const struct process *) &((&bqueue)->_queue[0])); /* add back to ready queue*/
                    if (addq < 0) fprintf(stderr, "ERROR: add failed\n");
                    
                    remove_first(&bqueue);
                    qsort( (&rqueue)->_queue, (&rqueue)->_size, sizeof( struct process ), compare);
                }
                        
            }
            else{ /* no preempt occur */
                if ((&bqueue)->_queue[0].burst_countdown == 0){ /* burst finish */
                
                    /* switchout time set */
                    if (switchout == 0) {
                        (&bqueue)->_queue[0]._n_burst--;
                        
                        if ((&bqueue)->_queue[0]._n_burst !=0){
                            printf("time %dms: Process %c completed a CPU burst; %d bursts to go %s\n",current_time, (&bqueue)->_queue[0]._PID, (&bqueue)->_queue[0]._n_burst, queue_status(&rqueue));
                            printf("time %dms: Process %c switching out of CPU; will block on I/O until time %dms %s\n",current_time, (&bqueue)->_queue[0]._PID, current_time+((&bqueue)->_queue[0]._t_io)+3, queue_status(&rqueue));
                            fflush(stdout);

                        }
                        else {
                            printf("time %dms: Process %c terminated %s\n",current_time, (&bqueue)->_queue[0]._PID, queue_status(&rqueue));
                            fflush(stdout);
                            turnaround = turnaround + current_time - (&bqueue)->_queue[0]._t_arrival + 3;
                        }
                        switchout = 1;
                        cs++;
                        (&bqueue)->_queue[0].switch_out_countdown = t_cs/2; 
                        
                        
                    }/* switchout time finish */
                    else if ((&bqueue)->_queue[0].switch_out_countdown != 0){
                        
                        (&bqueue)->_queue[0].switch_out_countdown--;
                    } 
                    
                    if (switchout == 1 && (&bqueue)->_queue[0].switch_out_countdown == 0)  {
                        switchout = 0;
                        if ((&bqueue)->_queue[0]._n_burst != 0){
                            (&bqueue)->_queue[0].io_countdown = (&bqueue)->_queue[0]._t_io;

                            addq = add_process((&ioqueue),(const struct process *) &((&bqueue)->_queue[0]));
                            if (addq < 0) fprintf(stderr, "ERROR: add failed\n");
                        }
                        else stillworking--;
                        
                        remove_first(&bqueue);
                    }

                }
                else (&bqueue)->_queue[0].burst_countdown--; /* keep bursting */
            }
    
        }
        
        /*check io queue*/
        for (i = 0; i < (int)(&ioqueue)->_size && (&ioqueue)->_queue != NULL; i++){
            
            if ((&ioqueue)->_queue[i].io_countdown > 0 ) (&ioqueue)->_queue[i].io_countdown--;
            else{
                
                /* check if preempt occur */
                if ((&bqueue)->_queue != NULL && check_preempt(&((&bqueue)->_queue[0]),&((&ioqueue)->_queue[i])) > 0 && switchout == 0){
                    
                    printf("time %dms: Process %c completed I/O and will preempt %c %s\n", current_time, (&ioqueue)->_queue[i]._PID, (&bqueue)->_queue[0]._PID, queue_status(&rqueue));
                    fflush(stdout);
                    
                    pp++;
                    
                    addq = add_process((&rqueue),(const struct process *) &((&ioqueue)->_queue[i]));
                    if (addq < 0) fprintf(stderr, "ERROR: add failed\n");
                    
                    remove_process(&ioqueue, &((&ioqueue)->_queue[i]));
                    /* if the old first one in ready queue are switching in */
                    if (switchin == 1) switchin = 0;
                    if (i < (int)(&ioqueue)->_size) i--;
                    qsort( (&rqueue)->_queue, (&rqueue)->_size, sizeof( struct process ), compare);
                }
                else{
                    addq = add_process((&rqueue),(const struct process *) &((&ioqueue)->_queue[i]));
                    if (addq < 0) fprintf(stderr, "ERROR: add failed\n");
                    qsort( (&rqueue)->_queue, (&rqueue)->_size, sizeof( struct process ), compare);
                    
                    printf("time %dms: Process %c completed I/O; added to ready queue %s\n", current_time, (&ioqueue)->_queue[i]._PID, queue_status(&rqueue));
                    fflush(stdout);
                    
                    remove_process(&ioqueue, &((&ioqueue)->_queue[i]));
                    if (i < (int)(&ioqueue)->_size) i--;
                    
                }
                
            }
        }
         /*chck if any new process*/
        for (i = 0; i < (int)q->_size; i++){
            if (current_time == q->_queue[i]._t_arrival){
                
                cpu += (q->_queue[i]._t_burst) * (q->_queue[i]._n_burst);
                burst_num += q->_queue[i]._n_burst;
                turnaround -=  (q->_queue[i]._t_io) * (q->_queue[i]._n_burst - 1);
                /* check if preempt occur */
                if ((&bqueue)->_queue != NULL && check_preempt(&((&bqueue)->_queue[0]),&(q->_queue[i])) > 0 && switchout == 0) {
                                
                    printf("time %dms: Process %c arrived and will preempt %c %s\n", current_time, q->_queue[i]._PID, (&bqueue)->_queue[0]._PID, queue_status(&rqueue));
                    fflush(stdout);
                    
                    pp++;
                    
                    addq = add_process((&rqueue),(const struct process *) &(q->_queue[i]));
                    if (addq < 0) fprintf(stderr, "ERROR: add failed\n");
                    /* if the old first one in ready queue are switching in */
                    qsort( (&rqueue)->_queue, (&rqueue)->_size, sizeof( struct process ), compare);
                
                }
                else {
                    addq = add_process((&rqueue),(const struct process *) &(q->_queue[i]));
                    if (addq < 0) fprintf(stderr, "ERROR: add failed\n");
                    qsort( (&rqueue)->_queue, (&rqueue)->_size, sizeof( struct process ), compare);
                
                    printf("time %dms: Process %c arrived and added to ready queue %s\n", current_time, q->_queue[i]._PID, queue_status(&rqueue));
                    fflush(stdout);
                }
                
            }
        }
        
        
        
        /*No burst, but some ready*/
        if ((&bqueue)->_queue == NULL && (&rqueue)->_queue != NULL){
            
            /* switchin time set */
            if (switchin == 0) {
                switchin = 1;
                (&rqueue)->_queue[0].switch_in_countdown = t_cs/2;
            }/* switchin time finish */
            else if ((&rqueue)->_queue[0].switch_in_countdown != 0){
                (&rqueue)->_queue[0].switch_in_countdown--;
            }
            
            if (switchin == 1 && (&rqueue)->_queue[0].switch_in_countdown == 0)  {  
                switchin = 0;
                if ((&rqueue)->_queue[0].burst_countdown != 0) {
                    addq = add_process((&bqueue),(const struct process *) &((&rqueue)->_queue[0]));
                    if (addq < 0) fprintf(stderr, "ERROR: add failed\n");
                    
                    remove_first(&rqueue);
                    printf("time %dms: Process %c started using the CPU with %dms remaining %s\n", current_time, (&bqueue)->_queue[0]._PID, (&bqueue)->_queue[0].burst_countdown, queue_status(&rqueue));
                    fflush(stdout);
                    (&bqueue)->_queue[0].burst_countdown--;
                }
                else {
                    (&rqueue)->_queue[0].burst_countdown = (&rqueue)->_queue[0]._t_burst-1;
                    addq = add_process((&bqueue),(const struct process *) &((&rqueue)->_queue[0]));
                    if (addq < 0) fprintf(stderr, "ERROR: add failed\n");
                    
                    remove_first(&rqueue);
                    printf("time %dms: Process %c started using the CPU %s\n",current_time, (&bqueue)->_queue[0]._PID, queue_status(&rqueue));
                    fflush(stdout);
                }
            }/* switchin time count down */
            
        }
        
        
        
        
        current_time++;
    }
    
    
    current_time--;
    
    wait = wait/burst_num-3;
    
    cpu = cpu / burst_num;
    turnaround = turnaround / burst_num;
    
    
    printf("time %dms: Simulator ended for SRT %s\n", current_time, queue_status(&rqueue));
    fflush(stdout);
}