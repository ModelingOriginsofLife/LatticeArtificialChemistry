cmake_minimum_required( VERSION 2.6 )

project( lattice_artificial_chemistry )

FIND_PACKAGE( wxWidgets COMPONENTS html aui ${WXGLCANVASLIBS} core adv base )
# we need version 2.9 or higher but http://public.kitware.com/Bug/view.php?id=10694
include( "${wxWidgets_USE_FILE}" )

add_executable( lattice_artificial_chemistry
  WIN32
  MACOSX_BUNDLE
  World.cpp
  World.hpp
  World_wx.cpp
  World_wx.hpp
  Atom.cpp
  Atom.hpp
  Reaction.hpp
  fatal_if.cpp
  fatal_if.hpp
  window.cpp
)

#-------------------------------- build ------------------------------------------------------

target_link_libraries( lattice_artificial_chemistry ${wxWidgets_LIBRARIES} )

if( NOT CMAKE_BUILD_TYPE ) 
  message( STATUS "No build type selected, default to Release" )
  set( CMAKE_BUILD_TYPE "Release" )
endif()

if( WIN32 )
  # prevent link errors with wxMSW 2.9.x
  add_definitions( -DwxDEBUG_LEVEL=0 )
endif()

# avoid security warnings
if( MSVC )
  add_definitions( /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_WARNINGS )
endif()

# ensure we link the C runtime statically (N.B. still appears as /MD in the CMake gui but ignore this)
# see: http://www.cmake.org/Wiki/CMake_FAQ#Dynamic_Replace
foreach( var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO )
  string( REGEX REPLACE "/MD" "/MT" ${var} "${${var}}" )
endforeach()

