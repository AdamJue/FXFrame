
set(ProjectName "GuiMeshPlugin")

project(GuiMeshPlugin)

find_package(ZLIB REQUIRED)

file(GLOB GuiMeshPlugin_ROOT_CPP *.cpp)
file(GLOB GuiMeshPlugin_ROOT_H *.h)

add_library(${ProjectName} SHARED
        ${GuiMeshPlugin_ROOT_CPP}
        ${GuiMeshPlugin_ROOT_H}
)

target_link_libraries(${ProjectName} PRIVATE
        ZLIB::ZLIB
)

#if (MSVC)
#    target_link_libraries(${ProjectName} PRIVATE
#            ws2_32
#            FXMessageDefine
#    )
#else()
#    target_link_libraries(${ProjectName} PRIVATE
#          FXMessageDefine
#    )
#endif ()


set_target_properties( ${ProjectName} PROPERTIES OUTPUT_NAME_DEBUG ${ProjectName})
set_target_properties( ${ProjectName} PROPERTIES PREFIX "")
set_target_properties( ${ProjectName} PROPERTIES
        FOLDER "FXTutorial/GuiMeshPlugin"
        ARCHIVE_OUTPUT_DIRECTORY ${FXOutPutDir}
        RUNTIME_OUTPUT_DIRECTORY ${FXOutPutDir}
        LIBRARY_OUTPUT_DIRECTORY ${FXOutPutDir} )


install(TARGETS ${ProjectName}
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)