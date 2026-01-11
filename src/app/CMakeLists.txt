add_executable(FEM-program
    mainwindow.cpp
    main.cpp
    context-menu/treeContextMenu.cpp
    ../graphics/qtgl/qtgl.cpp
)

target_link_libraries(FEM-program PRIVATE Qt6::Widgets Qt6::OpenGLWidgets opengl)

set_target_properties(FEM-program PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)