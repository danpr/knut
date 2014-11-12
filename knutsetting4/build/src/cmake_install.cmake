# Install script for directory: /home/dan/Projekty/knut/knutsetting4/src

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
    SET(CMAKE_INSTALL_CONFIG_NAME "debugfull")
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
  IF(EXISTS "$ENV{DESTDIR}/usr/lib/kde4/libexec/kcmknutsettinghelper" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/lib/kde4/libexec/kcmknutsettinghelper")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/lib/kde4/libexec/kcmknutsettinghelper"
         RPATH "")
  ENDIF()
  FILE(INSTALL DESTINATION "/usr/lib/kde4/libexec" TYPE EXECUTABLE FILES "/home/dan/Projekty/knut/knutsetting4/build/src/kcmknutsettinghelper")
  IF(EXISTS "$ENV{DESTDIR}/usr/lib/kde4/libexec/kcmknutsettinghelper" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/lib/kde4/libexec/kcmknutsettinghelper")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/lib/kde4/libexec/kcmknutsettinghelper")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/etc/dbus-1/system.d" TYPE FILE FILES "/home/dan/Projekty/knut/knutsetting4/build/src/org.kde.kcontrol.knutsetting.conf")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/share/dbus-1/system-services" TYPE FILE FILES "/home/dan/Projekty/knut/knutsetting4/build/src/org.kde.kcontrol.knutsetting.service")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}/usr/lib/kde4/kcm_knutsetting.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/lib/kde4/kcm_knutsetting.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/lib/kde4/kcm_knutsetting.so"
         RPATH "")
  ENDIF()
  FILE(INSTALL DESTINATION "/usr/lib/kde4" TYPE MODULE FILES "/home/dan/Projekty/knut/knutsetting4/build/lib/kcm_knutsetting.so")
  IF(EXISTS "$ENV{DESTDIR}/usr/lib/kde4/kcm_knutsetting.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/lib/kde4/kcm_knutsetting.so")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/lib/kde4/kcm_knutsetting.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

