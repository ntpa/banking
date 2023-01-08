use v5.24;
use Text::ASCIITable;
use Number::Format qw(:subs); 

my %transactions;
my $longest = 0;
my $total_deposit_amount = 0;
my $total_withdrawal_amount = 0;

# Do not change these values
use constant COUNT => 0;
use constant TOTAL_AMOUNT => 1;
my $DEPOSITS = 'DEPOSITS';
my $WITHDRAWALS = 'WITHDRAWALS';

while (<>) {
  chomp;
  # retrieve fields
  if ($_ =~ m/(\d{4}),
    (?<date>\d{4}-\d{2}-\d{2}),
    (?<amount>-?.*),
    (?<type>.*),
    (?<description>.*),
    (?<balance>.*)/x) {

    # save copy of matches. read up on perl persistance of captures
    my $date = $+{date};
    my $amount = $+{amount};
    my $description = $+{description};
    my $balance = $+{balance};

    # Further filter descriptions! 
    # 1) Remove numbers from description. Require minimum in order to recoginze
    # company name with digit (e.g. 7-ELEVEN
    $description =~ s/[0-9]{3,}//g;
    # 2) Group 'Digital Card' and 'Debit Card' Purchases
    $description =~ s/(Debit|Digital).* - //;

    if (exists $transactions{$description}) {
      # number transactions
      $transactions{$description}[COUNT]++; 
      # total amount 
      $transactions{$description}[TOTAL_AMOUNT] += $amount;
    }
    else {
      # run more filtering
      $transactions{$description}[COUNT] = 1;
      $transactions{$description}[TOTAL_AMOUNT] = $amount;
    }
  
  # get longest description for concluding format
  my $description_length = length( $description );
  $longest = $description_length if $description_length > $longest; 

  # Deposit/Withdrawals Totals
  if ($amount > 0) {
    $transactions{$DEPOSITS}[COUNT]++;
    $transactions{$DEPOSITS}[TOTAL_AMOUNT] += $amount; 
  }
  else {
    $transactions{$WITHDRAWALS}[COUNT]++;
    $transactions{$WITHDRAWALS}[TOTAL_AMOUNT] += $amount;
  }

  } else {
   die "CSV file contains line with incorrect format: $!"
  }

} # No more input


my $t = Text::ASCIITable->new({ headingText => 'Transaction Summary' });
$t->setCols('Transaction Description', 'Total Transactions', 'Total Amount(USD)', 'Average Amount(USD)');

# Wait as long as you can to format monetary numbers


# sort descending order
foreach my $key (sort { $transactions{$b}[COUNT] <=> $transactions{$a}[COUNT] } keys %transactions) {
  # printf "%-${longest}s: %d for %d\n", $key, $transactions{$key}[0], $transactions{$key}[1];
  $t->addRow($key, $transactions{$key}[COUNT], format_number($transactions{$key}[TOTAL_AMOUNT],2),
    format_number($transactions{$key}[TOTAL_AMOUNT]/$transactions{$key}[COUNT],2,1));
}

# Totals 
$t->addRowLine();
$t->addRow('Deposits', $transactions{$DEPOSITS}[COUNT],
  format_number($transactions{$DEPOSITS}[TOTAL_AMOUNT]),
  format_number($transactions{$DEPOSITS}[TOTAL_AMOUNT]/$transactions{$DEPOSITS}[COUNT]));
$t->addRow('Withdrawals', $transactions{$WITHDRAWALS}[COUNT],
  format_number($transactions{$WITHDRAWALS}[TOTAL_AMOUNT]),
  format_number($transactions{$WITHDRAWALS}[TOTAL_AMOUNT]/$transactions{$WITHDRAWALS}[COUNT]));

print $t;


