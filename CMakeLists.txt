if(CONFIG_SHAPELIB)

set(SHAPELIB_DIR ${ZEPHYR_CURRENT_MODULE_DIR})

zephyr_library()

zephyr_include_directories(
    ${SHAPELIB_DIR}/
)

zephyr_library_sources(
    ${SHAPELIB_DIR}/shpopen.c
    ${SHAPELIB_DIR}/dbfopen.c
    ${SHAPELIB_DIR}/shptree.c
    ${SHAPELIB_DIR}/sbnsearch.c
    ${SHAPELIB_DIR}/shapefil.h
    shapefil_private.h
    ${SHAPELIB_DIR}/shapelib.def
    
    # For Zephyr
    safileio.c
)

endif()