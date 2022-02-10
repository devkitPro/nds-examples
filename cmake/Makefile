SUBDIRS:= `ls | egrep -v '^(CVS)$$'`
all:
	@for i in $(SUBDIRS); do if test -e $$i/CMakeLists.txt ; then (arm-none-eabi-cmake -B $$i/build -S $$i && cmake --build $$i/build) || { exit 1;} fi; done;
clean:
	@for i in $(SUBDIRS); do rm -rf $$i/build; done;
