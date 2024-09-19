# Overview
<h5>This C program emulates page replacement in memory using the FIFO (First In First Out) and LRU (Least Recently Used) replacement strategies. This program allows for user input of the replacement strategy, amount of page references, latency in page swap in and out, and input output file names. This program is meant to be run on a linux server, but can be adapted to any compiler. This is a project that I created while taking Operating Systems. </h5>


Instructions For Execution:

1. Navigate to the directory where the filename.c file is located using "cd filename".

2. Type in the command "gcc -o filename filename.c" to compile the code.

3. To view "input_file_name.txt" that the program reads, type in "vim input_file_name.txt" while in the prog2 directory.

4. Type in "./program <"FIFO" or "LRU"> <Frame Count> <Swap in time> <Swap out time> <input_file_name.txt> <output_file_name.txt>" 
   to execute the code. Where you replace "<>" with a value as an argument.

5. If executed properly, the command line output should show 
   the "total page falts: #".
   
6. To view "output_file_name.txt" that the program generated, type in "vim output_file_name.txt" while in the prog2 directory.

7. Inside "output_file_name.txt", you should see the line number, R or W, input being read, F or H, victim page,
   swap in time, and swap out time.
   
