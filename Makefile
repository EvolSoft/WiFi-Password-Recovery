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

all: obj/WPRApp.o obj/WPRMain.o obj/NetworkData.o obj/NetworkDataFormat1.o obj/AboutDialog.o obj/LanguageHelper.o obj/NetworkInfoDialog.o obj/NetworkWizard.o obj/WelcomeDialog.o obj/WlanInfo.o obj/XMLHelper.o obj/resource.res
	xgettext -k_ -o "WiFi Password Recovery.pot" AboutDialog.cpp NetworkInfoDialog.cpp NetworkWizard.cpp WelcomeDialog.cpp WlanInfo.cpp WPRApp.cpp WPRMain.cpp
	$(CC) -o "bin\WiFi Password Recovery.exe" obj/WPRApp.o obj/WPRMain.o obj/NetworkData.o obj/NetworkDataFormat1.o obj/AboutDialog.o obj/LanguageHelper.o obj/NetworkInfoDialog.o obj/NetworkWizard.o obj/WelcomeDialog.o obj/WlanInfo.o obj/XMLHelper.o obj/resource.res $(LFLAGS)
	
obj/NetworkData.o: NetworkData/NetworkData.cpp NetworkData/NetworkData.h
	$(CC) $(CFLAGS) -c NetworkData/NetworkData.cpp -o obj/NetworkData.o
	
obj/NetworkDataFormat1.o: NetworkData/NetworkDataFormat1.cpp NetworkData/NetworkDataFormat1.h
	$(CC) $(CFLAGS) -c NetworkData/NetworkDataFormat1.cpp -o obj/NetworkDataFormat1.o
	
obj/AboutDialog.o: AboutDialog.cpp AboutDialog.h
	$(CC) $(CFLAGS) -c AboutDialog.cpp -o obj/AboutDialog.o
	
obj/LanguageHelper.o: LanguageHelper.cpp LanguageHelper.h
	$(CC) $(CFLAGS) -c LanguageHelper.cpp -o obj/LanguageHelper.o
	
obj/NetworkInfoDialog.o: NetworkInfoDialog.cpp NetworkInfoDialog.h
	$(CC) $(CFLAGS) -c NetworkInfoDialog.cpp -o obj/NetworkInfoDialog.o

obj/NetworkWizard.o: NetworkWizard.cpp NetworkWizard.h
	$(CC) $(CFLAGS) -c NetworkWizard.cpp -o obj/NetworkWizard.o
	
obj/WelcomeDialog.o: WelcomeDialog.cpp WelcomeDialog.h
	$(CC) $(CFLAGS) -c WelcomeDialog.cpp -o obj/WelcomeDialog.o
	
obj/WlanInfo.o: WlanInfo.cpp WlanInfo.h
	$(CC) $(CFLAGS) -c WlanInfo.cpp -o obj/WlanInfo.o

obj/WPRApp.o: WPRApp.cpp WPRApp.h
	$(CC) $(CFLAGS) -c WPRApp.cpp -o obj/WPRApp.o

obj/WPRMain.o: WPRMain.cpp WPRMain.h
	$(CC) $(CFLAGS) -c WPRMain.cpp -o obj/WPRMain.o

obj/XMLHelper.o: XMLHelper.cpp XMLHelper.h
	$(CC) $(CFLAGS) -c XMLHelper.cpp -o obj/XMLHelper.o
	
obj/resource.res: resource.rc
	windres.exe $(RFLAGS) -i resource.rc -o obj\resource.res

clean:
	@$(del) obj\*.o
	@$(del) obj\*.res
	@$(del) bin\*.exe