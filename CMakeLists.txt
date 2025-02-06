cmake_minimum_required(VERSION 3.5)

set(PROG_TYPE usbasp)


# Variables regarding the AVR chip
set(MCU   atmega644p)
set(F_CPU 12000000)
set(BAUD  9600)
set(PICTOR_FASTMODE)
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

# Files to be compiled
file(GLOB_RECURSE SRC_FILES
        "${SRC_PATH}/*.cpp"
        "${SRC_PATH}/*.cc"
        "${SRC_PATH}/*.c"
        "${SRC_PATH}/*.cxx"
        "${SRC_PATH}/*.S"
        "${SRC_PATH}/*.s"
        "${SRC_PATH}/*.sx"
        "${SRC_PATH}/*.asm"
)

file(GLOB_RECURSE LIB_FILES
        "${LIB_PATH}/*.cpp"
        "${LIB_PATH}/*.cc"
        "${LIB_PATH}/*.c"
        "${LIB_PATH}/*.cxx"
        "${LIB_PATH}/*.S"
        "${LIB_PATH}/*.s"
        "${LIB_PATH}/*.sx"
        "${LIB_PATH}/*.asm"
)

file(GLOB_RECURSE FONT_FILES
        "${FONT_FILES}/*.h"
)

# Compiler flags

set(CDEBUG    "-gstabs -g -ggdb")
set(CWARN     "-Wall -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
set(COPT      "-Os -lm -lprintf_flt")
set(CMCU      "-mmcu=${MCU}")
set(CDEFS     "-DF_CPU=${F_CPU} -DBAUD=${BAUD}")

set(CFLAGS   "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CTUNING} -fpermissive")

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")

# Project setup
include_directories(${INC_PATH} ${SRC_PATH} ${LIB_PATH} ${FONT_PATH})

add_executable(${PROJECT_NAME} ${SRC_FILES} ${LIB_FILES} ${FONT_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

# Compiling targets
add_custom_target(strip ALL     ${AVRSTRIP} "${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" DEPENDS strip)
add_custom_target(eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.eeprom" DEPENDS strip)

add_custom_target(flash ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex)


set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_NAME}.hex;${PROJECT_NAME}.eeprom;${PROJECT_NAME}.lst")
