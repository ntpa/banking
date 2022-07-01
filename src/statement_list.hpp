#ifndef STATEMENT_LIST_HPP
#define STATEMENT_LIST_HPP

#include <string> 
#include <map>
#include <vector> 
#include <optional>
#include <iostream> 
#include <functional>
#include <limits>
#include <regex>
#include <fstream> 
#include <cstdlib>
// Libraries not in standard
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/special_defs.hpp>
#include "entry.h"

class Statement {
  int amount{};
  int balance{};  
  std::string date; 
public: 
  Statement(int amount, int balance, const std::string& date)
  : amount{ amount }, balance { balance }
  , date{ date } {}
  ~Statement() = default; 
  Statement() = default;
  Statement& operator=(const Statement& other) = default; 
  Statement(const Statement& sl) = default;
  Statement(Statement&& sl) = delete;
  Statement& operator=(Statement&& other) = delete;

  int getAmount() const { return amount; }
  int getBalance() const { return balance; }
  std::string getDate() const { return date; } 
}; 


class StatementList {
  std::map<boost::gregorian::date, Statement> list;    
  boost::gregorian::date start_date{boost::date_time::special_values::max_date_time};
  boost::gregorian::date end_date{boost::date_time::special_values::min_date_time}; 
public: 
  /*
   * shallow copy 
   * c variable naming style to indicate type of data dealing with 
   * undefined behavior if number entries invalid
   * DOES NOT MODIFY Struct Entry 
   */ 
  explicit StatementList(struct Entry **pentries, size_t number_entries); 
  explicit StatementList(const std::vector<Statement>& statements); 
  StatementList() = default; 
  ~StatementList() = default; 
  StatementList(const StatementList& sl) = default;
  StatementList(StatementList&& sl) = delete; 
  StatementList& operator=(const StatementList& other) = delete; 
  StatementList& operator=(StatementList&& other) = delete; 
  
  void addStatement(const Statement& statement); 
  /*
   * returns lower bound to given date
   * if given date is before statementlist range then return the 1st recorded statement
   * if given date is after statementlist range then return the last recorded statement
   */ 
  Statement& operator[](boost::gregorian::date date);
  friend std::ostream& operator<<(std::ostream& os, const StatementList& sl); 

  std::optional<int> getAmount(boost::gregorian::date date) const;
  std::optional<int> getBalance(boost::gregorian::date date) const; 

  int getMaxDeposit() const;
  int getMaxWithdrawal() const;
  int getMaxBalance() const;
  int getMinBalance() const;
  /* 
   * StatementList merges all statements on the same date,
   * so the methods below will not sum to the total number of 
   * statements in the input .csv file
   */  
  int getNumStatements() const; 
  int getNumDeposits() const; 
  int getNumWithdrawals() const; 

  boost::gregorian::date getMaxDepositDate() const;
  boost::gregorian::date getMaxWithdrawalDate() const;
  boost::gregorian::date getMaxBalanceDate() const;
  boost::gregorian::date getMinBalanceDate() const;
  boost::gregorian::date getStartDate() const;
  boost::gregorian::date getEndDate() const; 
 
  // Ranges are all inclusive from (begin-end)
  StatementList getDateRange(boost::gregorian::date start, boost::gregorian::date end) const; 
  StatementList getAmountRange(int min, int max) const; 
  StatementList getBalanceRange(int min, int max) const; 

  void printList() const;
}; 

#endif /* STATEMENT_LIST_HPP */
