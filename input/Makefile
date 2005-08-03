SUBDIRS:= `ls | egrep -v '^(CVS)$$'`
all:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then make  -C $$i; fi; done;
clean:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then make  -C $$i clean; fi; done;
install:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then make  -C $$i install; fi; done;
