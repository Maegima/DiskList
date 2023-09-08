OBJDIR=obj
BINDIR=bin
CC = g++
CFLAGS = -Wall -g `pkg-config --cflags --libs gtkmm-4.0`
MD5PROJ=md5proj
  
$(BINDIR)/disklist: $(OBJDIR)/disklist.o $(OBJDIR)/FileInfo.o $(OBJDIR)/md5.o $(OBJDIR)/resources.o $(OBJDIR)/MainWindow.o $(OBJDIR)/Application.o $(BINDIR)
	$(CC) $(CFLAGS) -o $(BINDIR)/disklist $(OBJDIR)/disklist.o $(OBJDIR)/FileInfo.o $(OBJDIR)/md5.o $(OBJDIR)/resources.o $(OBJDIR)/MainWindow.o $(OBJDIR)/Application.o

$(OBJDIR)/disklist.o: DiskList.cpp FileInfo.hpp md5/md5.hpp $(OBJDIR)
	$(CC) $(CFLAGS) -c DiskList.cpp -o $(OBJDIR)/disklist.o

$(OBJDIR)/FileInfo.o: FileInfo.cpp FileInfo.hpp $(OBJDIR)
	$(CC) $(CFLAGS) -c FileInfo.cpp -o $(OBJDIR)/FileInfo.o

$(OBJDIR)/resources.o: resources.c $(OBJDIR)
	glib-compile-resources --target=resources.c --generate-source application.gresource.xml
	$(CC) $(CFLAGS) -c resources.c -o $(OBJDIR)/resources.o

$(OBJDIR)/MainWindow.o: MainWindow.cpp MainWindow.h $(OBJDIR)
	$(CC) $(CFLAGS) -c MainWindow.cpp -o $(OBJDIR)/MainWindow.o

$(OBJDIR)/Application.o: Application.cpp Application.h $(OBJDIR)
	$(CC) $(CFLAGS) -c Application.cpp -o $(OBJDIR)/Application.o

$(OBJDIR)/md5.o: md5/md5.cpp md5/md5.hpp $(OBJDIR) $(MD5PROJ)
	$(CC) $(CFLAGS) -c md5/md5.cpp -o $(OBJDIR)/md5.o

md5/md5.hpp: $(MD5PROJ)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

$(MD5PROJ):
	[ -d "md5" ] || git clone https://github.com/Maegima/md5.git
	cd md5 && git pull