#!/bin/perl

use strict;

sub prepare_byte;
sub declare_byte;

sub prepare_native;
sub declare_native;

sub finish_c;

sub parse_nm;

# -- main code -- #

my $native = 0;

my $cout = $ARGV[0];
my $fun = $ARGV[1];
my $obj;

if( $#ARGV >= 2 )
{
  $obj = $ARGV[2];
  $native = 1;
}

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

if( $native != 0 )
{
  &parse_nm( $obj, \@symbols );

  open( CC, ">$cout.cc" );

  &prepare_native();

  for my $sym (@symbols)
  {
    &declare_native( $$sym[0], $$sym[2], $$sym[1] );
  }

  &finish_c();
}

else # byte
{
  open( CC, ">$cout.cc" );
  open( ML, ">$cout.ml" );

  &prepare_byte( $module, length( @symbols ) );

  my $index = 0;
  for my $sym (@symbols)
  {
    &declare_byte( $$sym[0], $index++, $$sym[1], $$sym[2], $$sym[3] );
  }

  &finish_c();
}

# -- common case -- #

sub prepare_c {
  print CC <<EOF
#include "caml/caml_support.hpp"

extern "C" {

#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/memory.h>

#define caml_callback0( f ) ( caml_callback( (f), Val_unit ) )
#define caml_callback1 caml_callback

EOF
;
}

sub finish_c {
  print CC <<EOF
}

EOF
;
}

sub proto_c {
  my( $func, $arity ) = @_;

  print CC "value $module\_$func(";

  if( $arity > 0 )
  {
    my( $i ) = 1;
    print CC "value arg1";
    print CC ", value arg$i" while( $i++ < $arity );
  }

  print CC ")\n";
}

sub call_c {
  my( $arity, $symbol ) = @_;

  if( $arity < 4 )
  {
    my $i = 0;
    print CC "  value ret = caml_callback$arity( (value)$symbol\n";
    print CC "    , arg$i\n" while( $i++ < $arity );
    print CC "  );\n";
  }
  else
  {
    my $i = 1;
    print CC "  value args [", $arity, "] = {\n";
    print CC "      arg1\n";
    print CC "    , arg$i\n" while( $i++ < $arity );
    print CC "  };\n";
    print CC "  value ret = caml_callbackN( (value)$symbol, $arity, args );\n";
  }

  print CC "  return ret;\n";
}

# -- bytecode case -- #

# bytecode export mechanism :
# - export function using a callback regitration in <ML>
# - the registration is provided by <CC> in "__export_functions_$module"
# - call is made using registered values

sub prepare_byte {
  my( $module, $size ) = @_;

  &prepare_c();
  print CC <<EOF
static value fun_table[$size];

CAMLprim value __export_functions_$module( value fun, value index )
{
  CAMLparam2( fun, index );
  size_t n = Int_val( index );
  fun_table[n] = fun;
  caml_register_generational_global_root( &fun_table[n] );
  CAMLreturn( Val_unit );
}
EOF
  ;

  print ML "external export : 'a -> int -> unit = \"__export_functions_$module\"\n";
}

sub declare_byte {
  my( $func, $index, $arity, $mod, $sym ) = @_;

  # C side
  &proto_c( $func, $arity );
  print CC "{\n";
  print CC "  caml::initializer.touch();\n";
  &call_c ( $arity, "(fun_table[$index])" );
  print CC "}\n";

  # Caml side
  $mod =~ s/\b(\w)/\U$1/g;
  print ML "let () = export $mod.$sym $index\n";
}

# -- native case -- #

# native export mechanism :
# - there is no help <ML> file used for registration
# - calling is made by creating a caml closure
# - if $arity > 1 then closure = { caml_curry$arity, $arity, $symbol }
# - else closure = { $symbol, 1 }

sub prepare_native {
  &prepare_c();
}

sub declare_native {
  my( $func, $symbol, $arity ) = @_;

  &proto_c( $func, $arity );

  print CC "{\n";
  print CC "  caml::initializer.touch();\n";

  if( $arity < 2 )
  {
    print CC <<EOF
  extern void $symbol();
  static value fun[] =
    { (value)&$symbol, Val_int(1) };
EOF
    ;
  }
  else
  {
    print CC <<EOF
  extern void caml_curry$arity();
  extern void $symbol();
  static value fun[] =
    { (value)&caml_curry$arity, Val_int($arity), (value)&$symbol };
EOF
    ;
  }

  &call_c( $arity, "((value*)fun)" );
  print CC "}\n\n";

}

sub parse_nm {
  my( $obj, $syms ) = @_;

  open( NM, "nm --defined-only $obj |" ) || die( "Unsuccesful command : nm" );

  for my $sym ( @$syms )
  {
    # @sym = ( exported-name, arity, module-name, symbol-name )
    my $module = $$sym[2];
    my $name = $$sym[3];

    $module =~ s/\b(\w)/\U$1/g;

    while( my $line = <NM> )
    {
      if( $line =~ /^\s*[0-9a-f]+ T (caml[A-Za-z0-9_]+)\s*/ )
      {
        my $symbol = $1;
        if( $symbol =~ m/$module\__$name/ )
        {
          $$sym[2] = $symbol;
          goto cont;
        }
      }
    }

    die( "Unfound symbol : $name" );

    cont:
      ;
  }
}