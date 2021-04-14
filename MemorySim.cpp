/****************************************************************************************
 *  Memory Simulator: "MemorySim.cpp"
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
 *  Description: Memory Simulator Class that takes user input to define memory
 *          configuration. Also reads a sequence of read and write operations from a
 *          text file to create a sequence of main memory addresses table and a cache
 *          memory table. Both tables are represented by classes of their own: 
 *          AddressTable and CacheTable.
 * 
 *          NOTE: Assumes user will only enter correct input.
 *  
 ***************************************************************************************/

#include "AddressTable.cpp"
#include "CacheTable.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

/****************************************************************************************
 * Class Name:          MemorySim
 * Purpose:             Simulates Cache Memory based on user-chosen configuration
 ***************************************************************************************/
class MemorySim {
    public:
        // Lab Function to Run all required tests
        void RunAll();

        // User Functions
        void UserPrompt();
        void SimulatorOutput();
        void ReadFile();
        void InitializeCacheTable();
        void PopulateAddressTable();
        void RunMemorySequence(); 
        void PrintAddressTable();
        void PrintHitRates();
        void PrintCacheTable();

    private:
        // Variables Set by User
        int mainMemorySize;
        int cacheSize;
        int blockSize;
        int mappingAssociation;
        bool replacementPolicy;
        string fileName; 

        // Variables Calculated and Set by Program
        int addressLines;
        int offset;
        int index;
        int tag;
        int totalCacheSize;

        // Table Data Structures
        AddressTable addressSequenceTable;
        CacheTable cacheMemoryTable;

        // Math & Calculation Functions
        void CalculateAddressLines();
        void CalculateOffsetBits();
        void CalculateIndexBits();
        void CalculateTagBits();
        void CalculateTotalCacheSize();

        // Helper Functions
        string MakeBinaryTag(int address);
};

/****************************************************************************************
 * Function Name:       RunAll
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Runs all of the User accessible functions required for lab demo
 ***************************************************************************************/
void MemorySim::RunAll() {
    // Prompt User for Information
    UserPrompt();

    // Perform Simulator Output
    SimulatorOutput();

    // Initialize the Cache Table
    InitializeCacheTable();

    // Read User Text File
    ReadFile();

    // Populate the Rest of the Address Table
    PopulateAddressTable();

    // Run Sequence of Main Memory Accesses to populate Cache Table
    RunMemorySequence();

    // Print the Address Table
    PrintAddressTable();

    // Calculate and Print the Hit Rates
    PrintHitRates();

    // Print the Cache Table
    PrintCacheTable();
}

/****************************************************************************************
 * Function Name:       UserPrompt
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Collects and Stores user input for main memory size, cache 
 *                      size, block size, mapping associativity, replacement policy,
 *                      and input file name. 
 ***************************************************************************************/
void MemorySim::UserPrompt() {
    // Temporary Variable for Char Input
    char userCharInput;

    // Prompt User for Main Memory Size
    cout << "Enter the size of Main Memory in bytes: ";
    cin >> mainMemorySize;

    // Prompt User for Cache Size
    cout << "Enter the size of the cache in bytes: ";
    cin >> cacheSize;

    // Prompt User for Block Size
    cout << "Enter the cache block/line size: ";
    cin >> blockSize;

    // Prompt User for N-Way Set-Associativity
    cout << endl << "Enter the degree of set-associativity ";
    cout << "(input n for an n-way set-associative mapping): ";
    cin >> mappingAssociation;

    // Prompt User for Replacement Policy
    cout << "Enter the replacement policy (L = LRU, F = FIFO): ";
    cin >> userCharInput;

    // Convert char input to bool
    // LRU = 0/FALSE, FIFO = 1/TRUE
    if(userCharInput == 'L') {
        // User chose LRU Replacement Policy
        replacementPolicy = 0;
    } 
    else {
        // User chose FIFO Replacement Policy
        replacementPolicy = 1;
    }

    // Prompt User for Text File Name
    cout << "Enter the name of the input file containing the list ";
    cout << "of memory references generated by the CPU: ";
    cin >> fileName;
}

/****************************************************************************************
 * Function Name:       SimulatorOutput
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Calls helper functions to calculate addressline fields and
 *                      total cache size. Prints those values to the user.
 ***************************************************************************************/
void MemorySim::SimulatorOutput() {
    // Calculate Address Lines needed
    CalculateAddressLines();

    // Calculate how Address bits are divided among offset, index, and tag
    CalculateOffsetBits();
    CalculateIndexBits();
    CalculateTagBits();

    // Calculate Total Cache Size by adding overhead bits to cache size
    CalculateTotalCacheSize();

    // Print Simulator Output to User
    cout << endl << "Simulator Output: " << endl;
    cout << "Total address lines required = " << addressLines << endl;
    cout << "Number of bits for offset = " << offset << endl;
    cout << "Number of bits for index = " << index << endl;
    cout << "Number of bits for tag = " << tag << endl;
    cout << "Total cache size required = " << totalCacheSize << " bytes" << endl;
}

/****************************************************************************************
 * Function Name:       ReadFile
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Opens the text file chosen by the User and reads the first 
 *                      line to know how many memory address sequences there are. It
 *                      then adds each memory access as a row to the Address Table.
 ***************************************************************************************/
void MemorySim::ReadFile() {
    // Temporary Variable for File Line
    string line;

    // Open File
    ifstream file (fileName);

    // Read First Line
    getline(file,line);

    // Set Address Table's size to number on first line of Text File
    // Note: stoi() converts the string to integer
    addressSequenceTable.SetSize(stoi(line));

    // Read Line by Line of File
    while(getline(file,line)) {
        // Skip if line is empty
        if(line.length() != 0) {
            // Add Row to the Sequence Table
            addressSequenceTable.AddRow(line);

            // Update Cache Table
            //cacheMemoryTable.Update(line);
        } 
    }

    // Close File
    file.close();
}

/****************************************************************************************
 * Function Name:       InitializeCacheTable
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Creates a default cache table of correct sizing. (Empty table,
 *                      no read or write operations have occurred yet)
 ***************************************************************************************/
void MemorySim::InitializeCacheTable() {
    // Initialize CacheTable with cacheLines and Tag Size
    cacheMemoryTable.SetCache((cacheSize / blockSize), tag);
}

/****************************************************************************************
 * Function Name:       PopulateAddressTable
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Fills the main memory block, cache memory set, and cache memory 
 *                      block columns of the Address Table.
 ***************************************************************************************/
void MemorySim::PopulateAddressTable() {
    // Populate Main Memory Blocks
    addressSequenceTable.CalculateMMBlock(blockSize);

    // Populate Cache Memory Set Numbers
    addressSequenceTable.CalculateCMSet(cacheSize, blockSize, mappingAssociation);

    // Populate Cache Memory Block Numbers
    addressSequenceTable.CalculateCMBlock(mappingAssociation);
}

/****************************************************************************************
 * Function Name:       RunMemorySequence
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Goes row by row and performs each memory access in the order
 *                      they appeared on the text file. Populates the Cache Table
 *                      
 *                      3 Major components/cases occuring here:
 *                          
 *                          1. The program is looking for a valid bit of 1 and a tag
 *                          match to populate the isHit column of the Address Table
 * 
 *                          2. The program is searching for an empty cache memory
 *                          block in the Cache Table to store tag info, valid bit,
 *                          dirty bit, and data into. 
 *                          
 *                          3. The program is replacing occupied cache memory blocks
 *                          using the user's chosen replacement policy. 
 * 
 *                      At the end of this function, the Cache Table will appear how
 *                      cache memory would look after the set of reads and writes
 *                      provided in the text file.
 ***************************************************************************************/
void MemorySim::RunMemorySequence() {    
    // For each line of Sequence Table, Get cache block numbers and check them for Tag
    for(int i=0; i < (addressSequenceTable.GetSize()); i++) {
        // Declare variable for whether Tag was Found and variable for temporary Tag
        bool tagFound = false;
        string tempTag = MakeBinaryTag(addressSequenceTable.GetMMAddress(i));
        
        // Declare a firstCMBlock integer
        int firstCMBlock = addressSequenceTable.GetFirstCMBlock(i);

        // Declare index variable for Case 3 to compare clock times
        int index = firstCMBlock;

        // Case 1: Search for a Tag Match
        for(int j=0; j < mappingAssociation; j++) {
            // Check the valid bit at all cache memory blocks that item can be mapped to
            // Loop below won't run once tag is found
            if((!tagFound) && (cacheMemoryTable.GetValidBit(j + firstCMBlock))) {
                // Check if tag is a match
                if(tempTag == cacheMemoryTable.GetBinaryTag(j + firstCMBlock)) {
                    // Set Hit on Address Table
                    addressSequenceTable.SetHit(i);

                    // If IsWrite operation, Set Dirty Bit to true on Cache Table
                    if(addressSequenceTable.GetIsWrite(i)) {
                        cacheMemoryTable.SetDirtyBit((j + firstCMBlock), true);
                    }

                    // Set clockTime for LRU
                    // replacement policy = 0, LRU 
                    if(!replacementPolicy) {
                        // Update clock time whenever accessed in Case 1 or Case 3
                        cacheMemoryTable.SetTimeClock(j + firstCMBlock);
                    }

                    // Set tagFound bit
                    tagFound = true;
                }
            }
        }

        // Case 2: Search for Empty Spot for New Tag
        // Check all cache memory blocks
        for(int j=0; j < mappingAssociation; j++) {
            // Check for false valid bit, means the block is empty
            if((!tagFound) && ((cacheMemoryTable.GetValidBit(j + firstCMBlock)) == 0)) {
                // Set the Valid Bit
                cacheMemoryTable.SetValidBit(j + firstCMBlock, 1);

                // Check if isWrite operation
                if(addressSequenceTable.GetIsWrite(i)) {
                    // Set Dirty Bit to true on Cache Table
                    cacheMemoryTable.SetDirtyBit((j + firstCMBlock), true);
                }

                // Set Tag
                cacheMemoryTable.SetBinaryTag((j + firstCMBlock), addressSequenceTable.GetMMAddress(i), addressLines);

                // Set Data
                cacheMemoryTable.SetData((j + firstCMBlock), (addressSequenceTable.GetMMBlock(i)));

                // Set Clock Time
                cacheMemoryTable.SetTimeClock(j + firstCMBlock);

                // Set tagFound bit
                tagFound = true;
            }
        }

        // Case 3: No Empty Spots, Find FIFO/LRU cache block
        // If LRU, replacement policy = 0 
        // (Set Time Clock whenever accessed in Case 1 or 3)
        // If FIFO, replacement policy = 1 
        // (Only Set Time Clock whenever accessed in Case 3)
        if(!tagFound) {
            // Check all cache memory blocks
            for(int j=0; j < mappingAssociation; j++) {
                
                // Compare time clock at each cache memory block
                if(cacheMemoryTable.GetTimeClock(j + firstCMBlock) < cacheMemoryTable.GetTimeClock(index)) {
                    // Lower time clock means FIFO or LRU item
                    // Set index to smaller value
                    index = j + firstCMBlock;
                }
            }

            // Index = least recently used item or first item entered
            // Replace item at index

            // Set Valid Bit
            cacheMemoryTable.SetValidBit(index, 1);

            // Check if isWrite operation
            if(addressSequenceTable.GetIsWrite(i)) {
                // Set Dirty Bit to true on Cache Table
                cacheMemoryTable.SetDirtyBit(index, 1);
            }
            else {
                // Set Dirty Bit to false if not a write operation
                cacheMemoryTable.SetDirtyBit(index, 0);
            }

            // Set Tag
            cacheMemoryTable.SetBinaryTag(index, addressSequenceTable.GetMMAddress(i), addressLines);

            // Set Data
            cacheMemoryTable.SetData(index, (addressSequenceTable.GetMMBlock(i)));

            // Set Clock Time
            cacheMemoryTable.SetTimeClock(index);

            // Set tagFound true
            tagFound = true;
        }
    }
}

/****************************************************************************************
 * Function Name:       PrintAddressTable
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Calls the Print function from the Address Table structure to
 *                      print the address sequence table as the user would see it.
 ***************************************************************************************/
void MemorySim::PrintAddressTable() {
    // Print the Address Table
    addressSequenceTable.Print();
}

/****************************************************************************************
 * Function Name:       PrintHitRates
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Calculates the highest possible hit rate by storing one of 
 *                      each main memory block in a vector. The size of the vector
 *                      indicates how many unique main memory blocks are going to be
 *                      accessed. Subtracting total reads and writes by the size of
 *                      the tempBlocks vector yields the highest total Hits that 
 *                      can occur.
 ***************************************************************************************/
void MemorySim::PrintHitRates() {
    // Declare vector to count multiple instances of mm block access
    vector<int> tempBlocks;

    // Calculate total Hits Possible
    for(int i=0; i < addressSequenceTable.GetSize(); i++) {
        // Declare flag for whether mmblock was found in temp vector
        bool blockFound = false;

        for(int j=0; j < tempBlocks.size(); j++) {
            if (addressSequenceTable.GetMMBlock(i) == tempBlocks.at(j)) {
                // Set blockFound flag to true if mmblock found
                blockFound = true;
            }
        }

        // If block wasn't found
        if(!blockFound) {
            // Add block to temp vector
            tempBlocks.push_back(addressSequenceTable.GetMMBlock(i));
        }
    }

    // Calculate Total Hits
    int totalAccesses = addressSequenceTable.GetSize();
    int totalHits = totalAccesses - tempBlocks.size();

    // Calculate Highest Possible Hit rate
    double highestHitRate = 100.0 * ((double)totalHits / (double)totalAccesses);

    // Print out Highest Possible Hit Rate
    cout << endl << "Highest possible hit rate = " << totalHits << "/";
    cout << addressSequenceTable.GetSize() << " = " << highestHitRate << "%" << endl;

    // Calculate & Print Actual Hit Rate
    addressSequenceTable.CalculateActualHitRate();

    // Free up memory space, Clear vector
    tempBlocks.clear();
}

/****************************************************************************************
 * Function Name:       PrintCacheTable
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Calls the Print function from the Cache Table structure to 
 *                      print the cache memory table as the user would see it
 ***************************************************************************************/
void MemorySim::PrintCacheTable() {
    // Call Cache Table Print Function
    cacheMemoryTable.Print();
}

/****************************************************************************************
 * Function Name:       CalculateAddressLines
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Calculates and stores the number of address lines needed 
 *                      based on the size of Main Memory. Address Lines = log2 (Main 
 *                      Memory Size)
 ***************************************************************************************/
void MemorySim::CalculateAddressLines() {
    // Calculate number of required address lines
    addressLines = log2(mainMemorySize);
}

/****************************************************************************************
 * Function Name:       CalculateOffsetBits
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Calculates the number of offset bits needed in the address 
 *                      line. Offset is log2(block size).
 ***************************************************************************************/
void MemorySim::CalculateOffsetBits() {
    // Calculate number of offset bits
    offset = log2(blockSize);
}

/****************************************************************************************
 * Function Name:       CalculateIndexBits
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Calculates the number of index bits needed in the address line.
 *                      For N-Way Set Associativity, the number of tag bits is equal to
 *                      log2((main memory size / cache memory size) / N).
 ***************************************************************************************/
void MemorySim::CalculateIndexBits() {
    // Calculate Index
    index = log2((cacheSize/blockSize)/mappingAssociation);
}

/****************************************************************************************
 * Function Name:       CalculateTagBits
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Calculates the number of tag bits needed in the address line.
 *                      For N-Way Set Associativity, the number of tag bits is equal 
 *                      to log2(N * (main memory size / cache memory size)).
 ***************************************************************************************/
void MemorySim::CalculateTagBits() {
    // Calculate Tag
    tag = log2((mainMemorySize/cacheSize)*mappingAssociation);
}

/****************************************************************************************
 * Function Name:       CalculateTotalCacheSize
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Calculates the total Cache Size required by adding the overhead
 *                      bits to the cache size. Overhead bits include 1 valid bit, 1 
 *                      dirty bit, and the tag bits.
 ***************************************************************************************/
void MemorySim::CalculateTotalCacheSize() {
    // Total Cache Size = 1 (Valid Bit) + 1 (Dirty Bit) + Tag + Cache Size
    totalCacheSize = 2 + tag + cacheSize;
}

/****************************************************************************************
 * Function Name:       MakeBinaryTag
 * Input Parameters:    int - represents the main memory address in decimal 
 * Return Value:        string - represents the binary tag as a string of 1's and 0's
 * Purpose:             Calculates the tag in decimal then converts the decimal tag
 *                      into a binary version. Returns the binary tag as a string.
 ***************************************************************************************/
string MemorySim::MakeBinaryTag(int address) {
    // Declare temporary String variable
    string temp = "";
    
    // Get Tag from Address
    for(int i=0; i < (offset + index); i++) {
        // Shift Right
        address = address / 2;
    }

    // Convert Tag to Binary
    for(int i=0; i < tag; i++) {
        // Combine lsb and temp string
        temp = to_string(address % 2) + temp;

        // Shift Right 
        address = address / 2;
    }

    // Return temp string
    return temp;
}