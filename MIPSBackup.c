/*
    Nathan Wilson and Dillon Garcia
    MIPS Project
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    To-Do:
        1- Work on Execute (Mainly Add, Sub, Load, Store)
        2 - Work on DM and WB
        3 - Look at ID again
*/

#define MAX_IM_SIZE 500

typedef struct {
    int op, rx, ry, rz;
} Instruction;

Instruction Instruction_Fetch(int *PC, int InstructionMemory[]);
Instruction Instruction_Decode(Instruction IFID, int RegisterFile[], int DataMemory[]);
Instruction Execute(Instruction IDEX, int RegisterFile[], int DataMemory[], int *PC, int *flag);



int main(int argc, char* argv[])
{
    // Open up the given file name from the user to read
    FILE *inputFile = fopen(argv[1], "r");

    // Store all our instructions in the Instruction Memory Array
    int InstructionMemory[MAX_IM_SIZE];
    int InstructionCounter = 0;

    // Read in all the values in inputFile until we reach the end
    while(!feof(inputFile))
    {
        fscanf(inputFile, "%d", &InstructionMemory[InstructionCounter]);
        InstructionCounter++;
    }

    int counter = InstructionCounter / 4;

    // Close the input file
    fclose(inputFile);

    // Controls when the while loop will end
    int flag = 1;

    // Controls the Program Counter
    int PC = 0;

    // Intializes all the buffers
    Instruction IFID, IDEX, EXDM, DMWB; 

    // Creating a Data Memory array of size 16 and a Register File array of size 8 to store
    // values in their respective arrays.
    int DataMemory[16];
    int RegisterFile[8];   

    while(flag)
    {
        if(counter <= 1)
        {
            flag = 0;
        }
        // Assigns the first buffer (Instruction Fetch -> IFID -> Instruction Decode) all the necessary values
        // from the Instruction Memory and incriments the Program counter by 4
        IFID = Instruction_Fetch(&PC, InstructionMemory);
        printf("IFID: %d %d %d %d\n", IFID.op, IFID.rx, IFID.ry, IFID.rz);
        
        IDEX = Instruction_Decode(IFID, RegisterFile, DataMemory);
        printf("IDEX: %d %d %d %d\n", IDEX.op, IDEX.rx, IDEX.ry, IDEX.rz);

        EXDM = Execute(IDEX, RegisterFile, DataMemory, &PC, &flag);
        printf("EXDM: %d %d %d %d\n", EXDM.op, EXDM.rx, EXDM.ry, EXDM.rz);

        counter -= 1;
    }
    return 0;
}

Instruction Instruction_Fetch(int *PC, int InstructionMemory[])
{
    // Creating an Instruction structure for the Instruction Fetch
    Instruction IF;

    // Give MAR1 the value of the program counter
    int MAR1 = *PC;

    // Incriment the program counter by 4 (One for each bit in our design)
    *PC += 4;

    // Assigns all the values of the Instruction Fetch
    IF.op = InstructionMemory[MAR1];
    IF.rx = InstructionMemory[MAR1 + 1];
    IF.ry = InstructionMemory[MAR1 + 2];
    IF.rz = InstructionMemory[MAR1 + 3];
    
    // Returns the Instruction Fetch structure
    return IF;
}

Instruction Instruction_Decode(Instruction IFID, int RegisterFile[], int DataMemory[])
{
    // Creates an initial Instruction Decode structure to take in the variables from IFID
    Instruction ID;
    ID.op = IFID.op;
    ID.rx = IFID.rx;
    ID.ry = IFID.ry;
    ID.rz = IFID.rz;

    int v1, v2, v3, v4;

    switch(ID.op)
    {
        // Lw (Load Word) (NOT USED)
        case 1:
            break;
        
        // Add
        case 2:
            v1 = RegisterFile[ID.ry];
            v2 = RegisterFile[ID.rz];
            ID.ry = v1;
            ID.rz = v2;
            break;

        // Store Word (NOT USED)
        case 3:
            break;

        // Subtract
        case 4:
            v3 = RegisterFile[ID.ry];
            v4 = RegisterFile[ID.rz];
            ID.ry = v3;
            ID.rz = v4;
            break;
        
        // Input
        case 5:
            break;

        // Output
        case 6:
            break;

        // End of Program
        case 7:
            break;

        // Jmp
        case 8:
            break;

        // Beqz
        case 9:
            ID.ry = 0;
            ID.rz = 0;
            break;

        default:
            break;
    }

    return ID;
}

Instruction Execute(Instruction IDEX, int RegisterFile[], int DataMemory[], int *PC, int *flag)
{
    // Initialize an Instruction struct for Execute
    Instruction EX;

    EX.op = IDEX.op;
    EX.rx = IDEX.rx;
    EX.ry = IDEX.ry;
    EX.rz = IDEX.rz;

    int result;
    int userNumber; 

    switch(EX.op)
    {
        // Lw (Load Word) (USED)
        case 1:
            RegisterFile[EX.rx] = DataMemory[EX.rz];
            break;
        
        // Add
        case 2:
            result = EX.ry + EX.rz;

            EX.ry = 0;
            EX.rz = result;

            break;

        // Store Word (NOT USED)
        case 3:
            DataMemory[EX.rz] = RegisterFile[EX.rx];
            break;

        // Subtract
        case 4:
            result = EX.ry - EX.rz;
            
            // Set ry to 0 and put the result of EX.ry - EX.rz in EX.rz
            EX.ry = 0;
            EX.rz = result;
            break;
        
        // Input
        case 5:
            printf("Enter a number: ");
            scanf("%d", &userNumber);

            EX.rx = userNumber;
            break;

        // Output
        case 6:
            printf("%d\n", EX.rx);
            break;

        // End of Program
        case 7:

            break;

        // Jmp
        case 8:
            break;

        // Beqz
        case 9:
            break;

        default:
            break;       
    }
    return EX;
}
