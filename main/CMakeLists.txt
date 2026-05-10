if("${IDF_TARGET}" STREQUAL "linux")
    idf_component_register(SRC_DIRS "src"
                       INCLUDE_DIRS "include"
                       REQUIRES cpong)
else()
    idf_component_register(SRC_DIRS "src"
                       INCLUDE_DIRS "include"
                       REQUIRES cpong simple_wifi)
endif()
