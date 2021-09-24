#-------------------------------------------------------------------------------
# Makefile  for FSFW Example
#-------------------------------------------------------------------------------
#		User-modifiable options
#-------------------------------------------------------------------------------
# Fundamentals on the build process of C/C++ Software:
# https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

# Make documentation: https://www.gnu.org/software/make/manual/make.pdf
# Online: https://www.gnu.org/software/make/manual/make.html
# General rules: http://make.mad-scientist.net/papers/rules-of-makefiles/#rule3
SHELL = /bin/sh

FRAMEWORK_PATH = fsfw
MISSION_PATH = mission
CONFIG_PATH = bsp_linux/fsfwconfig
TEST_PATH = 
# Board support package path. This will generally contain code which is 
# not portable and depends on the hardware.
BSP_PATH = bsp_linux
BASE_NAME = fsfw-example

OS_FSFW = linux
BOARD = linux
OUTPUT_FOLDER = $(BOARD)

BINARY_NAME = $(BASE_NAME)-$(BOARD)

# Default debug level (for full debugging information)
DEBUG_LEVEL = -g3

# Default optimization level. 
# 1. -O0 default for debugging
# 2. -Os for size
# 3. -O3/-O2 for speed and size.
OPTIMIZATION = -O0

# Output directories
BUILDPATH = _bin
DEPENDPATH = _dep
OBJECTPATH = _obj

ifeq ($(MAKECMDGOALS),release)
BUILD_FOLDER = release
else 
BUILD_FOLDER = debug
endif

DEPENDDIR = $(DEPENDPATH)/$(OUTPUT_FOLDER)/$(BUILD_FOLDER)
OBJDIR = $(OBJECTPATH)/$(OUTPUT_FOLDER)/$(BUILD_FOLDER)
BINDIR = $(BUILDPATH)/$(OUTPUT_FOLDER)/$(BUILD_FOLDER)

CLEANDEP = $(DEPENDPATH)/$(OUTPUT_FOLDER)
CLEANOBJ = $(OBJECTPATH)/$(OUTPUT_FOLDER)
CLEANBIN = $(BUILDPATH)

#-------------------------------------------------------------------------------
#		Tools and Includes
#-------------------------------------------------------------------------------
CROSS_COMPILE =

# c++ compiler
CXX := $(CROSS_COMPILE)g++
# c compiler
CC := $(CROSS_COMPILE)gcc
SIZE := $(CROSS_COMPILE)size

# Empty initialization is important for submakefile handling!
CXXSRC := 
CSRC := 
INCLUDES := 


# Directries where $(directoryname).mk files should be included from
SUBDIRS := $(BSP_PATH) $(TEST_PATH) $(CONFIG_PATH) $(MISSION_PATH) \
		$(FRAMEWORK_PATH) 


# This is a hack from http://make.mad-scientist.net/the-eval-function/
#
# The problem is, that included makefiles should be aware of their relative path
# but not need to guess or hardcode it. So we set $(CURRENTPATH) for them. If
# we do this globally and the included makefiles want to include other makefiles as
# well, they would overwrite $(CURRENTPATH), screwing the include after them.
#
# By using a for-loop with an eval'd macro, we can generate the code to include all
# sub-makefiles (with the correct $(CURRENTPATH) set) before actually evaluating
# (and by this possibly changing $(CURRENTPATH)) them.
#
# This works recursively, if an included makefile wants to include, it can safely set 
# $(SUBDIRS) (which has already been evaluated here) and do
# "$(foreach S,$(SUBDIRS),$(eval $(INCLUDE_FILE)))"
# $(SUBDIRS) must be relative to the project root, so to include subdir foo, set
# $(SUBDIRS) = $(CURRENTPATH)/foo.
define INCLUDE_FILE
CURRENTPATH := $S
include $(S)/$(notdir $S).mk
endef
$(foreach S,$(SUBDIRS),$(eval $(INCLUDE_FILE)))

# Include all includes defined as INCLUDES=...
I_INCLUDES += $(addprefix -I, $(INCLUDES))

# Debug Info
# $(info $${I_INCLUDES} is [${I_INCLUDES}])

#-------------------------------------------------------------------------------
#		Source Files
#-------------------------------------------------------------------------------
# Additional source files which were not includes by other .mk
# files are added here.
# To help Makefile find source files, the source location paths
# can be added by using the VPATH variable
# See: https://www.gnu.org/software/make/manual/html_node/General-Search.html
# It is recommended to only use VPATH to add source locations
# See: http://make.mad-scientist.net/papers/how-not-to-use-vpath/

# Please ensure that no files are included by both .mk file and here !

# All C Sources included by .mk files are assigned here
# Add the objects to sources so dependency handling works
C_OBJECTS += $(CSRC:.c=.o)

# Objects built from Assembly source files
ASM_OBJECTS = $(ASRC:.S=.o)

# Objects built from C++ source files
CXX_OBJECTS +=  $(CXXSRC:.cpp=.o)

#-------------------------------------------------------------------------------
#		Build Configuration + Output
#-------------------------------------------------------------------------------
WARNING_FLAGS = -Wall -Wshadow=local -Wextra -Wimplicit-fallthrough=1 \
		-Wno-unused-parameter 
CUSTOM_DEFINES += -DAPI=LINUX -DLINUX=2
# Flags for both c++ and c
CXXFLAGS += -I.  $(OPTIMIZATION) $(DEBUG_LEVEL) $(DEPFLAGS) $(WARNING_FLAGS) \
		-fmessage-length=0 $(CUSTOM_DEFINES) $(I_INCLUDES)
# Flags for c only
CFLAGS +=
# Flags for c++ only
CPPFLAGS += -std=c++11 -fno-exceptions 

# For debugging.
# $(info $${CXXFLAGS} is [${CXXFLAGS}])	
# $(info $${CFLAGS} is [${CFLAGS}])	
# $(info $${CPPFLAGS} is [${CPPFLAGS}])	


# See: https://stackoverflow.com/questions/6687630/how-to-remove-unused-c-c-symbols-with-gcc-and-ld
# Used to throw away unused code. Reduces code size significantly !
# -Wl,--gc-sections: needs to be passed to the linker to discard unused sections
# KEEP_UNUSED_CODE = 1
ifdef KEEP_UNUSED_CODE
PROTOTYPE_OPTIMIZATION = 
UNUSED_CODE_REMOVAL = 
else
PROTOTYPE_OPTIMIZATION = -ffunction-sections -fdata-sections
UNUSED_CODE_REMOVAL = -Wl,--gc-sections
# Link time optimization
# See https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html for reference
# Link time is larger and size of object files can not be retrieved
# but resulting binary is smaller. Could be used in mission/deployment build
# Requires -ffunction-section in linker call
LINK_TIME_OPTIMIZATION = -flto
OPTIMIZATION += $(PROTOTYPE_OPTIMIZATION)
endif 

# Dependency Flags
# These flags tell the compiler to build dependencies
# See: https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html
# Using following guide: 
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPENDDIR)/$*.d

LINKFLAGS = $(DEBUG_LEVEL) $(UNUSED_CODE_REMOVAL) $(OPTIMIZATION) -pthread 
LINKLIB = -lrt

TARGET = Debug
OPTIMIZATION_MESSAGE = Off
DEBUG_MESSAGE = Off\(no -DDEBUG\)

# Define Messages
MSG_INFO = Software: Hosted Flight Software Framework \(FSFW\) example.
MSG_TARGET = Target Build: $(TARGET)
MSG_DEPENDENCY = Collecting dependencies for:
MSG_COMPILING = Compiling:
MSG_LINKING = Linking:
MSG_DEBUG = Debug level: $(DEBUG_LEVEL), FSFW Debugging: $(DEBUG_MESSAGE)
			
MSG_OPTIMIZATION = Optimization: $(OPTIMIZATION), $(OPTIMIZATION_MESSAGE)

#-------------------------------------------------------------------------------
#		Rules
#-------------------------------------------------------------------------------
# Makefile rules: https://www.gnu.org/software/make/manual/html_node/Rules.html
# This is the primary section which defines the ruleset to build
# the executable from the sources.

# Default target
default: all

all: debug

# building the different builds.
release debug: executable

# Target specific variables for release build.
release: OPTIMIZATION = -O2
release: DEBUG_LEVEL = -g0
release: TARGET = Release
release: OPTIMIZATION_MESSAGE = On

# Target specific variables for debug build.
debug: CXXDEFINES += -DDEBUG
debug: DEBUG_MESSAGE = On\(-DDEBUG\)

# Cleans all files 
hardclean: 
	-rm -rf $(BUILDPATH)
	-rm -rf $(OBJECTPATH)
	-rm -rf $(DEPENDPATH)

# Only clean files for current build
clean:
	-rm -rf $(CLEANOBJ)
	-rm -rf $(CLEANBIN)
	-rm -rf $(CLEANDEP)
	
# Only clean binaries. Useful for changing the binary type when object 
# files are already compiled so complete rebuild is not necessary
cleanbin:
	-rm -rf $(BUILDPATH)/$(OUTPUT_FOLDER)

executable: $(BINDIR)/$(BINARY_NAME).elf
	@echo
	@echo $(MSG_INFO)
	@echo $(MSG_TARGET)
	@echo $(MSG_OPTIMIZATION)
	@echo $(MSG_DEBUG)

# For debugging.
# $(info $${C_OBJECTS} is [${C_OBJECTS}])	
# $(info $${CXX_OBJECTS} is [${CXX_OBJECTS}])	

C_OBJECTS_PREFIXED = $(addprefix $(OBJDIR)/, $(C_OBJECTS))
CXX_OBJECTS_PREFIXED = $(addprefix $(OBJDIR)/, $(CXX_OBJECTS))
ASM_OBJECTS_PREFIXED = $(addprefix $(OBJDIR)/, $(ASM_OBJECTS))
ALL_OBJECTS_PREFIXED = $(ASM_OBJECTS_PREFIXED) $(C_OBJECTS_PREFIXED) \
					   $(CXX_OBJECTS_PREFIXED) 

# Useful for debugging the Makefile
# Also see: https://www.oreilly.com/openbook/make3/book/ch12.pdf
# $$(info $${ALL_OBJECTS_PREFIXED} is [${ALL_OBJECTS_PREFIXED}])

# Automatic variables are used here extensively. Some of them
# are escaped($$) to suppress immediate evaluation. The most important ones are:
# $@: Name of Target (left side of rule)
# $<: Name of the first prerequisite (right side of rule)
# @^: List of all prerequisite, omitting duplicates
# @D: Directory and file-within-directory part of $@

# Generates binary and displays all build properties
# -p with mkdir ignores error and creates directory when needed.

# SHOW_DETAILS = 1

$(BINDIR)/$(BINARY_NAME).elf: $(ALL_OBJECTS_PREFIXED)
	@echo $(MSG_LINKING) Target $@
	@mkdir -p $(@D)
ifdef SHOW_DETAILS
	$(CXX) $(LINKFLAGS) $(LINK_INCLUDES) -o $@ $^ $(LINKLIB)
else
	@$(CXX) $(LINKFLAGS) $(LINK_INCLUDES) -o $@ $^ $(LINKLIB)
endif
ifeq ($(BUILD_FOLDER), release)
# With Link Time Optimization, section size is not available
	$(SIZE) $@
else
	@$(SIZE) $^ $@
endif


$(OBJDIR)/%.o: %.cpp
$(OBJDIR)/%.o: %.cpp $(DEPENDDIR)/%.d | $(DEPENDDIR)
	@echo 
	@echo $(MSG_COMPILING) $<
	@mkdir -p $(@D)
ifdef SHOW_DETAILS
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
else
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
endif


$(OBJDIR)/%.o: %.c 
$(OBJDIR)/%.o: %.c $(DEPENDDIR)/%.d | $(DEPENDDIR) 
	@echo 
	@echo $(MSG_COMPILING) $<
	@mkdir -p $(@D)
ifdef SHOW_DETAILS
	$(CC) $(CXXFLAGS) $(CFLAGS) -c -o $@ $<
else
	@$(CC) $(CXXFLAGS) $(CFLAGS) -c -o $@ $<
endif
#-------------------------------------------------------------------------------
#		Dependency Handling
#-------------------------------------------------------------------------------

# Dependency Handling according to following guide:
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
$(DEPENDDIR):
	@mkdir -p $(@D)
DEPENDENCY_RELATIVE = $(CSRC:.c=.d) $(CXXSRC:.cpp=.d)
# This is the list of all dependencies
DEPFILES = $(addprefix $(DEPENDDIR)/, $(DEPENDENCY_RELATIVE))
# Create subdirectories for dependencies
$(DEPFILES):
	@mkdir -p $(@D)
# Include all dependencies
include $(wildcard $(DEPFILES))


.PHONY: release debug hardclean clean binclean all

