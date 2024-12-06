#include <UtilsScripts\Als>

GetVehicleDealerCost(carid) // source : lrp - function
{
	if(carid == 401) return 230000; //Bravura
	if(carid == 585) return 210000;  //Emperor
	if(carid == 546) return 220000; //Intruder
	if(carid == 547) return 220000; //Primo
	if(carid == 549) return 260000; //Tampa
	if(carid == 560) return 560000; //Sultan
	if(carid == 550) return 280000; //Sunrise
	if(carid == 551) return 210000; //Merit
	if(carid == 562) return 530000; //Elegy
	if(carid == 540) return 210000; //Vincent
	if(carid == 542) return 280000; //Clover
	if(carid == 529) return 260000; //Willard
	if(carid == 527) return 220000; //Cadrona
	if(carid == 517) return 210000; //Majestic
	if(carid == 518) return 200000; //Buccaneer
	if(carid == 507) return 260000; //Elegant
	if(carid == 516) return 210000; //Nebula
	if(carid == 492) return 180000; //Greenwood
	if(carid == 491) return 170000; //Virgo
	if(carid == 474) return 90000; //Hermes
	if(carid == 436) return 220000; //Previon
	if(carid == 445) return 250000; //Admiral
	if(carid == 419) return 270000; //Esperanto
	if(carid == 426) return 290000; //Premier
	if(carid == 421) return 230000; //Washington
	if(carid == 410) return 200000; //Manana
	if(carid == 405) return 190000; //Sentinel
	if(carid == 466) return 200000; //Glendale
	if(carid == 467) return 190000; //Oceanic
	if(carid == 581) return 160000; //BF-400
	if(carid == 586) return 140000; //Wayfarer
	if(carid == 521) return 540000; //FCR-900
	if(carid == 468) return 100000; //Sanchez
	if(carid == 471) return 310000; //Quad
	if(carid == 462) return 60000; //Faggio
	if(carid == 463) return 190000; //Freeway
	if(carid == 461) return 180000; //PCJ-600
	if(carid == 404) return 190000; //Perenniel
	if(carid == 458) return 380000; //Solair
	if(carid == 561) return 400000; //Stratum
	if(carid == 479) return 290000; //Regina
	if(carid == 418) return 290000; //Moonbeam
	if(carid == 534) return 290000; //Remington
	if(carid == 535) return 390000; //Slamvan
	if(carid == 536) return 290000; //Blade
	if(carid == 566) return 270000; //Tahoma
	if(carid == 567) return 270000; //Savanna
	if(carid == 575) return 250000; //Broadway
	if(carid == 576) return 180000; //Tornado
	if(carid == 412) return 190000; //Voodoo
	if(carid == 400) return 320000; //Landstalker
	if(carid == 500) return 360000; //Mesa
	if(carid == 579) return 500000; //Huntley
	if(carid == 554) return 390000; //Yosemite
	if(carid == 545) return 300000; //Hustler
	if(carid == 483) return 300000; //Camper
	if(carid == 508) return 490000; //Journey
	if(carid == 480) return 480000; //Comet
	if(carid == 439) return 430000; //Stallion
	if(carid == 533) return 420000; //Feltzer
	if(carid == 413) return 230000; //Pony
	if(carid == 543) return 140000; //Sadler
	if(carid == 422) return 170000; //Bobcat
	if(carid == 478) return 120000; //Walton
	if(carid == 482) return 200000; //Burrito
	if(carid == 600) return 130000; //Picador
	if(carid == 602) return 220000; //Alpha
	if(carid == 587) return 390000; //Euros
	if(carid == 589) return 330000; //Club
	if(carid == 565) return 330000; //Flash
	if(carid == 559) return 250000; //Jester
	if(carid == 558) return 150000; //Uranus
	if(carid == 429) return 900000; //Banshee
	if(carid == 402) return 280000; //Bufallo
	if(carid == 415) return 550000; //Cheetah
	if(carid == 475) return 170000; //Sabre
	if(carid == 477) return 380000; //ZR-350
	if(carid == 496) return 280000; //Blista Compact
 	return -1;
}

function IsAVehicleDealerVeh(carid)
{
	foreach(new d : Dealership)
    {
		for(new v = 0; v < MAX_DEALERSHIPVEHICLES; v++) 
		{
			if(carid == CarDealershipInfo[d][cdVehicleId][v]) return 1;
		}
	}	
	return 0;
}

function GetCarDealershipVehicleId(vehicleid)
{
    foreach(new d : Dealership)
    {
        for(new v = 0; v < MAX_DEALERSHIPVEHICLES; v++)
        {
            if(vehicleid == CarDealershipInfo[d][cdVehicleId][v])
            {
                return v;
            }
		}
    }
    return 0;
}

GetCarDealershipId(vehicleid)
{
    foreach(new d : Dealership)
    {
        for(new v = 0; v < MAX_DEALERSHIPVEHICLES; v++)
        {
            if(CarDealershipInfo[d][cdVehicleId][v] == vehicleid)
            {
                return d;
            }
		}
    }
    return -1;
}

/* function */
function LoadcDealerships()
{
	new rows = cache_num_rows(), queryBuffer[212];
 	if(rows)
  	{
		for(new id; id < rows; id++)
		{
			cache_get_value_name_int(id, "ID", CarDealershipInfo[id][cdID]);
			cache_get_value_name(id, "owner", CarDealershipInfo[id][cdOwner], 128);
			cache_get_value_name(id, "name", CarDealershipInfo[id][cdMessage], 128);

			cache_get_value_name_float(id, "entrancex", CarDealershipInfo[id][cdEntranceX]);
			cache_get_value_name_float(id, "entrancey", CarDealershipInfo[id][cdEntranceY]);
			cache_get_value_name_float(id, "entrancez", CarDealershipInfo[id][cdEntranceZ]);

			cache_get_value_name_int(id, "till", CarDealershipInfo[id][cdTill]);
			cache_get_value_name_int(id, "interior", CarDealershipInfo[id][cdInterior]);

			cache_get_value_name_float(id, "vehiclespawnx", CarDealershipInfo[id][cdVehicleSpawn][0]);
			cache_get_value_name_float(id, "vehiclespawny", CarDealershipInfo[id][cdVehicleSpawn][1]);
			cache_get_value_name_float(id, "vehiclespawnz", CarDealershipInfo[id][cdVehicleSpawn][2]);
			cache_get_value_name_float(id, "vehiclespawna", CarDealershipInfo[id][cdVehicleSpawn][3]);

			cache_get_value_name_float(id, "radius", CarDealershipInfo[id][cdRadius]);
			cache_get_value_name_int(id, "price", CarDealershipInfo[id][cdPrice]);
			cache_get_value_name_int(id, "type", CarDealershipInfo[id][cdType]);
			for (new j = 0; j < 10; j ++)
			{
				format(queryBuffer, 24, "vehiclex%d", j);
				cache_get_value_name_float(id, queryBuffer, CarDealershipInfo[id][cdVehicleSpawnX][j]);

				format(queryBuffer, 24, "vehicley%d", j);
				cache_get_value_name_float(id, queryBuffer, CarDealershipInfo[id][cdVehicleSpawnY][j]);

				format(queryBuffer, 24, "vehiclez%d", j);
				cache_get_value_name_float(id, queryBuffer, CarDealershipInfo[id][cdVehicleSpawnZ][j]);

				format(queryBuffer, 24, "vehiclea%d", j);
				cache_get_value_name_float(id, queryBuffer, CarDealershipInfo[id][cdVehicleSpawnAngle][j]);

				format(queryBuffer, 24, "vehicleprice%d", j);
				cache_get_value_name_int(id, queryBuffer, CarDealershipInfo[id][cdVehicleCost][j]);

				format(queryBuffer, 24, "vehiclemodel%d", j);
				cache_get_value_name_int(id, queryBuffer, CarDealershipInfo[id][cdVehicleModel][j]);
			}
			CreateDealershipPickup(id);
			Iter_Add(Dealership, id);
		}
		printf("*** [Database: Loaded] dealership data (%d count).", rows);
	}
}

SavecDealership(id)
{
	new cQuery[5000];
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "UPDATE car_dealer SET ");
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%sowner = '%s', ", cQuery, CarDealershipInfo[id][cdOwner]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%sname = '%s', ", cQuery, CarDealershipInfo[id][cdMessage]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%sentrancex = '%f', ", cQuery, CarDealershipInfo[id][cdEntranceX]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%sentrancey = '%f', ", cQuery, CarDealershipInfo[id][cdEntranceY]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%sentrancez = '%f', ", cQuery, CarDealershipInfo[id][cdEntranceZ]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%still = '%d', ", cQuery, CarDealershipInfo[id][cdTill]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%sinterior = '%d', ", cQuery, CarDealershipInfo[id][cdInterior]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclespawnx = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawn][0]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclespawny = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawn][1]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclespawnz = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawn][2]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclespawna = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawn][3]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%sradius = '%f', ", cQuery, CarDealershipInfo[id][cdRadius]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%sprice = '%d', ", cQuery, CarDealershipInfo[id][cdPrice]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%stype = '%d', ", cQuery, CarDealershipInfo[id][cdType]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclex0 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnX][0]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicley0 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnY][0]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclez0 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnZ][0]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclea0 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnAngle][0]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicleprice0 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleCost][0]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclemodel0 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleModel][0]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclex1 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnX][1]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicley1 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnY][1]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclez1 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnZ][1]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclea1 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnAngle][1]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicleprice1 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleCost][1]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclemodel1 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleModel][1]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclex2 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnX][2]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicley2 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnY][2]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclez2 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnZ][2]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclea2 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnAngle][2]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicleprice2 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleCost][2]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclemodel2 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleModel][2]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclex3 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnX][3]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicley3 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnY][3]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclez3 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnZ][3]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclea3 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnAngle][3]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicleprice3 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleCost][3]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclemodel3 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleModel][3]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclex4 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnX][4]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicley4 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnY][4]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclez4 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnZ][4]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclea4 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnAngle][4]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicleprice4 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleCost][4]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclemodel4 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleModel][4]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclex5 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnX][5]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicley5 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnY][5]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclez5 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnZ][5]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclea5 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnAngle][5]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicleprice5 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleCost][5]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclemodel5 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleModel][5]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclex6 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnX][6]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicley6 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnY][6]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclez6 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnZ][6]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclea6 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnAngle][6]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicleprice6 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleCost][6]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclemodel6 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleModel][6]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclex7 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnX][7]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicley7 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnY][7]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclez7 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnZ][7]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclea7 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnAngle][7]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicleprice7 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleCost][7]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclemodel7 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleModel][7]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclex8 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnX][8]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicley8 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnY][8]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclez8 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnZ][8]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclea8 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnAngle][8]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicleprice8 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleCost][8]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclemodel8 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleModel][8]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclex9 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnX][9]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicley9 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnY][9]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclez9 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnZ][9]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclea9 = '%f', ", cQuery, CarDealershipInfo[id][cdVehicleSpawnAngle][9]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehicleprice9 = '%d', ", cQuery, CarDealershipInfo[id][cdVehicleCost][9]);
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%svehiclemodel9 = '%d' ", cQuery, CarDealershipInfo[id][cdVehicleModel][9]);
	
	mysql_format(sqlcon, cQuery, sizeof(cQuery), "%s WHERE ID = '%d'", cQuery, CarDealershipInfo[id][cdID]);
	mysql_tquery(sqlcon, cQuery);
	return 1;
}    

function cdDealer(dealershipid)
{
	CarDealershipInfo[dealershipid][cdID] = cache_insert_id();
	SavecDealership(dealershipid);
	CreateDealershipPickup(dealershipid);
	return 1;
}

CreateDealershipPickup(d)
{
    new String[500];

    if(IsValidDynamic3DTextLabel(CarDealershipInfo[d][cdTextLabel]))
	    DestroyDynamic3DTextLabel(CarDealershipInfo[d][cdTextLabel]);

    if(IsValidDynamicPickup(CarDealershipInfo[d][cdPickupID]))
	    DestroyDynamicPickup(CarDealershipInfo[d][cdPickupID]);

	if(CarDealershipInfo[d][cdEntranceX] != 0.0 && CarDealershipInfo[d][cdEntranceY] != 0.0)
	{
		CarDealershipInfo[d][cdPickupID] = CreateDynamicPickup(1239, 1, CarDealershipInfo[d][cdEntranceX], CarDealershipInfo[d][cdEntranceY], CarDealershipInfo[d][cdEntranceZ]);
		if(strcmp(CarDealershipInfo[d][cdOwner], "-"))
		{
			format(String, sizeof(String),"{00FFFF}[id:%d]\n{FFFF00}%s\n"YELLOW_E"Owner: "WHITE_E"%s",d, CarDealershipInfo[d][cdMessage], CarDealershipInfo[d][cdOwner]);
			CarDealershipInfo[d][cdTextLabel] = CreateDynamic3DTextLabel(String,COLOR_TWAQUA,CarDealershipInfo[d][cdEntranceX], CarDealershipInfo[d][cdEntranceY], CarDealershipInfo[d][cdEntranceZ]+0.75,3.0,INVALID_PLAYER_ID,INVALID_VEHICLE_ID,1);
		}
		else
		{
			format(String, sizeof(String), "{00FFFF}[id:%d]\n"WHITE_E"This dealership for sale\n{FFFFFF}Price: "GREEN_E"$%s\n"WHITE_E"Use '"YELLOW_E"/buydealership"WHITE_E"' for purchase this dealership",d, FormatNumber(CarDealershipInfo[d][cdPrice]));
			CarDealershipInfo[d][cdTextLabel] = CreateDynamic3DTextLabel(String,COLOR_TWAQUA,CarDealershipInfo[d][cdEntranceX], CarDealershipInfo[d][cdEntranceY], CarDealershipInfo[d][cdEntranceZ]+0.75,3.0,INVALID_PLAYER_ID,INVALID_VEHICLE_ID,1);
		}
	}
	for(new fv = 0; fv < MAX_DEALERSHIPVEHICLES; fv++)
	{
		if(IsValidDynamic3DTextLabel(CarDealershipInfo[d][cdVehicleLabel][fv]))
            DestroyDynamic3DTextLabel(CarDealershipInfo[d][cdVehicleLabel][fv]);

		if (IsValidVehicle(CarDealershipInfo[d][cdVehicleId][fv]))
        {
			DestroyVehicle(CarDealershipInfo[d][cdVehicleId][fv]);
            CarDealershipInfo[d][cdVehicleId][fv] = INVALID_VEHICLE_ID;
        }

		if(CarDealershipInfo[d][cdVehicleModel][fv] != 0 && CarDealershipInfo[d][cdVehicleSpawnY][fv] != 0.0)
		{
			CarDealershipInfo[d][cdVehicleId][fv] = CreateVehicle(CarDealershipInfo[d][cdVehicleModel][fv], CarDealershipInfo[d][cdVehicleSpawnX][fv], CarDealershipInfo[d][cdVehicleSpawnY][fv], CarDealershipInfo[d][cdVehicleSpawnZ][fv], CarDealershipInfo[d][cdVehicleSpawnAngle][fv], -1, -1, -1);
			if(IsValidVehicle(CarDealershipInfo[d][cdVehicleId][fv]))
			{
				format(String, sizeof(String), "[slot:%d]\n{00FFFF}%s\n"WHITE_E"Price: "GREEN_E"$%s\n"WHITE_E"Fuel Capacity: "YELLOW_E"100.0\n"WHITE_E"Fuel Total: "YELLOW_E"100.0",fv, GetVehicleName(CarDealershipInfo[d][cdVehicleId][fv]), FormatNumber(CarDealershipInfo[d][cdVehicleCost][fv]));
				CarDealershipInfo[d][cdVehicleLabel][fv] = CreateDynamic3DTextLabel(String, COLOR_ARWIN, 0.0, 0.0, 0.0, 8.0, INVALID_PLAYER_ID, CarDealershipInfo[d][cdVehicleId][fv]);
				
                SetVehicleNumberPlate(CarDealershipInfo[d][cdVehicleId][fv],""GREEN_E"DEALER");
				SetVehicleHealth(CarDealershipInfo[d][cdVehicleId][fv], 1000);
			}
		}
		else if(CarDealershipInfo[d][cdVehicleModel][fv] == 0 && CarDealershipInfo[d][cdVehicleSpawnY][fv] != 0.0)
		{
			format(String, sizeof(String), "[slot:%d]\n"GREEN_E"No vehicle to preview\n"WHITE_E"Use '"YELLOW_E"/dm > Vehicles"WHITE_E"' to display vehicle here",fv);
			CarDealershipInfo[d][cdVehicleLabel][fv] = CreateDynamic3DTextLabel(String,COLOR_ARWIN,CarDealershipInfo[d][cdVehicleSpawnX][fv], CarDealershipInfo[d][cdVehicleSpawnY][fv], CarDealershipInfo[d][cdVehicleSpawnZ][fv],8.0,INVALID_PLAYER_ID,INVALID_VEHICLE_ID);
		}
	}
	return 1;
}

SellCarDealership(dealershipid)
{
	new String[1024];
	CarDealershipInfo[dealershipid][cdOwned] = 0;
	strmid(CarDealershipInfo[dealershipid][cdOwner], "-", 0, MAX_PLAYER_NAME, 255);

	format(String, sizeof(String), "{00FFFF}[id:%d]\n"WHITE_E"This dealership for sale\n{FFFFFF}Price: "GREEN_E"$%s\n"WHITE_E"Use '"YELLOW_E"/buydealership"WHITE_E"' for purchase this dealership",dealershipid, FormatNumber(CarDealershipInfo[dealershipid][cdPrice]));
	UpdateDynamic3DTextLabelText(CarDealershipInfo[dealershipid][cdTextLabel], COLOR_TWAQUA, String);
	SavecDealership(dealershipid);
    return 1;
}

IsPlayerOwnerOfCDEx(playerid, id)
{
	if(!IsPlayerConnected(playerid) && !PlayerData[playerid][pSpawned]) return 0;
	if(id == -1) return 0;
	if(!strcmp(CarDealershipInfo[id][cdOwner], PlayerData[playerid][pName], true)) return 1;
	return 0;
}

forward Vehicle_Dealer(playerid, dealershipid, cdvehicleid, modelid, price, x, y, z, a);
public Vehicle_Dealer(playerid, dealershipid, cdvehicleid, modelid, price, x, y, z, a)
{
	CarDealershipInfo[dealershipid][cdVehicleDBID][cdvehicleid] = cache_insert_id();
	CarDealershipInfo[dealershipid][cdVehicleModel][cdvehicleid] = modelid;
    CarDealershipInfo[dealershipid][cdVehicleCost][cdvehicleid] = price;
    CarDealershipInfo[dealershipid][cdVehicleSpawnX][cdvehicleid] = x;
    CarDealershipInfo[dealershipid][cdVehicleSpawnY][cdvehicleid] = y;
    CarDealershipInfo[dealershipid][cdVehicleSpawnZ][cdvehicleid] = z;
    CarDealershipInfo[dealershipid][cdVehicleSpawnAngle][cdvehicleid] = a;

	SavecDealership(dealershipid);
	return 1;
}

DestroyCarDealershipVehicle(dealershipid, cdvehicleid)
{
    CarDealershipInfo[dealershipid][cdVehicleModel][cdvehicleid] = 0;
    CarDealershipInfo[dealershipid][cdVehicleCost][cdvehicleid] = 0;
    CarDealershipInfo[dealershipid][cdVehicleSpawnX][cdvehicleid] = 0.0;
    CarDealershipInfo[dealershipid][cdVehicleSpawnY][cdvehicleid] = 0.0;
    CarDealershipInfo[dealershipid][cdVehicleSpawnZ][cdvehicleid] = 0.0;
    CarDealershipInfo[dealershipid][cdVehicleSpawnAngle][cdvehicleid] = 0.0;

    if(IsValidDynamic3DTextLabel(CarDealershipInfo[dealershipid][cdVehicleLabel][cdvehicleid]))
        DestroyDynamic3DTextLabel(CarDealershipInfo[dealershipid][cdVehicleLabel][cdvehicleid]);

    if(IsValidVehicle(CarDealershipInfo[dealershipid][cdVehicleId][cdvehicleid]))
        DestroyVehicle(CarDealershipInfo[dealershipid][cdVehicleId][cdvehicleid]);

    CarDealershipInfo[dealershipid][cdVehicleLabel][cdvehicleid] = Text3D:INVALID_3DTEXT_ID;
    CarDealershipInfo[dealershipid][cdVehicleId][cdvehicleid] = INVALID_VEHICLE_ID;

    return 1;
}

Dealership_IsEmployee(playerid, bizid) {
	new str[128], Cache: cache;
	format(str, sizeof(str), "SELECT * FROM `car_dealer_employee` WHERE `Name`='%s' AND `Biz`='%d'", PlayerData[playerid][pName], CarDealershipInfo[bizid][cdID]);
	cache = mysql_query(sqlcon, str);
	new result = cache_num_rows();
	cache_delete(cache);
	return result;
}

Dealer_ShowEmployee(playerid, id, type = 0)
{
	new query[255], Cache: cache;
	format(query, sizeof(query), "SELECT * FROM `car_dealer_employee` WHERE `Biz`='%d'", CarDealershipInfo[id][cdID]);
	cache = mysql_query(sqlcon, query);

	if(!cache_num_rows()) return SendErrorMessage(playerid, "There are no one employee for this business.");
	
	format(query, sizeof(query), "#\tName\tDate Added\n");
	for(new i; i < cache_num_rows(); i++)
    {
		new biz,
            time,
			name[24];

        cache_get_value_int(i, "ID", biz);
		cache_get_value_int(i, "Time", time);
		cache_get_value(i, "Name", name, sizeof(name));
		format(query, sizeof(query), "%s%d\t%s\t%s\n", query, biz, name, experieddate(time,3));
	}
	if (!type)
		Dialog_Show(playerid, "unused", DIALOG_STYLE_TABLIST_HEADERS, "Business Employee", query, "Close", "");
	else
		Dialog_Show(playerid, Dealer_RemoveEmployee, DIALOG_STYLE_TABLIST_HEADERS, "Remove Employee", query, "Remove", "Close");

	cache_delete(cache);
	return 1;
}

DealerEmployee_Remove(id)
{
	new query[200];
	format(query,sizeof(query),"DELETE FROM `car_dealer_employee` WHERE `ID`='%d'", id);
	mysql_tquery(sqlcon, query);
	return 1;
}

DealerEmployee_RemoveAll(bizid)
{
	new query[200];
	format(query,sizeof(query),"DELETE FROM `car_dealer_employee` WHERE `Biz`='%d'", CarDealershipInfo[bizid][cdID]);
	mysql_tquery(sqlcon, query);
	return 1;
}

DealerEmployee_GetCount(bizid)
{
	new query[144], Cache: check, count;
	mysql_format(sqlcon, query, sizeof(query), "SELECT * FROM `car_dealer_employee` WHERE `Biz` = '%d'", CarDealershipInfo[bizid][cdID]);
	check = mysql_query(sqlcon, query);
	new result = cache_num_rows();
	if(result) {
		for(new i; i != result; i++) {
			count++;
		}
	}
	cache_delete(check);
	return count;
}

DealerEmployee_Add(playerid, bizid)
{
	new str[128];
	format(str, sizeof(str), "INSERT INTO `car_dealer_employee` SET `Name`='%s', `Biz`='%d', `Time`=UNIX_TIMESTAMP()", PlayerData[playerid][pName], CarDealershipInfo[bizid][cdID]);
	mysql_tquery(sqlcon, str);
	return 1;
}

Dialog:ShowDealer_Employee(playerid, response, listitem, inputtext[]) {
    static
        bizid = -1;

    if((bizid = GetPVarInt(playerid, "select_dealer")) != -1 && IsPlayerOwnerOfCDEx(playerid, bizid))
    {
		if (response) {
			switch(listitem)
			{
				case 0:
                {
					Dialog_Show(playerid, Dealer_AddEmployee, DIALOG_STYLE_INPUT, "Add Employe", "Masukkan nama ataupun id player untuk kamu masukkan sebagai -\npekerja di bisnis ini.", "Add", "Close");
				}
				case 1:
                {
					Dealer_ShowEmployee(playerid, bizid, 1);
				}
				case 2: 
                {
					DealerEmployee_RemoveAll(bizid);
					SendCustomMessage(playerid, "BUSINESS", "You've remove all employe on this business.");
				}
				case 3: Dealer_ShowEmployee(playerid, bizid, 0);
			}
		} else cmd_dm(playerid, "\1");
	}
    return 1;
}

Dialog:Dealer_RemoveEmployee(playerid, response, listitem, inputtext[]) {
    if (response) {
        DealerEmployee_Remove(strval(inputtext));
		SendCustomMessage(playerid, "BUSINESS", "You've remove list employee number #%d from your business.", strval(inputtext));
    }
    return 1;
}

Dialog:Dealer_AddEmployee(playerid, response, listitem, inputtext[]) {
    if(response)
	{
		new id,
			bizid = GetPVarInt(playerid, "select_dealer");

		if (DealerEmployee_GetCount(bizid) >= 3) return SendErrorMessage(playerid, "This business is limited 3 employee.");
		if (sscanf(inputtext, "u", id)) return Dialog_Show(playerid, Dealer_AddEmployee, DIALOG_STYLE_INPUT, "Add Employe", "Masukkan nama ataupun id player untuk kamu masukkan sebagai -\npekerja di bisnis ini.", "Add", "Close");
		
        if (!IsPlayerConnected(id)) return Dialog_Show(playerid, Dealer_AddEmployee, DIALOG_STYLE_INPUT, "Add Employe", "Player yang kamu masukkan tidak terkoneksi dalam game\nMasukkan nama ataupun id player untuk kamu masukkan sebagai -\npekerja di bisnis ini.", "Add", "Close");
        if (id == playerid) return Dialog_Show(playerid, Dealer_AddEmployee, DIALOG_STYLE_INPUT, "Add Employe", "Tidak dapat memasukkan karakter kamu sendiri\nMasukkan nama ataupun id player untuk kamu masukkan sebagai -\npekerja di bisnis ini.", "Add", "Close");
		if (Dealership_IsEmployee(id, bizid)) return Dialog_Show(playerid, Dealer_AddEmployee, DIALOG_STYLE_INPUT, "Add Employe", "Player tersebut sudah masuk dalam list employee\nMasukkan nama ataupun id player untuk kamu masukkan sebagai -\npekerja di bisnis ini.", "Add", "Close");

		DealerEmployee_Add(id, bizid);
		SendCustomMessage(playerid, "BUSINESS", "Kamu memasukkan "YELLOW_E"%s "WHITE_E"ke dalam list pekerja bisnismu.", PlayerData[id][pName]);
		SendCustomMessage(id, "BUSINESS", YELLOW_E"%s "WHITE_E"mempekerjakan kamu di bisnis miliknya (Bisnis: %s).", PlayerData[playerid][pName], CarDealershipInfo[bizid][cdMessage]);
	}
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    new String[500], string[512], count = Vehicle_Count(playerid), found = false, dealerid = GetPVarInt(playerid, "select_dealer");
    if(dialogid == DIALOG_CDEDIT)
	{ 
		if(response && dealerid != -1)
		{
            if(listitem == 0) // My Vehicles
			{
				format(string,sizeof(string),"Vehicle\tCost\n");
				for (new i = 0; i < 10; i ++)
				{
					if(CarDealershipInfo[dealerid][cdVehicleModel][i] != 0)
					{
						format(string,sizeof(string),"%s{00FFFF}%s\t"GREEN_E"$%s\n", string, GetVehicleName(CarDealershipInfo[dealerid][cdVehicleId][i]),FormatNumber(CarDealershipInfo[dealerid][cdVehicleCost][i]));
					}
					else
					{
						format(string, sizeof(string), "%sSpawn\n", string);
					}
					gListedItems[playerid][found] = i;
					found++;
				}
				if(found) 
				{
					ShowPlayerDialog(playerid, DIALOG_CDEDITCARS, DIALOG_STYLE_TABLIST_HEADERS, "Car Dealership:", string, "Edit", "Back");
				}
			}
			else if(listitem == 1) // Till
			{
				new listitems[] = "1. Withdraw\n2. Deposit";
			    ShowPlayerDialog(playerid,DIALOG_CDTILL,DIALOG_STYLE_LIST,"Choose an item to continue", listitems,"Select","Cancel");
			}
			else if(listitem == 2) // change message/name
			{
				ShowPlayerDialog(playerid, DIALOG_CDNAME, DIALOG_STYLE_INPUT, "Car Dealership:", " Choose the new name:", "Edit", "Back");
			}
			else if(listitem == 3) // employee menu
			{
				Dialog_Show(playerid, ShowDealer_Employee, DIALOG_STYLE_LIST, "Employees Management", "Add Employee\nRemove Employee\nRemove All Employees\nEmployees Members", "Select", "Close");
			}
		}
		else
		{
			SavecDealership(dealerid);
            SetPVarInt(playerid, "select_dealer", -1);
		}
	}
	else if(dialogid == DIALOG_CDTILL)
	{
	    if(response && GetPVarInt(playerid, "select_dealer") != -1)
		{
            if(listitem == 0) // Withdraw
			{
				format(String, sizeof(String), "{FFFFFF}You have {00FF00}$%d {FFFFFF}in your till account.\nHow much money to withdraw?", CarDealershipInfo[GetPVarInt(playerid, "select_dealer")][cdTill]);
				ShowPlayerDialog(playerid,DIALOG_CDWITHDRAW,DIALOG_STYLE_INPUT,"Withdraw", String,"Select","Cancel");
			}
			else if(listitem == 1) // Deposit
			{
				format(String, sizeof(String), "{FFFFFF}You have {00FF00}$%d {FFFFFF}in your till account.\nHow much money to deposit?", CarDealershipInfo[GetPVarInt(playerid, "select_dealer")][cdTill]);
				ShowPlayerDialog(playerid,DIALOG_CDDEPOSIT,DIALOG_STYLE_INPUT,"Deposit", String,"Select","Cancel");
			}
		}
		else
		{
            SavecDealership(GetPVarInt(playerid, "select_dealer"));
            SetPVarInt(playerid, "select_dealer", -1);
		}
	}
	else if(dialogid == DIALOG_CDWITHDRAW)
	{
		if(response && GetPVarInt(playerid, "select_dealer") != -1)
		{
		    if(IsNumeric(inputtext))
	        {
	             new money = strval(inputtext);
	             if(CarDealershipInfo[GetPVarInt(playerid, "select_dealer")][cdTill] < money)
	             {
                     format(String, sizeof(String), "You don't have that much in your till!\n\nYou have $%d in your till account.\n\nHow much money to withdraw?", CarDealershipInfo[GetPVarInt(playerid, "select_dealer")][cdTill]);
				     ShowPlayerDialog(playerid,DIALOG_CDWITHDRAW,DIALOG_STYLE_INPUT,"Withdraw", String,"Select","Cancel");
                     return 1;
	             }
	             CarDealershipInfo[GetPVarInt(playerid, "select_dealer")][cdTill] -= money;
	             GiveMoney(playerid, money);
	             format(String, sizeof(String), "You have successfully withdrawn $%d from your till, new balance: $%d", money, CarDealershipInfo[GetPVarInt(playerid, "select_dealer")][cdTill]);
	             SendClientMessageEx(playerid, COLOR_GRAD2, String);
	             SavecDealership(GetPVarInt(playerid, "select_dealer"));
                 SetPVarInt(playerid, "select_dealer", -1);
			}
        }
		else
		{
             SavecDealership(GetPVarInt(playerid, "select_dealer"));
             SetPVarInt(playerid, "select_dealer", -1);
		}
	}
	else if(dialogid == DIALOG_CDDEPOSIT)
	{ 
		if(response && GetPVarInt(playerid, "select_dealer") != -1)
		{
		    if(IsNumeric(inputtext))
	        {
	             new money = strval(inputtext);
	             if(GetMoney(playerid) < money)
	             {
                     format(String, sizeof(String), "You don't have that much in your wallet!\n\nYou have $%d in your till account.\n\n\tHow much money to deposit?", CarDealershipInfo[GetPVarInt(playerid, "select_dealer")][cdTill]);
				     ShowPlayerDialog(playerid,DIALOG_CDDEPOSIT,DIALOG_STYLE_INPUT,"Deposit", String,"Select","Cancel");
                     return 1;
	             }
	             CarDealershipInfo[GetPVarInt(playerid, "select_dealer")][cdTill] += money;
	             GiveMoney(playerid, -money);
	             format(String, sizeof(String), "You have successfully deposited $%d to your till, new balance: $%d", money, CarDealershipInfo[GetPVarInt(playerid, "select_dealer")][cdTill]);
	             SendClientMessageEx(playerid, COLOR_GRAD2, String);
	             SavecDealership(GetPVarInt(playerid, "select_dealer"));
                 SetPVarInt(playerid, "select_dealer", -1);
			}
		}
		else
		{
             SavecDealership(GetPVarInt(playerid, "select_dealer"));
             SetPVarInt(playerid, "select_dealer", -1);
		}
	}
	else if(dialogid == DIALOG_CDNAME)
	{ 
		if(response && GetPVarInt(playerid, "select_dealer") != -1)
		{
			new d = GetPVarInt(playerid, "select_dealer");
            if(!strlen(inputtext))
			{
			    SendClientMessageEx(playerid, COLOR_LIGHTRED, "** You must type a name **");
    	        ShowPlayerDialog(playerid, DIALOG_CDNAME, DIALOG_STYLE_INPUT, "Car Dealership:", " Choose the new name:", "Edit", "Back");
		        return 1;
			}
			strmid(CarDealershipInfo[d][cdMessage], inputtext, 0, strlen(inputtext), 255);
	        format(String,128,"{00FFFF}[id:%d]\n{FFFF00}%s\n{00FF00}Owner: %s",d, CarDealershipInfo[d][cdMessage], CarDealershipInfo[d][cdOwner]);
	        UpdateDynamic3DTextLabelText(CarDealershipInfo[d][cdTextLabel], COLOR_TWAQUA, String);
	        SavecDealership(GetPVarInt(playerid, "select_dealer"));
		}
	}
	else if(dialogid == DIALOG_CDEDITCARS)
	{ 
		if(response && GetPVarInt(playerid, "select_dealer") != -1)
		{
			if(CarDealershipInfo[GetPVarInt(playerid, "select_dealer")][cdVehicleModel][gListedItems[playerid][listitem]] == 0)
			{
				PlayerData[playerid][pListitem] = gListedItems[playerid][listitem];
				new listitems[] = "Motorcycle\nSUV\nPickup Vehicles\nLowriders\nSaloons\nSport";
				ShowPlayerDialog(playerid,DIALOG_CDNEWVEH,DIALOG_STYLE_LIST,"Car Dealership:", listitems,"Select","Cancel");
			}
		}
		else
		{
		    new listitems[] = "Vehicles\nDealership - Safes\nChange Name Dealership";
			ShowPlayerDialog(playerid,DIALOG_CDEDIT,DIALOG_STYLE_LIST,"Choose an item to continue", listitems,"Select","Cancel");
		}
	}
	
	else if(dialogid == DIALOG_CDEDITPARK)
	{ // car dealership dialog
		if(response && GetPVarInt(playerid, "select_dealer") != -1 && PlayerData[playerid][pListitem] != -1 &&  GetPVarInt(playerid, "select_dealervehpos") == 1 || GetPVarInt(playerid, "select_dealervehnew"))
		{
			new Float: x, Float: y, Float: z, Float: a;
			GetPlayerPos(playerid,x,y,z);
	        GetPlayerFacingAngle(playerid, a);
			new d = GetPVarInt(playerid, "select_dealer"), v = PlayerData[playerid][pListitem];
			
			if(IsPlayerInRangeOfPoint(playerid, CarDealershipInfo[d][cdRadius], CarDealershipInfo[d][cdEntranceX], CarDealershipInfo[d][cdEntranceY], CarDealershipInfo[d][cdEntranceZ]))
		    {
				CarDealershipInfo[d][cdVehicleSpawnX][v] = x;
				CarDealershipInfo[d][cdVehicleSpawnY][v] = y;
				CarDealershipInfo[d][cdVehicleSpawnZ][v] = z;
				CarDealershipInfo[d][cdVehicleSpawnAngle][v] = a;
				SetPVarInt(playerid, "select_dealervehpos", 0);
				SetPVarInt(playerid, "select_dealervehnew", 0);
				CreateDealershipPickup(d);

				TogglePlayerControllable(playerid, true);
				SavecDealership(d);
			}
			else
			{
				SendClientMessageEx(playerid, COLOR_GREY, "ERROR: You are out of this Car Dealership radius, please try again.");
				TogglePlayerControllable(playerid, true);
			}
		}
		else if(response && GetPVarInt(playerid, "select_dealer") != -1 && GetPVarInt(playerid, "select_dealervehpos") == 2)
		{
			new Float: x, Float: y, Float: z, Float: a;
			new d;
			GetPlayerPos(playerid,x,y,z);
	        GetPlayerFacingAngle(playerid, a);
			d = GetPVarInt(playerid, "select_dealer");
			if(IsPlayerInRangeOfPoint(playerid, CarDealershipInfo[d][cdRadius], CarDealershipInfo[d][cdEntranceX], CarDealershipInfo[d][cdEntranceY], CarDealershipInfo[d][cdEntranceZ]))
		    {
				CarDealershipInfo[d][cdVehicleSpawn][0] = x;
				CarDealershipInfo[d][cdVehicleSpawn][1] = y;
				CarDealershipInfo[d][cdVehicleSpawn][2] = z;
				CarDealershipInfo[d][cdVehicleSpawn][3] = a;
				SetPVarInt(playerid, "select_dealervehpos", 0);
				TogglePlayerControllable(playerid, true);
				SavecDealership(d);
			}
			else
			{
				SendClientMessageEx(playerid, COLOR_GREY, "ERROR: You are out of this Car Dealership radius, please try again.");
				TogglePlayerControllable(playerid, true);
			}
		}
		else if(!response && GetPVarInt(playerid, "select_dealer") != -1 && GetPVarInt(playerid, "select_dealervehpos") == 2)
		{
            TogglePlayerControllable(playerid, true);
			SetPVarInt(playerid, "select_dealervehpos", 0);
		}
	}
	else if(dialogid == DIALOG_CDNEWVEH)
	{ // car dealership dialog
		if(response && GetPVarInt(playerid, "select_dealer") != -1)
		{
			if(listitem == 0) // Motorcycle
			{
				ShowPlayerSelectionMenu(playerid, MODEL_SELECTION_Motorcycle, "Dealership Vehicle:", dmvehicle1, sizeof(dmvehicle1));
			}	
			else if(listitem == 1) // SUV
			{
				ShowPlayerSelectionMenu(playerid, MODEL_SELECTION_SUV, "Dealership Vehicle:", dmvehicle2, sizeof(dmvehicle2));
			}	
			else if(listitem == 2) // Pickup Vehicles
			{
				ShowPlayerSelectionMenu(playerid, MODEL_SELECTION_PickupVehicles, "Dealership Vehicle:", dmvehicle3, sizeof(dmvehicle3));
			}	
			else if(listitem == 3) // Lowriders
			{
				ShowPlayerSelectionMenu(playerid, MODEL_SELECTION_Lowriders, "Dealership Vehicle:", dmvehicle4, sizeof(dmvehicle4));
			}	
			else if(listitem == 4) // Saloons
			{
				ShowPlayerSelectionMenu(playerid, MODEL_SELECTION_Saloons, "Dealership Vehicle:", dmvehicle5, sizeof(dmvehicle5));
			}	
			else if(listitem == 5) // Sport
			{
				ShowPlayerSelectionMenu(playerid, MODEL_SELECTION_Sport, "Dealership Vehicle:", dmvehicle6, sizeof(dmvehicle6));
			}	
	        else
	        {
				SendClientMessageEx(playerid, COLOR_GREY, "ERROR: You are out of this Car Dealership radius, please try again.");
				TogglePlayerControllable(playerid, true);
	        }
		}
		else
		{
            TogglePlayerControllable(playerid, true);
			SetPVarInt(playerid, "select_dealer", -1);
		}
	}
	else if(dialogid == DIALOG_CDBUY)
	{
	    // Account Eating Bug Fix
	    if(!IsPlayerInAnyVehicle(playerid))
		{
		    TogglePlayerControllable(playerid, 1);
			SendClientMessageEx(playerid,COLOR_GRAD2,"You need to be in the vehicle you wish to purchase.");
			return 1;
		}

		new vehicleid = GetPlayerVehicleID(playerid);
		new v = GetCarDealershipVehicleId(vehicleid);
		new d = GetCarDealershipId(vehicleid);
		if(response)
		{
            if(CarDealershipInfo[d][cdVehicleSpawn][0] == 0.0 && CarDealershipInfo[d][cdVehicleSpawn][1] == 0.0 && CarDealershipInfo[d][cdVehicleSpawn][2] == 0.0)
            {
				SendClientMessageEx(playerid, COLOR_GRAD1, "ERROR: The owner of this Car Dealership hasn't set the purchased vehicles spawn point.");
				RemovePlayerFromVehicle(playerid);
				new Float:slx, Float:sly, Float:slz;
				GetPlayerPos(playerid, slx, sly, slz);
				SetPlayerPos(playerid, slx, sly, slz+1.2);
				TogglePlayerControllable(playerid, 1);
				return 1;
            }
            new limit = 3;
			if(count >= limit)
			{
				SendClientMessageEx(playerid, COLOR_GREY, "ERROR: Slot kendaraan anda penuh");
                RemovePlayerFromVehicle(playerid);
                new Float:slx, Float:sly, Float:slz;
				GetPlayerPos(playerid, slx, sly, slz);
				SetPlayerPos(playerid, slx, sly, slz+1.2);
                TogglePlayerControllable(playerid, 1);
				return 1;
			}
		    new randcolor1 = RandomEx(0, 126);
		    new randcolor2 = RandomEx(0, 126);
		    SetPlayerPos(playerid, CarDealershipInfo[d][cdVehicleSpawn][0], CarDealershipInfo[d][cdVehicleSpawn][1], CarDealershipInfo[d][cdVehicleSpawn][2]+2);
		    TogglePlayerControllable(playerid, 1);
			
	        if(GetMoney(playerid) < CarDealershipInfo[d][cdVehicleCost][v])
	        {
				SendClientMessageEx(playerid, COLOR_GRAD1, "ERROR: You don't have enough money to buy this.");
				RemovePlayerFromVehicle(playerid);
				return 1;
			}
            GiveMoney(playerid, -CarDealershipInfo[d][cdVehicleCost][v]);
		    CarDealershipInfo[d][cdTill] += ((CarDealershipInfo[d][cdVehicleCost][v]*5)/100)+CarDealershipInfo[d][cdVehicleCost][v];
      		
			Vehicle_Create(PlayerData[playerid][pID], CarDealershipInfo[d][cdVehicleModel][v], CarDealershipInfo[d][cdVehicleSpawn][0], CarDealershipInfo[d][cdVehicleSpawn][1], CarDealershipInfo[d][cdVehicleSpawn][2], CarDealershipInfo[d][cdVehicleSpawn][3], randcolor1, randcolor2);
            //Plate
			DestroyVehicle(CarDealershipInfo[d][cdVehicleId][v]);
			DestroyDynamic3DTextLabel(CarDealershipInfo[d][cdVehicleLabel][v]);
			CarDealershipInfo[d][cdVehicleId][v] = INVALID_VEHICLE_ID;

			CarDealershipInfo[d][cdVehicleModel][v] = 0;
    		CarDealershipInfo[d][cdVehicleCost][v] = 0;
			format(String, sizeof(String), "[slot:%d]\n"GREEN_E"No vehicle to preview\n"WHITE_E"Use '"YELLOW_E"/dm > Vehicles"WHITE_E"' to display vehicle here",v);
			CarDealershipInfo[d][cdVehicleLabel][v] = CreateDynamic3DTextLabel(String,COLOR_ARWIN,CarDealershipInfo[d][cdVehicleSpawnX][v], CarDealershipInfo[d][cdVehicleSpawnY][v], CarDealershipInfo[d][cdVehicleSpawnZ][v],8.0,INVALID_PLAYER_ID,INVALID_VEHICLE_ID);
            SavecDealership(d);
		}
		else
		{
            RemovePlayerFromVehicle(playerid);
            TogglePlayerControllable(playerid, 1);
			return 1;
		}
	}
	else if(dialogid == DIALOG_CDSELL)
	{
		if(response)
		{
			if(GetPVarInt(playerid, "select_dealer") == -1) return 1;
            GiveMoney(playerid, CarDealershipInfo[GetPVarInt(playerid, "select_dealer")][cdPrice]);
			SellCarDealership(GetPVarInt(playerid, "select_dealer"));
			format(String, sizeof(String), "Car Dealership successfully sold for %s!", FormatNumber(CarDealershipInfo[GetPVarInt(playerid, "select_dealer")][cdPrice]));
			SendClientMessageEx(playerid, COLOR_WHITE, String);
		}
		else
		{
            SetPVarInt(playerid, "select_dealer", -1);
			return 1;
		}
	}
    return 0;
}

public OnPlayerSelectionMenuResponse(playerid, extraid, response, listitem, modelid)
{
	switch(extraid)
	{
    	case MODEL_SELECTION_Motorcycle:
	    {
	        if(response)
	        {
                new Float: x, Float: y, Float: z, Float: a, harga, String[212];
                new d = GetPVarInt(playerid, "select_dealer");
                GetPlayerPos(playerid,x,y,z);
                GetPlayerFacingAngle(playerid, a);
                if(CarDealershipInfo[d][cdTill] >= GetVehicleDealerCost(modelid))
                {
                    harga = GetVehicleDealerCost(modelid);
                }
                else
                {
                    SendClientMessageEx(playerid, COLOR_GRAD2, "Anda harus mempunyai modal sebesar "GREEN_E"$%s", FormatNumber(GetVehicleDealerCost(modelid)));
                    return 1;
                }
                if(modelid < 400||modelid > 611||modelid == 411||modelid == 415||modelid == 424||modelid == 434||modelid == 451||modelid == 494||modelid == 502||modelid == 503||modelid == 495||modelid == 506||modelid == 503)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "ID Kendaraan tidak diperbolehkan!");
                    return 1;
                }
                new cdvehicleid = PlayerData[playerid][pListitem];
                DestroyDynamic3DTextLabel(CarDealershipInfo[d][cdVehicleLabel][cdvehicleid]);
                if(CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid] == 0.0 && CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid] == 0.0)
                {
                    new vehicleid = CreateVehicle(modelid, x, y, z, a, -1, -1, 6);
                    CarDealershipInfo[d][cdVehicleId][cdvehicleid] = vehicleid;
                    CarDealershipInfo[d][cdVehicleModel][cdvehicleid] = modelid;
                    CarDealershipInfo[d][cdVehicleCost][cdvehicleid] = harga;
                    CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid] = x;
                    CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid] = y;
                    CarDealershipInfo[d][cdVehicleSpawnZ][cdvehicleid] = z;
                    CarDealershipInfo[d][cdVehicleSpawnAngle][cdvehicleid] = a;
                    PutPlayerInVehicle(playerid, vehicleid, 0);
                    format(String, sizeof(String), "[slot:%d]\n{00FFFF}%s\n"WHITE_E"Price: "GREEN_E"$%s\n"WHITE_E"Fuel Capacity: "YELLOW_E"100.0\n"WHITE_E"Fuel Total: "YELLOW_E"100.0",cdvehicleid, GetVehicleName(vehicleid), FormatNumber(CarDealershipInfo[d][cdVehicleCost][cdvehicleid]));
                    CarDealershipInfo[d][cdVehicleLabel][cdvehicleid] = CreateDynamic3DTextLabel(String,COLOR_ARWIN,0.0, 0.0, 0.0,5.0,INVALID_PLAYER_ID,vehicleid,1);
                }
                else
                {
                    new vehicleid = CreateVehicle(modelid, CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnZ][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnAngle][cdvehicleid], -1, -1, 6);
                    CarDealershipInfo[d][cdVehicleId][cdvehicleid] = vehicleid;
                    CarDealershipInfo[d][cdVehicleModel][cdvehicleid] = modelid;
                    CarDealershipInfo[d][cdVehicleCost][cdvehicleid] = harga;
                    
                    format(String, sizeof(String), "[slot:%d]\n{00FFFF}%s\n"WHITE_E"Price: "GREEN_E"$%s\n"WHITE_E"Fuel Capacity: "YELLOW_E"100.0\n"WHITE_E"Fuel Total: "YELLOW_E"100.0",cdvehicleid, GetVehicleName(vehicleid), FormatNumber(CarDealershipInfo[d][cdVehicleCost][cdvehicleid]));
                    CarDealershipInfo[d][cdVehicleLabel][cdvehicleid] = CreateDynamic3DTextLabel(String,COLOR_ARWIN,0.0, 0.0, 0.0,5.0,INVALID_PLAYER_ID,vehicleid,1);
                }
                SendClientMessageEx(playerid, COLOR_YELLOW, "You have created Car Dealership ID %d for Dealership ID %d to your position.", cdvehicleid, d);
                SavecDealership(d);
                CarDealershipInfo[d][cdTill] -= harga;
            }
        }   
        case MODEL_SELECTION_SUV:
	    {
	        if(response)
	        {
                new Float: x, Float: y, Float: z, Float: a, harga, String[212];
                new d = GetPVarInt(playerid, "select_dealer");
                GetPlayerPos(playerid,x,y,z);
                GetPlayerFacingAngle(playerid, a);
                if(CarDealershipInfo[d][cdTill] >= GetVehicleDealerCost(modelid))
                {
                    harga = GetVehicleDealerCost(modelid);
                }
                else
                {
                    SendClientMessageEx(playerid, COLOR_GRAD2, "Anda harus mempunyai modal sebesar "GREEN_E"$%s", FormatNumber(GetVehicleDealerCost(modelid)));
                    return 1;
                }
                if(modelid < 400||modelid > 611||modelid == 411||modelid == 415||modelid == 424||modelid == 434||modelid == 451||modelid == 494||modelid == 502||modelid == 503||modelid == 495||modelid == 506||modelid == 503)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "ID Kendaraan tidak diperbolehkan!");
                    return 1;
                }
                new cdvehicleid = PlayerData[playerid][pListitem];
                DestroyDynamic3DTextLabel(CarDealershipInfo[d][cdVehicleLabel][cdvehicleid]);
                if(CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid] == 0.0 && CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid] == 0.0)
                {
                    new vehicleid = CreateVehicle(modelid, x, y, z, a, -1, -1, 6);
                    CarDealershipInfo[d][cdVehicleId][cdvehicleid] = vehicleid;
                    CarDealershipInfo[d][cdVehicleModel][cdvehicleid] = modelid;
                    CarDealershipInfo[d][cdVehicleCost][cdvehicleid] = harga;
                    CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid] = x;
                    CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid] = y;
                    CarDealershipInfo[d][cdVehicleSpawnZ][cdvehicleid] = z;
                    CarDealershipInfo[d][cdVehicleSpawnAngle][cdvehicleid] = a;
                    PutPlayerInVehicle(playerid, vehicleid, 0);
                    format(String, sizeof(String), "[slot:%d]\n{00FFFF}%s\n"WHITE_E"Price: "GREEN_E"$%s\n"WHITE_E"Fuel Capacity: "YELLOW_E"100.0\n"WHITE_E"Fuel Total: "YELLOW_E"100.0",cdvehicleid, GetVehicleName(vehicleid), FormatNumber(CarDealershipInfo[d][cdVehicleCost][cdvehicleid]));
                    CarDealershipInfo[d][cdVehicleLabel][cdvehicleid] = CreateDynamic3DTextLabel(String,COLOR_ARWIN,0.0, 0.0, 0.0,5.0,INVALID_PLAYER_ID,vehicleid,1);
                }
                else
                {
                    new vehicleid = CreateVehicle(modelid, CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnZ][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnAngle][cdvehicleid], -1, -1, 6);
                    CarDealershipInfo[d][cdVehicleId][cdvehicleid] = vehicleid;
                    CarDealershipInfo[d][cdVehicleModel][cdvehicleid] = modelid;
                    CarDealershipInfo[d][cdVehicleCost][cdvehicleid] = harga;
                    
                    format(String, sizeof(String), "[slot:%d]\n{00FFFF}%s\n"WHITE_E"Price: "GREEN_E"$%s\n"WHITE_E"Fuel Capacity: "YELLOW_E"100.0\n"WHITE_E"Fuel Total: "YELLOW_E"100.0",cdvehicleid, GetVehicleName(vehicleid), FormatNumber(CarDealershipInfo[d][cdVehicleCost][cdvehicleid]));
                    CarDealershipInfo[d][cdVehicleLabel][cdvehicleid] = CreateDynamic3DTextLabel(String,COLOR_ARWIN,0.0, 0.0, 0.0,5.0,INVALID_PLAYER_ID,vehicleid,1);
                }
                SendClientMessageEx(playerid, COLOR_YELLOW, "You have created Car Dealership ID %d for Dealership ID %d to your position.", cdvehicleid, d);
                SavecDealership(d);
                CarDealershipInfo[d][cdTill] -= harga;
            }
        }  
        case MODEL_SELECTION_PickupVehicles:
	    {
	        if(response)
	        {
                new Float: x, Float: y, Float: z, Float: a, harga, String[212];
                new d = GetPVarInt(playerid, "select_dealer");
                GetPlayerPos(playerid,x,y,z);
                GetPlayerFacingAngle(playerid, a);
                if(CarDealershipInfo[d][cdTill] >= GetVehicleDealerCost(modelid))
                {
                    harga = GetVehicleDealerCost(modelid);
                }
                else
                {
                    SendClientMessageEx(playerid, COLOR_GRAD2, "Anda harus mempunyai modal sebesar "GREEN_E"$%s", FormatNumber(GetVehicleDealerCost(modelid)));
                    return 1;
                }
                if(modelid < 400||modelid > 611||modelid == 411||modelid == 415||modelid == 424||modelid == 434||modelid == 451||modelid == 494||modelid == 502||modelid == 503||modelid == 495||modelid == 506||modelid == 503)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "ID Kendaraan tidak diperbolehkan!");
                    return 1;
                }
                new cdvehicleid = PlayerData[playerid][pListitem];
                DestroyDynamic3DTextLabel(CarDealershipInfo[d][cdVehicleLabel][cdvehicleid]);
                if(CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid] == 0.0 && CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid] == 0.0)
                {
                    new vehicleid = CreateVehicle(modelid, x, y, z, a, -1, -1, 6);
                    CarDealershipInfo[d][cdVehicleId][cdvehicleid] = vehicleid;
                    CarDealershipInfo[d][cdVehicleModel][cdvehicleid] = modelid;
                    CarDealershipInfo[d][cdVehicleCost][cdvehicleid] = harga;
                    CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid] = x;
                    CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid] = y;
                    CarDealershipInfo[d][cdVehicleSpawnZ][cdvehicleid] = z;
                    CarDealershipInfo[d][cdVehicleSpawnAngle][cdvehicleid] = a;
                    PutPlayerInVehicle(playerid, vehicleid, 0);
                    format(String, sizeof(String), "[slot:%d]\n{00FFFF}%s\n"WHITE_E"Price: "GREEN_E"$%s\n"WHITE_E"Fuel Capacity: "YELLOW_E"100.0\n"WHITE_E"Fuel Total: "YELLOW_E"100.0",cdvehicleid, GetVehicleName(vehicleid), FormatNumber(CarDealershipInfo[d][cdVehicleCost][cdvehicleid]));
                    CarDealershipInfo[d][cdVehicleLabel][cdvehicleid] = CreateDynamic3DTextLabel(String,COLOR_ARWIN,0.0, 0.0, 0.0,5.0,INVALID_PLAYER_ID,vehicleid,1);
                }
                else
                {
                    new vehicleid = CreateVehicle(modelid, CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnZ][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnAngle][cdvehicleid], -1, -1, 6);
                    CarDealershipInfo[d][cdVehicleId][cdvehicleid] = vehicleid;
                    CarDealershipInfo[d][cdVehicleModel][cdvehicleid] = modelid;
                    CarDealershipInfo[d][cdVehicleCost][cdvehicleid] = harga;
                    
                    format(String, sizeof(String), "[slot:%d]\n{00FFFF}%s\n"WHITE_E"Price: "GREEN_E"$%s\n"WHITE_E"Fuel Capacity: "YELLOW_E"100.0\n"WHITE_E"Fuel Total: "YELLOW_E"100.0",cdvehicleid, GetVehicleName(vehicleid), FormatNumber(CarDealershipInfo[d][cdVehicleCost][cdvehicleid]));
                    CarDealershipInfo[d][cdVehicleLabel][cdvehicleid] = CreateDynamic3DTextLabel(String,COLOR_ARWIN,0.0, 0.0, 0.0,5.0,INVALID_PLAYER_ID,vehicleid,1);
                }
                SendClientMessageEx(playerid, COLOR_YELLOW, "You have created Car Dealership ID %d for Dealership ID %d to your position.", cdvehicleid, d);
                SavecDealership(d);
                CarDealershipInfo[d][cdTill] -= harga;
            }
        }  
        case MODEL_SELECTION_Lowriders:
	    {
	        if(response)
	        {
                new Float: x, Float: y, Float: z, Float: a, harga, String[212];
                new d = GetPVarInt(playerid, "select_dealer");
                GetPlayerPos(playerid,x,y,z);
                GetPlayerFacingAngle(playerid, a);
                if(CarDealershipInfo[d][cdTill] >= GetVehicleDealerCost(modelid))
                {
                    harga = GetVehicleDealerCost(modelid);
                }
                else
                {
                    SendClientMessageEx(playerid, COLOR_GRAD2, "Anda harus mempunyai modal sebesar "GREEN_E"$%s", FormatNumber(GetVehicleDealerCost(modelid)));
                    return 1;
                }
                if(modelid < 400||modelid > 611||modelid == 411||modelid == 415||modelid == 424||modelid == 434||modelid == 451||modelid == 494||modelid == 502||modelid == 503||modelid == 495||modelid == 506||modelid == 503)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "ID Kendaraan tidak diperbolehkan!");
                    return 1;
                }
                new cdvehicleid = PlayerData[playerid][pListitem];
                DestroyDynamic3DTextLabel(CarDealershipInfo[d][cdVehicleLabel][cdvehicleid]);
                if(CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid] == 0.0 && CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid] == 0.0)
                {
                    new vehicleid = CreateVehicle(modelid, x, y, z, a, -1, -1, 6);
                    CarDealershipInfo[d][cdVehicleId][cdvehicleid] = vehicleid;
                    CarDealershipInfo[d][cdVehicleModel][cdvehicleid] = modelid;
                    CarDealershipInfo[d][cdVehicleCost][cdvehicleid] = harga;
                    CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid] = x;
                    CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid] = y;
                    CarDealershipInfo[d][cdVehicleSpawnZ][cdvehicleid] = z;
                    CarDealershipInfo[d][cdVehicleSpawnAngle][cdvehicleid] = a;
                    PutPlayerInVehicle(playerid, vehicleid, 0);
                    format(String, sizeof(String), "[slot:%d]\n{00FFFF}%s\n"WHITE_E"Price: "GREEN_E"$%s\n"WHITE_E"Fuel Capacity: "YELLOW_E"100.0\n"WHITE_E"Fuel Total: "YELLOW_E"100.0",cdvehicleid, GetVehicleName(vehicleid), FormatNumber(CarDealershipInfo[d][cdVehicleCost][cdvehicleid]));
                    CarDealershipInfo[d][cdVehicleLabel][cdvehicleid] = CreateDynamic3DTextLabel(String,COLOR_ARWIN,0.0, 0.0, 0.0,5.0,INVALID_PLAYER_ID,vehicleid,1);
                }
                else
                {
                    new vehicleid = CreateVehicle(modelid, CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnZ][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnAngle][cdvehicleid], -1, -1, 6);
                    CarDealershipInfo[d][cdVehicleId][cdvehicleid] = vehicleid;
                    CarDealershipInfo[d][cdVehicleModel][cdvehicleid] = modelid;
                    CarDealershipInfo[d][cdVehicleCost][cdvehicleid] = harga;
                    
                    format(String, sizeof(String), "[slot:%d]\n{00FFFF}%s\n"WHITE_E"Price: "GREEN_E"$%s\n"WHITE_E"Fuel Capacity: "YELLOW_E"100.0\n"WHITE_E"Fuel Total: "YELLOW_E"100.0",cdvehicleid, GetVehicleName(vehicleid), FormatNumber(CarDealershipInfo[d][cdVehicleCost][cdvehicleid]));
                    CarDealershipInfo[d][cdVehicleLabel][cdvehicleid] = CreateDynamic3DTextLabel(String,COLOR_ARWIN,0.0, 0.0, 0.0,5.0,INVALID_PLAYER_ID,vehicleid,1);
                }
                SendClientMessageEx(playerid, COLOR_YELLOW, "You have created Car Dealership ID %d for Dealership ID %d to your position.", cdvehicleid, d);
                SavecDealership(d);
                CarDealershipInfo[d][cdTill] -= harga;
            }
        }  
        case MODEL_SELECTION_Saloons:
	    {
	        if(response)
	        {
                new Float: x, Float: y, Float: z, Float: a, harga, String[212];
                new d = GetPVarInt(playerid, "select_dealer");
                GetPlayerPos(playerid,x,y,z);
                GetPlayerFacingAngle(playerid, a);
                if(CarDealershipInfo[d][cdTill] >= GetVehicleDealerCost(modelid))
                {
                    harga = GetVehicleDealerCost(modelid);
                }
                else
                {
                    SendClientMessageEx(playerid, COLOR_GRAD2, "Anda harus mempunyai modal sebesar "GREEN_E"$%s", FormatNumber(GetVehicleDealerCost(modelid)));
                    return 1;
                }
                if(modelid < 400||modelid > 611||modelid == 411||modelid == 415||modelid == 424||modelid == 434||modelid == 451||modelid == 494||modelid == 502||modelid == 503||modelid == 495||modelid == 506||modelid == 503)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "ID Kendaraan tidak diperbolehkan!");
                    return 1;
                }
                new cdvehicleid = PlayerData[playerid][pListitem];
                DestroyDynamic3DTextLabel(CarDealershipInfo[d][cdVehicleLabel][cdvehicleid]);
                if(CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid] == 0.0 && CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid] == 0.0)
                {
                    new vehicleid = CreateVehicle(modelid, x, y, z, a, -1, -1, 6);
                    CarDealershipInfo[d][cdVehicleId][cdvehicleid] = vehicleid;
                    CarDealershipInfo[d][cdVehicleModel][cdvehicleid] = modelid;
                    CarDealershipInfo[d][cdVehicleCost][cdvehicleid] = harga;
                    CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid] = x;
                    CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid] = y;
                    CarDealershipInfo[d][cdVehicleSpawnZ][cdvehicleid] = z;
                    CarDealershipInfo[d][cdVehicleSpawnAngle][cdvehicleid] = a;
                    PutPlayerInVehicle(playerid, vehicleid, 0);
                    format(String, sizeof(String), "[slot:%d]\n{00FFFF}%s\n"WHITE_E"Price: "GREEN_E"$%s\n"WHITE_E"Fuel Capacity: "YELLOW_E"100.0\n"WHITE_E"Fuel Total: "YELLOW_E"100.0",cdvehicleid, GetVehicleName(vehicleid), FormatNumber(CarDealershipInfo[d][cdVehicleCost][cdvehicleid]));
                    CarDealershipInfo[d][cdVehicleLabel][cdvehicleid] = CreateDynamic3DTextLabel(String,COLOR_ARWIN,0.0, 0.0, 0.0,5.0,INVALID_PLAYER_ID,vehicleid,1);
                }
                else
                {
                    new vehicleid = CreateVehicle(modelid, CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnZ][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnAngle][cdvehicleid], -1, -1, 6);
                    CarDealershipInfo[d][cdVehicleId][cdvehicleid] = vehicleid;
                    CarDealershipInfo[d][cdVehicleModel][cdvehicleid] = modelid;
                    CarDealershipInfo[d][cdVehicleCost][cdvehicleid] = harga;
                    
                    format(String, sizeof(String), "[slot:%d]\n{00FFFF}%s\n"WHITE_E"Price: "GREEN_E"$%s\n"WHITE_E"Fuel Capacity: "YELLOW_E"100.0\n"WHITE_E"Fuel Total: "YELLOW_E"100.0",cdvehicleid, GetVehicleName(vehicleid), FormatNumber(CarDealershipInfo[d][cdVehicleCost][cdvehicleid]));
                    CarDealershipInfo[d][cdVehicleLabel][cdvehicleid] = CreateDynamic3DTextLabel(String,COLOR_ARWIN,0.0, 0.0, 0.0,5.0,INVALID_PLAYER_ID,vehicleid,1);
                }
                SendClientMessageEx(playerid, COLOR_YELLOW, "You have created Car Dealership ID %d for Dealership ID %d to your position.", cdvehicleid, d);
                SavecDealership(d);
                CarDealershipInfo[d][cdTill] -= harga;
            }
        }  
        case MODEL_SELECTION_Sport:
	    {
	        if(response)
	        {
                new Float: x, Float: y, Float: z, Float: a, harga, String[212];
                new d = GetPVarInt(playerid, "select_dealer");
                GetPlayerPos(playerid,x,y,z);
                GetPlayerFacingAngle(playerid, a);
                if(CarDealershipInfo[d][cdTill] >= GetVehicleDealerCost(modelid))
                {
                    harga = GetVehicleDealerCost(modelid);
                }
                else
                {
                    SendClientMessageEx(playerid, COLOR_GRAD2, "Anda harus mempunyai modal sebesar "GREEN_E"$%s", FormatNumber(GetVehicleDealerCost(modelid)));
                    return 1;
                }
                if(modelid < 400||modelid > 611||modelid == 411||modelid == 415||modelid == 424||modelid == 434||modelid == 451||modelid == 494||modelid == 502||modelid == 503||modelid == 495||modelid == 506||modelid == 503)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "ID Kendaraan tidak diperbolehkan!");
                    return 1;
                }
                new cdvehicleid = PlayerData[playerid][pListitem];
                DestroyDynamic3DTextLabel(CarDealershipInfo[d][cdVehicleLabel][cdvehicleid]);
                if(CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid] == 0.0 && CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid] == 0.0)
                {
                    new vehicleid = CreateVehicle(modelid, x, y, z, a, -1, -1, 6);
                    CarDealershipInfo[d][cdVehicleId][cdvehicleid] = vehicleid;
                    CarDealershipInfo[d][cdVehicleModel][cdvehicleid] = modelid;
                    CarDealershipInfo[d][cdVehicleCost][cdvehicleid] = harga;
                    CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid] = x;
                    CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid] = y;
                    CarDealershipInfo[d][cdVehicleSpawnZ][cdvehicleid] = z;
                    CarDealershipInfo[d][cdVehicleSpawnAngle][cdvehicleid] = a;
                    PutPlayerInVehicle(playerid, vehicleid, 0);
                    format(String, sizeof(String), "[slot:%d]\n{00FFFF}%s\n"WHITE_E"Price: "GREEN_E"$%s\n"WHITE_E"Fuel Capacity: "YELLOW_E"100.0\n"WHITE_E"Fuel Total: "YELLOW_E"100.0",cdvehicleid, GetVehicleName(vehicleid), FormatNumber(CarDealershipInfo[d][cdVehicleCost][cdvehicleid]));
                    CarDealershipInfo[d][cdVehicleLabel][cdvehicleid] = CreateDynamic3DTextLabel(String,COLOR_ARWIN,0.0, 0.0, 0.0,5.0,INVALID_PLAYER_ID,vehicleid,1);
                }
                else
                {
                    new vehicleid = CreateVehicle(modelid, CarDealershipInfo[d][cdVehicleSpawnX][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnY][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnZ][cdvehicleid], CarDealershipInfo[d][cdVehicleSpawnAngle][cdvehicleid], -1, -1, 6);
                    CarDealershipInfo[d][cdVehicleId][cdvehicleid] = vehicleid;
                    CarDealershipInfo[d][cdVehicleModel][cdvehicleid] = modelid;
                    CarDealershipInfo[d][cdVehicleCost][cdvehicleid] = harga;
                    
                    format(String, sizeof(String), "[slot:%d]\n{00FFFF}%s\n"WHITE_E"Price: "GREEN_E"$%s\n"WHITE_E"Fuel Capacity: "YELLOW_E"100.0\n"WHITE_E"Fuel Total: "YELLOW_E"100.0",cdvehicleid, GetVehicleName(vehicleid), FormatNumber(CarDealershipInfo[d][cdVehicleCost][cdvehicleid]));
                    CarDealershipInfo[d][cdVehicleLabel][cdvehicleid] = CreateDynamic3DTextLabel(String,COLOR_ARWIN,0.0, 0.0, 0.0,5.0,INVALID_PLAYER_ID,vehicleid,1);
                }
                SendClientMessageEx(playerid, COLOR_YELLOW, "You have created Car Dealership ID %d for Dealership ID %d to your position.", cdvehicleid, d);
                SavecDealership(d);
                CarDealershipInfo[d][cdTill] -= harga;
            }
        }
	}
	return 1;
}
CMD:dmvehicleposspawn(playerid, params[])
{
    if(PlayerData[playerid][pAdmin] >= 5)
	{
	    new d;
	    if(sscanf(params, "d", d))  return SendClientMessageEx(playerid, COLOR_WHITE, "USAGE: /dmvehicleposspawn [dealerid]");
	    new Float: x, Float: y, Float: z, Float: a;
		GetPlayerPos(playerid,x,y,z);
		GetPlayerFacingAngle(playerid, a);
		CarDealershipInfo[d][cdVehicleSpawn][0] = x;
		CarDealershipInfo[d][cdVehicleSpawn][1] = y;
		CarDealershipInfo[d][cdVehicleSpawn][2] = z;
		CarDealershipInfo[d][cdVehicleSpawn][3] = a;
		SavecDealership(d);
		SendClientMessageEx(playerid, COLOR_ARWIN, "DEALERSHIP: "YELLOW_E"Succesfull set vehicle pos spawn "YELLOW_E"dealership id {00FFFF}%d", d);
	}
	else
	{
        SendClientMessageEx(playerid, COLOR_GREY, " You are not allowed to use this command.");
	}
    return 1;
}
	
CMD:dmvehiclepos(playerid, params[])
{
    if(PlayerData[playerid][pAdmin] >= 5)
	{
	    new d, v;
	    if(sscanf(params, "dd", d, v)) 
		{
			SendClientMessageEx(playerid, COLOR_WHITE, "USAGE: /dmvehiclepos [dealerid] [slot]");
			return 1;
		}
	    new Float: x, Float: y, Float: z, Float: a;
		GetPlayerPos(playerid,x,y,z);
		GetPlayerFacingAngle(playerid, a);
		CarDealershipInfo[d][cdVehicleSpawnX][v] = x;
		CarDealershipInfo[d][cdVehicleSpawnY][v] = y;
		CarDealershipInfo[d][cdVehicleSpawnZ][v] = z;
		CarDealershipInfo[d][cdVehicleSpawnAngle][v] = a;
		SetPVarInt(playerid, "select_dealervehpos", 0);
		SetPVarInt(playerid, "select_dealervehnew", 0);
		CreateDealershipPickup(d);

		SavecDealership(d);
		SendClientMessageEx(playerid, COLOR_ARWIN, "DEALERSHIP: "YELLOW_E"Succesfull set vehicle pos slot {00FFFF}%d "YELLOW_E"dealership id {00FFFF}%d", v, d);
	}
	else
	{
        SendClientMessageEx(playerid, COLOR_GREY, " You are not allowed to use this command.");
	}
    return 1;
}
	
CMD:editcd(playerid, params[])
{
    static
        d,
        type[24],
        string[128];

    if(PlayerData[playerid][pAdmin] < 5)
        return PermissionError(playerid);

    if(sscanf(params, "ds[24]S()[128]", d, type, string))
    {
        SendSyntaxMessage(playerid, "/editcd [dealerid] [name]");
        SendClientMessage(playerid, COLOR_YELLOW, "[NAMES]:{FFFFFF} owner, location, money");
        return 1;
    }
    if(!Iter_Contains(Dealership, d)) return SendErrorMessage(playerid, "You have specified an invalid ID.");

    if(!strcmp(type, "location", true))
    {
		GetPlayerPos(playerid, CarDealershipInfo[d][cdEntranceX], CarDealershipInfo[d][cdEntranceY], CarDealershipInfo[d][cdEntranceZ]);
		CreateDealershipPickup(d);

		SavecDealership(d);
		SendClientMessageEx(playerid, COLOR_ARWIN, "DEALERSHIP: "YELLOW_E"Succesfull location dealerid {00FFFF}%d",d);
    }
    else if(!strcmp(type, "owner", true))
    {
        new owners[MAX_PLAYER_NAME];

        if(sscanf(string, "s[32]", owners))
            return SendSyntaxMessage(playerid, "/editcd [dealerid] [owner] [player name] (use '-' to no owner)");

        format(CarDealershipInfo[d][cdOwner], MAX_PLAYER_NAME, owners);
  
        CreateDealershipPickup(d);

		SavecDealership(d);
        SendAdminMessage(COLOR_RED, "%s has adjusted the owner of dealer ID: %d to %s", GetName(playerid), d, owners);
    }
	else if(!strcmp(type, "money", true))
    {
        new money;

        if(sscanf(string, "d", money))
            return SendSyntaxMessage(playerid, "/editcd [dealerid] [money] [Ammount]");

        CarDealershipInfo[d][cdTill] = money;
        SavecDealership(d);
        SendAdminMessage(COLOR_RED, "%s has adjusted the money of Dealer ID: %d to %s.", GetName(playerid), d, FormatNumber(money));
    }
    return 1;
}

//DEALER
CMD:dm(playerid, params[])
{
    foreach(new d : Dealership)
	{
        if(IsPlayerInRangeOfPoint(playerid, 3.0, CarDealershipInfo[d][cdEntranceX], CarDealershipInfo[d][cdEntranceY], CarDealershipInfo[d][cdEntranceZ])) {
            if(IsPlayerOwnerOfCDEx(playerid, d))
			{
                SetPVarInt(playerid, "select_dealer", d);
                SetPVarInt(playerid, "select_dealerveh", -1);
                SetPVarInt(playerid, "select_dealervehpos", 0);
                SetPVarInt(playerid, "select_dealervehnew", 0);
                new listitems[] = "Vehicles\nDealership - Safes\nChange Name Dealership\nEmployees";
                ShowPlayerDialog(playerid,DIALOG_CDEDIT,DIALOG_STYLE_LIST,"Choose an item to continue", listitems,"Select","Cancel");
                return 1;
            }
            else
			{
                SendClientMessageEx(playerid, COLOR_GREY, "You do not own that Car Dealership.");
                return 1;
            }
        }
    }
    SendClientMessageEx(playerid, COLOR_GREY, "ERROR: You must be standing inside the radius of the Car Dealership.");
    return 1;
}

CMD:destroydealership(playerid, params[])
{
    if(PlayerData[playerid][pAdmin] < 5)
	{
        SendClientMessageEx(playerid, COLOR_GREY, " You are not allowed to use this command.");
        return 1;
    }

    new string[128], query[128], dealershipid;
    if(sscanf(params, "d", dealershipid)) return SendClientMessageEx(playerid, COLOR_WHITE, "USAGE: /destroydealership [dealershipid]");
    if((dealershipid < 0 || dealershipid >= MAX_CARDEALERSHIPS))
        return SendErrorMessage(playerid, "You have specified an invalid ID.");
	if(!Iter_Contains(Dealership, dealershipid)) return SendErrorMessage(playerid, "The bisnis you specified ID of doesn't exist.");
    SendCustomMessage(playerid, "SUCCES", "You have successfully deleted the dealer's business property "AQUA_E"ID: %d", dealershipid);
	CarDealershipInfo[dealershipid][cdEntranceX] = 0.0;
	CarDealershipInfo[dealershipid][cdEntranceY] = 0.0;
	CarDealershipInfo[dealershipid][cdEntranceZ] = 0.0;
	CarDealershipInfo[dealershipid][cdRadius] = 0.0;
	CarDealershipInfo[dealershipid][cdTill] = 0;
	CarDealershipInfo[dealershipid][cdOwned] = 0;
	CarDealershipInfo[dealershipid][cdPrice] = 0;
	CarDealershipInfo[dealershipid][cdType] = 0;
	format(string, sizeof(string), "-");
	strmid(CarDealershipInfo[dealershipid][cdOwner], string, 0, strlen(string), 255);
	format(string, sizeof(string), "None");
	strmid(CarDealershipInfo[dealershipid][cdMessage], string, 0, strlen(string), 255);
	DestroyDynamic3DTextLabel(CarDealershipInfo[dealershipid][cdTextLabel]);
	DestroyDynamicPickup(CarDealershipInfo[dealershipid][cdPickupID]);
	CarDealershipInfo[dealershipid][cdPickupID] = 0;
	CarDealershipInfo[dealershipid][cdTextLabel] = Text3D:INVALID_3DTEXT_ID;
	CarDealershipInfo[dealershipid][cdVehicleSpawn][0] = 0.0;
    CarDealershipInfo[dealershipid][cdVehicleSpawn][1] = 0.0;
    CarDealershipInfo[dealershipid][cdVehicleSpawn][2] = 0.0;
    CarDealershipInfo[dealershipid][cdVehicleSpawn][3] = 0.0;
	for(new v = 0; v < MAX_DEALERSHIPVEHICLES; v++)
	{
		DestroyCarDealershipVehicle(dealershipid, v);
		CarDealershipInfo[dealershipid][cdVehicleId][v] = INVALID_VEHICLE_ID;
	}
	Iter_Remove(Dealership, dealershipid);
	mysql_format(sqlcon, query, sizeof(query), "DELETE FROM car_dealer WHERE ID=%d", CarDealershipInfo[dealershipid][cdID]);
	mysql_tquery(sqlcon, query);
    return 1;
}

CMD:buydealership(playerid, params[])
{
    foreach(new d : Dealership)
	{
        if(IsPlayerInRangeOfPoint(playerid, 3.0, CarDealershipInfo[d][cdEntranceX], CarDealershipInfo[d][cdEntranceY], CarDealershipInfo[d][cdEntranceZ]))
		{
			if(strcmp(CarDealershipInfo[d][cdOwner], "-")) return SendErrorMessage(playerid, "That Car Dealership is already owned and it's not for sale.");

			if(GetMoney(playerid) < CarDealershipInfo[d][cdPrice])
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Uang Anda kurang untuk membeli dealer kendaraan ini.");
				return 1;
			}
			GiveMoney(playerid, -CarDealershipInfo[d][cdPrice]);
			GetPlayerName(playerid, CarDealershipInfo[d][cdOwner], MAX_PLAYER_NAME);
			new String[212];
			format(String, sizeof(String), "{00FFFF}[id:%d]\n{FFFF00}%s\n{FFFFFF}Owner: {00FF00}%s",d, CarDealershipInfo[d][cdMessage], CarDealershipInfo[d][cdOwner]);
			UpdateDynamic3DTextLabelText(CarDealershipInfo[d][cdTextLabel], COLOR_TWAQUA, String);
			SavecDealership(d);
        }
    }
    return 1;
}

CMD:selldealership(playerid, params[])
{
	new string[128];

    foreach(new d : Dealership)
	{
        if(IsPlayerInRangeOfPoint(playerid, 3.0, CarDealershipInfo[d][cdEntranceX], CarDealershipInfo[d][cdEntranceY], CarDealershipInfo[d][cdEntranceZ]))
		{
            if(IsPlayerOwnerOfCDEx(playerid, d))
			{
                SetPVarInt(playerid, "select_dealer", d);
                format(string,128,"Are you sure you want to sell this Car Dealership for $%s?\n.", FormatNumber(CarDealershipInfo[d][cdPrice]));
                ShowPlayerDialog(playerid,DIALOG_CDSELL,DIALOG_STYLE_MSGBOX,"Warning:",string,"Sell","Cancel");
                return 1;
            }
            else
			{
                SendClientMessageEx(playerid, COLOR_GREY, "You are not the owner of this car dealership.");
                return 1;
            }
        }
    }
    SendClientMessageEx(playerid, COLOR_GREY, "You have to be near a car dealership.");
    return 1;
}

CMD:destroycdveh(playerid, params[])
{
    if(PlayerData[playerid][pAdmin] < 5)
	{
        SendClientMessageEx(playerid, COLOR_GREY, " You are not allowed to use this command.");
        return 1;
    }

    new string[128], vehid;
    if(sscanf(params, "d", vehid)) return SendClientMessageEx(playerid, COLOR_WHITE, "USAGE: /destroycdveh [vehicleid]");
    DestroyCarDealershipVehicle(GetCarDealershipId(vehid), GetCarDealershipVehicleId(vehid));
    SavecDealership(GetCarDealershipId(vehid));
    format(string, sizeof(string), "Car Dealership Vehicle deleted with ID %d.", vehid);
    SendClientMessageEx(playerid, COLOR_GRAD1, string);
    return 1;
}

CMD:createdealership(playerid, params[])
{
    if(PlayerData[playerid][pAdmin] >= 5)
	{
	    new query[512], dealershipid = Iter_Free(Dealership), Float:X,Float:Y,Float:Z;

	    GetPlayerPos(playerid,X,Y,Z);
	    if(dealershipid == -1) return SendErrorMessage(playerid, "Car Dealerships limit reached.");
        format(CarDealershipInfo[dealershipid][cdOwner], 128, "-");
        CarDealershipInfo[dealershipid][cdEntranceX] = X;
        CarDealershipInfo[dealershipid][cdEntranceY] = Y;
        CarDealershipInfo[dealershipid][cdEntranceZ] = Z;
        CarDealershipInfo[dealershipid][cdRadius] = 30;
        CarDealershipInfo[dealershipid][cdPrice] = 6500000;
        CarDealershipInfo[dealershipid][cdType] = 1;
        format(CarDealershipInfo[dealershipid][cdMessage], 212, "");
        SendCustomMessage(playerid, "SUCCES", "You have succeeded in creating a dealer business property at a price "GREEN_E"$%s "WHITE_E", ID:"AQUA_E"[%d]", FormatNumber(CarDealershipInfo[dealershipid][cdPrice]), dealershipid);
        
        Iter_Add(Dealership, dealershipid);
        mysql_format(sqlcon, query, sizeof(query), "INSERT INTO `car_dealer` SET entrancex='%f', entrancey='%f', entrancez='%f', price='%d'", CarDealershipInfo[dealershipid][cdEntranceX], CarDealershipInfo[dealershipid][cdEntranceY], CarDealershipInfo[dealershipid][cdEntranceZ], CarDealershipInfo[dealershipid][cdPrice]);
	    mysql_tquery(sqlcon, query, "cdDealer", "i", dealershipid);
	}
	else PermissionError(playerid);
    return 1;
}

CMD:gotocd(playerid, params[])
{
    if(PlayerData[playerid][pAdmin] < 5)
	{
        SendClientMessageEx(playerid, COLOR_GREY, " You are not allowed to use this command.");
        return 1;
    }
	new dealershipid;
    if(sscanf(params, "d", dealershipid)) return SendClientMessageEx(playerid, COLOR_WHITE, "USAGE: /gotodealership [dealershipid]");
    if (!Iter_Contains(Dealership, dealershipid))
        return SendErrorMessage(playerid, "Invalid CarDealership id.");
    SetPlayerPos(playerid, CarDealershipInfo[dealershipid][cdEntranceX], CarDealershipInfo[dealershipid][cdEntranceY], CarDealershipInfo[dealershipid][cdEntranceZ]);
    return 1;
}
