/*
    house-structure defines
*/
#define STRUCTURE_SELECT_EDITOR         1
#define STRUCTURE_SELECT_DELETE	        2
#define STRUCTURE_SELECT_RETEXTURE	    3
#define STRUCTURE_SELECT_COPY		    4

/*
    House Structure Function
*/
GetStructureNameByModel(model) {
    new name[32];

    for (new i = 0; i < sizeof(g_aHouseStructure); i ++) if (g_aHouseStructure[i][e_StructureModel] == model) {
        strcat(name, g_aHouseStructure[i][e_StructureName]);
        break;
    }
    return name;
}

HouseStructure_Save(id, houseid)
{
    if (Iter_Contains(HouseStruct[houseid], id))
    {
        static
            query[600];

        format(query, sizeof(query), "UPDATE `house_struct` SET `HouseID`='%d', `Model`='%d', `PosX`='%.4f', `PosY`='%.4f', `PosZ`='%.4f', `RotX`='%.4f', `RotY`='%.4f', `RotZ`='%.4f', `Material`='%d', `Color`='%d', `Type`='%d' WHERE `ID`='%d'",
            HouseData[houseid][houseSQLID],
            HouseStructure[houseid][id][structureModel],
            HouseStructure[houseid][id][structurePos][0],
            HouseStructure[houseid][id][structurePos][1],
            HouseStructure[houseid][id][structurePos][2],
            HouseStructure[houseid][id][structureRot][0],
            HouseStructure[houseid][id][structureRot][1],
            HouseStructure[houseid][id][structureRot][2],
            HouseStructure[houseid][id][structureMaterial],
            HouseStructure[houseid][id][structureColor],
            HouseStructure[houseid][id][structureType],
            HouseStructure[houseid][id][structureID]
        );
        return mysql_tquery(sqlcon, query);
    }
    return 0;
}

HouseStructure_Refresh(id, houseid)
{
    if (Iter_Contains(HouseStruct[houseid], id))
    {
        if (!IsValidDynamicObject(HouseStructure[houseid][id][structureObject]))
        {
            HouseStructure[houseid][id][structureObject] = CreateDynamicObject(HouseStructure[houseid][id][structureModel], HouseStructure[houseid][id][structurePos][0], HouseStructure[houseid][id][structurePos][1], HouseStructure[houseid][id][structurePos][2], HouseStructure[houseid][id][structureRot][0], HouseStructure[houseid][id][structureRot][1], HouseStructure[houseid][id][structureRot][2], HouseData[houseid][houseSQLID], HouseData[houseid][houseInt][1]);
        }
        HouseStructure_ObjectUpdate(id, houseid);

        foreach (new i : Player) if (IsPlayerInRangeOfPoint(i, 5.0, HouseStructure[houseid][id][structurePos][0], HouseStructure[houseid][id][structurePos][1], HouseStructure[houseid][id][structurePos][2])) {
            Streamer_Update(i);
        }
    }
    return 1;
}

HouseStructure_ObjectUpdate(id, houseid) 
{
    if (Iter_Contains(HouseStruct[houseid], id)) 
    {
		Streamer_SetFloatData(STREAMER_TYPE_OBJECT, HouseStructure[houseid][id][structureObject], E_STREAMER_X, HouseStructure[houseid][id][structurePos][0]);
		Streamer_SetFloatData(STREAMER_TYPE_OBJECT, HouseStructure[houseid][id][structureObject], E_STREAMER_Y, HouseStructure[houseid][id][structurePos][1]);
		Streamer_SetFloatData(STREAMER_TYPE_OBJECT, HouseStructure[houseid][id][structureObject], E_STREAMER_Z, HouseStructure[houseid][id][structurePos][2]);

		Streamer_SetFloatData(STREAMER_TYPE_OBJECT, HouseStructure[houseid][id][structureObject], E_STREAMER_R_X, HouseStructure[houseid][id][structureRot][0]);
		Streamer_SetFloatData(STREAMER_TYPE_OBJECT, HouseStructure[houseid][id][structureObject], E_STREAMER_R_Y, HouseStructure[houseid][id][structureRot][1]);
		Streamer_SetFloatData(STREAMER_TYPE_OBJECT, HouseStructure[houseid][id][structureObject], E_STREAMER_R_Z, HouseStructure[houseid][id][structureRot][2]);

		Streamer_SetIntData(STREAMER_TYPE_OBJECT, HouseStructure[houseid][id][structureObject], E_STREAMER_WORLD_ID, HouseData[houseid][houseSQLID]);
		Streamer_SetIntData(STREAMER_TYPE_OBJECT, HouseStructure[houseid][id][structureObject], E_STREAMER_INTERIOR_ID, HouseData[houseid][houseInt][1]);
        
        if (HouseStructure[houseid][id][structureMaterial] > 0) {
            if (HouseStructure[houseid][id][structureModel] == 1502 || HouseStructure[houseid][id][structureModel] == 14414) SetDynamicObjectMaterial(HouseStructure[houseid][id][structureObject], 1, GetTModel(HouseStructure[houseid][id][structureMaterial]), GetTXDName(HouseStructure[houseid][id][structureMaterial]), GetTextureName(HouseStructure[houseid][id][structureMaterial]), HouseStructure[houseid][id][structureColor]);
            else SetDynamicObjectMaterial(HouseStructure[houseid][id][structureObject], 0, GetTModel(HouseStructure[houseid][id][structureMaterial]), GetTXDName(HouseStructure[houseid][id][structureMaterial]), GetTextureName(HouseStructure[houseid][id][structureMaterial]), HouseStructure[houseid][id][structureColor]);
        }
		return 1;
    }
    return 0;
}

CreateHouseInterior(houseid) 
{
    new id = -1;
    for (new i = 0; i < sizeof(g_aHouseInteriors); i ++) if (g_aHouseInteriors[i][e_Type] == HouseData[houseid][houseType]) 
    {
        HouseStructure_Add(houseid, g_aHouseInteriors[i][e_ObjModel], g_aHouseInteriors[i][e_ObjPosX], g_aHouseInteriors[i][e_ObjPosY], g_aHouseInteriors[i][e_ObjPosZ], g_aHouseInteriors[i][e_ObjRotX], g_aHouseInteriors[i][e_ObjRotY], g_aHouseInteriors[i][e_ObjRotZ], 1);
    }
    return 1;
}

HouseStructure_CopyObject(id, houseid)
{
    new current = HouseStructure[houseid][id][structureObject],
        model,
        Float:curPos[3],
        Float:curRot[3],
        matModel,
        txdName[32],
        textureName[32],
        matColor,
        textureID = 0;

    model = Streamer_GetIntData(STREAMER_TYPE_OBJECT, current, E_STREAMER_MODEL_ID);
    Streamer_GetFloatData(STREAMER_TYPE_OBJECT, current, E_STREAMER_X, curPos[0]);
    Streamer_GetFloatData(STREAMER_TYPE_OBJECT, current, E_STREAMER_Y, curPos[1]);
    Streamer_GetFloatData(STREAMER_TYPE_OBJECT, current, E_STREAMER_Z, curPos[2]);
    Streamer_GetFloatData(STREAMER_TYPE_OBJECT, current, E_STREAMER_R_X, curRot[0]);
    Streamer_GetFloatData(STREAMER_TYPE_OBJECT, current, E_STREAMER_R_Y, curRot[1]);
    Streamer_GetFloatData(STREAMER_TYPE_OBJECT, current, E_STREAMER_R_Z, curRot[2]);

    if (HouseStructure[houseid][id][structureMaterial] > 0)
    {
        GetDynamicObjectMaterial(current, 0, matModel, txdName, textureName, matColor);
        textureID = HouseStructure[houseid][id][structureMaterial];
    }

    new copyId = HouseStructure_Add(houseid, model, curPos[0], curPos[1], curPos[2], curRot[0], curRot[1], curRot[2]);

    if (copyId == cellmin)
        return cellmin;

    if (textureID != 0) SetDynamicObjectMaterial(HouseStructure[houseid][copyId][structureObject], 0, matModel, txdName, textureName, matColor), HouseStructure[houseid][copyId][structureMaterial] = textureID, HouseStructure_Save(copyId, houseid);

    return copyId;
}

function HouseStructure_GetCount(houseid)
{
    new count;
    foreach (new i : HouseStruct[houseid]) if (HouseStructure[houseid][i][structureType] == 0) count++;
    return count;
}

function OnLoadHouseStructure(houseid)
{
    new rows = cache_num_rows(),
        id = cellmin;

    if (rows)
    {
        for (new i = 0; i < rows; i ++) if ((id = Iter_Free(HouseStruct[houseid])) != cellmin) 
        {
            Iter_Add(HouseStruct[houseid], id);

            cache_get_value_int(i, "ID", HouseStructure[houseid][id][structureID]);
            cache_get_value_int(i, "Model", HouseStructure[houseid][id][structureModel]);
            cache_get_value_float(i, "PosX", HouseStructure[houseid][id][structurePos][0]);
            cache_get_value_float(i, "PosY", HouseStructure[houseid][id][structurePos][1]);
            cache_get_value_float(i, "PosZ", HouseStructure[houseid][id][structurePos][2]);
            cache_get_value_float(i, "RotX", HouseStructure[houseid][id][structureRot][0]);
            cache_get_value_float(i, "RotY", HouseStructure[houseid][id][structureRot][1]);
            cache_get_value_float(i, "RotZ", HouseStructure[houseid][id][structureRot][2]);
            cache_get_value_int(i, "Material", HouseStructure[houseid][id][structureMaterial]);
            cache_get_value_int(i, "Color", HouseStructure[houseid][id][structureColor]);
            cache_get_value_int(i, "Type", HouseStructure[houseid][id][structureType]);

            HouseStructure_Refresh(id, houseid);
        }
    }
    return 1;
}

HouseStructure_Add(houseid, modelid, Float:x, Float:y, Float:z, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, type = 0)
{
    static
        id = -1;

    if(!Iter_Contains(House, houseid))
        return 0;

    if ((id = Iter_Free(HouseStruct[houseid])) != -1)
    {
        Iter_Add(HouseStruct[houseid], id);
        HouseStructure[houseid][id][structureModel] = modelid;
        HouseStructure[houseid][id][structurePos][0] = x;
        HouseStructure[houseid][id][structurePos][1] = y;
        HouseStructure[houseid][id][structurePos][2] = z;
        HouseStructure[houseid][id][structureRot][0] = rx;
        HouseStructure[houseid][id][structureRot][1] = ry;
        HouseStructure[houseid][id][structureRot][2] = rz;
        HouseStructure[houseid][id][structureMaterial] = 0;
        HouseStructure[houseid][id][structureColor] = 0;
        HouseStructure[houseid][id][structureType] = type;

        HouseStructure_Refresh(id, houseid);
        new query[128];
        mysql_format(sqlcon, query, sizeof(query), "INSERT INTO `house_struct` (`HouseID`) VALUES ('%d')", HouseData[houseid][houseSQLID]);
        mysql_tquery(sqlcon, query, "OnHouseStructureCreated", "dd", id, houseid);

        return id;
    }

    return -1;
}

HouseStructure_DeleteAll(houseid)
{
    if (Iter_Contains(House, houseid)) 
    {
        foreach (new id : HouseStruct[houseid]) if (HouseStructure[houseid][id][structureType] == 0)
        {
            new query[128];
            mysql_format(sqlcon, query, sizeof(query), "DELETE FROM `house_struct` WHERE `ID`='%d'", HouseStructure[houseid][id][structureID]);
            mysql_tquery(sqlcon, query);
            if (IsValidDynamicObject(HouseStructure[houseid][id][structureObject])) {
                DestroyDynamicObject(HouseStructure[houseid][id][structureObject]);
                HouseStructure[houseid][id][structureObject] = INVALID_STREAMER_ID;
            }

            new tmp_houseStructure[houseStructure];
            HouseStructure[houseid][id] = tmp_houseStructure;

            new current = id;
            Iter_SafeRemove(HouseStruct[houseid], current, id);
        }
    }
    return 1;
}

HouseStructure_Delete(id, houseid) 
{
    if (Iter_Contains(HouseStruct[houseid], id)) 
    {
        new query[128];
        mysql_format(sqlcon, query, sizeof(query), "DELETE FROM `house_struct` WHERE `ID`='%d'", HouseStructure[houseid][id][structureID]);
        mysql_tquery(sqlcon, query);
        if (IsValidDynamicObject(HouseStructure[houseid][id][structureObject])) {
            DestroyDynamicObject(HouseStructure[houseid][id][structureObject]);
            HouseStructure[houseid][id][structureObject] = INVALID_STREAMER_ID;
        }

        Iter_Remove(HouseStruct[houseid], id);

        new tmp_houseStructure[houseStructure];
        HouseStructure[houseid][id] = tmp_houseStructure;
    }
    return 1;
}

function OnHouseStructureCreated(id, houseid)
{
    HouseStructure[houseid][id][structureID] = cache_insert_id();
    HouseStructure_Save(id, houseid);
    return 1;
}

/*
    Dialog house-structure
*/
Dialog:ListedStructure(playerid, response, listitem, inputtext[])
{
    if (response)
    {
        new id = PlayerData[playerid][pInHouse];

        if (id != -1 && (House_PlayerOwn(playerid, id) || HouseData[id][houseBuilder] == PlayerData[playerid][pID])) {
            PlayerData[playerid][pEditStructure] = ListedStructure[playerid][listitem];
            PlayerData[playerid][pEditHouseStructure] = id;
            Dialog_Show(playerid, StructureList, DIALOG_STYLE_LIST, "House Structure: Edit", "Move\nRetexture\nCopy\nDestroy", "Select", "Back");
        }
    } else Dialog_Show(playerid, House_Structure, DIALOG_STYLE_LIST, "House Builder Menu", "Create Structure\nStructure list\n"RED"Reset Structure"WHITE, "Select", "Back");
    return 1;
}

Dialog:StructureList(playerid, response, listitem, inputtext[]) 
{
    if (response) 
    {
        new houseid = PlayerData[playerid][pInHouse], id = PlayerData[playerid][pEditStructure];

        if (houseid != -1) 
        {
            switch (listitem) 
            {
                case 0: {
                    if (HouseStructure[houseid][id][structureType] > 0)
                        return SendErrorMessage(playerid, "Cannot move static house structure.");

                    PlayerData[playerid][pEditHouseStructure] = houseid;
                    PlayerData[playerid][pEditStructure] = id;
                    PlayerData[playerid][pEditingMode] = EDIT_OBJECT_HOUSE_STRUCTURE;
                    D:1("editing mode: %i, id structure: %i, id house struct: %i", PlayerData[playerid][pEditingMode],  PlayerData[playerid][pEditStructure],  PlayerData[playerid][pEditHouseStructure]);
                    EditDynamicObject(playerid, HouseStructure[houseid][id][structureObject]);
                    SendCustomMessage(playerid, "HOUSE", "You're now editing %s.", GetStructureNameByModel(HouseStructure[houseid][id][structureModel]));
                }
                case 1: {
                    SetPVarInt(playerid, "structureObj", id);
                    PlayerData[playerid][pEditHouseStructure] = houseid;
                    Dialog_Show(playerid, House_StructureRetexture, DIALOG_STYLE_INPUT, "Retexture House Structure", "Please input the texture name below:\n"YELLOW_E"[model] [txdname] [texture] [opt: alpha] [opt: red] [opt: green] [opt: blue]", "Retexture", "Cancel");
                }
                case 2: {
                    if (HouseStructure[houseid][id][structureType] > 0)
                        return SendErrorMessage(playerid, "Cannot copy static house structure.");
                    
                    new price;

                    for (new i = 0; i < sizeof(g_aHouseStructure); i ++) if (g_aHouseStructure[i][e_StructureModel] == HouseStructure[houseid][id][structureModel]) {
                        price = g_aHouseStructure[i][e_StructureCost];
                    }

                    if (Inventory_Count(playerid, "Component") < price)
                        return SendErrorMessage(playerid, "You need %d Components to copy this structure.", price);

                    new copyId = HouseStructure_CopyObject(id, houseid);

                    if (copyId == cellmin)
                        return SendErrorMessage(playerid, "Your house has reached maximum of structure");

                    Inventory_Remove(playerid, "Component", price);
                    PlayerData[playerid][pEditStructure] = copyId;
                    EditingHouseStructure[playerid] = 1;
                    PlayerData[playerid][pEditHouseStructure] = houseid;
                    PlayerData[playerid][pEditingMode] = EDIT_OBJECT_HOUSE_STRUCTURE;
                    EditDynamicObject(playerid, HouseStructure[houseid][copyId][structureObject]);
                    SendCustomMessage(playerid, "BUILDER", "You have copied structure for "GREEN_E"%d components", price);
                    SendCustomMessage(playerid, "BUILDER", "You're now editing copied object of %s.", GetStructureNameByModel(HouseStructure[houseid][id][structureModel]));
                }
                case 3: {
                    if (HouseStructure[houseid][id][structureType] > 0)
                        return SendErrorMessage(playerid, "Cannot destroy static house structure.");

                    SendCustomMessage(playerid, "HOUSE", "You've been successfully deleted %s", GetStructureNameByModel(HouseStructure[houseid][id][structureModel]));
                    HouseStructure_Delete(id, houseid);
                }
            }
        }
    } else Dialog_Show(playerid, House_Structure, DIALOG_STYLE_LIST, "House Builder Menu", "Create Structure\nStructure list\n"RED"Reset Structure"WHITE, "Select", "Back");
    return 1;
}

Dialog:House_Structure(playerid, response, listitem, inputtext[])
{
    if (response) 
    {
        new id = PlayerData[playerid][pInHouse];
        if (id != -1) 
        {
            switch (listitem) 
            {
                case 0: {
                    new str[128];
                    strcat(str, "Type\tCost (components)\n");
                    for (new i = 0; i < sizeof(g_aHouseStructure); i ++) {
                        format(str, sizeof(str), "%s%s\t%d\n", str, g_aHouseStructure[i][e_StructureName], g_aHouseStructure[i][e_StructureCost]);
                    }
                    Dialog_Show(playerid, House_StructureAdd, DIALOG_STYLE_TABLIST_HEADERS, "House Structure Modification", str, "Select", "Back");
                }/*
                case 1: {
                    PlayerData[playerid][pEditHouseStructure] = id;
                    SelectStructureType[playerid] = STRUCTURE_SELECT_EDITOR;
                    SelectObject(playerid);
                    SendCustomMessage(playerid, "BUILDER", "Please select the structure, you wish to move.");
                }
                case 2: {
                    PlayerData[playerid][pEditHouseStructure] = id;
                    SelectStructureType[playerid] = STRUCTURE_SELECT_RETEXTURE;
                    SelectObject(playerid);
                    SendCustomMessage(playerid, "BUILDER", "Please select the structure, you wish to retexture.");
                }
                case 3: {
                    PlayerData[playerid][pEditHouseStructure] = id;
                    SelectStructureType[playerid] = STRUCTURE_SELECT_DELETE;
                    SelectObject(playerid);
                    SendCustomMessage(playerid, "BUILDER", "Please select the structure, you wish to destroy.");
                }
                case 4: {
                    PlayerData[playerid][pEditHouseStructure] = id;
                    SelectStructureType[playerid] = STRUCTURE_SELECT_COPY;
                    SelectObject(playerid);
                    SendCustomMessage(playerid, "BUILDER", "Please select the structure, you wish to copy.");
                }*/
                case 1: {
                    PlayerData[playerid][pEditHouseStructure] = id;
                    Dialog_Show(playerid, House_StructureType, DIALOG_STYLE_LIST, "Structure Type", "Static\nCostum", "Select", "Cancel");
                }
                case 2: {
                    if (!HouseStructure_GetCount(id))
                        return SendErrorMessage(playerid, "There are no structures in this house.");
                    
                    PlayerData[playerid][pEditHouseStructure] = id;
                    Dialog_Show(playerid, House_StructureDestroy, DIALOG_STYLE_MSGBOX, "Destroy All Structures", RED_E"WARNING:\n"WHITE_E"Are you sure you want to destroy all structures in this house?", "Yes", "No");
                }/*
                case 7: {
                    PlayerData[playerid][pEditFurnHouse] = id;
                    SelectFurnitureType[playerid] = FURNITURE_SELECT_MOVE;
                    SelectObject(playerid);
                    SendCustomMessage(playerid, "BUILDER", "Plase select the furniture, you wish to move.");
                }
                case 8: {
                    PlayerData[playerid][pEditFurnHouse] = id;
                    SelectFurnitureType[playerid] = FURNITURE_SELECT_DESTROY;
                    SelectObject(playerid);
                    SendCustomMessage(playerid, "BUILDER", "Plase select the furniture, you wish to destroy.");
                }
                case 9: {
                    PlayerData[playerid][pEditFurnHouse] = id;
                    SelectFurnitureType[playerid] = FURNITURE_SELECT_STORE;
                    SelectObject(playerid);
                    SendCustomMessage(playerid, "BUILDER", "Plase select the furniture, you wish to store.");
                }
                case 10: {
                    new
                        count = 0,
                        string[MAX_FURNITURE * 64];

                    if(!Furniture_GetCount(id))
                        return SendErrorMessage(playerid, "This house doesn't have any furniture spawned.");

                    strcat(string, "Model\tDistance\n");
                    foreach (new i : HouseFurnitures[id])
                    {
                        if(FurnitureData[id][i][furnitureUnused]) 
                            format(string, sizeof(string), "%s%s\t(Not placed)\n", string, FurnitureData[id][i][furnitureName]);
                        else 
                            format(string, sizeof(string), "%s%s\t%.2f\n", string, FurnitureData[id][i][furnitureName], GetPlayerDistanceFromPoint(playerid, FurnitureData[id][i][furniturePos][0], FurnitureData[id][i][furniturePos][1], FurnitureData[id][i][furniturePos][2]));

                        ListedFurniture[playerid][count++] = i;
                    }
                    Dialog_Show(playerid, ListedFurniture, DIALOG_STYLE_TABLIST_HEADERS, "House Furniture", string, "Select", "Cancel");
                }
                case 11: Dialog_Show(playerid, House_Abandon, DIALOG_STYLE_MSGBOX, "House Abandon", "Are you sure want to abandon your house?", "Sure", "Back");*/
            }
        }
    }
    return 1;
}

Dialog:House_StructureDestroy(playerid, response, listitem, inputtext[]) {
    if (!response)
        return Dialog_Show(playerid, House_Structure, DIALOG_STYLE_LIST, "House Builder Menu", "Create Structure\nStructure list\n"RED"Reset Structure"WHITE, "Select", "Back");
    
    new houseid = PlayerData[playerid][pEditHouseStructure];

    if (houseid != -1 && (House_PlayerOwn(playerid, houseid) || HouseData[houseid][houseBuilder] == PlayerData[playerid][pID])) {
        HouseStructure_DeleteAll(houseid);
        SendCustomMessage(playerid, "BUILDER", "All structures in this house have been destroyed.");
    }
    return 1;
}

Dialog:House_StructureType(playerid, response, listitem, inputtext[]) {
    if (response) {
        new id = PlayerData[playerid][pInHouse];
        if (id != -1) {
            switch (listitem) {
                case 0: {
                    new
                        count = 0,
                        string[90 * 64];

                    strcat(string, "Model\tDistance\n");
                    foreach (new i : HouseStruct[id]) if (HouseStructure[id][i][structureType] > 0) {
                        format(string, sizeof(string), ORANGE_E"%sStatic\t%.2f\n", string, GetPlayerDistanceFromPoint(playerid, HouseStructure[id][i][structurePos][0], HouseStructure[id][i][structurePos][1], HouseStructure[id][i][structurePos][2]));
                        ListedStructure[playerid][count++] = i;
                    }
                    Dialog_Show(playerid, ListedStructure, DIALOG_STYLE_TABLIST_HEADERS, "Static House Structure", string, "Select", "Cancel");
                }
                case 1: {
                    new
                        count = 0,
                        string[MAX_HOUSE_STRUCTURES * 64];

                    if(!HouseStructure_GetCount(id))
                        return SendErrorMessage(playerid, "This house doesn't have any structures.");

                    strcat(string, "Model\tDistance\n");
                    foreach (new i : HouseStruct[id]) if (HouseStructure[id][i][structureType] == 0) {
                        format(string, sizeof(string), "%s%s\t%.2f\n", string, GetStructureNameByModel(HouseStructure[id][i][structureModel]), GetPlayerDistanceFromPoint(playerid, HouseStructure[id][i][structurePos][0], HouseStructure[id][i][structurePos][1], HouseStructure[id][i][structurePos][2]));
                        ListedStructure[playerid][count++] = i;
                    }
                    Dialog_Show(playerid, ListedStructure, DIALOG_STYLE_TABLIST_HEADERS, "Custom House Structure", string, "Select", "Cancel");
                }
            }
        }
    }
    return 1;
}

Dialog:House_StructureAdd(playerid, response, listitem, inputtext[])
{
    new houseid = PlayerData[playerid][pInHouse];
    if (houseid != -1) 
    {
        if (response)
        {
            static
                Float:x,
                Float:y,
                Float:z,
                Float:angle
            ;

            GetPlayerPos(playerid, x, y, z);
            GetPlayerFacingAngle(playerid, angle);

            if (Inventory_Count(playerid, "Component") < g_aHouseStructure[listitem][e_StructureCost])
                return SendErrorMessage(playerid, "You need %d component to created %s structure", g_aHouseStructure[listitem][e_StructureCost], g_aHouseStructure[listitem][e_StructureName]);

            if(HouseStructure_GetCount(houseid) >= MAX_HOUSE_STRUCTURES)
                return SendErrorMessage(playerid, "You can only have %d structure items in your house.", MAX_HOUSE_STRUCTURES);

            new id = HouseStructure_Add(houseid, g_aHouseStructure[listitem][e_StructureModel], x+1, y, z, 0.0, 0.0, angle);

            if (id == cellmin)
                return SendErrorMessage(playerid, "Server has been reached maximum of house structure");

            Inventory_Remove(playerid, "Component", g_aHouseStructure[listitem][e_StructureCost]);
            PlayerData[playerid][pEditStructure] = id;
            PlayerData[playerid][pEditHouseStructure] = houseid;
            PlayerData[playerid][pEditingMode] = EDIT_OBJECT_HOUSE_STRUCTURE;
            EditingHouseStructure[playerid] = 1;
            EditDynamicObject(playerid, HouseStructure[houseid][id][structureObject]);
            SendCustomMessage(playerid, "HOUSE", "You've been created "YELLOW_E"%s "WHITE_E"structure for "GREEN_E"%d components", g_aHouseStructure[listitem][e_StructureName], g_aHouseStructure[listitem][e_StructureCost]);
        } else Dialog_Show(playerid, House_Structure, DIALOG_STYLE_LIST, "House Builder Menu", "Create Structure\nStructure list\n"RED"Reset Structure"WHITE, "Select", "Back");
    }
    return 1;
}

Dialog:House_StructureRetexture(playerid, response, listitem, inputtext[])
{
    new houseid = PlayerData[playerid][pInHouse];
    if (houseid != -1) 
    {
        new id = GetPVarInt(playerid, "structureObj");
        if (response) 
        {
            new model,color[4];
            new const txdname[32],texture[32];
            if (isnull(inputtext))
                return Dialog_Show(playerid, House_StructureRetexture, DIALOG_STYLE_INPUT, "Retexture House Structure", "Please input the texture name below:\n"YELLOW_E"[model] [txdname] [texture] [opt: alpha] [opt: red] [opt: green] [opt: blue]", "Retexture", "Cancel"), SendErrorMessage(playerid, "Invalid input!");

            if (sscanf(inputtext, "ds[32]s[32]D(0)D(0)D(0)D(0)",model,txdname,texture,color[0],color[1],color[2],color[3]))
                return Dialog_Show(playerid, House_StructureRetexture, DIALOG_STYLE_INPUT, "Retexture House Structure", "Please input the texture name below:\n"YELLOW_E"[model] [txdname] [texture] [opt: alpha] [opt: red] [opt: green] [opt: blue]", "Retexture", "Cancel"), SendErrorMessage(playerid, "Invalid input!");

            if (!IsValidTexture(texture))
                return Dialog_Show(playerid, House_StructureRetexture, DIALOG_STYLE_INPUT, "Retexture House Structure", "Please input the texture name below:\n"YELLOW_E"[model] [txdname] [texture] [opt: alpha] [opt: red] [opt: green] [opt: blue]", "Retexture", "Cancel"), SendErrorMessage(playerid, "Texture model tidak terdaftar dalam database");

            if (Inventory_Count(playerid, "Component") < 10)
                return SendErrorMessage(playerid, "You need 10 components to retexture the house structure");

            HouseStructure[houseid][id][structureMaterial] = GetTextureIndex(texture);
            HouseStructure[houseid][id][structureColor] = RGBAToInt(color[1], color[2], color[3], color[0]);
            HouseStructure_Refresh(id, houseid);
            HouseStructure_Save(id, houseid);

            Inventory_Remove(playerid, "Component", 10);
            SendCustomMessage(playerid, "BUILDER", "You've been retextured the house structure with "YELLOW_E"10 components");
        } else Dialog_Show(playerid, House_Structure, DIALOG_STYLE_LIST, "House Builder Menu", "Create Structure\nStructure list\n"RED"Reset Structure"WHITE, "Select", "Back");
    }
    return 1;
}