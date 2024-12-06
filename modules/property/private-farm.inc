
enum E_FARM
{
	farmID,
	farmName[50],
	farmOwner[MAX_PLAYER_NAME],
	farmPrice,
	Float:farmPosX,
	Float:farmPosY,
	Float:farmPosZ,
	Float:farmAreaX,
	Float:farmAreaY,
	Float:farmAreaZ,
	farmPickup,
	Text3D:farmLabel
};

new FarmData[MAX_FARM][E_FARM],
	Iterator:FarmIds<MAX_FARM>;

Farm_Save(id)
{
	new dquery[2048];
	format(dquery, sizeof(dquery), "UPDATE farm SET name='%s', owner='%s', price='%d', posx='%f', posy='%f', posz='%f'",
	FarmData[id][farmName],
	FarmData[id][farmOwner],
	FarmData[id][farmPrice],
	FarmData[id][farmPosX],
	FarmData[id][farmPosY],
	FarmData[id][farmPosZ]);

	format(dquery, sizeof(dquery), "%s, areax='%f', areay='%f', areaz='%f' WHERE ID='%d'",
	dquery,
	FarmData[id][farmAreaX],
	FarmData[id][farmAreaY],
	FarmData[id][farmAreaZ],
	FarmData[id][farmID]);
	return mysql_tquery(sqlcon, dquery);
}

Farm_Refresh(id)
{
	if(id != -1)
	{
        if(IsValidDynamicPickup(FarmData[id][farmPickup]))
            DestroyDynamicPickup(FarmData[id][farmPickup]);

        if(IsValidDynamic3DTextLabel(FarmData[id][farmLabel]))
            DestroyDynamic3DTextLabel(FarmData[id][farmLabel]);

		new string[900];
        if(strcmp(FarmData[id][farmOwner], "-"))
		{
            format(string, sizeof(string), "[ID: %i]\n{FFFFFF}Name: "GREEN"%s", id, FarmData[id][farmName]);
            FarmData[id][farmPickup] = CreateDynamicPickup(19132, 23, FarmData[id][farmPosX], FarmData[id][farmPosY], FarmData[id][farmPosZ]+0.2, 0, 0, _, 8.0);
			FarmData[id][farmLabel] = CreateDynamic3DTextLabel(string, COLOR_ARWIN, FarmData[id][farmPosX], FarmData[id][farmPosY], FarmData[id][farmPosZ]+0.5, 2.5, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
		}		
        else
        {
            format(string, sizeof(string), "[ID: %i]\n{00D900}This farm is for sale\n{FFFF00}Price: {00FF00}$%s\n{00D900}use '/buy' for buy this Farm", id, FormatNumber(FarmData[id][farmPrice]));
            FarmData[id][farmPickup] = CreateDynamicPickup(19132, 23, FarmData[id][farmPosX], FarmData[id][farmPosY], FarmData[id][farmPosZ]+0.2, 0, 0, _, 8.0);
			FarmData[id][farmLabel] = CreateDynamic3DTextLabel(string, COLOR_ARWIN, FarmData[id][farmPosX], FarmData[id][farmPosY], FarmData[id][farmPosZ]+0.5, 2.5, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
		}
		return id;
	}
	return -1;
}

function OnFarmCreated(id)
{
	FarmData[id][farmID] = cache_insert_id();
	Farm_Save(id);
	Farm_Refresh(id);
	return 1;
}

function Farm_Load()
{
    new rows = cache_num_rows();
 	if(rows)
  	{
   		new fid, name[50], owner[MAX_PLAYER_NAME];
		for(new i; i < rows; i++) if((fid = Iter_Free(FarmIds)) != INVALID_ITER_ID)
		{
  			cache_get_value_name_int(i, "ID", FarmData[fid][farmID]);
	    	cache_get_value_name(i, "name", FarmData[fid][farmName]);
		    cache_get_value_name(i, "owner", FarmData[fid][farmOwner]);

			cache_get_value_name_int(i, "price", FarmData[fid][farmPrice]);
		    cache_get_value_name_float(i, "extposx", FarmData[fid][farmPosX]);
		    cache_get_value_name_float(i, "extposy", FarmData[fid][farmPosY]);
		    cache_get_value_name_float(i, "extposz", FarmData[fid][farmPosZ]);
			cache_get_value_name_float(i, "safex", FarmData[fid][farmAreaX]);
			cache_get_value_name_float(i, "safey", FarmData[fid][farmAreaY]);
			cache_get_value_name_float(i, "safez", FarmData[fid][farmAreaZ]);
			cache_get_value_name_int(i, "product", FarmData[fid][laProduct]);
			cache_get_value_name_int(i, "white", FarmData[fid][laWhite]);
			cache_get_value_name_int(i, "orange", FarmData[fid][laOrange]);

			Iter_Add(FarmIds, fid);
			Farm_Refresh(fid);
	    }
	    printf("*** [Database: Loaded] farmer data (%d count).", rows);
	}
}

CMD:farmamenu(playerid, params[])
{
	if(!IsPlayerADuty(playerid, 5))
		return SendSendErrorMessageMessage(playerid, DUTY_COMMAND);

	new option[32], arg[123];
	if(sscanf(params, "s[32]S()[123]", option, arg))
		return SendSyntaxMessage(playerid, "/farmamenu [create]");

	switch(HashStr(option))
	{
		case HS<create>:
		{
			new fid;
			if((fid = Iter_Free(PlantIds)) != INVALID_ITER_ID)
			{
				new price, query[128];
				if(sscanf(arg, "d", price)) return SendSyntaxMessage(playerid, "/farmamnenu create [price]");

				format(FarmData[fid][farmName], 50, "No Name");
				format(FarmData[fid][farmOwner], 50, "-");
				format(FarmData[fid][laPegawai1], 50, "-");
				format(FarmData[fid][laPegawai2], 50, "-");
				format(FarmData[fid][laPegawai3], 50, "-");
				FarmData[fid][farmPrice] = price;

				GetPlayerPos(playerid, FarmData[fid][farmPosX], FarmData[fid][farmPosY], FarmData[fid][farmPosZ]);

				FarmData[fid][laProduct] = 0;
				FarmData[fid][farmAreaX] = 0;
				FarmData[fid][farmAreaY] = 0;
				FarmData[fid][farmAreaZ] = 0;

				Iter_Add(FarmIds, fid);
				SendAdminMessage(COLOR_ARWIN, "AdmWarn: "YELLOW_E"%s "WHITE_E"telah membuat Ladang ID "YELLOW_E"%d Harga "GREEN_E"$%s.", PlayerData[playerid][pAccount], fid, FormatNumber(price));
				mysql_format(sqlcon, query, sizeof(query), "INSERT INTO farm SET price='%d'", price);
				mysql_tquery(sqlcon, query, "OnFarmCreated", "i", fid);
			}
			return SendSendErrorMessageMessage(playerid, "You cant create more farm slot empty!");
		}
		case HS<delete>:
		{
			new fid, query[128];
			if(sscanf(arg, "i", fid)) return SendSyntaxMessage(playerid, "/farmamenu delete [fid]");
			if(!Iter_Contains(FarmIds, fid)) return SendSendErrorMessageMessage(playerid, "The you specified ID of doesn't exist.");

			format(FarmData[fid][farmName], 50, "None");
			format(FarmData[fid][farmOwner], 50, "None");
			format(FarmData[fid][laPegawai1], 50, "-");
			format(FarmData[fid][laPegawai2], 50, "-");
			format(FarmData[fid][laPegawai3], 50, "-");
			FarmData[fid][farmPosX] = 0;
			FarmData[fid][farmPosY] = 0;
			FarmData[fid][farmPosZ] = 0;
			
			FarmData[fid][laProduct] = 0;
			FarmData[fid][farmAreaX] = 0;
			FarmData[fid][farmAreaY] = 0;
			FarmData[fid][farmAreaZ] = 0;

			Iter_Remove(FarmIds, fid);

			mysql_format(sqlcon, query, sizeof(query), "UPDATE characters SET farmID=-1, farmRank=0 WHERE farmID=%d", fid);
			mysql_tquery(sqlcon, query);

			foreach(new ii : Player)
			{
				if(PlayerData[ii][pFarm] == fid)
				{
					PlayerData[ii][pFarm]= -1;
					PlayerData[ii][pFarmRank] = 0;
				}
			}
			mysql_format(sqlcon, query, sizeof(query), "DELETE FROM farm WHERE ID=%d", fid);
			mysql_tquery(sqlcon, query);
			SendAdminMessage(TOMATO, "AdmCmd: %s telah menghapus LAdang ID: %d.", PlayerData[playerid][pAccount], fid);
		}
	}
	return 1;
}

CMD:farmedit(playerid, params[])
{
    static
        fid,
        type[24],
        string[128];

    if(!IsPlayerADuty(playerid, 5))
        return SendSendErrorMessageMessage(playerid,DUTY_COMMAND);

    if(sscanf(params, "ds[24]S()[128]", fid, type, string))
    {
        SendSyntaxMessage(playerid, "/farmedit [id] [name]");
        SendClientMessage(playerid, COLOR_SERVER, "OPTION:{FFFFFF} location, name, area");
        return 1;
    }
	if(!Iter_Contains(FarmIds, fid)) return SendErrorMessage(playerid, "The you specified ID of doesn't exist.");

    if(!strcmp(type, "location", true))
    {
		GetPlayerPos(playerid, FarmData[fid][farmPosX], FarmData[fid][farmPosY], FarmData[fid][farmPosZ]);

        Farm_Save(fid);
        Farm_Refresh(fid);
        SendAdminMessage(TOMATO, "AdmCmd: %s has adjusted the location of entrance ID: %d.", PlayerData[playerid][pAccount], fid);
    }
    else if(!strcmp(type, "name", true))
    {
        new name[50];

        if(sscanf(string, "s[50]", name))
            return SendSyntaxMessage(playerid, "/farmedit [id] [name] [text:name]");

        format(FarmData[fid][farmName], 50, name);
		Farm_Save(fid);

        SendAdminMessage(TOMATO, "AdmCmd: %s has changed the Ladang name ID: %d to: %s.", PlayerData[playerid][pAccount], fid, name);
    }
    if(!strcmp(type, "area", true))
    {
		GetPlayerPos(playerid, FarmData[fid][farmAreaX], FarmData[fid][farmAreaY], FarmData[fid][farmAreaZ]);

        Farm_Save(fid);
        Farm_Refresh(fid);
        SendStaffMessage(TOMATO, "AdmCmd: %s has adjusted the location of entrance ID: %d.", PlayerData[playerid][pAccount], fid);
    }
    return 1;
}

CMD:farmmenu(playerid, params[])
{
	if(sscanf(params, "s[32]S()[123]", opt, arg))
		return SendSyntaxMessage(playerid, "/farmmenu [info]");

	if((fid = Farm_NearIsOwner(playerid)) == -1)
        return SendErrorMessage(playerid, "You must owner farm to use this command!");

	switch(HashStr(opt))
	{
		case HS<info>: Farm_ShowInfo(playerid, fid)
	}
	return 1;
}

function Farm_ShowInfo(playerid, fid)
{
	new Cache:cache;
	mysql_format(sqlcon, query, sizeof query, "SELECT * FROM `farm` WHERE `ID` = '%i'", fid);
	cache = mysql_query(sqlcon, query);

	new rows = cache_num_rows();
 	if(rows)
  	{
 		new name[50],
 			owner[50],
            wheat,
			orange,
			product,
			string[512];

		cache_get_value_name(0, "name", name);
		cache_get_value_name(0, "owner", owner);

		format(string, sizeof(string), "Farm ID: %d\nFarm Name: %s\nFarm owner: %s", fid, name, owner);

		ShowPlayerDialog(playerid, "unused", DIALOG_STYLE_MSGBOX, "Farm Info", string, "Okay", "");
	}
	cache_delete(cache);
	return 1;
}

CMD:sellfarm(playerid, params[])
{
	new fid = PlayerData[playerid][pFarm];
	if(PlayerData[playerid][pFarm] == -1)
        return SendErrorMessage(playerid, "You must in farm member to use this command!");
	if(PlayerData[playerid][pFarmRank] < 6)
		return SendErrorMessage(playerid, "You must farm owner!");
	new string[212];
	new pay;
	pay = FarmData[fid][farmPrice] / 80;
	format(string, sizeof(string), ""WHITE_E"Farm Name: "YELLOW_E"%s\n"WHITE_E"Farm Price: "GREEN_E"%s\nAnda Yakin ingin menjual farm", FarmData[fid][farmName], FormatMoney(pay));
	ShowPlayerDialog(playerid, DIALOG_FARMSELL, DIALOG_STYLE_MSGBOX, "Farm Sell", string, "Okay", "Cancel");
	return 1;	
}

CMD:givefarm(playerid, params[])
{
	new otherid;
	new fid = Farm_NearIsOwner(playerid);
	if(fid == -1)
        return SendErrorMessage(playerid, "You must in farm member to use this command!");
		
	if(sscanf(params, "u", otherid)) 
		return SendSyntaxMessage(playerid, "/givefarm [playerid/name]");
	
	if(!IsPlayerConnected(otherid) || !NearPlayer(playerid, otherid, 4.0))
        return SendErrorMessage(playerid, "The specified player is disconnected or not near you.");

	GetPlayerName(otherid, FarmData[fid][farmOwner], MAX_PLAYER_NAME);
	SendClientMessageEx(playerid, COLOR_ARWIN, "FARM: "WHITE_E"Anda berhasil memberika farm kepada player "YELLOW_E"%s", PlayerData[otherid][pName]);
	SendClientMessageEx(otherid, COLOR_ARWIN, "FARM: "YELLOW_E"%s "WHITE_E"berhasil memberikan farm kepada anda", PlayerData[playerid][pName]);
	Farm_Refresh(fid);
	Farm_Save(fid);
	return 1;	
}