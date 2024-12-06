enum
{
	GPS_TYPE_LIBRARY = 0,
	GPS_TYPE_HOUSE
};

enum e_gps_location
{
	gpsSQLID,
	gpsLib[32],
	gpsName[32],
	Float:gpsPos[3]
};

enum e_library_gps
{
	libSQLID,
	libName[32]
}

#define MAX_GPS_LOCATION 20
#define MAX_GPS_LIBRARY 10

new LocationGPS[MAX_GPS_LOCATION][e_gps_location];
new LibraryGPS[MAX_GPS_LIBRARY][e_library_gps];

new Iterator:GPSLib<MAX_GPS_LIBRARY>;
new Iterator:GPSLocation<MAX_GPS_LOCATION>;

new PlayerGPSList[MAX_PLAYERS][30];
new PlayerGPSLocationList[MAX_PLAYERS][20];
new Float:PlayerGPSPos[MAX_PLAYERS][20][3];

OGMI:LoadGps()
{
	new Cache:result, query[280];
	mysql_format(sqlcon, query, sizeof query, "SELECT * FROM `gps_library`");
	result =  mysql_query(sqlcon, query);
	{
		for(new i; i < cache_num_rows(); i++)
		{
			LibraryGPS[i][libSQLID] = cache_get_field_content_int(i, "SQLID");
			cache_get_field_content(i, "name", LibraryGPS[i][libName]);
			Iter_Add(GPSLib, i);
		}
		printf("*** [GPS - Library] Loaded %i rows", cache_num_rows());
	}
	cache_delete(result);
	mysql_format(sqlcon, query, sizeof query, "SELECT * FROM `gps_location`");
	result = mysql_query(sqlcon, query);
	{
		for(new i; i < cache_num_rows(); i++)
		{
			cache_get_field_content(i, "name", LocationGPS[i][gpsName]);
			cache_get_field_content(i, "library", LocationGPS[i][gpsLib]);
			
			LocationGPS[i][gpsSQLID] = cache_get_field_content_int(i, "SQLID");

			LocationGPS[i][gpsPos][0] = cache_get_field_float(i, "posx");
			LocationGPS[i][gpsPos][1] = cache_get_field_float(i, "posy");
			LocationGPS[i][gpsPos][2] = cache_get_field_float(i, "posz");
			Iter_Add(GPSLocation, i);
		}
		printf("*** [GPS - Location] Loaded %i rows", cache_num_rows());
	}
	cache_delete(result);
	return 1;
}

SSCANF:GPSMenu(str[])
{
	if(!strcmp(str, "createlib", true)) return 1;
	else if(!strcmp(str, "createlocation", true)) return 2;
	else if(!strcmp(str, "deletelib", true)) return 3;
	else if(!strcmp(str, "deletelocation", true)) return 4;
	return 0;
}

CMD:gpsmenu(playerid, params[])
{
	if(!IsPlayerADuty(playerid, 3))
		return PermissionError(playerid);
	
	new action, str[128];
	if(sscanf(params, "k<GPSMenu>S()[123]", action, str))
	{
		SendSyntaxMessage(playerid, "/gpsmenu [createlib/createlocation/deletelib/deletelocation]");
		return 1;
	}
	switch(action)
	{
		case 1:
		{
			if(isnull(str)) return SendSyntaxMessage(playerid, "/gpsmenu createlib [names (32 letters)]");
			new id = Iter_Free(GPSLib), sqlfmt[300];

			mysql_format(sqlcon, sqlfmt, sizeof sqlfmt, "INSERT INTO `gps_library` SET `name` = '%s'", str);
			mysql_tquery(sqlcon, sqlfmt);

			format(LibraryGPS[id][libName],32, "%s", str);
			SendAdminMessage(X11_TOMATO, "%screate a GPS library with the name %s(%i)", ReturnName(playerid), str, id);
			Iter_Add(GPSLib, id);
		}
		case 2:
		{
			new libid, name[32];
			if(sscanf(str, "is[32]", libid, name)) return SendSyntaxMessage(playerid, "/gpsmenu createlocation [libid] [names (32 letters)]");
			if(!Iter_Contains(GPSLib, libid)) return SendErrorMessage(playerid, "There is no library ID.");
			new id = Iter_Free(GPSLocation);

			format(LocationGPS[id][gpsLib], 32, "%s", LibraryGPS[libid][libName]);
			format(LocationGPS[id][gpsName],32, "%s", name);

			GetPlayerPos(playerid, PositionXYZ(LocationGPS[id][gpsPos]));

			new Cache:result = mysql_query(sqlcon, sprintf("INSERT INTO `gps_location` SET `name` = '%s', `library` = '%s', `posx` = %f, `posy` = %f, `posz` = %f", name, LibraryGPS[libid][libName], PositionXYZ(LocationGPS[id][gpsPos])));
			LibraryGPS[id][libSQLID] = cache_insert_id();
			cache_delete(result);

			SendAdminMessage(X11_TOMATO, "%screate a GPS location with the name %s", ReturnName(playerid), name);
			Iter_Add(GPSLocation, id);
		}
		case 3:
		{
			new libid;
			if(sscanf(str, "i", libid)) return SendSyntaxMessage(playerid, "/gpsmenu deletelib [libid]");
			if(!Iter_Contains(GPSLib, libid)) return SendErrorMessage(playerid, "There is no library ID.");

			SendAdminMessage(X11_TOMATO, "%s delete a GPS Library %s(id: %i)", ReturnName(playerid), LibraryGPS[libid][libName], libid);
			mysql_tquery(sqlcon, sprintf("DELETE FROM `gps_library` WHERE `SQLID`= %i", LibraryGPS[libid][libSQLID]));
			foreach(new locid : GPSLocation) if(!strcmp(LibraryGPS[libid][libName], LocationGPS[locid][gpsLib], true))
			{
				LocationGPS[locid][gpsLib][0] = EOS;
				LocationGPS[locid][gpsName][0] = EOS;
				mysql_tquery(sqlcon, sprintf("DELETE FROM `gps_location` WHERE `library` = '%s'", LibraryGPS[libid][libName]));
				Iter_Remove(GPSLocation, locid);
			}
			format(LibraryGPS[libid][libName], 32, "");
			Iter_Remove(GPSLib, libid);
		}
		case 4:
		{
			new locid;
			if(sscanf(str, "i", locid)) return SendSyntaxMessage(playerid, "/gpsmenu deletelocation [locationid]");
			if(!Iter_Contains(GPSLocation, locid)) return SendErrorMessage(playerid, "There is no Location ID.");

			SendAdminMessage(X11_TOMATO, "%s menghapus lokasi %s(%i) dari perpustakaan %s", ReturnName(playerid), LocationGPS[locid][gpsName], locid, LocationGPS[locid][gpsLib]);
			format(LocationGPS[locid][gpsName], 32, "");
			format(LocationGPS[locid][gpsLib], 32, "");

			mysql_tquery(sqlcon, sprintf("DELETE FROM `gps_location` WHERE `SQLID`= %i", LocationGPS[locid][gpsSQLID]));
			Iter_Remove(GPSLocation, locid);
		}
	}
	return 1;
}

CMD:gps(playerid, params[])
{
	if(!PlayerHasItem(playerid, "GPS"))
		return SendErrorMessage(playerid, "Anda tidak memiliki GPS.");

	new fmt[400], count;
	foreach(new i : GPSLib)
	{
		format(fmt, sizeof fmt, "%s%s\n", fmt, LibraryGPS[i][libName]);
		PlayerGPSList[playerid][count++] = i;
	}
	format(fmt, sizeof fmt, "%s- My house\n", fmt);
	if(count) Dialog_Show(playerid, GpsMenu, DIALOG_STYLE_LIST, "GPS Menu", fmt, "Select", "Close");
	else SendServerMessage(playerid, "There is no GPS library.");
	return 1;
}

Dialog:GpsMenu(playerid, response, listitem, inputtext[])
{
	if(response)
	{
		new
			fmt[450],
			list_text[800],
			count = 0
		;
		PlayerData[playerid][pListitem] = listitem;
		new arr_HeaderGPS[][40] =
		{
			"#(ID)\tLocation\tDistance\n",
			"#(houseid)\tLocation\tDistance\n"
		};

		switch(HashStr(inputtext))
		{
			case HS<My house>:
			{
				SetPVarInt(playerid, "TypeGPS", GPS_TYPE_HOUSE);
				foreach(new house : House) if(House_PlayerOwn(playerid, house))
				{
					format(fmt, sizeof fmt, "%s%i\t%s\t%.2f\n", fmt, house, GetLocation(PositionXYZ(HouseData[house][houseExterior])), GetPlayerDistanceFromPoint(playerid, PositionXYZ(HouseData[house][houseExterior])));
					PlayerGPSPos[playerid][count][0] = HouseData[house][houseExterior][0];
					PlayerGPSPos[playerid][count][1] = HouseData[house][houseExterior][1];
					PlayerGPSPos[playerid][count][2] = HouseData[house][houseExterior][2];
					PlayerGPSLocationList[playerid][count++] = house;
				}
			}
		}

		foreach(new i : GPSLocation) 
		{
			if(!strcmp(LibraryGPS[listitem][libName], LocationGPS[i][gpsLib], false))
			{
				SetPVarInt(playerid, "TypeGPS", GPS_TYPE_LIBRARY);
				format(fmt, sizeof fmt, "%s%i(%i)\t%s\t%.2f\n", fmt, count, i, LocationGPS[i][gpsName], GetPlayerDistanceFromPoint(playerid, PositionXYZ(LocationGPS[i][gpsPos])));
				PlayerGPSLocationList[playerid][count++] = i;
			}
		}
		format(list_text, sizeof list_text, "%s%s", arr_HeaderGPS[GetPVarInt(playerid, "TypeGPS")], fmt);
		Dialog_Show(playerid, gps_select, DIALOG_STYLE_TABLIST_HEADERS, "GPS Location", list_text, "Select", "Close");
	}
	return 1;
}

Dialog:gps_select(playerid, response, listitem, inputtext[])
{
	if(response)
	{
		new 
			type = GetPVarInt(playerid, "TypeGPS"),
			gpsid = PlayerGPSLocationList[playerid][listitem]
		;
		PlayerData[playerid][pGPS] = true;
		switch(type)
		{
			case GPS_TYPE_LIBRARY: SetPlayerCheckpoint(playerid, PositionXYZ(LocationGPS[gpsid][gpsPos]), 3.5); // library
			case GPS_TYPE_HOUSE: SetPlayerCheckpoint(playerid, PositionXYZ(PlayerGPSPos[playerid][listitem]), 3.5); // rental
		}
	}
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	if(PlayerData[playerid][pGPS])
		DisablePlayerCheckpoint(playerid);

	return 1;
}