# Define source files.
set(IMGUI_SFML_FILES
    "imconfig-SFML.h"
    "imgui-SFML.cpp"
    "imgui-SFML.h"
)

# Create a static library.
add_library("SFML-ImGui" STATIC ${IMGUI_SFML_FILES})
target_include_directories("SFML-ImGui" PRIVATE "../" "../../SFML-2.5.1/include")
target_include_directories("SFML-ImGui" PUBLIC "./")

# Link SFML libraries.
target_link_libraries("SFML-ImGui" "sfml-system")
target_link_libraries("SFML-ImGui" "sfml-window")
target_link_libraries("SFML-ImGui" "sfml-graphics")
target_link_libraries("SFML-ImGui" "ImGui")
