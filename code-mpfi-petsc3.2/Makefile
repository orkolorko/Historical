#If you ever happen to want to link against installed libraries
#in a given directory, LIBDIR, you must either use libtool, and
#specify the full pathname of the library, or use the `-LLIBDIR'
#flag during linking and do at least one of the following:
#   - add LIBDIR to the `LD_LIBRARY_PATH' environment variable
#     during execution
#   - add LIBDIR to the `LD_RUN_PATH' environment variable
#     during linking
#   - use the `-Wl,-rpath -Wl,LIBDIR' linker flag
#   - have your system administrator add LIBDIR to `/etc/ld.so.conf'

ALL: genmatrix eigenerr markovize
CFLAGS= -g -O0
CPPFLAGS =
CLEANFILES =

PETSC_INCL=-I${PETSC_DIR}/include
MPFI_INCL=-I${MPFI_DIR}/include -DGMP -Xlinker -rpath -Xlinker ${MPFI_DIR}/lib \
-I${MPFI_DIR}/include -L${MPFI_DIR}/lib -lmpfi -lmpfr -lgomp 

IRRAM_INCL=-I${IRRAM_DIR}/include -DGMP -Xlinker -rpath -Xlinker ${MPFI_DIR}/lib \
-I${MPFI_DIR}/include -L${MPFI_DIR}/lib -lmpfi -lmpfr -lgomp

include ${PETSC_DIR}/conf/variables
include ${PETSC_DIR}/conf/rules

genmatrix: genmatrix.o actualgen-reviewed.o functions-mpfi.o 
	@echo "\n--> Linking the executable... <-- \n"
	-${CLINKER} -o genmatrix genmatrix.o actualgen-reviewed.o functions-mpfi.o ${PETSC_KSP_LIB} ${PETSC_INCL} ${MPFI_INCL}
	${RM} genmatrix.o actualgen-reviewed.o dynamic.o functions-mpfi.o
	@echo "\n-->...done. <-- \n"

eigenerr: eigenerr.o eigenrig.o
	@echo "\n--> Linking the executable... <-- \n"
	-${CLINKER} ${PETSC_INCL} -o eigenerr eigenerr.o eigenrig.o ${PETSC_KSP_LIB}
	${RM} eigenerr.o eigenrig.o
	@echo "\n-->...done. <-- \n"
	
markovize: markovize.o
	@echo "\n--> Linking the executable... <-- \n"
	-${CLINKER} ${PETSC_INCL} -g -o markovize markovize.o ${PETSC_KSP_LIB}
	${RM} markovize.o
	@echo "\n-->...done. <-- \n"

markovize.o:
	@echo "\n--> compiling markovize.o... <-- \n"
	-${PCC} -c -g ${PETSC_INCL} ${IRRAM_INCL} markovize.cxx ${PETSC_MAT_LIB}
	
genmatrix.o:
	@echo "\n--> compiling genmatrix.o... <-- \n"
	-${PCC} -c ${PETSC_INCL} ${MPFI_INCL} genmatrix.cxx ${PETSC_MAT_LIB}

actualgen-reviewed.o:
	@echo "\n--> compiling actualgen-reviewed.o... <-- \n"
	-${PCC} -c -g -fopenmp ${PETSC_INCL} ${MPFI_INCL} actualgen-reviewed.cxx ${PETSC_MAT_LIB}

eigenrig.o:
	@echo "\n--> compiling eigenrig.o... <-- \n"
	-${PCC} -c ${PETSC_INCL} eigenrig.cxx ${PETSC_MAT_LIB}

functions-mpfi.o:
	@echo "\n--> compiling functions-mpfi.o... <-- \n"
	-${PCC} -c ${PETSC_INCL} ${MPFI_INCL} functions-mpfi.cxx ${PETSC_MAT_LIB}
