#define MAX_SERVER_DATA 100
#define MAX_LIB_SERVER_DATA 30

enum E_SERVER_DATA
{
    ORM:serverORM,
    serverID,
    serverName[50],
    serverLib[50],
    
    serverValueStr[80],
    serverValueInt,
    Float:serverValueFloat
}

enum E_LIB_SERVER_DATA
{
    LIB_ID, // simpan id file
    LIB_NAME[50]
}

new ServerLib[MAX_LIB_SERVER_DATA][E_LIB_SERVER_DATA];
new ServerData[MAX_SERVER_DATA][E_SERVER_DATA];

new Iterator:ServerData<MAX_SERVER_DATA>;
new Iterator:ServerLib<MAX_LIB_SERVER_DATA>;

new ListedServerData[MAX_PLAYERS][100];

Function:Data_GetPosition(library[], data[], &Float:x, &Float:y, &Float:z)
{
    foreach(new i : ServerData) if(!strcmp(library, ServerData[i][serverLib], true) && !strcmp(data, ServerData[i][serverName], true))
    {
        sscanf(ServerData[i][serverValueStr], "p<,>fff", x, y, z);
        D:4("Data_GetPosition(library[] = \"%s\", string[] = \"%s\", x=%f, y=%f, z=%f)", library, data, x,y,z);
        return 1;
    }
    return 0;
}
Float:Data_GetFloat(library[], string[])
{
    foreach(new i : ServerData) if(!strcmp(library, ServerData[i][serverLib], true) && !strcmp(string, ServerData[i][serverName], true))
    {
        D:4("Data_GetFloat(string[] = \"%s\") succes load variable : %s = %f", string, ServerData[i][serverName], ServerData[i][serverValueFloat]);
        return ServerData[i][serverValueFloat];
    }
	return 0.0;
}

Data_GetInt(library[], string[])
{
	foreach(new i : ServerData) if(!strcmp(library, ServerData[i][serverLib], true) && !strcmp(string, ServerData[i][serverName], true))
	{
		D:4("Data_GetInt(setting[] = \"%s\") succes load variable : %s = %i", string, ServerData[i][serverName], ServerData[i][serverValueInt]);
		return ServerData[i][serverValueInt];
	}
	return 0;
}

Data_GetString(library[], string[], output[], len = sizeof(output))
{
	foreach(new i : ServerData) if(!strcmp(library, ServerData[i][serverLib], true) && !strcmp(string, ServerData[i][serverName], true))
	{
        strcat(output, ServerData[i][serverValueStr], len);
		D:4("Data_GetString(string[] = \"%s\", output[] = \"%s\", len = %i) succes load variable : %s = %s", string, output, len, ServerData[i][serverName], ServerData[i][serverValueStr]);
		return 1;
	}
	return 0;
}

/*
    function init
*/
Function:OnInsertServerData(id)
{
    ServerData[id][serverID] = cache_insert_id();
    Data_Save(id);
    return 1;
}

Function:Data_Save(id)
{
    new str[500];
    mysql_format(sqlcon, str, sizeof(str), "UPDATE " SQL_TABLE_SERVER_DATA"SET `Name` = '%s', `lib` = '%s', `value_string`='%s', `value_integer`=%i, `value_float`=%.1f WHERE `ID` = %i",
    ServerData[id][serverName],
    ServerData[id][serverLib],
    ServerData[id][serverValueStr],
    ServerData[id][serverValueInt],
    ServerData[id][serverValueFloat],
    ServerData[id][serverID]);
    mysql_tquery(sqlcon, str);
    D:1("*** [Server Setting] Update data | SQLID: %i | Name: %s | Library: %s | Float: %.1f | String: %s | Integer: %i", ServerData[id][serverID], ServerData[id][serverName], ServerData[id][serverLib], ServerData[id][serverValueFloat], ServerData[id][serverValueStr], ServerData[id][serverValueInt]);
    return 1;
}

Dialog_ServerMenu(playerid, const string[])
{
    yield 1;
    static dialog[dialog_data];
    await_arr(dialog) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_TABLIST_HEADERS, "Server Data Display", string, "Select", "Exit");

    if(dialog[Response])
    {
        new listitem = ListedServerData[playerid][dialog[Listitem]],
            str[1080],
            count
        ;
        strcat(str, "#\tLibrary Data\n");
        foreach(new id : ServerData) if(!strcmp(ServerLib[listitem][LIB_NAME], ServerData[id][serverLib], true))
        {
            format(str, sizeof(str), "%s%i\t%s\n", str, count, ServerData[id][serverName]);
            ListedServerData[playerid][count++] = id;
        }
        static show[dialog_data];
        await_arr(show) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_TABLIST_HEADERS, "Server Data Menu", str, "Select", "Back");
        if(show[Response])
        {
            new listid = ListedServerData[playerid][show[Listitem]];

            static edit[dialog_data];
            await_arr(edit) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_INPUT, "Edit Server Data", "Enter the data type and its value\nExample:\n\n\tstring:ini adalah text\n\tfloat:10.0\n\tint:100", "input", "back");
            if(edit[Response])
            {
                new type_data[10], input_data[80];
                if(sscanf(edit[Inputtext], "p<:>s[10]s[80]", type_data, input_data))
                    return Dialog_ServerMenu(playerid, string), SendErrorMessage(playerid, "You have to enter the data!");
                
                switch(HashStr(type_data))
                {
                    case HS<string>:
                    {
                        strcat(ServerData[listid][serverValueStr], input_data);
                        SendCustomMessage(playerid, "SETTING", "anda berhasil mengubah data string %s menjadi:", ServerData[listid][serverName]);
                        SendCustomMessage(playerid, "SETTING", "%s", ServerData[listid][serverValueStr]);
                    }
                    case HS<int>:
                    {
                        ServerData[listid][serverValueInt] = strval(input_data);
                        SendCustomMessage(playerid, "SETTING", "anda berhasil mengubah data int %s menjadi: %i", ServerData[listid][serverName], ServerData[listid][serverValueInt]);
                    }
                    case HS<float>:
                    {
                        ServerData[listid][serverValueFloat] = floatstr(input_data);
                        SendCustomMessage(playerid, "SETTING", "anda berhasil mengubah data float %s menjadi: %i", ServerData[listid][serverName], ServerData[listid][serverValueFloat]);
                    }
                }
            }
        }
    }
    return 1;
}
/*
    init / exit
*/
OGMI:ServerData()
{
    new Cache:cache = mysql_query(sqlcon, "SELECT * FROM " SQL_TABLE_SERVER_DATA);
    {
        for(new i; i < cache_num_rows(); i++)
        {
            ServerData[i][serverID] = cache_get_field_content_int(i, "ID");
            cache_get_field_content(i, "Name", ServerData[i][serverName]);
            cache_get_field_content(i, "lib", ServerData[i][serverLib]);
            cache_get_field_content(i, "value_string", ServerData[i][serverValueStr]);
            ServerData[i][serverValueInt] = cache_get_field_content_int(i, "value_integer");
            ServerData[i][serverValueFloat] = cache_get_field_content_float(i, "value_float");
            printf("*** [Server Data] sqlid: %i, name: %s", ServerData[i][serverID], ServerData[i][serverName]);
            Iter_Add(ServerData, i);
        }
    }
    printf("*** [Server Data] Loading server data %i rows.. ", cache_num_rows());
    cache_delete(cache);

    new id;
    for(new dini; dini < MAX_LIB_SERVER_DATA; dini++) if((id = Iter_Free(ServerLib)) != -1)
    {
        new file[50];
        format(file, sizeof file, "server_data_lib/%i.ini", dini);
        if(dini_Exists(file))
        {
            ServerLib[id][LIB_ID] = dini;
            strcat(ServerLib[id][LIB_NAME], dini_Get(file, "name-lib"));
            Iter_Add(ServerLib, id);
            D:3("*** [Server Lib] loaded \"%s\" with name-lib: %s | iter-id: %i(id_file: %i)", file, ServerLib[id][LIB_NAME], id, ServerLib[id][LIB_ID]);
        }
    }
	return 1;
}

OGME:ServerData()
{
	foreach(new id : ServerData) {
        Data_Save(id);
    }
	printf("[SETTING SERVER] Successfully saving setting server");
	return 1;
}

CMD:smenu(playerid, params[])
{
    if(IsPlayerADuty(playerid, DEVELOPER))
    {
        new opt[32], arg[128];
        if(sscanf(params, "s[32]S()[123]", opt, arg))
            return SendSyntaxMessage(playerid, "/smenu [createdata/createlib/deletedata/deletelib/list]");
        
        switch(HashStr(opt))
        {
            case HS<getpos>:
            {
                new lib[50], data[50];
                if(sscanf(arg, "p<,>s[50]s[50]", lib, data)) return SendSyntaxMessage(playerid, "/smenu getpos [string:data]");
                foreach(new i : ServerData) if(!strcmp(ServerData[i][serverLib], lib, true) && !strcmp(ServerData[i][serverName], data, true))
                {
                    new Float:pos[3];
                    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
                    format(ServerData[i][serverValueStr], 80, "%f,%f,%f", pos[0], pos[1], pos[2]);
                    SendAdminMessage(X11_TOMATO, "AdmCmd: %s create coordinates with data server \"%s/%s\"",ReturnName(playerid), lib, data);
                    return 1;
                }
                return SendErrorMessage(playerid, "No found library \"%s\" or data \"%s\"", lib, data);
            }
            case HS<createdata>:
            {
                new names[50], libid;
                if(sscanf(arg, "is[50]",  libid, names)) return SendSyntaxMessage(playerid, "/servermenu createdata [libid] [name-data]");
                if(!Iter_Contains(ServerLib, libid)) return SendErrorMessage(playerid, "Invalid server-lib ID!");

                new i = Iter_Free(ServerData);
                if(i != -1)
                {
                    format(ServerData[i][serverName], 50, names);
                    format(ServerData[i][serverLib], 50, ServerLib[libid][LIB_NAME]);
                    format(ServerData[i][serverValueStr], 50, "none");
                    ServerData[i][serverValueInt] = 0;
                    ServerData[i][serverValueFloat] = 0.0;
                    new str[200];
                    Iter_Add(ServerData, i);
                    mysql_format(sqlcon, str, 180, "INSERT INTO " SQL_TABLE_SERVER_DATA" SET `Name` = '%s'", names);
                    mysql_tquery(sqlcon, str, "OnInsertServerData", "i", i);
                    SendAdminMessage(X11_TOMATO, "AdmCmd: %s making data \"%s\" in Library \"%s\"", ReturnName(playerid), names, ServerLib[libid][LIB_NAME]);
                }
            }
            case HS<createlib>:
            {
                new libname[50];
                if(sscanf(arg, "s[50]", libname)) return SendSyntaxMessage(playerid, "/servermenu createlib [lib-name]");
                new id = Iter_Free(ServerLib), file[300];
                
                for(new dini; dini < MAX_LIB_SERVER_DATA; dini++)
                {
                    format(file, 300, "server_data_lib/%i.ini", dini);
                    if(!dini_Exists(file))
                    {
                        dini_Create(file);
                        dini_Set(file, "name-lib", libname);
                        ServerLib[id][LIB_ID] = dini;
                        strcat(ServerLib[id][LIB_NAME], libname);
                        SendAdminMessage(X11_TOMATO, "%s create a data server library \"%s(id:%i, dini: %i)\"", ReturnName(playerid), ServerLib[id][LIB_NAME], id, ServerLib[id][LIB_ID]);
                        Iter_Add(ServerLib, id);
                        break;
                    }
                }
            }
            case HS<deletelib>:
            {
                new libid;
                if(sscanf(arg, "i", libid)) return SendSyntaxMessage(playerid, "/servermenu deletelib [libid]");
                if(!Iter_Contains(ServerLib, libid)) return SendErrorMessage(playerid, "Invalid server-lib ID!");
                new file[300];
                format(file, 300, "server_data_lib/%i.ini", ServerLib[libid][LIB_ID]);
                if(dini_Exists(file))
                {
                    dini_Remove(file);
                    ServerLib[libid][LIB_ID] = 0;

                    foreach(new dataid : ServerData) if(!strcmp(ServerLib[libid][LIB_NAME], ServerData[dataid][serverLib]))
                    {
                        ServerData[dataid][serverLib][0] = EOS;
                        ServerData[dataid][serverName][0] = EOS;
                        mysql_tquery(sqlcon, sprintf("DELETE FROM "SQL_TABLE_SERVER_DATA" WHERE `lib` = '%s'", ServerLib[libid][LIB_NAME]));
                        Iter_Remove(ServerData, dataid);
                    }
                    D:3("*** [Server Lib] delete \"%s\"", file);
                    Iter_Remove(ServerLib, libid);
                }
            }
            case HS<list>:
            {
                new str[1080], count;
                format(str, sizeof str, "#\tLibrary Data Server\n");
                foreach(new i : ServerLib)
                {
                    format(str, sizeof(str), "%s%i\t%s\n", str, i, ServerLib[i][LIB_NAME]);
                    ListedServerData[playerid][count++] = i;
                }
                Dialog_ServerMenu(playerid, str);
            }
        }
        return 1;
    }
    PermissionError(playerid);
    return 1;
}