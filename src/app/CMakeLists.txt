add_executable(FEM-program
    mainwindow.cpp
    main.cpp
    context-menu/treeContextMenu.cpp
     ../graphics/qtgl/qtgl.cpp
     # модули FEM
     ${CMAKE_HOME_DIRECTORY}/src/mesh/mesh.cpp
   ${CMAKE_HOME_DIRECTORY}/src/generalElement/load/load.cpp
   ${CMAKE_HOME_DIRECTORY}/src/generalElement/displacement/displacement.cpp
   ${CMAKE_HOME_DIRECTORY}/src/elements/plates/plates.cpp
   ${CMAKE_HOME_DIRECTORY}/src/elements/femelement.cpp
   ${CMAKE_HOME_DIRECTORY}/src/elements/load/femload.cpp
   ${CMAKE_HOME_DIRECTORY}/src/elements/displacement/femdisplacement.cpp
   ${CMAKE_HOME_DIRECTORY}/src/elements/elementprovider.cpp
   ${CMAKE_HOME_DIRECTORY}/src/solver/solver.cpp
#   ${CMAKE_HOME_DIRECTORY}/src/elements/material/PlateMaterial.cpp
)

target_include_directories(FEM-program
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/app
        ${CMAKE_CURRENT_SOURCE_DIR}/src/app/context-menu
        ${CMAKE_CURRENT_SOURCE_DIR}/src/elements
        ${CMAKE_CURRENT_SOURCE_DIR}/src/elements/load
        ${CMAKE_CURRENT_SOURCE_DIR}/src/elements/displacement
        # ${CMAKE_CURRENT_SOURCE_DIR}/src/elements/material
        ${CMAKE_CURRENT_SOURCE_DIR}/src/elements/plates
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh
        ${CMAKE_CURRENT_SOURCE_DIR}/src/solver
)


target_link_libraries(FEM-program PRIVATE Qt6::Widgets Qt6::OpenGLWidgets Qt6::Gui Eigen3::Eigen)

set_target_properties(FEM-program PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)