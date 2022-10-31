message(STATUS "downloading...
     src='https://github.com/fmtlib/fmt/archive/refs/tags/8.0.1.zip'
     dst='/root/work/maria/server/build/extra/libfmt/src/8.0.1.zip'
     timeout='none'")




file(DOWNLOAD
  "https://github.com/fmtlib/fmt/archive/refs/tags/8.0.1.zip"
  "/root/work/maria/server/build/extra/libfmt/src/8.0.1.zip"
  SHOW_PROGRESS
  EXPECTED_HASH;MD5=e77873199e897ca9f780479ad68e25b1
  # no TIMEOUT
  STATUS status
  LOG log)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
  message(FATAL_ERROR "error: downloading 'https://github.com/fmtlib/fmt/archive/refs/tags/8.0.1.zip' failed
  status_code: ${status_code}
  status_string: ${status_string}
  log: ${log}
")
endif()

message(STATUS "downloading... done")
