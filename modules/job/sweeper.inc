#include <UtilsScripts\Als>
/* 
    sweeper init
*/
OGME:SweeperExit()
{
	foreach(new i : Player)
	{
	    if(!IsPlayerConnected(i)) continue;
	    if(PlayerData[i][pSidejob] == SIDEJOB_SWEEPER) ResetSweeperInfo(i, true);
	}
	
	return 1;
}
/*
	function general*
*/
ResetSweeperInfo(playerid, bool: removeTD = false, bool:removeVeh = false)
{
	new vehicleid = PlayerData[playerid][SweeperVehicle];

    PlayerData[playerid][SweeperUpdate] = 0;
	PlayerData[playerid][SweeperDistance] = 0;
	PlayerData[playerid][SweeperLastPos][0] = 0.0;
	PlayerData[playerid][SweeperVehicle] = INVALID_VEHICLE_ID;
	PlayerData[playerid][pSidejob] = SIDEJOB_NONE;
	PlayerData[playerid][pSidejobTime] = -1;
	DeletePVar(playerid, "is_trash_sweeper_full");
	if(removeTD)
	{
		PlayerTextDrawDestroy(playerid, PlayerData[playerid][pSidejobText]);
		//PlayerData[playerid][pSidejobText] = INVALID_TEXT_DRAW;
	}
	if(removeVeh)
	{
		Sidejob_VehicleRespawn(vehicleid);
	}
	return 1;
}

Function: Sweeper_ClearTrash(playerid, vehicleid)
{
	new trashid = Trash_Near(playerid, 1.0);
	if(trashid != -1)
	{
		if(!strcmp(TrashData[trashid][TRASH_TYPE], "sweeper"))
		{
			if(Trash_Clear(trashid))
			{
				VehCore[vehicleid][vehTrash]++;
				SendCustomMessage(playerid, "SWEEPER", "You clean the trash by vehicle. (total trash: %i)", VehCore[vehicleid][vehTrash]);
			}
		}
	}
	return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	ResetSweeperInfo(playerid, true, true);
	return ALS_CONTINUE;
}

ptask Sweeper[100](playerid)
{
    new vehicleid = PlayerData[playerid][SweeperVehicle];
	if(PlayerData[playerid][pSidejob] == SIDEJOB_SWEEPER && Vehicle_Type(vehicleid, "sweeper") && IsPlayerInVehicle(playerid, vehicleid))
	{
		if(VehCore[vehicleid][vehTrash] >= 30 && !GetPVarInt(playerid, "is_trash_sweeper_full"))
		{
			new Float:x, Float:y, Float:z;
			Data_GetPosition("sweeper", "destroy-trash-pos", x, y, z);
			VehCore[vehicleid][vehTrash] = 30;
			PlayerData[playerid][pGPS] = true;
			SetPlayerCheckpoint(playerid, x, y, z, 7.0);
			SendCustomMessage(playerid, "SWEEPER", "anda telah mengumpulkan "YELLOW"sampah"WHITE" sebanyak "GREEN"30"WHITE", silahkan kumpulkan sampah dan dapatkan salary anda!");
			SetPVarInt(playerid, "is_trash_sweeper_full", 1);
		}

		if(PlayerData[playerid][SweeperLastPos][0] == 0.0) GetPlayerPos(playerid, PlayerData[playerid][SweeperLastPos][0], PlayerData[playerid][SweeperLastPos][1], PlayerData[playerid][SweeperLastPos][2]);
		PlayerData[playerid][SweeperDistance] += floatround(GetPlayerDistanceFromPoint(playerid, PlayerData[playerid][SweeperLastPos][0], PlayerData[playerid][SweeperLastPos][1], PlayerData[playerid][SweeperLastPos][2]));
		GetPlayerPos(playerid, PlayerData[playerid][SweeperLastPos][0], PlayerData[playerid][SweeperLastPos][1], PlayerData[playerid][SweeperLastPos][2]);
		Sweeper_ClearTrash(playerid, vehicleid);
		new string[120];
		format(string, sizeof(string), "~b~~h~Sweeper Job~n~~n~~w~Cleaned: ~y~%d Meters~n~~w~Trash: ~y~%d", PlayerData[playerid][SweeperDistance], VehCore[vehicleid][vehTrash]);
		PlayerTextDrawSetString(playerid, PlayerData[playerid][pSidejobText], string);
	}
	return 1;
}