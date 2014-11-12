# Install script for directory: /home/dan/Projekty/knut/knutclient4/doc/cs

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/share/doc/kde/HTML/cs/knutclient" TYPE FILE FILES
    "/home/dan/Projekty/knut/knutclient4/build/doc/cs/index.cache.bz2"
    "/home/dan/Projekty/knut/knutclient4/doc/cs/index.docbook"
    "/home/dan/Projekty/knut/knutclient4/doc/cs/ksetting-cs.png"
    "/home/dan/Projekty/knut/knutclient4/doc/cs/variables-cs.png"
    "/home/dan/Projekty/knut/knutclient4/doc/cs/tkicker-cs.png"
    "/home/dan/Projekty/knut/knutclient4/doc/cs/mkicker-cs.png"
    "/home/dan/Projekty/knut/knutclient4/doc/cs/msetting-cs.png"
    "/home/dan/Projekty/knut/knutclient4/doc/cs/usetting-cs.png"
    "/home/dan/Projekty/knut/knutclient4/doc/cs/asetting-cs.png"
    "/home/dan/Projekty/knut/knutclient4/doc/cs/knutclient-cs.png"
    "/home/dan/Projekty/knut/knutclient4/doc/cs/psetting-cs.png"
    "/home/dan/Projekty/knut/knutclient4/doc/cs/new-cs.png"
    "/home/dan/Projekty/knut/knutclient4/doc/cs/fsetting-cs.png"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  execute_process(COMMAND /usr/bin/cmake -E create_symlink "/usr/share/doc/kde/HTML/cs/common"  "$ENV{DESTDIR}/usr/share/doc/kde/HTML/cs/knutclient/common" )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

