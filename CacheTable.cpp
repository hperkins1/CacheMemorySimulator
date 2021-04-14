/****************************************************************************************
 *  Memory Simulator: "CacheTable.cpp"
 * 
 *  Author:     Hannah Perkins
 *  CWID:       11472559
 *  Assignment: Lab #7
 * 
 *  Date:       7 April 2021
 *  Course:     ECE 487
 * 
 *  Function:   Simulates the Cache Table rows and columns
 * 
 *  Description: Cache Table Class that simulates how the cache would look after a 
 *          series of read and write operations. 
 *  
 ***************************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

/****************************************************************************************
 * Class Name:          CacheTable
 * Purpose:             Simulates the rows and columns of the Cache Memory Table
 ***************************************************************************************/
class CacheTable {
    public: 
        CacheTable();                               // Constructor
        ~CacheTable();                              // Destructor

        // User Functions
        void SetCache(int lines, int tag);
        void Print();

        // Getter Functions
        bool GetValidBit(int index);
        string GetBinaryTag(int index);
        int GetTimeClock(int index);

        // Setter Functions
        void SetDirtyBit(int index, bool newBit);
        void SetValidBit(int index, bool newBit);
        void SetBinaryTag(int index, int address, int totalAddressSize);
        void SetData(int index, int mmBlock);
        void SetTimeClock(int index);

    private:
        // Variables Populated by User Settings/Input
        int cacheLines;
        int tagSize;

        // Variables for Cache Memory Rows
        vector<bool> dirtyBit;
        vector<bool> validBit;
        vector<string> binaryTag;
        vector<string> data;

        // Time Variables
        int virtualClock;
        vector<int> timeClock;
};

/****************************************************************************************
 * Constructor
 ***************************************************************************************/
CacheTable::CacheTable() {
    // Initialize virtualClock to 0
    virtualClock = 0;
}

/****************************************************************************************
 * Destructor
 ***************************************************************************************/
CacheTable::~CacheTable() {
    // Clear Vectors to free up memory space for next simulation
    dirtyBit.clear();
    validBit.clear();
    binaryTag.clear();
    data.clear();
    timeClock.clear();
}

/****************************************************************************************
 * Function Name:       SetCache
 * Input Parameters:    int lines - represents number of cache lines are available
 *                      int tag - represents number of tag bits
 * Return Value:        void
 * Purpose:             Initializes the sizes of all vecctors/rows of the table. Sets
 *                      all dirty bits and valid bits to 0. Sets data bits to "xxx" 
 *                      and tag bits to correct size of 'don't-cares'. Also sets the
 *                      vector size of time clock and sets all values to 0.
 *                      TLDR: Creates default cache table
 ***************************************************************************************/
void CacheTable::SetCache(int lines, int tag) {
    // Set cacheLines and tagSize integers
    cacheLines = lines;
    tagSize = tag;

    // Create temp Variable for binary tag
    string tempTag = "";

    for(int i=0; i<tagSize; i++) {
        tempTag += "x";
    }

    // Set size of dirty and valid bits
    // Initialize all to 0
    dirtyBit.resize(cacheLines, 0);
    validBit.resize(cacheLines, 0);

    // Initialize binary tag bits
    binaryTag.resize(cacheLines, tempTag);

    // Initialize data bits
    data.resize(cacheLines, "xxx");

    // Initialize timeClock to 0
    timeClock.resize(cacheLines, 0);
}

/****************************************************************************************
 * Function Name:       Print
 * Input Parameters:    void
 * Return Value:        void
 * Purpose:             Prints the cache table as it would appear in cache memory. 
 *                      Prints cache block numbers, dirty bits, valid bits, tag bits,
 *                      and data of each cache line address.
 ***************************************************************************************/
void CacheTable::Print() {
    // Print Title
    cout << endl << "Final \"status\" of the cache:" << endl;

    // Print Column Headers
    cout << "cache blk #" << "\t\t" << "dirty bit" << "\t" << "valid bit";
    cout << "\t" << "tag" << "\t\t" << "data" << endl;
    cout << "----------------------------------------------------------------------";
    cout << "--------------" << endl;

    // Print Rows
    for(int i=0; i < cacheLines; i++) {
        // Print Cache Block Number
        cout << "\t" << i;

        // Print Dirty Bit
        cout << "\t\t" << dirtyBit.at(i);

        // Print Valid Bit
        cout << "\t\t" << validBit.at(i);

        // Print Binary Tag
        cout << "\t\t" << binaryTag.at(i);

        // Print Data
        cout << "\t\t" << data.at(i); 
        cout << endl;
    }
}

/****************************************************************************************
 * Function Name:       GetValidBit
 * Input Parameters:    int - represents the cache row index to access
 * Return Value:        bool - represents the status of the valid bit
 *                              (1 = Valid, 0 = Invalid)
 * Purpose:             Returns the valid bit of the the given indexed cache row
 ***************************************************************************************/
bool CacheTable::GetValidBit(int index) {
    // Return the valid bit at given index
    return validBit.at(index);
}

/****************************************************************************************
 * Function Name:       GetBinaryTag
 * Input Parameters:    int - represents the cache row index to access
 * Return Value:        string - represents the binary tag at the given index
 * Purpose:             Returns the binary tag from the given index as a string of 1's 
 *                      and 0's.
 ***************************************************************************************/
string CacheTable::GetBinaryTag(int index) {
    // Return the binaryTag at given Index
    return binaryTag.at(index);
}

/****************************************************************************************
 * Function Name:       GetTimeClock
 * Input Parameters:    int - represents the cache row index to access
 * Return Value:        int - represents the number of operations to occur as time 
 *                      passes. (Using this count of operations as an internal clock to
 *                      keep track of items that were most recently used as well as 
 *                      which items were entered first)
 * Purpose:             Returns the "virtual" clock integer indicating how many total 
 *                      read and write operations were performed at the time of when 
 *                      this item was set.
 ***************************************************************************************/
int CacheTable::GetTimeClock(int index) {
    // Return timeClock at given index
    return timeClock.at(index);
}

/****************************************************************************************
 * Function Name:       SetDirtyBit
 * Input Parameters:    int index - represents the cache row index to change
 *                      bool newBit - represents the value that dirty bit will be set to
 * Return Value:        void
 * Purpose:             Sets the dirty bit value to newBit value at the given index
 ***************************************************************************************/
void CacheTable::SetDirtyBit(int index, bool newBit) {
    // Set the dirtyBit at index equal to the bool newBit
    dirtyBit.at(index) = newBit;
}

/****************************************************************************************
 * Function Name:       SetValidBit
 * Input Parameters:    int index - represents the cache row index to change
 *                      int newBit - represents the value that valid bit will be set to
 * Return Value:        void
 * Purpose:             Sets the valid bit value to newBit value at the given index
 ***************************************************************************************/
void CacheTable::SetValidBit(int index, bool newBit) {
    // Set the validBit at index equal to the bool newBit
    validBit.at(index) = newBit;
}

/****************************************************************************************
 * Function Name:       SetBinaryTag
 * Input Parameters:    int index - represents the cache row index to change
 *                      int address - represents the main memory address as a decimal
 *                      number
 *                      int totalAddressSize - represents the total number of bits for
 *                      the main memory address
 * Return Value:        void
 * Purpose:             Uses a given main memory address and total address bit size to 
 *                      calculate and convert the tag into a string of 1's and 0's
 ***************************************************************************************/
void CacheTable::SetBinaryTag(int index, int address, int totalAddressSize) {
    // Declare temporary String variable
    string temp = "";
    
    // Get Tag from Address
    for(int i=0; i < (totalAddressSize - tagSize); i++) {
        // Shift Right
        address = address / 2;
    }

    // Convert Tag to Binary
    for(int i=0; i < tagSize; i++) {
        // Find LSB
        temp = to_string(address % 2) + temp;

        // Shift Right 
        address = address / 2;
    }

    // Set binaryTag to temp
    binaryTag.at(index) = temp;
}

/****************************************************************************************
 * Function Name:       SetData
 * Input Parameters:    int index - represents the cache row index to change
 *                      int mmBlock - represents the main memory block to save to the 
 *                      data field
 * Return Value:        void
 * Purpose:             Stores the given main memory block in the data field at the
 *                      given cache row index. Data is stored as a string.
 ***************************************************************************************/
void CacheTable::SetData(int index, int mmBlock) {
    // Declare variable for temporary string
    string temp = "mm blk # ";

    // Append mmBlock number to temp string
    temp += to_string(mmBlock);

    // Set data at index equal to temp string
    data.at(index) = temp;
}

/****************************************************************************************
 * Function Name:       SetTimeClock
 * Input Parameters:    int - represents the cache row index to change
 * Return Value:        void
 * Purpose:             Increments the "virtual clock" by one and also sets the 
 *                      timeClock element at a given index to the "virtual clock"
 *                      value. The "Virtual clock" value indicates how many total
 *                      read and write operations have occurred at this given time.
 *                      This function is only called when a read or write occurs
 *                      and is necessary in order to implement LRU and FIFO
 *                      replacement policies.
 ***************************************************************************************/
void CacheTable::SetTimeClock(int index) {
    // Increment Virtual Clock
    virtualClock++;

    // Set time Clock at index to equal virtual clock
    timeClock.at(index) = virtualClock;
}