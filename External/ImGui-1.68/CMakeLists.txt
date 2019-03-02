# Define source files.
set(IMGUI_FILES
    "imconfig.h"
    "imgui.cpp"
    "imgui.h"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_internal.h"
    "imgui_widgets.cpp"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
)

# Create a static library.
add_library("ImGui" STATIC ${IMGUI_FILES})
target_include_directories("ImGui" PUBLIC "./" "./examples")
