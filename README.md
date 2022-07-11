# Banking

**Capital One bank statements (.csv) analyzer.**

#### Dependencies

- Postgres SQL: Version 14 >=
  - libpxx [Official C++ client API for PostgreSQL](pqxx.org/)

#### Requirements

- Please remove the header from your .csv file.
- Please include your Postgres Database credentials in a file named ```creds.hpp```

```cpp
namespace Credentials {
  const std::string user{<DatabaseUser>};
  const std::string password{<DatabaseUserPassword>};
  const std::string dbName{<DatabaseName>};
};
```

- Postgre back-end requires there to be a database on system with the table of the format below:

```sql
CREATE TABLE statements (
  date date NOT NULL,
  amount integer NOT NULL,
  description text NOT NULL,
  balance integer NOT NULL
);
```

> The executable will create its output file, but will fail when program execution begins if *database and credenetials are not properly instantiated.*

Assuming all files in src/ in the same directory compile and build executable in Linux(**Debian**) with:

```
g++ -Wall -Wextra -lpqxx -lpq statement_list.cpp main.cpp -o <executable>
```

**Run the application:**


```
./<executable> <csv_file_path> <output_file_path>
```

Please stay tuned. If you wish to contribute, or recommend implementations please do not hesitate to reach out!


***This application is not endorsed by Capital One***


