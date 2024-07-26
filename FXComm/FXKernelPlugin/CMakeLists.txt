cmake_minimum_required(VERSION 3.28)
set(ProjectName "FXKernelPlugin")

project(ProjectName)

file(GLOB FXKernelPlugin_ROOT_CPP *.cpp)
file(GLOB FXKernelPlugin_ROOT_H *.h)


add_library(${ProjectName} SHARED
        ${FXKernelPlugin_ROOT_CPP}
        ${FXKernelPlugin_ROOT_H}
)

set_target_properties( ${ProjectName} PROPERTIES OUTPUT_NAME_DEBUG ${ProjectName})
set_target_properties( ${ProjectName} PROPERTIES PREFIX "")
set_target_properties( ${ProjectName} PROPERTIES
        FOLDER "FXComm/FXKernelPlugin"
        ARCHIVE_OUTPUT_DIRECTORY ${FXOutPutDir}
        RUNTIME_OUTPUT_DIRECTORY ${FXOutPutDir}
        LIBRARY_OUTPUT_DIRECTORY ${FXOutPutDir} )


if(UNIX)
    target_link_libraries(${ProjectName}  -static-libstdc++)
    add_definitions(
            -D_USRDLL
            -DELPP_NO_DEFAULT_LOG_FILE
    )
else()
    add_definitions(
            -DWIN
            -D_USRDLL
            -DELPP_NO_DEFAULT_LOG_FILE
    )

endif()


install(TARGETS ${ProjectName}
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)