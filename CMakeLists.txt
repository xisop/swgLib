cmake_minimum_required (VERSION 2.6)

project( swgLib )

set( VERSION_MAJOR 0 )
set( VERSION_MINOR 1 )
set( VERSION_PATCH 0 )
set( VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH} )

option( BUILD_32BIT  "Build 32-bit instead of 64-bit" OFF)
option( BUILD_STATIC "Build static libs" OFF)
option( BUILD_SHARED "Build shared libs" ON)

if( WIN32 )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_WIN32_WINNT=0x0502")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_SCL_SECURE_NO_WARNINGS")
    add_definitions( -D_WIN32_WINNT=0x0502 )
    add_definitions( -D_SCL_SECURE_NO_WARNINGS )
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
#    configure_file(swgLib.pc.in_cmake swgLib.pc)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wc++11-compat")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-promo")
  if(BUILD_32BIT)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
  else(BUILD_32BIT)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
  endif(BUILD_32BIT)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

include_directories( ${PROJECT_SOURCE_DIR}/include )
link_directories(    ${PROJECT_SOURCE_DIR}/lib )

set( APPS_DIR "${PROJECT_SOURCE_DIR}/apps" )

################################################################################
# Library sources                                                              #
################################################################################

file( GLOB SWG_HDR "${PROJECT_SOURCE_DIR}/include/swgLib/*.hpp" )
file( GLOB SWG_SRC "${PROJECT_SOURCE_DIR}/src/swgLib/*.cpp" )
file( GLOB TRE_HDR "${PROJECT_SOURCE_DIR}/include/treLib/*.hpp" )
file( GLOB TRE_SRC "${PROJECT_SOURCE_DIR}/src/treLib/*.cpp" )

# Shared libraries
if( BUILD_SHARED )
    # Library swgLib
    add_library( swg-shared SHARED ${SWG_HDR} ${SWG_SRC} )
    set_target_properties( swg-shared PROPERTIES OUTPUT_NAME swgLib )
    set_target_properties( swg-shared PROPERTIES CLEAN_DIRECT_OUTPUT 1 )

    # Library treLib
    add_library( tre-shared SHARED ${TRE_HDR} ${TRE_SRC} )
    set_target_properties( tre-shared PROPERTIES OUTPUT_NAME treLib )
    set_target_properties( tre-shared PROPERTIES CLEAN_DIRECT_OUTPUT 1 )

    if(BUILD_32BIT)
        install( TARGETS swg-shared tre-shared
             RUNTIME DESTINATION lib
             LIBRARY DESTINATION lib
             COMPONENT sharedLib )
    else(BUILD_32BIT)
        install( TARGETS swg-shared tre-shared
             RUNTIME DESTINATION lib64
             LIBRARY DESTINATION lib64
             COMPONENT sharedLib )
    endif(BUILD_32BIT)
endif( BUILD_SHARED )

#Static libraries
if( BUILD_STATIC )
    # Library swgLib
    add_library( swg-static STATIC ${SWG_HDR} ${SWG_SRC} )

    # Library treLib
    add_library( tre-static STATIC ${TRE_HDR} ${TRE_SRC} )

    if( WIN32)
        set_target_properties( swg-static PROPERTIES OUTPUT_NAME swgLib_s )
        set_target_properties( tre-static PROPERTIES OUTPUT_NAME treLib_s )
    else( WIN32 )
        set_target_properties( swg-static PROPERTIES OUTPUT_NAME swgLib )
        set_target_properties( tre-static PROPERTIES OUTPUT_NAME treLib )
    endif( WIN32 )

    set_target_properties( swg-static PROPERTIES CLEAN_DIRECT_OUTPUT 1 )
    set_target_properties( tre-static PROPERTIES CLEAN_DIRECT_OUTPUT 1 )

    if(BUILD_32BIT)
        install( TARGETS swg-static tre-static
                 ARCHIVE DESTINATION lib
                 COMPONENT staticLib )
    else(BUILD_32BIT)
        install( TARGETS swg-static tre-static
                 ARCHIVE DESTINATION lib64
                 COMPONENT staticLib )
    endif(BUILD_32BIT)
endif( BUILD_STATIC )

install( DIRECTORY include/swgLib include/treLib
         DESTINATION include
         COMPONENT headers )

################################################################################
# Applications                                                                 #
################################################################################

if( BUILD_SHARED )
  add_executable( iffDump ${APPS_DIR}/iffDump.cpp )
  target_link_libraries( iffDump swg-shared )

  add_executable( readLOD ${APPS_DIR}/readLOD.cpp )
  target_link_libraries( readLOD swg-shared )

  add_executable( readMSH ${APPS_DIR}/readMSH.cpp )
  target_link_libraries( readMSH swg-shared )

  add_executable( readSWG ${APPS_DIR}/readSWG.cpp )
  target_link_libraries( readSWG swg-shared )

  add_executable( readTRN ${APPS_DIR}/readTRN.cpp )
  target_link_libraries( readTRN swg-shared )
endif( BUILD_SHARED )

if( BUILD_STATIC )
  add_executable( iffDump_s ${APPS_DIR}/iffDump.cpp )
  target_link_libraries( iffDump_s swg-static )

  add_executable( readLOD_s ${APPS_DIR}/readLOD.cpp )
  target_link_libraries( readLOD_s swg-shared )

  add_executable( readMSH_s ${APPS_DIR}/readMSH.cpp )
  target_link_libraries( readMSH_s swg-shared )

  add_executable( readSWG_s ${APPS_DIR}/readSWG.cpp )
  target_link_libraries( readSWG_s swg-shared )

  add_executable( readTRN_s ${APPS_DIR}/readTRN.cpp )
  target_link_libraries( readTRN_s swg-shared )
endif( BUILD_STATIC )
