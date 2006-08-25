SUBDIRS:= `ls | egrep -v '^(CVS)$$'`
all:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE)  -C $$i; fi; done;
clean:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE)  -C $$i clean; fi; done;
install:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE)  -C $$i install; fi; done;
