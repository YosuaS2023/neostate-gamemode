forward Pump_Load(bizid);
public Pump_Load(bizid)
{
	static
	    rows,
		id = -1;

	rows = cache_num_rows();

	for (new i = 0; i < rows; i ++) if ((id = Iter_Free(Pump)) != -1)
	{
	    PumpData[id][pumpExists] = true;
	    PumpData[id][pumpBusiness] = bizid;
        
	    PumpData[id][pumpID] = cache_get_field_int(i, "pumpID");
	    PumpData[id][pumpPos][0] = cache_get_field_float(i, "pumpPosX");
	    PumpData[id][pumpPos][1] = cache_get_field_float(i, "pumpPosY");
	    PumpData[id][pumpPos][2] = cache_get_field_float(i, "pumpPosZ");
	    PumpData[id][pumpPos][3] = cache_get_field_float(i, "pumpPosA");
	    PumpData[id][pumpFuel] = cache_get_field_int(i, "pumpFuel");

	    PumpData[id][pumpObject] = CreateDynamicObject(1676, PumpData[id][pumpPos][0], PumpData[id][pumpPos][1], PumpData[id][pumpPos][2], 0.0, 0.0, PumpData[id][pumpPos][3]);
	    Pump_Refresh(id);
        Iter_Add(Pump, id);
	}
	return 1;
}

Pump_Delete(pumpid)
{
	if (pumpid != -1 && PumpData[pumpid][pumpExists])
	{
	    new
	        string[90];

		format(string, sizeof(string), "DELETE FROM `fuel_station` WHERE `ID` = '%d' AND `pumpID` = '%d'", BizData[PumpData[pumpid][pumpBusiness]][bizID], PumpData[pumpid][pumpID]);
		mysql_tquery(sqlcon, string);

        if (IsValidDynamic3DTextLabel(PumpData[pumpid][pumpText3D]))
		    DestroyDynamic3DTextLabel(PumpData[pumpid][pumpText3D]);

		if (IsValidDynamicObject(PumpData[pumpid][pumpObject]))
		    DestroyDynamicObject(PumpData[pumpid][pumpObject]);

		foreach (new i : Player) if (PlayerData[i][pGasPump] == pumpid) {
		    StopRefilling(i);
		}
	    PumpData[pumpid][pumpExists] = false;
	    PumpData[pumpid][pumpFuel] = 0;
		Iter_Remove(Pump, pumpid);
	}
	return 1;
}

Pump_Create(playerid, bizid)
{
    static
	    Float:x,
	    Float:y,
	    Float:z,
		Float:angle,
		string[64],
		id = -1;

	if (GetPlayerPos(playerid, x, y, z) && GetPlayerFacingAngle(playerid, angle))
	{
		if ((id = Iter_Free(Pump)) != -1)
  		{
	        x += 5.0 * floatsin(-angle, degrees);
	        y += 5.0 * floatcos(-angle, degrees);

			PumpData[id][pumpExists] = true;
			PumpData[id][pumpBusiness] = bizid;
			PumpData[id][pumpPos][0] = x;
			PumpData[id][pumpPos][1] = y;
			PumpData[id][pumpPos][2] = z;
			PumpData[id][pumpPos][3] = angle;
            PumpData[id][pumpFuel] = 2000;
			PumpData[id][pumpObject] = CreateDynamicObject(1676, x, y, z, 0.0, 0.0, angle);

			format(string, sizeof(string), "INSERT INTO `fuel_station` (`ID`) VALUES(%d)", BizData[bizid][bizID]);
			mysql_tquery(sqlcon, string, "OnPumpCreated", "d", id);
			Iter_Add(Pump, id);
			return id;
		}
	}
	return -1;
}

Pump_Refresh(pumpid)
{
	if (pumpid != -1 && PumpData[pumpid][pumpExists])
	{
	    static
	        string[128];

		format(string, sizeof(string), "[Fuel Station %i]\n{FFFFFF}Fuel Left: %d liters", pumpid, PumpData[pumpid][pumpFuel]);

        if (IsValidDynamic3DTextLabel(PumpData[pumpid][pumpText3D]))
            DestroyDynamic3DTextLabel(PumpData[pumpid][pumpText3D]);

		if (IsValidDynamicObject(PumpData[pumpid][pumpObject]))
		    DestroyDynamicObject(PumpData[pumpid][pumpObject]);

		PumpData[pumpid][pumpText3D] = CreateDynamic3DTextLabel(string, COLOR_DARKBLUE, PumpData[pumpid][pumpPos][0], PumpData[pumpid][pumpPos][1], PumpData[pumpid][pumpPos][2], 15.0);
        PumpData[pumpid][pumpObject] = CreateDynamicObject(1676, PumpData[pumpid][pumpPos][0], PumpData[pumpid][pumpPos][1], PumpData[pumpid][pumpPos][2], 0.0, 0.0, PumpData[pumpid][pumpPos][3]);
	}
	return 1;
}

Pump_Save(pumpid)
{
	static
	    query[256];

	format(query, sizeof(query), "UPDATE `fuel_station` SET `pumpPosX` = '%.4f', `pumpPosY` = '%.4f', `pumpPosZ` = '%.4f', `pumpPosA` = '%.4f', `pumpFuel` = '%d' WHERE `ID` = '%d' AND `pumpID` = '%d'",
	    PumpData[pumpid][pumpPos][0],
	    PumpData[pumpid][pumpPos][1],
	    PumpData[pumpid][pumpPos][2],
		PumpData[pumpid][pumpPos][3],
	    PumpData[pumpid][pumpFuel],
	    BizData[PumpData[pumpid][pumpBusiness]][bizID],
	    PumpData[pumpid][pumpID]
	);
	printf("[Save - Pump] berhasil menyimpan data pump %i id", pumpid);
	return mysql_tquery(sqlcon, query);
}

Pump_Nearest(playerid)
{
    for (new i = 0; i != MAX_GAS_PUMPS; i ++) if (PumpData[i][pumpExists] && IsPlayerInRangeOfPoint(playerid, 4.0, PumpData[i][pumpPos][0], PumpData[i][pumpPos][1], PumpData[i][pumpPos][2]) && PumpData[i][pumpExists]) {
	    return i;
	}
	return -1;
}

StopRefilling(playerid)
{
    PlayerData[playerid][pGasPump] = -1;
    PlayerData[playerid][pGasStation] = -1;
   	PlayerData[playerid][pRefill] = INVALID_VEHICLE_ID;
	PlayerData[playerid][pRefillPrice] = 0;
}

stock IsPumpOccupied(pumpid)
{
	foreach (new i : Player) if (PlayerData[i][pRefill] != INVALID_VEHICLE_ID) {
	    if (PlayerData[i][pGasPump] == pumpid) return 1;
	}
	return 0;
}

task RefuelCheck[500]()
{
	new
	    string[128];

	foreach (new i : Player) if(IsPlayerSpawn(i))
	{
	    if (!PlayerData[i][pSpawned] || PlayerData[i][pRefill] == INVALID_VEHICLE_ID)
	        continue;

        if (PlayerData[i][pRefill] != INVALID_VEHICLE_ID && PlayerData[i][pGasPump] != -1)
		{
		    PlayerData[i][pRefillPrice]++;

		    VehCore[PlayerData[i][pRefill]][vehFuel] ++;
		    PumpData[PlayerData[i][pGasPump]][pumpFuel] --;

		    if (PumpData[PlayerData[i][pGasPump]][pumpExists])
			{
			    format(string, sizeof(string), "[Fuel Station %i]\n{FFFFFF}Fuel Left: %d liters", PlayerData[i][pGasPump], PumpData[PlayerData[i][pGasPump]][pumpFuel]);
			    UpdateDynamic3DTextLabelText(PumpData[PlayerData[i][pGasPump]][pumpText3D], COLOR_DARKBLUE, string);
			}
			if (VehCore[PlayerData[i][pRefill]][vehFuel] >= 100 || GetEngineStatus(PlayerData[i][pRefill]) || !PumpData[PlayerData[i][pGasPump]][pumpExists] || PumpData[PlayerData[i][pGasPump]][pumpFuel] < 0)
			{
			    VehCore[PlayerData[i][pRefill]][vehFuel] = 100;

			    GiveMoney(i, -PlayerData[i][pRefillPrice]);
			    SendServerMessage(i, "You have refilled your vehicle for $%d.", PlayerData[i][pRefillPrice]);

			    if (PumpData[PlayerData[i][pGasPump]][pumpExists])
				{
					if (PumpData[PlayerData[i][pGasPump]][pumpFuel] < 0)
						PumpData[PlayerData[i][pGasPump]][pumpFuel] = 0;

					BizData[PlayerData[i][pGasStation]][bizVault] += PlayerData[i][pRefillPrice];
					Business_Save(PlayerData[i][pGasStation]);

					Pump_Save(PlayerData[i][pGasPump]);
				}
				StopRefilling(i);
			}
		}
	}
	return 1;
}

forward OnPumpCreated(pumpid);
public OnPumpCreated(pumpid)
{
    PumpData[pumpid][pumpID] = cache_insert_id();
	Pump_Save(pumpid);
	Pump_Refresh(pumpid);

	return 1;
}

SSCANF:pump(const str[])
{
    if(!strcmp(str, "create", true)) return 1;
    if(!strcmp(str, "delete", true)) return 2;
    if(!strcmp(str, "fuel", true)) return 3;
    return 0;
}
CMD:pumpmenu(playerid, params[])
{
	if(!IsPlayerADuty(playerid, 3))
	    return PermissionError(playerid);

    static action, nextParams[123];
	if (sscanf(params, "k<pump>S()[123]", action, nextParams))
	    return SendSyntaxMessage(playerid, "/pumpmenu [create, delete, fuel]");

    switch(action)
    {
        case 1:
        {
            static id, bizid;
            if(sscanf(nextParams, "i", bizid))
                return SendSyntaxMessage(playerid, "/pumpmenu [create] [businessid]");

            if ((bizid < 0 || bizid >= MAX_BUSINESS) || !BizData[bizid][bizExists])
                return SendErrorMessage(playerid, "You enter an invalid business ID.");

            if (GetPlayerInterior(playerid) > 0 || GetPlayerVirtualWorld(playerid) > 0)
                return SendErrorMessage(playerid, "You can only create gas pumps outside interiors.");

            id = Pump_Create(playerid, bizid);

            if (id == -1)
                return SendErrorMessage(playerid, "The business has reached the limit for gas pumps.");

            SendServerMessage(playerid, "You have successfully created gas pump ID: %d.", id);

            PlayerData[playerid][pEditPump] = id;
			PlayerData[playerid][pEditingMode] = EDIT_OBJECT_PUMP;
            EditDynamicObject(playerid, PumpData[id][pumpObject]);

        }
        case 2:
        {
            static id;
            if (sscanf(nextParams, "d", id))
                return SendSyntaxMessage(playerid, "/pumpmenu [delete] [pump-id]");

            if ((id < 0 || id >= MAX_GAS_PUMPS) || !PumpData[id][pumpExists])
                return SendErrorMessage(playerid, "Invalid pump ID.");

            Pump_Delete(id);
            SendServerMessage(playerid, "You have successfully destroyed pump ID: %d.", id);
        }
        case 3:
        {
            static id, amount;
            if (sscanf(nextParams, "dd", id, amount))
                return SendSyntaxMessage(playerid, "/pumpmenu [fuel] [pump-id] [fuel-amount]");

            if ((id < 0 || id >= MAX_GAS_PUMPS) || !PumpData[id][pumpExists])
                return SendErrorMessage(playerid, "Invalid pump ID.");

            PumpData[id][pumpFuel] = amount;

            Pump_Refresh(id);
            Pump_Save(id);

            SendServerMessage(playerid, "You have set the fuel to %d for pump ID: %d.", amount, id);
        }
    }
	return 1;
}

CMD:refuel(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);

	if (PlayerData[playerid][pRefill] != INVALID_VEHICLE_ID)
	{
	    BizData[PlayerData[playerid][pGasStation]][bizVault] += PlayerData[playerid][pRefillPrice];
		Business_Save(PlayerData[playerid][pGasStation]);

        GiveMoney(playerid, -PlayerData[playerid][pRefillPrice]);

		SendServerMessage(playerid, "You have refueled your vehicle for $%d.", PlayerData[playerid][pRefillPrice]);
        StopRefilling(playerid);

        return 1;
	}

	if (!vehicleid)
	    return SendErrorMessage(playerid, "You are not inside any vehicle!");

	if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
	    return SendErrorMessage(playerid, "You must be the driver of the vehicle!");

	if (GetEngineStatus(vehicleid))
	    return SendErrorMessage(playerid, "You must turn the engine off first.");

	new id = Pump_Nearest(playerid);

	if (id != -1)
	{
		if (VehCore[vehicleid][vehFuel] > 95)
			return SendErrorMessage(playerid, "This vehicle doesn't need any fuel.");

		if (IsPumpOccupied(id))
		    return SendErrorMessage(playerid, "This fuel pump is already occupied.");

		if (PumpData[id][pumpFuel] < 1)
   			return SendErrorMessage(playerid, "This pump doesn't have enough fuel.");

		PlayerData[playerid][pGasPump] = id;
		PlayerData[playerid][pGasStation] = PumpData[id][pumpBusiness];

		PlayerData[playerid][pRefill] = vehicleid;
		SendNearbyMessage(playerid, 30.0, COLOR_PURPLE, "** %s has started refilling their vehicle.", ReturnName(playerid));
	}
	else
	{
		SendErrorMessage(playerid, "You are not in range of any unused gas pump.");
	}
	return 1;
}