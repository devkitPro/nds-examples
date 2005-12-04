SUBDIRS:= $(shell ls | egrep -v '^(CVS)$$')

DATESTRING	:=	$(shell date +%Y)$(shell date +%m)$(shell date +%d)

#---------------------------------------------------------------------------------
all:
#---------------------------------------------------------------------------------
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE) -C $$i ; fi; done;
	@rm -fr bin
	@mkdir -p bin
	@find ./ -name *.nds -exec cp -fv {} bin \;

#---------------------------------------------------------------------------------
clean:
#---------------------------------------------------------------------------------
	@rm -fr bin
	@rm -f *.bz2
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE)  -C $$i clean; fi; done;

#---------------------------------------------------------------------------------
install:
#---------------------------------------------------------------------------------
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE)  -C $$i install; fi; done;

#---------------------------------------------------------------------------------
dist: clean
#---------------------------------------------------------------------------------
	@rm -fr bin
	@tar --exclude=*CVS* -cvjf nds-examples-$(DATESTRING).tar.bz2 *
