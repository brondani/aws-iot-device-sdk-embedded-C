# CMSIS pack generation macros
# Requirements: PackChk and 7zip

cmake_minimum_required(VERSION 3.15)

set(CMSIS_TOOLS_DIR "${CMAKE_CURRENT_LIST_DIR}")

macro(cmsis_set_pack_info name description vendor license url version date changelog output)
  set(CMSIS_PACK_NAME ${name})
  set(CMSIS_PACK_DESCRIPTION ${description})
  set(CMSIS_PACK_VENDOR ${vendor})
  set(CMSIS_PACK_LICENSE ${license})
  set(CMSIS_PACK_URL ${url})
  set(CMSIS_PACK_VERSION ${version})
  set(CMSIS_PACK_DATE ${date})
  set(CMSIS_PACK_CHANGE_LOG ${changelog})
  set(CMSIS_PDSC_FILE "${CMSIS_PACK_VENDOR}.${CMSIS_PACK_NAME}.pdsc")
  set(CMSIS_PACK_FILE "${CMSIS_PACK_VENDOR}.${CMSIS_PACK_NAME}.${CMSIS_PACK_VERSION}.pack")
  set(CMSIS_OUTPUT_DIR ${output})
  file(COPY ${CMSIS_PACK_LICENSE} DESTINATION ${CMSIS_OUTPUT_DIR})
endmacro()

macro(cmsis_add_component component class group subgroup variant version description root)
  list(APPEND COMPONENTS "${component}")
  set(${component}_class "${class}")
  set(${component}_group "${group}")
  set(${component}_subgroup "${subgroup}")
  set(${component}_variant "${variant}")
  set(${component}_version "${version}")
  set(${component}_description "${description}")
  set(${component}_root "${root}")
endmacro()

macro(cmsis_add_dependency component dependency class group subgroup variant version)
  list(APPEND DEPENDENCIES_${component} ${dependency})
  set(${component}_${dependency}_class "${class}")
  set(${component}_${dependency}_group "${group}")
  set(${component}_${dependency}_subgroup "${subgroup}")
  set(${component}_${dependency}_variant "${variant}")
  set(${component}_${dependency}_version "${version}")
endmacro()

macro(cmsis_set_condition_content component)
  string(APPEND CMSIS_PACK_CONDITIONS "\n    <condition id=\"${component} Condition\">")
  foreach(dependency ${DEPENDENCIES_${component}})
    string(APPEND CMSIS_PACK_CONDITIONS "\n      <require")
    if(NOT ${component}_${dependency}_class STREQUAL "")
      string(APPEND CMSIS_PACK_CONDITIONS " Cclass=\"${${component}_${dependency}_class}\"")
    endif()
    if(NOT ${component}_${dependency}_group STREQUAL "")
      string(APPEND CMSIS_PACK_CONDITIONS " Cgroup=\"${${component}_${dependency}_group}\"")
    endif()
    if(NOT ${component}_${dependency}_subgroup STREQUAL "")
      string(APPEND CMSIS_PACK_CONDITIONS " Csub=\"${${component}_${dependency}_subgroup}\"")
    endif()
    if(NOT ${component}_${dependency}_variant STREQUAL "")
      string(APPEND CMSIS_PACK_CONDITIONS " Cvariant=\"${${component}_${dependency}_variant}\"")
    endif()
    if(NOT ${component}_${dependency}_version STREQUAL "")
      string(APPEND CMSIS_PACK_CONDITIONS " Cversion=\"${${component}_${dependency}_version}\"")
    endif()
    string(APPEND CMSIS_PACK_CONDITIONS "/>")
  endforeach()
  string(APPEND CMSIS_PACK_CONDITIONS "\n    </condition>")
endmacro()

macro(cmsis_set_file_content component file category)
  file(RELATIVE_PATH relative ${CMAKE_CURRENT_LIST_DIR}/${${component}_root} ${file})
  string(PREPEND relative "${component}/")
  string(APPEND CMSIS_PACK_COMPONENTS "\n        <file category=\"${category}\" name=\"${relative}")
  if(${category} STREQUAL "include")
    string(APPEND CMSIS_PACK_COMPONENTS "/")
  endif()
  string(APPEND CMSIS_PACK_COMPONENTS "\"/>")
  get_filename_component(destination ${CMSIS_OUTPUT_DIR}/${relative} DIRECTORY)
  file(COPY ${file} DESTINATION ${destination})
endmacro()

macro(cmsis_set_component_content component)
  string(APPEND CMSIS_PACK_COMPONENTS "\n    <component")
  if(NOT ${component}_class STREQUAL "")
    string(APPEND CMSIS_PACK_COMPONENTS " Cclass=\"${${component}_class}\"")
  endif()
  if(NOT ${component}_group STREQUAL "")
    string(APPEND CMSIS_PACK_COMPONENTS " Cgroup=\"${${component}_group}\"")
  endif()
  if(NOT ${component}_subgroup STREQUAL "")
    string(APPEND CMSIS_PACK_COMPONENTS " Csub=\"${${component}_subgroup}\"")
  endif()
  if(NOT ${component}_variant STREQUAL "")
    string(APPEND CMSIS_PACK_COMPONENTS " Cvariant=\"${${component}_variant}\"")
  endif()
  if(NOT ${component}_version STREQUAL "")
    string(APPEND CMSIS_PACK_COMPONENTS " Cversion=\"${${component}_version}\"")
  endif()
  if(NOT ${component}_condition STREQUAL "")
    string(APPEND CMSIS_PACK_COMPONENTS " condition=\"${component} Condition\"")
  endif()
  string(APPEND CMSIS_PACK_COMPONENTS ">\n      <description>${${component}_description}</description>\n      <files>")
  string(TOUPPER ${component} component_uppercase)
  foreach(src ${${component_uppercase}_SOURCES})
    cmsis_set_file_content(${component} ${src} source)
  endforeach()
  foreach(inc ${${component_uppercase}_INCLUDE_PUBLIC_DIRS})
    cmsis_set_file_content(${component} ${inc} include)
  endforeach()
  string(APPEND CMSIS_PACK_COMPONENTS "\n      </files>\n    </component>")
endmacro()

macro(cmsis_generate_pack)
  # Iterate over components
  foreach(component ${COMPONENTS})
    string(TOLOWER ${component} component_lowercase)
    include(${${component}_root}/${component_lowercase}FilePaths.cmake)
    cmsis_set_condition_content(${component})
    cmsis_set_component_content(${component})
  endforeach()

  # Generate PDSC file
  configure_file(${CMSIS_TOOLS_DIR}/template.pdsc ${CMSIS_OUTPUT_DIR}/${CMSIS_PDSC_FILE})

  # Pack checking utility check
  # https://github.com/ARM-software/CMSIS_5/tree/master/CMSIS/Utilities/<os>/PackChk
  find_program (packchk PackChk)
  if(${packchk} STREQUAL "packchk-NOTFOUND")
    message("Error: No PackChk Utility found\nAction: Add PackChk to your path")
  else()
    execute_process(COMMAND ${packchk} ${CMSIS_PDSC_FILE} -x M362 WORKING_DIRECTORY ${CMSIS_OUTPUT_DIR})
  endif()

  # Compress files with 7zip
  find_program (zip 7z)
  if(${zip} STREQUAL "zip-NOTFOUND")
    message("Error: No 7zip Utility found\nAction: Add 7zip to your path")
  else()
    execute_process(COMMAND ${zip} a ${CMSIS_PACK_FILE} -tzip WORKING_DIRECTORY ${CMSIS_OUTPUT_DIR})
  endif()
endmacro()
