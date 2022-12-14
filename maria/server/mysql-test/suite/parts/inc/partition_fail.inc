# Include file to decrease test code duplication

--eval $create_statement
--eval $insert_statement
--let $dbug_flag= `select @@session.debug_dbug`
--echo # $dbug_flag: BEFORE failure
--replace_result #p# #P# #sp# #SP#
--let $DATADIR= `select @@datadir`
--list_files $DATADIR/test
SHOW CREATE TABLE t1;
--sorted_result
SELECT * FROM t1;
--disable_abort_on_error
--eval $fail_statement
--enable_abort_on_error
--echo # $dbug_flag: AFTER failure
--replace_result #p# #P# #sp# #SP#
--list_files $DATADIR/test
SHOW CREATE TABLE t1;
--sorted_result
SELECT * FROM t1;
DROP TABLE t1;

--eval $create_statement
--eval $insert_statement
--echo # $dbug_flag: BEFORE failure (under LOCK TABLE)
--replace_result #p# #P# #sp# #SP#
--list_files $DATADIR/test
SHOW CREATE TABLE t1;
--sorted_result
SELECT * FROM t1;
LOCK TABLE t1 WRITE;
--disable_abort_on_error
--eval $fail_statement
--enable_abort_on_error
--echo # $dbug_flag: AFTER failure (under LOCK TABLE)
--replace_result #p# #P# #sp# #SP#
--list_files $DATADIR/test
SHOW CREATE TABLE t1;
--sorted_result
SELECT * FROM t1;
UNLOCK TABLES;
DROP TABLE t1;
