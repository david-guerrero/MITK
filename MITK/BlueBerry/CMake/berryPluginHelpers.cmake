# _MACRO_CREATE_PLUGIN_NAME(output_name
#                           INPUT dir
#                           [BASEDIR basedir]
#                           )
#
# sets output_name to the plugin-name
# the plugin-name is computed by removing basedir from dir and
# replacing slashes with points
#
# Example: BASEDIR=/root
#          INPUT=/root/org/blueberry/ui.qt
#          output_name=org.blueberry.ui.qt
#
MACRO(_MACRO_CREATE_PLUGIN_NAME output_name)

  MACRO_PARSE_ARGUMENTS(_CREATE_NAME "INPUT;BASEDIR" "" ${ARGN})

  STRING(REPLACE "${_CREATE_NAME_BASEDIR}/" "" _name_with_slashes ${_CREATE_NAME_INPUT})
  STRING(REPLACE "/" "." _name_with_points ${_name_with_slashes})

  SET(${output_name} ${_name_with_points})

ENDMACRO()

MACRO(_MACRO_REQUIRED_BUNDLES_LIST output_list input_file)

  SET(_file "${input_file}")
  FILE(STRINGS "${_file}" _mfline LENGTH_MINIMUM 1 REGEX " *Require-Bundle *: *[^ ]+")
  STRING(REGEX REPLACE "^[^:]*[:](.*)" "\\1" _reqb_input "${_mfline}")

  SET(_reqb_dependencies )
  IF(_reqb_input)
    STRING(REPLACE "," ";" _reqb_dependencies ${_reqb_input})
  ENDIF(_reqb_input)

  FOREACH(_dep ${_reqb_dependencies})
    # remove leading and trailing whitespace
    STRING(STRIP ${_dep} _trimeddep)
    LIST(APPEND ${output_list} ${_trimeddep})
  ENDFOREACH(_dep ${_reqb_dependencies})
ENDMACRO()



# _MACRO_SETUP_PLUGIN_DEPENDENCIES(
#      _explicit_libs
#      PLUGIN_NAME _plugin_name
#      )
#
MACRO(_MACRO_SETUP_PLUGIN_DEPENDENCIES _explicit_libs)

  SET(_plugin_src_dir ${${BUNDLE-SYMBOLICNAME}_SRC_DIR})
  SET(_plugin_dependencies ${REQUIRE-BUNDLE})
  SET(_bundle_id ${BUNDLE-SYMBOLICNAME})
  
  FOREACH(_dep ${_plugin_dependencies})
    STRING(REPLACE . _ _dep_target ${_dep})
    SET(${_explicit_libs} ${${_explicit_libs}} ${_dep_target})
  ENDFOREACH()

  SET(_plugin_old_dependencies "")
  SET(_dep_error 0)
  WHILE(NOT "${_plugin_dependencies}" STREQUAL "${_plugin_old_dependencies}")
    SET(_plugin_old_dependencies ${_plugin_dependencies})
    
    FOREACH(_dep ${_plugin_dependencies})

      SET(_plugin_src_dir ${${_dep}_SRC_DIR})
      SET(_is_ctk_dep )
      IF(NOT _plugin_src_dir) # maybe it is a CTK plugin dependency
        STRING(REPLACE . _ _dep_target ${_dep})
        #MESSAGE("check if ${_dep_target}_DEPENDENCIES is set: ${${_dep_target}_DEPENDENCIES}")
        #MESSAGE("looking if ${_dep_target} is in: ${CTK_PLUGIN_LIBRARIES}")
        # Check if it is an external CTK plugin dependency
        ctkMacroGetAllNonProjectTargetLibraries("${CTK_PLUGIN_LIBRARIES}" _external_ctk_plugins)
        LIST(FIND _external_ctk_plugins ${_dep_target} dep_found)
        IF(dep_found GREATER -1)
          SET(_BUILD_${_dep} ON) # Mark it as available
          SET(_is_ctk_dep 1)
        ELSE()
          #Check if it is an internal CTK plugin dependency
          IF(${_dep_target}_DEPENDENCIES)
            SET(_BUILD_${_dep} ON)
            GetMyTargetLibraries("${${_dep_target}_DEPENDENCIES}" _ctk_deps)
            #MESSAGE("********** _ctk_deps: ${_ctk_deps}")
            FOREACH(_ctk_dep_target ${_ctk_deps})
              STRING(REPLACE _ . _ctk_dep ${_ctk_dep_target})
              LIST(APPEND _plugin_dependencies ${_ctk_dep})
              SET(_BUILD_${_ctk_dep} ON)
            ENDFOREACH()
            #MESSAGE("******* new dependencies: ${_plugin_dependencies}")
            SET(_is_ctk_dep 1)
            #MESSAGE("new _plugin_dependencies: ${_plugin_dependencies}")
          ENDIF()
        ENDIF()
      ENDIF()

      IF(NOT _is_ctk_dep)
        IF(NOT _plugin_src_dir)
          MESSAGE(SEND_ERROR "Plug-in dependency \"${_dep}\" does not seem to exist.")
          SET(_dep_error 1)
          BREAK()
        ENDIF()

        #_MACRO_REQUIRED_BUNDLES_LIST(_plugin_dependencies "${_plugin_src_dir}/META-INF/MANIFEST.MF")
        INCLUDE(${${_dep}_BIN_DIR}/RequireBundle.cmake)
        LIST(APPEND _plugin_dependencies ${REQUIRE-BUNDLE})
      ENDIF()

    ENDFOREACH()
    
    LIST(REMOVE_DUPLICATES _plugin_dependencies)
    LIST(SORT _plugin_dependencies)
    
  ENDWHILE()

  IF(NOT _dep_error)
    # iterator over all dependencies
    SET(_plugins_turned_off )
    SET(PLUGIN_LINK_LIBRARIES )
    FOREACH(_dep ${_plugin_dependencies})

      # collect the ids of plugins we will have to build
      IF(NOT _BUILD_${_dep})
        LIST(APPEND _plugins_turned_off ${_dep})
      ENDIF()
    
      # set include and link directories
      STRING(REPLACE . _ _dep_target ${_dep})
      IF(${_dep}_SRC_DIR)
        # This is an external BlueBerry Bundle dependency
        #MESSAGE("${_dep} is an external BlueBerry Bundle dependency")
        SET(_dep_src_dir ${${_dep}_SRC_DIR}/src)

        IF(EXISTS "${${_dep}_SRC_DIR}/includes.cmake")
          INCLUDE(${${_dep}_SRC_DIR}/includes.cmake)
          FOREACH(_added_incldir ${ADDITIONAL_INCLUDE_DIRECTORIES})
            LIST(APPEND _dep_src_dir ${${_dep}_SRC_DIR}/${_added_incldir})
          ENDFOREACH()
        ENDIF()
      ELSEIF(${_dep_target}_INCLUDE_DIRS)
        # This is an external CTK Plugin dependency
        #MESSAGE("${_dep} is an external CTK Plugin dependency")
        SET(_dep_src_dir ${${_dep_target}_INCLUDE_DIRS})
      ELSEIF(${_dep}_SOURCE_DIR)
        # This is an internal BlueBerry Bundle dependency
        #MESSAGE("${_dep} is an internal BlueBerry Bundle dependency")
        SET(_dep_src_dir ${${_dep}_SOURCE_DIR}/src)

        IF(EXISTS "${${_dep}_SOURCE_DIR}/includes.cmake")
          INCLUDE(${${_dep}_SOURCE_DIR}/includes.cmake)
          FOREACH(_added_incldir ${ADDITIONAL_INCLUDE_DIRECTORIES})
            LIST(APPEND _dep_src_dir ${${_dep}_SOURCE_DIR}/${_added_incldir})
          ENDFOREACH()
        ENDIF()
      ELSEIF(${_dep_target}_SOURCE_DIR)
        # This is an internal CTK plugin dependency
        #MESSAGE("${_dep} is an internal CTK plugin dependency")
        SET(_dep_src_dir )
        ctkFunctionGetIncludeDirs(_dep_src_dir ${_dep_target})

        IF(${_dep_target}_INCLUDE_SUFFIXES)
          FOREACH(_suffix ${${_dep_target}_INCLUDE_SUFFIXES})
            LIST(APPEND _dep_src_dir ${${_dep_target}_SOURCE_DIR}/${_suffix})
          ENDFOREACH()
        ELSE()
          LIST(APPEND _dep_src_dir ${${_dep_target}_SOURCE_DIR})
        ENDIF()
      ENDIF()

      SET(_dep_out_dir ${${_dep}_OUT_DIR})

      IF(${_dep}_BIN_DIR)
        SET(_dep_bin_dir ${${_dep}_BIN_DIR})
      ELSE()
        SET(_dep_bin_dir ${${_dep_target}_BINARY_DIR})
      ENDIF()
        
      LINK_DIRECTORIES(${_dep_out_dir}/bin)
      INCLUDE_DIRECTORIES(${_dep_src_dir})
      INCLUDE_DIRECTORIES(${_dep_bin_dir})

      LIST(APPEND PLUGIN_LINK_LIBRARIES ${_dep_target})
    
    ENDFOREACH(_dep ${_plugin_dependencies})
      
    IF(_plugins_turned_off)
      MESSAGE(SEND_ERROR "Unmet dependencies: The plugin ${_bundle_id} depends on the following plugins:\n${_plugins_turned_off}.\nSwitch them on in order to build ${_FIND_DEPS_PLUGIN_NAME}.")
    ENDIF(_plugins_turned_off)
  ENDIF()

ENDMACRO()

MACRO(_MACRO_ENABLE_QT4_PLUGINS)

  SET(_enable_qt_plugin ${ARGV0})
  STRING(REGEX MATCH ".*\\.qt.*" _qt_matched ${_enable_qt_plugin})
  IF(_qt_matched)
    IF(QT4_FOUND)
      SET(ENABLE_PLUGIN 1)
    ELSE()
      SET(ENABLE_PLUGIN 0)
    ENDIF()
  ENDIF()

ENDMACRO()

MACRO(_MACRO_CREATE_ENABLE_PLUGIN_CODE)
    SET(_enable_plugins_file "${CMAKE_CURRENT_BINARY_DIR}/berryEnablePlugin.cmake")
    SET(_enable_plugins_filecontent "SET(_enable_bundle 1)")
    FOREACH(_macro_name ${ARGN})
      SET(_enable_plugins_filecontent "${_enable_plugins_filecontent}
      IF(_enable_bundle)
        SET(ENABLE_PLUGIN 1)
        ${_macro_name}(\${BUNDLE-SYMBOLICNAME})
        IF(NOT ENABLE_PLUGIN)
          SET(_enable_bundle 0)
        ENDIF()
      ENDIF()")
    ENDFOREACH()
    CONFIGURE_FILE("${BLUEBERRY_SOURCE_DIR}/CMake/berryEnablePlugin.cmake.in" "${_enable_plugins_file}" @ONLY)
ENDMACRO()


