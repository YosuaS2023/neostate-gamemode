#include <UtilsScripts\Als>
/* 
    sweeper defines
*/
#define     UPDATE_TIME         (500)   // in milliseconds

stock ResetSweeperInfo(playerid, bool: removeTD = false)
{
    PlayerData[playerid][SweeperUpdate] = 0;
	PlayerData[playerid][SweeperTrash] = 0;
	PlayerData[playerid][SweeperDistance] = 0;
	PlayerData[playerid][SweeperLastPos][0] = 0.0;
	PlayerData[playerid][SweeperVehicle] = INVALID_VEHICLE_ID;
	PlayerData[playerid][pSidejob] = SIDEJOB_NONE;
	PlayerData[playerid][pSidejobTime] = -1;
	if(removeTD) PlayerTextDrawDestroy(playerid, PlayerData[playerid][SweeperText]);
	return 1;
}

Function:Sweeper_TextDraw(playerid)
{
	PlayerData[playerid][SweeperText] = CreatePlayerTextDraw(playerid, 40.000000, 305.000000, "~b~~h~Sweeper Job~n~~n~~w~Cleaned: ~y~0 Meters");
    PlayerTextDrawBackgroundColor(playerid, PlayerData[playerid][SweeperText], 255);
	PlayerTextDrawFont(playerid, PlayerData[playerid][SweeperText], 1);
	PlayerTextDrawLetterSize(playerid, PlayerData[playerid][SweeperText], 0.240000, 1.100000);
	PlayerTextDrawColor(playerid, PlayerData[playerid][SweeperText], -1);
	PlayerTextDrawSetOutline(playerid, PlayerData[playerid][SweeperText], 1);
	PlayerTextDrawSetProportional(playerid, PlayerData[playerid][SweeperText], 1);
	PlayerTextDrawSetSelectable(playerid, PlayerData[playerid][SweeperText], 0);
	PlayerTextDrawShow(playerid, PlayerData[playerid][SweeperText]);
}

Function: Sweeper_ClearTrash(playerid)
{
	new trashid = Trash_Near(playerid, 2.5);
	if(trashid != -1)
	{
		if(!strcmp(TrashData[trashid][TRASH_TYPE], "sweeper"))
		{
			if(Trash_Clear(trashid))
			{
				PlayerData[playerid][SweeperTrash]++;
				SendCustomMessage(playerid, "SWEEPER", "You clean the trash by vehicle. (total trash: %i)", PlayerData[playerid][SweeperTrash]);
			}
		}
	}
	return 1;
}

OGME:SweeperExit()
{
	foreach(new i : Player)
	{
	    if(!IsPlayerConnected(i)) continue;
	    if(PlayerData[i][pSidejob] == SIDEJOB_SWEEPER) ResetSweeperInfo(i, true);
	}
	
	return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	ResetSweeperInfo(playerid, true);
	return ALS_CONTINUE;
}

hook OnPlayerUpdate(playerid)
{
    new vehicleid = GetPlayerVehicleID(playerid);
	if(PlayerData[playerid][pSidejob] == SIDEJOB_SWEEPER && Vehicle_Type(vehicleid, "sweeper") && PlayerData[playerid][SweeperUpdate] < tickcount())
	{
		if(PlayerData[playerid][SweeperLastPos][0] == 0.0) GetPlayerPos(playerid, PlayerData[playerid][SweeperLastPos][0], PlayerData[playerid][SweeperLastPos][1], PlayerData[playerid][SweeperLastPos][2]);
		
		PlayerData[playerid][pSidejobTime] = 60;
		PlayerData[playerid][SweeperUpdate] = tickcount()+Data_GetInt("sweeper-update-time");
		PlayerData[playerid][SweeperDistance] += floatround(GetPlayerDistanceFromPoint(playerid, PlayerData[playerid][SweeperLastPos][0], PlayerData[playerid][SweeperLastPos][1], PlayerData[playerid][SweeperLastPos][2]));
		GetPlayerPos(playerid, PlayerData[playerid][SweeperLastPos][0], PlayerData[playerid][SweeperLastPos][1], PlayerData[playerid][SweeperLastPos][2]);
		
		Sweeper_ClearTrash(playerid);
		new string[120];
		format(string, sizeof(string), "~b~~h~Sweeper Job~n~~n~~w~Cleaned: ~y~%d Meters~n~~w~Trash: ~y~%d", PlayerData[playerid][SweeperDistance], PlayerData[playerid][SweeperTrash]);
		PlayerTextDrawSetString(playerid, PlayerData[playerid][SweeperText], string);
	}
	return 0;
}