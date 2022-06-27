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
  Statement(int amount, int balance, std::string date)
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
  StatementList(struct Entry **pentries, size_t number_entries); 
  StatementList(const std::vector<Statement>& statements);
  
  StatementList() = default; 
  StatementList(const StatementList& sl) = default;

  StatementList(StatementList&& sl) = delete; 
  StatementList& operator=(const StatementList& other) = delete; 
  StatementList& operator=(StatementList&& other) = delete; 
  
  void addStatement(const Statement& statement); 

  
  // returns lower bound to given date
  // if given date is before statementlist range then return the 1st recorded statement
  // if given date is after statementlist range then return the last recorded statement
  Statement& operator[](boost::gregorian::date date);

  const std::optional<int> getAmount(boost::gregorian::date date) const;
  const std::optional<int> getBalance(boost::gregorian::date date) const; 

  int getMaxDeposit() const;
  boost::gregorian::date getMaxDepositDate() const;

  int getMaxWithdrawal() const;
  boost::gregorian::date getMaxWithdrawalDate() const;

  int getMaxBalance() const;
  boost::gregorian::date getMaxBalanceDate() const;

  int getMinBalance() const;
  boost::gregorian::date getMinBalanceDate() const;
 

  boost::gregorian::date getStartDate() const;
  boost::gregorian::date getEndDate() const; 
  
  int size() const { return list.size(); }
 
  // (start,end] is an inclusive range
  const StatementList getStatementRange(boost::gregorian::date start, boost::gregorian::date end) const; 

  void printList() const;
  friend std::ostream& operator<<(std::ostream& os, const StatementList& sl) {
    os << "#Date Amount Balance\n"; 
    
    for (const auto& it : sl.list) {
      os << it.first  << ' ' << (it.second).getAmount()
      << ' ' << (it.second).getBalance() << '\n';
      
    }
    return os;

  }
}; 



#endif /* STATEMENT_LIST_HPP */
