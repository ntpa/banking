#include <fstream>
#include <iostream>
#include <cstring> 

#include "statement_list.hpp"
#include "entry.h"
#include <stdio.h>

// Author: Nathan Tadesse
// Date: 06-19-22
// Filename: main.cpp




/*
 * main() - takes in space seperated file which represents Capital One Bank Statements from certain time period
 *          and creates a .dat file for gnuplot usage. 
 *
 * @argc: number of command line arguments
 * @argv: the strings representing command line arguments. Expects a filename that points to a
 *        csv file of the representation in description, and an output file specifier for the .dat file
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


/*
 * Macro below represents the maximum line length of the input file.
 * On *nix systems the maximum line length of a file can be found by
 * running 'wc -L inputfile'. Replace that number+1(include space for null termination char)
 * with the MACRO value below.
 *
 */
#define CSV_STATEMENT_LENGTH 130
#define TEMP_STATEMENT_LENGTH 18 

int main(int argc, char *argv[]) {

  if (argc != 3) {
    std::cout << "Usage: <executable> <input-file> <output-file>\n"; 
    return -1; 
  }

  /**** START OF C Function Calls ****/ 

  FILE *p_inputfile = fopen(argv[1], "r");  
  const char *temp_filename = "temp.out"; 
  FILE *p_tmpfile = fopen(temp_filename, "w"); 

  if (p_inputfile == NULL || p_tmpfile == NULL) {
    perror("Failed to open .csv file"); 
    return -1; 
  }

  char csv_statement[CSV_STATEMENT_LENGTH];
  size_t number_entries = 0;
  while(fgets(csv_statement, CSV_STATEMENT_LENGTH, p_inputfile)) {
    number_entries++;
  }
  fseek(p_inputfile, 0, SEEK_SET);

  struct Entry *pentries[number_entries];
  for (size_t j = 0; fgets(csv_statement, CSV_STATEMENT_LENGTH, p_inputfile); j++) {
    // dynamic memory returned
    pentries[j] = create_entry(csv_statement);
    if (pentries[j]) {
      fprintf(p_tmpfile, "%s %d %d\n", get_date(pentries[j]), get_amount(pentries[j]), get_balance(pentries[j]));
    }
    else {
      perror("Failed to parse csv_statement.\n");
    }
  } /* Get next statement */

  /* No more operations on entries past here */
  free_entries(number_entries, pentries);
  /* End of file operations */

  fclose(p_inputfile);
  fclose(p_tmpfile); 
  /**** END OF C Function Calls ****/ 

  std::ifstream ifs(temp_filename, std::ifstream::in); 
  std::ofstream ofs(argv[2], std::ofstream::out); 
  if (!ifs.is_open() || !ofs.is_open())  {
    std::cout << "Failed to open files. Please check directory permissions\n"; 
    return -1; 
  }

  StatementList sl(TEMP_STATEMENT_LENGTH, std::regex{R"((\S{8}) (-?\d+) (-?\d+))"} );
  char temp_statement[TEMP_STATEMENT_LENGTH]; 
  while(ifs.getline(temp_statement, TEMP_STATEMENT_LENGTH)) { /* For every statement */ 
    sl.addStatement(temp_statement); 
 
  } /* Finished reading file */ 
  
  ofs << sl; 
  std::cout << "Max deposit: " << sl.getMaxDeposit() << " happened on " << sl.getMaxDepositDate() << '\n';
  std::cout << "Max withdrawal: " << sl.getMaxWithdrawal() << " happened on " << sl.getMaxWithdrawalDate() << '\n';
  std::cout << "Max balance: " << sl.getMaxBalance() << " happened on " << sl.getMaxBalanceDate() << '\n';
  std::cout << "Min balance: " << sl.getMinBalance() << " happened on " << sl.getMinBalanceDate() << '\n';
}
