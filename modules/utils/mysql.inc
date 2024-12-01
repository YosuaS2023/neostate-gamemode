function SQL_UpdateCharacterInt(playerid, struct[], value)
{
    if(!IsPlayerConnected(playerid)) return printf("*** [MySQL] The player ID %i does not exist on the server.", playerid);
    static sql_fmt[1080];
    mysql_format(sqlcon, sql_fmt, sizeof sql_fmt, "UPDATE `characters` SET `%s` = '%i' WHERE `pID` = %i;", struct, value, PlayerData[playerid][pID]);
    mysql_tquery(sqlcon, sql_fmt);
    return printf("%s", sql_fmt);
}

function SQL_UpdateCharacterFloat(playerid, struct[], Float:value)
{
    if(!IsPlayerConnected(playerid)) return printf("*** [MySQL] The player ID %i does not exist on the server.", playerid);
    static sql_fmt[1080];
    mysql_format(sqlcon, sql_fmt, sizeof sql_fmt, "UPDATE `characters` SET `%s` = '%f' WHERE `pID` = %i;", struct, value, PlayerData[playerid][pID]);
    mysql_tquery(sqlcon, sql_fmt);
    return printf("%s", sql_fmt);
}

function MySQL_LoadCharacterInt(playerid, struct[])
{
    if(!IsPlayerConnected(playerid)) return printf("*** [MySQL] The player ID %i does not exist on the server.", playerid);
    new sql_fmt[120], ret;
    mysql_format(sqlcon, sql_fmt, sizeof sql_fmt, "SELECT * FROM `characters` WHERE `pID` = %i;", PlayerData[playerid][pID]);
    new Cache:result = mysql_query(sqlcon, sql_fmt);
    ret = cache_get_field_int(0, struct);
    cache_delete(result);
    return ret;
}

function MySQL_LoadCharacter(playerid)
{
    for(new i; i < sizeof struct_character; i++)
    {
        switch(struct_character[i][structType])
        {
            case SQL_INT:
            {
                PlayerData[playerid][struct_character[i][structChar]] = MySQL_LoadCharacterInt(playerid, struct_character[i][structName]);
            }
        }
    }
    return 1;
}

function SQL_SaveCharacter(playerid)
{
    for(new i; i < sizeof struct_character; i++)
    {
        switch(struct_character[i][structType])
        {
            case SQL_INT:
            {
                SQL_UpdateCharacterInt(playerid, struct_character[i][structName], PlayerData[playerid][struct_character[i][structChar]]);
            }

        }
    }
    return 1;
}

function OnQueryFinished(extraid, threadid)
{
    switch(threadid)
    {
        case THREAD_INSERT_ECONOMY:
        {
            EconomyServer[extraid][economySQLID] = cache_insert_id();
            SaveEconomy();
        }
    }
    return 1;
}
function SQL_UpdateCustomInt(sqlid, table[], struct[], value)
{
    static sql_fmt[1080];
    mysql_format(sqlcon, sql_fmt, sizeof sql_fmt, "UPDATE `%s` SET `%s` = '%i' WHERE `SQLID` = %i;", table, struct, value, sqlid);
    mysql_tquery(sqlcon, sql_fmt);
    return printf("%s", sql_fmt);
}

function SQL_UpdateCustomFloat(sqlid, table[], struct[], Float:value)
{
    static sql_fmt[1080];
    mysql_format(sqlcon, sql_fmt, sizeof sql_fmt, "UPDATE `%s` SET `%s` = '%f' WHERE `SQLID` = %i;", table, struct, value, sqlid);
    mysql_tquery(sqlcon, sql_fmt);
    return printf("%s", sql_fmt);
}

function SQL_UpdateCustom(sqlid, table[], struct[], value[])
{
    static sql_fmt[1080];
    mysql_format(sqlcon, sql_fmt, sizeof sql_fmt, "UPDATE `%s` SET `%s` = '%s' WHERE `SQLID` = %i;", table, struct, value, sqlid);
    mysql_tquery(sqlcon, sql_fmt);
    return printf("%s", sql_fmt);
}
