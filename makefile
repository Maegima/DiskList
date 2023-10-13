OBJDIR=obj
BINDIR=bin
IMGUIDIR = imgui
EXECUTABLE = $(BINDIR)/disklist

OBJS_FILES = imgui.o imgui_demo.o imgui_draw.o imgui_tables.o imgui_widgets.o
OBJS_FILES += imgui_impl_sdl2.o imgui_impl_opengl3.o
OBJS_FILES += DiskList.o Application.o MainWindow.o Image.o DirectoryEntry.o FileInfo.o md5/md5.o
OBJS_FILES += DBType.o DBInt.o DBText.o DBTypeDict.o DBTable.o FileEntry.o
OBJS=$(OBJS_FILES:%=$(OBJDIR)/%)

CXX = g++
CXXFLAGS = -Wall -g -std=c++20 -I$(IMGUIDIR) -I$(IMGUIDIR)/backends -IApplication
LINUX_GL_LIBS = -lGL
LIBS = -lpthread -ldl -lm -lsqlite3

##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------

## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2
## If you're on a Raspberry Pi and want to use the legacy drivers,
## use the following instead:
# LINUX_GL_LIBS = -L/opt/vc/lib -lbrcmGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) -ldl `sdl2-config --libs`

	CXXFLAGS += `sdl2-config --cflags`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
	LIBS += -L/usr/local/lib -L/opt/local/lib

	CXXFLAGS += `sdl2-config --cflags`
	CXXFLAGS += -I/usr/local/include -I/opt/local/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
    ECHO_MESSAGE = "MinGW"
    LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`

    CXXFLAGS += `pkg-config --cflags sdl2`
    CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(EXECUTABLE): MD5PROJ $(OBJS) $(BINDIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXECUTABLE) $(LIBS)

$(OBJDIR)/%.o: Application/%.cpp $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Application/Windows/%.cpp $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Application/Controllers/%.cpp $(OBJDIR) $(OBJDIR)/md5
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Application/Models/DBType/%.cpp $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Application/Models/DBTable/%.cpp $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(IMGUIDIR)/%.cpp $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(IMGUIDIR)/backends/%.cpp $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/md5:
	mkdir -p $(OBJDIR)/md5

$(BINDIR):
	mkdir -p $(BINDIR)

install:
	[ -d "$(IMGUIDIR)" ] || git clone https://github.com/ocornut/imgui.git
	cd "$(IMGUIDIR)" && git pull

all: $(BINDIR)/$(EXECUTABLE)
	@echo Build complete for $(ECHO_MESSAGE)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

MD5PROJ:
	cd Application/Controllers && [ -d "md5" ] || git clone https://github.com/Maegima/md5.git
	cd Application/Controllers/md5 && git pull