#include "statement_list.hpp"
#include "entry.h"

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
    perror("Usage: <executable> <input-file> <output-file>\n"); 
    return -1; 
  }
  FILE *p_inputfile = fopen(argv[1], "r");  
  if (p_inputfile == NULL) {
    perror("Failed to open .csv file"); 
    return -1; 
  }

  char csv_statement[CSV_STATEMENT_LENGTH];
  struct Entry *pentry;
  StatementList statementList; 
  for (size_t j = 0; fgets(csv_statement, CSV_STATEMENT_LENGTH, p_inputfile); j++) {
    // dynamic memory returned
    pentry = create_entry(csv_statement);
    if (pentry) {
      const Statement statement(get_amount(pentry), get_balance(pentry), get_date(pentry)); 
      statementList.addStatement(statement); 
      free_entry(pentry); 
    }
    else {
      perror("Failed to parse statement"); 
    }
  }

  fclose(p_inputfile);
  /* End of input file operations */

  std::ofstream ofs(argv[2], std::ofstream::out); 
  if (!ofs.is_open())  {
    perror("Failed to open output file. Please check directory permissions\n");
    return -1; 
  }

  
  ofs << "======  " << statementList.getStartDate() << " - " << statementList.getEndDate() << " ======" << '\n';
  ofs << "Number of statements: " << statementList.getNumStatements() << '\n'; 
  ofs << "Number of deposits: " << statementList.getNumDeposits() << "  Number of withdrawals: " << statementList.getNumWithdrawals() << '\n'; 
  ofs << "Max deposit: " << statementList.getMaxDeposit() << " happened on " << statementList.getMaxDepositDate() << '\n';
  ofs << "Max withdrawal: " << statementList.getMaxWithdrawal() << " happened on " << statementList.getMaxWithdrawalDate() << '\n';
  ofs << "Max balance: " << statementList.getMaxBalance() << " happened on " << statementList.getMaxBalanceDate() << '\n';
  ofs << "Min balance: " << statementList.getMinBalance() << " happened on " << statementList.getMinBalanceDate() << '\n';
  ofs << "===================================================" << '\n';  
  ofs << statementList;
  ofs.close(); 
}
