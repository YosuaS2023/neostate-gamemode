new farmer_attach_trailer[6];
new farmer_tractor[6];
new farmer_vehicle[6];

forward AttachTrailerFarmer(playerid);

function ogmi_LoadFarmerWheat()
{
	//pickup
	AddStaticPickup(1274, 1, -1068.2452,-1211.4783,129.7352, 0);
	Create3DTextLabel("Type '"YELLOW"/sell"WHITE"' to sell.", COLOR_WHITE, -1068.2452,-1211.4783,129.7352, 40.0, 0, 0);
	
	AddStaticPickup(1239, 1, -1080.9950,-1195.5438,129.5373, 0);
	Create3DTextLabel("Type '"YELLOW"/buyseed"WHITE"' to buy wheat seed.", COLOR_WHITE, -1080.9950,-1195.5438,129.5373, 40.0, 0, 0);
	
	// harvester
	farmer_vehicle[0] = CreateVehicle(532, -1146.4846, -1216.4688, 130.1527, 0.0000, -1, -1, 100);
	farmer_vehicle[1] = CreateVehicle(532, -1133.4136, -1216.5178, 130.1527, 0.0000, -1, -1, 100);
	farmer_vehicle[2] = CreateVehicle(532, -1159.4052, -1216.4301, 130.1527, 0.0000, -1, -1, 100);
	farmer_vehicle[3] = CreateVehicle(532, -1172.7479, -1216.3771, 130.1527, 0.0000, -1, -1, 100);
	farmer_vehicle[4] = CreateVehicle(532, -1187.3530, -1216.1278, 130.1527, 0.0000, -1, -1, 100);
	farmer_vehicle[5] = CreateVehicle(532, -1200.5715, -1215.9579, 130.1527, 0.0000, -1, -1, 100);
	// tractor
	farmer_tractor[0] = CreateVehicle(531, -1116.6208, -1281.2704, 128.8731, 0.0000, -1, -1, 100);
	farmer_tractor[1] = CreateVehicle(531, -1111.1036, -1281.2610, 128.8731, 0.0000, -1, -1, 100);
	farmer_tractor[2] = CreateVehicle(531, -1105.7200, -1281.4061, 128.8731, 0.0000, -1, -1, 100);
	farmer_tractor[3] = CreateVehicle(531, -1099.8613, -1281.5104, 128.8731, 0.0000, -1, -1, 100);
	farmer_tractor[4] = CreateVehicle(531, -1094.8220, -1281.5619, 128.8731, 0.0000, -1, -1, 100);
	farmer_tractor[5] = CreateVehicle(531, -1089.4532, -1281.5878, 128.8731, 0.0000, -1, -1, 100);
	//attach trailer
	farmer_attach_trailer[0] = CreateVehicle(610, -1116.6277, -1287.5369, 128.5087, 0.0000, -1, -1, 100);
	farmer_attach_trailer[1] = CreateVehicle(610, -1111.0574, -1287.5143, 128.5087, 0.0000, -1, -1, 100);
	farmer_attach_trailer[2] = CreateVehicle(610, -1105.5824, -1287.4360, 128.5087, 0.0000, -1, -1, 100);
	farmer_attach_trailer[3] = CreateVehicle(610, -1099.9484, -1287.4661, 128.5087, 0.0000, -1, -1, 100);
	farmer_attach_trailer[4] = CreateVehicle(610, -1094.7124, -1287.5112, 128.5087, 0.0000, -1, -1, 100);
	farmer_attach_trailer[5] = CreateVehicle(610, -1089.3505, -1287.6187, 128.5087, 0.0000, -1, -1, 100);
	
	for(new i = 0; i < 6; i++)
	{
		VehCore[farmer_tractor[i]][vehFuel] = 100;
		VehCore[farmer_vehicle[i]][vehFuel] = 100;
		VehCore[farmer_tractor[i]][vehPlanting] = false;
	}
	return 1;
}

CMD:buyseed(playerid, params[])
{
	if (IsPlayerInRangeOfPoint(playerid, 5.0, -1080.9950,-1195.5438,129.5373))
	{
		new seeds;
		if(sscanf(params, "d", seeds))
				return SendSyntaxMessage(playerid,"/buyseed [amount:seed]");
		
		if(GetMoney(playerid) < @economy get<wheat_seed> * seeds)
				return SendErrorMessage(playerid, "You don't have enough money.");

		Inventory_Add(playerid, "wheat seed", 0, seeds);
		GiveMoney(playerid, -@economy get<wheat_seed> * seeds);
	}
	return 1;
}

CMD:sell(playerid, params[])
{
	if (IsPlayerInRangeOfPoint(playerid, 5.0, -1068.2452,-1211.4783,129.7352))
	{
		new arg[32], amount;	
		if(sscanf(params, "s[32]d", arg, amount))
			return SendSyntaxMessage(playerid, "/sell [input: wheat] [amount]");
				
		switch(HashStr(arg))
		{
			case HS<wheat>:
			{
				if(Inventory_Count(playerid, "wheat") < amount)	
					return SendErrorMessage(playerid, "You don't have wheat!");

				GiveMoney(playerid, @economy get<farmer_wheat> * amount);
				Inventory_Remove(playerid, "wheat", amount);
				SendServerMessage(playerid, "you sell wheat as much %i with %s price", amount, FormatNumber(@economy get<farmer_wheat> * amount));
			}
		}
	}
	return 1;
}

ptask IsPlayerFarmer[300](playerid)
{
	new Float:X, Float:Y, Float:Z, Float:Vx, Float:Vy, Float:Vz, Float:Vxi, Float:Vyi, Float:Vzi;
	new vehicleid = GetPlayerVehicleID(playerid);

	GetPlayerPos(playerid, X, Y, Z);
	GetVehicleRelativePos(GetVehicleTrailer(GetPlayerVehicleID(playerid)), Vx, Vy, Vz, Vxi, Vyi - 3, Vzi);
	
	if(GetVehicleFarmer(playerid) && VehCore[vehicleid][vehPlanting])
	{
		if(GetVehicleSpeedKMH( GetPlayerVehicleID(playerid) ) > Float:10.0 && Inventory_Count(playerid, "wheat seed") != 0)
		{
			GetVehicleRelativePos(GetVehicleTrailer(GetPlayerVehicleID(playerid)), Vx, Vy, Vz, Vxi, Vyi - 3, Vzi);
			
			if (X <= -1006.72791 && X >= -1194.97083 && Y <= -915.93036 && Y >= -1064.27112) 
			{
				foreach(new plant : PlantIds) if(IsPlayerInRangeOfPoint(playerid, 10.0, PositionXYZ(PlantData[plant][plantPos]))) return 1;
				Plant_Create(playerid, PLANT_WHEAT, Vx, Vy, Vz, Vz - 4, 0.01);
				Inventory_Remove(playerid, "wheat seed");
			}
		
		}
	}
	
	else if(GetPlayerVehicleID(playerid) == GetVehicleHarvest(playerid))
	{
		new Float:x, Float: y, Float:z;
		foreach(new i : PlantIds)
		{
			GetDynamicObjectPos(PlantData[i][plantObject], x, y, z);
			if(GetVehicleDistanceFromPoint(GetPlayerVehicleID(playerid), x, y, z) < 7.0 && !IsDynamicObjectMoving(PlantData[i][plantObject]))
			{
				Plant_Destroy(i);
				Inventory_Add(playerid, "wheat", 0, 1);
				break;
			}
			
		}
	}
	return 1;

}

hook OnPlayerConnect(playerid)
{
	RemoveBuildingForPlayer(playerid, 3276, -1116.8594, -1255.3594, 129.0625, 0.25);
	RemoveBuildingForPlayer(playerid, 3276, -1105.1094, -1255.3594, 129.0625, 0.25);
	RemoveBuildingForPlayer(playerid, 708, -1124.4922, -1140.8984, 128.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 3276, -1117.4844, -1150.9688, 129.0625, 0.25);
	RemoveBuildingForPlayer(playerid, 3276, -1105.7969, -1150.9688, 129.0625, 0.25);
}

stock GetVehicleRelativePos(vehicleid, &Float:x, &Float:y, &Float:z, Float:xoff= 0.0, Float:yoff= 0.0, Float:zoff= 0.0)
{
    new Float:rot;
    GetVehicleZAngle(vehicleid, rot);
    rot = 360 - rot;
    GetVehiclePos(vehicleid, x, y, z);
    x = floatsin(rot, degrees) * yoff + floatcos(rot, degrees) * xoff + x;
    y = floatcos(rot, degrees) * yoff - floatsin(rot, degrees) * xoff + y;
    z = zoff + z;
}

stock GetVehicleFarmer(playerid)
{
	new vehicle = 0;
	for(new i = 0; i < 6; ++i)
	{
		if(GetPlayerVehicleID(playerid) == farmer_tractor[i])
		{
			vehicle = farmer_tractor[i];
		}
	}

	return vehicle;
}

stock GetVehicleHarvest(playerid)
{
	new vehicle = 0;
	for(new i = 0; i < 6; ++i)
	{
		if(GetPlayerVehicleID(playerid) == farmer_vehicle[i])
		{
			vehicle = farmer_vehicle[i];
		}
	}

	return vehicle;
}

public AttachTrailerFarmer(playerid)
{
    new Float:vX, Float:vY, Float:vZ;
	for(new i = 0; i < 6; ++i)
	{
		GetVehiclePos(farmer_attach_trailer[i],vX,vY,vZ);
		if(IsPlayerInRangeOfPoint(playerid, 5.0, vX, vY, vZ))  
		{
			AttachTrailerToVehicle(farmer_attach_trailer[i],GetPlayerVehicleID(playerid));
			return 1;
		}
	}
	return 0;
}  
