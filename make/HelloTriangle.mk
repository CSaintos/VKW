.PHONY: all compile build clean

#? Variables
TARGET_NAME = HelloTriangle
INCLUDES = \
	-Ilib/Vulkan/Include \
	-Iapp/test/lib/GLFW \
	-Iapp/test/lib/glm \
	-Ibuild/vkw/inc \
	-Iapp/test/HelloTriangle
LDFLAGS = \
	-Lbuild/vkw/lib \
	-Lapp/test/lib/GLFW \
	-Llib/Vulkan/Lib
LINKS = \
	-lvkw \
	-llibglfw3 \
	-lgdi32 \
	-lvulkan-1
DEFINES =
SRCDIR = \
	app/test/HelloTriangle
SRCFILES = \
	HelloTriangle.cpp

#* Constants
OBJDIR = bin/$(TARGET_NAME)
TARGETDIR = build/$(TARGET_NAME)
SRCS = $(addprefix $(addsuffix /, $(SRCDIR)), $(SRCFILES))
OBJECTS = $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(SRCFILES)))
TARGET = $(TARGETDIR)/$(TARGET_NAME).exe

#? Custom build cmds
define POSTBUILDCMDS
	@echo Post build commands
	glslc $(SRCDIR)/Base.vert -o $(TARGETDIR)/vert.spv
	glslc $(SRCDIR)/Base.frag -o $(TARGETDIR)/frag.spv
endef

all: $(OBJDIR) $(OBJECTS) $(TARGETDIR) $(TARGET)
	$(POSTBUILDCMDS)

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

# Build target from objects
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(LINKS) -o $(TARGET)
	@echo build

compile: $(OBJDIR) | $(OBJECTS)
build: $(TARGETDIR) | $(TARGET)

clean: 
ifneq ($(wildcard $(OBJDIR)),)
	rmdir /s /q $(subst /,\\,$(OBJDIR))
endif
ifneq ($(wildcard $(TARGETDIR)),)
	rmdir /s /q $(subst /,\\,$(TARGETDIR))
endif
	@echo clean