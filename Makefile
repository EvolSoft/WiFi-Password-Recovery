CC=g++
CLIBS=D:\mingw-w64\mingw32\i686-w64-mingw32\lib
IDIR=D:\mingw-w64\mingw32\i686-w64-mingw32\include
WXIDIR=D:\wxMSW-2.8.12\include 
WXMSWDIR=D:\wxMSW-2.8.12\lib\gcc_dll\mswu
WXLDIR=D:\wxMSW-2.8.12\lib\gcc_dll
DEBUG=-g
CFLAGS=-Wall -pipe -mthreads -Wno-unused-local-typedefs -D__GNUWIN32__ -D__WXMSW__ -DWXUSINGDLL -DwxUSE_UNICODE -O2 -I$(IDIR) -I$(WXIDIR) -I$(WXMSWDIR)
RFLAGS=-J rc -O coff -I$(WXIDIR) -I$(WXMSWDIR)
LFLAGS=-L$(WXLDIR) -s -mthreads -mwindows -lwxmsw28u $(CLIBS)/libwlanapi.a $(CLIBS)/libcrypt32.a
del=$(if $(filter $(OS),Windows_NT),del,rm -f)

all: obj/WlanInfo.o obj/WPRApp.o obj/WPRInfoDialog.o obj/WPRMain.o obj/WPRWlanDialog.o obj/pugixml.o obj/resource.res
	$(CC) -o "bin\WiFi Password Recovery.exe" obj/pugixml.o obj/WlanInfo.o obj/WPRApp.o obj/WPRInfoDialog.o obj/WPRMain.o obj/WPRWlanDialog.o obj/resource.res $(LFLAGS)
	
obj/WlanInfo.o: WlanInfo.cpp WlanInfo.h
	$(CC) $(CFLAGS) -c WlanInfo.cpp -o obj/WlanInfo.o

obj/WPRApp.o: WPRApp.cpp WPRApp.h
	$(CC) $(CFLAGS) -c WPRApp.cpp -o obj/WPRApp.o

obj/WPRInfoDialog.o: WPRInfoDialog.cpp WPRInfoDialog.h
	$(CC) $(CFLAGS) -c WPRInfoDialog.cpp -o obj/WPRInfoDialog.o

obj/WPRMain.o: WPRMain.cpp WPRMain.h
	$(CC) $(CFLAGS) -c WPRMain.cpp -o obj/WPRMain.o

obj/WPRWlanDialog.o: WPRWlanDialog.cpp WPRWlanDialog.h
	$(CC) $(CFLAGS) -c WPRWlanDialog.cpp -o obj/WPRWlanDialog.o
	
obj/pugixml.o: pugixml.cpp pugixml.hpp pugiconfig.hpp
	$(CC) $(CFLAGS) -c pugixml.cpp -o obj/pugixml.o
	
obj/resource.res: resource.rc
	windres.exe $(RFLAGS) -i resource.rc -o obj\resource.res

clean:
	@$(del) obj\*.o
	@$(del) obj\*.res
	@$(del) bin\*.exe