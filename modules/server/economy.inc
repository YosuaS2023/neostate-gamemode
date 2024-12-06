function Economy_GetPriceByName(economy[])
{
	foreach(new i : Economy) if(!strcmp(economy, EconomyServer[i][economyName], true))
	{
		printf("Economy_GetPriceByName(from[]) succes load variable : %s = %i", EconomyServer[i][economyName], EconomyServer[i][economyPrice]);
		return EconomyServer[i][economyPrice];
	}
	return -1;
}

Economy_GetSalesTax(amount)
{
	return amount * EconomyData[TAX_RATE] / 100;
}

function Economy_GetAfterSalesTax(amount)
{
	return amount + Economy_GetSalesTax(amount);
}

Float:Economy_GetSalesInflation(amount)
{
	return amount * EconomyData[INFLATION_RATE];
}

function Economy_GetAfterSalesInflation(amount)
{
	return amount + floatround(Economy_GetSalesInflation(amount));
}

function Economy_AddSupply(amount)
{
	EconomyData[SUPPLY] += amount;
	/*
		Example: 
		260 > 120 * 2 = 240
		260 / 2 = 130
	*/
	new bool:do_inflation_reduction = (EconomyData[SUPPLY] > EconomyData[RESERVE] * 2 && EconomyData[INFLATION_RATE] > 0.0);

	if(do_inflation_reduction)
	{
		new supply = EconomyData[SUPPLY],
			new_supply = (supply / 2)
		;
		EconomyData[SUPPLY] = EconomyData[RESERVE] = new_supply;
		EconomyData[INFLATION_RATE] -= Float:1.0;
		D:2("*** Economy: prevent inflation, new supply: %i, inflation rate: %.1f%%", new_supply, EconomyData[INFLATION_RATE]);
	}
	return 1;
}

function Economy_RequestSupply(amount)
{
	if(EconomyData[SUPPLY] > amount)
	{
		EconomyData[SUPPLY] -= amount;
		return amount;
	}

	new supply = EconomyData[SUPPLY],
		reserve = EconomyData[RESERVE]
	;
	EconomyData[SUPPLY] = (reserve * 2) + supply - amount; // 60 * 2 = 120 + 40 = 160 - 45 = 115
	EconomyData[RESERVE] = (reserve * 2);
	EconomyData[INFLATION_RATE] += Float:1.0;
	D:2("*** Economy: The city was forced to print supply to %i (supply) and inflation occurred with the %.1f%%", EconomyData[INFLATION_RATE]);
	return 1;
}

SaveEconomy()
{
	foreach(new i : Economy)
	{
		new query[300];
		format(query, sizeof query, "UPDATE `economy` SET `name` = '%s', `price` = '%i' WHERE `SQLID` = '%i'",
			EconomyServer[i][economyName],
			EconomyServer[i][economyPrice],
			EconomyServer[i][economySQLID]);


		mysql_tquery(sqlcon, query);
	}
	return 1;
}

LoadEconomy()
{
	new count, Cache:result;
	result = mysql_query(sqlcon, "SELECT * FROM `economy`");
	{
		count = cache_num_rows();
		if(count)
		{
			for(new i ; i < count; i++)
			{
				EconomyServer[i][economySQLID] = cache_get_field_int(i, "SQLID");
				EconomyServer[i][economyPrice] = cache_get_field_int(i, "price");

				cache_get_field_content(i, "name", EconomyServer[i][economyName]);
				Iter_Add(Economy, i);
			}
		}
	}
	printf("*** [Economy] Loaded %i rows", count);
	cache_delete(result);

	new path_economy[32] = "economy.cfg";
	if(dini_Exists(path_economy))
	{
		EconomyData[SUPPLY] = dini_Int(path_economy, "supply");
		EconomyData[RESERVE] = dini_Int(path_economy, "reserve");
		EconomyData[TAX_RATE] = dini_Int(path_economy, "taxrate");
		EconomyData[INFLATION_RATE] = dini_Float(path_economy, "inflation_rate");
		EconomyData[MATERIAL_PRICE] = dini_Int(path_economy, "material_price");
	}
	else
	{
		dini_Create(path_economy);
		dini_IntSet(path_economy, "supply", 100_000_000);
		dini_IntSet(path_economy, "reserve", 100_000_000);
		dini_IntSet(path_economy, "taxrate", 7);
		dini_FloatSet(path_economy, "inflation_rate", 1.0);
		dini_IntSet(path_economy, "material_price", 50);

		EconomyData[SUPPLY] = 100_000_000;
		EconomyData[RESERVE] = 100_100_100;
		EconomyData[INFLATION_RATE] = 1.0;
		EconomyData[TAX_RATE] = 7;
		EconomyData[MATERIAL_PRICE] = 50;
	}
	return 1;
}

OGMI:Economy()
{
	LoadEconomy();
	return 1;
}

OGME:Economy()
{
	SaveEconomy();
	new path_economy[32] = "economy.cfg";
	if(dini_Exists(path_economy))
	{
		dini_IntSet(path_economy, "supply", EconomyData[SUPPLY]);
		dini_IntSet(path_economy, "reserve", EconomyData[RESERVE]);
		dini_IntSet(path_economy, "taxrate", EconomyData[TAX_RATE]);
		dini_FloatSet(path_economy, "inflation_rate", EconomyData[INFLATION_RATE]);
		dini_IntSet(path_economy, "material_price", EconomyData[MATERIAL_PRICE]);
	}
	else printf("*** [Error-Dini] Failed to save \"scriptfiles/economy.cfg\" because it is not available in the folder.");
	printf("*** Successfully saving EconomySave");
	return 1;
}


CMD:economymenu(playerid, params[])
{
    if(!IsPlayerADuty(playerid, DEVELOPER)) return PermissionError(playerid);
	new str[1080], listid;

	format(str, sizeof str, "#\tName\tValue\n");
	for(new i; i< sizeof arr_economyData; i++)
	{
		PlayerEconomyList[playerid][i] = -1;
		format(str, sizeof str, "%s%i\t%s(Read Only)\t%i\n", str, i, arr_economyData[i][economyName], EconomyData[arr_economyData[i][economyVar]]);
		listid = i+1;
	}
	PlayerEconomyList[playerid][listid] = 1;
	format(str, sizeof str, "%s%i\tEdit economy\tClick for edit economy\n", str, listid);
	Dialog_Show(playerid, economymenu, DIALOG_STYLE_TABLIST_HEADERS, "Economy Menu", str, "Select", "Close");
	return 1;
}

CMD:createeconomy(playerid, params[])
{
    if(IsPlayerADuty(playerid, DEVELOPER))
    {
        new names[32], value;
        if(sscanf(params, "s[32]i", names, value)) return SendSyntaxMessage(playerid, "/createeconomy [names] [value/price]");

        new id = Iter_Free(Economy);
        format(EconomyServer[id][economyName], 32, names);
        EconomyServer[id][economyPrice] = value;
        mysql_tquery(sqlcon, "INSERT INTO `economy` (`name`) VALUES ('new_economy')", "OnQueryFinished", "ii", id, THREAD_INSERT_ECONOMY);
        Iter_Add(Economy, id);
        SendAdminMessage(X11_TOMATO, "%s create economic data %s price %s", ReturnName(playerid), names, value);
        return 1;
    }
    PermissionError(playerid);
    return 1;
}

Dialog:economymenu(playerid, response,listitem, inputtext[])
{
	if(response)
	{
		if(PlayerEconomyList[playerid][listitem] == 1)
		{
			new str[1080];
			format(str, sizeof(str), "Name\tAmount\n");
			foreach(new i : Economy)
			{
				format(str, sizeof(str), "%s%s\t%s\n", str, EconomyServer[i][economyName], FormatNumber(EconomyServer[i][economyPrice]));
			}
			Dialog_Show(playerid, edit_economy, DIALOG_STYLE_TABLIST_HEADERS, "Edit economy",str, "Select", "Cancel");
		}
	}
	return 1;
}

Dialog:edit_economy(playerid, response, listitem, inputtext[])
{
	if(response)
	{
		PlayerData[playerid][pListitem] = listitem;
		Dialog_Show(playerid, economyedit, DIALOG_STYLE_INPUT, sprintf("Economy Menu -> %s", EconomyServer[listitem][economyName]), sprintf("Input the amount to change "YELLOW"%s", EconomyServer[listitem][economyName]), "Input","Cancel");
	}
	return 1;
}
Dialog:economyedit(playerid, response, listitem, inputtext[])
{
	if(response)
	{
		new input = strval(inputtext), listid = PlayerData[playerid][pListitem];
		if(input < 1) return SendErrorMessage(playerid, "input cannot be below 1." );

		EconomyServer[listid][economyPrice] = input;
		SendAdminMessage(X11_TOMATO, "%s Has changed the economy %s to %s", GetName(playerid), EconomyServer[listid][economyName], FormatNumber(input));
		//WriteLog("./logs/economy.txt", sprintf(" %s Has changed the economy %s to %s", GetName(playerid), EconomyServer[listid][economyName], FormatNumber(input)));
		SaveEconomy();
	}
	return 1;
}