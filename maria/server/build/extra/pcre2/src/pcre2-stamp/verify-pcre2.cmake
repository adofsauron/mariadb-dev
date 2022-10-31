set(file "/root/work/maria/server/build/extra/pcre2/src/pcre2-10.40.zip")
message(STATUS "verifying file...
     file='${file}'")
set(expect_value "798698846982ce171d881ed0d7535c2a")
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
