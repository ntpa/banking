#include <pqxx/pqxx>
#include <sstream>
#include "statement_list.hpp"
#include "parser.hpp"
#include "creds.hpp"

// Author: Nathan Tadesse
// Date: 06-19-22
// Filename: main.cpp




/*
 * main() - takes in space seperated file which represents Capital One Bank Statements from certain time period
 *          and creates a .dat file for gnuplot usage. 
 *
 * @argc: number of command line arguments
 * @argv: the strings representing command line arguments. Expects a filename that points to a
 *        csv file of the representation in description, and an output file specifier for the .dat file
 *
 *  Description:
 *          Given an input file that references a file containing a Capital One Bank Statement
 *          main() will traverse the file and combine all entries that happened on the same date.
 * 
 *          An entry will have the format below -
 *          Date Amount Balance
 *  
 * Return: 0 represents successful operation, otherwise error in execution
 *
 */

int main(int argc, char *argv[]) {
  if (argc != 3) {
    perror("Usage: <executable> <input-file> <output-file>\n"); 
    return -1; 
  }
 
  std::ifstream ifs(argv[1], std::ifstream::in);
  std::ofstream ofs(argv[2], std::ofstream::out); 
  if (!ifs.is_open() || !ofs.is_open()) {
    perror("Failed to open input or output file\n"); 
    return -1;
  }

  aria::csv::CsvParser parser(ifs);
  StatementList statementList;
  enum Field { date = 1, amount = 2, description = 4, balance = 5 };
  for (auto& row : parser) {
    // remove single quotes for future SQL statement
    // excessive copying 
    std::string description = row.at(Field::description); 
    auto descriptionWithoutSingleQuotes = [&description]() -> std::string {
      for (auto it = description.begin(); it != description.end(); it++) {
        if (*it == '\'') {
          description.erase(it); 
        }
      }
      return description; 
    };
    const Statement statement(std::stoi(row.at(Field::amount))
                             , std::stoi(row.at(Field::balance))
                             , row.at(Field::date)
                             , descriptionWithoutSingleQuotes()); 
    statementList.addStatement(statement); 
  } /* Read all rows in CSV file */ 
  
  ofs << statementList;
  ifs.close(); 
  ofs.close();

  // Postgre Database operations
  try {
    const auto list = statementList.getList();  
    std::stringstream execStatement;
    execStatement << "INSERT INTO statements VALUES";
    // Construct one multiple insert statement to avoid transaction closing errors
    size_t counter{0}; 
    for (const auto& entry : list) {  
      Statement statement = entry.second; 
      execStatement << " ( \'";
      execStatement << boost::gregorian::to_simple_string(entry.first);
      execStatement << "\', ";
      execStatement << std::to_string(statement.getAmount());
      execStatement <<  ", ";
      
      execStatement << "\'" << statement.getDescription() << "\'"; 
      execStatement << ", "; 
      // End properly on final value
      if (counter == statementList.getNumStatements() - 1) {
        execStatement << std::to_string(statement.getBalance()) << ");"; 
      }
      else {
        execStatement <<  std::to_string(statement.getBalance()) <<  "),";
      }
      counter++; 
    }
    // Wait to start connection to avoid timeout during above generation
    std::stringstream credentials; 
    credentials << "user=" << Credentials::user;
    credentials << " password=" << Credentials::password;
    credentials << " dbname=" << Credentials::dbName; 
    pqxx::connection c(credentials.str());
    pqxx::nontransaction w(c); // transactional integrity not require   

    // Only safe for 'trusted' statements
    pqxx::result r = w.exec(execStatement);
    w.commit();
  }
  catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

}
