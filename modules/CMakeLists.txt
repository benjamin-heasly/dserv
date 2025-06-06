cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

if(WIN32)
	add_definitions(-DUSE_TCL_STUBS)
	include_directories(c:/usr/local/include ../src)
	link_directories(c:/usr/local/lib/$ENV{VSCMD_ARG_TGT_ARCH})
	set(TCLLIB tclstub.lib)
	set(CMAKE_SHARED_LINKER_FLAGS "/NODEFAULTLIB:MSCVRT")
elseif(APPLE)
	add_definitions(-DMACOS -Dunix -DLINUX -fPIC -DUSE_TCL_STUBS)
	include_directories(../src /usr/local/include)
	link_directories(/usr/local/lib)
	set(CMAKE_MODULE_LINKER_FLAGS "-undefined dynamic_lookup")
	set(TCLLIB tclstub)
	set(CMAKE_SHARED_MODULE_SUFFIX .dylib)

	if(DEFINED "CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM")
		# Configure for apple code signing and notarization.
		set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "--strict --timestamp --options=runtime")
		set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_INJECT_BASE_ENTITLEMENTS "NO")
		set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Developer ID Application")
		set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual")
		set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0")
		set(CMAKE_SKIP_RPATH TRUE)
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "QNX")
	add_definitions(-DUSE_TCL_STUBS)
	include_directories(../local/include)
	link_directories(../local/aarch64/lib)
	set(TCLLIB tclstub)
else()
	add_definitions(-DUSE_TCL_STUBS -DLINUX)
	include_directories(../src /usr/local/include)
	link_directories(/usr/local/lib)
	set(TCLLIB tclstub)
endif()

include_directories(${TCL_INCLUDE_DIR} .)

##
## timer
##
set(MODULE timer)
project(${MODULE})
add_library(${MODULE} MODULE ${MODULE}/${MODULE}.c)
set_target_properties(${MODULE} PROPERTIES PREFIX "dserv_")
target_link_libraries(${MODULE} ${DLSH} ${TCLLIB} pthread)

##
## ain
##
set(MODULE ain)
project(${MODULE})
add_library(${MODULE} MODULE ${MODULE}/${MODULE}.c)
set_target_properties(${MODULE} PROPERTIES PREFIX "dserv_")
target_link_libraries(${MODULE} ${DLSH} ${TCLLIB} pthread)

##
## juicer
##
set(MODULE juicer)
project(${MODULE})
add_library(${MODULE} MODULE ${MODULE}/${MODULE}.c)
set_target_properties(${MODULE} PROPERTIES PREFIX "dserv_")
target_link_libraries(${MODULE} ${DLSH} ${TCLLIB} pthread)

##
## gpio_input
##
set(MODULE gpio_input)
project(${MODULE})
add_library(${MODULE} MODULE ${MODULE}/${MODULE}.c)
set_target_properties(${MODULE} PROPERTIES PREFIX "dserv_")
target_link_libraries(${MODULE} ${DLSH} ${TCLLIB} pthread)

##
## gpio_output
##
set(MODULE gpio_output)
project(${MODULE})
add_library(${MODULE} MODULE ${MODULE}/${MODULE}.c)
set_target_properties(${MODULE} PROPERTIES PREFIX "dserv_")
target_link_libraries(${MODULE} ${DLSH} ${TCLLIB} pthread)

##
## rmt
##
set(MODULE rmt)
project(${MODULE})
add_library(${MODULE} MODULE ${MODULE}/${MODULE}.c)
set_target_properties(${MODULE} PROPERTIES PREFIX "dserv_")
target_link_libraries(${MODULE} ${DLSH} ${TCLLIB} pthread)

##
## eventlog
##
set(MODULE eventlog)
project(${MODULE})
add_library(${MODULE} MODULE ${MODULE}/${MODULE}.c)
set_target_properties(${MODULE} PROPERTIES PREFIX "dserv_")
target_link_libraries(${MODULE} ${DLSH} ${TCLLIB} pthread)

##
## sound
##
set(MODULE sound)
project(${MODULE})
add_library(${MODULE} MODULE ${MODULE}/${MODULE}.c)
set_target_properties(${MODULE} PROPERTIES PREFIX "dserv_")
target_link_libraries(${MODULE} ${DLSH} ${TCLLIB} pthread)

##
## usbio
##
set(MODULE usbio)
project(${MODULE})
add_library(${MODULE} MODULE ${MODULE}/${MODULE}.c)
set_target_properties(${MODULE} PROPERTIES PREFIX "dserv_")
target_link_libraries(${MODULE} ${DLSH} ${TCLLIB} pthread)

##
## ina226
##
set(MODULE ina226)
project(${MODULE})
add_library(${MODULE} MODULE ${MODULE}/${MODULE}.c)
set_target_properties(${MODULE} PROPERTIES PREFIX "dserv_")
target_link_libraries(${MODULE} ${DLSH} ${TCLLIB} pthread)

##
## joystick4
##
set(MODULE joystick4)
project(${MODULE})
add_library(${MODULE} MODULE ${MODULE}/${MODULE}.c)
set_target_properties(${MODULE} PROPERTIES PREFIX "dserv_")
target_link_libraries(${MODULE} ${DLSH} ${TCLLIB} pthread)

##
## touch
##
set(MODULE touch)
project(${MODULE})
if (UNIX AND NOT APPLE)
set (EVDEVLIB evdev)
include_directories(/usr/include/libevdev-1.0)
endif()
add_library(${MODULE} MODULE ${MODULE}/${MODULE}.c)
set_target_properties(${MODULE} PROPERTIES PREFIX "dserv_")
target_link_libraries(${MODULE} ${DLSH} ${TCLLIB} ${EVDEVLIB} pthread)


install(TARGETS
	ain juicer gpio_input gpio_output touch
	rmt eventlog sound timer usbio ina226 joystick4
	DESTINATION dserv/modules COMPONENT dserv)
