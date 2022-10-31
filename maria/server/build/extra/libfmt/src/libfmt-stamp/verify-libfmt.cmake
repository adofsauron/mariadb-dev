set(file "/root/work/maria/server/build/extra/libfmt/src/8.0.1.zip")
message(STATUS "verifying file...
     file='${file}'")
set(expect_value "e77873199e897ca9f780479ad68e25b1")
file(MD5 "${file}" actual_value)
if("${actual_value}" STREQUAL "${expect_value}")
  message(STATUS "verifying file... done")
else()
  message(FATAL_ERROR "error: MD5 hash of
  ${file}
does not match expected value
  expected: ${expect_value}
    actual: ${actual_value}
")
endif()
