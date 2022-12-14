#
# This include file is used by more than one test suite
# (currently rpl and binlog_encryption).
# Please check all dependent tests after modifying it.
#
# Usage:
#
# --let $use_remote_mysqlbinlog= 1 # optional
# --source suite/rpl/include/rpl_skip_replication.inc
#
# The script uses MYSQLBINLOG to verify certain results.
# By default, it uses binary logs directly. If it is undesirable,
# this behavior can be overridden by setting $use_remote_binlog
# as shown above.
# The value will be unset after every execution of the script,
# so if it is needed, it should be set explicitly before each call.
#

--source include/have_innodb.inc
--source include/master-slave.inc

--connection slave
--source include/stop_slave.inc
CHANGE MASTER TO MASTER_USE_GTID=NO;
--source include/start_slave.inc
--connection master

connection slave;
# Test that SUPER is required to change @@replicate_events_marked_for_skip.
CREATE USER 'nonsuperuser'@'127.0.0.1';
GRANT ALTER,CREATE,DELETE,DROP,EVENT,INSERT,PROCESS,REPLICATION SLAVE,
      SELECT,UPDATE ON *.* TO 'nonsuperuser'@'127.0.0.1';
connect(nonpriv, 127.0.0.1, nonsuperuser,, test, $SLAVE_MYPORT,);
connection nonpriv;
--error ER_SPECIFIC_ACCESS_DENIED_ERROR
SET GLOBAL replicate_events_marked_for_skip=FILTER_ON_MASTER;
disconnect nonpriv;
connection slave;
DROP USER'nonsuperuser'@'127.0.0.1';

SELECT @@global.replicate_events_marked_for_skip;
--error ER_SLAVE_MUST_STOP
SET GLOBAL replicate_events_marked_for_skip=FILTER_ON_SLAVE;
SELECT @@global.replicate_events_marked_for_skip;
STOP SLAVE;
--error ER_GLOBAL_VARIABLE
SET SESSION replicate_events_marked_for_skip=FILTER_ON_MASTER;
SELECT @@global.replicate_events_marked_for_skip;
SET GLOBAL replicate_events_marked_for_skip=FILTER_ON_MASTER;
SELECT @@global.replicate_events_marked_for_skip;
START SLAVE;

connection master;
SELECT @@skip_replication;
--error ER_LOCAL_VARIABLE
SET GLOBAL skip_replication=1;
SELECT @@skip_replication;

CREATE TABLE t1 (a INT PRIMARY KEY, b INT) ENGINE=myisam;
CREATE TABLE t2 (a INT PRIMARY KEY, b INT) ENGINE=innodb;
INSERT INTO t1(a) VALUES (1);
INSERT INTO t2(a) VALUES (1);


# Test that master-side filtering works.
SET skip_replication=1;

CREATE TABLE t3 (a INT PRIMARY KEY, b INT) ENGINE=myisam;
INSERT INTO t1(a) VALUES (2);
INSERT INTO t2(a) VALUES (2);

# Inject a rotate event in the binlog stream sent to slave (otherwise we will
# fail sync_slave_with_master as the last event on the master is not present
# on the slave).
FLUSH NO_WRITE_TO_BINLOG LOGS;

sync_slave_with_master;
connection slave;
SHOW TABLES;
SELECT * FROM t1;
SELECT * FROM t2;

connection master;
DROP TABLE t3;

FLUSH NO_WRITE_TO_BINLOG LOGS;
sync_slave_with_master;


# Test that slave-side filtering works.
connection slave;
STOP SLAVE;
SET GLOBAL replicate_events_marked_for_skip=FILTER_ON_SLAVE;
START SLAVE;

connection master;
SET skip_replication=1;
CREATE TABLE t3 (a INT PRIMARY KEY, b INT) ENGINE=myisam;
INSERT INTO t1(a) VALUES (3);
INSERT INTO t2(a) VALUES (3);

# Inject a rotate event in the binlog stream sent to slave (otherwise we will
# fail sync_slave_with_master as the last event on the master is not present
# on the slave).
FLUSH NO_WRITE_TO_BINLOG LOGS;

sync_slave_with_master;
connection slave;
SHOW TABLES;
SELECT * FROM t1;
SELECT * FROM t2;

connection master;
DROP TABLE t3;

FLUSH NO_WRITE_TO_BINLOG LOGS;
sync_slave_with_master;
connection slave;
STOP SLAVE;
SET GLOBAL replicate_events_marked_for_skip=REPLICATE;
START SLAVE;


# Test that events with @@skip_replication=1 are not filtered when filtering is
# not set on slave.
connection master;
SET skip_replication=1;
CREATE TABLE t3 (a INT PRIMARY KEY, b INT) ENGINE=myisam;
INSERT INTO t3(a) VALUES(2);
sync_slave_with_master;
connection slave;
SELECT * FROM t3;
connection master;
DROP TABLE t3;

#
# Test that the slave will preserve the @@skip_replication flag in its
# own binlog.
#

TRUNCATE t1;
sync_slave_with_master;
connection slave;
RESET MASTER;

connection master;
SET skip_replication=0;
INSERT INTO t1 VALUES (1,0);
SET skip_replication=1;
INSERT INTO t1 VALUES (2,0);
SET skip_replication=0;
INSERT INTO t1 VALUES (3,0);

sync_slave_with_master;
connection slave;
# Since slave has @@replicate_events_marked_for_skip=REPLICATE, it should have
# applied all events.
SELECT * FROM t1 ORDER by a;

STOP SLAVE;
SET GLOBAL replicate_events_marked_for_skip=FILTER_ON_MASTER;
let $SLAVE_DATADIR= `select @@datadir`;

connection master;
TRUNCATE t1;

# Now apply the slave binlog to the master, to check that both the slave
# and mysqlbinlog will preserve the @@skip_replication flag.

--let $mysqlbinlog_args= $SLAVE_DATADIR/slave-bin.000001
if ($use_remote_mysqlbinlog)
{
    --let $mysqlbinlog_args= --read-from-remote-server --protocol=tcp --host=127.0.0.1 --port=$SLAVE_MYPORT -uroot slave-bin.000001
    --let $use_remote_mysqlbinlog= 0
}
--exec $MYSQL_BINLOG $mysqlbinlog_args > $MYSQLTEST_VARDIR/tmp/rpl_skip_replication.binlog
--exec $MYSQL test < $MYSQLTEST_VARDIR/tmp/rpl_skip_replication.binlog

# The master should have all three events.
SELECT * FROM t1 ORDER by a;

# The slave should be missing event 2, which is marked with the
# @@skip_replication flag.

connection slave;
START SLAVE;

connection master;
sync_slave_with_master;

connection slave;
SELECT * FROM t1 ORDER by a;

#
# Test that @@sql_slave_skip_counter does not count skipped @@skip_replication
# events.
#

connection master;
TRUNCATE t1;

sync_slave_with_master;
connection slave;
STOP SLAVE;
# We will skip two INSERTs (in addition to any skipped due to
# @@skip_replication). Since from 5.5 every statement is wrapped in
# BEGIN ... END, we need to skip 6 events for this.
SET GLOBAL sql_slave_skip_counter=6;
SET GLOBAL replicate_events_marked_for_skip=FILTER_ON_SLAVE;
START SLAVE;

connection master;
# Need to fix @@binlog_format to get consistent event count.
SET @old_binlog_format= @@binlog_format;
SET binlog_format= statement;
SET skip_replication=0;
INSERT INTO t1 VALUES (1,5);
SET skip_replication=1;
INSERT INTO t1 VALUES (2,5);
SET skip_replication=0;
INSERT INTO t1 VALUES (3,5);
INSERT INTO t1 VALUES (4,5);
SET binlog_format= @old_binlog_format;

sync_slave_with_master;
connection slave;

# The slave should have skipped the first three inserts (number 1 and 3 due
# to @@sql_slave_skip_counter=2, number 2 due to
# @@replicate_events_marked_for_skip=FILTER_ON_SLAVE). So only number 4
# should be left.
SELECT * FROM t1;


#
# Check that BINLOG statement preserves the @@skip_replication flag.
#
connection slave;
# Need row @@binlog_format for BINLOG statements containing row events.
--source include/stop_slave.inc
SET @old_slave_binlog_format= @@global.binlog_format;
SET GLOBAL binlog_format= row;
--source include/start_slave.inc

connection master;
TRUNCATE t1;

SET @old_binlog_format= @@binlog_format;
SET binlog_format= row;
# Format description log event.
BINLOG 'wlZOTw8BAAAA8QAAAPUAAAAAAAQANS41LjIxLU1hcmlhREItZGVidWctbG9nAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAEzgNAAgAEgAEBAQEEgAA2QAEGggAAAAICAgCAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAA371saA==';
# INSERT INTO t1 VALUES (1,8)  # with @@skip_replication=1
BINLOG 'wlZOTxMBAAAAKgAAAGMBAAAAgCkAAAAAAAEABHRlc3QAAnQxAAIDAwAC
wlZOTxcBAAAAJgAAAIkBAAAAgCkAAAAAAAEAAv/8AQAAAAgAAAA=';
# INSERT INTO t1 VALUES (2,8)  # with @@skip_replication=0
BINLOG 'wlZOTxMBAAAAKgAAADwCAAAAACkAAAAAAAEABHRlc3QAAnQxAAIDAwAC
wlZOTxcBAAAAJgAAAGICAAAAACkAAAAAAAEAAv/8AgAAAAgAAAA=';
SET binlog_format= @old_binlog_format;

SELECT * FROM t1 ORDER BY a;
sync_slave_with_master;
connection slave;
# Slave should have only the second insert, the first should be ignored due to
# the @@skip_replication flag.
SELECT * FROM t1 ORDER by a;

--source include/stop_slave.inc
SET GLOBAL binlog_format= @old_slave_binlog_format;
--source include/start_slave.inc


# Test that it is not possible to change @@skip_replication inside a
# transaction or statement, thereby replicating only parts of statements
# or transactions.
connection master;
SET skip_replication=0;

BEGIN;
--error ER_INSIDE_TRANSACTION_PREVENTS_SWITCH_SKIP_REPLICATION
SET skip_replication=0;
--error ER_INSIDE_TRANSACTION_PREVENTS_SWITCH_SKIP_REPLICATION
SET skip_replication=1;
ROLLBACK;
SET skip_replication=1;
BEGIN;
--error ER_INSIDE_TRANSACTION_PREVENTS_SWITCH_SKIP_REPLICATION
SET skip_replication=0;
--error ER_INSIDE_TRANSACTION_PREVENTS_SWITCH_SKIP_REPLICATION
SET skip_replication=1;
COMMIT;
SET autocommit=0;
INSERT INTO t2(a) VALUES(100);
--error ER_INSIDE_TRANSACTION_PREVENTS_SWITCH_SKIP_REPLICATION
SET skip_replication=1;
ROLLBACK;
SET autocommit=1;

SET skip_replication=1;
--delimiter |
CREATE FUNCTION foo (x INT) RETURNS INT BEGIN SET SESSION skip_replication=x; RETURN x; END|
CREATE PROCEDURE bar(x INT) BEGIN SET SESSION skip_replication=x; END|
CREATE FUNCTION baz (x INT) RETURNS INT BEGIN CALL bar(x); RETURN x; END|
--delimiter ;
--error ER_STORED_FUNCTION_PREVENTS_SWITCH_SKIP_REPLICATION
SELECT foo(0);
--error ER_STORED_FUNCTION_PREVENTS_SWITCH_SKIP_REPLICATION
SELECT baz(0);
--error ER_STORED_FUNCTION_PREVENTS_SWITCH_SKIP_REPLICATION
SET @a= foo(1);
--error ER_STORED_FUNCTION_PREVENTS_SWITCH_SKIP_REPLICATION
SET @a= baz(1);
--error ER_STORED_FUNCTION_PREVENTS_SWITCH_SKIP_REPLICATION
UPDATE t2 SET b=foo(0);
--error ER_STORED_FUNCTION_PREVENTS_SWITCH_SKIP_REPLICATION
UPDATE t2 SET b=baz(0);
--error ER_STORED_FUNCTION_PREVENTS_SWITCH_SKIP_REPLICATION
INSERT INTO t1 VALUES (101, foo(1));
--error ER_STORED_FUNCTION_PREVENTS_SWITCH_SKIP_REPLICATION
INSERT INTO t1 VALUES (101, baz(0));
SELECT @@skip_replication;
CALL bar(0);
SELECT @@skip_replication;
CALL bar(1);
SELECT @@skip_replication;
DROP FUNCTION foo;
DROP PROCEDURE bar;
DROP FUNCTION baz;


# Test that master-side filtering happens on the master side, and that
# slave-side filtering happens on the slave.

# First test that events do not reach the slave when master-side filtering
# is configured. Do this by replicating first with only the IO thread running
# and master-side filtering; then change to no filtering and start the SQL
# thread. This should still skip the events, as master-side filtering
# means the events never reached the slave.
connection master;
SET skip_replication= 0;
TRUNCATE t1;
sync_slave_with_master;
connection slave;
STOP SLAVE;
SET GLOBAL replicate_events_marked_for_skip=FILTER_ON_MASTER;
START SLAVE IO_THREAD;
connection master;
SET skip_replication= 1;
INSERT INTO t1(a) VALUES (1);
SET skip_replication= 0;
INSERT INTO t1(a) VALUES (2);
--source include/save_master_pos.inc
connection slave;
--source include/sync_io_with_master.inc
STOP SLAVE IO_THREAD;
SET GLOBAL replicate_events_marked_for_skip=REPLICATE;
START SLAVE;
connection master;
sync_slave_with_master;
connection slave;
# Now only the second insert of (2) should be visible, as the first was
# filtered on the master, so even though the SQL thread ran without skipping
# events, it will never see the event in the first place.
SELECT * FROM t1;

# Now tests that when slave-side filtering is configured, events _do_ reach
# the slave.
connection master;
SET skip_replication= 0;
TRUNCATE t1;
sync_slave_with_master;
connection slave;
STOP SLAVE;
SET GLOBAL replicate_events_marked_for_skip=FILTER_ON_SLAVE;
START SLAVE IO_THREAD;
connection master;
SET skip_replication= 1;
INSERT INTO t1(a) VALUES (1);
SET skip_replication= 0;
INSERT INTO t1(a) VALUES (2);
--source include/save_master_pos.inc
connection slave;
--source include/sync_io_with_master.inc
STOP SLAVE IO_THREAD;
SET GLOBAL replicate_events_marked_for_skip=REPLICATE;
START SLAVE;
connection master;
sync_slave_with_master;
connection slave;
# Now both inserts should be visible. Since filtering was configured to be
# slave-side, the event is in the relay log, and when the SQL thread ran we
# had disabled filtering again.
SELECT * FROM t1 ORDER BY a;


# Clean up.
connection master;
SET skip_replication=0;
DROP TABLE t1,t2;
connection slave;
STOP SLAVE;
SET GLOBAL replicate_events_marked_for_skip=REPLICATE;
START SLAVE;

--source include/rpl_end.inc
