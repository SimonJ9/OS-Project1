/* CSCI-4210 Operating Systems Project 1
 *
 * Group Members:
 *
 * Yifan Xu: xuy11
 * Hantian Jiang: jiangh5
 * Chenjun Zhou: zhouc3
 */

#include "file_io_handler.h"
#include "FCFS.h"
#include "SRT.h"
#include "RR.h"

int main(int argc, char* argv[]) {
    
    if(argc != 3)
    {
        fprintf(stderr, "ERROR: Invalid command ormat\n");
        fprintf(stderr, "USAGE: ./a.out <input-file> <stats-output-file>\n");
        return EXIT_FAILURE;
    }
    
    /*data section*/
    FILE *input_file = NULL, *output_file = NULL;
    struct proc_queue process_queue;
    
    /*Check input and output file status*/
    input_file = check_file_status(input_file, argv[1], "r");
    if(input_file == NULL)
    {
        fprintf(stderr, "ERROR: Cannot open input file\n");
        return EXIT_FAILURE;
    }
    output_file = check_file_status(output_file, argv[2], "w");
    if(output_file == NULL)
    {
        fprintf(stderr, "ERROR: Cannot create output file\n");
        return EXIT_FAILURE;
    }
    
    queue_initialize(&process_queue);
    int input_status = parse_input(input_file, &process_queue);
    if(input_status < 0)
    {
        fprintf(stderr, "ERROR: Invalid input file format\n");
        return  EXIT_FAILURE;
    }

#if 0
    print_queue(&process_queue);
#endif

    /* Simulation Configuration
     *
     * n: number of processes to simulate
     * m: number of processors available within CPU
     * t_cs: context switch time
     * t_slice: time slice for RR
     *
     */
    const unsigned int n = process_queue._size;
    const unsigned int m = 1;
    const unsigned int t_cs = 6;
    const unsigned int t_slice = 94;
    
    /*Avoid warnings*/
    if(n != process_queue._size ||
        m > 1)
    {
        return EXIT_FAILURE;    
    }
    
    /*Do Sim-Algos Here*/
    sim_FCFS(&process_queue, t_cs, output_file);
    
    sim_SRT(&process_queue, t_cs, output_file);
    
    sim_RR(&process_queue, t_cs, t_slice, output_file);
    



    close_file(input_file);
    close_file(output_file);
        
    return EXIT_SUCCESS;
}