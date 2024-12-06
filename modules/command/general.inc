CMD:help(playerid, params[])
{
    yield 1;
    static data[dialog_data];
    await_arr(data) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_LIST, "Help menu", "General Command", "select", "exit");
    switch(data[Listitem])
    {
        case 0:
        {

            static str[400];
            format(str, sizeof(str), \
            "/enter - masuk/keluar ruangan\n" \
            "/stats - periksa status karakter\n"\
            "/inventory - periksa isi tas\n" \
			"/myprop - periksa property yang dimiliki\n" \
			"/buy - membeli sesuatu di point anda berada\n" \
			"/admins - periksa admin apa saja yang sedang online"
            );
            Dialog_Show(playerid, "unused", DIALOG_STYLE_MSGBOX, "General Command", str, "exit", "");
        }
    }
    return 1;
}

CMD:admins(playerid, const params[])
{
	new rank[32];
	Admin_GetRank(playerid, rank);
	SendClientMessage(playerid, -1, "<-----[Admin Duty]----->");
	foreach(new admins : Player) if(PlayerData[admins][pAdmin] > 0)
	{
		SendClientMessageEx(playerid, -1, "[-] %s(ID:%i) - %s(%s)", ReturnName(admins), admins, rank, (PlayerData[playerid][pAduty] == 1) ? "On duty" : "Off duty");
	}
	SendClientMessage(playerid, -1, "");
	return 1;
}

CMD:stats(playerid, params[])
{
    static names[32], data[700];

    format(names, sizeof(names), "Stats %s's", GetName(playerid));
    format(data, sizeof(data), "Health: [%.1f] - Energy: [%i] - Money: [%s] - Online(s): [%i]\n",PlayerData[playerid][pHealth], GetEnergy(playerid), FormatNumber(GetMoney(playerid)), PlayerData[playerid][pOnline]);
	Dialog_Show(playerid, "unused", DIALOG_STYLE_MSGBOX, names, data, "exit", "");
    return 1;
}

CMD:myprop(playerid, params[])
{
	new data[1080], found;
	format(data, sizeof(data), "#\tLocation\tProperty\n");
	foreach(new houseid : House) if(!strcmp(GetCharacterName(playerid), HouseData[houseid][houseOwner], true))
	{
		format(data, sizeof(data), "%s%i\t%s\tHouse\n", data, found, GetLocation(PositionXYZ(HouseData[houseid][houseExterior])));
		found++;
	}

	for(new bizid; bizid < MAX_BUSINESS; bizid++) if(Biz_IsOwner(playerid, bizid))
	{
		format(data, sizeof(data), "%s%i\t%s\tBusiness(%s)\n", data, found, GetLocation(PositionXYZ(BizData[bizid][bizExt])), GetBizType(BizData[bizid][bizType]));
		found++;
	}

	foreach(new dealerid : Dealership) if(!strcmp(GetCharacterName(playerid), CarDealershipInfo[dealerid][cdOwner], true))
	{
		format(data, sizeof(data), "%s%i\t%s\tDealership\n", data, found, GetLocation(CarDealershipInfo[dealerid][cdEntranceX], CarDealershipInfo[dealerid][cdEntranceY], CarDealershipInfo[dealerid][cdEntranceZ]));
		found++;
	}

	if(found) Dialog_Show(playerid, "unused", DIALOG_STYLE_TABLIST_HEADERS, "My property", data, "select", "");
	else SendErrorMessage(playerid, "anda tidak memiliki property.");
	return 1;
}

CMD:inventory(playerid, params[])
{
	PlayerData[playerid][pStorageSelect] = 0;
	OpenInventory(playerid);
	return 1;
}

CMD:enter(playerid, params[])
{
	if(!IsPlayerInAnyVehicle(playerid))
	{
		foreach(new HouseID : House) if(IsPlayerInRangeOfPoint(playerid, 2.5, PositionXYZ(HouseData[HouseID][houseExterior])))
		{
			new name[MAX_PLAYER_NAME+1];
			GetPlayerName(playerid, name, sizeof(name));

			if(HouseData[HouseID][houseLocked] && HouseData[HouseID][houseOwned] &&!strcmp(name, HouseData[HouseID][houseOwner])) {
				SendErrorMessage(playerid, "This is your house but its locked. Use /hmenu to unlock it.");
				return 1;
			}
			if(HouseData[HouseID][houseLocked]) {
				SendCustomMessage(playerid, "HOUSE","The house is locked!");
				return 1;
			}

			new Float:hx = HouseData[HouseID][houseInterior][0], Float:hy = HouseData[HouseID][houseInterior][1], Float:hz = HouseData[HouseID][houseInterior][2];
			new hint=HouseData[HouseID][houseInt][1], hvw=HouseData[HouseID][houseSQLID];

			SetPlayerPositionEx(playerid, hx, hy, hz, 180);
			SetPlayerInterior(playerid, hint);
			SetPlayerVirtualWorld(playerid, hvw);
			PlayerData[playerid][pInHouse] = HouseID;
		}
		new houseid = PlayerData[playerid][pInHouse];
		if(houseid != -1 && IsPlayerInRangeOfPoint(playerid, 2.8, PositionXYZ(HouseData[houseid][houseExterior])))
		{
			SetPlayerPositionEx(playerid, PositionXYZ(HouseData[houseid][houseInterior]), 180);

			PlayerData[playerid][pInHouse] = -1;
			SetPlayerInterior(playerid, HouseData[houseid][houseInt][0]);
			SetPlayerVirtualWorld(playerid, 0);
			SetCameraBehindPlayer(playerid);
		}

		forex(bid, MAX_BUSINESS) if(BizData[bid][bizExists])
		{
			if(IsPlayerInRangeOfPoint(playerid, 2.8, BizData[bid][bizExt][0], BizData[bid][bizExt][1], BizData[bid][bizExt][2]))
			{
				if(BizData[bid][bizLocked])
					return SendErrorMessage(playerid, "This business is Locked by the Owner!");

				PlayerData[playerid][pInBiz] = bid;
				SetPlayerPosEx(playerid, BizData[bid][bizInt][0], BizData[bid][bizInt][1], BizData[bid][bizInt][2]);

				SetPlayerInterior(playerid, BizData[bid][bizInterior]);
				SetPlayerVirtualWorld(playerid, BizData[bid][bizWorld]);
				SetCameraBehindPlayer(playerid);
				SetPlayerWeather(playerid, 0);
			}
	    }
		new inbiz = PlayerData[playerid][pInBiz];
		if(PlayerData[playerid][pInBiz] != -1 && IsPlayerInRangeOfPoint(playerid, 2.8, BizData[inbiz][bizInt][0], BizData[inbiz][bizInt][1], BizData[inbiz][bizInt][2]))
		{
			SetPlayerPos(playerid, BizData[inbiz][bizExt][0], BizData[inbiz][bizExt][1], BizData[inbiz][bizExt][2]);

			PlayerData[playerid][pInBiz] = -1;
			SetPlayerInterior(playerid, 0);
			SetPlayerVirtualWorld(playerid, 0);
			SetCameraBehindPlayer(playerid);
		}
	}
	return 1;
}

CMD:buy(playerid, params[])
{
	new biz_id = PlayerData[playerid][pInBiz];
	if(PlayerData[playerid][pInBiz] != -1  && IsPlayerInRangeOfPoint(playerid, 2.5, PositionXYZ(BizData[biz_id][bizPoint])) && GetPlayerInterior(playerid) == BizData[PlayerData[playerid][pInBiz]][bizInterior] && GetPlayerVirtualWorld(playerid) == BizData[PlayerData[playerid][pInBiz]][bizWorld])
	{
	    ShowBusinessMenu(playerid);
	}

   	new HouseID;
    if((HouseID = GetNearbyHouse(playerid)) != INVALID_ITER_ID)
	{
		if(HouseData[HouseID][houseOwned]) return SendErrorMessage(playerid, "You need to be at a house which is on sale.");
		if(!PlayerData[playerid][pBizLicense]) return SendCustomMessage(playerid, "HOUSE", ""YELLOW"Lisensi bisnis"WHITE" anda telah melewati "RED"batas waktu");
		if(GetMoney(playerid) < HouseData[HouseID][housePrice])
			return SendErrorMessage(playerid, "You don't have enough money to buy a house property.");

		new query[128], name[MAX_PLAYER_NAME+1], housecost = HouseData[HouseID][housePrice];
		GiveMoney(playerid, -housecost, ECONOMY_ADD_SUPPLY);
		GetPlayerName(playerid, name, sizeof(name));

		mysql_format(sqlcon, query, sizeof(query), "UPDATE `houses` SET Owner='%s', Owned=1 WHERE ID=%d",name,HouseData[HouseID][houseSQLID]);
		mysql_tquery(sqlcon,query,"OnUpdateHouseOwner","iis",playerid, HouseID, name);
	}
	return 1;
}