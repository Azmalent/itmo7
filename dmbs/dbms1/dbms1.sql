create or replace procedure print_mview_info
as
	my_schema varchar2(7);

	cursor my_mviews is 
		select name as view_name, referenced_name as table_name
		from sys.dba_dependencies 
		where type = 'MATERIALIZED VIEW' and lower(owner) = lower(my_schema);

	n integer := 1;
begin
	select sys_context('userenv', 'current_schema') into my_schema from dual;

	dbms_output.enable();
	dbms_output.put_line('Имя схемы: ' || my_schema || chr(13) || chr(10));

	dbms_output.put_line(' No. Имя представления                    Имя таблицы');
	dbms_output.put_line(' --- -----------------------------------  --------------------------------------');

	for row in my_mviews loop
		dbms_output.put_line(lpad(n, 4) || ' ' || rpad(row.view_name, 37) || row.table_name);
		n := n + 1;
	end loop;

end print_mview_info;
/

prompt ===================================
prompt

set serveroutput on format wrapped
exec print_mview_info