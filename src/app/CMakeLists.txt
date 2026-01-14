add_executable(FEM-program
    mainwindow.cpp
    main.cpp
    context-menu/treeContextMenu.cpp
     ../graphics/qtgl/qtgl.cpp
     # модули FEM
     ../mesh/mesh.cpp
     ${CMAKE_HOME_DIRECTORY}/src/elements/displacement/displacement.cpp
   ${CMAKE_HOME_DIRECTORY}/src/elements/load/load.cpp
   ${CMAKE_HOME_DIRECTORY}/src/elements/plates/plates.cpp
   ${CMAKE_HOME_DIRECTORY}/src/elements/element.cpp
   ${CMAKE_HOME_DIRECTORY}/src/elements/load/femload.cpp
#   ${CMAKE_HOME_DIRECTORY}/src/elements/material/PlateMaterial.cpp

)


target_link_libraries(FEM-program PRIVATE Qt6::Widgets Qt6::OpenGLWidgets Qt6::Gui opengl Eigen3::Eigen)

set_target_properties(FEM-program PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)