CXX = g++
CFLAGs = -Wall -g
TARGET = build/vkw/lib/vkw.lib
INCLUDES += -Iapp/inc -Ilib/Vulkan/Include
OBJDIR = bin
SRCDIR = app/src

all: clean directory compile static_build obj_clean

compile:
	@echo compile
	$(CXX) $(INCLUDES) -c $(SRCDIR)/Buffer.cpp -o $(OBJDIR)/Buffer.o $(CFLAGS)
	$(CXX) $(INCLUDES) -c $(SRCDIR)/Command.cpp -o $(OBJDIR)/Command.o $(CFLAGS)
	$(CXX) $(INCLUDES) -c $(SRCDIR)/Descriptor.cpp -o $(OBJDIR)/Descriptor.o $(CFLAGS)
	$(CXX) $(INCLUDES) -c $(SRCDIR)/Framebuffer.cpp -o $(OBJDIR)/Framebuffer.o $(CFLAGS)
	$(CXX) $(INCLUDES) -c $(SRCDIR)/GraphicsPipeline.cpp -o $(OBJDIR)/GraphicsPipeline.o $(CFLAGS)
	$(CXX) $(INCLUDES) -c $(SRCDIR)/Instance.cpp -o $(OBJDIR)/Instance.o $(CFLAGS)
	$(CXX) $(INCLUDES) -c $(SRCDIR)/LogicalDevice.cpp -o $(OBJDIR)/LogicalDevice.o $(CFLAGS)
	$(CXX) $(INCLUDES) -c $(SRCDIR)/PhysicalDevice.cpp -o $(OBJDIR)/PhysicalDevice.o $(CFLAGS)
	$(CXX) $(INCLUDES) -c $(SRCDIR)/Presentation.cpp -o $(OBJDIR)/Presentation.o $(CFLAGS)
	$(CXX) $(INCLUDES) -c $(SRCDIR)/QueueFamilyIndices.cpp -o $(OBJDIR)/QueueFamilyIndices.o $(CFLAGS)
	$(CXX) $(INCLUDES) -c $(SRCDIR)/RenderPass.cpp -o $(OBJDIR)/RenderPass.o $(CFLAGS)
	$(CXX) $(INCLUDES) -c $(SRCDIR)/Swapchain.cpp -o $(OBJDIR)/Swapchain.o $(CFLAGS)
	$(CXX) $(INCLUDES) -c $(SRCDIR)/Synchronization.cpp -o $(OBJDIR)/Synchronization.o $(CFLAGS)
	$(CXX) $(INCLUDES) -c $(SRCDIR)/Validation.cpp -o $(OBJDIR)/Validation.o $(CFLAGS)

static_build:
	$(AR) -rcs $(TARGET) $(OBJDIR)/Buffer.o
	$(AR) -rcs $(TARGET) $(OBJDIR)/Command.o
	$(AR) -rcs $(TARGET) $(OBJDIR)/Descriptor.o
	$(AR) -rcs $(TARGET) $(OBJDIR)/Framebuffer.o
	$(AR) -rcs $(TARGET) $(OBJDIR)/GraphicsPipeline.o
	$(AR) -rcs $(TARGET) $(OBJDIR)/Instance.o
	$(AR) -rcs $(TARGET) $(OBJDIR)/LogicalDevice.o
	$(AR) -rcs $(TARGET) $(OBJDIR)/PhysicalDevice.o
	$(AR) -rcs $(TARGET) $(OBJDIR)/Presentation.o
	$(AR) -rcs $(TARGET) $(OBJDIR)/QueueFamilyIndices.o
	$(AR) -rcs $(TARGET) $(OBJDIR)/RenderPass.o
	$(AR) -rcs $(TARGET) $(OBJDIR)/Swapchain.o
	$(AR) -rcs $(TARGET) $(OBJDIR)/Synchronization.o
	$(AR) -rcs $(TARGET) $(OBJDIR)/Validation.o

clean:
	@echo clean
	@rmdir /s /q build\\vkw\\lib

obj_clean:
	@echo obj_clean
	@del bin\\*.o

directory:
	@echo directory
	@if not exist build\\vkw\\lib mkdir build\\vkw\\lib