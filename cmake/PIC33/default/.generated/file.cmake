# The following variables contains the files used by the different stages of the build process.
set(PIC33_default_default_XC16_FILE_TYPE_assemble)
set(PIC33_default_default_XC16_FILE_TYPE_assemblePreproc)
set_source_files_properties(${PIC33_default_default_XC16_FILE_TYPE_assemblePreproc} PROPERTIES LANGUAGE C)
set(PIC33_default_default_XC16_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../APP.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ECU_Layer/LED/ECU_LED.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../MCAL_Layer/DEVICE_CONFIG.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../MCAL_Layer/GPIO/HAL_GPIO.c")
set(PIC33_default_default_XC16_FILE_TYPE_link)

# The (internal) path to the resulting build image.
set(PIC33_default_internal_image_name "${CMAKE_CURRENT_SOURCE_DIR}/../../../_build/PIC33/default/default.hex")

# The name of the resulting image, including namespace for configuration.
set(PIC33_default_image_name "PIC33_default_default.hex")

# The name of the image, excluding the namespace for configuration.
set(PIC33_default_original_image_name "default.hex")

# The output directory of the final image.
set(PIC33_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/PIC33")
