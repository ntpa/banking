#!/bin/awk -f
# Filename: sum_transactions.awk
# Author: NTPA
# Date: September 20, 2022
# Description: Sum the total number and dollar amount of transactions for each transaction description in a Captial One statements csv
#             
#

BEGIN { FS = "," }

{ num[$5] += 1; } # Field 5 is transaction description Field
{ amount[$5] += $3 } # Field 3 is transaction amount

NR == 1 { end_date = $2 } 

$3 >= 0 { sum_deposits += $3; num_deposits++ }
$3 < 0 { sum_withdrawals += $3 num_withdrawals++}

END { 
      print ""
      print "Total of", NR, "transactions from", $2, "to", end_date
      print "Breakdown:"
      printf("\t%d deposits for a total sum of %s USD (%s USD per deposit)\n",
             num_deposits, addcomma(sum_deposits), addcomma(sum_deposits/num_deposits))
      printf("\t%d withdrawals for a total sum of %s USD (%s USD per withdrawal)\n",
             num_withdrawals, addcomma(sum_withdrawals), addcomma(sum_withdrawals/num_withdrawals))
    }

# Taken from "The AWK Programming Language"
# addcomma - put commas in numbers 
# input: a number per line
# output: the input number followed by
# the number with commas and two decimal places
function addcomma(x, num) {
  if (x < 0)
    return "-" addcomma(-x)
  num = sprintf( "%.2f", x) # num is dddddd.dd
  while (num ~ /[0-9][0-9][0-9][0-9]/)
    sub(/[0-9][0-9][0-9][,.]/, ",&", num)
  return num
}
