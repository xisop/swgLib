cmake_minimum_required (VERSION 3.0)

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
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
  else(BUILD_32BIT)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
  endif(BUILD_32BIT)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

include_directories( ${PROJECT_SOURCE_DIR}/include )
include_directories( ${PROJECT_SOURCE_DIR}/zlib-1.2.11 )
link_directories(    ${PROJECT_SOURCE_DIR}/lib )

set( APPS_DIR "${PROJECT_SOURCE_DIR}/apps" )

################################################################################
# Library sources                                                              #
################################################################################


if( BUILD_SWGLIB )
  file( GLOB SWG_HDR "${PROJECT_SOURCE_DIR}/include/swgLib/*.hpp"
					 "${PROJECT_SOURCE_DIR}/include/treLib/*.hpp"
                     "${PROJECT_SOURCE_DIR}/zlib-1.2.11/*.hpp"					 )
  file( GLOB SWG_SRC "${PROJECT_SOURCE_DIR}/src/swgLib/*.cpp"
					 "${PROJECT_SOURCE_DIR}/src/treLib/*.cpp"
					 "${PROJECT_SOURCE_DIR}/src/md5.c"
					 "${PROJECT_SOURCE_DIR}/zlib-1.2.11/*.c")

  # Shared swgLib
  if( BUILD_SHARED )
    # Library swgLib
    add_library( swg-shared SHARED ${SWG_HDR} ${SWG_SRC} )
    set_target_properties( swg-shared PROPERTIES OUTPUT_NAME swgLib )
    set_target_properties( swg-shared PROPERTIES CLEAN_DIRECT_OUTPUT 1 )

    if(BUILD_32BIT)
        install( TARGETS swg-shared
             LIBRARY DESTINATION lib
             COMPONENT sharedLib )
    else(BUILD_32BIT)
        install( TARGETS swg-shared
             LIBRARY DESTINATION lib64
             COMPONENT sharedLib )
    endif(BUILD_32BIT)
  endif( BUILD_SHARED )

  #Static libraries
  if( BUILD_STATIC )
    # Library swgLib
    add_library( swg-static STATIC ${SWG_HDR} ${SWG_SRC} )

    if( WIN32)
        set_target_properties( swg-static PROPERTIES OUTPUT_NAME swgLib_s )
    else( WIN32 )
        set_target_properties( swg-static PROPERTIES OUTPUT_NAME swgLib )
    endif( WIN32 )

    set_target_properties( swg-static PROPERTIES CLEAN_DIRECT_OUTPUT 1 )

    if(BUILD_32BIT)
        install( TARGETS swg-static
                 ARCHIVE DESTINATION lib
                 COMPONENT staticLib )
    else(BUILD_32BIT)
        install( TARGETS swg-static
                 ARCHIVE DESTINATION lib64
                 COMPONENT staticLib )
    endif(BUILD_32BIT)
  endif( BUILD_STATIC )

  install( DIRECTORY include/swgLib
  	   DESTINATION include
           COMPONENT headers )
endif( BUILD_SWGLIB )


################################################################################
# Applications                                                                 #
################################################################################

if( BUILD_SHARED )
  if( BUILD_SWGLIB )
    #swgLib apps
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

     #treLib apps
    add_executable( treDump ${APPS_DIR}/treDump.cpp )
    target_link_libraries( treDump swg-shared )

    add_executable( treBuild ${APPS_DIR}/treBuild.cpp )
    target_link_libraries( treBuild swg-shared )

    add_executable( testArchive ${APPS_DIR}/testArchive.cpp )
    target_link_libraries( testArchive swg-shared )
 endif( BUILD_SWGLIB )

endif( BUILD_SHARED )

if( BUILD_STATIC )
  if( BUILD_SWGLIB )
    #swgLib apps
    add_executable( iffDump_s ${APPS_DIR}/iffDump.cpp )
    target_link_libraries( iffDump_s swg-static )

    add_executable( readLOD_s ${APPS_DIR}/readLOD.cpp )
    target_link_libraries( readLOD_s swg-static )

    add_executable( readMSH_s ${APPS_DIR}/readMSH.cpp )
    target_link_libraries( readMSH_s swg-static )

    add_executable( readSWG_s ${APPS_DIR}/readSWG.cpp )
    target_link_libraries( readSWG_s swg-static )

    add_executable( readTRN_s ${APPS_DIR}/readTRN.cpp )
    target_link_libraries( readTRN_s swg-static )

    #treLib apps
    add_executable( treDump_s ${APPS_DIR}/treDump.cpp )
    target_link_libraries( treDump_s swg-static )

    add_executable( treBuild_s ${APPS_DIR}/treBuild.cpp )
    target_link_libraries( treBuild_s swg-static )

    add_executable( testArchive_s ${APPS_DIR}/testArchive.cpp )
    target_link_libraries( testArchive_s swg-static )
  endif( BUILD_SWGLIB )

endif( BUILD_STATIC )
