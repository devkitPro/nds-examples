SUBDIRS:= `ls | egrep -v '^(CVS|tools)$$'`
all:
	@for i in $(SUBDIRS); do if test -d $$i; then $(MAKE)  -C $$i; fi; done;
clean:
	@for i in $(SUBDIRS); do if test -d $$i; then $(MAKE)  -C $$i clean; fi; done;

export:
	@for i in $(SUBDIRS); do if test -d $$i; then $(MAKE)  -C $$i export; fi; done;

