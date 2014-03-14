cmake_minimum_required( VERSION 2.6 )

project( lattice_shape )

find_package( wxWidgets COMPONENTS core base REQUIRED )
# we need version 2.9 or higher but http://public.kitware.com/Bug/view.php?id=10694
include( "${wxWidgets_USE_FILE}" )

add_executable( lattice_shape
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

target_link_libraries( lattice_shape ${wxWidgets_LIBRARIES} )

if( NOT CMAKE_BUILD_TYPE ) 
  message( STATUS "No build type selected, default to Release" )
  set( CMAKE_BUILD_TYPE "Release" )
endif()
