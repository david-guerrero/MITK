set(LIBNAME "PocoZip")
#set(LIBNAMED "${LIBNAME}d")

aux_source_directory(src SRCS)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} PROPERTIES 
  DEFINE_SYMBOL Zip_EXPORTS
#COMPILE_FLAGS ${DEBUG_CXX_FLAGS}
	VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}
	SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
target_link_libraries( ${LIBNAME} PocoUtil PocoXML PocoFoundation)

#add_library( ${LIBNAMED} ${LIB_MODE} ${SRCS} )
#set_target_properties( ${LIBNAMED}  
#	PROPERTIES COMPILE_FLAGS "${RELEASE_CXX_FLAGS}" 
#	VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}
#	SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
#target_link_libraries( ${LIBNAMED} PocoUtild PocoXMLd PocoFoundationd)

#add_subdirectory(samples)
#add_subdirectory(testsuite)
