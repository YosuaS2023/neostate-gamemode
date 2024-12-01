ptask EnergyUpdate[30000](playerid)
{
	if(PlayerData[playerid][pEnergy] > 0)
	{
	    PlayerData[playerid][pEnergy]--;
	}
	return 1;
}

task RentalUpdate[1000]()
{
	forex(i, MAX_PLAYER_VEHICLE) if(VehicleData[i][vExists] && VehicleData[i][vRental] != -1)
	{
	    if(VehicleData[i][vRentTime] > 0)
	    {
	        VehicleData[i][vRentTime]--;
	        if(VehicleData[i][vRentTime] <= 0)
	        {
	            foreach(new playerid : Player) if(VehicleData[i][vOwner] == PlayerData[playerid][pID])
	            {
	            	SendClientMessageEx(playerid, COLOR_SERVER, "RENTAL: {FFFFFF}Masa rental kendaraan %s telah habis, kendaraan otomatis dihilangkan.", GetVehicleModelName(VehicleData[i][vModel]));
				}
				Vehicle_Delete(i);
			}
		}
	}
}

task VehicleUpdate[4000]()
{
	forex(i, MAX_VEHICLES) if (IsEngineVehicle(i) && GetEngineStatus(i))
	{
	    if (GetFuel(i) > 0)
	    {
	        VehCore[i][vehFuel]--;
			if (GetFuel(i) <= 0)
			{
			    VehCore[i][vehFuel] = 0;
	      		SwitchVehicleEngine(i, false);
	      		GameTextForPlayer(GetVehicleDriver(i), "Vehicle out of ~r~Fuel!", 3000, 5);
			}
		}
	}
	forex(i, MAX_PLAYER_VEHICLE) if(VehicleData[i][vExists])
	{
		if(VehicleData[i][vInsuTime] != 0 && VehicleData[i][vInsuTime] <= gettime())
		{
			VehicleData[i][vInsuTime] = 0;
		}
	}
	return 1;
}
ptask PlayerUpdate[1000](playerid)
{
	if(PlayerData[playerid][pSpawned])
	{
		SetPlayerProgressBarValue(playerid, ENERGYBAR[playerid], PlayerData[playerid][pEnergy]);
		SetPlayerProgressBarColour(playerid, ENERGYBAR[playerid], ConvertHBEColor(PlayerData[playerid][pEnergy]));
		new vehicleid = GetPlayerVehicleID(playerid);
		
		if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		{
		    if(IsSpeedoVehicle(vehicleid))
		    {
		        new Float:vHP, vehname[64], speedtd[64], healthtd[64];
		        GetVehicleHealth(vehicleid, vHP);
		        format(healthtd, sizeof(healthtd), "%.1f", vHP);
		        PlayerTextDrawSetString(playerid, HEALTHTD[playerid], healthtd);

		        format(vehname, sizeof(vehname), "%s", GetVehicleName(vehicleid));
		        PlayerTextDrawSetString(playerid, VEHNAMETD[playerid], vehname);
		        
		        format(speedtd, sizeof(speedtd), "%iKM/H", GetVehicleSpeedKMH(vehicleid));
		        PlayerTextDrawSetString(playerid, KMHTD[playerid], speedtd);
		        
		        SetPlayerProgressBarValue(playerid, FUELBAR[playerid], VehCore[vehicleid][vehFuel]);
			}
		}
	}
	return 1;
}

ptask OnlinePerSec[1000](playerid)
{
    if(PlayerData[playerid][pSpawned])
    {
        PlayerData[playerid][pOnline]++;
		if(PlayerData[playerid][pSalaryClaim] > 0)
		{
			if(gettime() >= PlayerData[playerid][pSalaryClaim])
			{
				SendCustomMessage(playerid, "SALARY", "anda dapat mengambil salary anda sekarang.");
				PlayerData[playerid][pSalaryClaim] = 0;
			}
		}
		if(PlayerData[playerid][pIDCardTime] > 0)
		{
			if(gettime() >= PlayerData[playerid][pIDCardTime])
			{
				SendCustomMessage(playerid, "IDCARD", ""YELLOW"ID Card"WHITE" anda telah melewati "RED"batas waktu"WHITE);
				SendCustomMessage(playerid, "IDCARD", "anda dapat memperpanjang waktunya dengan ke "GREEN"cityhall");
				PlayerData[playerid][pIDCardTime] = 0;
			}
		}
		if(PlayerData[playerid][pHouseLicense] > 0)
		{
			if(gettime() >= PlayerData[playerid][pHouseLicense])
			{
				SendCustomMessage(playerid, "LICENSE", ""YELLOW"Lisensi rumah"WHITE" anda telah melewati "RED"batas waktu"WHITE);
				SendCustomMessage(playerid, "LICENSE", "anda dapat memperpanjang waktunya dengan ke "GREEN"cityhall");
				PlayerData[playerid][pHouseLicense] = 0;
			}
		}
		if(PlayerData[playerid][pBizLicense] > 0)
		{
			if(gettime() >= PlayerData[playerid][pBizLicense])
			{
				SendCustomMessage(playerid, "LICENSE", ""YELLOW"Lisensi bisnis"WHITE" anda telah melewati "RED"batas waktu"WHITE);
				SendCustomMessage(playerid, "LICENSE", "anda dapat memperpanjang waktunya dengan ke "GREEN"cityhall");
				PlayerData[playerid][pBizLicense] = 0;
			}
		}
    }
    return 1;
}

task OneSecondTime[1000]()
{
	foreach(new plant_id : PlantIds) if(!IsDynamicObjectMoving(PlantData[plant_id][plantObject]) && PlantData[plant_id][plantPos][3] != PlantData[plant_id][plantPos][2])
	{
		PlantData[plant_id][plantPos][3] = PlantData[plant_id][plantPos][2];
		SendDebugMessage("plant id "GREEN"%i"WHITE" growth", plant_id);
	}
	return 1;
}