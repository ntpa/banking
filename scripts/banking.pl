use v5.24;
use Text::ASCIITable;

my %transactions;
my $longest = 0;

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
      $transactions{$description}[0]++; 
      # total amount 
      $transactions{$description}[1] += $amount;
    }
    else {
      # run more filtering
      $transactions{$description}[0] = 1;
      $transactions{$description}[1] = $amount;
    }
  
  # get longest description for concluding format
  my $description_length = length( $description );
  $longest = $description_length if $description_length > $longest; 

  } else {
   die "CSV file contains line with incorrect format: $!"
  }

} # No more input


my $t = Text::ASCIITable->new({ headingText => 'Transaction Summary' });
$t->setCols('Transaction Description', 'Total Transactions', 'Total Amount');

# sort descending order
foreach my $key (sort { $transactions{$b}[0] <=> $transactions{$a}[0] } keys %transactions) {
  # printf "%-${longest}s: %d for %d\n", $key, $transactions{$key}[0], $transactions{$key}[1];
  $t->addRow($key, $transactions{$key}[0], $transactions{$key}[1]);
}

print $t;


