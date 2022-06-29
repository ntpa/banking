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
    StatementList sl; 
    REQUIRE(sl.size() == 0);
    REQUIRE(sl.getMaxDeposit() == std::numeric_limits<int>::min()); 
    REQUIRE(sl.getMaxWithdrawal() == std::numeric_limits<int>::max()); 
    REQUIRE(sl.getMaxBalance() == std::numeric_limits<int>::min()); 
    REQUIRE(sl.getMinBalance() == std::numeric_limits<int>::max()); 

    date d1(special_values::max_date_time); 
    date d2(special_values::min_date_time); 
    REQUIRE(sl.getStartDate() == d1); 
    REQUIRE(sl.getEndDate() == d2); 

  }
  SECTION("Construct from entries") {
    StatementList sl(pentries, number_entries); 
    REQUIRE(sl.size() == number_entries-1); 
    REQUIRE(sl.getMaxDeposit() == 50); 
    REQUIRE(sl.getMaxWithdrawal() == -130); 
    REQUIRE(sl.getMaxBalance() == 320); 
    REQUIRE(sl.getMinBalance() == 270); 


    REQUIRE(sl.getMaxDepositDate() == maxDepositDate);
    REQUIRE(sl.getMaxWithdrawalDate() == maxWithdrawalDate); 
    REQUIRE(sl.getMaxBalanceDate() == maxBalanceDate);
    REQUIRE(sl.getMinBalanceDate() == minBalanceDate); 
  }

  SECTION("Construct from vector of Statements") {
    std::vector<Statement> statements; 
    Statement s(get_amount(pentries[0]), get_balance(pentries[0]), get_date(pentries[0])); 
    Statement s2(get_amount(pentries[1]), get_balance(pentries[1]), get_date(pentries[1]));
    Statement s3(get_amount(pentries[2]), get_balance(pentries[2]), get_date(pentries[2])); 
    
    statements.push_back(s); statements.push_back(s2); statements.push_back(s3); 
    StatementList sl(statements);  
    
    REQUIRE(sl.size() == number_entries-1); // statements on same day are merged 
    REQUIRE(sl.getMaxDeposit() == 50); 
    REQUIRE(sl.getMaxWithdrawal() == -130); 
    REQUIRE(sl.getMaxBalance() == 320); 
    REQUIRE(sl.getMinBalance() == 270); 
    REQUIRE(sl.getMaxDepositDate() == maxDepositDate);
    REQUIRE(sl.getMaxWithdrawalDate() == maxWithdrawalDate); 
    REQUIRE(sl.getMaxBalanceDate() == maxBalanceDate);
    REQUIRE(sl.getMinBalanceDate() == minBalanceDate); 
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
  StatementList sl(statements);  

  SECTION("Assignment operator") {
    StatementList sl2 = sl;
    REQUIRE(sl2.getMaxDeposit() == sl.getMaxDeposit()); 
    REQUIRE(sl2.getMaxWithdrawal() == sl.getMaxWithdrawal()); 
    REQUIRE(sl2.getMaxBalance() == sl.getMaxBalance());
    REQUIRE(sl2.getMinBalance() == sl.getMinBalance()); 
    
    REQUIRE(sl2.getMaxDepositDate() == sl.getMaxDepositDate());
    REQUIRE(sl2.getMaxWithdrawalDate() == sl.getMaxWithdrawalDate());
    REQUIRE(sl2.getMaxBalanceDate() == sl.getMaxBalanceDate());
    REQUIRE(sl2.getMinBalanceDate() == sl.getMinBalanceDate()); 
    REQUIRE(sl2.getStartDate() == sl.getStartDate());
    REQUIRE(sl2.getEndDate() == sl.getEndDate()); 
  }
  SECTION("Copy operation") {
    StatementList sl2(sl); 
    REQUIRE(sl2.getMaxDeposit() == sl.getMaxDeposit()); 
    REQUIRE(sl2.getMaxWithdrawal() == sl.getMaxWithdrawal()); 
    REQUIRE(sl2.getMaxBalance() == sl.getMaxBalance());
    REQUIRE(sl2.getMinBalance() == sl.getMinBalance()); 
    
    REQUIRE(sl2.getMaxDepositDate() == sl.getMaxDepositDate());
    REQUIRE(sl2.getMaxWithdrawalDate() == sl.getMaxWithdrawalDate());
    REQUIRE(sl2.getMaxBalanceDate() == sl.getMaxBalanceDate());
    REQUIRE(sl2.getMinBalanceDate() == sl.getMinBalanceDate()); 
    REQUIRE(sl2.getStartDate() == sl.getStartDate());
    REQUIRE(sl2.getEndDate() == sl.getEndDate()); 
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
  StatementList sl(statements);  

  
  date validDate1(boost::gregorian::from_string("2020-May-10")); 
  date validDate2(boost::gregorian::from_string("2020-Sep-20")); 
  REQUIRE(sl[validDate1].getAmount() == -130);
  REQUIRE(sl[validDate1].getBalance() == 270); 
  REQUIRE(sl[validDate2].getAmount() == 50);
  REQUIRE(sl[validDate2].getBalance() == 320); 

  date beforeStartDate(boost::gregorian::from_string("2020-Jan-23")); 
  date afterEndDate(boost::gregorian::from_string("2024-Aug-10")); 

  REQUIRE(sl[beforeStartDate].getAmount() == -130);
  REQUIRE(sl[beforeStartDate].getBalance() == 270); 
  REQUIRE(sl[afterEndDate].getAmount() == 50); 
  REQUIRE(sl[afterEndDate].getBalance() == 320); 

}
