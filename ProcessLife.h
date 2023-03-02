#ifndef PROCESSLIFE_H
#define PROCESSLIFE_H

#include <iostream>
#include <ostream>
#include <string>
#include "CreateLife.h"

class ProcessLife {
   public:
      ProcessLife(std::string boundaryRule, int rows, int columns, float populationDensity);
      ProcessLife(std::string boundaryRule, std::string inputTxtFile);
      ~ProcessLife();        

      void ProcessBoard();
      bool IsEmpty();
      bool IsEqual();

      char** GetCurrentBoard() { return this->currentBoard; }
      char** GetPrevBoard() { return this->prevBoard; }
      void OutputCurrBoard(std::ostream& stream);

   private:

      int CountNumOfNeighbors(char** &board, int row, int column);
      void OutputBoard(std::ostream& stream, char** &board);
      void CopyArray(char** boardToCopy, char** board);
      void SetBoundaryRule(std::string boundaryRule);
      void ApplyBoundaryRules(char** &board);

      void CreateClassicBoard(char** &board);
      void CreateMirrorBoard(char** &board);
      void CreateDoughnutBoard(char** &board);
      
      char** currentBoard;
      char** prevBoard;
      char** tempBoard;

      int numOfNeighbors;
      int rows;
      int columns;
      int rowsForMarginBoard;
      int columnsForMarginBoard;

      enum BoundaryRule { CLASSIC, MIRROR, DOUGHNUT };
      BoundaryRule boundaryRule;

};


#endif