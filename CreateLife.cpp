// Class to create a 2-dimensional array for the Game of Life
// 2 ways to create the 2d array
// 1. User's input for rows, columns and population density
// 2. User's txt file with its own map and dimensions

#include <iostream>
#include <string>
#include <algorithm>    // for sort()
#include <cstdlib>      // for rand()
#include "CreateLife.h"

// Creates 2-dimensional array based off user input for rows, columns and population density
CreateLife::CreateLife(int rows, int columns, float populationDensity) {

    this->rows = rows;
    this->columns = columns;
    this->rowsForMargin = this->rows + 2;
    this->columnsForMargin = this->columns + 2;

    this->array = new char*[this->rowsForMargin];
    for (int i = 0; i < this->rowsForMargin; i++) {
        this->array[i] = new char[this->columnsForMargin];
    }

    CreateArray(populationDensity);

}

// Creates a 2-dimensional array from the user's input as a txt file
CreateLife::CreateLife(std::string inputTxtFile) {

    CreateFileArray(inputTxtFile);

}

// De-allocate memory from array[] and randomPositions[]
CreateLife::~CreateLife() {

    // Since 2-dimensional array is a double pointer, each row has to be de-allocated
    for (int i = 0; i < this->rows; i++) {
       delete [] this->array[i];
    }
    delete [] this->array;

}

void CreateLife::CreateArray(float populationDensity) {

    // Value from 0-1, Indicates how much life should be on the 2d array.
    // ex: 0.5 -> 50% of life, 0.67 -> 67% of life
    this->populationDensity = populationDensity; 

    this->allCellsAreAlive = false;
    this->allCellsAreDead = false;

    this->numElements = this->rows * this->columns;
    this->totalPopulation = numElements * this->populationDensity;
    int* randomPositions = new int[this->totalPopulation];

    // Cover special cases if all cells are alive/dead to prevent from generating randomPositions
    if (this->totalPopulation == 0) {
        this->allCellsAreDead = true;
    } else if (this->totalPopulation == this->numElements) {
        this->allCellsAreAlive = true;
    } else {
        CreateRandPositions(randomPositions);
    }

    // Keep track of the overall index pos to compare which live cells to place from the randomPosition[]
    int overallIndexPos = 0;
    int randArrayIndex = 0;

    // Creates an array which will hold char* type values 
    // Which will then point to other column arrays that will hold char values

    for (int i = 0; i < this->rowsForMargin; i++) {
        for (int j = 0; j < this->columnsForMargin; j++) {
            
            if ( (i == 0 || i == (this->rowsForMargin- 1)) || (j == 0 || j == (this->columnsForMargin - 1)) ) { // This allows us to manipulate the outer edges only 
                this->array[i][j] = '0';
            } else {

                if (allCellsAreAlive == true) {       // Special Case: All cells are alive
                    this->array[i][j] = 'X'; 
                } else if (allCellsAreDead == true) { // Special Case: All cells are dead
                    this->array[i][j] = '-';
                } else if (overallIndexPos == randomPositions[randArrayIndex]) { // Compares the overall index position to see if
                    this->array[i][j] = 'X';                                     // its part of the randomPositions[] to become a 
                    randArrayIndex++;                                            // live cell
                } else {
                    this->array[i][j] = '-';
                }

                overallIndexPos++; 
            }

        }
    } 
    delete [] randomPositions; 
}

// Create an array from a user's txt file
// Expected Format:
// rows
// columns
// ----
// --x-
// -xx-
void CreateLife::CreateFileArray(std::string &inputTxtFile) {
    
    this->inputTxtFile = inputTxtFile;
    this->inFS.open(this->inputTxtFile);

    // Attempt to open file
    if (!this->inFS.is_open()) {
        std::cout << "Cannot open: " << this->inputTxtFile << std::endl;
        throw std::runtime_error("Error opening file");
    }

    // Assigns row and column values
    this->inFS >> this->rows;
    this->inFS >> this->columns;
    this->rowsForMargin = this->rows + 2;
    this->columnsForMargin = this->columns + 2;

    // Create 2-dimensional array and tranfer over values from text file
    this->array = new char*[this->rowsForMargin];
    for (int i = 0; i < this->rowsForMargin; i++) {
        this->array[i] = new char[this->columnsForMargin];
        for (int j = 0; j < this->columnsForMargin; j++) {
            if ( (i == 0 || i == (this->rowsForMargin- 1)) || (j == 0 || j == (this->columnsForMargin - 1)) ) { // This allows us to manipulate the outer edges only 
                this->array[i][j] = '-';
            } else {
                inFS >> this->array[i][j];
            }
        }
    }

    this->inFS.close();
}

// Create an array with random index positions which match the population density of
void CreateLife::CreateRandPositions(int* &randomPositions) {

    srand(time(0)); // Generates unique seed for everytime rand() is called 

    int availablePositions [this->numElements];
    int numElementsCopy = this->numElements; // Creates copy to not manipulate origin value
    int randIndexPosition = 0;

    // Create all possible index positions in an array of numElements size
    // ex: [0, 1, 2, ... n]
    for (int i = 0; i < this->numElements; i++) {
        availablePositions[i] = i;
    }

    // Create list of random index positions
    for (int i = 0; i < this->totalPopulation; i++) {
    
        randIndexPosition = rand() % numElementsCopy;

        randomPositions[i] = availablePositions[randIndexPosition];

        // Removes the element which was used for randomPositions[] from the availablePositions[]
        // Allows to have a unique set of numbers
        for (int j = randIndexPosition; j < numElementsCopy; j++) {
            availablePositions[j] = availablePositions[j+1];
        }

        // Reduce the number of elements to choose from for each iteration as we reach the population density
        numElementsCopy--;
    }

    // Sorts the list of random index position to use later when creating the array
    std::sort(randomPositions, randomPositions + this->totalPopulation);
}

void CreateLife::PrintArray() {

    std::cout << std::endl;

    for (int i = 0; i < this->rowsForMargin; i++) {
        for (int j = 0; j < this->columnsForMargin; j++) {
            std::cout << this->array[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

// For debuggin class
/*int main() {

    CreateLife foo(6, 4, 0.2);
    //std::cout << "Creating file array" << std::endl;
    //CreateLife foo("input.txt");
    foo.PrintArray();
    char** board1 = foo.GetArray();

    for (int i = 0; i < foo.GetNumOfRows() + 2; i++) {
       for (int j = 0; j < foo.GetNumOfColumns() + 2; j++) {
           std::cout << board1[i][j] << " ";
       }
       std::cout << std::endl;
    }


    // Test to see if I can overwrite this->array and it only needs to de-allocate it once
    std::cout << "Creating empty array and assigning it to board2" << std::endl;
    foo.CreateArray(0);
    foo.PrintArray();
    char** board2 = foo.GetArray();    

    for (int i = 0; i < foo.GetNumOfRows() + 2; i++) {
       for (int j = 0; j < foo.GetNumOfColumns() + 2; j++) {
           std::cout << board2[i][j] << " ";
       }
       std::cout << std::endl;
    }
    
    std::cout << "Printing Board 1 again" << std::endl;

    for (int i = 0; i < foo.GetNumOfRows() + 2; i++) {
       for (int j = 0; j < foo.GetNumOfColumns() + 2; j++) {
           std::cout << board1[i][j] << " ";
       }
       std::cout << std::endl;
    }

    //std::cout << "# of Rows: " << foo.rows << std::endl;    
    //std::cout << "# of Columns: " << foo.columns << std::endl;    

    //std::cout << "Printing array: " << std::endl;


    

    return 0;
}*/