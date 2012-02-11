#!/bin/perl

use strict;

sub prepare_byte;
sub declare_byte;

sub prepare_native;
sub declare_native;

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

my @symbols = ( );

open( F, $fun ) || die( "Cannot open file $fun" );

my $module = <F>;
chomp( $module );

my $count = 0;
while( my $line = <F> ) {
  if( $line =~ /^([0-9]+)\s(\w+)/ )
  {
    my @sym = ( $2, $1, "" );
    $symbols[ $count++ ] = \@sym;
  }
}

close F;

@symbols = sort {$$a[0] cmp $$b[0]} @symbols;

if( $native != 0 )
{
  &parse_nm( $module, $obj, \@symbols );

  open( CC, ">$cout.c" );
  open( HH, ">$cout.h" );

  &prepare_native();

  for my $sym (@symbols)
  {
    &declare_native( $$sym[0], $$sym[2], $$sym[1] );
  }
}

else # byte
{
  open( CC, ">$cout.c" );
  open( HH, ">$cout.h" );
  open( ML, ">$cout.ml" );

  &prepare_byte( $module, length( @symbols ) );

  my $index = 0;
  for my $sym (@symbols)
  {
    &declare_byte( $$sym[0], $index++, $$sym[1] );
  }
}

# -- common case -- #

sub prepare_c {
  print HH <<EOF
#include <caml/mlvalues.h>

EOF
;

  print CC <<EOF
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/memory.h>

#define caml_callback0( f ) ( caml_callback( (f), Val_unit ) )
#define caml_callback1 caml_callback

EOF
;
}

sub proto_c {
  my( $func, $arity ) = @_;

  print HH "value $module\_$func(";
  print CC "value $module\_$func(";

  if( $arity > 0 )
  {
    my( $i ) = 1;
  }

  if( $arity > 0 )
  {
    my( $i ) = 1;
    print HH "value";
    print CC "value arg1";

    while( $i++ < $arity )
    {
      print HH ", value";
      print CC ", value arg$i";
    }
  }

  print HH ");\n";
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

  print ML "external export : 'a -> int -> unit = \"__export_functions_$module\"";
  $module =~ s/\b(\w)/\U$1/g;
  print ML "open $module";
}

sub declare_byte {
  my( $func, $index, $arity ) = @_;

  # C side
  &proto_c( $func, $arity );
  print CC "{\n";
  &call_c ( $arity, "(fun_table[$index])" );
  print CC "}\n";

  # Caml side
  print ML "let () = export $func $index\n";
}

# -- native case -- #

sub prepare_native {
  &prepare_c();
}

sub declare_native {
  my( $func, $symbol, $arity ) = @_;

  &proto_c( $func, $arity );

  print CC <<EOF
{
  extern void $symbol();
  static void* fnc = &$symbol;
EOF
  ;

  &call_c( $arity, "(&fun)" );
  print CC "}\n";

}

sub parse_nm {
  my( $module, $obj, $syms ) = @_;
  $module =~ s/\b(\w)/\U$1/g;

  open( NM, "nm --defined-only $obj |" ) || die( "Unsuccesful command : nm" );

  for my $sym ( @$syms )
  {
    my $name = $$sym[0];
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