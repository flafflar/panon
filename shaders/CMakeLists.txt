find_package(Qt6 COMPONENTS ShaderTools REQUIRED)

function(add_shader shader_name glsl_versions)

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/image.frag
        COMMAND m4 
            -I ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/${shader_name}/image.frag 
            > ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/image.frag
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${shader_name}/image.frag
    )

    add_custom_target(
        ${shader_name}_image.frag
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/image.frag
    )

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/image.qsb
        COMMAND Qt6::qsb
            ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/image.frag
            --glsl ${glsl_versions}
            -o ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/image.qsb
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/image.frag
    )

    add_custom_target(
        ${shader_name}_image.qsb ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/image.qsb
    )

    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${shader_name}/image.qsb 
        DESTINATION plasmoid/contents/shaders/${shader_name}
    )

endfunction()

add_shader(default "100 es,120,150,440")