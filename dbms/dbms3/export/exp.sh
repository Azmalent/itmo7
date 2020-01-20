#!/usr/bin/bash
ORACLE_USER=admin
ORACLE_PASSWORD=oracle
DATE=$(date +"%Y.%m.%d_%H.%M")

exp ${ORACLE_USER}/${ORACLE_PASSWORD} full=y \
    file=/u01/aee11/backup/auto/${DATE}_${ORACLE_SID}_full_export.dmp \
    log=/u01/aee11/backup/auto/${DATE}_${ORACLE_SID}_full_export.log \
    consistent=y statistics=none

scp -i ~/.ssh/id_rsa /u01/aee11/backup/auto/${DATE}* oracle@db192:/u01/aee11/backup/auto
