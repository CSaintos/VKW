.PHONY: all post compile build clean

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
	Buffer.cpp \
	Command.cpp \
	Descriptor.cpp \
	Framebuffer.cpp \
	GraphicsPipeline.cpp \
	Instance.cpp \
	LogicalDevice.cpp \
	PhysicalDevice.cpp \
	Presentation.cpp \
	QueueFamilyIndices.cpp \
	RenderPass.cpp \
	Swapchain.cpp \
	Synchronization.cpp \
	Validation.cpp

#* Constants
OBJDIR = bin/$(TARGET_NAME)
TARGETDIR = build/$(TARGET_NAME)
TARGETLIBDIR = $(TARGETDIR)/lib
SRCS = $(addprefix $(addsuffix /, $(SRCDIR)), $(SRCFILES))
OBJECTS = $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(SRCFILES)))
TARGET = $(TARGETLIBDIR)/$(TARGET_NAME).lib

#? Custom build cmds
define POSTBUILDCMDS
	@echo copying vkw/inc
	@xcopy app\\inc build\\vkw\\inc /E /I /Y /Q
endef

all: $(TARGETLIBDIR) $(TARGET)
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
$(TARGETLIBDIR):
ifeq ($(wildcard $(TARGETLIBDIR)),)
	@mkdir $(subst /,\\,$(TARGETLIBDIR))
	@echo create build directory
endif

# Build target from objects
$(TARGET): $(OBJECTS)
	@$(AR) -rcs $(TARGET) $(OBJECTS)
	@echo build

# force compile
compile: $(OBJDIR) | $(OBJECTS)
# force build
build: $(TARGETLIBDIR) | $(TARGET)

clean:
ifneq ($(wildcard $(OBJDIR)),)
	rmdir /s /q $(subst /,\\,$(OBJDIR))
endif
ifneq ($(wildcard $(TARGETDIR)),)
	rmdir /s /q $(subst /,\\,$(TARGETDIR))
endif
	@echo clean