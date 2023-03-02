// Outputs the boards created and processed from CreateLife.cpp & ProcessLife.cpp

#include "OutputLife.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h> // for usleep() when pausing in between generations

OutputLife::OutputLife() {
    
    char inputType;
    char outputType;
    std::string boundaryMode = "";
    bool pause = false;

    PromptUser();

}

OutputLife::~OutputLife() {}

void OutputLife::PromptUser() {

    std::cout << "Welcome to the Game of Life!\n";
    std::cout << "Do you want generate a random array (R) or input your own map (M)? " << std::endl;
    std::cin >> inputType;

    // Sanitizes inputs for 'R' or 'M' char values only
    while (!(inputType == 'R' || inputType == 'M')) {
        std::cout << "Incorrect input! Choose either random array (R) or your own map text file (M): " << std::endl;
        std::cin >> inputType;
    }

    if (inputType == 'R') { PromptBoardDimensions(); } // Gets board properties from user
    else { ReadFromFile(); }                           // Gets board properties from user's .txt file
        
    PromptBoundaryMode();                              // Prompt's which boundary rules to use

    if (inputType == 'R') {
        // Creates a ProcessLife object off of the user's inputs
        ProcessLife gameOfLife(boundaryMode, this->userRow, this->userColumn, this->userPopulationDensity);

        PromptOutputChoice();
        if (outputType == 'P' || outputType == 'E') { OutputToConsole(gameOfLife);} 
        else { OutputToTextFile(gameOfLife); }

    } else {
        // Creates a grid off of the user's .txt file
        ProcessLife gameOfLife(boundaryMode, this->userTextFile);

        PromptOutputChoice();
        if (outputType == 'P' || outputType == 'E') { OutputToConsole(gameOfLife);} 
        else { OutputToTextFile(gameOfLife); }
    }


}

// Prompt user for boundary rules
void OutputLife::PromptBoundaryMode() {

    std::cout << "Which game mode would you like?\n" 
                << "(C) Classic | (M) Mirror | (D) Doughnut ";
    std::cin >> boundaryMode;

    while (!(boundaryMode == "C" || boundaryMode == "M" || boundaryMode == "D")) {
        std::cout << "Incorrect input!\n" 
                    << "(C) Classic | (M) Mirror | (D) Doughnut ";
        std::cin >> boundaryMode;
    }

    if (boundaryMode == "C") { boundaryMode = "classic"; }
    else if (boundaryMode == "M") { boundaryMode = "mirror"; }
    else if (boundaryMode == "D") { boundaryMode = "doughnut"; }
    
}

// Prompt user for output choice
void OutputLife::PromptOutputChoice() {

    std::cout << "How do you want to output each generation?\n"
              << "(P) Pause In-Between  | (E) Press ENTER Key | (T) Output to a .txt file" << std::endl;        
    std::cin >> outputType;

    while (!(outputType == 'P' || outputType == 'E' || outputType == 'T')) {
        std::cout << "Incorrect input!\n"
                  << "(P) Pause In-Between  | (E) Press ENTER Key | (T) Output to a .txt file" << std::endl;
        std::cin >> outputType;
    }

    if (outputType == 'P') { pause = true; }

}

// Prompt user's board dimensions
void OutputLife::PromptBoardDimensions() {

    std::cout << "Enter # of rows: ";
    std::cin >> this->userRow;

    std::cout << "Enter # of columns: ";
    std::cin >> this->userColumn;

    std::cout << "Enter the population density (between 0 and 1): ";
    std::cin >> this->userPopulationDensity;

    // Checks if userPopulationDensity is out of bounds
    while (this->userPopulationDensity < 0.0 || this->userPopulationDensity > 1.0) {
        std::cout << "Value is ";
        if (this->userPopulationDensity < 0.0) { std::cout << "negative. "; } 
        else { std::cout << "too big. "; }
        std::cout << "Please enter a value between 0 and 1" << std::endl;
        std::cin >> this->userPopulationDensity;
    }
}

// Prompts user's text file
void OutputLife::ReadFromFile() {
    std::cout << "Enter your text file: ";
    std::cin >> this->userTextFile;
}

void OutputLife::OutputToConsole(ProcessLife &gameOfLife) {

    int generationNum = 0;
    int second = 1000000; // for delay function, 1,000,000 ms is 1 second
    std::string enterKey = " ";

    // Output the initial board, generation 0
    std::cout << "Generation: " << generationNum << std::endl;
    gameOfLife.OutputCurrBoard(std::cout);
    getline(std::cin, enterKey); // used to skip over '\n' that was left in the buffer from std::cin

    // Keep processing the board until its stable or leave it looping infinitely
    while (true) {
        
        // Either pauses between generation or user presses ENTER key
        if (!pause) {

            std::cout << "Press ENTER for next generation ";
            getline(std::cin, enterKey);

            while(enterKey != "") {
                std::cout << "Invalid Key! Press ENTER" << std::endl;
                getline(std::cin, enterKey);
            }

        } else {
            usleep(1*second); // Sleep function
        }

        gameOfLife.ProcessBoard();
        generationNum++;

        // Outputs board in its current generation
        std::cout << "Generation: " << generationNum << std::endl;
        gameOfLife.OutputCurrBoard(std::cout);

        // If curr and prev board is equal or empty, allows user to exit
        if (gameOfLife.IsEqual() || gameOfLife.IsEmpty()) {

            std::cout << "The board is ";
            if (gameOfLife.IsEqual()) {
                std::cout << "stable, the previous and current board are the same." << std::endl;
            } else {
                std::cout << "empty." << std::endl;
            }

            std::cout << "Press ENTER to exit ";
            getline(std::cin, enterKey);

            while(enterKey != "") {
                std::cout << "Invalid Key! Press ENTER" << std::endl;
                getline(std::cin, enterKey);
            }
            
            break;
        }

    }

}

void OutputLife::OutputToTextFile(ProcessLife &gameOfLife) {
    
    std::string outputFileName = "GameOfLife.txt";
    std::ofstream outFS;

    // Attempts to open output file
    outFS.open(outputFileName);

    if (!outFS.is_open()) {
        std::cout << "Cannot create: " << outputFileName << std::endl;
        throw std::runtime_error("Error creating file");
    }

    // Output the initial board, generation 0
    int generationNum = 0;
    outFS << "Generation: " << generationNum << std::endl;
    gameOfLife.OutputCurrBoard(outFS);

    // Keep processing the board until its stable or leave it looping infinitely
    while (true) {
        gameOfLife.ProcessBoard();
        generationNum++;
        
        // Outputs each generation and its counter
        outFS << "Generation: " << generationNum << std::endl;
        gameOfLife.OutputCurrBoard(outFS);

        // Checks if the curr and prev board is equal or empty. Then allows user to exit if true
        if (gameOfLife.IsEqual() || gameOfLife.IsEmpty()) {
            outFS << "The board is ";
            if (gameOfLife.IsEqual()) {
                outFS << "stable, the previous and current board are the same.\n";
            } else {
                outFS << "empty.\n";
            }
            break;
        }
    }

    outFS.close();
}

/*int main() {
    OutputLife playGame;

    return 0;
}*/
