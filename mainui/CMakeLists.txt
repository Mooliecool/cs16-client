#
# Copyright (c) 2015 Pavlo Lavrenenko
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#щ

cmake_minimum_required(VERSION 2.6.0)
project (MAINUI)

set (CMAKE_CXX_STANDARD 11)

set (MAINUI_LIBRARY xashmenu)

set (MAINUI_SOURCES udll_int.cpp )

if( NOT WIN32 AND NOT MINGW )
	add_compile_options( -Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-unused-variable )
endif()

if( NOT XASH_SDK )
	set( XASH_SDK "../" )
endif( )

include_directories (${XASH_SDK}/common ${XASH_SDK}/engine ${XASH_SDK}/pm_shared . controls/ menus/ ../public )
add_library (${MAINUI_LIBRARY} SHARED ${MAINUI_SOURCES})
add_compile_options(-Wl,--no-undefined)
add_definitions( -DMAINUI_STUB )

if( MSVC )
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif( )

set_target_properties (${MAINUI_LIBRARY} PROPERTIES
	POSITION_INDEPENDENT_CODE 1 )

# Name library as it named in Unkle Mike's Xash3D
if( WIN32 )
	set_target_properties(${MAINUI_LIBRARY} PROPERTIES
		OUTPUT_NAME menu )
endif()

if( NOT WIN32 )
	install( TARGETS ${MAINUI_LIBRARY} DESTINATION ${LIB_INSTALL_DIR}/xash3d 
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
else()
	install( TARGETS ${MAINUI_LIBRARY}
		CONFIGURATIONS Debug 
		RUNTIME DESTINATION ${LIB_INSTALL_DIR}/Debug/
		LIBRARY DESTINATION ${LIB_INSTALL_DIR}/Debug/ )
	install( FILES $<TARGET_FILE_DIR:${MAINUI_LIBRARY}>/$<TARGET_PROPERTY:${MAINUI_LIBRARY},OUTPUT_NAME>.pdb
		CONFIGURATIONS Debug 
		DESTINATION ${LIB_INSTALL_DIR}/Debug/ )
	install( TARGETS ${MAINUI_LIBRARY}
		CONFIGURATIONS Release 
		RUNTIME DESTINATION ${LIB_INSTALL_DIR}/Release/
		LIBRARY DESTINATION ${LIB_INSTALL_DIR}/Release/ )
endif()
