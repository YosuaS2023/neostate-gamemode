CMD:aduty(playerid, params[])
{
    if(PlayerData[playerid][pAdmin] < 1)
        return SendErrorMessage(playerid, "You don't have permission to use this command!");
        
	if(!PlayerData[playerid][pAduty])
	{
	    PlayerData[playerid][pAduty] = true;
	    SetPlayerColor(playerid, 0xFF0000FF);
	    SetPlayerName(playerid, PlayerData[playerid][pUCP]);
		SendAdminMessage(X11_TOMATO, "%s is now on duty as an admin.", PlayerData[playerid][pUCP]);
	}
	else
	{
	    PlayerData[playerid][pAduty] = false;
	    SetPlayerColor(playerid, COLOR_WHITE);
	    SetPlayerName(playerid, PlayerData[playerid][pName]);
		SendAdminMessage(X11_TOMATO, "%s is now off duty as an admin.", PlayerData[playerid][pUCP]);
	}
	return 1;
}

CMD:goto(playerid, params[])
{
	new option[32], arg[123];
	if(!IsPlayerADuty(playerid, 1)) return PermissionError(playerid);
	if(sscanf(params, "s[32]S()[123]", option, arg)) return SendSyntaxMessage(playerid, "/goto [vehicle]");
	switch(HashStr(option))
	{
		case HS<vehicle>:
		{
			new vehicleid;
			if(sscanf(arg, "i", vehicleid)) return SendSyntaxMessage(playerid, "/goto vehicle [int:vehicle id]");
			if(!IsValidVehicle(vehicleid)) return SendErrorMessage(playerid, "Invalid Vehicle ID!");
			new Float:x, Float:y, Float:z;
			GetVehiclePos(vehicleid, x, y, z);
			SetPlayerPos(playerid, x, y, z);
		}
	}
	return 1;
}
CMD:setitem(playerid, params[])
{
	new
	    userid,
		item[32],
		amount;

	if (!IsPlayerADuty(playerid, 4))
	    return PermissionError(playerid);

	if (sscanf(params, "uds[32]", userid, amount, item))
	    return SendSyntaxMessage(playerid, "/setitem [playerid/name] [amount] [item name]");

	for (new i = 0; i < sizeof(g_aInventoryItems); i ++) if (!strcmp(g_aInventoryItems[i][e_InventoryItem], item, true))
	{
        Inventory_Set(userid, g_aInventoryItems[i][e_InventoryItem], g_aInventoryItems[i][e_InventoryModel], amount);

		return SendServerMessage(playerid, "You have set %s's \"%s\" to %d.", ReturnName(userid), item, amount);
	}
	return 1;
}

CMD:veh(playerid, params[])
{

	if(!IsPlayerADuty(playerid, 3)) return 1;
	new
	    model[32],
		color1,
		color2;

	if (sscanf(params, "s[32]I(-1)I(-1)", model, color1, color2))
	    return SendSyntaxMessage(playerid, "/veh [model id/name] <color 1> <color 2>");

	if ((model[0] = GetVehicleModelByName(model)) == 0)
	    return SendErrorMessage(playerid, "Invalid model ID.");

	new
	    Float:x,
	    Float:y,
	    Float:z,
	    Float:a,
		vehicleid;

	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, a);

	vehicleid = CreateVehicle(model[0], x, y + 2, z, a, color1, color2, 0);

	if (GetPlayerInterior(playerid) != 0)
	    LinkVehicleToInterior(vehicleid, GetPlayerInterior(playerid));

	if (GetPlayerVirtualWorld(playerid) != 0)
		SetVehicleVirtualWorld(vehicleid, GetPlayerVirtualWorld(playerid));

	PutPlayerInVehicle(playerid, vehicleid, 0);
	SwitchVehicleEngine(vehicleid, true);
	VehCore[vehicleid][vehFuel] = 100;
	SendServerMessage(playerid, "You have spawned a %s.", ReturnVehicleModelName(model[0]));
	return 1;
}

CMD:amenu(playerid, params[])
{
	if(!IsPlayerADuty(playerid, 1))
		return PermissionError(playerid);

	static fmt[300];
	if(PlayerData[playerid][pAdmin] >= 1) format(fmt, sizeof fmt, "Command Administrator\n");
	if(PlayerData[playerid][pAdmin] >= 4) format(fmt, sizeof fmt, "%sDebug Statistic\n", fmt);
	Dialog_Show(playerid, "admin_menu", DIALOG_STYLE_LIST, "Admin Menu", fmt, "Select", "Exit");
	return 1;
}

Dialog:admin_menu(playerid, response, listitem, const inputtext[])
{
	if(response)
	{
		switch(HashStr(inputtext))
		{
			case HS<Command Administrator>:
			{
				static form[1080] = "";
				if(PlayerData[playerid][pAdmin] >= STAFF)
				{
					strcat(form, "[Command Staff]:\n/aduty, /near\n\n");
				}
				if(PlayerData[playerid][pAdmin] >= SUPERVISOR)
				{
					strcat(form, "[Command Supervisor]:\n/\n\n");
				}
				if(PlayerData[playerid][pAdmin] >= OPERATIONS_LEAD)
				{
					strcat(form, "[Command Operations Lead]:\n/setlevel, /veh, /gpsmenu, /atmmenu(access menu: create, delete), /addsalary, /vsmenu, /pumpmenu, /actormenu\n\n");
				}
				if(PlayerData[playerid][pAdmin] >= OWNER)
				{
					strcat(form, "[Command Owner]:\n/atmmenu(all access), /accountatm, /setitem, /houseamenu\n\n");
				}
				strcat(form, "Note: jika ada yang mensalah gunakan perintah admin, akan di keluarkan dari administrator.");
				Dialog_Show(playerid, unused, DIALOG_STYLE_MSGBOX, "Admin Command", form, "Exit", "");
			}
			case HS<Debug Statistic>:
			{
				static dialog[dialog_data], userid;
				yield 1;
				await_arr(dialog) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_INPUT, "Check Debug Statistic Player", "Enter the name of the player/id player you want to check the complete status:", "input", "exit");
				if(dialog[Response])
				{
					if(sscanf(dialog[Inputtext], "u", userid))
						return SendErrorMessage(playerid, "ID Players are not found.");

					new fmt[1080];
					format(fmt, sizeof fmt, "Display Debug Statistic %s's\n\n", GetName(userid));

					for(new i; i < sizeof(struct_character); i++)
					{
						format(fmt, sizeof(fmt), "%s%s: %i\n", fmt, struct_character[i][structDisplay], PlayerData[userid][struct_character[i][structChar]]);
					}
					Dialog_Show(playerid, "unsued", DIALOG_STYLE_MSGBOX, "Debug Statistic", fmt, "", "");
				}
			}
		}
	}
	return 1;
}

CMD:setlevel(playerid, params[])
{
	if(!IsPlayerADuty(playerid, 3))
		return PermissionError(playerid);

	static userid, level;
	if(sscanf(params, "ui", userid, level))
		return SendSyntaxMessage(playerid, "/setlevel [username/playerid] [level]");

	if(!IsPlayerConnected(userid))
		return SendErrorMessage(playerid, "Invalid Player ID");

	PlayerData[userid][pLevel] = level;
	SendAdminMessage(X11_TOMATO, "%s merubah level pemain %s(id:%i) menjadi %i level", ReturnName(playerid), ReturnName(userid), userid, level);
	SendServerMessage(playerid, "ADMIN", "%s merubah level anda menjadi %i level", ReturnName(playerid), level);
	return 1;
}