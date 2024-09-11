find_package(Qt6 COMPONENTS ShaderTools REQUIRED)

function(add_shader_raw shader_name glsl_versions)

    foreach(shader_file IN LISTS ARGN)
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/${shader_file}.frag
            COMMAND m4 
                -I ${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}/${shader_name}/${shader_file}.frag 
                > ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/${shader_file}.frag
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${shader_name}/${shader_file}.frag
        )

        add_custom_target(
            ${shader_name}_${shader_file}.frag
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/${shader_file}.frag
        )

        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/${shader_file}.qsb
            COMMAND Qt6::qsb
                ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/${shader_file}.frag
                --glsl ${glsl_versions}
                -o ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/${shader_file}.qsb
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/${shader_file}.frag
        )

        add_custom_target(
            ${shader_name}_${shader_file}.qsb ALL
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/${shader_file}.qsb
        )

        install(
            FILES ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/${shader_file}.qsb 
            DESTINATION plasmoid/contents/shaders/${shader_name}
        )

        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${shader_name})
    endforeach()

endfunction()

function(add_shader shader_name glsl_versions)
    add_shader_raw(${shader_name} ${glsl_versions} image)
endfunction()

function(add_buffered_shader shader_name glsl_versions)
    add_shader_raw(${shader_name} ${glsl_versions} image buffer)
endfunction()

add_shader(default "100 es,120,150")
