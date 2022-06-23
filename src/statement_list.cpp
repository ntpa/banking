#include "statement_list.hpp"

static boost::gregorian::date fromUSDateToGregorianDate(const std::string date) {
  std::string undelimited_date = "20"; // assume no date is prior to year 2000
  undelimited_date.push_back(date[6]); undelimited_date.push_back(date[7]); // year
  undelimited_date.push_back(date[0]); undelimited_date.push_back(date[1]); // month
  undelimited_date.push_back(date[3]); undelimited_date.push_back(date[4]); // day

  auto boost_date = boost::gregorian::from_undelimited_string(undelimited_date);
  return boost_date; 
}

// Any statements added to StatementList pass through this function 
void StatementList::addStatement(const Statement& statement) {
  auto gregorian_date = fromUSDateToGregorianDate(statement.getDate()); 
    auto it = list.find(gregorian_date);
    if (it != list.end()) {
        /* balance remains unchanged because the first read balance 
         * represents the sum balance from all statements on that date
         */ 
        int new_amount = (it->second).getAmount() + statement.getAmount(); 
        Statement temp_statement(new_amount, (it->second).getBalance()
                                , (it->second).getDate()); 
        list[gregorian_date] = temp_statement;
    }
    else {
      // operator[] chosen over insert() and emplace() methods due to 
      // std::pair template operation 
      list[gregorian_date] = statement;
    }
}
 
StatementList::StatementList(struct Entry **pentries, size_t number_entries) { 
  for (size_t i = 0; i < number_entries; i++) {
    if (pentries[i] == NULL) {
      // Likely incorrect number_entries given 
      break;
    }
    Statement statement(get_amount(pentries[i]), get_balance(pentries[i])
                        , get_date(pentries[i])); 
    this->addStatement(statement);
  }
}

StatementList::StatementList(const std::vector<Statement>& statements) {
  for (const auto& statement : statements) {
    this->addStatement(statement); 
  } 

}


const std::optional<int> StatementList::getAmount(boost::gregorian::date date) const {
  const auto it = list.find(date);
  if (it != list.end()) {
    return (it->second).getAmount();
  }
  return std::nullopt;
}

const std::optional<int> StatementList::getBalance(boost::gregorian::date date) const {
  const auto it = list.find(date);
  if (it != list.end()) {
    return (it->second).getBalance(); 
  }
  return std::nullopt;
}

int StatementList::getMaxDeposit() const {
  int max = std::numeric_limits<int>::min();
  for (const auto& it : list) {
    const int amount = (it.second).getAmount(); 
    max = amount > max ? amount : max;
  }
  return max;
}


int StatementList::getMaxWithdrawal() const {
  int min = std::numeric_limits<int>::max();
  for (const auto& it : list) {
    const int amount = (it.second).getAmount();
    min = amount < min ? amount : min;
  }
  return min;
}

int StatementList::getMaxBalance() const {
  int max = std::numeric_limits<int>::min();
  for (const auto& it : list) {
    const int balance = (it.second).getBalance(); 
    max = balance > max ? balance : max;
  }
  return max;
}

int StatementList::getMinBalance() const {
  int min = std::numeric_limits<int>::max();
  for (const auto& it : list) {
    const int balance = (it.second).getBalance();
    min = balance < min ? balance : min;
  }
  return min;
}

std::string StatementList::getMaxDepositDate() const {
  int max = std::numeric_limits<int>::min();
  boost::gregorian::date date{};
  for (const auto& it: list) {
    const int amount = (it.second).getAmount();
    max = amount > max ? amount : max;
    date = max == amount ? it.first : date;
  }
  return to_iso_extended_string(date);
}

std::string StatementList::getMaxWithdrawalDate() const {
  int min = std::numeric_limits<int>::max();
  boost::gregorian::date date{};
  for (const auto& it: list) {
    const int amount = (it.second).getAmount();
    min = amount < min ? amount : min;
    date = min == amount ? it.first : date;
  }
  return to_iso_extended_string(date);
}

std::string StatementList::getMaxBalanceDate() const {
  int max = std::numeric_limits<int>::min();
  boost::gregorian::date date{};
  for (const auto& it : list) {
    const int balance = (it.second).getBalance(); 
    max = balance > max ? balance : max;
    date = max == balance ? it.first : date;
  }
  return to_iso_extended_string(date);
}

std::string StatementList::getMinBalanceDate() const {
  int min = std::numeric_limits<int>::max();
  boost::gregorian::date date{};
  for (const auto& it : list) {
    const int balance = (it.second).getBalance();
    min = balance < min ? balance : min;
    date = min == balance ? it.first : date;
  }
  return to_iso_extended_string(date);
}


void StatementList::printList() const {
  size_t x{0};
  for (const auto& it : list) {
    // TODO: make 'x' the date period between statements
    std::cout << x++ << ' ' <<  (it.second).getAmount()
    << ' ' << (it.second).getBalance() << '\n';
  }
}


