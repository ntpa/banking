#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "statement_list.hpp"

TEST_CASE("Statement Constructor", "[Statement]") {
  SECTION("Default") {
    Statement statement; 
    REQUIRE(statement.getAmount() == 0);
    REQUIRE(statement.getBalance() == 0); 
    REQUIRE(statement.getDate().empty() == true);
    REQUIRE(statement.getDescription().empty() == true); 
  }
  SECTION("Parametrized") {
    Statement statement(200, 250, "06/13/22", "Deposit from Simply Checking XXXXXX0133");
    REQUIRE(statement.getAmount() == 200);
    REQUIRE(statement.getBalance() == 250);
    REQUIRE(statement.getDate() == "06/13/22"); 
    REQUIRE(statement.getDescription() == "Deposit from Simply Checking XXXXXX0133");

    Statement statement2(-100, 150, "06/13/22", "Withdrawal from PAYPAL to NATHANIEL TADESSE INST XFER");
    REQUIRE(statement2.getAmount() == -100);
    REQUIRE(statement2.getBalance() == 150);
    REQUIRE(statement2.getDate() == "06/13/22"); 
    REQUIRE(statement2.getDescription() == "Withdrawal from PAYPAL to NATHANIEL TADESSE INST XFER");
  }
}

TEST_CASE("StatementList Constructors", "[StatementList]") {
  SECTION("Default") {
    StatementList statementList; 
    REQUIRE(statementList.getNumStatements() == 0);
    REQUIRE(statementList.getNumDeposits() == 0);
    REQUIRE(statementList.getNumWithdrawals() == 0); 
    REQUIRE(statementList.getMaxDeposit() == std::numeric_limits<int>::min()); 
    REQUIRE(statementList.getMaxWithdrawal() == std::numeric_limits<int>::max()); 
    REQUIRE(statementList.getMaxBalance() == std::numeric_limits<int>::min()); 
    REQUIRE(statementList.getMinBalance() == std::numeric_limits<int>::max()); 

    boost::gregorian::date d1(boost::date_time::special_values::max_date_time); 
    boost::gregorian::date d2(boost::date_time::special_values::min_date_time); 
    boost::gregorian::date d3; 
    REQUIRE(statementList.getStartDate() == d1); 
    REQUIRE(statementList.getEndDate() == d2); 
    REQUIRE(statementList.getMaxDepositDate() == d3); 
    REQUIRE(statementList.getMaxWithdrawalDate() == d3);
    REQUIRE(statementList.getMaxBalanceDate() == d3);
    REQUIRE(statementList.getMinBalanceDate() == d3); 

  }

  SECTION("Construct from vector of Statements") {
    std::vector<Statement> statements; 
    statements.emplace_back(200, 250, "06/13/22", "Deposit from Simply Checking XXXXXX0133");
    statements.emplace_back(-100, 150, "06/13/22", "Withdrawal from PAYPAL to NATHANIEL TADESSE INST XFER");
    statements.emplace_back(-50, 100, "06/15/22", "Withdrawal from PAYPAL to NATHANIEL TADESSE INST XFER"); 
    statements.emplace_back(-10, 90, "06/15/22", "Withdrawal from PAYPAL to NATHANIEL TADESSE INST XFER"); 
    statements.emplace_back(40, 130, "06/15/22", "Withdrawal from PAYPAL to NATHANIEL TADESSE INST XFER"); 
    
    StatementList statementList(statements); 
    REQUIRE(statementList.getNumStatements() == 5);
    REQUIRE(statementList.getNumDeposits() == 2);
    REQUIRE(statementList.getNumWithdrawals() == 3); 
    REQUIRE(statementList.getMaxDeposit() == 200);
    REQUIRE(statementList.getMaxWithdrawal() == -100);
    REQUIRE(statementList.getMaxBalance() == 250);
    REQUIRE(statementList.getMinBalance() == 90);

    boost::gregorian::date startDate(boost::gregorian::from_string("2022/06/13"));
    boost::gregorian::date endDate(boost::gregorian::from_string("2022/06/15"));
    boost::gregorian::date maxDepositDate(startDate);
    boost::gregorian::date maxWithdrawalDate(startDate);
    boost::gregorian::date maxBalanceDate(startDate);
    boost::gregorian::date minBalanceDate(endDate); 
    
    REQUIRE(statementList.getStartDate() == startDate); 
    REQUIRE(statementList.getEndDate() == endDate); 
    REQUIRE(statementList.getMaxDepositDate() == maxDepositDate); 
    REQUIRE(statementList.getMaxWithdrawalDate() == maxWithdrawalDate);
    REQUIRE(statementList.getMaxBalanceDate() == maxBalanceDate);
    REQUIRE(statementList.getMinBalanceDate() == minBalanceDate); 
    
  }
}


TEST_CASE("Assignment and Copy operations", "[StatementList]") {
  SECTION("Assignment operator") {
  
  }
  SECTION("Copy operation") {
  
  }

}
