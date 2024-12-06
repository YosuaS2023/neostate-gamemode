
#define cx_mysql_impl
enum SQL_TYPE
{
	SQL_INVALID,
	SQL_INT,
	SQL_VARCHAR,
	SQL_FLOAT
};

stock AddTableColumn(MySQL:sql, table[], column[], type, def[], after[],length[])
{
	static cx_sqlf[300];
	if(strlen(table) <= 0) return printf("[CoreX] (mysql) - The 'table' parameter is not used");
	if(strlen(column) <= 0) return printf("[CoreX] (mysql) - The 'column' parameter is not used");
	if(type == SQL_INVALID) return printf("[CoreX] (mysql) - The 'type' parameter is not used");
	if(strlen(after) <= 0) return printf("[CoreX] (mysql) - The 'after' parameter is not used");
	if(strlen(length) <= 0) return printf("[CoreX] (mysql) - The 'length' parameter is not used");
	switch(type)
	{
		case SQL_INT:
		{
			mysql_format(sql, cx_sqlf, sizeof(cx_sqlf), "ALTER TABLE `%s` ADD `%s` INT(%s) NULL DEFAULT '%s' AFTER `%s`", table, column, length, def, after);
		}
		case SQL_VARCHAR:
		{
			mysql_format(sql, cx_sqlf, sizeof(cx_sqlf), "ALTER TABLE `%s` ADD `%s` VARCHAR(%s) NULL DEFAULT '%s' AFTER `%s`", table, column, length, def, after);
		}
		case SQL_FLOAT:
		{
			mysql_format(sql, cx_sqlf, sizeof(cx_sqlf), "ALTER TABLE `%s` ADD `%s` FLOAT(%s) NULL DEFAULT '%s' AFTER `%s`", table, column, length, def, after);
		}
	}
	mysql_tquery(sql, cx_sqlf);
	printf("*** [CoreX] (mysql) success | query: %s", cx_sqlf);
	return 1;
}

cache_get_field_content(row, const field_name[], destination[], max_len = sizeof(destination))
{
    cache_get_value_name(row, field_name, destination, max_len);
}

stock cache_get_data(&rows, &fields)
{
    cache_get_row_count(rows);
    cache_get_field_count(fields);
}

#define cache_get_field_content_int cache_get_field_int
cache_get_field_int(row, const field_name[])
{
    new val;
    cache_get_value_name_int(row, field_name, val);
    return val;
}

#define cache_get_field_content_float cache_get_field_float
stock Float:cache_get_field_float(row, field_name[])
{
    new
        Float:ret;

    cache_get_value_name_float(row, field_name, ret);
    return ret;
}