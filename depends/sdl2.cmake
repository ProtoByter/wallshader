include(FetchContent)

function(getSDL2 version)
    FetchContent_Declare(sdl2
            GIT_REPOSITORY  https://github.com/libsdl-org/SDL
            GIT_TAG         release-${version}
            GIT_SHALLOW     TRUE
            SOURCE_DIR      ${CMAKE_BINARY_DIR}/sdl2-${version}-src
            )

    FetchContent_GetProperties(sdl2)
    if(NOT sdl2_POPULATED)
        message(STATUS "Fetching sdl2 ${version}")
        FetchContent_Populate(sdl2)
        message(STATUS "Fetched sdl2 ${version}")
    endif()

    add_subdirectory(${sdl2_SOURCE_DIR})
endfunction()