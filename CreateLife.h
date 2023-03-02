#ifndef CREATELIFE_H
#define CREATELIFE_H

#include <string>
#include <fstream>

class CreateLife {
    public:
        CreateLife(int rows, int columns, float populationDensity);
        CreateLife(std::string inputTxtFile);
        ~CreateLife();       

        char** GetArray() { return this->array; }
        void CreateArray(float populationDensity);
        int GetNumOfRows() { return this->rows; }
        int GetNumOfColumns() { return this->columns; }
        void PrintArray();

    private:
        char** array;

        std::string inputTxtFile;
        std::ifstream inFS;

        int rows;
        int columns;
        int rowsForMargin;
        int columnsForMargin;
        int numElements;
        int totalPopulation;
        float populationDensity;

        bool allCellsAreAlive;
        bool allCellsAreDead;

        void CreateFileArray(std::string &inputTxtFile);
        void CreateRandPositions(int* &randomPositions);

};

#endif