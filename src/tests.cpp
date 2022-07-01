#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "statement_list.hpp"

#define STATEMENT_LENGTH 130

using boost::date_time::special_values;
using boost::gregorian::date;  

TEST_CASE("Statement Constructor", "[Statement]") {
  Statement statement; 
  REQUIRE(statement.getAmount() == 0);
  REQUIRE(statement.getBalance() == 0); 
  REQUIRE(statement.getDate().empty() == true); 

}

TEST_CASE("StatementList Constructors", "[StatementList]") {
    const size_t number_entries = 3; 
    struct Entry *pentries[number_entries];
    char statement[STATEMENT_LENGTH] = "0546,09/20/2020,50,Debit,Fake transaction 1, 320";
    pentries[0] = create_entry(statement);
    char statement2[STATEMENT_LENGTH] = "0546,05/10/2020,-30,Debit,Fake transaction 1, 270";
    pentries[1] = create_entry(statement2);
    char statement3[STATEMENT_LENGTH] = "0546,05/10/2020,-100,Debit,Fake transaction 1, 300"; 
    pentries[2] = create_entry(statement3);

    /* For boost::gregorian::date */ 
    date maxDepositDate(boost::gregorian::from_string("2020-Sep-20")); 
    date maxWithdrawalDate(boost::gregorian::from_string("2020-May-10")); 
    date maxBalanceDate(boost::gregorian::from_string("2020-Sep-20")); 
    date minBalanceDate(boost::gregorian::from_string("2020-May-10")); 
  SECTION("Default constructor") {
    StatementList statementList; 
    REQUIRE(statementList.getNumStatements() == 0);
    REQUIRE(statementList.getMaxDeposit() == std::numeric_limits<int>::min()); 
    REQUIRE(statementList.getMaxWithdrawal() == std::numeric_limits<int>::max()); 
    REQUIRE(statementList.getMaxBalance() == std::numeric_limits<int>::min()); 
    REQUIRE(statementList.getMinBalance() == std::numeric_limits<int>::max()); 

    date d1(special_values::max_date_time); 
    date d2(special_values::min_date_time); 
    REQUIRE(statementList.getStartDate() == d1); 
    REQUIRE(statementList.getEndDate() == d2); 

  }
  SECTION("Construct from entries") {
    StatementList statementList((const struct Entry**)pentries, number_entries); 
    REQUIRE(statementList.getNumStatements() == number_entries-1); 
    REQUIRE(statementList.getMaxDeposit() == 50); 
    REQUIRE(statementList.getMaxWithdrawal() == -130); 
    REQUIRE(statementList.getMaxBalance() == 320); 
    REQUIRE(statementList.getMinBalance() == 270); 


    REQUIRE(statementList.getMaxDepositDate() == maxDepositDate);
    REQUIRE(statementList.getMaxWithdrawalDate() == maxWithdrawalDate); 
    REQUIRE(statementList.getMaxBalanceDate() == maxBalanceDate);
    REQUIRE(statementList.getMinBalanceDate() == minBalanceDate); 
  }

  SECTION("Construct from vector of Statements") {
    std::vector<Statement> statements; 
    Statement s(get_amount(pentries[0]), get_balance(pentries[0]), get_date(pentries[0])); 
    Statement s2(get_amount(pentries[1]), get_balance(pentries[1]), get_date(pentries[1]));
    Statement s3(get_amount(pentries[2]), get_balance(pentries[2]), get_date(pentries[2])); 
    
    statements.push_back(s); statements.push_back(s2); statements.push_back(s3); 
    StatementList statementList(statements);  
    
    REQUIRE(statementList.getNumStatements() == number_entries-1); // statements on same day are merged 
    REQUIRE(statementList.getMaxDeposit() == 50); 
    REQUIRE(statementList.getMaxWithdrawal() == -130); 
    REQUIRE(statementList.getMaxBalance() == 320); 
    REQUIRE(statementList.getMinBalance() == 270); 
    REQUIRE(statementList.getMaxDepositDate() == maxDepositDate);
    REQUIRE(statementList.getMaxWithdrawalDate() == maxWithdrawalDate); 
    REQUIRE(statementList.getMaxBalanceDate() == maxBalanceDate);
    REQUIRE(statementList.getMinBalanceDate() == minBalanceDate); 
  }

  free_entries(number_entries, pentries); 
}


TEST_CASE("Assignment and Copy operations", "[StatementList]") {
  const size_t number_entries = 3; 
  struct Entry *pentries[number_entries];
  char statement[STATEMENT_LENGTH] = "0546,09/20/2020,50,Debit,Fake transaction 1, 320";
  pentries[0] = create_entry(statement);
  char statement2[STATEMENT_LENGTH] = "0546,05/10/2020,-30,Debit,Fake transaction 1, 270";
  pentries[1] = create_entry(statement2);
  char statement3[STATEMENT_LENGTH] = "0546,05/10/2020,-100,Debit,Fake transaction 1, 300"; 
  pentries[2] = create_entry(statement3);

  std::vector<Statement> statements; 
  Statement s(get_amount(pentries[0]), get_balance(pentries[0]), get_date(pentries[0])); 
  Statement s2(get_amount(pentries[1]), get_balance(pentries[1]), get_date(pentries[1]));
  Statement s3(get_amount(pentries[2]), get_balance(pentries[2]), get_date(pentries[2])); 
  statements.push_back(s); statements.push_back(s2); statements.push_back(s3); 
  StatementList statementList(statements);  

  SECTION("Assignment operator") {
    StatementList statementList2 = statementList;
    REQUIRE(statementList2.getMaxDeposit() == statementList.getMaxDeposit()); 
    REQUIRE(statementList2.getMaxWithdrawal() == statementList.getMaxWithdrawal()); 
    REQUIRE(statementList2.getMaxBalance() == statementList.getMaxBalance());
    REQUIRE(statementList2.getMinBalance() == statementList.getMinBalance()); 
    
    REQUIRE(statementList2.getMaxDepositDate() == statementList.getMaxDepositDate());
    REQUIRE(statementList2.getMaxWithdrawalDate() == statementList.getMaxWithdrawalDate());
    REQUIRE(statementList2.getMaxBalanceDate() == statementList.getMaxBalanceDate());
    REQUIRE(statementList2.getMinBalanceDate() == statementList.getMinBalanceDate()); 
    REQUIRE(statementList2.getStartDate() == statementList.getStartDate());
    REQUIRE(statementList2.getEndDate() == statementList.getEndDate()); 
  }
  SECTION("Copy operation") {
    StatementList statementList2(statementList); 
    REQUIRE(statementList2.getMaxDeposit() == statementList.getMaxDeposit()); 
    REQUIRE(statementList2.getMaxWithdrawal() == statementList.getMaxWithdrawal()); 
    REQUIRE(statementList2.getMaxBalance() == statementList.getMaxBalance());
    REQUIRE(statementList2.getMinBalance() == statementList.getMinBalance()); 
    
    REQUIRE(statementList2.getMaxDepositDate() == statementList.getMaxDepositDate());
    REQUIRE(statementList2.getMaxWithdrawalDate() == statementList.getMaxWithdrawalDate());
    REQUIRE(statementList2.getMaxBalanceDate() == statementList.getMaxBalanceDate());
    REQUIRE(statementList2.getMinBalanceDate() == statementList.getMinBalanceDate()); 
    REQUIRE(statementList2.getStartDate() == statementList.getStartDate());
    REQUIRE(statementList2.getEndDate() == statementList.getEndDate()); 
  }

}

TEST_CASE("operator[]", "[StatementList]") {
  const size_t number_entries = 3; 
  struct Entry *pentries[number_entries];
  char statement[STATEMENT_LENGTH] = "0546,09/20/2020,50,Debit,Fake transaction 1, 320";
  pentries[0] = create_entry(statement);
  char statement2[STATEMENT_LENGTH] = "0546,05/10/2020,-30,Debit,Fake transaction 1, 270";
  pentries[1] = create_entry(statement2);
  char statement3[STATEMENT_LENGTH] = "0546,05/10/2020,-100,Debit,Fake transaction 1, 300"; 
  pentries[2] = create_entry(statement3);

  std::vector<Statement> statements; 
  Statement s(get_amount(pentries[0]), get_balance(pentries[0]), get_date(pentries[0])); 
  Statement s2(get_amount(pentries[1]), get_balance(pentries[1]), get_date(pentries[1]));
  Statement s3(get_amount(pentries[2]), get_balance(pentries[2]), get_date(pentries[2])); 
  statements.push_back(s); statements.push_back(s2); statements.push_back(s3); 
  StatementList statementList(statements);  

  
  date validDate1(boost::gregorian::from_string("2020-May-10")); 
  date validDate2(boost::gregorian::from_string("2020-Sep-20")); 
  REQUIRE(statementList[validDate1].getAmount() == -130);
  REQUIRE(statementList[validDate1].getBalance() == 270); 
  REQUIRE(statementList[validDate2].getAmount() == 50);
  REQUIRE(statementList[validDate2].getBalance() == 320); 

  date beforeStartDate(boost::gregorian::from_string("2020-Jan-23")); 
  date afterEndDate(boost::gregorian::from_string("2024-Aug-10")); 

  REQUIRE(statementList[beforeStartDate].getAmount() == -130);
  REQUIRE(statementList[beforeStartDate].getBalance() == 270); 
  REQUIRE(statementList[afterEndDate].getAmount() == 50); 
  REQUIRE(statementList[afterEndDate].getBalance() == 320); 

}
