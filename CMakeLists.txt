cmake_minimum_required (VERSION 3.6)

set (VSE_VERSION_1 0)
set (VSE_VERSION_2 0)
set (VSE_VERSION_3 1)
set (VSE_APP_NAME VisualScriptEngineWxWidgets)

function (SetCompilerOptions module)
	set (DebugOptions)
	set (ReleaseOptions)
	if (MSVC)
		target_compile_options (${module} PRIVATE /W4 /WX)
		set (DebugOptions -DDEBUG -D_DEBUG /Zi /Gy /GR /FC)
		set (ReleaseOptions /Oi /Gy)
	else ()
		target_compile_options (${module} PRIVATE -std=c++11 -Wall -Wextra -Werror)
		set (DebugOptions -DDEBUG -D_DEBUG -g -ggdb)
	endif ()
	target_compile_options (${module} PUBLIC
		"$<$<CONFIG:Debug>:${DebugOptions}>"
		"$<$<CONFIG:Release>:${ReleaseOptions}>"
	)
endfunction ()

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set (CMAKE_SUPPRESS_REGENERATION 1)
set (CMAKE_CONFIGURATION_TYPES Debug;Release)
set (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/DevKit CACHE PATH "Install prefix.")
set (CMAKE_DEBUG_POSTFIX "Debug")
set (VSE_DEVKIT_DIR $ENV{VSE_DEVKIT_DIR} CACHE PATH "VisualScriptEngine binary directory.")
set (WXWIDGETS_DIR $ENV{WXWIDGETS_DIR} CACHE PATH "wxWidgets binary directory.")
set (VSE_LIB_POSTFIX "$<$<CONFIG:Debug>:Debug>")

add_definitions (-DUNICODE -D_UNICODE)
link_directories (${VSE_DEVKIT_DIR}/lib)

project (VisualScriptEngineWxWidgets)

enable_testing ()

if (MSVC AND NOT ${WXWIDGETS_DIR} STREQUAL "")
	set (WXWIDGETS_LIBS_DIR)
	set (WXWIDGETS_PLATFORM_INCLUDE_DIR)
	if (MSVC)
		set (WXWIDGETS_LIBS_DIR ${WXWIDGETS_DIR}/lib/vc_x64_lib)
		set (WXWIDGETS_PLATFORM_INCLUDE_DIR ${WXWIDGETS_DIR}/include/msvc)
	endif ()
	link_directories (${WXWIDGETS_LIBS_DIR})

	# wxWidgetsAppSupport
	
	set (wxWidgetsAppSupportSourcesFolder Sources/wxWidgetsAppSupport)
	file (GLOB wxWidgetsAppSupportHeaderFiles ${wxWidgetsAppSupportSourcesFolder}/*.hpp)
	file (GLOB wxWidgetsAppSupportSourceFiles ${wxWidgetsAppSupportSourcesFolder}/*.cpp)
	set (
		wxWidgetsAppSupportFiles
		${wxWidgetsAppSupportHeaderFiles}
		${wxWidgetsAppSupportSourceFiles}
	)
	source_group ("Sources" FILES ${wxWidgetsAppSupportFiles})
	add_library (wxWidgetsAppSupport STATIC ${wxWidgetsAppSupportFiles})
	target_include_directories (
		wxWidgetsAppSupport PUBLIC
		${VSE_DEVKIT_DIR}/include
		${WXWIDGETS_DIR}/include
		${WXWIDGETS_PLATFORM_INCLUDE_DIR}
	)
	target_link_libraries (wxWidgetsAppSupport
		${VSE_DEVKIT_DIR}/lib/NodeEngine${VSE_LIB_POSTFIX}.lib
		${VSE_DEVKIT_DIR}/lib/NodeUIEngine${VSE_LIB_POSTFIX}.lib
		${VSE_DEVKIT_DIR}/lib/BuiltInNodes${VSE_LIB_POSTFIX}.lib	
	)
	target_compile_definitions (wxWidgetsAppSupport PUBLIC _CRT_SECURE_NO_WARNINGS)
	SetCompilerOptions (wxWidgetsAppSupport)
	install (TARGETS wxWidgetsAppSupport DESTINATION lib)
	install (FILES ${wxWidgetsAppSupportHeaderFiles} DESTINATION include)
	install (FILES ${wxWidgetsAppSupportSourceFiles} DESTINATION source)
	
	# wxWidgetsTestApp

	set (wxWidgetsTestAppSourcesFolder Sources/wxWidgetsTestApp)
	file (GLOB wxWidgetsTestAppHeaderFiles ${wxWidgetsTestAppSourcesFolder}/*.hpp)
	file (GLOB wxWidgetsTestAppSourceFiles ${wxWidgetsTestAppSourcesFolder}/*.cpp)
	set (
		wxWidgetsTestAppFiles
		${wxWidgetsTestAppHeaderFiles}
		${wxWidgetsTestAppSourceFiles}
	)
	source_group ("Sources" FILES ${wxWidgetsTestAppFiles})
	add_executable (wxWidgetsTestApp WIN32 ${wxWidgetsTestAppFiles})
	target_include_directories (
		wxWidgetsTestApp PUBLIC
		${NodeEngineSourcesFolder}
		${NodeUIEngineSourcesFolder}
		${BuiltInNodesSourcesFolder}
		${TestFrameworkSourcesFolder}
		${WindowsAppSupportSourcesFolder}
		${wxWidgetsAppSupportSourcesFolder}
		${VSE_DEVKIT_DIR}/include
		${WXWIDGETS_DIR}/include
		${WXWIDGETS_PLATFORM_INCLUDE_DIR}		
	)
	target_link_libraries (wxWidgetsTestApp
		${VSE_DEVKIT_DIR}/lib/NodeEngine${VSE_LIB_POSTFIX}.lib
		${VSE_DEVKIT_DIR}/lib/NodeUIEngine${VSE_LIB_POSTFIX}.lib
		${VSE_DEVKIT_DIR}/lib/BuiltInNodes${VSE_LIB_POSTFIX}.lib	
		${VSE_DEVKIT_DIR}/lib/WindowsAppSupport${VSE_LIB_POSTFIX}.lib	
		wxWidgetsAppSupport
	)
	target_compile_definitions (wxWidgetsTestApp PUBLIC _CRT_SECURE_NO_WARNINGS)
	SetCompilerOptions (wxWidgetsTestApp)

endif ()
