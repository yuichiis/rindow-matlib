project(pkgconfig)

add_custom_command(
    OUTPUT rindowmatlib.pc
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/rindowmatlib.pc.in
    COMMAND sed -e s@%RINDOW_MATLIB_VERSION%@${PROJECT_VERSION}@ ${CMAKE_CURRENT_SOURCE_DIR}/rindowmatlib.pc.in | sed -e s@%INSTALL_DIR%@${CMAKE_INSTALL_PREFIX}@ > ${CMAKE_CURRENT_BINARY_DIR}/rindowmatlib.pc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/rindowmatlib.pc DESTINATION lib/pkgconfig)
