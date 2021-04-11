
%module sample
%{
#include "sample.h"
%}
%include <cpointer.i>
%include <typemaps.i>

typedef unsigned short  uint16_t;
typedef unsigned int  uint32_t;
typedef unsigned char  uint8_t;
typedef signed short  int16_t;
typedef signed int  int32_t;
typedef signed char  int8_t;

%include "mts_sample.h"
%include "sample.h"


//%extend mts_bind_options_t {
//    mts_bind_options_t * create() {
//        return (mts_bind_options_t *) malloc(sizeof(mts_bind_options_t));
//    }
//    void delete() {
//        free(self);
//    }
//};


