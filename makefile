OBJDIR=obj
BINDIR=bin
EXECUTABLE = $(BINDIR)/disklist

OBJS_FILES = DiskList.o Application.o Image.o FileInfo.o md5/md5.o
OBJS_FILES += MainWindow.o ListingWindow.o CardPanel.o
OBJS_FILES += DBType.o DBInt.o DBText.o DBTypeDict.o DBTable.o FileEntry.o
OBJS=$(OBJS_FILES:%=$(OBJDIR)/%)

CXX = g++
CXXFLAGS = -Wall -g -std=c++20 -IApplication `wx-config --cxxflags --libs`
LINUX_GL_LIBS = -lGL
LIBS = -lpthread -ldl -lm -lsqlite3

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(EXECUTABLE): $(OBJS) $(BINDIR)
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
 
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/md5:
	mkdir -p $(OBJDIR)/md5

$(BINDIR):
	mkdir -p $(BINDIR)

install:
	cd Application/Controllers && [ -d "md5" ] || git clone https://github.com/Maegima/md5.git
	cd Application/Controllers/md5 && git pull

all: $(BINDIR)/$(EXECUTABLE)
	@echo Build complete for $(ECHO_MESSAGE)

clean:
	rm -rf $(OBJDIR) $(BINDIR)