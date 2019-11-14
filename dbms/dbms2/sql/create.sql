CREATE DATABASE farlake
        USER SYS IDENTIFIED BY admin
        USER SYSTEM IDENTIFIED BY admin
        LOGFILE GROUP 1 ('/u01/aee11/logs/redo01a.log') SIZE 10M REUSE,
                GROUP 2 ('/u01/aee11/logs/redo02a.log') SIZE 10M REUSE,
                GROUP 3 ('/u01/aee11/logs/redo03a.log') SIZE 10M REUSE
        MAXLOGFILES 5
        MAXLOGMEMBERS 5
        MAXLOGHISTORY 10
        MAXDATAFILES 50
        CHARACTER SET UTF8
        DATAFILE '/u01/aee11/farlake/node02/esaku76.dbf' SIZE 100M REUSE AUTOEXTEND ON,
        '/u01/aee11/farlake/node04/ulogo60.dbf' SIZE 100M REUSE AUTOEXTEND ON
        SYSAUX DATAFILE '/u01/aee11/farlake/node04/zam60.dbf' SIZE 100M REUSE AUTOEXTEND ON
        MAXSIZE UNLIMITED
        DEFAULT TABLESPACE users
                DATAFILE '/u01/aee11/farlake/node02/izucota132.dbf' SIZE 50M REUSE AUTOEXTEND ON MAXSIZE UNLIMITED,
                         '/u01/aee11/farlake/node02/uxusife241.dbf' SIZE 50M REUSE AUTOEXTEND ON MAXSIZE UNLIMITED
        DEFAULT TEMPORARY TABLESPACE temp
                TEMPFILE '/u01/aee11/farlake/temp01.dbf' SIZE 100M REUSE
        UNDO TABLESPACE undotbs1
                DATAFILE '/u01/aee11/farlake/undotbs01.dbf' SIZE 100M REUSE AUTOEXTEND ON MAXSIZE UNLIMITED
/
 
