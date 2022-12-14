--disable_query_log
--disable_warnings
--error 0,ER_UNKNOWN_ERROR
CREATE TABLE t1 (a VARCHAR(10))
ENGINE=CONNECT TABLE_TYPE=XML OPTION_LIST='xmlsup=libxml2';
if (!`SELECT count(*) FROM INFORMATION_SCHEMA.TABLES
     WHERE TABLE_SCHEMA='test' AND TABLE_NAME='t1'
     AND ENGINE='CONNECT'
     AND CREATE_OPTIONS LIKE '%`table_type`=XML%'
     AND CREATE_OPTIONS LIKE '%xmlsup=libxml2%'`)
{
  DROP TABLE IF EXISTS t1;
  Skip Need LIBXML2;
}
DROP TABLE t1;
--enable_warnings
--enable_query_log


