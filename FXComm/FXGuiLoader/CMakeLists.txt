cmake_minimum_required(VERSION 3.28)
set(ProjectName "FXGuiLoader")

project(FXGuiLoader)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets LinguistTools Network Gui OpenGL Concurrent)

find_path(RAPIDXML_INCLUDE_DIRS "rapidxml/rapidxml.hpp")

qt_standard_project_setup()


file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE UI_FILES "${PROJECT_SOURCE_DIR}/*.ui")
file(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/*.hpp" "${PROJECT_SOURCE_DIR}/*.h")

#qt_add_resources(GUI_RESOURCES "${PROJECT_SOURCE_DIR}/resource/resources.qrc")


qt_add_executable(${PROJECT_NAME}
        ${GUI_TYPE}
        ${SRC_FILES}
        ${UI_FILES}
        ${GUI_RESOURCES}
        ${HEADER_FILES}
        ${GENERATED_CPP_FILES}
        ${GENERATED_GRPC_FILES}
)

#qt_add_translations(${PROJECT_NAME}
#        TS_FILES
#        ${CMAKE_SOURCE_DIR}/src/resource/translate/${PROJECT_NAME}_en_US.ts
#        ${CMAKE_SOURCE_DIR}/src/resource/translate/${PROJECT_NAME}_zh_CN.ts
#)


target_include_directories(${ProjectName} PRIVATE ${RAPIDXML_INCLUDE_DIRS})

target_link_libraries(${ProjectName} PRIVATE
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::Concurrent
)


set_target_properties( ${ProjectName} PROPERTIES OUTPUT_NAME_DEBUG ${ProjectName})
set_target_properties( ${ProjectName} PROPERTIES PREFIX "")
set_target_properties( ${ProjectName} PROPERTIES
        FOLDER "FXComm/FXGuiLoader"
        ARCHIVE_OUTPUT_DIRECTORY ${FXOutPutDir}
        RUNTIME_OUTPUT_DIRECTORY ${FXOutPutDir}
        LIBRARY_OUTPUT_DIRECTORY ${FXOutPutDir} )


if (WIN32)
    qt_generate_deploy_app_script(
            TARGET ${PROJECT_NAME}
            OUTPUT_SCRIPT deploy_script
            NO_UNSUPPORTED_PLATFORM_ERROR
    )

    # Call the deployment script on " cmake --install ".
    install(SCRIPT ${deploy_script})


    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND Qt6::windeployqt
            ARGS $<TARGET_FILE:${PROJECT_NAME}>
    )

    set(QtDirs platforms;styles;imageformats;networkinformation;translations)

    foreach (dir IN LISTS QtDirs)
        install(DIRECTORY ${CMAKE_BINARY_DIR}/${dir} DESTINATION ${FXOutPutDir})
    endforeach ()

    install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

    install(CODE [[
                file(GET_RUNTIME_DEPENDENCIES
                    LIBRARIES
                        $<TARGET_FILE:FXGuiLoader>
                    RESOLVED_DEPENDENCIES_VAR _r_deps
                    UNRESOLVED_DEPENDENCIES_VAR _u_deps
                )
                foreach(_file ${_r_deps})
                    string(TOLOWER ${_file} _file_lower)
                    if(NOT ${_file_lower} MATCHES "c:[\\/]windows[\\/]system32.*")
                        file(INSTALL
                            DESTINATION "${CMAKE_INSTALL_PREFIX}"
                            TYPE SHARED_LIBRARY
                            FOLLOW_SYMLINK_CHAIN
                            FILES "${_file}"
                        )
                    endif()
                endforeach()
            ]])

    set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
endif ()