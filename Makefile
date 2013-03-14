CC           = g++
CCFLAGS      = -Wall -ggdb 
LDFLAGS      = -Lusr/log4cpp/lib -llog4cpp -L/home/bweel/lib -lSDL_image -lSDL_ttf -lSDL 
RM           = rm -f 
MAKE         = make
#
# Specify the GSL home directory, such that it can be included 
#
GSL_HOME	 = /usr/lib/libgsl

#
# This path has been changed to include the liolib headers
#
INCLUDE      = -I$(GSL_HOME)/include -Iinclude/ext -Iinclude/contrib -Iinclude/core -Iprj/ -Iinclude/contrib/LioLib/lioutilities -Iinclude/contrib/LioLib/lioneuralnet -Iinclude/contrib/LioLib/liobehaviorlib -Iinclude/contrib/LioLib/liogenetics 
SRC_C	     = $(wildcard src/core/*.c src/contrib/*.c src/ext/*.c prj/**/src/*.c)

#
# This path has been changed to  compile all sources in the contrib and ext subdirectories
#
SRC_CC	     = $(wildcard src/core/*.cpp src/contrib/*.cpp src/contrib/**/*.cpp include/contrib/LioLib/liobehaviorlib/src/*.cpp\
include/contrib/LioLib/lioneuralnet/src/*.cpp include/contrib/LioLib/lioutilities/src/*.cpp src/ext/*.cpp src/ext/**/*.cpp prj/**/src/*.cpp)
INCLUDE_H = $(wildcard include/core/**/*.h include/contrib/*.cpp include/contrib/**/*.h include/contrib/LioLib/liobehaviorlib/behaviors/*.h\
include/contrib/LioLib/lioneuralnet/**/*.h include/contrib/LioLib/lioutilities/**/*.h include/ext/**/*.h include/ext/*.h prj/**/include/*.h)
OBJ          = $(SRC_C:.c=.o) $(SRC_CC:.cpp=.o)
TARGET       = roborobo

.IGNORE: 
#.SILENT: 

all: 
	#clear
#	$(MAKE) distclean
	$(MAKE) $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
#	$(MAKE) clean

%.o: %.cpp
	$(CC) -o $@ -c $< $(CCFLAGS) $(INCLUDE)

%.o: %.c
	$(CC) -o $@ -c $< $(CCFLAGS) $(INCLUDE)

clean:
	$(RM) $(OBJ) 

distclean:
	$(MAKE) clean 
	$(RM) $(TARGET) 

snapshot:
	clear
	make distclean
	rm -f roborobo-snapshot.tgz
	tar --exclude='._*' --exclude='build' --exclude='.DS_Store' --exclude='datalog_*' --exclude='properties_*' --exclude='roborobo-snapshot.tgz' --exclude='.svn' -cvf - ../`pwd | perl -ne '/.*\/(.*)$$/; print "$$1\n"';`/* | gzip -c > roborobo-snapshot.tgz
	@echo =-=-=-= 
	@echo =-=-=-=
	@echo =-=-=-=
	@echo Snapshot is created as roborobo-snapshot.tgz
	@echo .
	@echo All files have been archived except:
	@echo - datalog_* and properties_* files are NOT in the archive, wherever they were.
	@echo - .svn directories and contents are NOT in the archive
	@echo - macosx only: the macosx build directory is NOT in the archive
	@echo - macosx only: after uncompressing, re-create the symbolic link in the build/Debug dir.
	@echo - note: dont forget to re-compile as a make distclean was performed prior to building the archive
	@echo =-=-=-=
	@echo =-=-=-=
	@echo =-=-=-=

	

