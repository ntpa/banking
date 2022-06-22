#include "entry.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#define DATE_LENGTH 9
#define TYPE_LENGTH 7
#define DESCRIPTION_LENGTH 80
struct Transaction {
  int amount;
  char date[DATE_LENGTH];
  char type[TYPE_LENGTH]; // "Debit" or "Credit"
  char description[DESCRIPTION_LENGTH];
};

/*
 * create_transaction() - creates a transaction from its basic data
 * @amount: 
 *          amount in dollars of the transaction. A negative amount denotes withdrawal
 *          and positive amount denotes deposit
 *
 * @date: the date of the transaction, in the form "MM/DD/YY"
 * @type: the type of transaction, either "Debit" or "Credit"
 * @description: the establishment involved in the transaction
 *
 * Description:
 *              Given an amount, date, type and description create a transaction. Assumes input
 *              arguments valid. The transaction is dynamically allocated by the callee. If successful, return the created object,
 *              otherwise return a NULL value indicating a failure in creation.
 *
 * Return: pointer to dynamically allocated Transaction
 */

static struct Transaction *create_transaction(int amount, const char *date, const char *type, const char* description) {
  struct Transaction *pt =  malloc(sizeof(*pt));
  if (pt) {
    pt->amount = amount;
    strncpy(pt->date, date, DATE_LENGTH);
    strncpy(pt->type, type, TYPE_LENGTH);
    strncpy(pt->description, description, DESCRIPTION_LENGTH); 
  }
  return pt; 
}


/*
 * validate_statement() - validate and create entry given statement.
 * @account_num: the last 4 digits of account number tied to entry
 * @balance: the dollar amount in account after transaction
 * @amount:
 *          Dollar amount of transaction. Negative number indicates withdrawal
 *          and positive number indicates deposit. 
 * @date: The Gregorian calendar date in format MM/DD/YY
 * @type: Type of transaction, either "Debit" or "Credit"
 * @descripton: Additional information regarding transaction
 *
 * Description:
 *            Dynamically allocate an Entry object given its fields. Assume input parameters
 *            have already been validated. If successful, return the created object,
 *            otherwise return NULL on failure.
 *
 * Return: pointer to dynamically allocated Entry
 */

static struct Entry *validate_statement(const char account_num[ACCOUNT_NUM_LENGTH], int balance
                            ,int amount, const char *date, const char *type
                            , const char *description) {
  
  struct Entry *pentry = malloc(sizeof(*pentry)); 
  if (pentry) {
    strncpy(pentry->account_num, account_num, ACCOUNT_NUM_LENGTH);
    pentry->balance = balance;  
    pentry->ptransaction = create_transaction(amount, date, type, description); 
    if (pentry->ptransaction) {
      return pentry; 
    }
    else {
      goto OUT_FREE_ENTRY;  
    }
  }
  else {
    return NULL; 
  }

  OUT_FREE_ENTRY:
    free(pentry);
    pentry = NULL; 
    return NULL;
}


struct Entry *create_entry(char *statement) {
  // safe to modify statement through strtok
  const char* account_num = strtok(statement, ",");
  const char* date = strtok(NULL, ",");
  int amount = atoi(strtok(NULL, ","));
  const char* type = strtok(NULL, ",");
  const char* description = strtok(NULL, ",");
  int balance = atoi(strtok(NULL, "\n"));
  
  if (account_num && date && type && description) {
    struct Entry *pentry = validate_statement(account_num, balance, amount, date, type, description);
    if (pentry) {
      return pentry;
    }
    else {
      printf("Failed to create entry for statement below:\n%s\n", statement); 
    }
  }
  else {
    printf("Invalid account_num, date, type, or description in statement below:\n%s\n", statement);
  }
  
  return NULL;
}

extern void free_entry(struct Entry *pentry) {
  free(pentry->ptransaction);
  pentry->ptransaction = NULL;
  free(pentry);
  pentry = NULL; 
}

extern void free_entries(size_t number_entries, struct Entry *pentries[number_entries]) {
  for (size_t i = 0; i < number_entries; i++) {
    if (pentries[i]) {
      free_entry(pentries[i]); 
    }
  }
  pentries = NULL; 

} 

extern int get_amount(const struct Entry *pentry) {
  return pentry->ptransaction->amount;
}

extern int get_balance(const struct Entry *pentry) {
  return pentry->balance;
}

extern char *get_date(const struct Entry *pentry) {
  return pentry->ptransaction->date; 
}

extern char *get_description(const struct Entry *pentry) {
  return pentry->ptransaction->description; 
}


void print_entry(const struct Entry *pentry) {
 printf("%s,%s,%d,%s,%s,%.d\n",
            pentry->account_num, pentry->ptransaction->date
            ,pentry->ptransaction->amount, pentry->ptransaction->type
            , pentry->ptransaction->description, pentry->balance);
}
