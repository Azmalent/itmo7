#!/usr/bin/bash
read -r filename
imp admin/oracle full=y file="/u01/aee11/backup/auto/$filename" constraints=n ignore=y
