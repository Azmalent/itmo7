#!/usr/bin/bash
ORACLE_USER=admin
ORACLE_PASSWORD=oracle
DATE=$(date +"%Y.%m.%d_%H.%M")

expdp $ORACLE_USER/$ORACLE_PASSWORD full=y \
        directory=DATAPUMP_DIR \
        dumpfile=${DATE}_${ORACLE_SID}_full_export.dmp \
        logfile=${DATE}_${ORACLE_SID}_full_export.log \
        flashback_time=SYSTIMESTAMP

scp -i ~/.ssh/id_rsa /u01/aee11/backup/datapump/${DATE}* oracle@db192:/u01/aee11/backup/datapump
 
