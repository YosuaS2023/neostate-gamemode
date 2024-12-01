#include <UtilsScripts\Als>
/* 
    sweeper defines
*/
#define     UPDATE_TIME         (500)   // in milliseconds

new
	bool: SweeperJob[MAX_PLAYERS] = {false, ...},
	SweeperUpdate[MAX_PLAYERS] = {0, ...},
	SweeperDistance[MAX_PLAYERS] = {0, ...},
	SweeperTrash[MAX_PLAYERS],
	Float: SweeperLastPos[MAX_PLAYERS][3],
	PlayerText: SweeperText[MAX_PLAYERS];

stock ResetSweeperInfo(playerid, bool: removeTD = false)
{
    SweeperJob[playerid] = false;
    SweeperUpdate[playerid] = 0;
	SweeperTrash[playerid] = 0;
	SweeperDistance[playerid] = 0;
	if(removeTD) PlayerTextDrawDestroy(playerid, SweeperText[playerid]);
	return 1;
}

Function: Sweeper_ClearTrash(playerid)
{
	new trashid = Trash_Near(playerid, 1.0);
	if(trashid != -1)
	{
		if(!strcmp(TrashData[trashid][TRASH_TYPE], "sweeper"))
		{
			if(Trash_Clear(trashid))
			{
				SweeperTrash[playerid]++;
				SendCustomMesaage(playerid, "SWEEPER", "You clean the trash by vehicle. (total trash: %i)", SweeperTrash[playerid]);
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
	    if(SweeperJob[i]) ResetSweeperInfo(i, true);
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
	if(SweeperJob[playerid] && VehCore[vehicleid][vehType] == VEHICLE_TYPE_SWEEPER && SweeperUpdate[playerid] < tickcount())
	{
		SweeperUpdate[playerid] = tickcount()+Data_GetInt("sweeper-update-time");
		SweeperDistance[playerid] += floatround(GetPlayerDistanceFromPoint(playerid, SweeperLastPos[playerid][0], SweeperLastPos[playerid][1], SweeperLastPos[playerid][2]));
		GetPlayerPos(playerid, SweeperLastPos[playerid][0], SweeperLastPos[playerid][1], SweeperLastPos[playerid][2]);
		
		Sweeper_ClearTrash(playerid);
		new string[64];
		format(string, sizeof(string), "~b~~h~Sweeper Job~n~~n~~w~Cleaned: ~y~%d Meters~n~~w~Trash: ~y~%d", SweeperDistance[playerid], SweeperTrash[playerid]);
		PlayerTextDrawSetString(playerid, SweeperText[playerid], string);
	}
	return 1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate)
{
    new vid = GetPlayerVehicleID(playerid);
	if(newstate == PLAYER_STATE_DRIVER && VehCore[vid][vehType] == VEHICLE_TYPE_SWEEPER)
    {
        static dialog_confirm[dialog_data];
        await_arr(dialog_confirm) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_MSGBOX, "Start sweeper job", "Do you want to start sweeper job?\nClick 'Yes' if you want to start and Click 'No' if you don't want to start.", "Yes", "No");
	    
        if(dialog_confirm[Response])
        {
            SweeperText[playerid] = CreatePlayerTextDraw(playerid, 40.000000, 305.000000, "~b~~h~Sweeper Job~n~~n~~w~Cleaned: ~y~0 Meters");
            PlayerTextDrawBackgroundColor(playerid, SweeperText[playerid], 255);
            PlayerTextDrawFont(playerid, SweeperText[playerid], 1);
            PlayerTextDrawLetterSize(playerid, SweeperText[playerid], 0.240000, 1.100000);
            PlayerTextDrawColor(playerid, SweeperText[playerid], -1);
            PlayerTextDrawSetOutline(playerid, SweeperText[playerid], 1);
            PlayerTextDrawSetProportional(playerid, SweeperText[playerid], 1);
            PlayerTextDrawSetSelectable(playerid, SweeperText[playerid], 0);
            PlayerTextDrawShow(playerid, SweeperText[playerid]);
            
            SweeperDistance[playerid] = 0;
			SweeperTrash[playerid] = 0;
            GetPlayerPos(playerid, SweeperLastPos[playerid][0], SweeperLastPos[playerid][1], SweeperLastPos[playerid][2]);
            SweeperJob[playerid] = true;
            SendCustomMessage(playerid, "SWEEPER", "Drive around and clean the streets!");
            SendCustomMessage(playerid, "SWEEPER", "You will get your money when you leave your Sweeper.");
        } else SendServerMessage(playerid, "You cancel to start job sweeper.");
    }
	if(oldstate == PLAYER_STATE_DRIVER && SweeperJob[playerid])
	{
	    new salary_dist = floatround(SweeperDistance[playerid] * Data_GetFloat("sweeper-salary-per-dist")), salary_trash = floatround(SweeperTrash[playerid] * Data_GetFloat("sweeper-salary-per-trash")), string[80];
		new salary_total = salary_dist + salary_trash;
	    format(string, sizeof(string), "~n~~n~~w~Distance Cleaned: ~b~~h~~h~%d Meters~n~~w~Earned ~g~~h~~h~$%d", SweeperDistance[playerid], money);
	    GameTextForPlayer(playerid, string, 3000, 3);
	    AddPlayerSalary(playerid, salary_total);
	    ResetSweeperInfo(playerid, true);
	}
	
	return ALS_CONTINUE;
}