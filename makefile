OBJDIR=obj
BINDIR=bin
CC = g++
CFLAGS = -Wall -g `pkg-config --cflags --libs gtkmm-4.0`

SOBJS=resources.o DiskList.o Application.o MainWindow.o FileInfo.o md5.o

OBJS=$(SOBJS:%=$(OBJDIR)/%)

$(BINDIR)/disklist: MD5PROJ RESOURCES $(BINDIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BINDIR)/disklist

$(OBJDIR)/%.o: Application/%.c $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Application/%.cpp $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Application/Windows/%.cpp $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Application/Controllers/%.cpp $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: Application/Controllers/md5/%.cpp $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

RESOURCES: 
	glib-compile-resources --target=Application/resources.c --generate-source Application/application.gresource.xml

MD5PROJ:
	[ -d "md5" ] || git clone https://github.com/Maegima/md5.git
	cd md5 && git pull