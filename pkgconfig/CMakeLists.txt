add_custom_command(OUTPUT rindowmatlib.pc
                   COMMAND sed -e s@%RINDOW_MATLIB_VERSION%@${PROJECT_VERSION}@ rindowmatlib.pc.in | \
                   sed -e s@%INSTALL_DIR%@${CMAKE_INSTALL_FULL_LIBDIR}@ > rindowmatlib.pc
install(FILES rindowmatlib.pc DESTINATION lib/pkgconfig)
