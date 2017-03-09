/*
 *FCFS Simulation
 *
 */

void sim_FCFS(const struct proc_queue* q, const unsigned int t_cs, FILE* out)
{
    /*data section*/
    unsigned int current_time = 0;
    struct proc_queue queue;
    
    struct proc_queue ioqueue;
    struct process p;
    queue_initialize(&queue);
    
    queue_initialize(&ioqueue);
    printf("time %dms: Simulator started for FCFS %s\n", 
            current_time, queue_status(&queue));
    fflush(stdout);
    /*TODO: Implement Algo Here*/
    
    int stillworking = q->_size;
    int cs_time = 0;
    int add = 0;
    struct process* CPU = NULL;
    int t = 0;
    
    int cpu = 0;
    int turn = 0;
    int wait = 0;
    int n_cs = 0;
    //int finish = 0;
    //float turnaround_time = 0;
    //while (current_time < 3000){
    while (stillworking > 0){
        int i;
        //finish cpu
        if (CPU != NULL && CPU->burst_countdown == 0 && t == 2){
    		cs_time = t_cs/2;
    	    CPU->_n_burst = CPU->_n_burst-1;
    		if (CPU->_n_burst == 0){//terminate process
    			printf("time %dms: Process %c terminated %s\n", current_time, CPU->_PID, queue_status(&queue));
    			fflush(stdout);
    		}
    		else{ //kick into I/O
    			printf("time %dms: Process %c completed a (&CPU) burst; %d bursts to go %s\n", current_time, CPU->_PID, CPU->_n_burst, queue_status((&queue)));
    			fflush(stdout);
    			printf("time %dms: Process %c switching out of (&CPU); will block on I/O until time %dms %s\n", current_time, CPU->_PID, current_time+CPU->_t_io+cs_time, queue_status((&queue)));
    			fflush(stdout);
    		}
        }
        //printf("time %dms: after cpu\n", current_time);
        

 		if (CPU != NULL && CPU->switch_out_countdown == 0 && t == 3){
 		    //turnaround_time += current_time - ;
		    if (CPU->_n_burst != 0){
		      //  if (current_time > 2000 && current_time < 3000)
		      //      printf("CPU id is: %c", CPU->_PID);
    		    p = *CPU;
    		  //  if (current_time > 2000 && current_time < 3000){
    		  //      p._PID = 'Z';
    		  //      printf("P id is: %c", p._PID);
    		  //      printf("CPU id now is: %c", CPU->_PID);
    		  //  }
    		    add = add_process(&ioqueue, &p);
    		    if (add < 0){
    		       fprintf(stderr, "ERROR: Process %c add to I/O failed\n", p._PID);
    		    }
		    }else{
		        //printf("CPU turnaround time: %d", q->_queue[0].turnaround_time);
		        //finish += current_time;
		        stillworking--;
		    }
		    CPU = NULL;
		}  
		

        
        // if (current_time > 2610 && current_time < 2700){
        //     //printf("size of queue is %d\n", (&queue)->_size);
        //     if (CPU != NULL){
        //         printf("PCU not null");
        //         printf("time %dms: after push%d, %d, %d, %d\n", current_time, CPU->burst_countdown, CPU->switch_in_countdown, CPU->switch_out_countdown, CPU->io_countdown);
        //     }
                
        // }
        
        if (CPU != NULL && CPU->switch_in_countdown == 0 && t == 1){
            
            printf("time %dms: Process %c started using the CPU %s\n", current_time, CPU->_PID, queue_status(&queue));
            fflush(stdout); 
        }
        
        
		
        for (i = 0; i < (&ioqueue)->_size; i++){
            if ((&ioqueue)->_queue[i].io_countdown == 0){
                struct process p;
                p = (&ioqueue)->_queue[i];
                int remove = remove_process(&ioqueue, &p);
                if (remove < 0)
                    fprintf(stderr, "ERROR: remove failed\n");
                add = add_process(&queue, &p);
                if (add < 0)
                    fprintf(stderr, "ERROR: add failed\n");
                printf("time %dms: Process %c completed I/O; added to ready queue %s\n", current_time, p._PID, queue_status(&queue));
                fflush(stdout);
            }
        }
        
        //add a process to ready queue if time reaches arrive time
        for (i = 0; i < q->_size; i++){
            //printf("loop size: %d has %c", q->_size, q->_queue[i]._PID);
            if (current_time == q->_queue[i]._t_arrival){
                add = add_process(&queue, &(q->_queue[i]));
                if (add < 0){
    		       fprintf(stderr, "ERROR: Process %c add to CPU failed\n", (q->_queue[i])._PID);
    		    }
               printf("time %dms: Process %c arrived and added to ready queue %s\n", current_time, q->_queue[i]._PID, queue_status(&queue));
               fflush(stdout);
            }
        }
        //printf("time %dms: after add\n", current_time);
        
        
        //push into cpu
        if (CPU ==  NULL && (&queue)->_size > 0){
            p = (&queue)->_queue[0];
            
            CPU = &p;
            int remove = remove_process(&queue, CPU);
            if (remove < 0){
                fprintf(stderr, "ERROR: remove not smooth\n");
            }
            CPU->burst_countdown = CPU->_t_burst;
            CPU->switch_in_countdown = t_cs/2;
            CPU->switch_out_countdown = t_cs/2;
            CPU->io_countdown = CPU->_t_io;
            //printf("start in: %d\n", CPU->burst_countdown);
        }
        
        t = timePass(CPU);
        
        if (t == 1 || t == 3){
            n_cs += 1;
        }
        if (t == 2)
            cpu+=1;
        
        if (t > 0 && t < 4){
            turn += 1;
        }
        
        turn += (&queue)->_size;
        wait += (&queue)->_size;
        
        queue_timePass(&ioqueue);

        current_time++;
    }

    

    /*test*/

    
    /*c9test*/

    printf("time %dms: Simulator ended for FCFS\n", 
            current_time-1);
    fflush(stdout);
    
    //float burst_time = 0;
    int n_burst = 0;
    int i;
    for ( i = 0; i < q->_size; i++){
        //burst_time += q->_queue[i]._n_burst * q->_queue[i]._t_burst;
        n_burst += q->_queue[i]._n_burst;
    }
    //int tr_time = 0;
    // for (i = 0; i < q->_size; i++){
    //     printf("around time: %d", q->_queue[i].turnaround_time);
    //     finish -= q->_queue[i]._t_arrival;
    //     finish -= q->_queue[i]._t_io * (q->_queue[i]._n_burst - 1);
    // }
    
    
    
    
    /*-----@Simon hacked into your code!-----*/
    /*
    printf("Algorithm FCFS\n");
    printf("-- average CPU burst time: %.2f ms\n", (float)cpu/(float)n_burst);
    printf("-- average wait time: %.2f ms\n", (float)wait/(float)n_burst);
    printf("-- average turnaround time: %.2f ms\n", (float)turn/(float)n_burst);
    printf("-- total number of context switches: %d\n", n_cs/t_cs);
    printf("-- total number of preemptions: 0\n\n");
    */
    printf("\n");
    fflush(stdout);
	write_output
	(
		out, 
		(float)cpu/(float)n_burst, 
		(float)wait/(float)n_burst, 
		(float)turn/(float)n_burst, 
		n_cs/t_cs, 
		0, 
		"FCFS"
	);
	
	/*-----@Simon had run away _(:з」∠)_-----*/
}