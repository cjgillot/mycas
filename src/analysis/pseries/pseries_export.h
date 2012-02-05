#ifndef ANALYSIS_PSERIES_EXPORT_H
#define ANALYSIS_PSERIES_EXPORT_H

#include <caml/mlvalues.h>

#ifdef __cplusplus
#define CPP_BEGIN extern "C" {
#define CPP_END   }
#else
#define CPP_BEGIN
#define CPP_END
#endif

CPP_BEGIN

value pseries_export_empty();

#define UNA( name ) \
value pseries_export_##name( value )

#define BIN( name ) \
value pseries_export_##name( value, value )

UNA( make );

UNA( neg );
UNA( inv );

UNA( null );
UNA( unit );

UNA( diff );

BIN( map );

BIN( add );
BIN( sub );

BIN( mul );
BIN( div );

BIN( compare );

// for iterator
UNA( iterator_force  );
UNA( iterator_forced );

#undef BIN
#undef UNA

CPP_END

#undef CPP_BEGIN
#undef CPP_END

#endif
