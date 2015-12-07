#############################################################################
# Makefile for building: Photostovis
# Generated by qmake (2.01a) (Qt 4.8.6) on: Tue Dec 8 00:24:19 2015
# Project:  Photostovis.pro
# Template: app
# Command: /usr/lib/i386-linux-gnu/qt4/bin/qmake -o Makefile Photostovis.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = 
CFLAGS        = -pipe -O2 -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)    
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/lib/i386-linux-gnu/qt4/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.c \
		SyncManager.c \
		client.c \
		sha256.c \
		FileManager.c \
		boyer_moore.c \
		error_mgmt.c \
		scrambler.c \
		encrypt_main.c 
OBJECTS       = main.o \
		SyncManager.o \
		client.o \
		sha256.o \
		FileManager.o \
		boyer_moore.o \
		error_mgmt.o \
		scrambler.o \
		encrypt_main.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		deployment.pri \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		Photostovis.pro
QMAKE_TARGET  = Photostovis
DESTDIR       = 
TARGET        = Photostovis

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: Photostovis.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		deployment.pri \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf
	$(QMAKE) -o Makefile Photostovis.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
deployment.pri:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/shared.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
qmake:  FORCE
	@$(QMAKE) -o Makefile Photostovis.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/Photostovis1.0.0 || $(MKDIR) .tmp/Photostovis1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/Photostovis1.0.0/ && (cd `dirname .tmp/Photostovis1.0.0` && $(TAR) Photostovis1.0.0.tar Photostovis1.0.0 && $(COMPRESS) Photostovis1.0.0.tar) && $(MOVE) `dirname .tmp/Photostovis1.0.0`/Photostovis1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/Photostovis1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

main.o: main.c SyncManager.h \
		FileManager.h \
		sha256.h \
		client.h \
		encrypt_main.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o main.o main.c

SyncManager.o: SyncManager.c client.h \
		SyncManager.h \
		encrypt_main.h \
		FileManager.h \
		sha256.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o SyncManager.o SyncManager.c

client.o: client.c client.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o client.o client.c

sha256.o: sha256.c sha256.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o sha256.o sha256.c

FileManager.o: FileManager.c FileManager.h \
		sha256.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o FileManager.o FileManager.c

boyer_moore.o: boyer_moore.c boyer_moore.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o boyer_moore.o boyer_moore.c

error_mgmt.o: error_mgmt.c error_mgmt.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o error_mgmt.o error_mgmt.c

scrambler.o: scrambler.c scrambler.h \
		error_mgmt.h \
		boyer_moore.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o scrambler.o scrambler.c

encrypt_main.o: encrypt_main.c scrambler.h \
		error_mgmt.h \
		boyer_moore.h \
		encrypt_main.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o encrypt_main.o encrypt_main.c

####### Install

install_target: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/opt/Photostovis/bin/ || $(MKDIR) $(INSTALL_ROOT)/opt/Photostovis/bin/ 
	-$(INSTALL_PROGRAM) "$(QMAKE_TARGET)" "$(INSTALL_ROOT)/opt/Photostovis/bin/$(QMAKE_TARGET)"
	-$(STRIP) "$(INSTALL_ROOT)/opt/Photostovis/bin/$(QMAKE_TARGET)"

uninstall_target:  FORCE
	-$(DEL_FILE) "$(INSTALL_ROOT)/opt/Photostovis/bin/$(QMAKE_TARGET)"
	-$(DEL_DIR) $(INSTALL_ROOT)/opt/Photostovis/bin/ 


install:  install_target  FORCE

uninstall: uninstall_target   FORCE

FORCE:

