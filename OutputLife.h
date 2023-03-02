#include "ProcessLife.h"
#include <iostream>
#include <string>

class OutputLife {
    public:
        OutputLife();
        ~OutputLife();

    private:
        void PromptUser();
        void PromptBoundaryMode();
        void PromptOutputChoice();
        void PromptBoardDimensions();

        void ReadFromFile();

        void OutputToTextFile(ProcessLife &gameOfLife);
        void OutputToConsole(ProcessLife &gameOfLife);

        int userRow;
        int userColumn;
        float userPopulationDensity;
        int second;
        bool pause;

        char inputType;
        char outputType;

        std::string boundaryMode;
        std::string userTextFile;

};