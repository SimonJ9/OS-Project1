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
	struct proc_queue block_queue;
	struct process current_process;
	current_process._PID = 0;

	queue_initialize(&ready_queue);
	queue_initialize(&block_queue);

	char* status = queue_status(&ready_queue);

	printf("\ntime %dms: Simulator started for RR %s\n",
		current_time, status);
	free(status);

	/*TODO: Implement Algo Here*/

	/*cpu started working. time increasing in each loop*/
	while (1)
	{
		/*add process to ready queue upon arrival*/
		while (q->_queue[id]._t_arrival == current_time)
		{
			struct process tmp = q->_queue[id];
			tmp.burst_countdown = tmp._t_burst;
			tmp.io_countdown = tmp._t_io;
			tmp.switch_in_countdown = t_cs / 2;
			tmp.switch_out_countdown = t_cs / 2;
			tmp.slice_countdown = t_slice;
			add_process(&ready_queue, &tmp);
			status = queue_status(&ready_queue);
			printf("time %dms: Process %c arrived and added to ready queue %s\n", current_time, q->_queue[id]._PID, status);
			free(status);
			id++;
		}

		if (current_process._PID == 0 && ready_queue._size > 0)
		{
			current_process = ready_queue._queue[0];
		}

		if (current_process._PID != 0 && current_process.switch_in_countdown == 0)
		{
			remove_first(&ready_queue);
			current_process.switch_in_countdown = -1;
			status = queue_status(&ready_queue);
			printf("time %dms: Process %c started using the CPU %s\n", current_time, current_process._PID, status);
			free(status);
		}
		if (current_process._PID != 0 && current_process.burst_countdown == 0)
		{
			current_process._n_burst--;
			current_process.burst_countdown = current_process._t_burst;
			current_process.switch_in_countdown = -2;
			status = queue_status(&ready_queue);
			if (current_process._n_burst > 0)
			{
				printf("time %dms: Process %c completed a CPU burst, %d bursts to go %s\n",
					current_time, current_process._PID, current_process._n_burst, status);
				printf("time %dms: Process %c switching out of CPU; will block on I/O until time %dms. %s\n",
					current_time, current_process._PID, current_process._t_io + current_time + t_cs / 2, status);
				current_process.io_countdown = current_process._t_io;
				struct process tmp = current_process;
				tmp.io_countdown += t_cs/2;
				add_process(&block_queue, &tmp);
			}
			else
			{
				printf("time %dms: Process %c terminated %s\n",
					current_time, current_process._PID, status);
			}
			free(status);
		}

		/*preemption due to time slice*/
		if(current_process._PID != 0 && current_process.slice_countdown == 0)
		{
			status = queue_status(&ready_queue);
			if(ready_queue._size > 0)
			{
				printf("time %dms: Time slice expired; process %c preempted with %dms to go %s\n", 
					current_time, current_process._PID, current_process.burst_countdown, status);
				struct process tmp = current_process;
				tmp.slice_countdown = t_slice;
				tmp.switch_in_countdown = t_cs / 2;
				tmp.switch_out_countdown = t_cs / 2;
				add_process(&ready_queue, &tmp);
				current_process.switch_in_countdown = -2;
				current_process.slice_countdown = -1;
			}
			else
			{
				printf("time %dms: Time slice expired; no preemption because ready queue is empty %s\n", 
					current_time, status);
				current_process.slice_countdown = t_slice;
			}
			free(status);
		}
		

		/*io-blocked list countdown*/
		unsigned int i = 0;
		for (i = 0; i < block_queue._size; i++)
		{
			if (block_queue._queue[i].io_countdown == 0)
			{
				struct process t = block_queue._queue[i];
				t.switch_in_countdown = t_cs / 2;
				t.switch_out_countdown = t_cs / 2;
				t.io_countdown = t._t_io;
				t.slice_countdown = t_slice;
				add_process(&ready_queue, &t);
				status = queue_status(&ready_queue);
				printf("time %dms: Process %c completed I/O; added to ready queue %s\n",
					current_time, block_queue._queue[i]._PID, status);
				remove_process(&block_queue, &block_queue._queue[i]);
				i--;
				free(status);
			}
			else
			{
				block_queue._queue[i].io_countdown--;
			}
		}
		if (current_process._PID == 0 && ready_queue._size > 0)
		{
			current_process = ready_queue._queue[0];
		}
		
		if (current_process._PID != 0 && current_process.switch_in_countdown > 0)
		{
			current_process.switch_in_countdown--;
		}

		if (current_process.switch_in_countdown == -1 &&
			current_process.switch_out_countdown == t_cs / 2)
		{
			current_process.burst_countdown--;
			current_process.slice_countdown--;
		}
		if (current_process.switch_in_countdown == -2)
		{
			current_process.switch_out_countdown--;
		}
		
		if (current_process.switch_out_countdown == 0)
		{
			current_process._PID = 0;
		}
		
		/*clear current process*/
		if (current_process._n_burst == 0 && 
			current_process.burst_countdown == 0)
		{
			current_process._PID = 0;
		}

		current_time++;
		/*no more process to arrive*/
		if (ready_queue._size == 0 && 
			current_process._PID == 0)
		{
			if(block_queue._size == 0)
			{
				break;
			}
		}
	}


	printf("time %dms: Simulator ended for RR %s\n",
		current_time, queue_status(&ready_queue));
}