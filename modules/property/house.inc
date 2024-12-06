OGMI:House_Init()
{
	print("\n==========[House System Loaded]=========");
	print(" \n");
	print("==========================================\n");
	
    Iter_Init(HouseStruct);
	mysql_tquery(sqlcon,"SELECT * from `houses`","LoadHouses");
	
	return 1;
}

OGME:House_Exit()
{
	DestroyAllDynamicMapIcons();
	DestroyAllDynamic3DTextLabels();
	DestroyAllDynamicPickups();
	return 1;
}

hook OnPlayerConnect(playerid)
{
	SetPVarInt(playerid, "DeletingHouseID", -1);
	return 0;
}

/*
    Other functions
*/
PlayerToPoint(Float:radi, playerid, Float:x, Float:y, Float:z)
{
  if(IsPlayerConnected(playerid))
  {
    new Float:oldposx, Float:oldposy, Float:oldposz;
    new Float:tempposx, Float:tempposy, Float:tempposz;
    GetPlayerPos(playerid, oldposx, oldposy, oldposz);
    tempposx = (oldposx -x);
    tempposy = (oldposy -y);
    tempposz = (oldposz -z);
    if (((tempposx < radi) && (tempposx > -radi)) && ((tempposy < radi) && (tempposy > -radi)) && ((tempposz < radi) && (tempposz > -radi)))
    {
      return 1;
    }
  }
  return 0;
}

GetNearbyHouse(playerid)
{
	for(new i=0; i<MAX_HOUSES; i++)
  	{
      if(PlayerToPoint(1.5, playerid, HouseData[i][houseExterior][0], HouseData[i][houseExterior][1], HouseData[i][houseExterior][2]))
      	return i;
  	}
	return -1;
}
GetHouseExitPoint(playerid)
{
    for(new i=0; i<MAX_HOUSES; i++)
  	{
      if(PlayerToPoint(1.5, playerid, HouseData[i][houseInterior][0], HouseData[i][houseInterior][1], HouseData[i][houseInterior][2]))
      	return i;
  	}
	return -1;
}

function House_PlayerOwn(playerid, houseid)
{
    if(!strcmp(GetName(playerid), HouseData[houseid][houseOwner], true)) return 1;
    return 0;
}
/*
    main function houses
*/
public LoadHouses()
{
	    
    if(!cache_num_rows())
		return printf("*** [House] 0 Houses were loaded.");

	new Label[128], rows = cache_num_rows();
	

	TotalHouses = rows;
	for(new i=0;i<rows;i++)
	{
        cache_get_value_name_int(i, "ID", HouseData[i][houseSQLID]);

		cache_get_value_name(i, "Address", HouseData[i][houseAddress], 35);
		cache_get_value_name(i, "Description",HouseData[i][houseDesc], 128);
		cache_get_value_name(i, "Owner", HouseData[i][houseOwner], MAX_PLAYER_NAME+1);

  		cache_get_value_name_int(i, "Owned", bool:HouseData[i][houseOwned]);
        cache_get_value_name_int(i, "Locked", bool:HouseData[i][houseLocked]);
        cache_get_value_name_int(i, "Price", HouseData[i][housePrice]);
        
        cache_get_value_name_float(i, "ExteriorX", HouseData[i][houseExterior][0]);
        cache_get_value_name_float(i, "ExteriorY", HouseData[i][houseExterior][1]);
        cache_get_value_name_float(i, "ExteriorZ", HouseData[i][houseExterior][2]);

        cache_get_value_name_int(i, "InteriorE", HouseData[i][houseInt][0]);
        cache_get_value_name_int(i, "InteriorI", HouseData[i][houseInt][1]);
        
        cache_get_value_name_float(i, "InteriorX", HouseData[i][houseInterior][0]);
        cache_get_value_name_float(i, "InteriorY", HouseData[i][houseInterior][1]);
        cache_get_value_name_float(i, "InteriorZ", HouseData[i][houseInterior][2]);
        
        cache_get_value_name_int(i, "Custom_Interior", bool:HouseData[i][houseCustInt]);

        Iter_Add(House, i);
		//Assigning the Values and all:
		HouseData[i][housePickup] = CreateDynamicPickup(PICKUP_NEWHOUSE, 1, HouseData[i][houseExterior][0],  HouseData[i][houseExterior][1],  HouseData[i][houseExterior][2], 0, 0);
		HouseData[i][houseExists] = true;

		if(HouseData[i][houseOwned])
		{
            format(Label, sizeof(Label), "Owner: %s\n%s",HouseData[i][houseOwner],HouseData[i][houseAddress]);
		}
		else
		{
		    format(Label, sizeof(Label), "%s\nThis House is for sale.\n%s\nPrice: $%d\nDescription: %s\nUse /buyhouse to Buy.",HouseData[i][houseOwner],HouseData[i][houseAddress], HouseData[i][housePrice], HouseData[i][houseDesc]);
            HouseData[i][houseMapIcon] = CreateDynamicMapIcon(HouseData[i][houseExterior][0], HouseData[i][houseExterior][1], HouseData[i][houseExterior][2], 31, 1);
		}
		HouseData[i][house3DText] = CreateDynamic3DTextLabel(Label, COLOR_GREEN, HouseData[i][houseExterior][0], HouseData[i][houseExterior][1], HouseData[i][houseExterior][2]+0.5,30.0, .testlos = 1, .streamdistance = 30.0);
        new str[120];
		format(str, sizeof(str), "SELECT * FROM `house_struct` WHERE `HouseID` = '%d'", HouseData[i][houseSQLID]);
        mysql_tquery(sqlcon, str, "OnLoadHouseStructure", "d", i);
	}
	printf("*** [House] Successfully load house %i rows",rows);
	return 1;
}

public OnCreateHouse(playerid, houseID, price, Float:HX, Float:HY, Float:HZ)
{
	TotalHouses++;
	HouseData[houseID][houseExterior][0] = HX;
	HouseData[houseID][houseExterior][1] = HY;
	HouseData[houseID][houseExterior][2] = HZ;
	HouseData[houseID][houseInt][0] = 0;
	HouseData[houseID][houseOwned] = false;
	HouseData[houseID][houseSQLID] = cache_insert_id();
	HouseData[houseID][housePrice] = price;
	HouseData[houseID][houseExists] = true;

	new Label[128];
	format(Label, sizeof(Label), "%s\nThis House is for sale.\n%s\nPrice: $%d\nDescription: %s\nUse /buyhouse to Buy.",HouseData[houseID][houseOwner],HouseData[houseID][houseAddress], HouseData[houseID][housePrice], HouseData[houseID][houseDesc]);

	HouseData[houseID][house3DText] = CreateDynamic3DTextLabel(Label, COLOR_GREEN, HouseData[houseID][houseExterior][0], HouseData[houseID][houseExterior][1], HouseData[houseID][houseExterior][2]+0.5,30.0, .testlos = 1, .streamdistance = 30.0);
	HouseData[houseID][houseMapIcon] = CreateDynamicMapIcon(HouseData[houseID][houseExterior][0], HouseData[houseID][houseExterior][1], HouseData[houseID][houseExterior][2], 31, 1);
	HouseData[houseID][housePickup] = CreateDynamicPickup(PICKUP_NEWHOUSE, 1, HouseData[houseID][houseExterior][0],  HouseData[houseID][houseExterior][1],  HouseData[houseID][houseExterior][2], 0, 0);
	
	CreateHouseInterior(houseID);
	SendAdminMessage(X11_TOMATO, "AdmCmd: %s have Created a House. House ID: %d. Total Houses Now: %d", ReturnName(playerid), houseID, TotalHouses);
	return 1;
}

public OnDeleteHouse(playerid, houseID)
{
	TotalHouses--;
	if(TotalHouses <= 0) TotalHouses = 0;
	HouseData[houseID][houseExists] = false;
	HouseData[houseID][houseSQLID] = 0;

    Iter_Remove(House, houseID);
	DestroyDynamicPickup(HouseData[houseID][housePickup]);
	DestroyDynamicMapIcon(HouseData[houseID][houseMapIcon]);
	DestroyDynamic3DTextLabel(HouseData[houseID][house3DText]);

	SendAdminMessage(X11_TOMATO, "AdmCmd: %s have Deleted a House. House ID: %d. TotalHouses Now: %d", ReturnName(playerid), houseID,TotalHouses);
	return 1;
}

public OnDeleteAllHouses(playerid)
{
	foreach(new houseID : House)
	{
		HouseData[houseID][houseExists] = false;
		HouseData[houseID][houseSQLID] = 0;
		DestroyDynamicPickup(HouseData[houseID][housePickup]);
		DestroyDynamicMapIcon(HouseData[houseID][houseMapIcon]);
		DestroyDynamic3DTextLabel(HouseData[houseID][house3DText]);
        Iter_Remove(House, houseID);
	}
	TotalHouses=0;
	SendAdminMessage(X11_TOMATO, "AdmCmd: %s have Deleted all Houses.", ReturnName(playerid));
	return 1;
}

public OnUpdateHouseExterior(playerid, HouseID, Float:HX, Float:HY, Float:HZ, Interior)
{
	HouseData[HouseID][houseExterior][0] = HX;
	HouseData[HouseID][houseExterior][1] = HY;
	HouseData[HouseID][houseExterior][2] = HZ;
	
	HouseData[HouseID][houseInt][0] = Interior;

	DestroyDynamicPickup(HouseData[HouseID][housePickup]);
	DestroyDynamicMapIcon(HouseData[HouseID][houseMapIcon]);
	DestroyDynamic3DTextLabel(HouseData[HouseID][house3DText]);
	
	new Label[128];
	if(HouseData[HouseID][houseOwned]) format(Label, sizeof(Label), "Owner: %s\n%s",HouseData[HouseID][houseOwner],HouseData[HouseID][houseAddress]);
	else format(Label, sizeof(Label), "%s\nThis House is for sale.\n%s\nPrice: $%d\nDescription: %s\nUse /buyhouse to Buy.",HouseData[HouseID][houseOwner],HouseData[HouseID][houseAddress], HouseData[HouseID][housePrice], HouseData[HouseID][houseDesc]);

	HouseData[HouseID][house3DText] = CreateDynamic3DTextLabel(Label, COLOR_GREEN, HouseData[HouseID][houseExterior][0], HouseData[HouseID][houseExterior][1], HouseData[HouseID][houseExterior][2]+0.5,30.0, .testlos = 1, .streamdistance = 30.0);
	HouseData[HouseID][houseMapIcon] = CreateDynamicMapIcon(HouseData[HouseID][houseExterior][0], HouseData[HouseID][houseExterior][1], HouseData[HouseID][houseExterior][2], 31, 1);
	HouseData[HouseID][housePickup] = CreateDynamicPickup(PICKUP_NEWHOUSE, 1, HouseData[HouseID][houseExterior][0],  HouseData[HouseID][houseExterior][1],  HouseData[HouseID][houseExterior][2], 0, 0);

	SetPVarInt(playerid, "EditingHouse", -1);

	SendAdminMessage(X11_TOMATO, "AdmCmd: %s have successfully edited this house's exterior", ReturnName(playerid));
	return 1;
}

public OnUpdateHouseInterior(playerid, HouseID, Float:HX, Float:HY, Float:HZ, Interior)
{
    HouseData[HouseID][houseInterior][0] = HX;
	HouseData[HouseID][houseInterior][1] = HY;
	HouseData[HouseID][houseInterior][2] = HZ;

	HouseData[HouseID][houseInt][1] = Interior;
	
	SetPVarInt(playerid, "EditingHouse", -1);
	SendAdminMessage(X11_TOMATO, "AdmCmd: %s have successfully edited this house's Interior.", ReturnName(playerid));
	return 1;
}

public OnUpdateHouseDesc(playerid, HouseID, desc[])
{
	format(HouseData[HouseID][houseDesc], 128 ,"%s",desc);
	new Label[128];
	
	if(HouseData[HouseID][houseOwned]) format(Label, sizeof(Label), "Owner: %s\n%s",HouseData[HouseID][houseOwner],HouseData[HouseID][houseAddress]);
	else format(Label, sizeof(Label), "%s\nThis House is for sale.\n%s\nPrice: $%d\nDescription: %s\nUse /buyhouse to Buy.",HouseData[HouseID][houseOwner],HouseData[HouseID][houseAddress], HouseData[HouseID][housePrice], HouseData[HouseID][houseDesc]);

	UpdateDynamic3DTextLabelText(HouseData[HouseID][house3DText], COLOR_GREEN, Label);
	SetPVarInt(playerid, "EditingHouse", -1);
	SendAdminMessage(X11_TOMATO, "AdmCmd: %s have successfully edited this house's Description.", ReturnName(playerid));
	return 1;
}

public OnUpdateHouseOwner(playerid, HouseID, owner[])
{
	DestroyDynamicMapIcon(HouseData[HouseID][houseMapIcon]);
    format(HouseData[HouseID][houseOwner],25,"%s",owner);
	new Label[128];
	
	if(!strcmp(owner, "The State", true))
		HouseData[HouseID][houseOwned] = false;

	else HouseData[HouseID][houseOwned] = true;

	if(HouseData[HouseID][houseOwned]) format(Label, sizeof(Label), "Owner: %s\n%s",HouseData[HouseID][houseOwner],HouseData[HouseID][houseAddress]);
	else
	{
		format(Label, sizeof(Label), "%s\nThis House is for sale.\n%s\nPrice: $%d\nDescription: %s\nUse /buyhouse to Buy.",HouseData[HouseID][houseOwner],HouseData[HouseID][houseAddress], HouseData[HouseID][housePrice], HouseData[HouseID][houseDesc]);
        HouseData[HouseID][houseMapIcon] = CreateDynamicMapIcon(HouseData[HouseID][houseExterior][0], HouseData[HouseID][houseExterior][1], HouseData[HouseID][houseExterior][2], 31, 1);
	}
	UpdateDynamic3DTextLabelText(HouseData[HouseID][house3DText], COLOR_GREEN, Label);
	SetPVarInt(playerid, "EditingHouse", -1);
	if(GetPVarInt(playerid, "IsHouseAdmin")) SendAdminMessage(playerid, "AdmCmd: %s have successfully edited this house's Owner.", ReturnName(playerid));
	else SendServerMessage(playerid, "You have successfully bought this house!");
	return 1;
}

public OnUpdateHousePrice(playerid, HouseID, price)
{
	HouseData[HouseID][housePrice] = price;
	
	if(!HouseData[HouseID][houseOwned])
	{
	    new Label[128];
		format(Label, sizeof(Label), "%s\nThis House is for sale.\n%s\nPrice: $%d\nDescription: %s\nUse /buyhouse to Buy.",HouseData[HouseID][houseOwner],HouseData[HouseID][houseAddress], HouseData[HouseID][housePrice], HouseData[HouseID][houseDesc]);
        UpdateDynamic3DTextLabelText(HouseData[HouseID][house3DText], COLOR_GREEN, Label);
	}
	SetPVarInt(playerid, "EditingHouse", -1);
	SendAdminMessage(X11_TOMATO, "AdmCmd: %s have successfully edited this house's Price.", ReturnName(playerid));
	return 1;
}

public OnHouseLeft(playerid, HouseID)
{
	new str[64], HouseCost = HouseData[HouseID][housePrice]/2;
	format(str, sizeof(str), "Are you sure you want to leave this house? You wil recieve $%d. (Can not be undo).", HouseCost);

	new dialog[dialog_data];
	await_arr(dialog) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_MSGBOX, "Are you Sure?", str, "{00FF00}Accept", "{FF0000}Reject");

	if(dialog[Response])
	{
		HouseData[HouseID][houseOwned] = false;
		format(HouseData[HouseID][houseOwner],25, "The State");

		new Label[128];
		SendCustomMessage(playerid, "HOUSE", "You Sold your house to the state for {FF9911}%s.", FormatNumber(HouseCost));
		GiveMoney(playerid, HouseCost, ECONOMY_ADD_SUPPLY);

		HouseStructure_DeleteAll(HouseID);
		
		format(Label, sizeof(Label), "%s\nThis House is for sale.\n%s\nPrice: %s\nUse '/buy' to Buy.",HouseData[HouseID][houseOwner],HouseData[HouseID][houseAddress], FormatNumber(HouseData[HouseID][housePrice]));
		UpdateDynamic3DTextLabelText(HouseData[HouseID][house3DText], COLOR_GREEN, Label);
	}
	else return SendErrorMessage(playerid, "You chose not to sell the house.");
	return 1;
}

public OnHouseSold(playerid, targetid, HouseID, price)
{
    new str[100];
    new name[MAX_PLAYER_NAME+1]; 
    GetPlayerName(targetid, name, sizeof(name));
    format(str, sizeof(str), "You offered {00FFFF}%s {FFFFFF}to buy your house for {FFFF00}$%d{FFFFFF}. Waiting for their reply.",name, price);
    SendClientMessage(playerid, -1, str);
    GetPlayerName(playerid, name, sizeof(name));
	format(str, sizeof(str), "%s wants to sell you his house for $%d. Do you accept?", name, price);
	ShowPlayerDialog(targetid, DIALOG_HOUSE_ACCEPT, DIALOG_STYLE_MSGBOX,"{FFFFFF}Are you sure?",str,"{00FF00}Accept","{FF0000}Reject");
	SetPVarInt(targetid, "HouseBuying",HouseID);
	SetPVarInt(targetid, "HousePrice",price);
	SetPVarInt(targetid, "BuyingFrom",playerid);
	return 1;
}

public SendPlayerInside(playerid, HouseID)
{
	new name[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, name, sizeof(name));

	if(HouseID == INVALID_ITER_ID) {
		SendErrorMessage(playerid,"You need to be at a house door.");
		return 0;
	}
    if(HouseData[HouseID][houseLocked] && HouseData[HouseID][houseOwned] &&!strcmp(name, HouseData[HouseID][houseOwner])) {
		SendErrorMessage(playerid, "This is your house but its locked. Use /hlock to unlock it.");
		return 0;
	}
	if(HouseData[HouseID][houseLocked]) {
		SendClientMessage(playerid, 0xCCAA11FF,"This House is locked.");
		return 0;
	}

	new Float:hx = HouseData[HouseID][houseInterior][0], Float:hy = HouseData[HouseID][houseInterior][1], Float:hz = HouseData[HouseID][houseInterior][2], hint=HouseData[HouseID][houseInt][1], hvw=HouseData[HouseID][houseSQLID];
	SetPlayerPositionEx(playerid, hx, hy, hz, 180);
	SetPlayerInterior(playerid, hint);
	SetPlayerVirtualWorld(playerid, hvw);
	SetPVarInt(playerid, "InsideHouse", HouseID);
    PlayerData[playerid][pInHouse] = HouseID;
	return 1;
}

public SendPlayerOutside(playerid, HouseID)
{
	if(HouseID != INVALID_ITER_ID)
	{
		new Float:hx = HouseData[HouseID][houseExterior][0], Float:hy = HouseData[HouseID][houseExterior][1], Float:hz = HouseData[HouseID][houseExterior][2], hint=HouseData[HouseID][houseInt][0];
		SetPlayerPos(playerid, hx, hy, hz);
		SetPlayerInterior(playerid, hint);
		SetPlayerVirtualWorld(playerid, 0);
		PlayerData[playerid][pInHouse] = -1;
	}
	else SendClientMessage(playerid, 0xFF3300FF, "You need to be at house exit.");
	return 1;
}

/*
    Command house
*/
CMD:hmenu(playerid, params[])
{
    new opt[20], arg[123];
    if(sscanf(params, "s[20]S()[123]", opt, arg))
        return SendSyntaxMessage(playerid, "/hmenu [menu/lock]");

    switch(HashStr(opt))
    {
        case HS<menu>:
        {
            Dialog_Show(playerid, House_Menu, DIALOG_STYLE_LIST, "House Menu", "Give access to builders", "Select", "Cancel");
        }
		case HS<lock>:
		{
   			new HouseID = GetNearbyHouse(playerid);
			if(HouseID == INVALID_ITER_ID) return SendErrorMessage(playerid,"You need to be at your owned house.");
			new name[MAX_PLAYER_NAME+1];
			GetPlayerName(playerid, name, sizeof(name));
			if(strcmp(name, HouseData[HouseID][houseOwner]))
			{
				SendErrorMessage(playerid, "You need to be at your owned house.");
				return 1;
			}
			if(HouseData[HouseID][houseLocked])
			{
					HouseData[HouseID][houseLocked] = false;
					SendCustomMessage(playerid, "HOUSE", "You have {FF0000}unlocked{FFFF00} your house.");
			}
			else
			{
					HouseData[HouseID][houseLocked] = true;
					SendCustomMessage(playerid, "HOUSE", "You have {FF0000}locked{FFFF00} your house.");
			}
		}
    }
    return 1;
}

CMD:hbmenu(playerid, params[]) {
    static
        id = -1;

    if ((id = PlayerData[playerid][pInHouse]) != -1) {
        if (HouseData[id][houseBuilder] == PlayerData[playerid][pID] || House_PlayerOwn(playerid, id)) {
            yield 1;
			static dialog[dialog_data];
			await_arr(dialog) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_LIST, "House Builder Menu", "Managed Structure", "Select", "Cancel");
			if(dialog[Response])
			{
				switch(HashStr(dialog[Inputtext]))
				{
					case HS<Managed Structure>:
					{
						Dialog_Show(playerid, House_Structure, DIALOG_STYLE_LIST, "House Builder Menu", "Create Structure\nStructure list\n"RED"Reset Structure"WHITE, "Select", "Back");
					}
				}
			}
            return 1;
        }
        SendErrorMessage(playerid, "You don't have any permission to access this house structures");
        return 1;
    }
    SendErrorMessage(playerid, "You're not in any house.");
    return 1;
}

CMD:houseamenu(playerid, params[])
{
	if(!IsPlayerADuty(playerid, OWNER)) return PermissionError(playerid);

	static option[32], arg[123];
	if(sscanf(params, "s[32]S()[123]", option, arg))
		return SendSyntaxMessage(playerid, "/houseamenu [create/delete/alldelete/]");

	switch(HashStr(option))
	{
		case HS<create>:
		{
			new desc[128] = "None desc", price, type, houseID = Iter_Free(House);
			if(houseID == INVALID_ITER_ID) return SendErrorMessage(playerid, "cant add more houses.");
			if(sscanf(arg, "ii", price, type)) return SendSyntaxMessage(playerid, "/houseamenu create [price] [type]");


			new Float:PX, Float:PY, Float:PZ, query[512],zone[28], add[35];
			GetPlayerPos(playerid, PX, PY, PZ);
			GetPlayer2DZone(playerid, zone, sizeof(zone));

			HouseData[houseID][houseInterior][0] = arrHouseInteriors[type][eHouseX];
			HouseData[houseID][houseInterior][1] = arrHouseInteriors[type][eHouseY];
			HouseData[houseID][houseInterior][2] = arrHouseInteriors[type][eHouseZ];
			HouseData[houseID][houseInt][1] = arrHouseInteriors[type][eHouseInterior];
			HouseData[houseID][houseType] = type + 1;
				
			Iter_Add(House, houseID);
			format(add, sizeof(add), "%d, %s, San Andreas",houseID,zone);
			strmid(HouseData[houseID][houseAddress], add, 0, sizeof(add), 255);
			strmid(HouseData[houseID][houseDesc], desc, 0, sizeof(desc), 255);
			strmid(HouseData[houseID][houseOwner], "The State", 0, 9, 255);
			mysql_format(sqlcon, query, sizeof(query), "INSERT INTO `houses`(`Type`, `ExteriorX`,`ExteriorY`, `ExteriorZ`, `InteriorX`, `InteriorY`, `InteriorZ`, `InteriorI`, `Address`,`Price`, `Description`) VALUES (%i, %f, %f, %f, %f, %f, %f, %i, '%s', %i,'%s')", type+1, PX, PY, PZ, HouseData[houseID][houseInterior][0], HouseData[houseID][houseInterior][1], HouseData[houseID][houseInterior][2], HouseData[houseID][houseInt][1], add,price, desc);
			mysql_tquery(sqlcon, query, "OnCreateHouse", "iiifff", playerid, houseID, price, PX, PY, PZ);
		}
		case HS<delete>:
		{
			new HouseID = GetNearbyHouse(playerid);
			if(HouseID == INVALID_ITER_ID) return SendErrorMessage(playerid, "You need to be at one of the houses to delete it.");
			new Query[60];
			mysql_format(sqlcon, Query, sizeof(Query), "DELETE FROM `houses` WHERE ID = %i", HouseData[HouseID][houseSQLID]);
			mysql_tquery(sqlcon, Query, "OnDeleteHouse","ii",playerid,HouseID);
		}
		case HS<alldelete>:
		{
			if(TotalHouses == 0) return SendErrorMessage(playerid, "There are already 0 houses in database.");
			new Query[60];
			mysql_format(sqlcon, Query, sizeof(Query), "DELETE FROM `houses`");
			mysql_tquery(sqlcon, Query, "OnDeleteAllHouses","i",playerid);
		}
		case HS<edit>:
		{
			ShowPlayerDialog(playerid, DIALOG_EDIT_HOUSE_1, DIALOG_STYLE_INPUT,"Enter House ID","Enter a House ID you want to edit.", "Proceed", "Cancel");
		}
	}
	return 1;
}

CMD:househelp(playerid, params[])
{
	new playercmds[128];
	format(playercmds, sizeof(playercmds)," Player Commands: /hlock["YELLOW"Lock/Unlock House"WHITE"], /enter["YELLOW"Ente/Exit House"WHITE"]");
    SendClientMessage(playerid,COLOR_CLIENT, playercmds);
	format(playercmds, sizeof(playercmds),"-> "WHITE"/leavehouse["RED"Sell House to The State for Half the price"WHITE"] /sellhouse ["GREEN"To sell your house to another player."WHITE"]");
	SendClientMessage(playerid,COLOR_CLIENT, playercmds);
	SendClientMessage(playerid,COLOR_CLIENT, "-> " WHITE"/hbmenu ["YELLOW"Menu for home decoration"WHITE"]");
    if(PlayerData[playerid][pAdmin] >= OWNER) SendClientMessage(playerid, COLOR_CLIENT, "Admin Commands:"WHITE" /houseamenu");
	return 1;
}

/*
CMD:buyhouse(playerid, params[])
{
    new HouseID = GetNearbyHouse(playerid);
    if(HouseID == INVALID_ITER_ID) return SendErrorMessage(playerid, "You need to be at a house to buy it.");
	if(HouseData[HouseID][houseOwned]) return SendErrorMessage(playerid, "You need to be at a house which is on sale.");
	
    if(GetMoney(playerid) < HouseData[HouseID][housePrice])
        return SendErrorMessage(playerid, "You don't have enough money to buy a house property.");

	new query[128], name[MAX_PLAYER_NAME+1], housecost = HouseData[HouseID][housePrice];
	GiveMoney(playerid, -housecost);
	GetPlayerName(playerid, name, sizeof(name));

	mysql_format(sqlcon, query, sizeof(query), "UPDATE `houses` SET Owner='%s', Owned=1 WHERE ID=%d",name,HouseData[HouseID][houseSQLID]);
	mysql_tquery(sqlcon,query,"OnUpdateHouseOwner","iis",playerid, HouseID, name);
	return 1;
}*/

CMD:leavehouse(playerid, params[])
{
	new HouseID = GetNearbyHouse(playerid);
	if(HouseID == INVALID_ITER_ID) return SendErrorMessage(playerid, "You need to be at a house to buy it.");
	if(!HouseData[HouseID][houseOwned]) return SendErrorMessage(playerid, "You need to be at a house which is not already on sale.");
	new name[MAX_PLAYER_NAME+1], HouseOwner[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, name, sizeof(name));
	format(HouseOwner, sizeof(HouseOwner), "%s",HouseData[HouseID][houseOwner]);
	if(strcmp(HouseOwner, name)) return SendErrorMessage(playerid, "You need to be at a house that you own.");
	
	new query[128];
	mysql_format(sqlcon,query, sizeof(query), "UPDATE `houses` SET Owner='The State',Owned=0 WHERE ID = %d", HouseData[HouseID][houseSQLID]);
	mysql_tquery(sqlcon,query, "OnHouseLeft","ii",playerid, HouseID);
	return 1;
	
}

CMD:sellhouse(playerid, params[])
{
	new HouseID = GetNearbyHouse(playerid);
	if(HouseID == INVALID_ITER_ID) return SendErrorMessage(playerid, "You need to be at a house to buy it.");
	if(!HouseData[HouseID][houseOwned]) return SendErrorMessage(playerid, "You need to be at a house which is not already on sale.");

	new name[MAX_PLAYER_NAME+1], HouseOwner[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid, name, sizeof(name));
	format(HouseOwner, sizeof(HouseOwner), "%s",HouseData[HouseID][houseOwner]);
	if(strcmp(HouseOwner, name)) return SendErrorMessage(playerid, "You need to be at a house that you own.");

	new targetid, price, Float:px, Float:py, Float:pz;
	if(sscanf(params,"ui",targetid,price)) return SendSyntaxMessage(playerid, "/sellhouse [playerid/username][price]");
	GetPlayerName(targetid, name, sizeof(name));
	GetPlayerPos(targetid, px, py, pz);
	if(!PlayerToPoint(5.0,playerid, px, py, pz)) return SendErrorMessage(playerid, "You are not close enough with that player.");
	
	new query[128];
	mysql_format(sqlcon,query, sizeof(query), "UPDATE `houses` SET Owner='%s',Owned=1 WHERE ID = %d",name,HouseData[HouseID][houseSQLID]);
	mysql_tquery(sqlcon,query, "OnHouseSold","iiii", playerid, targetid, HouseID, price);
	return 1;
}

/*
    Dialog house
*/
hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
		case DIALOG_EDIT_HOUSE_1:
		{
			if(!response) return SendServerMessage(playerid, "You canceled house edditing.");
			if(!IsNumeric(inputtext)) return ShowPlayerDialog(playerid, DIALOG_EDIT_HOUSE_1, DIALOG_STYLE_INPUT,"Enter House ID","{FF0000}!{FFFFFF}You have to enter a number.", "Proceed", "Go Back");
			new hid = strval(inputtext);
			if(!HouseData[hid][houseExists]) return SendErrorMessage(playerid, "That house doesnt exist.");
			SetPVarInt(playerid, "EditingHouse",hid);

			SendServerMessage(playerid, "You are now edditing, House ID: {00FF13}%d",hid);

			new str[65];
			format(str, sizeof(str),"House ID:{FF0000}%d",hid);
			ShowPlayerDialog(playerid, DIALOG_EDIT_HOUSE_2,DIALOG_STYLE_LIST,str,"Change Exterior\nChange Interior\nChange Description\nSet Owner\nSet Price","Select","Back");
		}
		case DIALOG_EDIT_HOUSE_2:
		{
            if(!response){
            SetPVarInt(playerid, "EditingHouse", -1);
			return SendServerMessage(playerid, "You canceled house edditing."); }
			
			switch(listitem)
			{
				case 0:
				{
					ShowPlayerDialog(playerid, DIALOG_EDIT_EXTERIOR,DIALOG_STYLE_MSGBOX,"Change House Exterior: ","Do you want to change exterior?","Yes","Cancel");
				}
				case 1:
				{
					ShowPlayerDialog(playerid, DIALOG_EDIT_INTERIOR,DIALOG_STYLE_MSGBOX,"Change House Interior: ","Do you want to change interior?","Yes","Cancel");
				}
				case 2:
				{
					ShowPlayerDialog(playerid, DIALOG_EDIT_DESC,DIALOG_STYLE_INPUT,"Change Description for House: ", "Enter new description of the house: ","Update","Cancel");
				}
				case 3:
				{
					ShowPlayerDialog(playerid, DIALOG_EDIT_OWNER, DIALOG_STYLE_INPUT,"Change House Owner: ", "Enter the name of new Owner: \n({0000FF}Hint:Type \"{000088}The State{FFFFFF}\" without quotation marks to set House Owned to false.{FFFFFF})","Update","Cancel");
				}
				case 4:
				{
					ShowPlayerDialog(playerid, DIALOG_EDIT_PRICE, DIALOG_STYLE_INPUT,"Change House Price: ", "Enter New Price for the house: ","Update","Cancel");
				}

			}

		}
		//Dialog Exterior Editing------------------------
		case DIALOG_EDIT_EXTERIOR:
		{
            if(!response){
            SetPVarInt(playerid, "EditingHouse", -1);
			return SendErrorMessage(playerid, "You canceled house edditing.");
            }

            new Float:px, Float:py, Float:pz, pint=GetPlayerInterior(playerid), hid=GetPVarInt(playerid, "EditingHouse"), query[256];
            GetPlayerPos(playerid, px, py, pz);
            mysql_format(sqlcon,query,sizeof(query),"UPDATE `houses` SET ExteriorX=%f, ExteriorY=%f, ExteriorZ=%f, InteriorE=%d WHERE ID=%d", px, py, pz, pint, HouseData[hid][houseSQLID]);
            mysql_tquery(sqlcon,query,"OnUpdateHouseExterior","ifffii",playerid, hid,  px, py, pz, pint);
		}
		//----------------------------------------------------
		//Dialog Interior Editing-----------------------------
		case DIALOG_EDIT_INTERIOR:
		{
            if(!response){
            SetPVarInt(playerid, "EditingHouse", -1);
			return SendErrorMessage(playerid, "You canceled house edditing.");
            }

            new Float:px, Float:py, Float:pz, pint=GetPlayerInterior(playerid), hid=GetPVarInt(playerid, "EditingHouse"), query[256];
            GetPlayerPos(playerid, px, py, pz);
            mysql_format(sqlcon,query,sizeof(query),"UPDATE `houses` SET InteriorX=%f, InteriorY=%f, InteriorZ=%f, InteriorI=%d WHERE ID=%d", px, py, pz, pint, HouseData[hid][houseSQLID]);
            mysql_tquery(sqlcon,query, "OnUpdateHouseInterior", "ifffii", playerid, hid,  px, py, pz, pint);
				
		}
        //----------------------------------------
		case DIALOG_EDIT_DESC:
		{
            if(!response){
            SetPVarInt(playerid, "EditingHouse", -1);
			return SendErrorMessage(playerid, "You canceled house edditing."); }
			if(strlen(inputtext) > 128)
			{
			    ShowPlayerDialog(playerid, DIALOG_EDIT_DESC, DIALOG_STYLE_INPUT,"Change Description for House: ", "Invalid size! Max Lenght allowed: 128 Characters ","Update","Cancel");
			}
			new query[128], hid=GetPVarInt(playerid, "EditingHouse"),desc[128];
			format(desc, sizeof(desc), "%s",inputtext);
			mysql_format(sqlcon, query, sizeof(query), "UPDATE `houses` SET Description = '%s' WHERE ID=%i",desc,HouseData[hid][houseSQLID]);
			mysql_tquery(sqlcon, query, "OnUpdateHouseDesc","iis",playerid, hid, desc);
		}
		case DIALOG_EDIT_OWNER:
		{
            if(!response){
            SetPVarInt(playerid, "EditingHouse", -1);
			return SendErrorMessage(playerid, "You canceled house edditing."); }
			
			if(strlen(inputtext) > 25)
			{
			    ShowPlayerDialog(playerid, DIALOG_EDIT_OWNER,DIALOG_STYLE_INPUT,"Change House Owner: ", "Invalid Name size. Max Length Allowed: 24 Characters","Update","Cancel");
			}
			new query[128], hid=GetPVarInt(playerid, "EditingHouse"), owner[MAX_PLAYER_NAME+1];
			format(owner, sizeof(owner), "%s",inputtext);
			if(!strcmp(owner,"The State",true))
			{
				mysql_format(sqlcon, query, sizeof(query), "UPDATE `houses` SET Owner='The State',Owned=0 WHERE ID=%d",HouseData[hid][houseSQLID]);
			}
			else mysql_format(sqlcon, query, sizeof(query), "UPDATE `houses` SET Owner='%s', Owned=1 WHERE ID=%d", owner, HouseData[hid][houseSQLID]);

			mysql_tquery(sqlcon,query,"OnUpdateHouseOwner","iis", playerid, hid, owner);
		}
		case DIALOG_EDIT_PRICE:
		{
            if(!response){
            SetPVarInt(playerid, "EditingHouse", -1);
			return SendErrorMessage(playerid, "You canceled house edditing."); }
			
			if(!IsNumeric(inputtext)) return SendErrorMessage(playerid, "Invalid{0000FF}Price{FFFFFF}!");
			new price = strval(inputtext);
            if(price < 1) return SendErrorMessage(playerid, "Minimum Price 0.");

			new query[128], hid = GetPVarInt(playerid, "EditingHouse");
			mysql_format(sqlcon, query, sizeof(query),"UPDATE `houses` SET Price=%d WHERE ID=%d",price,HouseData[hid][houseSQLID]);
			mysql_tquery(sqlcon, query, "OnUpdateHousePrice","iii",playerid, hid, price);
		}
		case DIALOG_HOUSE_ACCEPT:
		{
            if(!response)
			{
                new str[64], name[25], targetid = GetPVarInt(playerid, "BuyingFrom"); GetPlayerName(targetid, name, sizeof(name));
                format(str, sizeof(str), "%s has rejected your house offer.",name );
				SendClientMessage(playerid, 0x22CCCCBB, "You chose not to buy the house.");
				SendClientMessage(targetid, 0x22CCCCBB, str);
            }
            else
			{
	            new HouseID = GetPVarInt(playerid, "HouseBuying"), targetid=GetPVarInt(playerid, "BuyingFrom"), price=GetPVarInt(playerid, "HousePrice");
				new name[MAX_PLAYER_NAME+1]; GetPlayerName(playerid, name, sizeof(name));
				HouseData[HouseID][houseOwned] = true;
				format(HouseData[HouseID][houseOwner],25,"%s",name);
				new str[100],Label[128];

				format(Label, sizeof(Label), "Owner: %s\n%s",HouseData[HouseID][houseOwner],HouseData[HouseID][houseAddress]);format(Label, sizeof(Label), "Owner: %s\n%s",HouseData[HouseID][houseOwner],HouseData[HouseID][houseAddress]);
				UpdateDynamic3DTextLabelText(HouseData[HouseID][house3DText], COLOR_GREEN, Label);

				format(str, sizeof(str), "You have sold your house to %s for %s.",name,FormatNumber(price));
				SendClientMessage(targetid, 0x0000EEAA, str);
				GetPlayerName(targetid, name, sizeof(name));
				format(str, sizeof(str), "You have purchased a house from %s for %s.Congratulations!",name,FormatNumber(price));
				SendClientMessage(playerid,0x0000EEAA, str);
				GiveMoney(targetid, price);
				GiveMoney(playerid, -price);
			}
		}
	}
	return 0;
}


Dialog:House_Menu(playerid, response, listitem, inputtext[]) {
    if (response)
    {
        new id = PlayerData[playerid][pInHouse];
        if (id != -1)
        {
            switch (listitem)
            {
                case 0: 
                {
                    if (HouseData[id][houseBuilder]) Dialog_Show(playerid, House_FiredBuilder, DIALOG_STYLE_MSGBOX, "Fired Builder", WHITE_E"Your house is already assigned a Builder the contract will be expire on "GREEN_E"%s"WHITE_E", do you want to fired them?", "Yes", "Back", experieddate(HouseData[id][houseBuilderTime],3));
                    else Dialog_Show(playerid, House_AssignBuilder, DIALOG_STYLE_INPUT, "House Assign Builder", "Please input the playerid or name:", "Assign", "Back");
                }
            }
        }
    }
    return 1;
}

Dialog:House_AssignBuilder(playerid, response, listitem, inputtext[]) {
    new houseid = PlayerData[playerid][pInHouse];

    if (houseid != -1) {
        if (response) {
            new userid;
            if (isnull(inputtext))
                return Dialog_Show(playerid, House_AssignBuilder, DIALOG_STYLE_INPUT, "House Assign Builder", "Please input the playerid or name:", "Assign", "Back"), SendErrorMessage(playerid, "Invalid input!");

            if (sscanf(inputtext, "u", userid))
                return Dialog_Show(playerid, House_AssignBuilder, DIALOG_STYLE_INPUT, "House Assign Builder", "Please input the playerid or name:", "Assign", "Back"), SendErrorMessage(playerid, "Invalid input!");

            if (!IsPlayerConnected(userid))
                return SendErrorMessage(playerid, "Invalid playerid or name!");

            if (userid == playerid)
                return SendErrorMessage(playerid, "Cannot assign yourself");

            HouseData[houseid][houseBuilder] = PlayerData[userid][pID];
            HouseData[houseid][houseBuilderTime] = (gettime()+((24*3600)*7));
            SendCustomMessage(playerid, "HOUSE", "You've been assigned "YELLOW_E"%s "WHITE_E"as your house builder, it will automatically fired for 7 days.", ReturnName(userid));
            SendCustomMessage(userid, "HOUSE", ""YELLOW_E"%s "WHITE_E"has assigned you as house builder.", ReturnName(playerid));
        }
    }
    return 1;
}

Dialog:House_FiredBuilder(playerid, response, listitem, inputtext[]) 
{
    if (!response)
        return cmd_hmenu(playerid, "\0");
    
    new houseid = PlayerData[playerid][pInHouse];
    if (houseid != -1) {
        HouseData[houseid][houseBuilder] = 0;
        HouseData[houseid][houseBuilderTime] = 0;
        SendCustomMessage(playerid, "HOUSE", "You've been fired your house builder.");
    }
    return 1;
}