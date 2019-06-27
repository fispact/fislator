project(TRANSLATE)

# program name and sources
set (TRANSLATE_EXE_NAME fislator)
set (TRANSLATE_EXE_SOURCES main.cpp)

# Make the executable
add_executable (${TRANSLATE_EXE_NAME} ${TRANSLATE_EXE_SOURCES})

# Set the exe properties
set_target_properties(${TRANSLATE_EXE_NAME}
                      PROPERTIES OUTPUT_NAME ${TRANSLATE_EXE_NAME}
                      VERSION ${FISLATOR_PROJECT_VERSION}
                      RUNTIME_OUTPUT_DIRECTORY ${FISLATOR_BUILD_DIR}/bin)