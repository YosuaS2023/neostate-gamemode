function Plant_Load()
{
	new Cache: cache, str[300];
	mysql_format(sqlcon, str, sizeof(str), "SELECT * FROM `plants`");
	cache = mysql_query(sqlcon, str);
	{
		new rows = cache_num_rows(), ids;
		for(new i = 0; i < rows; i++) if((ids = Iter_Free(PlantIds)) != INVALID_ITER_ID)
		{
			PlantData[ids][plantID] = cache_get_field_content_int(i, "plantID");
			PlantData[ids][plantModel] = cache_get_field_content_int(i, "plantModel");

			PlantData[ids][plantPos][0] = cache_get_field_content_float(i, "plantPosX");
			PlantData[ids][plantPos][1] = cache_get_field_content_float(i, "plantPosY");
			PlantData[ids][plantPos][2] = cache_get_field_content_float(i, "plantPosZ");
			PlantData[ids][plantPos][3] = cache_get_field_content_float(i, "plantGrowthZ");
            PlantData[ids][plantGrowthMillisecond] = cache_get_field_content_int(i, "plantGrowthMillisecond");
			PlantData[ids][plantType] = cache_get_field_content_int(i, "plantType");

			Plant_Spawn(ids);
			Iter_Add(PlantIds, ids);
		}
	}
	cache_delete(cache);
	return 1;
}

function Plant_Save(plantid)
{
	new str[300];
	mysql_format(sqlcon, str, sizeof str, "UPDATE `plants` SET `plantModel` = '%i', `plantPosX` = '%f', `plantPosY` = '%f', `plantPosZ` = '%f', `plantGrowthZ` = '%f', `plantGrowthMillisecond` ='%i', `plantType` = '%i' WHERE `plantID` = '%i';", PlantData[plantid][plantModel], PositionXYZ(PlantData[plantid][plantPos]), PlantData[plantid][plantPos][3], PlantData[plantid][plantGrowthMillisecond], PlantData[plantid][plantType], PlantData[plantid][plantID]);
	mysql_tquery(sqlcon, str);
	return 1;
}

function Plant_Destroy(plantid)
{
	if(Iter_Contains(PlantIds, plantid))
	{
		new str[300];
		DestroyDynamicObject(PlantData[plantid][plantObject]);
		PlantData[plantid][plantObject] = INVALID_STREAMER_ID;
		PlantData[plantid][plantPos][0] = 0.0;
		mysql_format(sqlcon, str, sizeof str, "DELETE FROM `plants` WHERE `plantID` = '%i'", PlantData[plantid][plantID]);
		mysql_tquery(sqlcon, str);
		Iter_Remove(PlantIds, plantid);
		return 1;
	}
	return 0;
}

function Plant_Create(playerid, type, Float:x, Float:y, Float:z, Float:GrowthZ, Float:milliseconds)
{
	static ids, Cache: cache, str[400];
	if((ids = Iter_Free(PlantIds)) != INVALID_ITER_ID)
	{
		PlantData[ids][plantPos][0] = x;
		PlantData[ids][plantPos][1] = y;
		PlantData[ids][plantPos][2] = z;
        PlantData[ids][plantPos][3] = GrowthZ;

		PlantData[ids][plantModel] = g_modelPlant[type];

        PlantData[ids][plantGrowthMillisecond] = milliseconds;
		PlantData[ids][plantType] = type;

		Iter_Add(PlantIds, ids);
		mysql_format(sqlcon, str, sizeof str, "INSERT INTO `plants` SET `plantModel` = '%i', `plantPosX` = '%f', `plantPosY` = '%f', `plantPosZ` = '%f', `plantGrowthZ` = '%f', `plantGrowthMillisecond` ='%i', `plantType` = '%i'", g_modelPlant[type], x, y, z, GrowthZ, milliseconds, type);
		cache = mysql_query(sqlcon, str);
		{
			PlantData[ids][plantID] = cache_insert_id();
		}
		cache_delete(cache);
		Plant_Spawn(ids);

		return ids;
	}
	return -1;
}

function Plant_Spawn(ids)
{
	if(IsValidDynamicObject(PlantData[ids][plantObject]))
		DestroyDynamicObject(PlantData[ids][plantObject]);

    if(PlantData[ids][plantPos][2] == PlantData[ids][plantPos][3])
    {
        PlantData[ids][plantObject] = CreateDynamicObject(PlantData[ids][plantModel], PlantData[ids][plantPos][0], PlantData[ids][plantPos][1], PlantData[ids][plantPos][2], 0.00000, 0.00000, 0.00000);
    }
    else
    {
        PlantData[ids][plantObject] = CreateDynamicObject(PlantData[ids][plantModel], PlantData[ids][plantPos][0], PlantData[ids][plantPos][1], PlantData[ids][plantPos][3], 0.00000, 0.00000, 0.00000);
        MoveDynamicObject(PlantData[ids][plantObject], PlantData[ids][plantPos][0], PlantData[ids][plantPos][1], PlantData[ids][plantPos][2], PlantData[ids][plantGrowthMillisecond]);
    }
	return 1;
}

OGMI:Plant()
{
    Plant_Load();
    return 1;
}

OGME:Plant()
{
	foreach(new i : PlantIds) Plant_Save(i);
	return 1;
}