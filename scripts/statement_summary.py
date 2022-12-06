# Filename: statement_summary.py
# Author: NTPA
# Date: December 6, 2022
# Description: Summary of statement list, 
#           Assumes: 
#               - No duplicate transactions 
#               - Proper date format(YYYY-MM-DD)
#               - No CSV Header


import sys
import csv
from datetime import date
from decimal import Decimal
# Monetary calculations. Inaccurate, but okay for simple use case in the program
def remove_exponent(d):
    return quantize(Decimal(1)) if d == d.to_integral() else d.normalize()

TWOPLACES = Decimal(10) ** -2       # same as Decimal('0.01')

if (len(sys.argv) < 2):
    print("Usage: python statement_summary.py <statement.csv>")
    exit()

with open (sys.argv[1], 'r', encoding="utf-8") as csvfile
    csv_reader = csv.reader(csvfile, delimiter=',')
    
    total_transactions = 0
    start_date = date.max
    end_date = date.min
    number_deposits = 0
    number_withdrawals = 0
    total_deposit_amount = Decimal(0)
    total_withdrawal_amount = Decimal(0)

    for row in csv_reader: 
        # Date Summary
        statement_date = date.fromisoformat(row[1])
        start_date = statement_date if start_date > statement_date else start_date
        end_date = statement_date if end_date < statement_date else end_date
        # Deposit/Withdrawal Summary
        if (Decimal(row[2]) > 0):
            number_deposits += 1
            total_deposit_amount += Decimal(row[2])
        else: 
            number_withdrawals += 1 
            total_withdrawal_amount += Decimal(row[2])

        total_transactions += 1

        
print("\nTotal of {} transactions from {} to {}".format(total_transactions, start_date, end_date))
print("Breakdown:")

print(f'\t{number_deposits} deposits for a total of {"{:,}".format(remove_exponent(total_deposit_amount))} USD',
    f' ({Decimal(total_deposit_amount/number_deposits).quantize(TWOPLACES)} USD per deposit)')
print(f'\t{number_withdrawals} for a total of {"{:,}".format(remove_exponent(total_withdrawal_amount))} USD',
    f' ({Decimal(total_withdrawal_amount/number_withdrawals).quantize(TWOPLACES)} USD per withdrawal)')


