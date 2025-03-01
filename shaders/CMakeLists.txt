find_package(Qt6 COMPONENTS ShaderTools REQUIRED)

function(add_shader_raw shader_name glsl_versions)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${shader_name})

    install(
        FILES ${CMAKE_CURRENT_SOURCE_DIR}/${shader_name}/texture.png
              ${CMAKE_CURRENT_SOURCE_DIR}/${shader_name}/meta.json
                ${CMAKE_CURRENT_SOURCE_DIR}/${shader_name}/hint.html
        DESTINATION plasmoid/contents/shaders/${shader_name}
        OPTIONAL
    )

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

    endforeach()

endfunction()

function(add_shader shader_name glsl_versions)
    add_shader_raw(${shader_name} ${glsl_versions} image)
endfunction()

function(add_shader_with_buffer shader_name glsl_versions)
    add_shader_raw(${shader_name} ${glsl_versions} image buffer)
endfunction()

# Use OpenGL 1.0, not ES, as ES doesn't support highp floats, which is what is used ~everywhere
add_shader_with_buffer(bar1ch "100,120,150")
add_shader(chain "100,120,150")
add_shader(default "100,120,150")
add_shader(oie1ch  "100,120,150")
add_shader_with_buffer(spectrogram "100,120,150")

# These are not used by anything at the moment, but require OpenGL 1.5 to be used
add_shader_raw("" "150" gldft wave-buffer)
