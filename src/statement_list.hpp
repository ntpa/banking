#ifndef STATEMENT_LIST_HPP
#define STATEMENT_LIST_HPP

#include <string> 
#include <map>
#include <optional>
#include <utility>
#include <iostream> 
#include <functional>
#include <limits>
#include <regex>
#include <fstream> 
// Libraries not in standard
#include <boost/date_time/gregorian/gregorian.hpp>
#include "entry.h"

class StatementList {


  std::map<boost::gregorian::date, std::pair<int, int>> statements;    
  const unsigned statementLength{}; 
  const std::regex regex{}; 

  void addStatement(boost::gregorian::date date, int amount, int balance) {
    auto it = statements.find(date);
    if (it != statements.end()) {
        // int remains unchanged because first read balance is the sum
        // of previous statements at respective date

        (it->second).first += amount;
    }
    else {
      statements.insert({date,{amount, balance}});
    }
  }

public: 
  /*
   * shallow copy 
   * c variable naming style to indicate type of data dealing with 
   * undefined behavior if number entries invalid
   * DOES NOT MODIFY Struct Entry 
   */ 
  StatementList(struct Entry **pentries, size_t number_entries); 
  explicit StatementList(const unsigned statementLength, const std::regex regex)
  : statementLength{ statementLength }
  , regex{ regex } {}
  ~StatementList() = default; 

  StatementList() = delete; 
  StatementList(const StatementList& sl) = delete;
  StatementList(StatementList&& sl) = delete; 
  StatementList& operator=(const StatementList& other) = delete; 
  StatementList& operator=(StatementList&& other) = delete; 

  /* 
   * Bug: For some reason I cannot seprate function declaration
   *      and definition for addStatement()
   *
   */
  void addStatement(char *statement) {
      const std::string statement_string{statement};
      std::smatch results;
      const auto matched = std::regex_match(statement_string, results, regex);
      if (matched) {
        // change results[1] --> MM/DD/YY to Boost Gregorian Date: YYYY/MM/DD
        std::string date = results[1]; 
        int amount = std::stoi(results[2]); 
        int balance = std::stoi(results[3]);

        std::string undelimited_date = "20"; // assume no date is prior to year 2000
        undelimited_date.push_back(date[6]); undelimited_date.push_back(date[7]); // year
        undelimited_date.push_back(date[0]); undelimited_date.push_back(date[1]); // month
        undelimited_date.push_back(date[3]); undelimited_date.push_back(date[4]); // day

        auto boost_date = boost::gregorian::from_undelimited_string(undelimited_date); 

        this->addStatement(boost_date, amount, balance);
      }
      else {
        // TODO: Implement proper exception handling
        std::cout << "Failed to match statement below to regex\n";
        std::cout << statement_string << '\n';
      }
  }

  

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
    
    os << "#Statement Amount Balance\n"; 
    
    for (const auto& statement : sl.statements) {
      os << statement.first  << ' ' << std::get<0>(statement.second)
      << ' ' << std::get<1>(statement.second) << '\n';
      
    }
    return os;

  }
}; 



#endif /* STATEMENT_LIST_HPP */
