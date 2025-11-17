
/*
Assignment:
HW4 - Complete Parser and Code Generator for PL/0
(with Procedures, Call, and Else)

Author(s): Sindhuja Sesham, Mikayla Philpot

Language: C (only)

To Compile:

Scanner:
gcc -O2 -std=c11 -o lex lex.c

Parser/Code Generator:
gcc -O2 -std=c11 -o parsercodegen_complete parsercodegen_complete.c

Virtual Machine:
gcc -O2 -std=c11 -o vm vm.c

To Execute (on Eustis):
./lex <input_file.txt>
./parsercodegen_complete
./vm elf.txt

where:
<input_file.txt> is the path to the PL/0 source program

Notes:
- lex.c accepts ONE command-line argument (input PL/0 source file)
- parsercodegen_complete.c accepts NO command-line arguments
- Input filename is hard-coded in parsercodegen_complete.c
- Implements recursive-descent parser for extended PL/0 grammar
- Supports procedures, call statements, and if-then-else
- Generates PM/0 assembly code (see Appendix A for ISA)
- VM must support EVEN instruction (OPR 0 11)
- All development and testing performed on Eustis

Class: COP3402 - System Software - Fall 2025

Instructor: Dr. Jie Lin

Due Date: Friday, November 21, 2025 at 11:59 PM ET
*/
#include <stdio.h>

// Create IR struct
struct instruction{
    int OP; 
    int L;
    int M;
};

int PAS [500] = {0};

// instruction register
struct instruction IR;
int instructionCount;
// Registers
int PC, BP, SP;

int instructionCount;

// BASE FUNCTION
int base ( int BP , int L ){
    int arb = BP ; // activation record base
    while ( L > 0) {
        arb = PAS [ arb ]; // follow static link
        L --;
    }
    return arb ;
}

/* Print out the PAS */
void print(int PAS[500]) {
    // label instructions based on opcode
    if(IR.OP == 1) {
        printf("\nLIT");
    }
    if(IR.OP == 2) {
        // label opcode 2 instructions based on memory
        if(IR.M == 0)
            printf("\nRTN");
        else if(IR.M == 1)
            printf("\nADD");
        else if(IR.M == 2)
            printf("\nSUB");
        else if (IR.M == 3)
            printf("\nMUL");
        else if (IR.M == 4)
            printf("\nDIV");
        else if (IR.M == 5)
            printf("\nEQL");
        else if (IR.M == 6)
            printf("\nNEQ");
        else if (IR.M == 7) {
            printf("\nLSS");
        }
        else if (IR.M == 8) {
            printf("\nLEQ");
        }
        else if (IR.M == 9) {
            printf("\nGTR");
        }
        else if (IR.M == 10) {
            printf("\nGEQ");
        }
        else if (IR.M == 10) {
            printf("\nEVEN");
        }
    }
    if(IR.OP == 3) {
        printf("\nLOD");
    }
    if(IR.OP == 4) {
        printf("\nSTO");
    }
    if(IR.OP == 5) {
        printf("\nCAL");
    }
    if(IR.OP == 6) {
        printf("\nINC");
       
    }
    if(IR.OP == 7) {
        printf("\nJMP");
        //printf("\nInside jump");
    }
    if(IR.OP == 8) {
        printf("\nJPC");
    }
    if(IR.OP == 9) {
        printf("\nSYS");

        
    }
    // print current register values
    printf("\t%d \t%d \t%d \t%d \t%d \t", IR.L, IR.M, PC, BP, SP);

    // print current stack
    int arb = BP;
    int prevLevel = 0;
    while(arb < instructionCount) {
        prevLevel = arb;
        arb = PAS[arb - 1];
    }
    for (int i = arb; SP <= i; i--) {
        if (prevLevel != 0 && prevLevel == i) {
            printf("| ");
        }
        printf("%d ",  PAS[i]);
        
    }
    printf("\n\n");
}

int main(int argc, char *argv[]) {

    // Checking argument count
    if(argc != 2){
        printf("Error! Wrong number of arguments.");
        return 1;
    }
    
    // File handling
    FILE *inputFile = fopen(argv[1], "r");

    if (inputFile == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
    
    // Scanning instructions into PAS
    int i = 499;
    
    while(fscanf(inputFile,"%d",&PAS[i]) == 1) {
            i--;
    }
    instructionCount = i;
    // Setting Global Variables
    PC = 499;
    SP = i + 1;
    BP = SP - 1;

    printf("\tL \tM \tPC \tBP \tSP \tstack\n");
    printf("\nInitial values: \t%d \t%d \t%d\n", PC, BP, SP);

    while(1){
        //printf("Here");
        // Setting Instruction Register
        IR.OP = PAS[PC];
        IR.L = PAS[PC-1];
        IR.M = PAS[PC-2];
        
        // Decrementing PC
        PC = PC - 3;

        // Lit Instruction
        if (IR.OP == 1) {
            SP = SP - 1;  
            PAS[SP] = IR.M;
            print(PAS);
        }
        // OPR Instruction
        else if (IR.OP == 2) {
            
            // RTN
            if(IR.M == 0){
                
                SP = BP + 1;
                BP = PAS[SP - 2];
                PC = PAS[SP - 3];

            // ADD
            } else if(IR.M == 1){
                PAS[SP + 1] = PAS [SP + 1] + PAS[SP];
                SP = SP + 1;
            // SUB
            } else if(IR.M == 2){
                PAS[SP + 1] = PAS [SP + 1] - PAS[SP];
                SP = SP + 1;
            // MUL
            } else if(IR.M == 3){
                PAS[SP + 1] = PAS [SP + 1] * PAS[SP];
                SP = SP + 1;
            // DIV
            } else if(IR.M == 4){
                PAS[SP + 1] = PAS [SP + 1] / PAS[SP];
                SP = SP + 1;
            // EQL
            } else if(IR.M == 5){
                PAS[SP + 1] = (PAS [SP + 1] == PAS[SP]);
                SP = SP + 1;
            // NEQ
            } else if(IR.M == 6){
                if(PAS[SP+1] != PAS[SP])
                    PAS[SP+1] = 1;
                else    
                    PAS[SP+1] = 0;
                SP++;
            // LSS
            } else if(IR.M == 7){
                if(PAS[SP+1] < PAS[SP])
                    PAS[SP+1] = 1;
                else    
                    PAS[SP+1] = 0;
                SP++;
            // LEQ
            } else if(IR.M == 8){
                if(PAS[SP+1] <= PAS[SP])
                    PAS[SP+1] = 1;
                else    
                    PAS[SP+1] = 0;
                SP++;
            // GTR
            } else if(IR.M == 9){
                if(PAS[SP+1] > PAS[SP])
                    PAS[SP+1] = 1;
                else    
                    PAS[SP+1] = 0;
                SP++;
            // GEQ
            } else if(IR.M == 10){
                if(PAS[SP+1] >= PAS[SP])
                    PAS[SP+1] = 1;
                else    
                    PAS[SP+1] = 0;
                SP++;
            } 
            // EVEN
            else if (IR.M == 11) {
                if(PAS[SP] % 2 == 0)
                    PAS[SP+1] = 1;
                else    
                    PAS[SP+1] = 0;
                SP++;
            }
            print(PAS);
        }
        // LOD Instruction
        else if (IR.OP == 3) {
            SP = SP - 1;
            PAS[SP] = PAS[base(BP, IR.L) - IR.M];
            print(PAS);
        }
        // STO Instruction 
        else if (IR.OP == 4) {
            // Store procedure
            PAS[base(BP, IR.L) - IR.M] = PAS[SP];
            SP++;
            print(PAS);
        }
        else if (IR.OP == 5) {
            // Call procedure
            PAS[SP - 1] = base(BP, IR.L);
            PAS[SP - 2] = BP;
            PAS[SP - 3] = PC;
            BP = SP - 1;
            PC = 499 - IR.M;
            print(PAS);
        }
        else if (IR.OP == 6) {
            // Allocating memory on the stack
            SP = SP - IR.M;
            print(PAS);
        }
        else if (IR.OP == 7) {  
            // Unconditional jumps
            PC = 499 - IR.M;
            //printf("Here Jump");
            print(PAS);
        }
        else if (IR.OP == 8) {
            // Conditional jumps
            if(PAS[SP] == 0) {
                PC = 499 - IR.M;
            }
            SP = SP + 1;
            print(PAS);
        }
        else if (IR.OP == 9) {
            if(IR.M== 1){
                // Output integer value at top of stack, then pop
                printf("Output result is: %d\n", PAS[SP]);
                SP = SP + 1;
            } else if(IR.M== 2){
                // Read an integer from stdin and push it
                SP = SP - 1;
                printf("\nPlease Enter an Integer: ");
                scanf("%d, ", &PAS[SP]);

                
            }
            else if (IR.M == 3) {
                print(PAS);
                // Halt program
                break;
            }
            print(PAS);
        }
        
        
    }
    
    fclose(inputFile);

}
