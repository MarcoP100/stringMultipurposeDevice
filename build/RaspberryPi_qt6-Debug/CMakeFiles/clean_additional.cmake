# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appStringMultipurposeDevice_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appStringMultipurposeDevice_autogen.dir/ParseCache.txt"
  "appStringMultipurposeDevice_autogen"
  )
endif()
