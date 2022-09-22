#!/bin/bash 
#
# Filename: append_statements.sh
#
#
# Usage: ./append_statements <statements-to-add> <statements-to-be-append> <output-file>
#
# This will overwrite any existing output file. However, will not overwrite the statement-to-be appended

if [[ $# -ne 3 ]]; then
  echo "Not enough arguements."
  echo "Usage: ./append_statements <statements-to-add> <statements-to-be-append> <output-file>"
else
  if [[ -f $1 && -f $2 ]]; then
    touch $3
    cat $2 > $3
    ./format_date.awk $1 >> $3
    sort -t "," -k 2 -r -u -o $3 $3 # sort in place
  else 
    echo "Cannot find file $1 and/or $2...exiting"
  fi 
fi
