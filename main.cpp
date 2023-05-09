#include <iostream>
#include "pin.H"

using namespace std;

int main(int argc, char *argv[])
{
    // init symbols table to add instrument in routines
    PIN_InitSymbols();

    // exit if pin init failed
    if(PIN_Init(argc,argv))
    {
        cerr << "load file " << argv[0] << "failed!" << endl;
        return -1;
    }

    // hook syscall


    // add image instrument
    IMG_AddInstrumentFunction(Image, NULL);

    // add instruction instrument
    INS_AddInstrumentFunction(Instruction, NULL);

    // start program
    PIN_StartProgram();

    return 0;
}