#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "entry.h" 

/* 
 * main() - takes in CSV file which represents Capital One Bank Statements from certain time period
 *          and creates a .out file with reduced fields 
 *  
 * @argc: number of command line arguments
 * @argv: the strings representing command line arguments. Expects a filename that points to a 
 *        Capital One Bank Statement CSV file, and an output file specifier for the new space delimted file 
 * 
 *  Description: 
 *          Given an input file that references a file containing a Capital One Bank Statement
 *          main() will traverse the file and determine statistics on it specific to its entries. 
 *          An entry will have the format below - 
 *          Account Number,Transaction Date,Transaction Amount,Transaction Type,Transaction Description,Balance
 *          
 *
 *                     
 * 
 *          TODO: 
 *                1) Determine the statistics/output desired.
 *                  - Min, Max values for amount, balance, date
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
#define STATEMENT_LENGTH 130


int main(int argc, char* argv[]) {
  
  if (argc != 3) {
    printf("Please provide a .csv file for input, and a file to output to.\n");
    return -1;
  } 
  FILE *p_inputfile = fopen(argv[1], "r");
  FILE *p_outputfile = fopen(argv[2], "w"); 
  if (p_inputfile == NULL || p_outputfile == NULL) {
    perror("Failed to open files\n");
    return -1; 
  }  


  char statement[STATEMENT_LENGTH];    
  size_t number_entries = 0; 
  while(fgets(statement, STATEMENT_LENGTH, p_inputfile)) {
    number_entries++;
  }
  fseek(p_inputfile, 0, SEEK_SET);
  
  struct Entry *pentries[number_entries]; 
  for (size_t j = 0; fgets(statement, STATEMENT_LENGTH, p_inputfile); j++) { 
    // dynamic memory returned 
    pentries[j] = create_entry(statement);
    if (pentries[j]) {
      fprintf(p_outputfile, "%s %d %d\n", get_date(pentries[j]), get_amount(pentries[j]), get_balance(pentries[j])); 
    }
    else {
      perror("Failed to parse statement.\n");
    }
  } /* Get next statement */

  /* No more operations on entries past here */
  free_entries(number_entries, pentries);
  /* End of file operations */ 
   
  fclose(p_inputfile); 
  fclose(p_outputfile); 
  return 0; 
}
