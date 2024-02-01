
#set(CPACK_PACKAGE_NAME "rindow-matlib")
#set(CPACK_PACKAGE_VERSION "0.1.0")
#set(CPACK_PACKAGE_DESCRIPTION "Rindow Math Matrix library")
#set(CPACK_PACKAGE_CONTACT "Rindow Developers <rindow@users.noreply.github.com>")
#set(CPACK_PACKAGE_LICENSE "BSD3-Clause")

set(CPACK_COMPONENTS_ALL  ${PROJECT_NAME})
# these are cache variables, so they could be overwritten with -D,
set(CPACK_PACKAGE_NAME ${PROJECT_NAME}
    CACHE STRING "The resulting package name"
)
# which is useful in case of packing only selected components instead of the whole thing
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Rindow Math Matrix library"
    CACHE STRING "Package description for the package metadata"
)
set(CPACK_PACKAGE_VENDOR "Rindow")


set(CPACK_VERBATIM_VARIABLES YES)

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
SET(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_SOURCE_DIR}/_packages")

# https://unix.stackexchange.com/a/11552/254512
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")#/${CMAKE_PROJECT_VERSION}")

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
file(STRINGS "/etc/os-release" data_list REGEX "^(ID|VERSION_ID|VERSION_CODENAME)=")
foreach(_var ${data_list})
if("${_var}" MATCHES "^(ID)=(.*)$")
  set(_var_id "${CMAKE_MATCH_2}")
elseif("${_var}" MATCHES "^(VERSION_ID)=(.*)$")
  set(_var_version_id "${CMAKE_MATCH_2}")
elseif("${_var}" MATCHES "^(VERSION_CODENAME)=(.*)$")
  set(_var_codename "${CMAKE_MATCH_2}")
endif()
endforeach()
set(TEST_PACKAGE_VERSION "${_var_id}${_var_version_id}${_var_codename}")

set(CPACK_PACKAGE_CONTACT "Rindow Developers <rindow@users.noreply.github.com>")
#set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Rindow Developers <rindow@users.noreply.github.com>")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://rindow.github.io/mathematics/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# package name for deb. If set, then instead of some-application-0.9.2-Linux.deb
# you'll get some-application_0.9.2_amd64.deb (note the underscores too)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
# that is if you want every group to have its own package,
# although the same will happen if this is not set (so it defaults to ONE_PER_GROUP)
# and CPACK_DEB_COMPONENT_INSTALL is set to YES
set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)#ONE_PER_GROUP)
# without this you won't be able to pack only specified component
set(CPACK_DEB_COMPONENT_INSTALL YES)

include(CPack)
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.7)")

#set(CPACK_INSTALL_CMAKE_PROJECTS "pkgwork rindow_matlib ....")
