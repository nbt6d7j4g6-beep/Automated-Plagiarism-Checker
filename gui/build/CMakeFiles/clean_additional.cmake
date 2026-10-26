# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles\\PlagiarismCheckerGUI_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\PlagiarismCheckerGUI_autogen.dir\\ParseCache.txt"
  "PlagiarismCheckerGUI_autogen"
  )
endif()
