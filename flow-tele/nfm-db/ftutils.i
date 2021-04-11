%module ftutils
%{
#include <ft_db.h>
/* Put headers and other declarations here */
%}


%typemap(in) ft_db_pt *in_db (ft_db_pt db) {
  if ((SWIG_ConvertPtr($input, (void **) &db, $*1_descriptor, SWIG_POINTER_EXCEPTION|SWIG_POINTER_DISOWN)) == -1)
    return NULL;
  $1 = &db;
}

%include<ft_db.h>
