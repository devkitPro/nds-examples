SUBDIRS:= `ls | egrep -v '^(CVS)$$'`

DATESTRING	:=	$(shell date +%Y)$(shell date +%m)$(shell date +%d)

all:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then make  -C $$i; fi; done;
clean:
	@rm -f *.bz2
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then make  -C $$i clean; fi; done;
install:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then make  -C $$i install; fi; done;

dist: clean
	@tar --exclude=*CVS* -cvjf nds-examples-$(DATESTRING).tar.bz2 *
