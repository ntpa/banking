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

struct StatementComp {
  /* 
   * goal is to sort by dates in increasing order(most recent date last).
   * Date is in following FIXED format: MM/DD/YY
   *                                    01234567
   * First sort by year then sort by month then sort by day
   *
   * Returns true if the first argument goes before the second argument 
   * in the strict weak ordering it defines, and false otherwise. 
   *
   */
  bool operator() (const std::string& lhs, const std::string& rhs) const {
    if (lhs.empty()) {
      return false;
    } 
    if (rhs.empty()) {
      return true; 
    }


    // sort by year
    if ( (lhs[6] == rhs[6]) && (lhs[7] == rhs[7])) {
      // if not clear after year - sort by month
      if ( (lhs[0] == rhs[0]) && (lhs[1] == rhs[1])) {
        // if not clear after month - sort by day
        if ( (lhs[3] == rhs[3]) && (lhs[4] == rhs[4])) {
          /* identical dates are being compared*/
          return false;  // must return false to indicate equivalence
        }
        else {
          // years and months same, but days differ
            return lhs[3] == rhs[3] ? lhs[4] < rhs[4] : lhs[3] < rhs[3]; 
        }
      }
      else {
        // years are same, but months differ
        return lhs[0] == rhs[0] ? lhs[1] < rhs[1] : lhs[0] < rhs[0];
      } 
    }

    // know that years are not identical 
    return lhs[6] == rhs[6] ? lhs[7] < rhs[7] : lhs[6] < rhs[6];
  }

}; 

class StatementList {
  using Date = std::string;
  using Amount = int;
  using Balance = int; 

  std::map<Date, std::pair<Amount, Balance>, StatementComp> statements;    
  const unsigned statementLength; 
  const std::regex regex; 

public: 
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
  void addStatement(std::string date, int amount, int balance) {
    auto it = statements.find(date);
    // is there a statement with the same date?
    if (it != statements.end()) {
        // Balance remains unchanged because first read balance is the sum
        // of previous statements at respective date

        (it->second).first += amount;
    }
    else {
      statements.insert({date,{amount, balance}});
    }
  }
  void addStatement(char *statement) {
      const std::string statement_string{statement};
      std::smatch results;
      const auto matched = std::regex_match(statement_string, results, regex);
      if (matched) {
        this->addStatement(results[1], std::stoi(results[2]), std::stoi(results[3]));
      }
      else {
        // TODO: Implement proper exception handling
        std::cout << "Failed to match regex\n";
      }
  }

  

  const std::optional<int> getAmount(std::string date) const;
  const std::optional<int> getBalance(std::string date) const; 

  int getMaxDeposit() const;
  int getMaxWithdrawal() const;
  int getMaxBalance() const;
  int getMinBalance() const;

  std::string getMaxDepositDate() const;
  std::string getMaxWithdrawalDate() const;
  std::string getMaxBalanceDate() const;
  std::string getMinBalanceDate() const;
  
  void printList() const;
}; 


#endif /* STATEMENT_LIST_HPP */
