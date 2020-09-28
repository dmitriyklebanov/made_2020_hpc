#Compiler and Linker
CC          := g++

# Decide whether the commands will be shwon or not
VERBOSE = TRUE

#The Directories, Source, Includes, Objects, Binary
SRCDIR      := src
BUILDDIR    := obj
LIBDIR      := lib
TARGETDIR   := bin
RESDIR      := res
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o

#The Target Binary Program Sources
TARGETS_SOURCES := \
	$(SRCDIR)/matrix/.tests/tests_perf.cpp \
	$(SRCDIR)/matrix/.tests/tests_matmul.cpp \

#Flags, Libraries and Includes
OPTIMIZE    := -g
CFLAGS      := -Wall -std=c++17 $(OPTIMIZE)
LIB         := -lm
INC         := -I $(SRCDIR) -I /usr/local/include
INCDEP      := -I $(SRCDIR)
SRCLIB      := $(LIBDIR)/src.a

#---------------------------------------------------------------------------------
#Internal targets ans variables
#---------------------------------------------------------------------------------

FIND_EXCEPT_TARGETS = $(foreach filename, $(TARGETS_SOURCES), $(addprefix ! -wholename , $(filename)))
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT) $(FIND_EXCEPT_TARGETS))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
TARGETS_BINARIES := $(patsubst $(SRCDIR)/%,$(TARGETDIR)/%,$(TARGETS_SOURCES:.$(SRCEXT)=))
# TARGETS_OBJECTS  := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(TARGETS_SOURCES:.$(SRCEXT)=.$(OBJEXT)))

# Hide or not the calls depending of VERBOSE
ifeq ($(VERBOSE),TRUE)
    HIDE =
else
    HIDE = @
endif

#Defauilt Make
all: $(TARGETS_BINARIES) directories

#Remake
remake: cleaner all

#Make the Directories
directories:
	$(HIDE) @mkdir -p $(TARGETDIR)
	$(HIDE) @mkdir -p $(BUILDDIR)
	$(HIDE) @mkdir -p $(LIBDIR)

#Clean only Objects
clean:
	$(HIDE) @$(RM) -rf $(BUILDDIR)
	$(HIDE) @$(RM) -rf $(LIBDIR)

#Full Clean, Objects and Binaries
cleaner: clean
	$(HIDE) @$(RM) -rf $(TARGETDIR)

#Pull in dependency info for *existing* .o files
# -include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))
# -include $(TARGETS_OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	echo Compiling $@
	$(HIDE) @mkdir -p $(dir $@)
	$(HIDE) $(CC) $(CFLAGS) $(INC) -c -o $@ $<
	$(HIDE) @$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)

#Create static library
static_sources: $(OBJECTS) directories
	echo Creating sources static library
	ar rc $(SRCLIB) $(OBJECTS)

#Build
$(TARGETDIR)/%: $(TARGETS_SOURCES) static_sources
	echo Building target $@
	$(HIDE) @mkdir -p $(dir $@)
	$(HIDE) $(CC) $(CFLAGS) $(INC) $(patsubst $(TARGETDIR)/%,$(SRCDIR)/%,$@).$(SRCEXT) $(SRCLIB) -o $@

#Non-File Targets
.PHONY: all remake clean cleaner directories

#---------------------------------------------------------------------------------
#Custom targets
#---------------------------------------------------------------------------------

test_matmul: bin/matrix/.tests/tests_matmul
	./bin/matrix/.tests/tests_matmul

test_perf: bin/matrix/.tests/tests_perf
	./bin/matrix/.tests/tests_perf
