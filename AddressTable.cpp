/****************************************************************************************
 *  Memory Simulator: "AddressTable.cpp"
 * 
 *  Author:     Hannah Perkins
 *  CWID:       11472559
 *  Assignment: Lab #7
 * 
 *  Date:       7 April 2021
 *  Course:     ECE 487
 * 
 *  Function:   Simulates the rows and columns of the Sequence of Memory Addresses Table
 * 
 *  Description: Address Table Class that simulates how the sequence of memory addresses 
 *          would look. Also displays a column to show if the main memory block is a 
 *          cache hit or miss.
 *  
 ***************************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

/****************************************************************************************
 * Class Name:          AddressTable
 * Purpose:             Simulates the rows and columns of the Sequence of Memory 
 *                      Addresses Table.
 ***************************************************************************************/
class AddressTable {
    public:
        AddressTable();             // Default Constructor
        ~AddressTable();            // Destructor

        // User Functions to Populate Table
        void SetSize(int s);
        void AddRow(string line);
        void CalculateMMBlock(int blockSize);
        void CalculateCMSet(int cacheSize, int blockSize, int mappingAssociation);
        void CalculateCMBlock(int mappingAssociation);

        // Getter Functions
        int GetSize();
        int GetMMAddress(int index);
        bool GetIsWrite(int index);
        int GetMMBlock(int index);
        int GetFirstCMBlock(int index);

        // Setter Function(s)
        void SetHit(int index);

        // Additional Calculation Function(s)
        void CalculateActualHitRate();

        // Print Table
        void Print();

    private:
        // Variables Populated by Text file
        int size;
        vector<bool> isWrite;
        vector<int> mmAddress;

        // Variables Calculated & Set by Program
        vector<string> binaryTag;
        vector<int> mmBlock;
        vector<int> cmSet;
        vector<vector<int>> cmBlock;        // 2D Vector 
        vector<bool> isHit;

        // Variables used for Cache Memory Table
        int cacheLines;
        int cacheSets;
};

/**************************************************************************************
 * Default Constructor
 *************************************************************************************/
AddressTable::AddressTable() {
    size = 0;
}

/**************************************************************************************
 * Destructor
 *************************************************************************************/
AddressTable::~AddressTable() {
    // Clear Vectors to free up memory space for next simulation
    isWrite.clear();
    mmAddress.clear();
    binaryTag.clear();
    mmBlock.clear();
    cmSet.clear();
    cmBlock.clear();
    isHit.clear();
}

/**************************************************************************************
 * Function Name:       SetSize
 * Input Parameters:    int - represents the total number of memory accesses 
 *                      (reads/writes) to occur
 * Return Value:        void
 * Purpose:             Sets the size variable to equal the integer s
 *************************************************************************************/
void AddressTable::SetSize(int s) {
    // Set address Table size equal to integer s
    size = s;
}

/**************************************************************************************
 * Function Name:       AddRow
 * Input Parameters:    string - represents the line to be parsed
 * Return Value:        void
 * Purpose:             Parses a string and adds an element to the isWrite and 
 *                      mmAddress vectors. 
 *************************************************************************************/
void AddressTable::AddRow(string line) {
    // Declare temporary variables for parsing string
    char operation;
    string tempAddress;

    // Read First char to determine if operation is read or write
    operation = line.at(0);

    // If operation is W, set isWrite to 1
    if (operation == 'W') {
        isWrite.push_back(1);
    }
    // If operation is R, set isWrite to 0
    else {
        isWrite.push_back(0);
    }

    // Get address as a substring starting at position 2
    tempAddress = line.substr(2);   

    // Convert string to int and add main memory address to vector
    mmAddress.push_back(stoi(tempAddress));

    // Add a Hit/Miss Variable (Initialized to 0/Miss)
    isHit.push_back(0);
}

/*************************************************************************************
 * Function Name:       CalculateMMBlock
 * Input Parameters:    int - represents size of the blocksize
 * Return Value:        void
 * Purpose:             Calculates the MMBlock Number for each main memory address 
 *                      that is read or written. Populates the mmBlock vector.
 ************************************************************************************/
void AddressTable::CalculateMMBlock(int blockSize) {
    // Calculate and Populate mmBlock Vector
    for(int i=0; i < size; i++) {
        // Check if blockSize 0. (Can't divide by 0)
        if (blockSize == 0) {
            mmBlock.push_back(0);
        }
        else {
            // Main Memory Block Number = Main Memory Address / Block Size
            mmBlock.push_back(mmAddress.at(i) / blockSize);
        }
    }
}

/**************************************************************************************
 * Function Name:       CalculateCMSet
 * Input Parameters:    int cacheSize - represents the size of the cache in bytes
 *                      int blockSize - represents the block size in bytes
 *                      int mappingAssociation - represents the chosen mapping 
 *                      Associaiton (1 = FIFO, 0 = LRU)
 * Return Value:        void
 * Purpose:             Calculates the Cache Memory Set Number for each main memory 
 *                      address that is read or written to. Populates the cmSet vector.
 *************************************************************************************/
void AddressTable::CalculateCMSet(int cacheSize, int blockSize, int mappingAssociation) {
    // Calculate Number of Cache Lines
    // Cache Lines = Cache Size / Block Size
    cacheLines = cacheSize/blockSize;

    // Calculate Total Number of Cache Sets according to N-Way Mapping Associativity
    cacheSets = cacheLines/mappingAssociation;
    
    // Calculate Cache Memory Set Number
    for(int i=0; i < size; i++) {
        // Cache Memory Set Number = MMBlock % cacheSets
        cmSet.push_back(mmBlock.at(i) % cacheSets);
    }
}

/**************************************************************************************
 * Function Name:       CalculateCMBlock
 * Input Parameters:    int mappingAssociation - represents the chosen mapping 
 *                      Association (1 = FIFO, 0 = LRU)
 * Return Value:        void
 * Purpose:             Calculates the Cache Memory Block Numbers for each main memory 
 *                      address that is read or written to. Populates the cmBlock 
 *                      vector.
 *************************************************************************************/
void AddressTable::CalculateCMBlock(int mappingAssociation) {
    // Declare temp vector to hold cache memory blocks
    vector<int> temp;

    // Populate Cache Memory Block vectors
    for(int i=0; i < size; i++) {
        // Number of cmBlocks equals mappingAssociation
        // Ex: 2-Way association means 2 memory blocks in vector
        for(int j=0; j < mappingAssociation; j++){
            temp.push_back((cmSet.at(i) * mappingAssociation) + j);
        }

        // Add temp vector of integers to cmBlock vector
        cmBlock.push_back(temp);

        // Erase items in temp
        temp.clear();
    }
}

/**************************************************************************************
 * Function Name:       GetSize
 * Input Parameters:    void
 * Return Value:        int - represents the number of read and write operations to 
 *                      occur
 * Purpose:             Returns the size integer indicating the total number of reads
 *                      and writes in the Address Table. (Alternatively, can be viewed
 *                      as returning the number of rows in the Address  Table)
 *************************************************************************************/
int AddressTable::GetSize() {
    // Return the number of read and write operations, AKA size
    return size;
}

/**************************************************************************************
 * Function Name:       GetMMAddress
 * Input Parameters:    int - represents the index/row to be accessed
 * Return Value:        int - represents the main memory address
 * Purpose:             Returns the main memory address from a given row of the Address
 *                      Table
 *************************************************************************************/
int AddressTable::GetMMAddress(int index) {
    // Return main memory address from index of table
    return mmAddress.at(index);
}

/***************************************************************************************
 * Function Name:       GetIsWrite
 * Input Parameters:    int index - represents the row of the Address Table to access
 * Return Value:        bool - represents whether or not the operation is a write 
 * Purpose:             Returns a true/1 if the element at the given index is a write 
 *                      operation. Returns a false/0 if the element is a read operation.
 **************************************************************************************/
bool AddressTable::GetIsWrite(int index) {
    // Return the bool from index of isWrite vector
    return isWrite.at(index);
}

/***************************************************************************************
 * Function Name:       GetMMBlock
 * Input Parameters:    int - represents the index/row to be accessed
 * Return Value:        int - represents the main memory block number
 * Purpose:             Returns the main memory block number from a given row in the 
 *                      Address Table
 **************************************************************************************/
int AddressTable::GetMMBlock(int index) {
    // Return the Main Memory block Number from index of mmBlock vector
    return mmBlock.at(index);
}

/**************************************************************************************
 * Function Name:       GetFirstCMBlock
 * Input Parameters:    int - represents the index/row to be accessed
 * Return Value:        int - represents the first item from the 2D vector, cache 
 *                      memory block
 * Purpose:             Returns the first item at the given index from the 2D vector
 *                      called cmBlock. Since the cache memory blocks are sequential
 *                      and there are as many blocks as mappingAssociation, just 
 *                      sending the first item in the vector will suffice rather than
 *                      sending a whole vector.
 **************************************************************************************/
int AddressTable::GetFirstCMBlock(int index) {
    // Return the first item from index of the 2D vector cmBlock
    return cmBlock[index][0];
}

/**************************************************************************************
 * Function Name:       SetHit
 * Input Parameters:    int - represents the index/row to be accessed
 * Return Value:        void
 * Purpose:             Sets the isHit bool at the given index to true. 
 *************************************************************************************/
void AddressTable::SetHit(int index) {
    // Set isHit vector at index to true
    isHit.at(index) = true;
}

/**************************************************************************************
 * Function Name:       CalculateActualHitRate
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Calculates the actual Hit Rate by reading the number of hits in 
 *                      the Address Table and dividing that number by the size (total 
 *                      number of read and write operations to occur). Also, prints the
 *                      actual hit rate for user to see.
 *************************************************************************************/
void AddressTable::CalculateActualHitRate() {
    // Declare double variable for actual hit rate
    double actualHitRate;

    // Initialize totalHits int to 0
    int totalHits = 0;

    // Check each line of table
    for(int i=0; i < size; i++) {
        // Sum total number of hits
        if(isHit.at(i)) {
            // Increment totalHits
            totalHits++;
        }
    }

    // Calculate actual hit rate
    actualHitRate = 100.0 * ((double)totalHits / (double)size);

    // Print Actual Hit Rate
    cout << "Actual hit rate = " << totalHits << "/" << size << " = " << actualHitRate;
    cout << "%" << endl;
}

/**************************************************************************************
 * Function Name:       Print
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Prints the Address Table as the user would see it 
 *************************************************************************************/
void AddressTable::Print() {
    // Print Column Headers
    cout << endl << "main memory address" << "\t" << "mm blk #" << "\t" << "cm set #";
    cout << "\t" << "cm blk #" << "\t" << "hit/miss" << endl;
    cout << "----------------------------------------------------------------------";
    cout << "--------------" << endl;

    // Print Rows
    for(int i=0; i < size; i++) {
        // Print Main Memory Address
        cout << "\t" << mmAddress.at(i);

        // Print Main Memory Block Number
        cout << "\t\t" << mmBlock.at(i);

        // Print Cache Memory Set Number
        cout << "\t\t" << cmSet.at(i) << "\t\t";

        // Print Cache Block Number

        // Case 1: Direct Mapping
        if(cmBlock[i].size() == 1) {
            // Only 1 Cache memory block to print
            cout << cmBlock[i][0];
        }

        // Case 2: 2-Way Associative Mapping
        else if(cmBlock[i].size() == 2) {
            // Print the 2 cache memory blocks as "# OR #"
            cout << cmBlock[i][0] << " or " << cmBlock[i][1];
        }

        // Case 3: N-Way Associative Mapping
        else {
            // Print the range of cache memory blocks as "# to #"
            cout << cmBlock[i][0] << " to " << (cmBlock[i][0] + (cmBlock[i].size()) - 1);
        }

        // Indentation/Formatting Print tabs
        if((cmBlock[i][0] < 10) || (cmBlock[i].size() == 1)) {
            cout << "\t\t";
        }
        else {
            cout << "\t";
        }

        // Print Hit or Miss
        
        // Case 1: Check if Hit
        if(isHit.at(i)) {
            // Print Hit
            cout << "hit" << endl;
        }
        
        // Case 2: Not a Hit
        else {
            // Print Miss
            cout << "miss" << endl;
        }
    }
}