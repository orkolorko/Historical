# - Try to find iRRAM
# Once done, this will define
#
#  iRRAM_FOUND - system has Magick++
#  iRRAM_INCLUDE_DIRS - the Magick++ include directories
#  iRRAM_LIBRARIES - link these to use Magick++

include(LibFindMacros)

# Dependencies
libfind_package(iRRAM MPFR)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(iRRAM_PKGCONF iRRAM)

# Include dir
find_path(iRRAM_INCLUDE_DIR
  NAMES iRRAM.h
  PATHS ${iRRAM_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(iRRAM_LIBRARY
  NAMES iRRAM
  PATHS ${iRRAM_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(iRRAM_PROCESS_INCLUDES iRRAM_INCLUDE_DIR iRRAM_INCLUDE_DIRS)
set(iRRAM_PROCESS_LIBS iRRAM_LIBRARY iRRAM_LIBRARIES)
libfind_process(iRRAM)
