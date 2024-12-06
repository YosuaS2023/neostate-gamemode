/*
    init
*/
OGMI:Trash()
{
    Trash_Load();
    return 1;
}

OGME:Trash()
{
    foreach(new i : Trash) Trash_Save(i);
    return 1;
}

task Trash[1000]()
{
    foreach(new i : Trash)
    {
        if(TrashData[i][TRASH_CLEAN])
        {
            TrashData[i][TRASH_COOLDOWN]++;
            if(TrashData[i][TRASH_COOLDOWN] >= Data_GetInt("config-trash", sprintf("cooldown-trash-%s", TrashData[i][TRASH_TYPE])))
            {
                TrashData[i][TRASH_CLEAN] = false;
                TrashData[i][TRASH_COOLDOWN] = 0;
                Trash_Sync(i);
            }
        }
    }
    return 1;
}

/*
    function general
*/
Trash_Near(playerid, Float:radius = 0.0)
{
    foreach(new i : Trash) if(IsPlayerInRangeOfPoint(playerid, radius, Array3X(TrashData[i][TRASH_POS])) && !TrashData[i][TRASH_CLEAN])
    {
        return i;
    }
    return -1;
}
/*
    function init
*/
Function:Trash_Load()
{
    new Cache:cache = mysql_query(sqlcon, "SELECT * FROM " SQL_TABLE_TRASH);
    {
        for(new i; i < cache_num_rows(); i++)
        {
            TrashData[i][TRASH_SQLID] = cache_get_field_int(i, "ID");
            TrashData[i][TRASH_OBJECTID] = cache_get_field_int(i, "Modelid");
            TrashData[i][TRASH_COOLDOWN] = cache_get_field_int(i, "Cooldown");

            cache_get_field_content(i, "Type", TrashData[i][TRASH_TYPE]);

            TrashData[i][TRASH_POS][0] = cache_get_field_float(i, "PosX");
            TrashData[i][TRASH_POS][1] = cache_get_field_float(i, "PosY");
            TrashData[i][TRASH_POS][2] = cache_get_field_float(i, "PosZ");

            TrashData[i][TRASH_ROT][0] = cache_get_field_float(i, "RotX");
            TrashData[i][TRASH_ROT][1] = cache_get_field_float(i, "RotY");
            TrashData[i][TRASH_ROT][2] = cache_get_field_float(i, "RotZ");

            TrashData[i][TRASH_CLEAN] = false;
            Iter_Add(Trash, i);
            Trash_Sync(i);
        }
        D:1("*** [Trash] Loaded %i rows", cache_num_rows());
    }
    cache_delete(cache);
    return 1;
}

Function:Trash_Create(playerid, modelid, const type[], Float:x, Float:y, Float:z)
{
    new id, string[512];
    if((id = Iter_Free(Trash)) != -1)
    {
        Iter_Add(Trash, id);
        TrashData[id][TRASH_POS][0] = x;
        TrashData[id][TRASH_POS][1] = y;
        TrashData[id][TRASH_POS][2] = z;
        
        strcat(TrashData[id][TRASH_TYPE], type);
        TrashData[id][TRASH_COOLDOWN] = 0;
        TrashData[id][TRASH_OBJECTID] = modelid;

        mysql_format(sqlcon, string, sizeof string, "INSERT INTO "SQL_TABLE_TRASH" SET `Type` = '%s', `PosX` = %f, `PosY`= %f, `PosZ` = %f", type, x, y, z);
        mysql_tquery(sqlcon, string, "OnInsertTrash", "ii", playerid, id);
        return id;
    }
    return -1;
}

Function:Trash_Copy(playerid, trashid)
{
    new
        modelid = Streamer_GetIntData(STREAMER_TYPE_OBJECT, TrashData[trashid][TRASH_OBJECT], E_STREAMER_MODEL_ID),
        Float:pos[3]
    ;

    Streamer_GetPosition(STREAMER_TYPE_OBJECT, TrashData[trashid][TRASH_OBJECT], pos[0], pos[1], pos[2]);

    new copyid = Trash_Create(playerid, modelid, TrashData[trashid][TRASH_TYPE], pos[0], pos[1], pos[2]);

    if(copyid == -1)
        return -1;

    return copyid;
}

Function:OnInsertTrash(playerid, trashid)
{
    TrashData[trashid][TRASH_SQLID] = cache_insert_id();
    WriteLog("trash.log", "[Trash Insert] membuat sqlid trash %i", cache_insert_id());
    Trash_Save(trashid);
    Trash_Sync(trashid);

    PlayerData[playerid][pEditingMode] = EDIT_OBJECT_TRASH;
    PlayerData[playerid][pEditTrash] = trashid;
    EditDynamicObject(playerid, TrashData[trashid][TRASH_OBJECT]);
    return 1;
}

Function:Trash_Save(trashid)
{
    new string[1080];
    format(string, 1080, "UPDATE `trash` SET `Modelid` = %i, `Cooldown` = %i, `Type` = '%s', `PosX` = %f, `PosY` = %f, `PosZ` = %f, `RotX` = %f, `RotY`= %f, `RotZ`= %f WHERE `ID` = %i",
        TrashData[trashid][TRASH_OBJECTID],
        TrashData[trashid][TRASH_COOLDOWN],
        TrashData[trashid][TRASH_TYPE],
        TrashData[trashid][TRASH_POS][0], TrashData[trashid][TRASH_POS][1], TrashData[trashid][TRASH_POS][2],
        TrashData[trashid][TRASH_ROT][0], TrashData[trashid][TRASH_ROT][1], TrashData[trashid][TRASH_ROT][2],
        TrashData[trashid][TRASH_SQLID]
    );
    mysql_tquery(sqlcon, string);
    printf(string);
    return 1;
}

Function:Trash_Sync(trashid)
{
	if(Iter_Contains(Trash, trashid))
	{
		if(IsValidDynamicObject(TrashData[trashid][TRASH_OBJECT]))
		{
			Streamer_SetPosition(STREAMER_TYPE_OBJECT, TrashData[trashid][TRASH_OBJECT], Array3X(TrashData[trashid][TRASH_POS]));
			Streamer_SetRotation(STREAMER_TYPE_OBJECT, TrashData[trashid][TRASH_OBJECT], Array3X(TrashData[trashid][TRASH_ROT]));
		}
		else if(!TrashData[trashid][TRASH_CLEAN])
		{
            TrashData[trashid][TRASH_OBJECT] = CreateDynamicObject(TrashData[trashid][TRASH_OBJECTID],  Array3X(TrashData[trashid][TRASH_POS]), Array3X(TrashData[trashid][TRASH_ROT]));
		}
		else 
		{
            TrashData[trashid][TRASH_OBJECT] = CreateDynamicObject(TrashData[trashid][TRASH_OBJECTID],  Array3X(TrashData[trashid][TRASH_POS]), Array3X(TrashData[trashid][TRASH_ROT]));
		}
        return 1;
	}
    return 0;
}

Function:Trash_Clear(trashid)
{
    if(!Data_GetInt("config-trash", sprintf("cooldown-trash-%s", TrashData[trashid][TRASH_TYPE])))
    {
        D:3("Trash_Clear(trashid = %i) tidak bisa dijalankan karna cooldown-trash-%s tidak ada atau tidak memiliki nilai", trashid, TrashData[trashid][TRASH_TYPE]);
        return 0;
    }
    if(IsValidDynamicObject(TrashData[trashid][TRASH_OBJECT])) {
        DestroyDynamicObject(TrashData[trashid][TRASH_OBJECT]);
        TrashData[trashid][TRASH_OBJECT] = INVALID_STREAMER_ID;
        TrashData[trashid][TRASH_CLEAN] = true;
        return 1;
    }
    
    return 0;
}

CMD:trashmenu(playerid, params[])
{
    new option[32], arg[123];
    if(sscanf(params, "s[32]S()[123]", option, arg))
        return SendSyntaxMessage(playerid, "/trashmenu [create/delete/edit]");

    switch(HashStr(option))
    {
        case HS<create>:
        {
            new type[30], Float:pos[3];
            if(sscanf(arg, "s[30]", type)) return SendSyntaxMessage(playerid, "/trashmenu create [string:type]");

            GetPlayerPos(playerid, Array3X(pos));

            new trashid = Trash_Create(playerid, Data_GetInt("config-trash", sprintf("def-modelid-trash-%s", type)), type, Array3X(pos));
            if(trashid == -1) return SendErrorMessage(playerid, "an error occurs when making trashid");
            else SendAdminMessage(X11_TOMATO, "%s membuat trash(id: %i) dengan type: %s", ReturnName(playerid), trashid, type);
        }
        case HS<delete>:
        {
            new id;
            if(sscanf(arg, "i", id)) return SendSyntaxMessage(playerid, "/trashmenu delete [int:trashid]");
            if(!Iter_Contains(Trash, id)) return SendErrorMessage(playerid, "Invalid trash ID!");

            DestroyDynamicObject(TrashData[id][TRASH_OBJECT]);

            mysql_tquery(sqlcon, sprintf("DELETE FROM "SQL_TABLE_TRASH" WHERE `ID` = '%i'", TrashData[id][TRASH_SQLID]));
            Iter_Remove(Trash, id);

            new empty[E_TRASH];
            TrashData[id] = empty;
            SendServerMessage(playerid, "delete trash id %i", id);
        }
        case HS<edit>:
        {
            new id;
            if(sscanf(arg, "i", id)) return SendSyntaxMessage(playerid, "/trashmenu delete [int:trashid]");
            if(PlayerData[playerid][pEditingMode] != NOT_EDIT_OBJECT) return SendErrorMessage(playerid, "You are still in an editing of an object!");
            if(!Iter_Contains(Trash, id)) return SendErrorMessage(playerid, "Invalid trash ID!");

            PlayerData[playerid][pEditingMode] = EDIT_OBJECT_TRASH;
            PlayerData[playerid][pEditTrash] = id;
            EditDynamicObject(playerid, TrashData[id][TRASH_OBJECT]);
            SendCustomMessage(playerid , "TRASH", "You are Editing Trashid: %i", id);
        }
        case HS<clone>:
        {
            new id;
            if(sscanf(arg, "i", id)) return SendSyntaxMessage(playerid, "/trashmenu clone [int:trashid]");
            if(PlayerData[playerid][pEditingMode] != NOT_EDIT_OBJECT) return SendErrorMessage(playerid, "You are still in an editing of an object!");
            if(!Iter_Contains(Trash, id)) return SendErrorMessage(playerid, "Invalid trash ID!");

            new copyid = Trash_Copy(playerid, id);
            if(copyid == -1) 
                return SendErrorMessage(playerid, "an error occurs when making trashid!");
            SendCustomMessage(playerid , "TRASH", "You managed to clone the trashid %i", id);
        }
    }
    return 1;
}