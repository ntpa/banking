#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "entry.h" 

/* 
 * main() - takes in CSV file which represents Capital One Bank Statements from certain time period
 *          and creates a .dat file for gnuplot usage
 *  
 * @argc: number of command line arguments
 * @argv: the strings representing command line arguments. Expects a filename that points to a 
 *        Capital One Bank Statement CSV file, and an output file specifier for the .dat file
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
  if (p_inputfile != NULL && p_outputfile != NULL) {

    char statement[STATEMENT_LENGTH];    
    
    size_t number_entries = 0; 
    while(fgets(statement, STATEMENT_LENGTH, p_inputfile)) {
      number_entries++;
    }
    
    /*
     *  The only fields I want for future programs.
     * Of course, easily can be done with an awk command
     * but I wanted more C experience 
     * 
     */ 
    int amounts[number_entries];
    int balances[number_entries];
    char *dates[number_entries]; 

    for (size_t i= 0; i < number_entries; i++) {
      amounts[i] = 0; 
      balances[i] = 0; 
    }

    size_t j = 0; 
    fseek(p_inputfile, 0, SEEK_SET);    

    while (fgets(statement, STATEMENT_LENGTH, p_inputfile)) { 
      struct Entry *pentry = create_entry(statement);
      if (pentry) {
       
         /* Process Entry */   
        amounts[j] = get_amount(pentry);
        balances[j] = get_balance(pentry); 
        dates[j] = get_date(pentry);
  
        // As described earlier, only care about Date, Amount, Balance
        fprintf(p_outputfile, "%s %d %d\n", dates[j], amounts[j], balances[j]); 
        
        free(pentry->ptransaction); 
        pentry->ptransaction = NULL;
        free(pentry); 
        pentry = NULL; 
    
        
        j++; 
      }
      else {
        perror("Failed to parse statement.\n");
      }
      
  
    } /* Get next statement */
    
    

    /* Unused statistics, may be valuable in future */     

    int max_amount = INT_MIN, min_amount = INT_MAX;
    int max_balance = INT_MIN, min_balance = INT_MAX;

    for (size_t i = 0; i < number_entries; i++) {
      const int amount = amounts[i];
      max_amount = amount >= max_amount ? amount : max_amount;
      min_amount = amount <= min_amount ? amount : min_amount;
      const int balance = balances[i];
      max_balance = balance >= max_balance ? balance : max_balance; 
      min_balance = balance <= min_balance ? balance : min_balance; 
    }

  } /* End of file operations */ 
   
  else {
    perror("Failed to open .csv/.dat file\n");
    return -1; 
  }
  
  

  fclose(p_inputfile); 
  fclose(p_outputfile); 
  return 0; 
}
