/**
 * Name: Carter, Cody
 * Email: ccar115@lsu.edu
 * Project: PA-2 (Page Replacement)
 * Instructor: Feng Chen
 * Class: cs4103-sp24
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

FILE *OutputFile;
int Swap_In_Cost; 
int Swap_Out_Cost;

struct PageReference {
    char operation; 
    int pageNumber;
};

struct Page
{
    int id;
    int modified;
    int time_loaded;
};

void Output_File(const char* Output_File_Name)
{
    OutputFile = fopen(Output_File_Name, "w");
    if (OutputFile == NULL)
    {
        printf("Error: Syntax error in output file name");
    }
}

void FIFO(struct Page* Num_Frame, int memSize, 
          struct PageReference* pageRefs, int numPageRefs)
{
    int head = 0;
    int num_fault_r = 0;
    int total_time = 0;
    int total_swap_in = 0;
    int total_swap_out = 0;
    int num_fault_w = 0;
    
    for(int i = 0; i < numPageRefs; i++)
    {
        int pageNumber = pageRefs[i].pageNumber;
        bool pageFound = false;
        int pageIndex = -1;
        
        for (int j = 0; j < memSize; j++) {
            if (Num_Frame[j].id == pageNumber) {
                pageFound = true;
                pageIndex = j;
                break;
            }
        }
        
        if (!pageFound)
        {
            int victim;
            
            
            if (Num_Frame[head].id != -1) {
                victim = Num_Frame[head].id;
            } else {
                victim = -1;
            }
            
            int swap_out_time = 0;
            if (victim != -1) 
            {
                swap_out_time = Swap_Out_Cost;
                total_swap_out += swap_out_time;
            }
            
            fprintf(OutputFile, "%d:   %c   %d   F   %d   %d   %d\n", i + 1, 
                    pageRefs[i].operation, pageNumber, victim, Swap_In_Cost, swap_out_time);
            
            Num_Frame[head].id = pageNumber;
            if (pageRefs[i].operation == 'W') 
            {
                Num_Frame[head].modified = 1;
                num_fault_w++;
            } 
            
            else 
            {
                Num_Frame[head].modified = 0;
                num_fault_r++;
            }
            
            Num_Frame[head].time_loaded = total_time;
            head = (head + 1) % memSize;
            total_time += Swap_In_Cost;
        }
        else
        {
            if (pageRefs[i].operation == 'W') {
                Num_Frame[pageIndex].modified = 1; 
            }
            fprintf(OutputFile, "%d:   %c   %d   H   -1   0   0\n", i + 1, 
                    pageRefs[i].operation, pageNumber);
        }
    }
    printf("Total number of page references: %d\n", numPageRefs);
    printf("Total number of page faults on read references: %d\n", num_fault_r);
    printf("Total number of page faults on write references: %d\n", num_fault_w); 
    printf("Total number of time units for swapping in pages from swap space: %d\n", total_time);
    printf("Total number of time units for swapping out pages to swap space: %d\n", total_swap_out);
}

void LRU(struct Page* Num_Frame, int memSize, 
struct PageReference* pageRefs, int numPageRefs)
{
    int num_fault = 0;
    int num_fault_r = 0;
    int num_fault_w = 0;
    int total_time_s = 0;
    int total_time_w = 0;
    
    for (int i = 0; i < numPageRefs; i++)
    {
        int pageNumber = pageRefs[i].pageNumber;
        int pageIndex = -1;
        
        for (int j = 0; j < memSize; j++) 
        {
            if (Num_Frame[j].id == pageNumber) 
            {
                pageIndex = j;
                break;
            }
        }
        
                
        if (pageIndex == -1)
        {
            int victim;
            if (Num_Frame[memSize - 1].id != -1) {
                victim = Num_Frame[memSize - 1].id;
            } else {
                victim = -1;
            }
            
            int swap_out_time = 0;
            if (victim != -1) 
            {
                swap_out_time = Swap_Out_Cost;
            }
            
            fprintf(OutputFile, "%d:   %c   %d   F   %d   %d   %d\n", i + 1, 
                pageRefs[i].operation, pageNumber, victim, Swap_In_Cost, swap_out_time);
            
            int replaceIndex = memSize - 1;
            
            for (int j = replaceIndex; j > 0; j--) 
            {
                Num_Frame[j] = Num_Frame[j - 1];
            }
            
            Num_Frame[0].id = pageNumber; 
            Num_Frame[0].modified = 0;
            num_fault++;
            
            if (pageRefs[i].operation == 'R')
            {
                num_fault_r++;
                total_time_s += Swap_In_Cost;
            }
            else if (pageRefs[i].operation == 'W')
            {
                num_fault_w++;
                total_time_w += Swap_Out_Cost;
            }
        }
        
        else
        {
            struct Page temp = Num_Frame[pageIndex];
            for (int j = pageIndex; j > 0; j--)
            {
                Num_Frame[j] = Num_Frame[j - 1];
            }
            Num_Frame[0] = temp;
            
            fprintf(OutputFile, "%d:   %c   %d   H   -1   0   0\n", i + 1, 
            pageRefs[i].operation, pageNumber);
        }
    }
    printf("Total number of page references: %d\n", numPageRefs);
    printf("Total number of page faults on read references: %d\n", num_fault_r);
    printf("Total number of page faults on write references: %d\n", num_fault_w); 
    printf("Total number of time units for swapping in pages from swap space: %d\n", total_time_s);
    printf("Total number of time units for swapping out pages to swap space: %d\n", total_time_w);
}

int main(int argc, char* argv[])
{
    char* rep_Algorithm = argv[1];
    int mem_Size = atoi(argv[2]);
    Swap_In_Cost = atoi(argv[3]);
    Swap_Out_Cost = atoi(argv[4]);
    char* Input_File = argv[5];
    char* Output_File_Name = argv[6];
    
    if (argc != 7)
    {
        printf("Error: Incorect number of input arguments");
        return 1;
    }
    
    if (strcmp(rep_Algorithm, "FIFO") != 0 && strcmp(rep_Algorithm, "LRU") != 0)
    {
        printf("Error: Incorrect replacement algorithm entered, use FIFO or LRU");
        return 1;
    }
    
    FILE *inputFile = fopen(Input_File, "r");
    Output_File(Output_File_Name);
    if (inputFile == NULL)
    {
        printf("Error: Syntax error in input file name");
        return 1;
    }
    
    struct PageReference pageRefs[1000]; 
    int numPageRefs = 0;
    
    while (fscanf(inputFile, " %c %d", &pageRefs[numPageRefs].operation, &pageRefs[numPageRefs].pageNumber) == 2) {
        numPageRefs++;
    }
    
    fclose(inputFile);
    
    struct Page Num_Frame[mem_Size];
    for (int i = 0; i < mem_Size; i++)
    {
        Num_Frame[i].id = -1;
        Num_Frame[i].modified = 0;
    }
    
    if (strcmp(rep_Algorithm, "FIFO") == 0)
    {
        FIFO(Num_Frame, mem_Size, pageRefs, numPageRefs);
    }
    
    if (strcmp(rep_Algorithm, "LRU") == 0)
    {
        LRU(Num_Frame, mem_Size, pageRefs, numPageRefs);
    }
    return 0;
}



