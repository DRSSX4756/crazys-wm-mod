cmake_minimum_required(VERSION 3.12)

set(SRC CLog.cpp
		utils/DirPath.cpp
		utils/FileList.cpp
		sConfig.cpp
		utils/cKeyValueStore.cpp
		utils/string.cpp
		cRng.cpp
		text/cTextRepo.cpp
		text/actions.cpp
		text/conditions.cpp
        utils/piecewise_linear.cpp)


set(INTERFACE_SRC
        interface/sColor.cpp
        interface/cAnimatedSurface.cpp
		interface/cFont.cpp
        interface/cInterfaceWindow.cpp
        interface/cSurface.cpp
        interface/cWindowManager.cpp
        interface/cChoiceMessage.cpp
        interface/CGraphics.cpp
        interface/cImageCache.cpp
        interface/cInterfaceWindowXML.cpp
		interface/cTimer.cpp
        interface/ffmpeg.cpp
        interface/cTheme.cpp)

set(WIDGETS_SRC
        widgets/cButton.cpp
        widgets/cListBox.cpp
        widgets/cCheckBox.cpp
        widgets/cSlider.cpp
        widgets/cEditBox.cpp
        widgets/cTextItem.cpp
        widgets/cScrollBar.cpp
        widgets/cImageItem.cpp
        widgets/cScreenGetInput.cpp
        widgets/cMessageBox.cpp
        )

SET(XML_SRC xml/getattr.cpp xml/util.cpp)

configure_file(include/Revision.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Revision.h)

add_library(WMEngineObjects OBJECT ${SRC} ${WIDGETS_SRC} ${INTERFACE_SRC} ${XML_SRC})
target_link_libraries(WMEngineObjects PUBLIC SDL2::SDL2 lua::lua Boost::headers tinyxml2 FFMPEG::FFMPEG)
target_link_libraries(WMEngineObjects PRIVATE SDL2::TTF SDL2::Image doctest::doctest)
set_target_properties(WMEngineObjects PROPERTIES CXX_STANDARD 14)
if (UNIX)
	target_compile_definitions(WMEngineObjects PUBLIC LINUX=1)
else()
	target_link_libraries(WMEngineObjects PRIVATE shlwapi)
endif(UNIX)

target_include_directories(WMEngineObjects PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(WMEngineObjects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
