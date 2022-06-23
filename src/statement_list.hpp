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
// Libraries not in standard
#include <boost/date_time/gregorian/gregorian.hpp>
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
  
    
  Statement(const Statement& sl) = delete;
  Statement(Statement&& sl) = delete;
  Statement& operator=(Statement&& other) = delete;

  int getAmount() const { return amount; }
  int getBalance() const { return balance; }
  std::string getDate() const { return date; } 


}; 


class StatementList {
  std::map<boost::gregorian::date, Statement> list;    
public: 
  /*
   * shallow copy 
   * c variable naming style to indicate type of data dealing with 
   * undefined behavior if number entries invalid
   * DOES NOT MODIFY Struct Entry 
   */ 
  StatementList(struct Entry **pentries, size_t number_entries); 
  StatementList(const std::vector<Statement>& statements);
  
  StatementList() = delete; 
  StatementList(const StatementList& sl) = delete;
  StatementList(StatementList&& sl) = delete; 
  StatementList& operator=(const StatementList& other) = delete; 
  StatementList& operator=(StatementList&& other) = delete; 
  
  void addStatement(const Statement& statement); 

  const std::optional<int> getAmount(boost::gregorian::date date) const;
  const std::optional<int> getBalance(boost::gregorian::date date) const; 

  int getMaxDeposit() const;
  int getMaxWithdrawal() const;
  int getMaxBalance() const;
  int getMinBalance() const;

  std::string getMaxDepositDate() const;
  std::string getMaxWithdrawalDate() const;
  std::string getMaxBalanceDate() const;
  std::string getMinBalanceDate() const;
  
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
