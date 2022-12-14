# Note that this test requires a fresh restart to not have problems with the
# old status values

--disable_service_connection

set @save_use_stat_tables=@@use_stat_tables;

set use_stat_tables='preferably';

--disable_warnings
DROP DATABASE IF EXISTS dbt3_s001;
--enable_warnings

CREATE DATABASE dbt3_s001;

use dbt3_s001;

set @save_optimizer_switch=@@optimizer_switch;
set optimizer_switch='extended_keys=off';
 
--disable_query_log
--disable_result_log
--disable_warnings
--source include/dbt3_s001.inc
truncate mysql.table_stats;
truncate mysql.column_stats;
truncate mysql.index_stats;
ANALYZE TABLE
customer, lineitem, nation, orders, part, partsupp, region, supplier;
--enable_warnings
--enable_result_log
--enable_query_log

select * from mysql.table_stats;
select * from mysql.index_stats;


#
# Test for parallel memory allocation for statistical data
# 
#    assumes that start the code of memory allocation for stats data has this line:
#
#       DEBUG_SYNC(thd, "statistics_mem_alloc_start1");
#       DEBUG_SYNC(thd, "statistics_mem_alloc_start2");
#

let $Q6=
select round(sum(l_extendedprice*l_discount),4) as revenue
from lineitem
where l_shipdate >= date '1994-01-01' 
      and l_shipdate < date '1994-01-01' + interval '1' year
      and l_discount between 0.06 - 0.01 and 0.06 + 0.01
      and l_quantity < 24;

flush table lineitem;
set use_stat_tables='never';
eval $Q6;

connect (con1, localhost, root,,);
connect (con2, localhost, root,,);

connection con1;
set debug_sync='statistics_mem_alloc_start1  WAIT_FOR second_thread_started_too';
set debug_sync='statistics_mem_alloc_start2  SIGNAL first_thread_working';
use dbt3_s001;
set use_stat_tables='preferably';
--send_eval $Q6                                            

connection con2;
set debug_sync='statistics_mem_alloc_start1  SIGNAL second_thread_started_too';
set debug_sync='statistics_mem_alloc_start2  WAIT_FOR first_thread_working';
use dbt3_s001;
set use_stat_tables='preferably';
--send_eval $Q6

connection con1;
--reap

connection con2;
--reap

connection default;
set use_stat_tables='preferably';
disconnect con1;
disconnect con2;
set debug_sync='RESET';

#
# Test for parallel statistics collection
# 
#    assumes that start of stats collection code has this line:
#
#       DEBUG_SYNC(thd, "statistics_collection_start1");
#       DEBUG_SYNC(thd, "statistics_collection_start2");
#

select * from mysql.index_stats where table_name='lineitem' order by index_name;
delete from mysql.index_stats 
  where table_name='lineitem' and
        index_name in ('i_l_shipdate', 'i_l_receiptdate');
select * from mysql.index_stats where table_name='lineitem' order by index_name;
--disable_result_log
--disable_warnings
analyze table lineitem persistent for columns() indexes (i_l_shipdate);
--enable_warnings
--enable_result_log
select * from mysql.index_stats where table_name='lineitem' order by index_name;
delete from mysql.index_stats 
  where table_name='lineitem' and index_name= 'i_l_shipdate';
select * from mysql.index_stats where table_name='lineitem' order by index_name;

connect (con1, localhost, root,,);
connect (con2, localhost, root,,);

connection con1;
set debug_sync='statistics_collection_start1  WAIT_FOR second_thread_started_too';
set debug_sync='statistics_collection_start2  SIGNAL first_thread_working';
use dbt3_s001;
set use_stat_tables='preferably';
--send analyze table lineitem persistent for columns() indexes (i_l_shipdate)

connection con2;
set debug_sync='statistics_collection_start1  SIGNAL second_thread_started_too';
set debug_sync='statistics_collection_start2  WAIT_FOR first_thread_working';
use dbt3_s001;
set use_stat_tables='preferably';
--send analyze table lineitem persistent for columns() indexes (i_l_receiptdate)

connection con1;
--disable_result_log
--disable_warnings
--reap
--enable_warnings
--enable_result_log

connection con2;
--disable_result_log
--disable_warnings
--reap
--enable_warnings
--enable_result_log

connection default;
disconnect con1;
disconnect con2;
set debug_sync='RESET';

select * from mysql.index_stats where table_name='lineitem' order by index_name, prefix_arity;

#
# Test for parallel statistics collection and update (innodb)
# 

select * from mysql.index_stats where table_name='lineitem'
 order by index_name, prefix_arity;
set debug_sync='RESET';

let $innodb_storage_engine= 0;
if (`SELECT UPPER(@@default_storage_engine) = 'INNODB'`)
{
  let $innodb_storage_engine= 1;
}

connect (con1, localhost, root,,);
connect (con2, localhost, root,,);

connection con1;
set debug_sync='statistics_collection_start SIGNAL parked WAIT_FOR finish';
use dbt3_s001;
set use_stat_tables='preferably';
--send analyze table lineitem persistent for all

connection con2;
set debug_sync='now WAIT_FOR parked';
use dbt3_s001;
set use_stat_tables='never';
if ($innodb_storage_engine)
{
  select * from lineitem where l_orderkey=1 and l_partkey=156;
  delete from lineitem where l_orderkey=1 and l_partkey=156;
  select * from lineitem where l_orderkey=1 and l_partkey=156;
}
set debug_sync='now SIGNAL finish';

connection con1;
--disable_result_log
--disable_warnings
--reap
--enable_warnings
--enable_result_log

connection default;
disconnect con1;
disconnect con2;
set debug_sync='RESET';

select * from mysql.index_stats where table_name='lineitem'
  order by index_name, prefix_arity;

#
# Bug mdev-3891: deadlock for ANALYZE and SELECT over mysql.index_stats
# 

set @save_global_use_stat_tables=@@global.use_stat_tables;
set global use_stat_tables='preferably';
set debug_sync='RESET';

connect (con1, localhost, root,,);
connect (con2, localhost, root,,);

connection con1;
set debug_sync='statistics_update_start SIGNAL parker WAIT_FOR go1 EXECUTE 1';
set debug_sync='thr_multi_lock_before_thr_lock SIGNAL go2 EXECUTE 3';
use dbt3_s001;
--send analyze table lineitem persistent for all

connection con2;
set debug_sync='open_and_process_table WAIT_FOR parker';
set debug_sync='statistics_read_start SIGNAL go1 WAIT_FOR go2';
use dbt3_s001;
--send select * from mysql.index_stats, lineitem where index_name= 'i_l_shipdate' and l_orderkey=1 and l_partkey=68 order by prefix_arity;

connection con1;
--disable_result_log
--disable_warnings
--reap
--enable_warnings
--enable_result_log

connection con2;
--disable_warnings
--reap
--enable_warnings

connection default;
disconnect con1;
disconnect con2;
set debug_sync='RESET';

set global use_stat_tables=@save_global_use_stat_tables;

DROP DATABASE dbt3_s001;

use test;

--enable_service_connection

#
# Bug mdev-4019: crash when executing in parallel ANALYZE and 
#                SELECT * FROM information_schema.statistics
# 

set @save_global_use_stat_tables=@@global.use_stat_tables;
set global use_stat_tables='preferably';
set debug_sync='RESET';

create table t1 (a int, b int, key(a));
insert t1 values (1,1),(2,2);

analyze table t1;

SET debug_sync='after_open_table_ignore_flush WAIT_FOR go';
send select * from information_schema.statistics where table_schema='test';

connect(con1, localhost, root);
connection con1;
select * from t1;
SET DEBUG_SYNC= "now SIGNAL go";

connection default;
reap;

connection default;
disconnect con1;
set debug_sync='RESET';

drop table t1;
set global use_stat_tables=@save_global_use_stat_tables;
set use_stat_tables=@save_use_stat_tables;
