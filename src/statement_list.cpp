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
    else { /* New Date */ 
      start_date = gregorian_date < start_date ? gregorian_date : start_date; 
      end_date = gregorian_date > end_date ? gregorian_date : end_date;     
  
      // operator[] chosen over insert() and emplace() methods due to 
      // std::pair bug in template conversion 
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

boost::gregorian::date StatementList::getMaxDepositDate() const {
  int max = std::numeric_limits<int>::min();
  boost::gregorian::date date{};
  for (const auto& it: list) {
    const int amount = (it.second).getAmount();
    max = amount > max ? amount : max;
    date = max == amount ? it.first : date;
  }
  return date;
}

boost::gregorian::date StatementList::getMaxWithdrawalDate() const {
  int min = std::numeric_limits<int>::max();
  boost::gregorian::date date{};
  for (const auto& it: list) {
    const int amount = (it.second).getAmount();
    min = amount < min ? amount : min;
    date = min == amount ? it.first : date;
  }
  return date;
}

boost::gregorian::date StatementList::getMaxBalanceDate() const {
  int max = std::numeric_limits<int>::min();
  boost::gregorian::date date{};
  for (const auto& it : list) {
    const int balance = (it.second).getBalance(); 
    max = balance > max ? balance : max;
    date = max == balance ? it.first : date;
  }
  return date;
}

boost::gregorian::date StatementList::getMinBalanceDate() const {
  int min = std::numeric_limits<int>::max();
  boost::gregorian::date date{};
  for (const auto& it : list) {
    const int balance = (it.second).getBalance();
    min = balance < min ? balance : min;
    date = min == balance ? it.first : date;
  }
  return date;
}


boost::gregorian::date StatementList::getStartDate() const { return start_date; }
boost::gregorian::date StatementList::getEndDate() const { return end_date; }


const StatementList StatementList::getStatementRange(boost::gregorian::date start, boost::gregorian::date end) const {
  // TODO: Handle incorrect date format given 
  // gracefully handle incorrect statement date range
  StatementList result; 
  if (start > end) {
    perror("Please make sure start date is prior to end date\n"); 
    return result;
  }

  start = start < start_date ? start_date : start;
  end = end > end_date ? end_date : end; 

  // safe to assume argument date is within range (start_date, end_date)
  auto findClosestStartDate = [this](boost::gregorian::date date) {
    return list.lower_bound(date); 
    
  };
  // functions seperate to allow end to be inclusive 
  auto findClosestEndDate =  [this](boost::gregorian::date date) {
    return list.upper_bound(date); 
  };
  // Bi-directional iterators
  auto it_start = findClosestStartDate(start);
  const auto it_end = findClosestEndDate(end); 

  for (; it_start != it_end; it_start++) {
    const Statement temp = it_start->second; 
    result.addStatement(temp); 
  }  

  return result;
}

void StatementList::printList() const {
  for (const auto& it : list) {
    std::cout << it.first << ' ' <<  (it.second).getAmount()
    << ' ' << (it.second).getBalance() << '\n';
  }
}


Statement& StatementList::operator[](boost::gregorian::date date) {
  if (date < start_date) {
    return list[start_date]; 
  }
  else if (date > end_date) {
    return list[end_date];
  }
  else {
    return (list.lower_bound(date))->second;
  }
}

