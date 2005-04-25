#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
 
#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	Complex2D
BUILD		:=	build
SOURCES		:=	gfx source data  
INCLUDES	:=	include build
ICON		:=	default_icon.bin
 
#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-mthumb -mthumb-interwork

# note: arm9tdmi isn't the correct CPU arch, but anything newer and LD
# *insists* it has a FPU or VFP, and it won't take no for an answer!
CFLAGS	:=	-g -Wall -O2\
 		-mcpu=arm9tdmi -mtune=arm9tdmi -fomit-frame-pointer\
		-ffast-math \
		$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM9

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-g $(ARCH) -mno-fpu -Wl,-Map,$(notdir $*.map)

#---------------------------------------------------------------------------------
# path to tools - this can be deleted if you set the path in windows
#---------------------------------------------------------------------------------
export PATH		:=	$(DEVKITARM):$(PATH)
 
#---------------------------------------------------------------------------------
# PATH to ndslib - just make a system variable called NDSLIBPATH and be done with it
#---------------------------------------------------------------------------------
export NDSLIBPATH	:=	/c/projects/devkitpro/libnds
 
#---------------------------------------------------------------------------------
# the prefix on the compiler executables
#---------------------------------------------------------------------------------
PREFIX			:=	arm-elf-
#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:= -lnds9
 
 
#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(NDSLIBPATH)
 
 
#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
 
export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
 
export CC		:=	$(PREFIX)gcc
export CXX		:=	$(PREFIX)g++
export AR		:=	$(PREFIX)ar
export OBJCOPY	:=	$(PREFIX)objcopy
 
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
PCXFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.pcx)))
BINFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bin)))
PALFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.pal)))
RAWFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.raw)))
MAPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.map)))
 
#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES	:=	$(MAPFILES:.map=.o) $(RAWFILES:.raw=.o) $(PALFILES:.pal=.o) $(BINFILES:.bin=.o) $(PCXFILES:.pcx=.o)\
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)
 
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)
 
.PHONY: $(BUILD) clean
 
#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
 
#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) *.elf *.nds* *.bin 
 
 
#---------------------------------------------------------------------------------
else
 
DEPENDS	:=	$(OFILES:.o=.d)
 
#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).nds.gba	: 	$(OUTPUT).nds

$(OUTPUT).nds	: 	$(OUTPUT).bin

$(OUTPUT).bin	:	$(OUTPUT).elf
 
$(OUTPUT).elf	:	$(OFILES)
 
#---------------------------------------------------------------------------------
%.nds.gba: %.nds
	dsbuild $< $@
	@echo built ... $(notdir $@)

#---------------------------------------------------------------------------------
%.nds: %.bin
	@ndstool -c $@ -9 $<
	@echo built ... $(notdir $@)


#---------------------------------------------------------------------------------
%.bin: %.elf
	@$(OBJCOPY) -O binary $< $@
	@echo built ... $(notdir $@)
 
#---------------------------------------------------------------------------------
%.elf:
	@echo linking binary
	@$(LD)  $(LDFLAGS) -specs=ds_arm9.specs $(OFILES) $(LIBPATHS) $(LIBS) -o $@
 
 
 
#---------------------------------------------------------------------------------
# Compile Targets for C/C++
#---------------------------------------------------------------------------------
 
#---------------------------------------------------------------------------------
%.o : %.cpp
	@echo $(notdir $<)
	@$(CXX) -MM $(CFLAGS) -o $*.d $<
	@$(CXX) $(CFLAGS) -c $< -o$@
 
#---------------------------------------------------------------------------------
%.o : %.c
	@echo $(notdir $<)
	@$(CC) -MM $(CFLAGS) -o $*.d $<
	@$(CC)  $(CFLAGS) -c $< -o$@
 
#---------------------------------------------------------------------------------
%.o : %.s
	@echo $(notdir $<)
	@$(CC) -MM $(CFLAGS) -o $*.d $<
	@$(CC)  $(ASFLAGS) -c $< -o$@
 
#---------------------------------------------------------------------------------
# canned command sequence for binary data
#---------------------------------------------------------------------------------
define bin2o
	cp $(<) $(*).tmp
	alignbin $(*).tmp
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm \
	--rename-section .data=.rodata,readonly,data,contents,alloc \
	--redefine-sym _binary_`(echo $(*) | tr . _)`_tmp_start=`(echo $(*) | tr . _)`\
	--redefine-sym _binary_`(echo $(*) | tr . _)`_tmp_end=`(echo $(*) | tr . _)`_end\
	--redefine-sym _binary_`(echo $(*) | tr . _)`_tmp_size=`(echo $(*) | tr . _)`_size\
	$(*).tmp $(@)
	echo "extern const u32" `(echo $(*) | tr . _)`"_end[];" >> $(*).h
	echo "extern const u8" `(echo $(*) | tr . _)`"[];" >> $(*).h
	echo "extern const u32" `(echo $(*) | tr . _)`_size[]";" >> $(*).h
	rm $(*).tmp
endef
 
#---------------------------------------------------------------------------------
%.o	:	%.pcx
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)
 
#---------------------------------------------------------------------------------
%.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)
 
#---------------------------------------------------------------------------------
%.o	:	%.raw
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)
 
#---------------------------------------------------------------------------------
%.o	:	%.pal
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)
 
#---------------------------------------------------------------------------------
%.o	:	%.map
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.o	:	%.mdl
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

 
-include $(DEPENDS)
 
#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------