### CMake targets ###

add_library(cmsis_aws_iot_backoff_algorithm)
target_sources(cmsis_aws_iot_backoff_algorithm PUBLIC ${BACKOFF_ALGORITHM_SOURCES})
target_include_directories(cmsis_aws_iot_backoff_algorithm PUBLIC ${BACKOFF_ALGORITHM_INCLUDE_PUBLIC_DIRS})
set_target_properties(cmsis_aws_iot_backoff_algorithm PROPERTIES DEFINE_SYMBOL "")
set_target_properties(cmsis_aws_iot_backoff_algorithm PROPERTIES LINKER_LANGUAGE C)

add_library(cmsis_aws_iot_defender)
target_sources(cmsis_aws_iot_defender PUBLIC ${DEFENDER_SOURCES})
target_include_directories(cmsis_aws_iot_defender PUBLIC ${DEFENDER_INCLUDE_PUBLIC_DIRS})
set_target_properties(cmsis_aws_iot_defender PROPERTIES DEFINE_SYMBOL "")
set_target_properties(cmsis_aws_iot_defender PROPERTIES LINKER_LANGUAGE C)

add_library(cmsis_aws_iot_shadow)
target_sources(cmsis_aws_iot_shadow PUBLIC ${SHADOW_SOURCES})
target_include_directories(cmsis_aws_iot_shadow PUBLIC ${SHADOW_INCLUDE_PUBLIC_DIRS})
set_target_properties(cmsis_aws_iot_shadow PROPERTIES DEFINE_SYMBOL "")
set_target_properties(cmsis_aws_iot_shadow PROPERTIES LINKER_LANGUAGE C)

add_library(cmsis_aws_iot_jobs)
target_sources(cmsis_aws_iot_jobs PUBLIC ${JOBS_SOURCES})
target_include_directories(cmsis_aws_iot_jobs PUBLIC ${JOBS_INCLUDE_PUBLIC_DIRS})
set_target_properties(cmsis_aws_iot_jobs PROPERTIES DEFINE_SYMBOL "")
set_target_properties(cmsis_aws_iot_jobs PROPERTIES LINKER_LANGUAGE C)

add_library(cmsis_aws_iot_ota)
target_sources(cmsis_aws_iot_ota PUBLIC ${OTA_SOURCES})
target_include_directories(cmsis_aws_iot_ota PUBLIC ${OTA_INCLUDE_PUBLIC_DIRS} ${OTA_INCLUDE_PRIVATE_DIRS})
set_target_properties(cmsis_aws_iot_ota PROPERTIES DEFINE_SYMBOL "")
set_target_properties(cmsis_aws_iot_ota PROPERTIES LINKER_LANGUAGE C)

add_library(cmsis_aws_iot_mqtt)
target_sources(cmsis_aws_iot_mqtt PUBLIC ${MQTT_SOURCES} ${MQTT_SERIALIZER_SOURCES})
target_include_directories(cmsis_aws_iot_mqtt PUBLIC ${MQTT_INCLUDE_PUBLIC_DIRS})
set_target_properties(cmsis_aws_iot_mqtt PROPERTIES DEFINE_SYMBOL "")
set_target_properties(cmsis_aws_iot_mqtt PROPERTIES LINKER_LANGUAGE C)

add_library(cmsis_aws_iot_http)
target_sources(cmsis_aws_iot_http PUBLIC ${HTTP_SOURCES})
target_include_directories(cmsis_aws_iot_http PUBLIC ${HTTP_INCLUDE_PUBLIC_DIRS})
set_target_properties(cmsis_aws_iot_http PROPERTIES DEFINE_SYMBOL "")
set_target_properties(cmsis_aws_iot_http PROPERTIES LINKER_LANGUAGE C)

add_library(cmsis_aws_iot_ota_mqtt)
target_sources(cmsis_aws_iot_ota_mqtt PUBLIC ${OTA_MQTT_SOURCES})
target_include_directories(cmsis_aws_iot_ota_mqtt PUBLIC ${OTA_MQTT_INCLUDE_PUBLIC_DIRS})
set_target_properties(cmsis_aws_iot_ota_mqtt PROPERTIES DEFINE_SYMBOL "")
set_target_properties(cmsis_aws_iot_ota_mqtt PROPERTIES LINKER_LANGUAGE C)

add_library(cmsis_aws_iot_ota_http)
target_sources(cmsis_aws_iot_ota_http PUBLIC ${OTA_HTTP_SOURCES})
target_include_directories(cmsis_aws_iot_ota_http PUBLIC ${OTA_HTTP_INCLUDE_PUBLIC_DIRS})
set_target_properties(cmsis_aws_iot_ota_http PROPERTIES DEFINE_SYMBOL "")
set_target_properties(cmsis_aws_iot_ota_http PROPERTIES LINKER_LANGUAGE C)

add_library(cmsis_aws_iot_ota_freertos)
target_sources(cmsis_aws_iot_ota_freertos PUBLIC ${OTA_OS_FREERTOS_SOURCES})
target_include_directories(cmsis_aws_iot_ota_freertos PUBLIC ${OTA_INCLUDE_OS_FREERTOS_DIRS})
set_target_properties(cmsis_aws_iot_ota_freertos PROPERTIES DEFINE_SYMBOL "")
set_target_properties(cmsis_aws_iot_ota_freertos PROPERTIES LINKER_LANGUAGE C)

add_library(cmsis_aws_iot_json)
target_sources(cmsis_aws_iot_json PUBLIC ${JSON_SOURCES})
target_include_directories(cmsis_aws_iot_json PUBLIC ${JSON_INCLUDE_PUBLIC_DIRS})
set_target_properties(cmsis_aws_iot_json PROPERTIES DEFINE_SYMBOL "")
set_target_properties(cmsis_aws_iot_json PROPERTIES LINKER_LANGUAGE C)

add_library(cmsis_tinycbor)
target_sources(cmsis_tinycbor PUBLIC ${TINYCBOR_SOURCES})
target_include_directories(cmsis_tinycbor PUBLIC ${TINYCBOR_INCLUDE_DIRS})
set_target_properties(cmsis_tinycbor PROPERTIES DEFINE_SYMBOL "")
set_target_properties(cmsis_tinycbor PROPERTIES LINKER_LANGUAGE C)

# OTA_SOURCES includes JSON_SOURCES variable from libraries/aws/ota-for-aws-iot-embedded-sdk/source/dependency/coreJSON/jsonFilePaths.cmake instead of libraries/standard/coreJSON/jsonFilePaths.cmake
# Such multiple checkouts of a single dependency should be ideally cleaned up.
include(${CMAKE_CURRENT_LIST_DIR}/libraries/aws/ota-for-aws-iot-embedded-sdk/source/dependency/coreJSON/jsonFilePaths.cmake)
add_library(cmsis_aws_iot_ota_json)
target_sources(cmsis_aws_iot_ota_json PUBLIC ${JSON_SOURCES})
target_include_directories(cmsis_aws_iot_ota_json PUBLIC ${JSON_INCLUDE_PUBLIC_DIRS})
set_target_properties(cmsis_aws_iot_ota_json PROPERTIES DEFINE_SYMBOL "")
set_target_properties(cmsis_aws_iot_ota_json PROPERTIES LINKER_LANGUAGE C)
