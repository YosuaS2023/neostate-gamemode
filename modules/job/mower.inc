#include <UtilsScripts\Als>
/* 
    mower defines
*/

stock Mower_ResetData(playerid, bool:removeTD = false, bool:removeVeh = false)
{
	new vehicleid;
    PlayerData[playerid][MowerUpdate] = 0;
	PlayerData[playerid][MowerGrass] = 0;
	PlayerData[playerid][MowerVehicle] = INVALID_VEHICLE_ID;

	PlayerData[playerid][pSidejob] = SIDEJOB_NONE;
	PlayerData[playerid][pSidejobTime] = -1;
	if(removeTD) PlayerTextDrawDestroy(playerid, PlayerData[playerid][pSidejobText]);
	if(removeVeh && (vehicleid = PlayerData[playerid][MowerVehicle]) != INVALID_VEHICLE_ID)
	{
	}
	return 1;
}

Function: Mower_ClearTrash(playerid)
{
	new trashid = Trash_Near(playerid, 2.0);
	if(trashid != -1)
	{
		if(!strcmp(TrashData[trashid][TRASH_TYPE], "mower"))
		{
			if(Trash_Clear(trashid))
			{
				PlayerData[playerid][MowerGrass]++;
				SendCustomMessage(playerid, "MOWER", "You pull out as much grass as %i", PlayerData[playerid][MowerGrass]);
			}
		}
	}
	return 1;
}

OGME:Mower()
{
	foreach(new i : Player)
	{
	    if(!IsPlayerConnected(i)) continue;
	    if(PlayerData[i][pSidejob] == SIDEJOB_MOWER) Mower_ResetData(i, true);
	}
	
	return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	Mower_ResetData(playerid, true);
	return ALS_CONTINUE;
}

hook OnPlayerUpdate(playerid)
{
    new vehicleid = GetPlayerVehicleID(playerid);
	if(PlayerData[playerid][pSidejob] == SIDEJOB_MOWER && Vehicle_Type(vehicleid, "mower") && PlayerData[playerid][MowerUpdate] < tickcount())
	{	
		PlayerData[playerid][pSidejobTime] = 60;
		PlayerData[playerid][MowerUpdate] = tickcount()+Data_GetInt("mower", "update-time");
		
		Mower_ClearTrash(playerid);
		new string[120];
		format(string, sizeof(string), "~b~~h~Mower Job~n~~n~~w~Grass: ~y~%d", PlayerData[playerid][MowerGrass]);
		PlayerTextDrawSetString(playerid, PlayerData[playerid][pSidejobText], string);
	}
	return 0;
}