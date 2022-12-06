# Filename: merge_statements.py
# Author: NTPA
# Date: December 5, 2022
# Description: Python script to replace functionality of awk scripts in directory
#               Not everyone uses a *nix machine... 
#                   
#               Given an two Capital One Statement files: 
# 
#                   1. Ensure correct date format YYYY-MM-DD, if not update. 
#                       (Capital One Statements CSV date currently comes standard
#                       with MM/DD/YY format. However, this does not play nice with 
#                       databases.) 
# 
#                   2. Remove any duplicate statements
#                   3. Append the two statement lists
#                   4. Print to STDOUT 

#       Use your favorite shell command(s) to sort output: 
#           - For example on UNIX system:
#               - sort -t "," -k 2 -r will sort in order of most recent transaction date
#                   to least recent transaction date 

import sys
import csv

# Format a given date to format YYYY-DD-MM. 
# Function assumes that dates come in the format(s): 
#   - YYYY-DD-MM
#   - MM/DD/YY (Capital One Statements at this point in time come in this format).
def format_date(transaction_date): 
    if (len(transaction_date.split('/')) == 3): # MM/DD/YY
        month, day, year = transaction_date.split('/')
        return '20{}-{}-{}'.format(year, month, day)
    else:
        return transaction_date # date already in correct format. see function call assumptions
    
    


if (len(sys.argv) < 3):
    print("Not enough arguments. Did you provide two Capital One Statement lists?")
    print("Usage: python merge_statements.py <statemen-list-1> <statement-list-2>")
    exit()

# Set chosen due to unique property. Important to take the 
# entire line as set value, because multiple transactions can happen on 
# the same date, while all being unique transactions. 
transactions = set()

# read statement 1 
with open (sys.argv[1], 'r', encoding="utf-8") as csvfile:
    csv_reader = csv.reader(csvfile, delimiter=',')
    next(csv_reader) # skip header
    for row in csv_reader: 
        transactions.add("{},{},{},{},{},{}".format(row[0], format_date(row[1]),
                                                  row[2], row[3], row[4], row[5]))
# read statement 2 
with open (sys.argv[2], 'r', encoding="utf-8") as csvfile:
    csv_reader = csv.reader(csvfile, delimiter=',')
    next(csv_reader) 
    for row in csv_reader: 
        transactions.add('{},{},{},{},{},{}'.format(row[0], format_date(row[1]),
                                                  row[2], row[3], row[4], row[5]))

for transaction in transactions:
    print(transaction)


