#include "statement_list.hpp"

const std::optional<int> StatementList::getAmount(boost::gregorian::date date) const {
  const auto it = statements.find(date);
  if (it != statements.end()) {
    return std::get<0>(it->second);
  }
  return std::nullopt;
}

const std::optional<int> StatementList::getBalance(boost::gregorian::date date) const {
  const auto it = statements.find(date);
  if (it != statements.end()) {
    return std::get<1>(it->second);
  }
  return std::nullopt;
}

int StatementList::getMaxDeposit() const {
  int max = std::numeric_limits<int>::min();
  for (const auto& statement: statements) {
    const int amount = std::get<0>(statement.second);
    max = amount > max ? amount : max;
  }
  return max;
}


int StatementList::getMaxWithdrawal() const {
  int min = std::numeric_limits<int>::max();
  for (const auto& statement : statements) {
    const int amount = std::get<0>(statement.second);
    min = amount < min ? amount : min;
  }
  return min;
}

int StatementList::getMaxBalance() const {
  int max = std::numeric_limits<int>::min();
  for (const auto& statement: statements) {
    const int balance = std::get<1>(statement.second);
    max = balance > max ? balance : max;
  }
  return max;
}

int StatementList::getMinBalance() const {
  int min = std::numeric_limits<int>::max();
  for (const auto& statement : statements) {
    const int balance = std::get<1>(statement.second);
    min = balance < min ? balance : min;
  }
  return min;
}

std::string StatementList::getMaxDepositDate() const {
  int max = std::numeric_limits<int>::min();
  boost::gregorian::date date{};
  for (const auto& statement: statements) {
    const int amount = std::get<0>(statement.second);
    max = amount > max ? amount : max;
    date = max == amount ? statement.first : date;
  }
  return to_iso_extended_string(date);
}

std::string StatementList::getMaxWithdrawalDate() const {
  int min = std::numeric_limits<int>::max();
  boost::gregorian::date date{};
  for (const auto& statement : statements) {
    const int amount = std::get<0>(statement.second);
    min = amount < min ? amount : min;
    date = min == amount ? statement.first : date;
  }
  return to_iso_extended_string(date);
}

std::string StatementList::getMaxBalanceDate() const {
  int max = std::numeric_limits<int>::min();
  boost::gregorian::date date{};
  for (const auto& statement: statements) {
    const int balance = std::get<1>(statement.second);
    max = balance > max ? balance : max;
    date = max == balance ? statement.first : date;
  }
  return to_iso_extended_string(date);
}

std::string StatementList::getMinBalanceDate() const {
  int min = std::numeric_limits<int>::max();
  boost::gregorian::date date{};
  for (const auto& statement : statements) {
    const int balance = std::get<1>(statement.second);
    min = balance < min ? balance : min;
    date = min == balance ? statement.first : date;
  }
  return to_iso_extended_string(date);
}


void StatementList::printList() const {
  size_t x{0};
  for (const auto& statement : statements) {
    std::cout << x++ << ' ' <<  std::get<0>(statement.second)
    << ' ' << std::get<1>(statement.second) << '\n';
  }
}


