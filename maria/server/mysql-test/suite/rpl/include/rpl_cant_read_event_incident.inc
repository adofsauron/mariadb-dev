#
# This include file is used by more than one test suite
# (currently rpl and binlog_encryption).
# Please check all dependent tests after modifying it
#

#
# Bug#11747416 : 32228 	A disk full makes binary log corrupt.
#
# 
# The test demonstrates reading from binlog error propagation to slave 
# and reporting there.
# Conditions for the bug include a crash at time of the last event to
# the binlog was written partly. With the fixes the event is not sent out 
# any longer, but rather the dump thread sends out a sound error message.
# 
# Crash is not simulated. A binlog with partly written event in its end is installed
# and replication is started from it.
#

--source include/have_binlog_format_mixed.inc
--source include/master-slave.inc

--connection slave
--source include/stop_slave.inc
CHANGE MASTER TO MASTER_USE_GTID=NO;
--source include/start_slave.inc

# Make sure the slave is stopped while we are messing with master.
# Otherwise we get occasional failures as the slave manages to re-connect
# to the newly started master and we get extra events applied, causing
# conflicts.
--source include/stop_slave.inc

--connection master
call mtr.add_suppression("Error in Log_event::read_log_event()");
--let $datadir= `SELECT @@datadir`

--let $rpl_server_number= 1
--source include/rpl_stop_server.inc

--remove_file $datadir/master-bin.000001
--copy_file $MYSQL_TEST_DIR/std_data/bug11747416_32228_binlog.000001 $datadir/master-bin.000001

--let $rpl_server_number= 1
--source include/rpl_start_server.inc

--source include/wait_until_connected_again.inc

# evidence of the partial binlog
--error ER_ERROR_WHEN_EXECUTING_COMMAND
show binlog events;

--connection slave
call mtr.add_suppression("Slave I/O: Got fatal error 1236 from master when reading data from binary log");
--let $master_use_gtid_option= No
--source include/reset_slave.inc
start slave;

# ER_MASTER_FATAL_ERROR_READING_BINLOG 1236
--let $slave_param=Last_IO_Errno
--let $slave_param_value=1236
--source include/wait_for_slave_param.inc

--let $slave_field_result_replace= / at [0-9]*/ at XXX/
--let $status_items= Last_IO_Errno, Last_IO_Error
--source include/show_slave_status.inc

#
# Cleanup
#

--connection master
reset master;

--connection slave
stop slave;
reset slave;
# Table was created from binlog, it may not be created if SQL thread is running
# slowly and IO thread reaches incident before SQL thread applies it.
--disable_warnings
drop table if exists t;
--enable_warnings
reset master;

--echo End of the tests
--let $rpl_only_running_threads= 1
--source include/rpl_end.inc
