CLEANEXTS   = o so

# Specify the source files, the target files,
# and the install directory
CC          = gcc
CFLAGS      = -std=c99 -fPIC -Wall -I/usr/cisco/packages/python/python-2.7.10/include/python2.7 -I.
LDFLAGS     = -L.

LIB            = libftutils.so
SOURCES     = ft_db.c ft_mem.c

OBJ			= ftutils
OBJSOURCES	= ft_utils.c 

SWIG        = /router/bin/swig
SWIGFLAGS    = -python
SWIGCFLAGS   = -I/usr/include/python2.6
SWIGLIB      = _ftutils.so
SWIGINTERFACES = ftutils.i
SWIGPY = $(subst .i,.py,$(SWIGINTERFACES))
SWIGSOURCES    = $(subst .i,_wrap.c,$(SWIGINTERFACES))

INSTALLDIR  = .

# echo a variable
# $(info   SWIGSOURCES is $(SWIGSOURCES))

.PHONY: all
all: $(SWIGSOURCES) $(LIB) $(SWIGLIB) # $(OBJ)

$(LIB): $(subst .c,.o,$(SOURCES))
	$(CC) -shared -fPIC $(LDFLAGS) -o $@ $^

$(SWIGSOURCES): $(SWIGINTERFACES)
	$(SWIG) $(SWIGFLAGS) $(SWIGINTERFACES)

$(SWIGLIB): $(subst .c,.o,$(SOURCES)) $(subst .c,.o,$(SWIGSOURCES))
	$(CC) -shared $(LDFLAGS) -o $@ $^

#$(OBJ): $(subst .c,.o,$(OBJSOURCES)) $(LIB)
#	$(CC) -o ftutils $(OBJSOURCES

.PHONY: install
install:
	mkdir -p $(INSTALLDIR)
	cp -p $(OUTPUTFILE) $(INSTALLDIR)

.PHONY: clean
clean:
	for file in $(CLEANEXTS); do rm -f *.$$file; done
	rm -f $(SWIGSOURCES)
	rm -f $(SWIGPY)
	rm -f $(SWIGLIB)

# Generate dependencies of .ccp files on .hpp files
# include $(subst .cpp,.d,$(SOURCES))
# 
# %.d: %.cpp
#     $(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
#   #         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#   #         rm -f $@.$$$$


