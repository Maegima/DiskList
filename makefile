OBJDIR=obj
BINDIR=bin
CC = g++
CFLAGS = -Wall -g
MD5PROJ=md5proj
  
$(BINDIR)/disklist: $(OBJDIR)/disklist.o $(OBJDIR)/FileInfo.o $(OBJDIR)/md5.o $(BINDIR)
	$(CC) $(CFLAGS) -o $(BINDIR)/disklist $(OBJDIR)/disklist.o $(OBJDIR)/FileInfo.o $(OBJDIR)/md5.o

$(OBJDIR)/disklist.o: DiskList.cpp FileInfo.hpp md5/md5.hpp $(OBJDIR)
	$(CC) $(CFLAGS) -c DiskList.cpp -o $(OBJDIR)/disklist.o

$(OBJDIR)/FileInfo.o: FileInfo.cpp FileInfo.hpp $(OBJDIR)
	$(CC) $(CFLAGS) -c FileInfo.cpp -o $(OBJDIR)/FileInfo.o
 
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