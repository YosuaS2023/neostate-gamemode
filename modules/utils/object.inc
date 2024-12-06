public OnPlayerEditDynamicObject(playerid, STREAMER_TAG_OBJECT:objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
    SendDebugMessage("EditDynamicObject(response: %i, objectid: %i)", response, objectid);
    if(response == EDIT_RESPONSE_FINAL)
    {
        switch(PlayerData[playerid][pEditingMode])
        {
            case EDIT_OBJECT_TRASH:
            {
                new idx = PlayerData[playerid][pEditTrash];
                if(idx != -1 && Iter_Contains(Trash, idx))
                {
                    TrashData[idx][TRASH_POS][0] = x;
                    TrashData[idx][TRASH_POS][1] = y;
                    TrashData[idx][TRASH_POS][2] = z;

                    TrashData[idx][TRASH_ROT][0] = rx;
                    TrashData[idx][TRASH_ROT][1] = ry;
                    TrashData[idx][TRASH_ROT][2] = rz;

                    Trash_Sync(idx);
                    Trash_Save(idx);
                    SendCustomMessage(playerid, "TRASH", "succeeded in changing the trashid position: %i", idx);
                }
                PlayerData[playerid][pEditTrash] = -1;
            }
            case EDIT_OBJECT_ATM:
            {
                new idx = PlayerData[playerid][pEditAtmID];
                if(idx != -1 && Iter_Contains(DynamicAtm, idx))
                {
                    AtmData[idx][AtmPos][0] = x;
                    AtmData[idx][AtmPos][1] = y;
                    AtmData[idx][AtmPos][2] = z;
                    AtmData[idx][AtmPos][3] = rx;
                    AtmData[idx][AtmPos][4] = ry;
                    AtmData[idx][AtmPos][5] = rz;

                    Atm_Sync(idx);
                    Atm_Save(idx);
                    SendCustomMessage(playerid, "ATM", "You change the position of ATM ID %i", idx);
                    PlayerData[playerid][pEditAtmID] = -1;
                }
            }
			case EDIT_OBJECT_PUMP:
			{
				new pump_id;
				if((pump_id = PlayerData[playerid][pEditPump]) != -1)
				{
					if(Iter_Contains(Pump, pump_id))
					{
						PumpData[pump_id][pumpPos][0] = x;
						PumpData[pump_id][pumpPos][1] = y;
						PumpData[pump_id][pumpPos][2] = z;
						PumpData[pump_id][pumpPos][3] = rz;

						Pump_Refresh(pump_id);
						Pump_Save(pump_id);

                        SendCustomMessage(playerid, "PUMP", "Successfully changed the position of the pump ID %i", pump_id);
						PlayerData[playerid][pEditPump] = -1;
					}
				}
			}
            case EDIT_OBJECT_HOUSE_STRUCTURE: 
			{
                new houseid = PlayerData[playerid][pInHouse], id = PlayerData[playerid][pEditStructure];
                SendDebugMessage("houseid: %i, id: %i", houseid, id);
                if (PlayerData[playerid][pEditStructure] != -1) 
				{
                    if (houseid != -1)
					{
                        if (Iter_Contains(HouseStruct[houseid], id))
						{
                            HouseStructure[houseid][id][structurePos][0] = x;
                            HouseStructure[houseid][id][structurePos][1] = y;
                            HouseStructure[houseid][id][structurePos][2] = z;
                            HouseStructure[houseid][id][structureRot][0] = rx;
                            HouseStructure[houseid][id][structureRot][1] = ry;
                            HouseStructure[houseid][id][structureRot][2] = rz;

                            SetDynamicObjectPos(objectid, x, y, z);
                            SetDynamicObjectRot(objectid, rx, ry, rz);
                            HouseStructure_Refresh(id, houseid);
                            HouseStructure_Save(id, houseid);

                            SendCustomMessage(playerid, "BUILDER", "Structure position has been saved.");

                            PlayerData[playerid][pEditHouseStructure] = -1;
                            PlayerData[playerid][pEditStructure] = -1;
                        }
                    }
                }
            }
        }
        PlayerData[playerid][pEditingMode] = NOT_EDIT_OBJECT;
    }
    else if(response == EDIT_RESPONSE_CANCEL)
    {
        new Float:position[3],Float:rotation[3];
        switch(PlayerData[playerid][pEditingMode])
        {
            case EDIT_OBJECT_TRASH:
            {
                new idx = PlayerData[playerid][pEditTrash];
                if(idx != -1 && Iter_Contains(Trash, idx))
                {
                    Trash_Sync(idx);
                    SendCustomMessage(playerid, "TRASH", "You cancel change the trash position %i", idx);
                    PlayerData[playerid][pEditTrash] = -1;
                }
            }
            case EDIT_OBJECT_ATM:
            {
                new idx = PlayerData[playerid][pEditAtmID];
                if(idx != -1 && Iter_Contains(DynamicAtm, idx))
                {
                    Atm_Sync(idx);
                    SendCustomMessage(playerid, "ATM", "You cancel change the ATM position %i", idx);
                    PlayerData[playerid][pEditAtmID] = -1;
                }
            }
            case EDIT_OBJECT_HOUSE_STRUCTURE: 
			{
                if(PlayerData[playerid][pEditStructure] != -1) 
				{
                    new slot = PlayerData[playerid][pEditStructure], houseid = PlayerData[playerid][pEditHouseStructure];

                    if (houseid != -1)
					{
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,HouseStructure[houseid][slot][structureObject],E_STREAMER_X,position[0]);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,HouseStructure[houseid][slot][structureObject],E_STREAMER_Y,position[1]);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,HouseStructure[houseid][slot][structureObject],E_STREAMER_Z,position[2]);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,HouseStructure[houseid][slot][structureObject],E_STREAMER_R_X,rotation[0]);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,HouseStructure[houseid][slot][structureObject],E_STREAMER_R_Y,rotation[1]);
                        Streamer_GetFloatData(STREAMER_TYPE_OBJECT,HouseStructure[houseid][slot][structureObject],E_STREAMER_R_Z,rotation[2]);
                        SetDynamicObjectPos(objectid,position[0],position[1],position[2]);
                        SetDynamicObjectRot(objectid,rotation[0],rotation[1],rotation[2]);

                        PlayerData[playerid][pEditHouseStructure] = -1;
                        PlayerData[playerid][pEditStructure] = -1;
                    }
                }
            }
            case EDIT_OBJECT_PUMP:
            {
                static pump_id;
				if((pump_id = PlayerData[playerid][pEditPump]) != -1)
				{
					if(Iter_Contains(Pump, pump_id))
					{
						Pump_Refresh(pump_id);
						PlayerData[playerid][pEditPump] = -1;
                        SendCustomMessage(playerid, "PUMP", "failed to change pump id %i", pump_id);
					}
				}
            }
        }
        PlayerData[playerid][pEditingMode] = NOT_EDIT_OBJECT;
    }
	return 1;
}

FUNC:: StreamerConfig()
{
	Streamer_MaxItems(STREAMER_TYPE_OBJECT, 990000);
	Streamer_MaxItems(STREAMER_TYPE_MAP_ICON, 2000);
	Streamer_MaxItems(STREAMER_TYPE_PICKUP, 2000);
	for(new playerid = (GetMaxPlayers() - 1); playerid != -1; playerid--)
	{
		Streamer_DestroyAllVisibleItems(playerid, 0);
	}
	Streamer_VisibleItems(STREAMER_TYPE_OBJECT, 1000);
	return 1;
}

Function:Streamer_SetPosition(type, STREAMER_ALL_TAGS:id, Float:x, Float:y, Float:z)
{
	Streamer_SetFloatData(type, id, E_STREAMER_X, x);
	Streamer_SetFloatData(type, id, E_STREAMER_Y, y);
	Streamer_SetFloatData(type, id, E_STREAMER_Z, z);
}

Function:Streamer_SetRotation(type, STREAMER_ALL_TAGS:id, Float:x, Float:y, Float:z)
{
	Streamer_SetFloatData(type, id, E_STREAMER_R_X, x);
	Streamer_SetFloatData(type, id, E_STREAMER_R_Y, y);
	Streamer_SetFloatData(type, id, E_STREAMER_R_Z, z);
}

Function:Streamer_GetPosition(type, STREAMER_ALL_TAGS:id, &Float:x, &Float:y, &Float:z)
{
	Streamer_GetFloatData(type, id, E_STREAMER_X, x);
	Streamer_GetFloatData(type, id, E_STREAMER_Y, y);
	Streamer_GetFloatData(type, id, E_STREAMER_Z, z);
}

Function:Streamer_GetRotation(type, STREAMER_ALL_TAGS:id, &Float:x, &Float:y, &Float:z)
{
	Streamer_GetFloatData(type, id, E_STREAMER_R_X, x);
	Streamer_GetFloatData(type, id, E_STREAMER_R_Y, y);
	Streamer_GetFloatData(type, id, E_STREAMER_R_Z, z);
}