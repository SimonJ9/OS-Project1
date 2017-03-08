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
	struct process current_process;
	current_process._PID = 0;

	queue_initialize(&ready_queue);

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
			add_process(&ready_queue, &(q->_queue[id]));
			status = queue_status(&ready_queue);
			printf("time %dms: Process %c arrived and added to ready queue %s\n", current_time, q->_queue[id]._PID, status);
			free(status);
			id++;
		}

		if (current_process._PID == 0 && ready_queue._size > 0)
		{
			current_process = ready_queue._queue[0];
			current_process.switch_in_countdown = t_cs / 2;
		}

		if (current_process.switch_in_countdown > 0)
		{
			current_process.switch_in_countdown--;
			current_time++;
			continue;
		}
		else if (current_process.switch_in_countdown == 0)
		{
			remove_first(&ready_queue);
			current_process.switch_in_countdown = -1;
			status = queue_status(&ready_queue);
			printf("time %dms: Process %c started using the CPU %s\n", current_time, current_process._PID, status);
			free(status);
		}

		//preemption due to time slice

		//if finished, block on io

		/*remove when complete*/
		if (current_time >= 100)
			break;
		current_time++;
	}


	printf("time %dms: Simulator ended for RR %s\n",
		current_time, queue_status(&ready_queue));
}