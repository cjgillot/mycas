#!/bin/perl

use strict;
use warnings;

my @classes = qw(basic numerical symbol
  sum prod power function
);

open( HH, ">rtti.hpp" );
open( ML, ">rtti.ml" );

print ML <<EOF ;
type rtti =
EOF

my $counter = 0;

foreach my $class ( @classes )
{
  $class =~ s/\b(\w)/\U$1/g;
  print ML "| $class\n";
  $class =~ s/(\w)/\U$1/g;
  print HH "#define $class\_RTTI $counter\n";
  ++$counter;
}

print HH "#define MAX_RTTI $counter\n";

close( HH ); close( ML );
