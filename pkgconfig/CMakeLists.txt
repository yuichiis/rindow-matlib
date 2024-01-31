add_custom_target(pkgconfig ALL 
                   COMMAND sed -e s@%RINDOW_MATLIB_VERSION%@${PROJECT_VERSION}@ ${CMAKE_CURRENT_SOURCE_DIR}/rindowmatlib.pc.in | sed -e s@%INSTALL_DIR%@${CMAKE_INSTALL_PREFIX}@ > ${CMAKE_CURRENT_BINARY_DIR}/rindowmatlib.pc)
install(FILES rindowmatlib.pc DESTINATION lib/pkgconfig)
