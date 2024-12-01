function Business_Create(playerid, type, price)
{
	new
	    Float:x,
	    Float:y,
	    Float:z;

	if (GetPlayerPos(playerid, x, y, z))
	{
		forex(i, MAX_BUSINESS)
		{
	    	if (!BizData[i][bizExists])
		    {
    	        BizData[i][bizExists] = true;
        	    BizData[i][bizOwner] = -1;
            	BizData[i][bizPrice] = price;
            	BizData[i][bizType] = type;

				format(BizData[i][bizName], 32, "None Business");
				format(BizData[i][bizOwnerName], MAX_PLAYER_NAME, "No Owner");
    	        BizData[i][bizExt][0] = x;
    	        BizData[i][bizExt][1] = y;
    	        BizData[i][bizExt][2] = z;

				if (type == 1)
				{
                	BizData[i][bizInt][0] = 363.22;
                	BizData[i][bizInt][1] = -74.86;
                	BizData[i][bizInt][2] = 1001.50;
					BizData[i][bizInterior] = 10;
					format(ProductName[i][0], 24, "French Fries");
					format(ProductName[i][1], 24, "Mac n Cheese");
					format(ProductName[i][2], 24, "Fried Chicken");
				}
				else if (type == 2)
				{
                	BizData[i][bizInt][0] = 5.73;
                	BizData[i][bizInt][1] = -31.04;
                	BizData[i][bizInt][2] = 1003.54;
					BizData[i][bizInterior] = 10;
					format(ProductName[i][0], 24, "Chitato");
					format(ProductName[i][1], 24, "Danone Mineral");
					format(ProductName[i][2], 24, "Mask");
					format(ProductName[i][3], 24, "First Aid");
				}
				else if(type == 3)
				{
                	BizData[i][bizInt][0] = 207.55;
                	BizData[i][bizInt][1] = -110.67;
                	BizData[i][bizInt][2] = 1005.13;
					BizData[i][bizInterior] = 15;
					format(ProductName[i][0], 24, "Uniqlo Clothes");
				}
				else if(type == 4)
				{
                	BizData[i][bizInt][0] = -2240.7825;
                	BizData[i][bizInt][1] = 137.1855;
                	BizData[i][bizInt][2] = 1035.4141;
					BizData[i][bizInterior] = 6;
					format(ProductName[i][0], 24, "Huawei Mate");
					format(ProductName[i][1], 24, "GPS");
					format(ProductName[i][2], 24, "Walkie Talkie");
					format(ProductName[i][3], 24, "Electric Credit");
				}
				BizData[i][bizVault] = 0;
				BizData[i][bizStock] = 100;

				Business_Refresh(i);
				mysql_tquery(sqlcon, "INSERT INTO `business` (`bizOwner`) VALUES(0)", "OnBusinessCreated", "d", i);
				return i;
			}
		}
	}
	return -1;
}

function Biz_IsOwner(playerid, id)
{
	if(!BizData[id][bizExists])
	    return 0;
	    
	if(BizData[id][bizOwner] == PlayerData[playerid][pID])
		return 1;
		
	return 0;
}

function Biz_GetCount(playerid)
{
	new count = 0;
	forex(i, MAX_BUSINESS) if(BizData[i][bizExists] && BizData[i][bizOwner] == PlayerData[playerid][pID])
	{
	    count++;
	}
	return count;
}

FUNC::OnBusinessCreated(bizid)
{
	if (bizid == -1 || !BizData[bizid][bizExists])
	    return 0;

	BizData[bizid][bizID] = cache_insert_id();
	BizData[bizid][bizWorld] = BizData[bizid][bizID]+1000;
	
	Business_Save(bizid);

	return 1;
}

FUNC::Business_Load()
{
	new rows = cache_num_rows(), str[128];
 	if(rows)
  	{
		forex(i, rows)
		{
		    BizData[i][bizExists] = true;
		    cache_get_value_name(i, "bizName", BizData[i][bizName]);
		    cache_get_value_name_int(i, "bizOwner", BizData[i][bizOwner]);
		    cache_get_value_name_int(i, "bizID", BizData[i][bizID]);
		    cache_get_value_name_float(i, "bizExtX", BizData[i][bizExt][0]);
		    cache_get_value_name_float(i, "bizExtY", BizData[i][bizExt][1]);
		    cache_get_value_name_float(i, "bizExtZ", BizData[i][bizExt][2]);
		    cache_get_value_name_float(i, "bizIntX", BizData[i][bizInt][0]);
		    cache_get_value_name_float(i, "bizIntY", BizData[i][bizInt][1]);
		    cache_get_value_name_float(i, "bizIntZ", BizData[i][bizInt][2]);
		    cache_get_value_name_float(i, "bizPointX", BizData[i][bizPoint][0]);
		    cache_get_value_name_float(i, "bizPointY", BizData[i][bizPoint][1]);
		    cache_get_value_name_float(i, "bizPointZ", BizData[i][bizPoint][2]);
			forex(j, 7)
			{
				format(str, 32, "bizProduct%d", j + 1);
				cache_get_value_name_int(i, str, BizData[i][bizProduct][j]);
				format(str, 32, "bizProdName%d", j + 1);
				cache_get_value_name(i, str, ProductName[i][j]);
			}

			cache_get_value_name_int(i, "bizVault", BizData[i][bizVault]);
			cache_get_value_name_int(i, "bizPrice", BizData[i][bizPrice]);
			cache_get_value_name_int(i, "bizType", BizData[i][bizType]);
			cache_get_value_name_int(i, "bizWorld", BizData[i][bizWorld]);
			cache_get_value_name_int(i, "bizInterior", BizData[i][bizInterior]);
			cache_get_value_name_int(i, "bizType", BizData[i][bizType]);
			cache_get_value_name_int(i, "bizStock", BizData[i][bizStock]);
			cache_get_value_name_int(i, "bizFuel", BizData[i][bizFuel]);
			cache_get_value_name(i, "bizOwnerName", BizData[i][bizOwnerName]);
			Business_Refresh(i);
			new query[300];
			mysql_format(sqlcon, query, sizeof query, "SELECT * FROM `fuel_station` WHERE `ID` = '%i'", BizData[i][bizID]);
			mysql_tquery(sqlcon, query, "Pump_Load", "i", BizData[i][bizID]);
		}
	}
	return 1;
}
function Business_Save(bizid)
{
	new
	    query[2048];

	mysql_format(sqlcon, query, sizeof(query), "UPDATE `business` SET `bizName` = '%s', `bizOwner` = '%d', `bizExtX` = '%f', `bizExtY` = '%f', `bizExtZ` = '%f', `bizIntX` = '%f', `bizIntY` = '%f', `bizIntZ` = '%f', `bizPointX` = '%f', `bizPointY` = '%f', `bizPointZ` = '%f",
		BizData[bizid][bizName],
		BizData[bizid][bizOwner],
		BizData[bizid][bizExt][0],
		BizData[bizid][bizExt][1],
		BizData[bizid][bizExt][2],
		BizData[bizid][bizInt][0],
		BizData[bizid][bizInt][1],
		BizData[bizid][bizInt][2],
		BizData[bizid][bizPoint][0],
		BizData[bizid][bizPoint][1],
		BizData[bizid][bizPoint][2]
	);
	forex(i, 7)
	{
		mysql_format(sqlcon, query, sizeof(query), "%s, `bizProduct%d` = '%d'", query, i + 1, BizData[bizid][bizProduct][i]);
	}
	forex(i, 7)
	{
		mysql_format(sqlcon, query, sizeof(query), "%s, `bizProdName%d` = '%s'", query, i + 1, ProductName[bizid][i]);
	}
	mysql_format(sqlcon, query, sizeof(query), "%s, `bizWorld` = '%d', `bizInterior` = '%d', `bizVault` = '%d', `bizType` = '%d', `bizStock` = '%d', `bizPrice` = '%d', `bizFuel` = '%d', `bizOwnerName` = '%s' WHERE `bizID` = '%d'",
		query,
		BizData[bizid][bizWorld],
		BizData[bizid][bizInterior],
		BizData[bizid][bizVault],
		BizData[bizid][bizType],
		BizData[bizid][bizStock],
		BizData[bizid][bizPrice],
		BizData[bizid][bizFuel],
		BizData[bizid][bizOwnerName],
		BizData[bizid][bizID]
	);
	return mysql_tquery(sqlcon, query);
}

GetBizType(type)
{
	new str[32];
	switch(type)
	{
	    case 1: str = "Fast Food";
	    case 2: str = "24/7";
	    case 3: str = "Clothes";
	    case 4: str = "Electronic";
	}
	return str;
}

FUNC::Business_Refresh(bizid)
{
	if (bizid != -1 && BizData[bizid][bizExists])
	{
		if (IsValidDynamic3DTextLabel(BizData[bizid][bizText]))
		    DestroyDynamic3DTextLabel(BizData[bizid][bizText]);

		if (IsValidDynamic3DTextLabel(BizData[bizid][bizTextPoint]))
		    DestroyDynamic3DTextLabel(BizData[bizid][bizTextPoint]);

		if (IsValidDynamicPickup(BizData[bizid][bizPickup]))
		    DestroyDynamicPickup(BizData[bizid][bizPickup]);

		if(IsValidDynamicCP(BizData[bizid][bizCP]))
		    DestroyDynamicCP(BizData[bizid][bizCP]);
		    
		new
		    string[256];

		if (BizData[bizid][bizOwner] == -1)
		{
			format(string, sizeof(string), "Type: {C6E2FF}%s\n{FFFFFF}Price: {C6E2FF}%s\n{FFFFFF}This business for sell", GetBizType(BizData[bizid][bizType]), FormatNumber(BizData[bizid][bizPrice]));
            BizData[bizid][bizText] = CreateDynamic3DTextLabel(string, -1, BizData[bizid][bizExt][0], BizData[bizid][bizExt][1], BizData[bizid][bizExt][2], 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1);
		}
		else
		{
  			format(string, sizeof(string), "Name: %s{FFFFFF}\nStatus: {C6E2FF}%s{FFFFFF}\nType: {C6E2FF}%s", BizData[bizid][bizName], (!BizData[bizid][bizLocked]) ? ("{00FF00}Open{FFFFFF}") : ("{FF0000}Closed{FFFFFF}"), GetBizType(BizData[bizid][bizType]));
			BizData[bizid][bizText] = CreateDynamic3DTextLabel(string, -1, BizData[bizid][bizExt][0], BizData[bizid][bizExt][1], BizData[bizid][bizExt][2], 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1);
		
			format(string, sizeof(string), "Type '/buy'");
            BizData[bizid][bizTextPoint] = CreateDynamic3DTextLabel(string, -1, BizData[bizid][bizPoint][0], BizData[bizid][bizPoint][1], BizData[bizid][bizPoint][2], 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1);
		}
		BizData[bizid][bizCP] = CreateDynamicCP(BizData[bizid][bizExt][0], BizData[bizid][bizExt][1], BizData[bizid][bizExt][2], 1.0, -1, -1, -1, 2.0);
		BizData[bizid][bizPickup] = CreateDynamicPickup(19130, 23, BizData[bizid][bizExt][0], BizData[bizid][bizExt][1], BizData[bizid][bizExt][2], -1, -1);
	}
	return 1;
}

/*
	    case 1: str = "Fast Food";
	    case 2: str = "24/7";
	    case 3: str = "Clothes";
*/

function SetProductPrice(playerid)
{
	new bid = PlayerData[playerid][pInBiz], string[712];
	if(!BizData[bid][bizExists])
	    return 0;

	switch(BizData[bid][bizType])
	{
	    case 1:
	    {
	        format(string, sizeof(string), "Product\tPrice\n%s\t%s\n%s\t%s\n%s\t%s",
				ProductName[bid][0],
				FormatNumber(BizData[bid][bizProduct][0]),
				ProductName[bid][1],
	            FormatNumber(BizData[bid][bizProduct][1]),
	            ProductName[bid][2],
	            FormatNumber(BizData[bid][bizProduct][2])
			);
		}
		case 2:
		{
		    format(string, sizeof(string), "Product\tPrice\n%s\t%s\n%s\t%s\n%s\t%s\n%s\t%s",
                ProductName[bid][0],
				FormatNumber(BizData[bid][bizProduct][0]),
				ProductName[bid][1],
	            FormatNumber(BizData[bid][bizProduct][1]),
	            ProductName[bid][2],
	            FormatNumber(BizData[bid][bizProduct][2]),
	            ProductName[bid][3],
	            FormatNumber(BizData[bid][bizProduct][3])
			);
		}
		case 3:
		{
		    format(string, sizeof(string), "Product\tPrice\nClothes\t%s",
                ProductName[bid][0],
		        FormatNumber(BizData[bid][bizProduct][0])
			);
		}
		case 4:
		{
		    format(string, sizeof(string), "Product\tPrice\n%s\t%s\n%s\t%s\n%s\t%s\n%s\t%s",
                ProductName[bid][0],
				FormatNumber(BizData[bid][bizProduct][0]),
				ProductName[bid][1],
	            FormatNumber(BizData[bid][bizProduct][1]),
	            ProductName[bid][2],
	            FormatNumber(BizData[bid][bizProduct][2]),
	            ProductName[bid][3],
	            FormatNumber(BizData[bid][bizProduct][3])
			);
		}
	}
	ShowPlayerDialog(playerid, DIALOG_BIZPRICE, DIALOG_STYLE_TABLIST_HEADERS, "Set Product Price", string, "Select", "Close");
	return 1;
}

function SetProductName(playerid)
{
	new bid = PlayerData[playerid][pInBiz], string[712];
	if(!BizData[bid][bizExists])
	    return 0;

	switch(BizData[bid][bizType])
	{
	    case 1:
	    {
	        format(string, sizeof(string), "Product\tPrice\n%s\t%s\n%s\t%s\n%s\t%s",
				ProductName[bid][0],
				FormatNumber(BizData[bid][bizProduct][0]),
				ProductName[bid][1],
	            FormatNumber(BizData[bid][bizProduct][1]),
	            ProductName[bid][2],
	            FormatNumber(BizData[bid][bizProduct][2])
			);
		}
		case 2:
		{
		    format(string, sizeof(string), "Product\tPrice\n%s\t%s\n%s\t%s\n%s\t%s\n%s\t%s",
                ProductName[bid][0],
				FormatNumber(BizData[bid][bizProduct][0]),
				ProductName[bid][1],
	            FormatNumber(BizData[bid][bizProduct][1]),
	            ProductName[bid][2],
	            FormatNumber(BizData[bid][bizProduct][2]),
	            ProductName[bid][3],
	            FormatNumber(BizData[bid][bizProduct][3])
			);
		}
		case 3:
		{
		    format(string, sizeof(string), "Product\tPrice\nClothes\t%s",
                ProductName[bid][0],
		        FormatNumber(BizData[bid][bizProduct][0])
			);
		}
		case 4:
		{
		    format(string, sizeof(string), "Product\tPrice\n%s\t%s\n%s\t%s\n%s\t%s\n%s\t%s",
                ProductName[bid][0],
				FormatNumber(BizData[bid][bizProduct][0]),
				ProductName[bid][1],
	            FormatNumber(BizData[bid][bizProduct][1]),
	            ProductName[bid][2],
	            FormatNumber(BizData[bid][bizProduct][2]),
	            ProductName[bid][3],
	            FormatNumber(BizData[bid][bizProduct][3])
			);
		}
	}
	ShowPlayerDialog(playerid, DIALOG_BIZPROD, DIALOG_STYLE_TABLIST_HEADERS, "Set Product Name", string, "Select", "Close");
	return 1;
}

function ShowBusinessMenu(playerid)
{
	new bid = PlayerData[playerid][pInBiz], string[712];
	if(!BizData[bid][bizExists])
	    return 0;
	    
	switch(BizData[bid][bizType])
	{
	    case 1:
	    {
	        format(string, sizeof(string), "Product\tPrice\n%s\t%s\n%s\t%s\n%s\t%s",
				ProductName[bid][0],
				FormatNumber(BizData[bid][bizProduct][0]),
				ProductName[bid][1],
	            FormatNumber(BizData[bid][bizProduct][1]),
	            ProductName[bid][2],
	            FormatNumber(BizData[bid][bizProduct][2])
			);
		}
		case 2:
		{
		    format(string, sizeof(string), "Product\tPrice\n%s\t%s\n%s\t%s\n%s\t%s\n%s\t%s",
                ProductName[bid][0],
				FormatNumber(BizData[bid][bizProduct][0]),
				ProductName[bid][1],
	            FormatNumber(BizData[bid][bizProduct][1]),
	            ProductName[bid][2],
	            FormatNumber(BizData[bid][bizProduct][2]),
	            ProductName[bid][3],
	            FormatNumber(BizData[bid][bizProduct][3])
			);
		}
		case 3:
		{
		    format(string, sizeof(string), "Product\tPrice\n%s\t%s",
                ProductName[bid][0],
		        FormatNumber(BizData[bid][bizProduct][0])
			);
		}
		case 4:
		{
		    format(string, sizeof(string), "Product\tPrice\n%s\t%s\n%s\t%s\n%s\t%s\n%s\t%s",
                ProductName[bid][0],
				FormatNumber(BizData[bid][bizProduct][0]),
				ProductName[bid][1],
	            FormatNumber(BizData[bid][bizProduct][1]),
	            ProductName[bid][2],
	            FormatNumber(BizData[bid][bizProduct][2]),
	            ProductName[bid][3],
	            FormatNumber(BizData[bid][bizProduct][3])
			);
		}
	}
	ShowPlayerDialog(playerid, DIALOG_BIZBUY, DIALOG_STYLE_TABLIST_HEADERS, "Business Product", string, "Select", "Close");
	return 1;
}

CMD:biz(playerid, params[])
{
	new
	    type[24],
	    string[128];

	if (sscanf(params, "s[24]S()[128]", type, string))
	{
	    SendSyntaxMessage(playerid, "/biz [name]");
	    SendClientMessage(playerid, COLOR_SERVER, "Names:{FFFFFF} buy, convertfuel, reqstock, menu, lock");
	    return 1;
	}
	if(!strcmp(type, "buy", true))
	{
	    if(Biz_GetCount(playerid) >= 1)
	        return SendErrorMessage(playerid, "You can only have 1 business!");
	        
		forex(i, MAX_BUSINESS)if(BizData[i][bizExists])
		{
      		if(IsPlayerInRangeOfPoint(playerid, 3.5, BizData[i][bizExt][0], BizData[i][bizExt][1], BizData[i][bizExt][2]))
			{
			    if(BizData[i][bizOwner] != -1)
			        return SendErrorMessage(playerid, "This business is already owned by someone!");
			        
				if(GetMoney(playerid) < BizData[i][bizPrice])
				    return SendErrorMessage(playerid, "You does not have enough money to buy this business!");
				    
				BizData[i][bizOwner] = PlayerData[playerid][pID];
                format(BizData[i][bizOwnerName], MAX_PLAYER_NAME, GetName(playerid));
                SendServerMessage(playerid, "You managed to buy this business for {00FF00}%s", FormatNumber(BizData[i][bizPrice]));
                GiveMoney(playerid, -BizData[i][bizPrice]);
                Business_Refresh(i);
                Business_Save(i);
			}
		}
	}
	else if(!strcmp(type, "menu", true))
	{
		if(PlayerData[playerid][pInBiz] != -1 && GetPlayerInterior(playerid) == BizData[PlayerData[playerid][pInBiz]][bizInterior] && GetPlayerVirtualWorld(playerid) == BizData[PlayerData[playerid][pInBiz]][bizWorld] && Biz_IsOwner(playerid, PlayerData[playerid][pInBiz]))
		{
		    ShowPlayerDialog(playerid, DIALOG_BIZMENU, DIALOG_STYLE_LIST, "Business Menu", "Set Product Name\nSet Product Price\nSet Business Name", "Select", "Close");
		}
		else
			SendErrorMessage(playerid, "Kamu tidak berada didalam bisnis milikmu!");
	}
	return 1;
}

CMD:bizamenu(playerid, params[])
{
    if(!IsPlayerADuty(playerid, DEVELOPER))
        return PermissionError(playerid);

	static option[32], arg[123];
	if(sscanf(params, "s[32]S()[123]", option, arg))
		return SendSyntaxMessage(playerid, "/bizamenu [create/delete/point/location/]");

	switch(HashStr(option))
	{
		case HS<create>:
		{
   		 	static
				type,
				price,
				id;

			if (sscanf(arg, "dd", type, price))
			{
				SendSyntaxMessage(playerid, "/bizamenu create [type] [price]");
				SendClientMessage(playerid, COLOR_SERVER, "TYPE:{FFFFFF} 1.Fast Food, 2.24/7, 3.Clothes, 4.Electronic");
				return 1;
			}
			if (type < 1 || type > 4)
				return SendErrorMessage(playerid, "Invalid type specified. Types range from 1 to 4.");

			id = Business_Create(playerid, type, price);

			if (id == -1)
				return SendErrorMessage(playerid, "The server has reached the limit for businesses.");

			SendAdminMessage(X11_TOMATO, "AdmBiz: %s has successfully created business ID: %d.", ReturnName(playerid), id);
			return 1;
		}

		case HS<location>:
		{
			static id;
			if(sscanf(arg, "i", id))
				return SendSyntaxMessage(playerid, "/bizamenu location [bizid]");
				
			if((id < 0 || id >= MAX_BUSINESS))
				return SendErrorMessage(playerid, "You have specified an invalid ID.");

			if(!BizData[id][bizExists])
				return SendErrorMessage(playerid, "You have specified an invalid ID.");

			GetPlayerPos(playerid, BizData[id][bizExt][0], BizData[id][bizExt][1], BizData[id][bizExt][2]);
			Business_Save(id);
			Business_Refresh(id);

			SendAdminMessage(X11_TOMATO, "AdmBiz: %s has changed the position of business id: %d", ReturnName(playerid), id);
		}
		case HS<point>:
		{
			static id;
			if(sscanf(arg, "i", id))
				return SendSyntaxMessage(playerid, "/bizamenu point [bizid]");
				
			if((id < 0 || id >= MAX_BUSINESS))
				return SendErrorMessage(playerid, "You have specified an invalid ID.");

			if(!BizData[id][bizExists])
				return SendErrorMessage(playerid, "You have specified an invalid ID.");

			GetPlayerPos(playerid, BizData[id][bizPoint][0], BizData[id][bizPoint][1], BizData[id][bizPoint][2]);
			Business_Save(id);
			Business_Refresh(id);

			SendAdminMessage(X11_TOMATO, "AdmBiz: %s has changed the position of the point buying business id: %d", ReturnName(playerid), id);
		}
	}
    return 1;
}