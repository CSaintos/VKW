
#? Variables
TARGET_NAME = vkw
INCLUDES = \
	-Ilib/Vulkan/Include \
	-Iapp/inc
LDFLAGS = 
LINKS = 
DEFINES = 
SRCDIR = \
	app/src
SRCFILES = \
	PhysicalDevice.cpp

#* Constants
OBJDIR = bin/$(TARGET_NAME)
TARGETDIR = build/$(TARGET_NAME)
SRCS = $(addprefix $(addsuffix /, $(SRCDIR)), $(SRCFILES))
OBJECTS = $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(SRCFILES)))
TARGET = $(TARGETDIR)/$(TARGET_NAME).lib

#? Custom build cmds
define POSTBUILDCMDS
endef

all: $(OBJDIR) $(OBJECTS)

# Create bin directory
$(OBJDIR):
ifeq ($(wildcard $(OBJDIR)),)
	@mkdir $(subst /,\\,$(OBJDIR))
	@echo create bin directory
endif

# Compile sources into objects
$(OBJECTS): $(SRCS)
	@$(CXX) $(INCLUDES) -c $(SRCS) -o $(OBJECTS)
	@echo compile

# Create build directory
$(TARGETDIR):
ifeq ($(wildcard $(TARGETDIR)),)
	@mkdir $(subst /,\\,$(TARGETDIR))
	@echo create build directory
endif

# force compile
compile: $(OBJDIR) | $(OBJECTS)
# force build
build: $(TARGETDIR) | $(TARGET)

clean:
ifneq ($(wildcard $(OBJDIR)),)
	rmdir /s /q $(subst /,\\,$(OBJDIR))
endif
ifneq ($(wildcard $(TARGETDIR)),)
	rmdir /s /q $(subst /,\\,$(TARGETDIR))
endif
	@echo clean