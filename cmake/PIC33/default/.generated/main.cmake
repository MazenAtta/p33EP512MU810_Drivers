include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(PIC33_default_library_list )

# Handle files with suffix s, for group default-XC16
if(PIC33_default_default_XC16_FILE_TYPE_assemble)
add_library(PIC33_default_default_XC16_assemble OBJECT ${PIC33_default_default_XC16_FILE_TYPE_assemble})
    PIC33_default_default_XC16_assemble_rule(PIC33_default_default_XC16_assemble)
    list(APPEND PIC33_default_library_list "$<TARGET_OBJECTS:PIC33_default_default_XC16_assemble>")
endif()

# Handle files with suffix S, for group default-XC16
if(PIC33_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(PIC33_default_default_XC16_assemblePreproc OBJECT ${PIC33_default_default_XC16_FILE_TYPE_assemblePreproc})
    PIC33_default_default_XC16_assemblePreproc_rule(PIC33_default_default_XC16_assemblePreproc)
    list(APPEND PIC33_default_library_list "$<TARGET_OBJECTS:PIC33_default_default_XC16_assemblePreproc>")
endif()

# Handle files with suffix c, for group default-XC16
if(PIC33_default_default_XC16_FILE_TYPE_compile)
add_library(PIC33_default_default_XC16_compile OBJECT ${PIC33_default_default_XC16_FILE_TYPE_compile})
    PIC33_default_default_XC16_compile_rule(PIC33_default_default_XC16_compile)
    list(APPEND PIC33_default_library_list "$<TARGET_OBJECTS:PIC33_default_default_XC16_compile>")
endif()

add_executable(${PIC33_default_image_name} ${PIC33_default_library_list})

target_link_libraries(${PIC33_default_image_name} PRIVATE ${PIC33_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
PIC33_default_link_rule(${PIC33_default_image_name})

# Add bin2hex target for converting built file to a .hex file.
add_custom_target(PIC33_default_Bin2Hex ALL
    ${MP_BIN2HEX} ${PIC33_default_image_name})
add_dependencies(PIC33_default_Bin2Hex ${PIC33_default_image_name})

# Post build target to copy built file to the output directory.
add_custom_command(TARGET ${PIC33_default_image_name} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory ${PIC33_default_output_dir}
                    COMMAND ${CMAKE_COMMAND} -E copy ${PIC33_default_image_name} ${PIC33_default_output_dir}/${PIC33_default_original_image_name}
                    BYPRODUCTS ${PIC33_default_output_dir}/${PIC33_default_original_image_name})
