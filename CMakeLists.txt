cmake_minimum_required(VERSION 3.30)

set(PROG_TYPE usbasp)
#set(PROG_TYPE c232hm)
#set(PROG_TYPE arduino)
#Better done in main tbch

# Variables regarding the AVR chip
set(MCU   atmega644p)
set(F_CPU 12000000)
set(BAUD  115200)
set(PICTOR_FASTMODE)
set(COM_PORT COM12)
add_definitions(-DF_CPU=${F_CPU})
set(CMAKE_CXX_COMPILER "avr-g++")

# program names
set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP  avr-objdump)
set(AVRSIZE  avr-size)
set(AVRDUDE  avrdude)

# Sets the compiler
# Needs to come before the project function
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_ASM_COMPILER   ${AVRC})

project (WattsApp C CXX ASM)

# Important project paths
set(BASE_PATH    "${CMAKE_SOURCE_DIR}")
set(SRC_PATH     "${BASE_PATH}/src")
set(LIB_PATH     "${BASE_PATH}/lib")
set(FONT_PATH    "${BASE_PATH}/fonts")
set(ARDUINO_PATH    "${LIB_PATH}/arduino")
set(USBDRV_PATH   "${LIB_PATH}/usbdrv")
set(CHECKSUM_PATH   "${LIB_PATH}/checksum")

# Files to be compiled
file(GLOB_RECURSE SRC_FILES
        "${SRC_PATH}/*.cpp"
        "${SRC_PATH}/*.cc"
        "${SRC_PATH}/*.c"
        "${SRC_PATH}/*.cxx"
        "${SRC_PATH}/*.S"
        "${SRC_PATH}/*.s"
        "${SRC_PATH}/*.sx"
)

file(GLOB_RECURSE LIB_FILES
        "${LIB_PATH}/*.cpp"
        "${LIB_PATH}/*.cc"
        "${LIB_PATH}/*.c"
        "${LIB_PATH}/*.cxx"
        "${LIB_PATH}/*.S"
        "${LIB_PATH}/*.s"
        "${LIB_PATH}/*.sx"
)

file(GLOB_RECURSE ARDUINO_FILES
        "${ARDUINO_PATH}/*.h"
        "${ARDUINO_PATH}/*.cpp"
        "${ARDUINO_PATH}/*.cc"
        "${ARDUINO_PATH}/*.c"
        "${ARDUINO_PATH}/*.cxx"
        "${ARDUINO_PATH}/*.S"
        "${ARDUINO_PATH}/*.s"
        "${ARDUINO_PATH}/*.sx"
)

file(GLOB_RECURSE CHECKSUM_FILES
        "${CHECKSUM_PATH}/*.h"
        "${CHECKSUM_PATH}/*.cpp"
        "${CHECKSUM_PATH}/*.cc"
        "${CHECKSUM_PATH}/*.c"
        "${CHECKSUM_PATH}/*.cxx"
        "${CHECKSUM_PATH}/*.S"
        "${CHECKSUM_PATH}/*.s"
        "${CHECKSUM_PATH}/*.sx"
)

file(GLOB_RECURSE USBDRV_FILES
        "${USBDRV_PATH}/*.h"
        "${USBDRV_PATH}/*.cpp"
        "${USBDRV_PATH}/*.cc"
        "${USBDRV_PATH}/*.c"
        "${USBDRV_PATH}/*.cxx"
        "${USBDRV_PATH}/*.S"
        "${USBDRV_PATH}/*.s"
        "${USBDRV_PATH}/*.sx"
)

# Compiler flags

set(CDEBUG    "-gstabs -g -ggdb")
set(CWARN     "-Wall -Wl,--gc-sections -Wl,--relax")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
set(COPT      "-Os -lm -lprintf_flt -Wl,-u,vfprintf")
set(CMCU      "-mmcu=${MCU}")
set(CDEFS     "-DF_CPU=${F_CPU} -DBAUD=${BAUD}")

set(CFLAGS   "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CTUNING} -Wstrict-prototypes")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CTUNING} -fpermissive")
SET(ASM_OPTIONS "-x assembler-with-cpp")

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS "${CFLAGS} ${ASM_OPTIONS}")

# Project setup
include_directories(${INC_PATH} ${SRC_PATH} ${LIB_PATH} ${FONT_PATH} ${ARDUINO_PATH} ${CHECKSUM_PATH} ${USBDRV_PATH})

add_executable(${PROJECT_NAME} ${SRC_FILES} ${LIB_FILES} ${ARDUINO_FILES} ${CHECKSUM_FILES} ${USBDRV_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

# Compiling targets
add_custom_target(strip ALL     ${AVRSTRIP} "${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" DEPENDS strip)
add_custom_target(eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.eeprom" DEPENDS strip)

if(PROG_TYPE STREQUAL "usbasp")
    add_custom_target(flash
            ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} -U flash:w:${PROJECT_NAME}.hex
            DEPENDS hex
            COMMENT "Flashing with UsbASP programmer"
    )
elseif(PROG_TYPE STREQUAL "arduino")
    add_custom_target(flash
            ${AVRDUDE} -c arduino -P ${COM_PORT} -b ${BAUD} -p ${MCU} -D -U flash:w:${PROJECT_NAME}.hex:a
            DEPENDS hex
            COMMENT "Flashing with Arduino programmer on ${COM_PORT}"
    )
elseif(PROG_TYPE STREQUAL "c232hm")
    add_custom_target(flash
            ${AVRDUDE} -c c232hm -p ${MCU} -D -U flash:w:${PROJECT_NAME}.hex:a
            DEPENDS hex
            COMMENT "Flashing with C232HM programmer"
    )
else()
    message(FATAL_ERROR "Unknown programmer type: ${PROG_TYPE}. Supported types: usbasp, arduino, c232hm")
endif()

# Optional bootloader targets
add_custom_target(flash_optiboot_c232hm
        ${AVRDUDE} -c c232hm -p ${MCU} -D -U flash:w:${BASE_PATH}/bootloader/optiboot_flash_atmega644p_UART0_115200_12000000L_B7_BIGBOOT.hex:a
        COMMENT "Flashing Optiboot bootloader with C232HM programmer"
)
add_custom_target(flash_bootloader_c232hm
        ${AVRDUDE} -c c232hm -p ${MCU} -D -U flash:w:${BASE_PATH}/bootloader/atmega644pa-12mhz_2048.hex:a
        COMMENT "Flashing UsbASP bootloader with C232HM programmer"
)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_NAME}.hex;${PROJECT_NAME}.eeprom;${PROJECT_NAME}.lst")
