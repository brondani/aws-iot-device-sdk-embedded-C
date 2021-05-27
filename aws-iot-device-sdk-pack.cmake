# Call this script to generate an example of a CMSIS pack
# git submodule update --init --recursive
# cmake -P aws-iot-device-sdk-pack.cmake

include (tools/cmsis/packgen.cmake)

### Set Pack Info ###

# cmsis_set_pack_info arguments:
#   name
#   description
#   vendor
#   license
#   url
#   version
#   date
#   changelog
#   output

cmsis_set_pack_info(
  "IoT C-SDK"
  "The AWS IoT Device SDK for Embedded C is a collection of C source\n\
      files that can be used in embedded applications to securely connect\n\
      to AWS IoT Core and interact with other AWS IoT services on AWS Cloud."
  "AWS"
  "LICENSE"
  "https://github.com/aws/aws-iot-device-sdk-embedded-C/"
  "1.0.0"
  "2021-05-26"
  "\n      Based on https://github.com/aws/aws-iot-device-sdk-embedded-C.git repository"
  "generated"
)

### Add Components ###

# cmsis_add_component arguments:
#   component
#   class
#   group
#   subgroup
#   variant
#   version
#   description
#   root

cmsis_add_component(
  "defender"
  "AWS IoT"
  "Features"
  "AWS IoT Defender"
  ""
  "1.0.0"
  "Client library for AWS IoT Device Defender service"
  "libraries/aws/device-defender-for-aws-iot-embedded-sdk"
)

cmsis_add_component(
  "shadow"
  "AWS IoT"
  "Features"
  "AWS IoT Shadow"
  ""
  "1.0.0"
  "Client library for AWS IoT Device Shadow service"
  "libraries/aws/device-shadow-for-aws-iot-embedded-sdk"
)

cmsis_add_component(
  "ota"
  "AWS IoT"
  "Features"
  "AWS IoT OTA"
  ""
  "3.0.0"
  "Client library for AWS Over-the-air Update service"
  "libraries/aws/ota-for-aws-iot-embedded-sdk"
)

cmsis_add_component(
  "json"
  "AWS IoT"
  "Features"
  "coreJSON"
  ""
  "3.0.0"
  "A parser strictly enforcing the ECMA-404 JSON standard"
  "libraries/standard/coreJSON"
)

### Add Dependencies ###

# cmsis_add_dependency arguments:
#   component
#   dependency
#   class
#   group
#   subgroup
#   variant
#   version

cmsis_add_dependency(
  "ota"
  "tinycbor"
  "Data Exchange"
  "CBOR"
  "TinyCBOR"
  ""
  ""
)

cmsis_add_dependency(
  "ota"
  "json"
  "AWS IoT"
  "Features"
  "coreJSON"
  ""
  ""
)

### Generate Pack ###

cmsis_generate_pack()
