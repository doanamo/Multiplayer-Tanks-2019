# Launcher extecutable target.
add_executable("Launcher" "Launcher.cpp")

# Add game target as dependency.
add_dependencies("Launcher" "Game")

# Set executable output directory (must be the same as game output directory).
set_target_properties("Launcher" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Output")

# Set main build directory as the working directory.
set_target_properties("Launcher" PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Deploy/")
