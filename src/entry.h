/* 
 * Author: Nathaniel Tadesse
 * Date: 06-13-2022
 * File: entry.h
 * Description: Top-level data structures and methods for 'Entry'. An 'Entry' contains: 
    - Account Number
    - Transaction 
    - Balance
 * Multiple bank statements make up the CSV file(past*days.csv). Each statement corresponds to an Entry

*/

#ifndef ENTRY_H
#define ENTRY_H

#define ACCOUNT_NUM_LENGTH 5

struct Transaction; 

struct Entry {
  char account_num[ACCOUNT_NUM_LENGTH]; // last 4 digits of account number(security)
  int balance; // balance after the transaction 
  struct Transaction *ptransaction; 

}; 

/*
 * create_entry() - create an entry.
 * @statment: A string pointing to a bank statement in the form
 *              "Account Number,Transaction Date,Transaction Amount,Transaction Type,Transaction Description,Balance"
 *            includes new-line character (as well as null terminator) at the end of string
 * Description:
 *            Dynamically allocate an Entry object given a bank statement . Assume input parameter
 *            valid. If successful, return the created object,
 *            otherwise return NULL value.
 *
 * Return: pointer to dynamically allocated Entry
 */

extern struct Entry *create_entry(char *statement);
/*
 * free_entry() - deallocate an entry
 * @pentry: A non-null entry 
 * 
 * Description: Free an entry. Assumes that entry is not null, as validated
 *              prior to function call
 * 
 * Return: Nothing
 * 
 */ 
extern void free_entry(struct Entry *pentry); 


// simple get functions
extern int get_amount(const struct Entry *pentry);
extern int get_balance(const struct Entry *pentry); 
extern char *get_date(const struct Entry *pentry); 
extern char *get_description(const struct Entry *pentry); 

/* 
 * print_entry() - print out an entry
 * @pentry: pointer to an entry record
 * 
 *
 * Description: Print out an entry to standard output with the form
 *              "Account Number,Transaction Date,Transaction Amount,Transaction Type,Transaction Description,Balance"
 *
 * Return: There is no return value 
 */
extern void print_entry(const struct Entry *pentry); 
#endif /* ENTRY_H */
