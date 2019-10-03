create or replace procedure print_mview_info
as
	schema varchar2(50);

	cursor mviews is 
		select name as view_name, referenced_name as table_name 
		from sys.dba_dependencies 
		where type = 'MATERIALIZED VIEW' and lower(owner) = lower(schema);

	n integer := 1;
	total number := 0;
begin
	dbms_output.enable();

	select '&schema' into schema from dual;
	
	select count(1) into total 
		from (select username from sys.dba_users) 
		where lower(username) = lower(schema) and rownum = 1;

	if total > 0 then
		dbms_output.put_line('Имя схемы: ' || schema || chr(13) || chr(10));

		dbms_output.put_line(' No. Имя представления                    Имя таблицы');
		dbms_output.put_line(' --- -----------------------------------  --------------------------------------');

		for row in mviews loop
			dbms_output.put_line(lpad(n, 4) || ' ' || rpad(row.view_name, 37) || row.table_name);
			n := n + 1;
		end loop;
	else
		dbms_output.put_line('Схема ' || schema || ' не найдена.');
	end if;

end print_mview_info;
/

prompt ===================================
prompt

set serveroutput on format wrapped
exec print_mview_info

select name as view_name, referenced_name as table_name 
		from sys.dba_dependencies 
		where type = 'MATERIALIZED VIEW' 
