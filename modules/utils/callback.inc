Dialog:unused(playerid, response, listitem,inputtext[])
{
    return 1;
}

main()
{
	print("[ Reliazed Gamemode Loaded ]");
}

Database_Connect()
{
	sqlcon = mysql_connect(DATABASE_ADDRESS,DATABASE_USERNAME,DATABASE_PASSWORD,DATABASE_NAME);

	if(mysql_errno(sqlcon) != 0)
	{
	    print("[MySQL] - Connection Failed!");
	    SetGameModeText("Reliazed | Connection Failed!");
	}
	else
	{
		print("[MySQL] - Connection Estabilished!");
		SetGameModeText("Reliazed | UCP System");
	}
	return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	if(IsPlayerADuty(playerid, 1))
	{
		yield 1;
		static dialog[dialog_data];
		await_arr(dialog) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_MSGBOX, "Teleport Map", "Do you want to teleport? \nClick \"yes\" if you want to teleport \nclick \"no\" if you don't want to teleport ", "yes", "no");
		if(dialog[Response])
		{
			SetPlayerPos(playerid, Float:fX, Float:fY, Float:fZ+10.0);
			SetPlayerInterior(playerid, 0);
			SetPlayerVirtualWorld(playerid, 0);
		}
	}
	return 1;
}
public OnGameModeInit()
{
	Database_Connect();
	CreateGlobalTextDraw();
	DisableInteriorEnterExits();
	EnableStuntBonusForAll(0);
	ManualVehicleEngineAndLights();
	StreamerConfig();
	/* Load from Database */
	mysql_tquery(sqlcon, "SELECT * FROM `business`", "Business_Load");
	mysql_tquery(sqlcon, "SELECT * FROM `dropped`", "Dropped_Load", "");
	mysql_tquery(sqlcon, "SELECT * FROM `rental`", "Rental_Load", "");
	
	mysql_tquery(sqlcon, "SELECT * FROM `car_dealer`", "LoadcDealerships");
	CallFunctionFromName("ogmi_", "");
	return 1;
}

public OnGameModeExit()
{	
	foreach(new playerid : Player)
	{
		SaveData(playerid);
	}
	CallFunctionFromName("ogme_", "");
	foreach(new pump_id : Pump) Pump_Save(pump_id);
	mysql_close(sqlcon);
	return 1;
}

public OnPlayerConnect(playerid)
{
	g_RaceCheck{playerid} ++;
	ResetVariable(playerid);
	CreatePlayerHUD(playerid);
	SetPlayerPos(playerid, 155.3337, -1776.4384, 14.8978+5.0);
	SetPlayerCameraPos(playerid, 155.3337, -1776.4384, 14.8978);
	SetPlayerCameraLookAt(playerid, 156.2734, -1776.0850, 14.2128);
	InterpolateCameraLookAt(playerid, 156.2734, -1776.0850, 14.2128, 156.2713, -1776.0797, 14.7078, 5000, CAMERA_MOVE);
	SetTimerEx("PlayerCheck", 1000, false, "ii", playerid, g_RaceCheck{playerid});
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	UnloadPlayerVehicle(playerid);
	SaveData(playerid);
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(newstate == PLAYER_STATE_DRIVER)
	{
	    new vehicleid = GetPlayerVehicleID(playerid);
	    new pvid = Vehicle_Inside(playerid);
	    new time[3];
	    if(IsSpeedoVehicle(vehicleid))
	    {
	        forex(i, 4)
	        {
	            PlayerTextDrawShow(playerid, SPEEDOTD[playerid][i]);
			}
			PlayerTextDrawShow(playerid, KMHTD[playerid]);
			PlayerTextDrawShow(playerid, VEHNAMETD[playerid]);
			PlayerTextDrawShow(playerid, HEALTHTD[playerid]);
			FUELBAR[playerid] = CreatePlayerProgressBar(playerid, 520.000000, 433.000000, 110.000000, 7.000000, 9109759, 100.000000, 0);
		}
		if(IsAVehicleDealerVeh(vehicleid))
		{
			new string[212];
			format(string, sizeof(string),"Would you like to buy this {00FFFF}%s?\n\n"WHITE_E"This vehicle costs {00FF00}$%s.", GetVehicleName(vehicleid), FormatNumber(CarDealershipInfo[GetCarDealershipId(vehicleid)][cdVehicleCost][GetCarDealershipVehicleId(vehicleid)]));
			ShowPlayerDialog(playerid,DIALOG_CDBUY,DIALOG_STYLE_MSGBOX,"Buying Vehicle:",string,"Buy","Cancel");
		}
		if(pvid != -1 && VehicleData[pvid][vRental] != -1)
		{
		    GetElapsedTime(VehicleData[pvid][vRentTime], time[0], time[1], time[2]);
		    SendClientMessageEx(playerid, COLOR_SERVER, "RENTAL: {FFFFFF}Sisa rental {00FFFF}%s {FFFFFF}milikmu adalah {FFFF00}%02d jam %02d menit %02d detik", GetVehicleName(vehicleid), time[0], time[1], time[2]);
		}
	}
	if(oldstate == PLAYER_STATE_DRIVER)
	{
        forex(i, 4)
        {
            PlayerTextDrawHide(playerid, SPEEDOTD[playerid][i]);
		}
		PlayerTextDrawHide(playerid, KMHTD[playerid]);
		PlayerTextDrawHide(playerid, VEHNAMETD[playerid]);
		PlayerTextDrawHide(playerid, HEALTHTD[playerid]);
		DestroyPlayerProgressBar(playerid, FUELBAR[playerid]);
	}
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_BIZPRICE)
	{
	    if(response)
	    {
			new str[256];
	        PlayerData[playerid][pListitem] = listitem;
	        format(str, sizeof(str), "{FFFFFF}Current Product Price: %s\n{FFFFFF}Silahkan masukan harga baru untuk product {00FFFF}%s", FormatNumber(BizData[PlayerData[playerid][pInBiz]][bizProduct][listitem]), ProductName[PlayerData[playerid][pInBiz]][listitem]);
	        ShowPlayerDialog(playerid, DIALOG_BIZPRICESET, DIALOG_STYLE_INPUT, "Set Product Price", str, "Set", "Close");
		}
		else
		    cmd_biz(playerid, "menu");
	}
	if(dialogid == DIALOG_BIZPROD)
	{
	    if(response)
	    {
			new str[256];
	        PlayerData[playerid][pListitem] = listitem;
	        format(str, sizeof(str), "{FFFFFF}Current Product Name: %s\n{FFFFFF}Silahkan masukan nama baru untuk product {00FFFF}%s", ProductName[PlayerData[playerid][pInBiz]][listitem], ProductName[PlayerData[playerid][pInBiz]][listitem]);
	        ShowPlayerDialog(playerid, DIALOG_BIZPRODSET, DIALOG_STYLE_INPUT, "Set Product Name", str, "Set", "Close");
		}
		else
		    cmd_biz(playerid, "menu");
	}
	if(dialogid == DIALOG_BIZPRODSET)
	{
	    if(response)
	    {
	        if(strlen(inputtext) < 1 || strlen(inputtext) > 24)
	            return SendErrorMessage(playerid, "Invalid Product name!");

			new id = PlayerData[playerid][pInBiz];
			new slot = PlayerData[playerid][pListitem];
			SendClientMessageEx(playerid, COLOR_SERVER, "BIZ: {FFFFFF}Kamu telah mengubah nama product dari {00FFFF}%s {FFFFFF}menjadi {00FFFF}%s", ProductName[id][slot], inputtext);
			format(ProductName[id][slot], 24, inputtext);
			cmd_biz(playerid, "menu");
			Business_Save(id);
		}
	}
	if(dialogid == DIALOG_BIZPRICESET)
	{
	    if(response)
	    {
	        if(strval(inputtext) < 1)
	            return SendErrorMessage(playerid, "Invalid Product price!");
	            
			new id = PlayerData[playerid][pInBiz];
			new slot = PlayerData[playerid][pListitem];
			SendClientMessageEx(playerid, COLOR_SERVER, "BIZ: {FFFFFF}Kamu telah mengubah harga product dari {009000}%s {FFFFFF}menjadi {009000}%s", FormatNumber(BizData[id][bizProduct][slot]), FormatNumber(strval(inputtext)));
			BizData[id][bizProduct][slot] = strval(inputtext);
			cmd_biz(playerid, "menu");
			Business_Save(id);
		}
	}
	if(dialogid == DIALOG_BIZMENU)
	{
	    if(response)
	    {
	        if(listitem == 0)
	        {
	            SetProductName(playerid);
			}
			if(listitem == 1)
			{
			    SetProductPrice(playerid);
			}
			if(listitem == 2)
			{
				new str[256];
				format(str, sizeof(str), "{FFFFFF}Current Biz Name: %s\n{FFFFFF}Silahkan masukan nama Business mu yang baru:\n\n{FFFFFF}Note: Max 24 Huruf!", BizData[PlayerData[playerid][pInBiz]][bizName]);
				ShowPlayerDialog(playerid, DIALOG_BIZNAME, DIALOG_STYLE_INPUT, "Business Name", str, "Set", "Close");
			}
		}
	}
	if(dialogid == DIALOG_RENTAL)
	{
	    if(response)
	    {
	        new rentid = PlayerData[playerid][pRenting];
	        if(GetMoney(playerid) < RentData[rentid][rentPrice][listitem])
	            return SendErrorMessage(playerid, "Kamu tidak memiliki cukup uang!");
	            
			new str[256];
			format(str, sizeof(str), "{FFFFFF}Berapa jam kamu ingin menggunakan kendaraan Rental ini ?\n{FFFFFF}Maksimal adalah {FFFF00}4 jam\n\n{FFFFFF}Harga per Jam: {009000}$%d", RentData[rentid][rentPrice][listitem]);
			ShowPlayerDialog(playerid, DIALOG_RENTTIME, DIALOG_STYLE_INPUT, "{FFFFFF}Rental Time", str, "Rental", "Close");
			PlayerData[playerid][pListitem] = listitem;
		}
	}
	if(dialogid == DIALOG_RENTTIME)
	{
	    if(response)
	    {
	        new id = PlayerData[playerid][pRenting];
	        new slot = PlayerData[playerid][pListitem];
			new time = strval(inputtext);
			if(time < 1 || time > 4)
			{
				new str[256];
				format(str, sizeof(str), "{FFFFFF}Berapa jam kamu ingin menggunakan kendaraan Rental ini ?\n{FFFFFF}Maksimal adalah {FFFF00}4 jam\n\n{FFFFFF}Harga per Jam: {009000}$%d", RentData[id][rentPrice][listitem]);
				ShowPlayerDialog(playerid, DIALOG_RENTTIME, DIALOG_STYLE_INPUT, "{FFFFFF}Rental Time", str, "Rental", "Close");
				return 1;
			}
			GiveMoney(playerid, -RentData[id][rentPrice][slot] * time);
			SendClientMessageEx(playerid, COLOR_SERVER, "RENTAL: {FFFFFF}Kamu telah menyewa {00FFFF}%s {FFFFFF}untuk %d Jam seharga {009000}$%d", GetVehicleModelName(RentData[id][rentModel][slot]), time, RentData[id][rentPrice][slot] * time);
            VehicleRental_Create(PlayerData[playerid][pID], RentData[id][rentModel][slot], RentData[id][rentSpawn][0], RentData[id][rentSpawn][1], RentData[id][rentSpawn][2], RentData[id][rentSpawn][3], time*3600, PlayerData[playerid][pRenting]);
		}
	}
	if(dialogid == DIALOG_BUYSKINS)
	{
	    if(response)
	    {
	        GiveMoney(playerid, -PlayerData[playerid][pSkinPrice]);
			SendNearbyMessage(playerid, 20.0, COLOR_PURPLE, "* %s has paid %s and purchased a %s.", ReturnName(playerid), FormatNumber(PlayerData[playerid][pSkinPrice]), ProductName[PlayerData[playerid][pInBiz]][0]);
			BizData[PlayerData[playerid][pInBiz]][bizStock]--;
			if(PlayerData[playerid][pGender] == 1)
			{
				UpdatePlayerSkin(playerid, g_aMaleSkins[listitem]);
			}
			else
			{
				UpdatePlayerSkin(playerid, g_aFemaleSkins[listitem]);
			}
		}
	}
	if(dialogid == DIALOG_DROPITEM)
	{
	    if(response)
	    {
			new
			    itemid = PlayerData[playerid][pListitem],
			    string[32],
				str[356];

			strunpack(string, InventoryData[playerid][itemid][invItem]);

			if (response)
			{
			    if (isnull(inputtext))
			        return format(str, sizeof(str), "Drop Item", "Item: %s - Quantity: %d\n\nPlease specify how much of this item you wish to drop:", string, InventoryData[playerid][itemid][invQuantity]),
					ShowPlayerDialog(playerid, DIALOG_DROPITEM, DIALOG_STYLE_INPUT, "Drop Item", str, "Drop", "Cancel");

				if (strval(inputtext) < 1 || strval(inputtext) > InventoryData[playerid][itemid][invQuantity])
				    return format(str, sizeof(str), "ERROR: Insufficient amount specified.\n\nItem: %s - Quantity: %d\n\nPlease specify how much of this item you wish to drop:", string, InventoryData[playerid][itemid][invQuantity]),
					ShowPlayerDialog(playerid, DIALOG_DROPITEM, DIALOG_STYLE_INPUT, "Drop Item", str, "Drop", "Cancel");

				DropPlayerItem(playerid, itemid, strval(inputtext));
			}
		}
	}
	if(dialogid == DIALOG_GIVEITEM)
	{
		if (response)
		{
		    static
		        userid = -1,
				itemid = -1,
				string[32];

			if (sscanf(inputtext, "u", userid))
			    return ShowPlayerDialog(playerid, DIALOG_GIVEITEM, DIALOG_STYLE_INPUT, "Give Item", "Please enter the name or the ID of the player:", "Submit", "Cancel");

			if (userid == INVALID_PLAYER_ID)
			    return ShowPlayerDialog(playerid, DIALOG_GIVEITEM, DIALOG_STYLE_INPUT, "Give Item", "ERROR: Invalid player specified.\n\nPlease enter the name or the ID of the player:", "Submit", "Cancel");

		    if (!IsPlayerNearPlayer(playerid, userid, 6.0))
				return ShowPlayerDialog(playerid, DIALOG_GIVEITEM, DIALOG_STYLE_INPUT, "Give Item", "ERROR: You are not near that player.\n\nPlease enter the name or the ID of the player:", "Submit", "Cancel");

		    if (userid == playerid)
				return ShowPlayerDialog(playerid, DIALOG_GIVEITEM, DIALOG_STYLE_INPUT, "Give Item", "ERROR: You can't give items to yourself.\n\nPlease enter the name or the ID of the player:", "Submit", "Cancel");

			itemid = PlayerData[playerid][pListitem];

			if (itemid == -1)
			    return 0;

			strunpack(string, InventoryData[playerid][itemid][invItem]);

			if (InventoryData[playerid][itemid][invQuantity] == 1)
			{
			    new id = Inventory_Add(userid, string, InventoryData[playerid][itemid][invModel]);

			    if (id == -1)
					return SendErrorMessage(playerid, "That player doesn't have anymore inventory slots.");

			    SendNearbyMessage(playerid, 30.0, COLOR_PURPLE, "* %s takes out a \"%s\" and gives it to %s.", ReturnName(playerid), string, ReturnName(userid));
			    SendServerMessage(userid, "%s has given you \"%s\" (added to inventory).", ReturnName(playerid), string);

				Inventory_Remove(playerid, string);
			    //Log_Write("logs/give_log.txt", "[%s] %s (%s) has given a %s to %s (%s).", ReturnDate(), ReturnName(playerid), PlayerData[playerid][pIP], string, ReturnName(userid, 0), PlayerData[userid][pIP]);
	  		}
			else
			{
				new str[152];
				format(str, sizeof(str), "Item: %s (Amount: %d)\n\nPlease enter the amount of this item you wish to give %s:", string, InventoryData[playerid][itemid][invQuantity], ReturnName(userid));
			    ShowPlayerDialog(playerid, DIALOG_GIVEAMOUNT, DIALOG_STYLE_INPUT, "Give Item", str, "Give", "Cancel");
			    PlayerData[playerid][pTarget] = userid;
			}
		}
	}
	if(dialogid == DIALOG_GIVEAMOUNT)
	{
		if (response && PlayerData[playerid][pTarget] != INVALID_PLAYER_ID)
		{
		    new
		        userid = PlayerData[playerid][pTarget],
		        itemid = PlayerData[playerid][pListitem],
				string[32],
				str[352];

			strunpack(string, InventoryData[playerid][itemid][invItem]);

			if (isnull(inputtext))
				return format(str, sizeof(str), "Item: %s (Amount: %d)\n\nPlease enter the amount of this item you wish to give %s:", string, InventoryData[playerid][itemid][invQuantity], ReturnName(userid)),
				ShowPlayerDialog(playerid, DIALOG_GIVEAMOUNT, DIALOG_STYLE_INPUT, "Give Item", str, "Give", "Cancel");

			if (strval(inputtext) < 1 || strval(inputtext) > InventoryData[playerid][itemid][invQuantity])
			    return format(str, sizeof(str), "ERROR: You don't have that much.\n\nItem: %s (Amount: %d)\n\nPlease enter the amount of this item you wish to give %s:", string, InventoryData[playerid][itemid][invQuantity], ReturnName(userid)),
				ShowPlayerDialog(playerid, DIALOG_GIVEAMOUNT, DIALOG_STYLE_INPUT, "Give Item", str, "Give", "Cancel");

	        new id = Inventory_Add(userid, string, InventoryData[playerid][itemid][invModel], strval(inputtext));

		    if (id == -1)
				return SendErrorMessage(playerid, "That player doesn't have anymore inventory slots.");

		    SendNearbyMessage(playerid, 30.0, COLOR_PURPLE, "* %s takes out a \"%s\" and gives it to %s.", ReturnName(playerid), string, ReturnName(userid));
		    SendServerMessage(userid, "%s has given you \"%s\" (added to inventory).", ReturnName(playerid), string);

			Inventory_Remove(playerid, string, strval(inputtext));
		  //  Log_Write("logs/give_log.txt", "[%s] %s (%s) has given %d %s to %s (%s).", ReturnDate(), ReturnName(playerid), PlayerData[playerid][pIP], strval(inputtext), string, ReturnName(userid, 0), PlayerData[userid][pIP]);
		}
	}
	if(dialogid == DIALOG_INVACTION)
	{
	    if(response)
	    {
		    new
				itemid = PlayerData[playerid][pListitem],
				string[64],
				str[256];

		    strunpack(string, InventoryData[playerid][itemid][invItem]);

		    switch (listitem)
		    {
		        case 0:
		        {
		            CallLocalFunction("OnPlayerUseItem", "dds", playerid, itemid, string);
		        }
		        case 1:
		        {
				    if(!strcmp(string, "Cellphone"))
				        return SendErrorMessage(playerid, "You can't do that on this item!");

				    if(!strcmp(string, "GPS"))
				        return SendErrorMessage(playerid, "You can't do that on this item!");
				        
					PlayerData[playerid][pListitem] = itemid;
					ShowPlayerDialog(playerid, DIALOG_GIVEITEM, DIALOG_STYLE_INPUT, "Give Item", "Please enter the name or the ID of the player:", "Submit", "Cancel");
		        }
		        case 2:
		        {
		            if (IsPlayerInAnyVehicle(playerid))
		                return SendErrorMessage(playerid, "You can't drop items right now.");

				    if(!strcmp(string, "Cellphone"))
				        return SendErrorMessage(playerid, "You can't do that on this item!");

				    if(!strcmp(string, "GPS"))
				        return SendErrorMessage(playerid, "You can't do that on this item!");

					else if (InventoryData[playerid][itemid][invQuantity] == 1)
						DropPlayerItem(playerid, itemid);

					else
						format(str, sizeof(str), "Item: %s - Quantity: %d\n\nPlease specify how much of this item you wish to drop:", string, InventoryData[playerid][itemid][invQuantity]),
						ShowPlayerDialog(playerid, DIALOG_DROPITEM, DIALOG_STYLE_INPUT, "Drop Item", str, "Drop", "Cancel");
				}
			}
		}
	}
    if(dialogid == DIALOG_INVENTORY)
    {
        if(response)
        {
		    new
		        name[48];

            strunpack(name, InventoryData[playerid][listitem][invItem]);
            PlayerData[playerid][pListitem] = listitem;

			switch (PlayerData[playerid][pStorageSelect])
			{
			    case 0:
			    {
		            format(name, sizeof(name), "%s (%d)", name, InventoryData[playerid][listitem][invQuantity]);
		            ShowPlayerDialog(playerid, DIALOG_INVACTION, DIALOG_STYLE_LIST, name, "Use Item\nGive Item\nDrop Item", "Select", "Cancel");
				}
			}
		}
	}
	if(dialogid == DIALOG_BIZBUY)
	{
	    if(response)
	    {
	        new bid = PlayerData[playerid][pInBiz], price, prodname[34];
	        if(bid != -1)
	        {
	            price = BizData[bid][bizProduct][listitem];
				prodname = ProductName[bid][listitem];
	            if(GetMoney(playerid) < price)
	                return SendErrorMessage(playerid, "You don't have enough money!");
	                
				if(BizData[bid][bizStock] < 1)
					return SendErrorMessage(playerid, "This business is out of stock.");
					
				switch(BizData[bid][bizType])
				{
				    case 1:
				    {
						if(listitem == 0)
						{
						    if(GetEnergy(playerid) >= 100)
						        return SendErrorMessage(playerid, "Your energy is already full!");

							PlayerData[playerid][pEnergy] += 20;
							SendNearbyMessage(playerid, 20.0, COLOR_PURPLE, "* %s has paid %s and purchased a %s.", ReturnName(playerid), FormatNumber(price), prodname);
							GiveMoney(playerid, -price);
							BizData[bid][bizStock]--;
						}
						if(listitem == 1)
						{
						    if(GetEnergy(playerid) >= 100)
						        return SendErrorMessage(playerid, "Your energy is already full!");

							PlayerData[playerid][pEnergy] += 40;
							SendNearbyMessage(playerid, 20.0, COLOR_PURPLE, "* %s has paid %s and purchased a %s.", ReturnName(playerid), FormatNumber(price), prodname);
							GiveMoney(playerid, -price);
							BizData[bid][bizStock]--;
						}
						if(listitem == 2)
						{
						    if(GetEnergy(playerid) >= 100)
						        return SendErrorMessage(playerid, "Your energy is already full!");

							PlayerData[playerid][pEnergy] += 15;
							SendNearbyMessage(playerid, 20.0, COLOR_PURPLE, "* %s has paid %s and purchased a %s.", ReturnName(playerid), FormatNumber(price), prodname);
							GiveMoney(playerid, -price);
							BizData[bid][bizStock]--;
						}
					}
					case 2:
					{
					    if(listitem == 0)
					    {
							Inventory_Add(playerid, "Snack", 2768, 1);
							SendNearbyMessage(playerid, 20.0, COLOR_PURPLE, "* %s has paid %s and purchased a %s.", ReturnName(playerid), FormatNumber(price), prodname);
							GiveMoney(playerid, -price);
							BizData[bid][bizStock]--;
						}
						if(listitem == 1)
						{
							Inventory_Add(playerid, "Water", 2958, 1);
							SendNearbyMessage(playerid, 20.0, COLOR_PURPLE, "* %s has paid %s and purchased a %s.", ReturnName(playerid), FormatNumber(price), prodname);
							GiveMoney(playerid, -price);
							BizData[bid][bizStock]--;
						}
						if(listitem == 2)
						{
							Inventory_Add(playerid, "Mask", 19036, 1);
							SendNearbyMessage(playerid, 20.0, COLOR_PURPLE, "* %s has paid %s and purchased a %s.", ReturnName(playerid), FormatNumber(price), prodname);
							GiveMoney(playerid, -price);
							BizData[bid][bizStock]--;
						}
						if(listitem == 3)
						{
							Inventory_Add(playerid, "Medkit", 1580, 1);
							SendNearbyMessage(playerid, 20.0, COLOR_PURPLE, "* %s has paid %s and purchased a %s.", ReturnName(playerid), FormatNumber(price), prodname);
							GiveMoney(playerid, -price);
							BizData[bid][bizStock]--;
						}
					}
					case 3:
					{
					    new gstr[1012];
					    if(PlayerData[playerid][pGender] == 1)
					    {
					        forex(i, sizeof(g_aMaleSkins))
					        {
					            format(gstr, sizeof(gstr), "%s%i\n", gstr, g_aMaleSkins[i]);
							}
							ShowPlayerDialog(playerid, DIALOG_BUYSKINS, DIALOG_STYLE_PREVIEW_MODEL, "Purchase Clothes", gstr, "Select", "Close");
						}
						else
						{
					        forex(i, sizeof(g_aFemaleSkins))
					        {
					            format(gstr, sizeof(gstr), "%s%i\n", gstr, g_aFemaleSkins[i]);
							}
							ShowPlayerDialog(playerid, DIALOG_BUYSKINS, DIALOG_STYLE_PREVIEW_MODEL, "Purchase Clothes", gstr, "Select", "Close");
						}
					}
					case 4:
					{
					    if(listitem == 0)
						{
						    if(PlayerHasItem(playerid, "Cellphone"))
						        return SendErrorMessage(playerid, "Kamu sudah memiliki Cellphone!");
						        
							PlayerData[playerid][pPhoneNumber] = PlayerData[playerid][pID]+RandomEx(13158, 98942);
							Inventory_Add(playerid, "Cellphone", 18867, 1);
							SendNearbyMessage(playerid, 20.0, COLOR_PURPLE, "* %s has paid %s and purchased a %s.", ReturnName(playerid), FormatNumber(price), prodname);
							GiveMoney(playerid, -price);
							BizData[bid][bizStock]--;
						}
					    if(listitem == 1)
						{
						    if(PlayerHasItem(playerid, "GPS"))
						        return SendErrorMessage(playerid, "Kamu sudah memiliki GPS!");

							Inventory_Add(playerid, "GPS", 18875, 1);
							SendNearbyMessage(playerid, 20.0, COLOR_PURPLE, "* %s has paid %s and purchased a %s.", ReturnName(playerid), FormatNumber(price), prodname);
							GiveMoney(playerid, -price);
							BizData[bid][bizStock]--;
						}
					    if(listitem == 2)
						{
						    if(PlayerHasItem(playerid, "Portable Radio"))
						        return SendErrorMessage(playerid, "Kamu sudah memiliki Portable Radio!");

							Inventory_Add(playerid, "Portable Radio", 19942, 1);
							SendNearbyMessage(playerid, 20.0, COLOR_PURPLE, "* %s has paid %s and purchased a %s.", ReturnName(playerid), FormatNumber(price), prodname);
							GiveMoney(playerid, -price);
							BizData[bid][bizStock]--;
						}
						if(listitem == 3)
						{
							PlayerData[playerid][pCredit] += 50;
							SendNearbyMessage(playerid, 20.0, COLOR_PURPLE, "* %s has paid %s and purchased a %s.", ReturnName(playerid), FormatNumber(price), prodname);
							GiveMoney(playerid, -price);
							BizData[bid][bizStock]--;
						}
					}
				}
			}
		}
	}
	if(dialogid == DIALOG_PIN)
	{
		extract inputtext -> new pin; else return ShowPlayerDialog(playerid, DIALOG_PIN, DIALOG_STYLE_INPUT, "PIN UCP", "Input your UCP Pin below", "Enter", "Cancel");
		new success = CheckPin(playerid, pin), str[150], query[100];
		if(!success) return KickEx(playerid);

    	mysql_format(sqlcon, query, sizeof(query), "UPDATE `playerucp` SET `input_pin` = 1 WHERE `UCP` = '%s'", GetName(playerid));
    	mysql_query(sqlcon, query);
		format(str, sizeof(str), "{FFFFFF}Welcome, {00FFFF}%s!\n\n{FFFFFF}Your UCP Account: {00FFFF}%s\n\n{FFFFFF}To secure your account, please create a password below.\n{FF00FF}Make sure to choose a strong and memorable one!", GetName(playerid), tempUCP[playerid]);
		ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register to Reliazed", str, "Register", "Exit");

	}
	if(dialogid == DIALOG_REGISTER)
	{
	    if(!response)
	        return Kick(playerid);

		new str[256];
	    format(str, sizeof(str), "{FFFFFF}UCP Account: {00FFFF}%s\n{FFFFFF}Attempts: {00FFFF}%d/5\n{FFFFFF}Create Password: {FF00FF}(Input Below)", GetName(playerid), PlayerData[playerid][pAttempt]);

        if(strlen(inputtext) < 7)
			return ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register to Reliazed", str, "Register", "Exit");

        if(strlen(inputtext) > 32)
			return ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register to Reliazed", str, "Register", "Exit");

        bcrypt_hash(playerid, "HashPlayerPassword", inputtext, BCRYPT_COST);
	}
	if(dialogid == DIALOG_LOGIN)
	{
	    if(!response)
	        return Kick(playerid);
	        
        if(strlen(inputtext) < 1)
        {
			new str[256];
            format(str, sizeof(str), "{FFFFFF}UCP Account: {00FFFF}%s\n{FFFFFF}Attempts: {00FFFF}%d/5\n{FFFFFF}Password: {FF00FF}(Input Below)", GetName(playerid), PlayerData[playerid][pAttempt]);
            ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login to Reliazed", str, "Login", "Exit");
            return 1;
		}
		new pwQuery[256], hash[BCRYPT_HASH_LENGTH];
		mysql_format(sqlcon, pwQuery, sizeof(pwQuery), "SELECT Password FROM PlayerUCP WHERE UCP = '%e' LIMIT 1", GetName(playerid));
		mysql_query(sqlcon, pwQuery);
		
        cache_get_value_name(0, "Password", hash, sizeof(hash));
        
        bcrypt_verify(playerid, "OnPlayerPasswordChecked", inputtext, hash);

	}
    if(dialogid == DIALOG_CHARLIST)
    {
		if(response)
		{
			if (PlayerChar[playerid][listitem][0] == EOS)
				return ShowPlayerDialog(playerid, DIALOG_MAKECHAR, DIALOG_STYLE_INPUT, "Create Character", "Insert your new Character Name\n\nExample: Finn_Xanderz, Javier_Cooper etc.", "Create", "Exit");

			PlayerData[playerid][pChar] = listitem;
			SetPlayerName(playerid, PlayerChar[playerid][listitem]);

			new cQuery[256];
			mysql_format(sqlcon, cQuery, sizeof(cQuery), "SELECT * FROM `characters` WHERE `Name` = '%s' LIMIT 1;", PlayerChar[playerid][PlayerData[playerid][pChar]]);
			mysql_tquery(sqlcon, cQuery, "LoadCharacterData", "d", playerid);
			
		}
	}
	if(dialogid == DIALOG_MAKECHAR)
	{
	    if(response)
	    {
		    if(strlen(inputtext) < 1 || strlen(inputtext) > 24)
				return ShowPlayerDialog(playerid, DIALOG_MAKECHAR, DIALOG_STYLE_INPUT, "Create Character", "Insert your new Character Name\n\nExample: Finn_Xanderz, Javier_Cooper etc.", "Create", "Back");

			if(!IsRoleplayName(inputtext))
				return ShowPlayerDialog(playerid, DIALOG_MAKECHAR, DIALOG_STYLE_INPUT, "Create Character", "Insert your new Character Name\n\nExample: Finn_Xanderz, Javier_Cooper etc.", "Create", "Back");

			new characterQuery[178];
			mysql_format(sqlcon, characterQuery, sizeof(characterQuery), "SELECT * FROM `characters` WHERE `Name` = '%s'", inputtext);
			mysql_tquery(sqlcon, characterQuery, "InsertPlayerName", "ds", playerid, inputtext);

		    format(PlayerData[playerid][pUCP], 22, GetName(playerid));
		}
	}
	if(dialogid == DIALOG_ORIGIN)
	{
	    if(!response)
	        return ShowPlayerDialog(playerid, DIALOG_ORIGIN, DIALOG_STYLE_INPUT, "Character Origin", "Please input your Character Origin:", "Continue", "Quit");

		if(strlen(inputtext) < 1)
		    return ShowPlayerDialog(playerid, DIALOG_ORIGIN, DIALOG_STYLE_INPUT, "Character Origin", "Please input your Character Origin:", "Continue", "Quit");

        format(PlayerData[playerid][pOrigin], 32, inputtext);
        ShowPlayerDialog(playerid, DIALOG_GENDER, DIALOG_STYLE_LIST, "Character Gender", "Male\nFemale", "Continue", "Cancel");
	}
	if(dialogid == DIALOG_GENDER)
	{
	    if(!response)
	        return ShowPlayerDialog(playerid, DIALOG_GENDER, DIALOG_STYLE_LIST, "Character Gender", "Male\nFemale", "Continue", "Cancel");

		if(listitem == 0)
		{
			PlayerData[playerid][pGender] = 1;
			PlayerData[playerid][pSkin] = 240;
			PlayerData[playerid][pHealth] = 100.0;
			SetupPlayerData(playerid);
		}
		if(listitem == 1)
		{
			PlayerData[playerid][pGender] = 2;
			PlayerData[playerid][pSkin] = 172;
			PlayerData[playerid][pHealth] = 100.0;
			SetupPlayerData(playerid);
			
		}
	}
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(newkeys & KEY_YES)
	{
	    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
	    {
			cmd_inventory(playerid, "");
		}
	}
	if((newkeys & KEY_SECONDARY_ATTACK ))
	{
		return cmd_enter(playerid, "");
	}
	return 1;
}

public OnPlayerSpawn(playerid)
{
	if(!PlayerData[playerid][pSpawned])
	{
	    PlayerData[playerid][pSpawned] = true;
	    GivePlayerMoney(playerid, PlayerData[playerid][pMoney]);
	    SetPlayerHealth(playerid, PlayerData[playerid][pHealth]);
	    SetPlayerSkin(playerid, PlayerData[playerid][pSkin]);
	    SetPlayerVirtualWorld(playerid, PlayerData[playerid][pWorld]);
		SetPlayerInterior(playerid, PlayerData[playerid][pInterior]);
		PlayerTextDrawShow(playerid, ENERGYTD[playerid][0]);
		PlayerTextDrawShow(playerid, ENERGYTD[playerid][1]);
		ENERGYBAR[playerid] = CreatePlayerProgressBar(playerid, 539.000000, 158.000000, 69.500000, 9.000000, 9109759, 100.000000, 0);
	}
	return 1;
}


public OnPlayerText(playerid, text[])
{
	if(PlayerData[playerid][pCalling] != INVALID_PLAYER_ID)
	{
		new lstr[1024];
		format(lstr, sizeof(lstr), "(Phone) %s says: %s", ReturnName(playerid), text);
		ProxDetector(10, playerid, lstr, 0xE6E6E6E6, 0xC8C8C8C8, 0xAAAAAAAA, 0x8C8C8C8C, 0x6E6E6E6E);
		SetPlayerChatBubble(playerid, text, COLOR_WHITE, 10.0, 3000);

		SendClientMessageEx(PlayerData[playerid][pCalling], COLOR_YELLOW, "(Phone) Caller says: %s", text);
		return 0;
	}
	else
	{
		new lstr[1024];
		format(lstr, sizeof(lstr), "%s says: %s", ReturnName(playerid), text);
		ProxDetector(10, playerid, lstr, 0xE6E6E6E6, 0xC8C8C8C8, 0xAAAAAAAA, 0x8C8C8C8C, 0x6E6E6E6E);
		SetPlayerChatBubble(playerid, text, COLOR_WHITE, 10.0, 3000);

		return 0;
	}
}

public OnVehicleSpawn(vehicleid)
{
	forex(i, MAX_PLAYER_VEHICLE)if(VehicleData[i][vExists])
	{
		if(vehicleid == VehicleData[i][vVehicle] && IsValidVehicle(VehicleData[i][vVehicle]))
		{
		    if(VehicleData[i][vRental] == -1)
		    {
				if(VehicleData[i][vInsurance] > 0)
	    		{
					VehicleData[i][vInsurance] --;
					VehicleData[i][vInsuTime] = gettime() + (1 * 86400);
					foreach(new pid : Player) if (VehicleData[i][vOwner] == PlayerData[pid][pID])
	        		{
	            		SendServerMessage(pid, "Kendaraan {00FFFF}%s {FFFFFF}milikmu telah hancur, kamu bisa Claim setelah 24 jam dari Insurance.", GetVehicleName(vehicleid));
					}

					if(IsValidVehicle(VehicleData[i][vVehicle]))
						DestroyVehicle(VehicleData[i][vVehicle]);

					VehicleData[i][vVehicle] = INVALID_VEHICLE_ID;
				}
				else
				{
					foreach(new pid : Player) if (VehicleData[i][vOwner] == PlayerData[pid][pID])
	        		{
	            		SendServerMessage(pid, "Kendaraan {00FFFF}%s {FFFFFF}milikmu telah hancur dan tidak akan dan tidak memiliki Insurance lagi.", GetVehicleName(vehicleid));
					}
					
					new query[128];
					mysql_format(sqlcon, query, sizeof(query), "DELETE FROM vehicle WHERE vehID = '%d'", VehicleData[i][vID]);
					mysql_query(sqlcon, query, true);

                    VehicleData[i][vExists] = false;
                    
					if(IsValidVehicle(VehicleData[i][vVehicle]))
						DestroyVehicle(VehicleData[i][vVehicle]);
				}
			}
			else
			{
				foreach(new pid : Player) if (VehicleData[i][vOwner] == PlayerData[pid][pID])
        		{
        		    GiveMoney(pid, -250);
            		SendServerMessage(pid, "Kendaraan Rental milikmu (%s) telah hancur, kamu dikenai denda sebesar {009000}$250!", GetVehicleName(vehicleid));
				}

				new query[128];
				mysql_format(sqlcon, query, sizeof(query), "DELETE FROM vehicle WHERE vehID = '%d'", VehicleData[i][vID]);
				mysql_query(sqlcon, query, true);

                VehicleData[i][vExists] = false;

				if(IsValidVehicle(VehicleData[i][vVehicle]))
					DestroyVehicle(VehicleData[i][vVehicle]);
			}
		}
	}
	return 1;
}

