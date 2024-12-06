function OnPlayerUseItem(playerid, itemid, name[])
{
	if(!strcmp(name, "Snack"))
	{
        if (PlayerData[playerid][pEnergy] > 90)
            return SendErrorMessage(playerid, "Energy milikmu sudah penuh.");

        PlayerData[playerid][pEnergy] += 10;
		Inventory_Remove(playerid, "Snack", 1);
		ApplyAnimation(playerid, "FOOD", "EAT_Burger", 4.1, 0, 0, 0, 0, 0, 1);
        SendNearbyMessage(playerid, 30.0, COLOR_PURPLE, "* %s takes a snack and eats it.", ReturnName(playerid));
	}
	else if(!strcmp(name, "Water"))
	{
        if (PlayerData[playerid][pEnergy] > 90)
            return SendErrorMessage(playerid, "Energy milikmu sudah penuh.");

        PlayerData[playerid][pEnergy] += 10;
		Inventory_Remove(playerid, "Water", 1);
        SendNearbyMessage(playerid, 30.0, COLOR_PURPLE, "* %s takes a water mineral and drinks it.", ReturnName(playerid));
	}
	return 1;
}

FUNC::Dropped_Load()
{
	new rows = cache_num_rows();
 	if(rows)
  	{
    	forex(i, rows)
		{
		    cache_get_value_name_int(i, "ID", DroppedItems[i][droppedID]);

			cache_get_value_name(i, "itemName", DroppedItems[i][droppedItem]);
			cache_get_value_name(i, "itemPlayer", DroppedItems[i][droppedPlayer]);

			cache_get_value_name_int(i, "itemModel", DroppedItems[i][droppedModel]);
			cache_get_value_name_int(i, "itemQuantity", DroppedItems[i][droppedQuantity]);
			cache_get_value_name_float(i, "itemX", DroppedItems[i][droppedPos][0]);
			cache_get_value_name_float(i, "itemY", DroppedItems[i][droppedPos][1]);
			cache_get_value_name_float(i, "itemZ", DroppedItems[i][droppedPos][2]);
			cache_get_value_name_int(i, "itemInt", DroppedItems[i][droppedInt]);
			cache_get_value_name_int(i, "itemWorld", DroppedItems[i][droppedWorld]);

			DroppedItems[i][droppedObject] = CreateDynamicObject(DroppedItems[i][droppedModel], DroppedItems[i][droppedPos][0], DroppedItems[i][droppedPos][1], DroppedItems[i][droppedPos][2], 0.0, 0.0, 0.0, DroppedItems[i][droppedWorld], DroppedItems[i][droppedInt]);
			DroppedItems[i][droppedText3D] = CreateDynamic3DTextLabel(DroppedItems[i][droppedItem], COLOR_SERVER, DroppedItems[i][droppedPos][0], DroppedItems[i][droppedPos][1], DroppedItems[i][droppedPos][2], 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, DroppedItems[i][droppedWorld], DroppedItems[i][droppedInt]);
		}
		printf("[DROPITEM] Loaded %d Dropped items from database.", rows);
	}
	return 1;
}

function Inventory_Clear(playerid)
{
	static
	    string[64];

	forex(i, MAX_INVENTORY)
	{
	    if (InventoryData[playerid][i][invExists])
	    {
	        InventoryData[playerid][i][invExists] = 0;
	        InventoryData[playerid][i][invModel] = 0;
	        InventoryData[playerid][i][invQuantity] = 0;
		}
	}
	format(string, sizeof(string), "DELETE FROM `inventory` WHERE `ID` = '%d'", PlayerData[playerid][pID]);
	return mysql_tquery(sqlcon, string);
}

function Inventory_GetItemID(playerid, item[])
{
	forex(i, MAX_INVENTORY)
	{
	    if (!InventoryData[playerid][i][invExists])
	        continue;

		if (!strcmp(InventoryData[playerid][i][invItem], item)) return i;
	}
	return -1;
}

function Inventory_GetFreeID(playerid)
{
	if (Inventory_Items(playerid) >= 20)
		return -1;

	forex(i, MAX_INVENTORY)
	{
	    if (!InventoryData[playerid][i][invExists])
	        return i;
	}
	return -1;
}

function Inventory_Items(playerid)
{
    new count;

    forex(i, MAX_INVENTORY) if (InventoryData[playerid][i][invExists]) {
        count++;
	}
	return count;
}

function Inventory_Count(playerid, item[])
{
	new itemid = Inventory_GetItemID(playerid, item);

	if (itemid != -1)
	    return InventoryData[playerid][itemid][invQuantity];

	return 0;
}

function PlayerHasItem(playerid, item[])
{
	return (Inventory_GetItemID(playerid, item) != -1);
}

function Inventory_Set(playerid, item[], model, amount)
{
	new itemid = Inventory_GetItemID(playerid, item);

	if (itemid == -1 && amount > 0)
		Inventory_Add(playerid, item, model, amount);

	else if (amount > 0 && itemid != -1)
	    Inventory_SetQuantity(playerid, item, amount);

	else if (amount < 1 && itemid != -1)
	    Inventory_Remove(playerid, item, -1);

	return 1;
}

function Inventory_SetQuantity(playerid, item[], quantity)
{
	new
	    itemid = Inventory_GetItemID(playerid, item),
	    string[128];

	if (itemid != -1)
	{
	    format(string, sizeof(string), "UPDATE `inventory` SET `invQuantity` = %d WHERE `ID` = '%d' AND `invID` = '%d'", quantity, PlayerData[playerid][pID], InventoryData[playerid][itemid][invID]);
	    mysql_tquery(sqlcon, string);

	    InventoryData[playerid][itemid][invQuantity] = quantity;
	}
	return 1;
}

stock Inventory_Remove(playerid, item[], quantity = 1)
{
	new
		itemid = Inventory_GetItemID(playerid, item),
		string[128];

	if (itemid != -1)
	{
	    if (InventoryData[playerid][itemid][invQuantity] > 0)
	    {
	        InventoryData[playerid][itemid][invQuantity] -= quantity;
		}
		if (quantity == -1 || InventoryData[playerid][itemid][invQuantity] < 1)
		{
		    InventoryData[playerid][itemid][invExists] = false;
		    InventoryData[playerid][itemid][invModel] = 0;
		    InventoryData[playerid][itemid][invQuantity] = 0;

		    format(string, sizeof(string), "DELETE FROM `inventory` WHERE `ID` = '%d' AND `invID` = '%d'", PlayerData[playerid][pID], InventoryData[playerid][itemid][invID]);
	        mysql_tquery(sqlcon, string);
		}
		else if (quantity != -1 && InventoryData[playerid][itemid][invQuantity] > 0)
		{
			format(string, sizeof(string), "UPDATE `inventory` SET `invQuantity` = `invQuantity` - %d WHERE `ID` = '%d' AND `invID` = '%d'", quantity, PlayerData[playerid][pID], InventoryData[playerid][itemid][invID]);
            mysql_tquery(sqlcon, string);
		}
		return 1;
	}
	return 0;
}

stock Inventory_Add(playerid, item[], model, quantity = 1)
{
	new
		itemid = Inventory_GetItemID(playerid, item),
		string[128];

	if (itemid == -1)
	{
	    itemid = Inventory_GetFreeID(playerid);

	    if (itemid != -1)
	    {
	        InventoryData[playerid][itemid][invExists] = true;
	        InventoryData[playerid][itemid][invModel] = model;
	        InventoryData[playerid][itemid][invQuantity] = quantity;

	        strpack(InventoryData[playerid][itemid][invItem], item, 32 char);

			format(string, sizeof(string), "INSERT INTO `inventory` (`ID`, `invItem`, `invModel`, `invQuantity`) VALUES('%d', '%s', '%d', '%d')", PlayerData[playerid][pID], item, model, quantity);
			mysql_tquery(sqlcon, string, "OnInventoryAdd", "dd", playerid, itemid);
	        return itemid;
		}
		return -1;
	}
	else
	{
	    format(string, sizeof(string), "UPDATE `inventory` SET `invQuantity` = `invQuantity` + %d WHERE `ID` = '%d' AND `invID` = '%d'", quantity, PlayerData[playerid][pID], InventoryData[playerid][itemid][invID]);
	    mysql_tquery(sqlcon, string);

	    InventoryData[playerid][itemid][invQuantity] += quantity;
	}
	return itemid;
}

FUNC::OnInventoryAdd(playerid, itemid)
{
	InventoryData[playerid][itemid][invID] = cache_insert_id();
	return 1;
}

FUNC::ShowInventory(playerid, targetid)
{
    if (!IsPlayerConnected(playerid))
	    return 0;

	new
	    items[MAX_INVENTORY],
		amounts[MAX_INVENTORY],
		str[512],
		string[352],
		count = 0;

	format(str, sizeof(str), "Name\tAmount\n");
	format(str, sizeof(str), "%s\nMoney\t%s", str, FormatNumber(GetMoney(targetid)));
    forex(i, 20)
	{
 		if (InventoryData[targetid][i][invExists])
        {
            count++;
   			items[i] = InventoryData[targetid][i][invModel];
   			amounts[i] = InventoryData[targetid][i][invQuantity];
   			strunpack(string, InventoryData[targetid][i][invItem]);
   			format(str, sizeof(str), "%s\n%s\t%d", str, string, amounts[i]);
		}
	}
	ShowPlayerDialog(playerid, DIALOG_NONE, DIALOG_STYLE_TABLIST_HEADERS, "Inventory Data", str,  "Close", "");
	return 1;

}


FUNC::OpenInventory(playerid)
{
    if (!IsPlayerConnected(playerid))
	    return 0;

	new
	    items[MAX_INVENTORY],
		amounts[MAX_INVENTORY],
		str[512],
		string[256],
		count = 0;

	format(str, sizeof(str), "Name\tAmount\n");
    forex(i, 20)
	{
 		if (InventoryData[playerid][i][invExists])
        {
            count++;
   			items[i] = InventoryData[playerid][i][invModel];
   			amounts[i] = InventoryData[playerid][i][invQuantity];
   			strunpack(string, InventoryData[playerid][i][invItem]);
   			format(str, sizeof(str), "%s\n%s\t%d", str, string, amounts[i]);
		}
	}
	if(count)
	{
		ShowPlayerDialog(playerid, DIALOG_INVENTORY, DIALOG_STYLE_TABLIST_HEADERS, "Inventory Data", str, "Select", "Close");
	}
	else
	{
	    ShowMessage(playerid, "~r~ERROR ~w~Tidak ada Item apapun di Inventory!", 3);
	}
	return 1;

}

DropItem(item[], player[], model, quantity, Float:x, Float:y, Float:z, interior, world, weaponid = 0, ammo = 0)
{
	new
	    query[300];

	forex(i, MAX_DROPPED_ITEMS) if (!DroppedItems[i][droppedModel])
	{
	    format(DroppedItems[i][droppedItem], 32, item);
	    format(DroppedItems[i][droppedPlayer], 24, player);

		DroppedItems[i][droppedModel] = model;
		DroppedItems[i][droppedQuantity] = quantity;
		DroppedItems[i][droppedWeapon] = weaponid;
  		DroppedItems[i][droppedAmmo] = ammo;
		DroppedItems[i][droppedPos][0] = x;
		DroppedItems[i][droppedPos][1] = y;
		DroppedItems[i][droppedPos][2] = z;

		DroppedItems[i][droppedInt] = interior;
		DroppedItems[i][droppedWorld] = world;

		DroppedItems[i][droppedObject] = CreateDynamicObject(model, x, y, z, 0.0, 0.0, 0.0, world, interior);

 		DroppedItems[i][droppedText3D] = CreateDynamic3DTextLabel(item, COLOR_SERVER, x, y, z, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, world, interior);

 		format(query, sizeof(query), "INSERT INTO `dropped` (`itemName`, `itemPlayer`, `itemModel`, `itemQuantity`, `itemWeapon`, `itemAmmo`, `itemX`, `itemY`, `itemZ`, `itemInt`, `itemWorld`) VALUES('%s', '%s', '%d', '%d', '%d', '%d', '%.4f', '%.4f', '%.4f', '%d', '%d')", item, player, model, quantity, weaponid, ammo, x, y, z, interior, world);
		mysql_tquery(sqlcon, query, "OnDroppedItem", "d", i);
		return i;
	}
	return -1;
}

DropPlayerItem(playerid, itemid, quantity = 1)
{
	if (itemid == -1 || !InventoryData[playerid][itemid][invExists])
	    return 0;

    new
		Float:x,
  		Float:y,
    	Float:z,
		Float:angle,
		string[32];

	strunpack(string, InventoryData[playerid][itemid][invItem]);

	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, angle);

	DropItem(string, ReturnName(playerid), InventoryData[playerid][itemid][invModel], quantity, x, y, z - 0.9, GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid));
 	Inventory_Remove(playerid, string, quantity);

	ApplyAnimation(playerid, "GRENADE", "WEAPON_throwu", 4.1, 0, 0, 0, 0, 0, 1);
 	SendNearbyMessage(playerid, 20.0, COLOR_PURPLE, "* %s has dropped a \"%s\".", ReturnName(playerid), string);
	return 1;
}

FUNC::LoadPlayerItems(playerid)
{
	new name[128];
	new count = cache_num_rows();
	if(count > 0)
	{
	    forex(i, count)
	    {
	        InventoryData[playerid][i][invExists] = true;

	        cache_get_value_name_int(i, "invID", InventoryData[playerid][i][invID]);
	        cache_get_value_name_int(i, "invModel", InventoryData[playerid][i][invModel]);
	        cache_get_value_name_int(i, "invQuantity", InventoryData[playerid][i][invQuantity]);

	        cache_get_value_name(i, "invItem", name);

			strpack(InventoryData[playerid][i][invItem], name, 32 char);
		}
	}
	return 1;
}