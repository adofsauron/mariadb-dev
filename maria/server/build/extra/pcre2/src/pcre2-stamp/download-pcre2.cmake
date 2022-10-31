message(STATUS "downloading...
     src='https://github.com/PhilipHazel/pcre2/releases/download/pcre2-10.40/pcre2-10.40.zip'
     dst='/root/work/maria/server/build/extra/pcre2/src/pcre2-10.40.zip'
     timeout='none'")




file(DOWNLOAD
  "https://github.com/PhilipHazel/pcre2/releases/download/pcre2-10.40/pcre2-10.40.zip"
  "/root/work/maria/server/build/extra/pcre2/src/pcre2-10.40.zip"
  SHOW_PROGRESS
  EXPECTED_HASH;MD5=798698846982ce171d881ed0d7535c2a
  # no TIMEOUT
  STATUS status
  LOG log)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
  message(FATAL_ERROR "error: downloading 'https://github.com/PhilipHazel/pcre2/releases/download/pcre2-10.40/pcre2-10.40.zip' failed
  status_code: ${status_code}
  status_string: ${status_string}
  log: ${log}
")
endif()

message(STATUS "downloading... done")
