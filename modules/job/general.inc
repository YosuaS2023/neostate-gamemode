Sidejob_TextDraw(playerid, text[])
{
	PlayerData[playerid][pSidejobText] = CreatePlayerTextDraw(playerid, 40.000000, 305.000000, text);
    PlayerTextDrawBackgroundColor(playerid, PlayerData[playerid][pSidejobText], 255);
	PlayerTextDrawFont(playerid, PlayerData[playerid][pSidejobText], 1);
	PlayerTextDrawLetterSize(playerid, PlayerData[playerid][pSidejobText], 0.240000, 1.100000);
	PlayerTextDrawColor(playerid, PlayerData[playerid][pSidejobText], -1);
	PlayerTextDrawSetOutline(playerid, PlayerData[playerid][pSidejobText], 1);
	PlayerTextDrawSetProportional(playerid, PlayerData[playerid][pSidejobText], 1);
	PlayerTextDrawSetSelectable(playerid, PlayerData[playerid][pSidejobText], 0);
	PlayerTextDrawShow(playerid, PlayerData[playerid][pSidejobText]);
}

Function: IsValidVehicleSidejob(vehicleid)
{
    if(IsVehicleServer(vehicleid))
    {
        switch(HashStr(VehCore[vehicleid][vehType]))
        {
            case HS<sweeper>: return 1;
            case HS<mower>: return 1;
        }
    }
    return 1;
}

Function: Sidejob_VehicleRespawn(vehicleid)
{
    if(vehicleid != INVALID_VEHICLE_ID && IsValidVehicle(vehicleid) && IsValidVehicleSidejob(vehicleid))
    {
        SetVehicleToRespawn(vehicleid);
        VehCore[vehicleid][vehTrash] = 0;
        VehCore[vehicleid][vehFuel] = 100;
        SetVehicleHealth(vehicleid, 1000.0);
        return 1;
    }
    return 0;
}

stock GetSidejobName(sidejobid, name[], length = sizeof(name))
{
    if(sidejobid == SIDEJOB_NONE) return 0;
    switch(sidejobid)
    {
        case SIDEJOB_SWEEPER: format(name, length, "sweeper");
        case SIDEJOB_MOWER: format(name, length, "mower");
    }
    return 1;
}

ptask SidejobGeneral[1000](playerid)
{
    switch(PlayerData[playerid][pSidejob])
    {
        case SIDEJOB_SWEEPER:
        {     
            if(PlayerData[playerid][SweeperVehicle] != INVALID_VEHICLE_ID && !IsPlayerInVehicle(playerid, PlayerData[playerid][SweeperVehicle]) && PlayerData[playerid][pSidejobTime] != -1)
            {
                PlayerData[playerid][pSidejobTime]--;
                GameTextForPlayer(playerid, sprintf("Return to Vehicle~n~in ~y~%i seconds", PlayerData[playerid][pSidejobTime]), 1_000, 6);
                if(PlayerData[playerid][pSidejobTime] <= 0)
                {
                    new vehicleid = PlayerData[playerid][SweeperVehicle];
                    new salary = floatround(PlayerData[playerid][SweeperDistance] * Data_GetFloat("sweeper", "salary-per-dist")) + floatround(Data_GetFloat("sweeper", "salary-per-trash") * VehCore[vehicleid][vehTrash]);

                    ResetSweeperInfo(playerid, true, true);
                    AddPlayerSalary(playerid, floatround(salary / Data_GetFloat("sweeper", "salary-fail")));
                    SendCustomMessage(playerid, "SWEEPER", "anda tidak kembali ke kendaraan sweeper");
                    SendCustomMessage(playerid, "SWEEPER", "anda hanya mendapatkan salary sebanyak "GREEN"%s", FormatNumber(floatround(salary / Data_GetFloat("sweeper", "salary-fail"))));
                }
            }
        }
        case SIDEJOB_MOWER:
        {
            if(PlayerData[playerid][MowerVehicle] != INVALID_VEHICLE_ID && !IsPlayerInVehicle(playerid, PlayerData[playerid][MowerVehicle]) && PlayerData[playerid][pSidejobTime] != -1)
            {
                PlayerData[playerid][pSidejobTime]--;
                GameTextForPlayer(playerid, sprintf("Return to Vehicle~n~in ~y~%i seconds", PlayerData[playerid][pSidejobTime]), 1_000, 6);
                if(PlayerData[playerid][pSidejobTime] <= 0)
                {
                    new vehicleid = PlayerData[playerid][MowerVehicle];
                    new salary = floatround(PlayerData[playerid][MowerGrass] * Data_GetFloat("mower", "salary-per-grass"));
                    
                    SetVehicleToRespawn(vehicleid);
                    VehCore[vehicleid][vehFuel] = 100;

                    PlayerData[playerid][pSidejob] = SIDEJOB_NONE;
                    PlayerData[playerid][MowerVehicle] = INVALID_VEHICLE_ID;

                    Mower_ResetData(playerid, true);
                    AddPlayerSalary(playerid, floatround(salary / Data_GetFloat("mower", "salary-fail")));
                    SendCustomMessage(playerid, "MOWER", "anda tidak kembali ke kendaraan mower");
                    SendCustomMessage(playerid, "MOWER", "anda hanya mendapatkan salary sebanyak "GREEN"%s", FormatNumber(floatround(salary / Data_GetFloat("mower", "salary-fail"))));
                }
            }
        }
    }
    return 1;
}

CMD:startjob(playerid, params[])
{
    yield 1;
    new sidejob_name[32],
        actorid = Actor_Nearest(playerid, 2.5),
        dialog[dialog_data]
    ;

    GetSidejobName(PlayerData[playerid][pSidejob], sidejob_name);
    if(PlayerData[playerid][pSidejob] != SIDEJOB_NONE) 
        return SendErrorMessage(playerid, "You haven't finished work \"%s\"", sidejob_name);
        
    switch(HashStr(ActorData[actorid][actorType]))
    {
        case HS<sidejob mower>:
        {
            await_arr(dialog) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_MSGBOX, "Start job mower", "Do you want to do a side job mower?\nClick \"yes\" If you want to do work, click \"no\" If you don't want to do work", "yes", "no");
            if(dialog[Response])
            {
                Sidejob_TextDraw(playerid, "~b~~h~Mower Job~n~~n~~w~Grass: ~y~0");
                
                PlayerData[playerid][pSidejob] = SIDEJOB_MOWER;
                PlayerData[playerid][MowerVehicle] = INVALID_VEHICLE_ID;
                PlayerData[playerid][MowerGrass] = 0;

                SendCustomMessage(playerid, "MOWER", "anda memulai sidejob mower, silahkan naik kendaraan yang disedia.");
                SendCustomMessage(playerid, "HINT", "silahkan anda bersihkan rumput yang terlihat di sekitar");
            }
            else return SendServerMessage(playerid, "anda membatalkan sidejob mower.");
        }
        case HS<sidejob sweeper>:
        {
            await_arr(dialog) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_MSGBOX, "Start job sweeper", "Do you want to do a side job sweeper?\nClick \"yes\" If you want to do work, click \"no\" If you don't want to do work", "yes", "no");
            if(dialog[Response])
            {
                Sidejob_TextDraw(playerid, "~b~~h~Sweeper Job~n~~n~~w~Cleaned: ~y~0 Meters~n~~w~Trash: ~y~0");
                PlayerData[playerid][pSidejob] = SIDEJOB_SWEEPER;
                PlayerData[playerid][SweeperVehicle] = INVALID_VEHICLE_ID;
                PlayerData[playerid][SweeperDistance] = 0;
                PlayerData[playerid][SweeperLastPos][0] = 0.0;

                SendCustomMessage(playerid, "SWEEPER", "anda memulai sidejob sweeper, silahkan naik kendaraan yang disedia.");
                SendCustomMessage(playerid, "HINT", "anda dapat mengumpulkan "RED"sampah" WHITE" hingga 30 sampah lalu mengumpulkan nya untuk mendapatkan "GREEN"salary"WHITE".");
                SendCustomMessage(playerid, "HINT", "anda bisa mengunakan perintah \"/finishjob\" untuk berhenti dan mengumpulkan sampah.");
                SendCustomMessage(playerid, "NOTE", "saat anda ingin mengakhiri pekerjaan, maka " YELLOW"sampah tidak dapat diambil lagi!");
            }
            else return SendServerMessage(playerid, "anda membatalkan sidejob sweeper.");
        }
    }
    return 1;
}

CMD:finishjob(playerid, params[])
{
    switch(PlayerData[playerid][pSidejob])
    {
        case SIDEJOB_SWEEPER:
        {
            new Float:x, Float:y, Float:z;
            new ret = Data_GetPosition("sweeper", "destroy-trash-pos", x, y, z);
            if(!GetPVarInt(playerid, "is_trash_sweeper_full"))
            {
                SendCustomMessage(playerid, "SWEEPER", "silahkan pergi ke " RED"checkpoint"WHITE" untuk mengumpulkan sampah dan mendapatkan salary!");
                PlayerData[playerid][pGPS] = true;
                SetPlayerCheckpoint(playerid, x, y, z, 5.0);
                SetPVarInt(playerid, "is_trash_sweeper_full", 1);
                return 1;
            }
            if(ret && IsPlayerInRangeOfPoint(playerid, 5.0, x, y, z) && IsPlayerInVehicle(playerid, PlayerData[playerid][SweeperVehicle]))
            {
                new vehicle_job = PlayerData[playerid][SweeperVehicle];
                if(VehCore[vehicle_job][vehTrash] <= 0)
                {
                    return SendErrorMessage(playerid, "kendaraan sweeper anda tidak memikiki sampah didalamnya!");
                }
                StartPlayerLoadingBar(playerid, 5, "destroy trash", 1000, "OnPlayerFinishSidejob");
            }
        }
    }
    return 1;
}

forward OnPlayerFinishSidejob(playerid);
public OnPlayerFinishSidejob(playerid)
{
    switch(PlayerData[playerid][pSidejob])
    {
        case SIDEJOB_SWEEPER:
        {
            new vehicleid = PlayerData[playerid][SweeperVehicle];
            new trash = VehCore[vehicleid][vehTrash];
            new salary = floatround(PlayerData[playerid][SweeperDistance] * Data_GetFloat("sweeper", "salary-per-dist")) + floatround(Data_GetFloat("sweeper", "salary-per-trash") * VehCore[vehicleid][vehTrash]);
        
            ResetSweeperInfo(playerid, true, true);
            AddPlayerSalary(playerid, floatround(salary));
            SendCustomMessage(playerid, "SWEEPER", "anda mengumpulkan sampah sebanyak %i dan mendaptakn salary "GREEN"%s", trash, FormatNumber(floatround(salary)));
        }
    }
}