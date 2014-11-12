# Install script for directory: /home/dan/Projekty/knut/knutclient4/doc/en

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
  FILE(INSTALL DESTINATION "/usr/share/doc/kde/HTML/en/knutclient" TYPE FILE FILES
    "/home/dan/Projekty/knut/knutclient4/build/doc/en/index.cache.bz2"
    "/home/dan/Projekty/knut/knutclient4/doc/en/index.docbook"
    "/home/dan/Projekty/knut/knutclient4/doc/en/psetting-en.png"
    "/home/dan/Projekty/knut/knutclient4/doc/en/new-en.png"
    "/home/dan/Projekty/knut/knutclient4/doc/en/asetting-en.png"
    "/home/dan/Projekty/knut/knutclient4/doc/en/mkicker-en.png"
    "/home/dan/Projekty/knut/knutclient4/doc/en/variables-en.png"
    "/home/dan/Projekty/knut/knutclient4/doc/en/usetting-en.png"
    "/home/dan/Projekty/knut/knutclient4/doc/en/fsetting-en.png"
    "/home/dan/Projekty/knut/knutclient4/doc/en/msetting-en.png"
    "/home/dan/Projekty/knut/knutclient4/doc/en/tkicker-en.png"
    "/home/dan/Projekty/knut/knutclient4/doc/en/knutclient-en.png"
    "/home/dan/Projekty/knut/knutclient4/doc/en/ksetting-en.png"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  execute_process(COMMAND /usr/bin/cmake -E create_symlink "/usr/share/doc/kde/HTML/en/common"  "$ENV{DESTDIR}/usr/share/doc/kde/HTML/en/knutclient/common" )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

