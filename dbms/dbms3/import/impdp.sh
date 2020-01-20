#!/usr/bin/bash
read -r filename
impdp admin/oracle content=all table_exists_action=replace full=y directory=DATAPUMP_DIR dumpfile="$filename" 
