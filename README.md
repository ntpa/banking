# banking

**Capital One bank statements (.csv) analyzer.**

Please remove the header from your .csv file. 

Assuming all files in src/ in the same directory compile and build executable in Linux(**Debian**) with:

```
g++ -Wall -Wextra entry.c statement_list.cpp main.cpp -o bankingcpp
```

Postgre back-end requires there to be a database on system with the table of the format below: 

```sql
CREATE TABLE statements (
  date date PRIMARY KEY,
  amount integer,
  description text,
  balance integer
);
```

The executable ```banking will still create its output file, but will fail when program execution begins for database operations.

Run the application: 

```
./bankingcpp <csv_file_path> <output_file_path> 
```


Please stay tuned. If you wish to contribute, or recommend implementations please do not hesitate to reach out. 


***This application is not endorsed by Capital One***

