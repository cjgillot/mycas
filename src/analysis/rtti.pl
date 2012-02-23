#!/bin/perl

use strict;
use warnings;

my @classes = qw(
  basic numerical symbol
  sum prod power
  function
);

my @functions = qw(
  exp log
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

print ML <<EOF ;
type corefuncs = [
EOF

foreach my $f ( @functions )
{
  $f =~ s/\b(\w)/\U$1/g;
  print ML "| `$f\n";

  my $fun = $f;
  $fun =~ s/(\w)/\U$1/g;
  print HH "#define $fun\_HASH \"$f\"\n";
}

print ML <<EOF ;
]
EOF

close( HH ); close( ML );
