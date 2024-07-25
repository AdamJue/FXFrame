
set(ProjectName "FXTutorial3")

project(ProjectName)

file(GLOB Tutorial3_ROOT_CPP *.cpp)
file(GLOB Tutorial3_ROOT_H *.h)



add_library(${ProjectName} SHARED
        ${Tutorial3_ROOT_CPP}
        ${Tutorial3_ROOT_H}
)

set_target_properties( ${ProjectName} PROPERTIES OUTPUT_NAME_DEBUG ${ProjectName})
set_target_properties( ${ProjectName} PROPERTIES PREFIX "")
set_target_properties( ${ProjectName} PROPERTIES
        FOLDER "FXTutorial/FXTutorial3"
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
    target_link_libraries(${ProjectName}  -static-libstdc++)
    add_definitions(
            -DWIN
            -D_USRDLL
            -DELPP_NO_DEFAULT_LOG_FILE
    )

endif()