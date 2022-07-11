# Banking

**Capital One bank statement logger**

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
CREATE TABLE <table_name> (
  date date NOT NULL,
  amount integer NOT NULL,
  description text NOT NULL,
  balance integer NOT NULL
);
````

#### Security

For remote operation on untrusted networks, it is recommended to select a more secure option for client authentication than the defaults in ```pga_hba.conf```

Please refer to [Client Authentication](https://www.postgresql.org/docs/current/client-authentication.html) and [Server Configuration](https://www.postgresql.org/docs/current/runtime-config.html) for more details. 

#### Executable operation

> The executable will create its output file, but will fail when program execution begins if *database and credentials are not properly instantiated.*

Assuming all files in src/ in the same directory compile and build executable in Linux(**Debian**) with:

```
g++ -Wall -Wextra -lpqxx -lpq -I/<include_path> statement_list.cpp main.cpp -o <executable>
```

```
./<executable> <csv_file_path> <output_file_path> <table_name> 
```

> If you do not desire back-end support, please use *"NOTABLE" for your <table_name>*, and the application will skip database operations. 

Please stay tuned. If you wish to contribute, or recommend implementations(especially if you can recommend a better name for the project) please do not hesitate to reach out!


***This application is not endorsed by Capital One***


