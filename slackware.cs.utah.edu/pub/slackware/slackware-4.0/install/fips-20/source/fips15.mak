#
# Borland C++ IDE generated makefile
#
.AUTODEPEND


#
# Borland C++ tools
#
IMPLIB  = Implib
BCCDOS  = Bcc +BccDos.cfg 
TLINK   = TLink
TLIB    = TLib
TASM    = Tasm
#
# IDE macros
#


#
# Options
#
IDE_LFLAGSDOS =  -LD:\BC4\LIB
IDE_BFLAGS = 
LLATDOS_fips15dexe =  -c -Tde -LG:\BC4\LIB
RLATDOS_fips15dexe =  -IG:\BC4\INCLUDE
BLATDOS_fips15dexe = 
LEAT_fips15dexe = $(LLATDOS_fips15dexe)
REAT_fips15dexe = $(RLATDOS_fips15dexe)
BEAT_fips15dexe = $(BLATDOS_fips15dexe)

#
# Dependency List
#
Dep_fips15 = \
   fips15.exe

fips15 : BccDos.cfg $(Dep_fips15)
  echo MakeNode fips15

Dep_fips15dexe = \
   calculat.obj\
   check.obj\
   cmdl_arg.obj\
   disk_io.obj\
   fat.obj\
   fipsspec.obj\
   getopt.obj\
   global.obj\
   hdstruct.obj\
   host_os.obj\
   input.obj\
   logdr_st.obj\
   main.obj\
   save.obj

fips15.exe : $(Dep_fips15dexe)
  $(TLINK)   @&&|
 /v $(IDE_LFLAGSDOS) $(LEAT_fips15dexe) +
G:\BC4\LIB\c0l.obj+
calculat.obj+
check.obj+
cmdl_arg.obj+
disk_io.obj+
fat.obj+
fipsspec.obj+
getopt.obj+
global.obj+
hdstruct.obj+
host_os.obj+
input.obj+
logdr_st.obj+
main.obj+
save.obj
$<,$*
G:\BC4\LIB\emu.lib+
G:\BC4\LIB\mathl.lib+
G:\BC4\LIB\cl.lib

|

calculat.obj :  calculat.cpp
  $(BCCDOS) -c $(CEAT_fips15dexe) -o$@ calculat.cpp

check.obj :  check.cpp
  $(BCCDOS) -c $(CEAT_fips15dexe) -o$@ check.cpp

cmdl_arg.obj :  cmdl_arg.cpp
  $(BCCDOS) -c $(CEAT_fips15dexe) -o$@ cmdl_arg.cpp

disk_io.obj :  disk_io.cpp
  $(BCCDOS) -c $(CEAT_fips15dexe) -o$@ disk_io.cpp

fat.obj :  fat.cpp
  $(BCCDOS) -c $(CEAT_fips15dexe) -o$@ fat.cpp

fipsspec.obj :  fipsspec.cpp
  $(BCCDOS) -c $(CEAT_fips15dexe) -o$@ fipsspec.cpp

getopt.obj :  getopt.c
  $(BCCDOS) -P- -c $(CEAT_fips15dexe) -o$@ getopt.c

global.obj :  global.cpp
  $(BCCDOS) -c $(CEAT_fips15dexe) -o$@ global.cpp

hdstruct.obj :  hdstruct.cpp
  $(BCCDOS) -c $(CEAT_fips15dexe) -o$@ hdstruct.cpp

host_os.obj :  host_os.cpp
  $(BCCDOS) -c $(CEAT_fips15dexe) -o$@ host_os.cpp

input.obj :  input.cpp
  $(BCCDOS) -c $(CEAT_fips15dexe) -o$@ input.cpp

logdr_st.obj :  logdr_st.cpp
  $(BCCDOS) -c $(CEAT_fips15dexe) -o$@ logdr_st.cpp

main.obj :  main.cpp
  $(BCCDOS) -c $(CEAT_fips15dexe) -o$@ main.cpp

save.obj :  save.cpp
  $(BCCDOS) -c $(CEAT_fips15dexe) -o$@ save.cpp

# Compiler configuration file
BccDos.cfg : 
   Copy &&|
-W-
-R
-v
-vi
-X-
-H
-ID:\BC4\INCLUDE
-H=fips15.csm
-ml
-f
-IG:\BC4\INCLUDE
| $@


