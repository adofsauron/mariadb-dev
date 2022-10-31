# Copyright (c) 2009, 2015, Oracle and/or its affiliates.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1335  USA 

# Make source distribution
# If bzr is present, run bzr export.
# Otherwise, just run cpack with source configuration.

SET(CMAKE_SOURCE_DIR "/root/work/maria/server")
SET(CMAKE_BINARY_DIR "/root/work/maria/server/build")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "mariadb-10.11.1")
SET(CMAKE_CPACK_COMMAND  "/usr/bin/cpack")
SET(CMAKE_COMMAND  "/usr/bin/cmake")
SET(GIT_EXECUTABLE "/usr/bin/git")
SET(GTAR_EXECUTABLE "/usr/bin/gtar")
SET(TAR_EXECUTABLE "/usr/bin/tar")
SET(CMAKE_GENERATOR "Unix Makefiles")
SET(CMAKE_MAKE_PROGRAM "/usr/bin/gmake")
SET(CMAKE_SYSTEM_NAME "Linux")

SET(VERSION "10.11.1")

SET(MYSQL_DOCS_LOCATION "")


SET(PACKAGE_DIR  ${CMAKE_BINARY_DIR}/${CPACK_SOURCE_PACKAGE_FILE_NAME})

FILE(REMOVE_RECURSE ${PACKAGE_DIR})
FILE(REMOVE ${PACKAGE_DIR}.tar.gz )

IF(GIT_EXECUTABLE)
  MESSAGE(STATUS "Running git checkout-index")
  EXECUTE_PROCESS(
    COMMAND "${GIT_EXECUTABLE}" checkout-index --all --prefix=${PACKAGE_DIR}/
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE RESULT
  )
  IF(NOT RESULT EQUAL 0)
    SET(GIT_EXECUTABLE)
  ENDIF()
  EXECUTE_PROCESS(
    COMMAND "${GIT_EXECUTABLE}" submodule foreach "${GIT_EXECUTABLE} checkout-index --all --prefix=${PACKAGE_DIR}/$path/"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE RESULT
  )
  IF(NOT RESULT EQUAL 0)
    SET(GIT_EXECUTABLE)
  ENDIF()
  EXECUTE_PROCESS(
    COMMAND "${GIT_EXECUTABLE}" submodule foreach "${GIT_EXECUTABLE} checkout-index --all --prefix=${PACKAGE_DIR}/wsrep-lib/$path/"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/wsrep-lib
    RESULT_VARIABLE RESULT
  )
  IF(NOT RESULT EQUAL 0)
    SET(GIT_EXECUTABLE)
  ENDIF()
  EXECUTE_PROCESS(
    COMMAND "${GIT_EXECUTABLE}" submodule foreach "${GIT_EXECUTABLE} checkout-index --all --prefix=${PACKAGE_DIR}/storage/columnstore/columnstore/$path/"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/storage/columnstore/columnstore
    RESULT_VARIABLE RESULT
  )
  IF(NOT RESULT EQUAL 0)
    SET(GIT_EXECUTABLE)
  ENDIF()
ENDIF()

CONFIGURE_FILE(${CMAKE_BINARY_DIR}/include/source_revision.h
       ${PACKAGE_DIR}/include/source_revision.h COPYONLY)

CONFIGURE_FILE(${CMAKE_BINARY_DIR}/storage/rocksdb/rdb_source_revision.h
       ${PACKAGE_DIR}/storage/rocksdb/rdb_source_revision.h COPYONLY)

IF(NOT GIT_EXECUTABLE)
  MESSAGE(STATUS "git not found or source dir is not a repo, use CPack")
  
  IF(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    # In-source build is the worst option, we have to cleanup source tree.

    # Save bison output first.
    CONFIGURE_FILE(${CMAKE_BINARY_DIR}/sql/sql_yacc.cc
       ${CMAKE_BINARY_DIR}/sql_yacc.cc COPYONLY)
    CONFIGURE_FILE(${CMAKE_BINARY_DIR}/sql/sql_yacc.hh
       ${CMAKE_BINARY_DIR}/sql_yacc.hh COPYONLY)

    IF(CMAKE_GENERATOR MATCHES "Makefiles")
    # make clean
    EXECUTE_PROCESS(
      COMMAND ${CMAKE_MAKE_PROGRAM} clean 
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    ENDIF()
    
    # Restore bison output
    CONFIGURE_FILE(${CMAKE_BINARY_DIR}/sql_yacc.cc
       ${CMAKE_BINARY_DIR}/sql/sql_yacc.cc COPYONLY)
    CONFIGURE_FILE(${CMAKE_BINARY_DIR}/sql_yacc.hh
       ${CMAKE_BINARY_DIR}/sql/sql_yacc.h COPYONLY)
    FILE(REMOVE ${CMAKE_BINARY_DIR}/sql_yacc.cc)
    FILE(REMOVE ${CMAKE_BINARY_DIR}/sql_yacc.hh)
  ENDIF()

  EXECUTE_PROCESS(
    COMMAND ${CMAKE_CPACK_COMMAND} -G TGZ --config ./CPackSourceConfig.cmake 
    ${CMAKE_BINARY_DIR}/CPackSourceConfig.cmake

    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
  EXECUTE_PROCESS(
  COMMAND ${CMAKE_COMMAND} -E tar xzf 
    ${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.gz 
    ${PACK_SOURCE_PACKAGE_FILE_NAME} 
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
ENDIF()

# Copy bison output
CONFIGURE_FILE(${CMAKE_BINARY_DIR}/sql/yy_mariadb.hh
   ${PACKAGE_DIR}/sql/yy_mariadb.hh COPYONLY)
CONFIGURE_FILE(${CMAKE_BINARY_DIR}/sql/yy_mariadb.cc
   ${PACKAGE_DIR}/sql/yy_mariadb.cc COPYONLY)
# Copy bison output
CONFIGURE_FILE(${CMAKE_BINARY_DIR}/sql/yy_oracle.hh
   ${PACKAGE_DIR}/sql/yy_oracle.hh COPYONLY)
CONFIGURE_FILE(${CMAKE_BINARY_DIR}/sql/yy_oracle.cc
   ${PACKAGE_DIR}/sql/yy_oracle.cc COPYONLY)

# Add documentation, if user has specified where to find them
IF(MYSQL_DOCS_LOCATION)
  MESSAGE("Copying documentation files from " ${MYSQL_DOCS_LOCATION})
  EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_directory "${MYSQL_DOCS_LOCATION}" "${PACKAGE_DIR}")
ENDIF()

# Ensure there is an "INFO_SRC" file.
INCLUDE(${CMAKE_BINARY_DIR}/info_macros.cmake)
IF(NOT EXISTS ${PACKAGE_DIR}/Docs/INFO_SRC)
  CREATE_INFO_SRC(${PACKAGE_DIR}/Docs)
ENDIF()

# In case we used CPack, it could have copied some
# extra files that are not usable on different machines.
FILE(REMOVE ${PACKAGE_DIR}/CMakeCache.txt)

# When packing source, prefer gnu tar  to "cmake -P tar"
# cmake does not preserve timestamps.gnuwin32 tar is broken, cygwin is ok

IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
  IF (EXISTS C:/cygwin/bin/tar.exe)
    SET(TAR_EXECUTABLE C:/cygwin/bin/tar.exe)
  ENDIF()
ENDIF()

IF(GTAR_EXECUTABLE)
  SET(GNUTAR ${GTAR_EXECUTABLE})
ELSEIF(TAR_EXECUTABLE)
  EXECUTE_PROCESS(
    COMMAND "${TAR_EXECUTABLE}" --version
    RESULT_VARIABLE RESULT OUTPUT_VARIABLE OUT ERROR_VARIABLE ERR
  )
  IF(RESULT EQUAL 0 AND OUT MATCHES "GNU")
    SET(GNUTAR ${TAR_EXECUTABLE})
  ENDIF()
ENDIF()

SET($ENV{GZIP} "--best")

IF(GNUTAR)
  SET(PACK_COMMAND 
  ${GNUTAR} cfz 
  ${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.gz 
  ${CPACK_SOURCE_PACKAGE_FILE_NAME}
  )
ELSE()
  SET(PACK_COMMAND ${CMAKE_COMMAND} -E tar cfz
  ${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.gz 
  ${CPACK_SOURCE_PACKAGE_FILE_NAME}
)
ENDIF()

MESSAGE(STATUS "Creating source package")

EXECUTE_PROCESS( 
  COMMAND ${PACK_COMMAND}
)
MESSAGE(STATUS "Source package ${PACKAGE_DIR}.tar.gz created")
