#include <UtilsScripts\als.inc>
enum e_server_veh
{
    vehSQLID,
    vehID,
    vehModel,
    vehColor[2],
    Float:vehHealth,
    Float:vehPos[4],
    vehInterior,
    vehVirtualWorld,
    vehLock,
    vehFuel,
    vehType[50],
    vehSave,
};

new VehServer[MAX_VEHICLES][e_server_veh],
    Iterator:DynamicVS<MAX_VEHICLES>;

#define VS:%0(%1) forward %0(%1); public %0(%1)

#define SetVehicleServerData(%0,%1,%2)              VehServer[%0][%1] = %2
#define GetVehicleServerData(%0,%1)                 VehServer[%0][%1]
#define AddVehicleServerData(%0,%1,%2)              VehServer[%0][%1] += %2

#define SetVehicleServerDataArr(%0,%1<%3>,%2)              VehServer[%0][%1][%3] = %2


Function:IsVehicleServer(vehicleid)
{
    foreach(new i : DynamicVS) if(VehServer[i][vehID] == vehicleid) return vehicleid;
    return INVALID_VEHICLE_ID;
}

OGMI:Vehicle_Server()
{
    LoadVehicleServer();
    return 1;
}

OGME:Vehicle_Server()
{
    foreach(new veh_id : DynamicVS) VS_SaveData(veh_id);
    printf("*** [Vehicle Server] Save database.");
    return 1;
}

VS:LoadVehicleServer()
{
    static Cache:result;
    result = mysql_query(sqlcon, sprintf("SELECT * FROM `vehicle_server`"));
    {
        if(cache_num_rows())
        {
            for(new i = 0; i < cache_num_rows(); i++)
            {
                cache_get_value_name(i, "vehType", VehServer[i][vehType]);
                SetVehicleServerData(i, vehSQLID,                   cache_get_field_content_int(i, "vehSQLID"));
                SetVehicleServerData(i, vehModel,                   cache_get_field_content_int(i, "vehModel"));

                SetVehicleServerDataArr(i, vehColor<0>,             cache_get_field_content_int(i, "vehColor"));
                SetVehicleServerDataArr(i, vehColor<1>,             cache_get_field_content_int(i, "vehColor1"));
                
                SetVehicleServerData(i, vehInterior,                cache_get_field_content_int(i, "vehInterior"));
                SetVehicleServerData(i, vehVirtualWorld,            cache_get_field_content_int(i, "vehVirtualWorld"));

                SetVehicleServerData(i, vehLock,                    cache_get_field_content_int(i, "vehLock"));
                cache_get_value_name_float(i, "vehHealth", VehServer[i][vehHealth]);
                SetVehicleServerData(i, vehFuel,                    cache_get_field_content_int(i, "vehFuel"));
                
                
                cache_get_value_name_float(i, "vehPosX", VehServer[i][vehPos][0]);
                cache_get_value_name_float(i, "vehPosY", VehServer[i][vehPos][1]);
                cache_get_value_name_float(i, "vehPosZ", VehServer[i][vehPos][2]);
                cache_get_value_name_float (i, "vehAngle", VehServer[i][vehPos][3]);

                VehServer[i][vehSave] = cache_get_field_content_int(i, "vehSave");
                Iter_Add(DynamicVS, i);
                if(!IsValidVehicle(VehServer[i][vehID])) 
                {
                    VS_Respawn(i); 
                }
            }
            printf("***[MySQL] Loaded a 'vehicle_server' database as much as %i", cache_num_rows());
        }
        else printf("***[MySQL] There is no contents in the database 'vehicle_server`");
    }
    cache_delete(result);
    return 1;
}

VS_Respawn(vehicleid)
{
    if(IsValidVehicle(VehServer[vehicleid][vehID]))
        DestroyVehicle(VehServer[vehicleid][vehID]);

	SetVehicleServerData(vehicleid, vehID, INVALID_VEHICLE_ID);
    VehServer[vehicleid][vehID] = CreateVehicle(VehServer[vehicleid][vehModel], VehServer[vehicleid][vehPos][0], VehServer[vehicleid][vehPos][1], VehServer[vehicleid][vehPos][2], VehServer[vehicleid][vehPos][3], VehServer[vehicleid][vehColor][0], VehServer[vehicleid][vehColor][1], 10001, 0);
	if(IsValidVehicle(VehServer[vehicleid][vehID]))
	{
		SetVehicleVirtualWorld(VehServer[vehicleid][vehID], VehServer[vehicleid][vehVirtualWorld]);
		LinkVehicleToInterior(VehServer[vehicleid][vehID], VehServer[vehicleid][vehInterior]);
		
        SetVehicleHealth(VehServer[vehicleid][vehID], 1000.0);

		if(VehServer[vehicleid][vehLock] == 1) SwitchVehicleDoors(VehServer[vehicleid][vehID], true);
		else SwitchVehicleDoors(VehServer[vehicleid][vehID], false);
        
        VehCore[VehServer[vehicleid][vehID]][vehFuel] = 100;
        format(VehCore[VehServer[vehicleid][vehID]][vehType], 50, VehServer[vehicleid][vehType]);
	}
	return 1;
}

CMD:vsmenu(playerid, params[])
{
    if(!IsPlayerADuty(playerid, 3))
        return PermissionError(playerid);

    new type[24], string[128], cQuery[1024], Float:x,Float:y,Float:z, Float:a;
    GetPlayerPos(playerid,x,y,z);
    GetPlayerFacingAngle(playerid,a);

    if(sscanf(params, "s[24]S()[128]", type, string)) 
        return SendSyntaxMessage(playerid, "/vsmenu [create/delete/save/list]");

    if(!strcmp(type, "create", true))
    {
        new model[32], typeid[50], color1, color2;

        if(sscanf(string, "p<,>s[32]s[50]dd", model, typeid, color1, color2))
            return SendSyntaxMessage(playerid, "/vehicle create [model/vehicle name] [string:type] [color1] [color2]");

        if((model[0] = GetVehicleModelByName(model)) == 0) 
            return SendErrorMessage(playerid, "Invalid vehicle name/id");

        new vehicleid = Iter_Free(DynamicVS);
	    if(vehicleid == -1)
            return SendErrorMessage(playerid, "There was an error when creating a server ID vehicle!");

        if(color1 < 0 || color1 > 255 || color2 < 0 || color2 > 255)
            return SendErrorMessage(playerid, "Color Number can't be below 0 or above 255 !");

        if(model[0] < 400 || model[0] > 611)
            return SendErrorMessage(playerid, "Vehicle Number can't be below 400 or above 611 !");

        Iter_Add(DynamicVS, vehicleid);
        mysql_format(sqlcon, cQuery, sizeof(cQuery), "INSERT INTO vehicle_server SET `vehType` = '%s', `vehModel` = %i, `vehColor` = %i, `vehColor1` = %i, `vehPosX` = %f, `vehPosY` = %f, `vehPosZ` = %f, `vehAngle` = %f", typeid, model[0], color1, color2, x, y, z, a);
        mysql_tquery(sqlcon, cQuery, "VS_Create", "ddsdddffff", playerid, vehicleid, typeid, model[0], color1, color2, x, y, z, a);
    }
    else if(!strcmp(type, "delete", true))
    {
        new slot, veh, query[128];

        if(sscanf(string, "d", slot))
            return SendSyntaxMessage(playerid, "/vehicle [delete] [vehicleid]");

        foreach(new vehicle : DynamicVS) if(VehServer[vehicle][vehID] == slot)
        {
            veh = vehicle;
        }

        SendCustomMessage(playerid, "VEHICLE", "You successfully delete the server vehicle %s(id: %i)", GetVehicleModelName(VehServer[veh][vehModel]), slot);

        mysql_format(sqlcon, query, sizeof(query), "DELETE FROM vehicle_server WHERE `vehSQLID` = %d", VehServer[veh][vehSQLID]);
        mysql_tquery(sqlcon, query);

        if(IsValidVehicle(VehServer[veh][vehID]))
            DestroyVehicle(VehServer[veh][vehID]);

        new tmp_VehicleData[e_server_veh];
        VehServer[veh] = tmp_VehicleData;

        VehServer[veh][vehID] = INVALID_VEHICLE_ID;
        Iter_Remove(DynamicVS, veh);
    }
    else if(!strcmp(type, "list", true))
    {
        new fmt[700] = "#(VSID)\tVehicle Name(VID)\n";
        foreach(new vid : DynamicVS)
        {
            format(fmt, sizeof fmt, "%s%i\t%s(VID: %i)\n", fmt, vid, GetVehicleModelName(VehServer[vid][vehModel]), VehServer[vid][vehID]);
        }
        Dialog_Show(playerid, unused, DIALOG_STYLE_TABLIST_HEADERS, "Vehicle Server list", fmt, "Close", "");
    }
    else if(!strcmp(type, "save", true))
    {
        new save, slot, veh;

        if(sscanf(string, "dd", slot, save))
            return SendSyntaxMessage(playerid, "/vehicle [save] [vehicleid] [0/1]");

        if(!IsValidVehicle(slot)) 
            return SendErrorMessage(playerid, "There is no vehicle ID.");

        foreach(new vehicle : DynamicVS) if(VehServer[vehicle][vehID] == slot) {
            veh = vehicle;
        }
        VehServer[veh][vehSave] = save;
        SendCustomMessage(playerid, "VEHICLE", "You have %s "AQUA_E"%s "WHITE"[vehicleid: %d | DBID: %d]", save ? "Changing this vehicle can be saved" : "Changing this vehicle cannot be stored", GetVehicleModelName(VehServer[veh][vehModel]), slot, VehServer[veh][vehSQLID]);
        new query[128];
        mysql_format(sqlcon, query, sizeof(query), "UPDATE vehicle_server SET `vehSave` = '%i' WHERE `vehSQLID` = %d", save, VehServer[veh][vehSQLID]);
        mysql_tquery(sqlcon, query);
    }
    else if(!strcmp(type, "spawn", true))
    {
        if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) 
            return SendErrorMessage(playerid, "You are not in driving.");

        new vsid = VS_GetID(GetPlayerVehicleID(playerid));
        if(vsid == -1) return SendErrorMessage(playerid, "The vehicle that you vehicle is not a server vehicle.");

        VS_SaveData(vsid);
        VS_Respawn(vsid);
        VehServer[vsid][vehSave] = 0;
        SendCustomMessage(playerid, "VEHICLE", "You store the SPAWN Vehicle Server Id %i position", vsid);
    }
    else if(!strcmp(type, "allrespawn", true))
    {
        foreach(new vsid : DynamicVS)
        {
            VS_Respawn(vsid);
        }
        SendClientMessageToAll(X11_TOMATO, "-> semua kendaraan yang tidak digunakan telah di munculkan kembali");
    }
    return 1;
}

VS_SaveData(vehicleid)
{
    if(VehServer[vehicleid][vehSave] == 0) return 1;
    GetVehicleHealth(VehServer[vehicleid][vehID], VehServer[vehicleid][vehHealth]);
    GetVehiclePos(VehServer[vehicleid][vehID], Array3X(VehServer[vehicleid][vehPos]));
    GetVehicleZAngle(VehServer[vehicleid][vehID], VehServer[vehicleid][vehPos][3]);
    VehServer[vehicleid][vehFuel] = VehCore[VehServer[vehicleid][vehID]][vehFuel];
    new cQuery[1238];
    /*
    mysql_format(sqlcon, cQuery, sizeof(cQuery), "UPDATE `vehicle_server` SET `vehHealth`='%.1f'", VehServer[vehicleid][vehHealth]);
    if(VehServer[vehicleid][vehLock] == 1) mysql_format(sqlcon, cQuery, sizeof(cQuery), "%s, `vehLock`=1", cQuery);
    else mysql_format(sqlcon, cQuery, sizeof(cQuery), "%s, `vehLock`=0", cQuery);
    mysql_format(sqlcon, cQuery, sizeof(cQuery), "%s, `vehFuel`=%i", cQuery, VehServer[vehicleid][vehFuel]);
    mysql_format(sqlcon, cQuery, sizeof(cQuery), "%s, `vehType`='%s'", cQuery, VehServer[vehicleid][vehType]);
    mysql_format(sqlcon, cQuery, sizeof(cQuery), "%s WHERE `vehSQLID` = %d", cQuery, VehServer[vehicleid][vehSQLID]);
    mysql_tquery(sqlcon, cQuery);
    */
    mysql_format(sqlcon, cQuery, sizeof(cQuery), "UPDATE `vehicle_server` SET");
    mysql_format(sqlcon, cQuery, sizeof(cQuery), "%s `vehPosX`=%f", cQuery, VehServer[vehicleid][vehPos][0]);
    mysql_format(sqlcon, cQuery, sizeof(cQuery), "%s, `vehPosY`=%f", cQuery, VehServer[vehicleid][vehPos][1]);
    mysql_format(sqlcon, cQuery, sizeof(cQuery), "%s, `vehPosZ`=%f", cQuery, VehServer[vehicleid][vehPos][2]);
    mysql_format(sqlcon, cQuery, sizeof(cQuery), "%s, `vehAngle`=%f", cQuery, VehServer[vehicleid][vehPos][3]);
    mysql_format(sqlcon, cQuery, sizeof(cQuery), "%s WHERE `vehSQLID` = %d", cQuery, VehServer[vehicleid][vehSQLID]);
    mysql_tquery(sqlcon, cQuery);

    // if(IsValidVehicle(VehicleServer[vehicleid][Vehicle])) DestroyVehicle(VehicleServer[vehicleid][Vehicle]);
    // new tmp_VehicleServer[vehicledata];
    // VehicleServer[vehicleid] = tmp_VehicleData;

    // VehicleServer[vehicleid][Vehicle] = INVALID_VEHICLE_ID;
    // new current = vehicleid;
    // Iter_SafeRemove(VehicleServer, current, vehicleid);
	return 1;
}

function VS_Create(playerid, vehicleid, const type[], model, color1, color2, Float:x, Float:y, Float:z, Float:a)
{
	VehServer[vehicleid][vehSQLID] = cache_insert_id();
    VehServer[vehicleid][vehModel] = model;
    strcat(VehServer[vehicleid][vehType], type);
	VehServer[vehicleid][vehColor][0] = color1;
	VehServer[vehicleid][vehColor][1] = color2;
    VehServer[vehicleid][vehPos][0] = x;
    VehServer[vehicleid][vehPos][1] = y;
    VehServer[vehicleid][vehPos][2] = z;
    VehServer[vehicleid][vehPos][3] = a;
    
	VehServer[vehicleid][vehLock] = 0;
	VehServer[vehicleid][vehHealth] = 1000;
    VehServer[vehicleid][vehSave] = 1;
	VehServer[vehicleid][vehFuel] = 100;
	VehServer[vehicleid][vehInterior] = VehServer[vehicleid][vehVirtualWorld] = 0;
    SendCustomMessage(playerid, "VEHICLE", ""YELLOW_E "You have created the vehicle "AQUA_E"%s", GetVehicleModelName(VehServer[vehicleid][vehModel]));
    VS_Respawn(vehicleid);
    return 1;
}


VS_GetID(vehicleid)
{
    if (!IsValidVehicle(vehicleid)) return -1;
    foreach (new i : DynamicVS) if (VehServer[i][vehID] == vehicleid) {
        return i;
    }
    return -1;
}