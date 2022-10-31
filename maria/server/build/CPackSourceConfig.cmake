# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


SET(CPACK_BINARY_BUNDLE "")
SET(CPACK_BINARY_CYGWIN "")
SET(CPACK_BINARY_DEB "")
SET(CPACK_BINARY_DRAGNDROP "")
SET(CPACK_BINARY_NSIS "")
SET(CPACK_BINARY_OSXX11 "")
SET(CPACK_BINARY_PACKAGEMAKER "")
SET(CPACK_BINARY_RPM "")
SET(CPACK_BINARY_STGZ "")
SET(CPACK_BINARY_TBZ2 "")
SET(CPACK_BINARY_TGZ "")
SET(CPACK_BINARY_TZ "")
SET(CPACK_BINARY_WIX "")
SET(CPACK_BINARY_ZIP "")
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
SET(CPACK_COMPONENTS_ALL "COMPONENT;Client;ClientPlugins;Common;Development;IniFiles;ManPagesClient;ManPagesDevelopment;ManPagesServer;ManPagesTest;Readme;Server;Server_Scripts;SharedLibraries;SqlBench;SupportFiles;Test;Unspecified;backup;connect-engine;gssapi-server;hashicorp-key-management;plugin-hashicorp-key-management;provider-lzma;rocksdb-engine;s3-engine")
SET(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
SET(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
SET(CPACK_GENERATOR "TGZ")
SET(CPACK_IGNORE_FILES "\\.git/;\\.git$;\\.gitignore$;\\.gitattributes$;CMakeCache\\.txt$;cmake_dist\\.cmake$;CPackSourceConfig\\.cmake$;CPackConfig.cmake$;/cmake_install\\.cmake$;/CTestTestfile\\.cmake$;/CMakeFiles/;/version_resources/;/_CPack_Packages/;\\.gz$;\\.zip$;/CMakeFiles/;/version_resources/;/_CPack_Packages/;scripts/make_binary_distribution$;scripts/msql2mysql$;scripts/mysql_config$;scripts/mysql_convert_table_format$;scripts/mysql_find_rows$;scripts/mysql_fix_extensions$;scripts/mysql_install_db$;scripts/mysql_secure_installation$;scripts/mysql_setpermission$;scripts/mysqlaccess$;scripts/mysqld_multi$;scripts/mysqld_safe$;scripts/mysqldumpslow$;scripts/mysqlhotcopy$;Makefile$;include/config\\.h$;include/my_config\\.h$;/autom4te\\.cache/;errmsg\\.sys$;\\.rpm$")
SET(CPACK_INSTALLED_DIRECTORIES "/root/work/maria/server;/")
SET(CPACK_INSTALL_CMAKE_PROJECTS "")
SET(CPACK_INSTALL_PREFIX "/usr/local/mysql")
SET(CPACK_MODULE_PATH "/root/work/maria/server/cmake;/root/work/maria/server/cmake/Internal/CPack")
SET(CPACK_NSIS_DISPLAY_NAME "MariaDB 10.11.1")
SET(CPACK_NSIS_INSTALLER_ICON_CODE "")
SET(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
SET(CPACK_NSIS_PACKAGE_NAME "MariaDB 10.11.1")
SET(CPACK_OUTPUT_CONFIG_FILE "/root/work/maria/server/build/CPackConfig.cmake")
SET(CPACK_PACKAGE_CONTACT "MariaDB Developers <maria-developers@lists.launchpad.net>")
SET(CPACK_PACKAGE_DEFAULT_LOCATION "/")
SET(CPACK_PACKAGE_DESCRIPTION "MariaDB: a very fast and robust SQL database server

It is GPL v2 licensed, which means you can use the it free of charge under the
conditions of the GNU General Public License Version 2 (http://www.gnu.org/licenses/).

MariaDB documentation can be found at https://mariadb.com/kb
MariaDB bug reports should be submitted through https://jira.mariadb.org

")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake/Templates/CPack.GenericDescription.txt")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MariaDB: a very fast and robust SQL database server")
SET(CPACK_PACKAGE_FILE_NAME "mariadb-10.11.1")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "MariaDB 10.11.1")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "MariaDB 10.11.1")
SET(CPACK_PACKAGE_NAME "MariaDB")
SET(CPACK_PACKAGE_RELOCATABLE "true")
SET(CPACK_PACKAGE_URL "http://mariadb.org")
SET(CPACK_PACKAGE_VENDOR "MariaDB Foundation")
SET(CPACK_PACKAGE_VERSION "10.11.1")
SET(CPACK_PACKAGE_VERSION_MAJOR "10")
SET(CPACK_PACKAGE_VERSION_MINOR "11")
SET(CPACK_PACKAGE_VERSION_PATCH "1")
SET(CPACK_RESOURCE_FILE_LICENSE "/usr/share/cmake/Templates/CPack.GenericLicense.txt")
SET(CPACK_RESOURCE_FILE_README "/usr/share/cmake/Templates/CPack.GenericDescription.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "/usr/share/cmake/Templates/CPack.GenericWelcome.txt")
SET(CPACK_RPM_connect-engine_PACKAGE_DESCRIPTION "Connect engine supports a number of file formats (dbf, xml, txt, bin, etc),
connections to ODBC tables and remote MySQL tables, as well as a number of
other interesting features.")
SET(CPACK_RPM_connect-engine_PACKAGE_SUMMARY "Connect storage engine for MariaDB")
SET(CPACK_RPM_cracklib-password-check_PACKAGE_DESCRIPTION "This password validation plugin uses cracklib to allow only
sufficiently secure (as defined by cracklib) user passwords in MariaDB.")
SET(CPACK_RPM_cracklib-password-check_PACKAGE_SUMMARY "CrackLib Password Validation Plugin for MariaDB")
SET(CPACK_RPM_gssapi-server_PACKAGE_DESCRIPTION "The gssapi authentication plugin allows the user to authenticate with services
that use the Generic Security Services Application Program Interface (GSSAPI).
The gssapi authentication plugin is most often used for authenticating with Microsoft Active Directory.")
SET(CPACK_RPM_gssapi-server_PACKAGE_SUMMARY "GSSAPI authentication plugin for MariaDB server")
SET(CPACK_RPM_hashicorp-key-management_PACKAGE_DESCRIPTION "This encryption plugin uses Hashicorp Vault for storing encryption
 keys for MariaDB Data-at-Rest encryption.")
SET(CPACK_RPM_hashicorp-key-management_PACKAGE_SUMMARY "Hashicorp Key Management plugin for MariaDB")
SET(CPACK_RPM_oqgraph-engine_PACKAGE_DESCRIPTION "The Open Query GRAPH computation engine, or OQGRAPH as the engine itself is called,
allows you to handle hierarchies (tree structures) and complex graphs
(nodes having many connections in several directions).
It is intended to be used for retrieving hierarchical information, such as those used for graphs,
routes or social relationships, in plain SQL.")
SET(CPACK_RPM_oqgraph-engine_PACKAGE_SUMMARY "OQGraph storage engine for MariaDB")
SET(CPACK_RPM_provider-bzip2_PACKAGE_DESCRIPTION "BZip2 compression support in the server and storage engines")
SET(CPACK_RPM_provider-bzip2_PACKAGE_SUMMARY "BZip2 compression support in the server and storage engines")
SET(CPACK_RPM_provider-lz4_PACKAGE_DESCRIPTION "LZ4 compression support in the server and storage engines")
SET(CPACK_RPM_provider-lz4_PACKAGE_SUMMARY "LZ4 compression support in the server and storage engines")
SET(CPACK_RPM_provider-lzma_PACKAGE_DESCRIPTION "LZMA compression support in the server and storage engines")
SET(CPACK_RPM_provider-lzma_PACKAGE_SUMMARY "LZMA compression support in the server and storage engines")
SET(CPACK_RPM_provider-lzo_PACKAGE_DESCRIPTION "LZO compression support in the server and storage engines")
SET(CPACK_RPM_provider-lzo_PACKAGE_SUMMARY "LZO compression support in the server and storage engines")
SET(CPACK_RPM_provider-snappy_PACKAGE_DESCRIPTION "Snappy compression support in the server and storage engines")
SET(CPACK_RPM_provider-snappy_PACKAGE_SUMMARY "Snappy compression support in the server and storage engines")
SET(CPACK_RPM_rocksdb-engine_PACKAGE_DESCRIPTION "The RocksDB storage engine is a high performance storage engine, aimed
at maximising storage efficiency while maintaining InnoDB-like performance.")
SET(CPACK_RPM_rocksdb-engine_PACKAGE_SUMMARY "RocksDB storage engine for MariaDB")
SET(CPACK_RPM_s3-engine_PACKAGE_DESCRIPTION "The S3 storage engine allows one to archive MariaDB tables in Amazon S3 (or any third-party public or private cloud that implements S3 API), but still have them accessible in MariaDB in read-only mode.")
SET(CPACK_RPM_s3-engine_PACKAGE_SUMMARY "Amazon S3 archival storage engine for MariaDB")
SET(CPACK_SET_DESTDIR "OFF")
SET(CPACK_SOURCE_CYGWIN "")
SET(CPACK_SOURCE_GENERATOR "TGZ")
SET(CPACK_SOURCE_IGNORE_FILES "\\.git/;\\.git$;\\.gitignore$;\\.gitattributes$;CMakeCache\\.txt$;cmake_dist\\.cmake$;CPackSourceConfig\\.cmake$;CPackConfig.cmake$;/cmake_install\\.cmake$;/CTestTestfile\\.cmake$;/CMakeFiles/;/version_resources/;/_CPack_Packages/;\\.gz$;\\.zip$;/CMakeFiles/;/version_resources/;/_CPack_Packages/;scripts/make_binary_distribution$;scripts/msql2mysql$;scripts/mysql_config$;scripts/mysql_convert_table_format$;scripts/mysql_find_rows$;scripts/mysql_fix_extensions$;scripts/mysql_install_db$;scripts/mysql_secure_installation$;scripts/mysql_setpermission$;scripts/mysqlaccess$;scripts/mysqld_multi$;scripts/mysqld_safe$;scripts/mysqldumpslow$;scripts/mysqlhotcopy$;Makefile$;include/config\\.h$;include/my_config\\.h$;/autom4te\\.cache/;errmsg\\.sys$;\\.rpm$")
SET(CPACK_SOURCE_INSTALLED_DIRECTORIES "/root/work/maria/server;/")
SET(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/root/work/maria/server/build/CPackSourceConfig.cmake")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "mariadb-10.11.1")
SET(CPACK_SOURCE_TBZ2 "")
SET(CPACK_SOURCE_TGZ "")
SET(CPACK_SOURCE_TOPLEVEL_TAG "Linux-Source")
SET(CPACK_SOURCE_TZ "")
SET(CPACK_SOURCE_ZIP "")
SET(CPACK_STRIP_FILES "")
SET(CPACK_SYSTEM_NAME "Linux")
SET(CPACK_TOPLEVEL_TAG "Linux-Source")
SET(CPACK_WIX_SIZEOF_VOID_P "8")
