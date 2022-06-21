#include <fstream>
#include <iostream>
#include <cstring> 

#include "statement_list.hpp"

// Author: Nathan Tadesse
// Date: 06-19-22
// Filename: main.cpp




/*
 * main() - takes in space seperated file which represents Capital One Bank Statements from certain time period
 *          and creates a .dat file for gnuplot usage. 
 *
 * @argc: number of command line arguments
 * @argv: the strings representing command line arguments. Expects a filename that points to a
 *        space delimted file of the representation in description, and an output file specifier for the .dat file
 *
 *  Description:
 *          Given an input file that references a file containing a Capital One Bank Statement
 *          main() will traverse the file and combine all entries that happened on the same date.
 * 
 *          An entry will have the format below -
 *          Date Amount Balance
 *
 *
 *
 *          TODO:
 *                1) Determine the statistics/output desired.
 *                2) Implement proper library for monetary calculations
 *                  - Current implentation uses trivial 'int'
 *
 *  Return: 0 represents successful operation, negative number indicates error in execution
 *
 */


int main(int argc, char *argv[]) {

  if (argc != 3) {
    std::cout << "Usage: <executable> <input-file> <output-file>\n"; 
    return -1; 
  }

  std::ifstream ifs(argv[1], std::ifstream::in); 
  std::ofstream ofs(argv[2], std::ofstream::out); 
  if (!ifs.is_open() || !ofs.is_open())  {
    std::cout << "Failed to open input/output file. Please check directory permissions\n"; 
    return -1; 
  }

  const unsigned statementLength = 18; 
  StatementList sl(statementLength, std::regex{R"((\S{8}) (-?\d+) (-?\d+))"} );

  char statement[statementLength]; 
  while(ifs.getline(statement, statementLength)) { /* For every statement */ 
    sl.addStatement(statement); 
 
  } /* Finished reading file */ 
  
  

  ofs << sl; 
  
  std::cout << "Max deposit: " << sl.getMaxDeposit() << " happened on " << sl.getMaxDepositDate() << '\n';
  std::cout << "Max withdrawal: " << sl.getMaxWithdrawal() << " happened on " << sl.getMaxWithdrawalDate() << '\n';
  std::cout << "Max balance: " << sl.getMaxBalance() << " happened on " << sl.getMaxBalanceDate() << '\n';
  std::cout << "Min balance: " << sl.getMinBalance() << " happened on " << sl.getMinBalanceDate() << '\n';
}
