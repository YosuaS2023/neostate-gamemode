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
                    new salary = (PlayerData[playerid][SweeperDistance] * floatround(Data_GetInt("sweeper-salary-per-dist"))) + (PlayerData[playerid][SweeperTrash] * floatround(Data_GetFloat("sweeper-salary-per-trash")));
                    
                    SetVehicleToRespawn(vehicleid);
                    VehCore[vehicleid][vehFuel] = 100;

                    PlayerData[playerid][pSidejob] = SIDEJOB_NONE;
                    PlayerData[playerid][SweeperVehicle] = INVALID_VEHICLE_ID;

                    ResetSweeperInfo(playerid, true);
                    AddPlayerSalary(playerid, (salary-Data_GetInt("sweeper-salary-fail")));
                    SendCustomMessage(playerid, "SWEEPER", "anda tidak kembali ke kendaraan sweeper");
                    SendCustomMessage(playerid, "SWEEPER", "anda hanya mendapatkan salary sebanyak %s(%s-%s)", FormatNumber((salary-Data_GetInt("sweeper-salary-fail"))), FormatNumber(salary), FormatNumber(Data_GetInt("sweeper-salary-fail")));
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
                    new salary = (PlayerData[playerid][MowerGrass] * floatround(Data_GetInt("mower-salary-per-trash")));
                    
                    SetVehicleToRespawn(vehicleid);
                    VehCore[vehicleid][vehFuel] = 100;

                    PlayerData[playerid][pSidejob] = SIDEJOB_NONE;
                    PlayerData[playerid][MowerVehicle] = INVALID_VEHICLE_ID;

                    Mower_ResetData(playerid, true);
                    AddPlayerSalary(playerid, (salary-Data_GetInt("mower-salary-fail")));
                    SendCustomMessage(playerid, "MOWER", "anda tidak kembali ke kendaraan mower");
                    SendCustomMessage(playerid, "MOWER", "anda hanya mendapatkan salary sebanyak %s(%s-%s)", FormatNumber((salary-Data_GetInt("mower-salary-fail"))), FormatNumber(salary), FormatNumber(Data_GetInt("mower-salary-fail")));
                }
            }
        }
    }
    return 1;
}

CMD:startjob(playerid, params[])
{
    new actorid = Actor_Nearest(playerid, 2.5);
    yield 1;
    static dialog[dialog_data];
    switch(HashStr(ActorData[actorid][actorType]))
    {
        case HS<sidejob mower>:
        {
            await_arr(dialog) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_MSGBOX, "Start job mower", "Do you want to do a side job mower?\nClick \"yes\" If you want to do work, click \"no\" If you don't want to do work", "yes", "no");
            if(dialog[Response])
            {
                Mower_ResetData(playerid);
                
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
                Sweeper_TextDraw(playerid);
                PlayerData[playerid][pSidejob] = SIDEJOB_SWEEPER;
                PlayerData[playerid][SweeperVehicle] = INVALID_VEHICLE_ID;
                PlayerData[playerid][SweeperTrash] = 0;
                PlayerData[playerid][SweeperDistance] = 0;
                PlayerData[playerid][SweeperLastPos][0] = 0.0;

                SendCustomMessage(playerid, "SWEEPER", "anda memulai sidejob sweeper, silahkan naik kendaraan yang disedia.");
                SendCustomMessage(playerid, "HINT", "silahkan "GREEN"bersihkan"WHITE" jalan yang ada "YELLOW"sampah"WHITE".");
            }
            else return SendServerMessage(playerid, "anda membatalkan sidejob sweeper.");
        }
    }
    return 1;
}