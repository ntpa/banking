#!/bin/awk -f
# Filename: format_date.awk
# Author: NTPA
# 
# Description: Change the date format of a Capital One statement CSV. Assumes that
#              the date is in the format MM/DD/YYYY. Also removes CSV header
#
#               This script should be called on a Capital One statement CSV prior to load into database.
#               Otherwise, there will be bugs that result from the database date format interpretation

  BEGIN { FS = ","; OFS = "," } 
  $2 ~ "-" { print $0 } # Date properly formatted, leave unchanged
  $2 ~ "/" { split($2, arr, "/"); $2 = "20"arr[3]"-"arr[1]"-"arr[2]; print $0} # Transform Date from MM/DD/YY to YYYY-MM-DD

