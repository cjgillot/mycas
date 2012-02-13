#!/bin/perl

use strict;

sub prepare;
sub declare;
sub finish;

# -- main code -- #

my $cout = $ARGV[0];
my $fun = $ARGV[1];

# array of exported symbols
# each symbol has the form ( exported-name, arity, module-name, symbol-name )
my @symbols = ( );

open( F, $fun ) || die( "Cannot open file $fun" );

# file syntax :
# first line is exportation prefix
# each other line has the form : arity module:symbol( as aliased-name)?

my $module = <F>;
chomp( $module );

my $count = 0;
while( my $line = <F> ) {
  if( $line =~ /^([0-9]+)\s+(\w+):(\w+)(\s+as\s+(\w+))?/ )
  {
    my @sym;
    if( defined $4 ) # some alias
    {
      @sym = ( $5, $1, $2, $3 );
    }
    else
    {
      @sym = ( $3, $1, $2, $3 );
    }
    $symbols[ $count++ ] = \@sym;
  }
}

close F;

# sort by seeked symbol, since nm does it
@symbols = sort {"$$a[2]__$$a[3]" cmp "$$b[2]__$$b[3]"} @symbols;

open( HH, ">$cout.h" );

&prepare();

for my $sym (@symbols)
{
  &declare( $$sym[0], $$sym[1] );
}

&finish();

# -- common case -- #

sub prepare {
  print HH <<EOF
#include <caml/mlvalues.h>

#ifdef __cplusplus
extern "C" {
#endif

EOF
;
}

sub finish {
  print HH <<EOF
#ifdef __cplusplus
}
#endif

EOF
;
}

sub declare {
  my( $func, $arity ) = @_;

  print HH "value $module\_$func(";

  if( $arity > 0 )
  {
    my( $i ) = 1;
    print HH "value";
    print HH ", value" while( $i++ < $arity );
  }

  print HH ");\n";
}
