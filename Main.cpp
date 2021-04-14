/****************************************************************************************
 *  Memory Simulator: "Main.cpp"
 * 
 *  Author:     Hannah Perkins
 *  CWID:       11472559
 *  Assignment: Lab #7
 * 
 *  Date:       7 April 2021
 *  Course:     ECE 487
 * 
 *  Function:   Simulates Cache Memory based on user-chosen configuration.
 * 
 *  Description:    The program uses 3 .cpp files to simulate how cache memory stores
 *          main memory blocks in a cache memory table. The first .cpp file is the 
 *          "MemorySim.cpp" file which handles user input and reads a text file to 
 *          construct a sequence of address access table as well as a cache memory 
 *          table. The second .cpp file is the "AddressTable.cpp" file which organizes
 *          and interprets all the information read from the text file. The 
 *          AddressTable calculates main memory blocks, cache memory blocks, cache sets,
 *          and cache hits/misses. The third .cpp file is the "CacheTable.cpp" file.
 *          The Cache Table represents how the cache memory would look to a user after
 *          their sequence of memory accesses is complete.
 * 
 *              Upon startup, the program will create an instance of the MemorySim 
 *          class and perform a list of necessary lab 7 functions under one test
 *          function called RunAll(). After running the memory simulation, the program
 *          prompts the user if they would like to continue with an additional 
 *          simulation. 
 * 
 *          NOTE: No error handling. Program assumes user always enters correct/valid
 *          input.
 *  
 ***************************************************************************************/

#include "MemorySim.cpp"
#include <iostream>
using namespace std;

/****************************************************************************************
 * Function Name:       ContinuePrompt
 * Input Parameters:    void
 * Return Value:        char - represents the user's choice of 'y' = 'yes' or 
 *                      'n' = 'no' 
 *                      to continue
 * Purpose:             Prompts the user if they would like to continue with an 
 *                      additional memory simulation. Collects the user's response 
 *                      as a char.
 ***************************************************************************************/
char ContinuePrompt() {
    // Declare temporary char variable
    char input;

    // Prompt User if they want to continue or exit
    cout << endl << "Continue? (y = yes, n =no): ";
    cin >> input;

    // Return user's response
    return input;
}

/*******************************************************************************
 * --------------------- MAIN -------------------------------------------------
 ******************************************************************************/
int main() {
    // Variable for whether user wants to continue or exit program
    char userInput;

    // Runs Memory Simulator once and checks after each simulation if
    // user wants to continue and run additional simulations.
    do {
        // Create an instance of MemorySim Class called MyMemory
        MemorySim MyMemory;

        // Consider Putting everything into a RunAll() function
        MyMemory.RunAll();

        // Prompt User to continue or not
        userInput = ContinuePrompt();
    } while(userInput == 'y');
}
