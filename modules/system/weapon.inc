#include <UtilsScripts\hook>
forward OnLoadPlayerWeapons(playerid);
public OnLoadPlayerWeapons(playerid) 
{
	new weaponid;
	
	for(new i = 0; i < cache_num_rows(); i++)
	{
		weaponid = cache_get_field_int(i, "weaponid");
		new slot = g_aWeaponSlots[weaponid];

		if(!(0 < weaponid < 46))
			continue;

	    PlayerGuns[playerid][slot][weapon_id] 			= weaponid;
		PlayerGuns[playerid][slot][weapon_slot] 		= slot;
		PlayerGuns[playerid][slot][weapon_ammo] 		= cache_get_field_int(i, "ammo");
		PlayerGuns[playerid][slot][weapon_durability] 	= cache_get_field_int(i, "durability");
		cache_get_field_content(i, "serial", PlayerGuns[playerid][slot][weapon_serial], 64);
	}
	RefreshWeapon(playerid);
	SetPlayerArmedWeapon(playerid, 0);
	return 1; 
}

hook OnPlayerSpawn(playerid)
{
    mysql_tquery(sqlcon, sprintf("SELECT * FROM `weapon_players` WHERE `userid` = '%d';", GetPlayerSQLID(playerid)), "OnLoadPlayerWeapons", "d", playerid);
    return ALS_CONTINUE;
}

hook OnPlayerDisconnect(playerid, reason) 
{
	if(PlayerData[playerid][pSpawned])
	{
		SavePlayerWeapon(playerid);

		for (new i = 0; i < MAX_WEAPON_SLOT; i ++) if(PlayerGuns[playerid][i][weapon_id]) {
	        PlayerGuns[playerid][i][weapon_id] = 0;
	        PlayerGuns[playerid][i][weapon_ammo] = 0;
	        PlayerGuns[playerid][i][weapon_slot] = 0;
	        PlayerGuns[playerid][i][weapon_durability] = 0;
	    }
	    ResetPlayerWeapons(playerid);
	}
    return ALS_CONTINUE;
}
 
hook OnPlayerShootDynObj(playerid, weaponid, objectid, Float:x, Float:y, Float:z)
{
	if(!IsPlayerDuty(playerid) && GetWeapon(playerid) == weaponid)
	{
		new slot = g_aWeaponSlots[weaponid];

		if(--PlayerGuns[playerid][slot][weapon_ammo] <= 0) {
			SetPlayerArmedWeapon(playerid, 0);
			PlayerGuns[playerid][slot][weapon_ammo] = 0;
			SendCustomMessage(playerid, "WEAPON", "Kamu kehabisan amunisi, isi kembali dengan perintah "YELLOW"/createammo.");
		}

		if(--PlayerGuns[playerid][slot][weapon_durability] <= 0) {
			SendCustomMessage(playerid, "WEAPON", "Senjata "RED"%s "WHITE"rusak, otomatis senjata kamu tidak dapat di gunakan kembali.", ReturnWeaponName(weaponid));

			if(PlayerGuns[playerid][slot][weapon_ammo])
			{
				if(Inventory_Add(playerid, "Materials", 11746, floatround((PlayerGuns[playerid][slot][weapon_ammo]/2))) != -1) {
					SendCustomMessage(playerid, "WEAPON", "Amunisi masih tersisa, otomatis digantikan dengan "YELLOW"%d material(s).", floatround((PlayerGuns[playerid][slot][weapon_ammo]/2)));
				}
			}
			ResetWeaponID(playerid, weaponid);
		}
	}
    return ALS_CONTINUE;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if(!IsPlayerDuty(playerid) && GetWeapon(playerid) == weaponid)
    {
        new slot = g_aWeaponSlots[weaponid];

        if(--PlayerGuns[playerid][slot][weapon_ammo] <= 0) {
            SetPlayerArmedWeapon(playerid, 0);
            PlayerGuns[playerid][slot][weapon_ammo] = 0;
            SendCustomMessage(playerid, "WEAPON", "Kamu kehabisan amunisi, isi kembali dengan perintah "YELLOW"/createammo.");
        }

        if(--PlayerGuns[playerid][slot][weapon_durability] <= 0) {
            SendCustomMessage(playerid, "WEAPON", "Senjata "RED"%s "WHITE"rusak, otomatis senjata kamu tidak dapat di gunakan kembali.", ReturnWeaponName(weaponid));

            if(PlayerGuns[playerid][slot][weapon_ammo])
            {
                if(Inventory_Add(playerid, "Materials", 11746, floatround((PlayerGuns[playerid][slot][weapon_ammo]/2))) != -1) {
                    SendCustomMessage(playerid, "WEAPON", "Amunisi masih tersisa, otomatis digantikan dengan "YELLOW"%d material(s).", floatround((PlayerGuns[playerid][slot][weapon_ammo]/2)));
                }
            }
            ResetWeaponID(playerid, weaponid);
        }
    }

	return 1;
}
/*
ptask Player_AmmoUpdate[1000](playerid)
{
    if((!PlayerData[playerid][pLogged]) || !PlayerData[playerid][pCreated] || PlayerData[playerid][pKicked])
        return 0;
		
	if(SQL_IsCharacterLogged(playerid) && !IsPlayerInEvent(playerid))
	{
		static
			weaponid;

		if((weaponid = GetWeapon(playerid)) != 0) 
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][textdraw_ammo], sprintf("%s", (PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_ammo]) ? (sprintf("%d", PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_ammo])) : ("No Ammo")));
		else 
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][textdraw_ammo], "_");
	}
	return 1;
}*/

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (PRESSED(KEY_FIRE) || PRESSED(KEY_HANDBRAKE))
	{
		new weaponid;

		if((weaponid = GetWeapon(playerid)) != 0 && !PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_ammo]) 
		{
			TogglePlayerControllable(playerid, 0);
			SetPlayerArmedWeapon(playerid, 0);
			TogglePlayerControllable(playerid, 1);
			SetCameraBehindPlayer(playerid);

			ShowPlayerFooter(playerid, "~y~~h~WARNING: ~w~Tidak ada amunisi di senjata ini.");
		}
	}
	return 1;
}


RemoveWeaponInSlot(playerid, slotid) 
{
    new
        arrWeapons[2][13];
        
    for (new i = 0; i < 13; i ++) {
        GetPlayerWeaponData(playerid, i, arrWeapons[0][i], arrWeapons[1][i]);
    }
    ResetPlayerWeapons(playerid);
    
    for (new i = 0; i < 13; i ++) if (i != slotid) {
        GivePlayerWeapon(playerid, arrWeapons[0][i], arrWeapons[1][i]);
    }
    return 1;
}

PlayerHasWeaponInSlot(playerid, weaponid) 
{
	if(weaponid == 0){
		return 0;
	}
	
    if(PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_slot] == g_aWeaponSlots[weaponid]) {
        return 1;
    }
    return 0;
}

GetPlayerAmmoEx(playerid, weaponid)
{
	new slot = g_aWeaponSlots[weaponid];
	return PlayerGuns[playerid][slot][weapon_ammo];
}

GivePlayerWeaponEx(playerid, weaponid, ammo = DEFAULT_AMMO, durability = WEAPON_DURABILITY, serial[] = "0") 
{
	if(!(0 < weaponid < 46))
		return 0;

	if(PlayerHasWeapon(playerid, weaponid))
		return 1;

	new query[255],
		slot = g_aWeaponSlots[weaponid];

	if(slot == 1 || slot == 10)
		ammo = durability = 1;

	PlayerGuns[playerid][slot][weapon_id] 			= weaponid;
	PlayerGuns[playerid][slot][weapon_ammo] 		= ammo;
	PlayerGuns[playerid][slot][weapon_slot] 		= slot;
	PlayerGuns[playerid][slot][weapon_durability] 	= durability;
	format(PlayerGuns[playerid][slot][weapon_serial], 64, "%s", serial);

	GivePlayerWeapon(playerid, weaponid, 999999);

	mysql_format(sqlcon, query, sizeof(query), "INSERT INTO weapon_players VALUES ('%d', '%d', '%d', '%d', '%d', '%d', '%s') ON DUPLICATE KEY UPDATE ammo = %d, durability = %d, created = %d", GetPlayerSQLID(playerid), slot, weaponid, ammo, durability, gettime(), serial, ammo, durability, gettime());
	mysql_tquery(sqlcon, query);
	return 1;
}

SavePlayerWeapon(playerid) 
{
	for(new i = 1; i < MAX_WEAPON_SLOT; i++)
	{
		if(!PlayerGuns[playerid][i][weapon_id]) 
			continue;
		
		new query[255];
		mysql_format(sqlcon, query, sizeof(query), "INSERT INTO weapon_players VALUES ('%d', '%d', '%d', '%d', '%d', '%d', '%s') ON DUPLICATE KEY UPDATE ammo = %d, durability = %d", GetPlayerSQLID(playerid), i, PlayerGuns[playerid][i][weapon_id], PlayerGuns[playerid][i][weapon_ammo], PlayerGuns[playerid][i][weapon_durability], gettime(), PlayerGuns[playerid][i][weapon_serial], PlayerGuns[playerid][i][weapon_ammo], PlayerGuns[playerid][i][weapon_durability]);
		mysql_tquery(sqlcon, query);
	}
	return 1;
}

ResetWeaponID(playerid, weaponid) 
{
	new slot = g_aWeaponSlots[weaponid];

    PlayerGuns[playerid][slot][weapon_id] = PlayerGuns[playerid][slot][weapon_ammo] = 0;
	PlayerGuns[playerid][slot][weapon_slot] = PlayerGuns[playerid][slot][weapon_durability] = 0;
	mysql_tquery(sqlcon, sprintf("DELETE FROM `weapon_players` WHERE `slot` = '%d' AND `userid` = '%d';", slot, GetPlayerSQLID(playerid)));

	RemoveWeaponInSlot(playerid, slot);
	return 1;
}

ResetWeapons(playerid)
{
	ResetPlayerWeapons(playerid);

    for (new i = 0; i < MAX_WEAPON_SLOT; i ++) if(PlayerGuns[playerid][i][weapon_id]) {
        PlayerGuns[playerid][i][weapon_id] = PlayerGuns[playerid][i][weapon_ammo] = 0;
		PlayerGuns[playerid][i][weapon_slot] = PlayerGuns[playerid][i][weapon_durability] = 0;

    }
	mysql_tquery(sqlcon, sprintf("DELETE FROM `weapon_players` WHERE `userid` = '%d';", GetPlayerSQLID(playerid)));
	return 1;
}

GetWeapon(playerid) 
{
    new weaponid = GetPlayerWeapon(playerid);

    if(1 < weaponid < 46 && PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_id] == weaponid)
        return weaponid;

    return 0;
}

ReturnWeaponCount(playerid) 
{
	new count;

	for (new i = 0; i < MAX_WEAPON_SLOT; i ++) if(PlayerGuns[playerid][i][weapon_id]) {
		count++;
    }
    return count;
}

PlayerHasWeapon(playerid, weaponid) 
{
    for (new i = 0; i < MAX_WEAPON_SLOT; i ++) if(PlayerGuns[playerid][i][weapon_id] == weaponid) {
        return 1;
    }
    return 0;
}
ReturnWeaponSerial(playerid, weaponid)
{
	new slot = g_aWeaponSlots[weaponid];

	if(PlayerGuns[playerid][slot][weapon_id] != 0)
		return strval(PlayerGuns[playerid][slot][weapon_serial]);

	return 0;
}

ReturnWeaponAmmo(playerid, weaponid)
{
	new slot = g_aWeaponSlots[weaponid];

	if(PlayerGuns[playerid][slot][weapon_id] != 0)
		return PlayerGuns[playerid][slot][weapon_ammo];

	return 0;
}

ReturnWeaponDurability(playerid, weaponid)
{
	new slot = g_aWeaponSlots[weaponid];
	
	if(PlayerGuns[playerid][slot][weapon_id] != 0)
		return PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_durability];

	return 0;
}

ReturnWeaponMaterial(weaponid)
{
	for(new i = 0; i < sizeof(g_aWeaponItems); i++) if(weaponid == g_aWeaponItems[i][wep_model]) {
		return g_aWeaponItems[i][wep_material];
	}
	return 0;
}

RefreshWeapon(playerid)
{
	ResetPlayerWeapons(playerid);

	for(new i = 1; i != MAX_WEAPON_SLOT; i++) if(PlayerGuns[playerid][i][weapon_id]) {
        GivePlayerWeapon(playerid, PlayerGuns[playerid][i][weapon_id], 999999);
    }
}

stock RefreshWeaponSlot(playerid, weaponid)
{
	if (weaponid < 0 || weaponid > 46)
		return 0;

	new slot = g_aWeaponSlots[weaponid];

	RemoveWeaponInSlot(playerid, slot);

	if(!PlayerGuns[playerid][slot][weapon_id])
		return 0;

    GivePlayerWeapon(playerid, PlayerGuns[playerid][slot][weapon_id], 999999);
    return 1;
}

WeaponGivePlayer(playerid)
{
	new
		weapon_list[128],
		count = 0;

	strcat(weapon_list, "WeaponID\tWeapon\tAmmo\tDurability\n");
	for(new i = 1; i != MAX_WEAPON_SLOT; i++) if(PlayerGuns[playerid][i][weapon_id]) {
		strcat(weapon_list, sprintf("%d\t%s\t%s\t%s\n", PlayerGuns[playerid][i][weapon_id],ReturnWeaponName(PlayerGuns[playerid][i][weapon_id]), (i == 1) ? (" ") : sprintf("%d", PlayerGuns[playerid][i][weapon_ammo]), (i == 1) ? (" ") :  (sprintf("%d", PlayerGuns[playerid][i][weapon_durability]))));
		count++;
	}

	if(count) Dialog_Show(playerid, GiveOnly, DIALOG_STYLE_TABLIST_HEADERS, "Weapons", weapon_list, "Give", "Close");
	else {
		strcat(weapon_list, "There is no weapon\t \t \n");
		Dialog_Show(playerid, GiveOnly, DIALOG_STYLE_TABLIST_HEADERS, "Weapons", weapon_list, "Give", "Close");
	}
	return 1;
}
ShowPlayerWeapon(playerid, userid)
{
	new
		weapon_list[1024],
		count = 0;

	strcat(weapon_list, "Weapon\tAmmo\tDurability\tSerial\n");
	for(new i = 1; i != MAX_WEAPON_SLOT; i++) if(PlayerGuns[playerid][i][weapon_id]) 
	{

		new 
			serial_number[15],
			serial_id[10],
			length = strlen(PlayerGuns[playerid][i][weapon_serial])
		;

		strmid(serial_number, PlayerGuns[playerid][i][weapon_serial], 0, 4);
		strmid(serial_id, PlayerGuns[playerid][i][weapon_serial], 4, length);
		
		if(!strcmp(PlayerGuns[playerid][i][weapon_serial], "0", true)) 
		{
			strcat(weapon_list, sprintf("%s\t%s\t%s\tNone\n", ReturnWeaponName(PlayerGuns[playerid][i][weapon_id]), (i == 1) ? (" ") : sprintf("%d", PlayerGuns[playerid][i][weapon_ammo]), (i == 1) ? (" ") :  (sprintf("%d", PlayerGuns[playerid][i][weapon_durability]))));
		}
		else
		{
			strcat(weapon_list, sprintf("%s\t%s\t%s\t%s-%s\n", ReturnWeaponName(PlayerGuns[playerid][i][weapon_id]), (i == 1) ? (" ") : sprintf("%d", PlayerGuns[playerid][i][weapon_ammo]), (i == 1) ? (" ") :  (sprintf("%d", PlayerGuns[playerid][i][weapon_durability])), serial_number, serial_id));
		}
		count++;
	}

	if(count) Dialog_Show(userid, ShowOnly, DIALOG_STYLE_TABLIST_HEADERS, "Weapons", weapon_list, "Close", "");
	else {
		strcat(weapon_list, "There is no weapon\t \t \n");
		Dialog_Show(userid, ShowOnly, DIALOG_STYLE_TABLIST_HEADERS, "Weapons", weapon_list, "Close", "");
	}
	return 1;
}
PlayerRepairWeapon(playerid)
{
	new
		weapon_list[128],
		count = 0;
        

	strcat(weapon_list, "WeaponID\tWeapon\tDurability\tRepair\n");
	for(new i = 1; i != MAX_WEAPON_SLOT; i++) if(PlayerGuns[playerid][i][weapon_id]) 
	{
		strcat(weapon_list, sprintf("%d\t%s\t%s\t(%s Materials)\n", PlayerGuns[playerid][i][weapon_id],ReturnWeaponName(PlayerGuns[playerid][i][weapon_id]), (i == 1) ? (" ") :  (sprintf("%d", PlayerGuns[playerid][i][weapon_durability])), (i == 1) ? (" ") :  (sprintf("%d", WEAPON_DURABILITY-PlayerGuns[playerid][i][weapon_durability]))));
		count++;
	}

	if(count) Dialog_Show(playerid, RepairOnly, DIALOG_STYLE_TABLIST_HEADERS, "Weapons", weapon_list, "Repair", "Close");
	else {
		strcat(weapon_list, "There is no weapon to repair\t \t \n");
		Dialog_Show(playerid, RepairOnly, DIALOG_STYLE_TABLIST_HEADERS, "Weapons", weapon_list, "Repair", "Close");
	}
	return 1;
}

stock IsValidWeaponSlot(weaponid)
{
	if (g_aWeaponSlots[weaponid] == 7 || g_aWeaponSlots[weaponid] == 8)
		return 0;

	if (g_aWeaponSlots[weaponid] == 11 && weaponid != 46)
		return 0;

	return 1;
}

CMD:weapons(playerid, params[]) 
{
	ShowPlayerWeapon(playerid, playerid);
	return 1;
}

CMD:weapon(playerid, params[]) 
{
	if(PlayerData[playerid][pInjured])
        return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Tidak bisa menggunakan perintah ini ketika injured.");

    if(PlayerData[playerid][pHospitalTime])
        return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Tidak bisa menggunakan perintah ini ketika dalam masa pemulihan.");

	if(IsPlayerDuty(playerid))
		return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Tidak bisa menggunakan perintah ini ketika on duty.");

	new category[32], string[32];

	if(sscanf(params, "s[32]S()[32]", category, string))
		return SendSyntaxMessage(playerid, "/weapon [give/view/acceptview/scrap/destroy]");

	if(!strcmp(category, "give"))
	{
		new userid;

		if(sscanf(string, "u", userid))
			return SendSyntaxMessage(playerid, "/weapon give [userid]");

		if(userid == INVALID_PLAYER_ID || !IsPlayerNearPlayer(playerid, userid, 5.0))
			return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Player tidak login atau tidak berada didekatmu.");

		if(PlayerData[userid][pLevel] < 2)
			return SendErrorMessage(playerid, "They must level 2 to use this.");

		if(IsPlayerInAnyVehicle(userid))
			return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Player tersebut harus turun terlebih dahulu dari kendaraan!.");
		SelectedPlayer[playerid] = userid;
		WeaponGivePlayer(playerid);
	}
	else if(!strcmp(category, "scrap"))
	{
		new weaponid, confirm[10] ;

		if(PlayerData[playerid][pInjured])
			return SendErrorMessage(playerid, "Kondisimu terluka, kamu tidak bisa scrap senjata ketika injured!");

		if(!(weaponid = GetWeapon(playerid)))
			return ShowPlayerFooter(playerid, "Kamu tidak memegang ~r~senjata ~w~apapun.");

		if(g_aWeaponSlots[weaponid] != 2 && g_aWeaponSlots[weaponid] != 3 && g_aWeaponSlots[weaponid] !=  4 && g_aWeaponSlots[weaponid] != 5 && g_aWeaponSlots[weaponid] != 6)
			return ShowPlayerFooter(playerid, "Senjata ini tidak dapat di ~r~scrap!.");

		if(sscanf(string, "s[10]", confirm)) {
			SendCustomMessage(playerid, "USAGE","/weapon scrap ['confirm']");
			SendCustomMessage(playerid, "INFO","Perintah ini untuk menggantikan senjata dan peluru menjadi material!");
			return 1;
		}

		if(!strcmp(confirm, "confirm"))
		{
			if(Inventory_Add(playerid, "Materials", 11746, floatround((ReturnWeaponMaterial(weaponid)/4))) == -1)
				return 1;

			SendCustomMessage(playerid, "WEAPON", "Berhasil melakukan scrap senjata "RED"%s "WHITE"menjadi "YELLOW"%d material(s)", ReturnWeaponName(weaponid), floatround((ReturnWeaponMaterial(weaponid)/4)));

			if(Inventory_Add(playerid, "Materials", 11746, floatround((PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_ammo]/2))) == -1)
				return 1;
			
			SendCustomMessage(playerid, "WEAPON", "Berhasil melakukan scrap "RED"%d aminisi "WHITE"menjadi "YELLOW"%d material(s)", PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_ammo], floatround((PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_ammo]/2)));

			ResetWeaponID(playerid, weaponid);
		}
	}
	else if(!strcmp(category, "destroy"))
	{
		new weaponid, confirm[10];

		if((weaponid = GetWeapon(playerid)) == 0)
			return ShowPlayerFooter(playerid, "Kamu tidak memegang senjata apapun.");

		if(sscanf(string, "s[10]", confirm)) {
			SendCustomMessage(playerid, "USAGE", "/weapon destroy ['confirm']");
			SendCustomMessage(playerid, "WARNING","Perintah ini digunakan untuk menghancurkan habis senjatamu, termasuk peluru di dalamnya!");
			SendCustomMessage(playerid, "WARNING","Tidak ada refund setelah menggunakan perintah ini!");
			return 1;
		}

		if(!strcmp(confirm, "confirm"))
		{
			SendCustomMessage(playerid, "WEAPON", "Berhasil menghancurkan senjata "RED"%s.", ReturnWeaponName(weaponid));
			ResetWeaponID(playerid, weaponid);
		}
	}
	else if(!strcmp(category, "view"))
	{
		new userid;

		if(sscanf(string, "u", userid))
			return SendSyntaxMessage(playerid, "/weapon view [playerid/PartOfName]");

		if(userid == INVALID_PLAYER_ID || !IsPlayerNearPlayer(playerid, userid, 5.0)) 
			return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Player tidak login atau tidak bedara didekatmu.");

		ShowPlayerWeapon(playerid, userid);		
		SendServerMessage(playerid, "Kamu memperlihatkan senjatamu kepada %s.", ReturnName(userid, 0));
	}
	else SendSyntaxMessage(playerid, "/weapon [give/view/acceptview/scrap/destroy]");
	return 1;
}
CMD:repairgun(playerid, params[]) 
{

	if(IsPlayerInAnyVehicle(playerid))
		return SendErrorMessage(playerid, "Turun dari kendaraan terlebih dahulu!");

	if(IsPlayerDuty(playerid)) 
		return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Kamu sedang duty faction.");

	if(GetPlayerJob(playerid) != JOB_GUN_MAKER)
		return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Kamu tidak bekerja sebagai arms dealer.");

	PlayerRepairWeapon(playerid);
	return 1;
}

CMD:craft(playerid, params[])
{
	if(IsPlayerInAnyVehicle(playerid))
		return SendErrorMessage(playerid, "Turun dari kendaraan terlebih dahulu!");

	if(IsPlayerDuty(playerid)) 
		return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Kamu sedang duty faction.");

	if(GetPlayerJob(playerid) != JOB_GUN_MAKER)
		return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Kamu tidak bekerja sebagai arms dealer.");

	if(PlayerData[playerid][pLevel] < 4)
		return SendErrorMessage(playerid, "They must level 2 to use this.");

	Dialog_Show(playerid, Crafting, DIALOG_STYLE_LIST, "Crafting", "Bobby Pin\t20 Materials", "Create", "Cancel");		
	return 1;
}

CMD:creategun(playerid, params[]) 
{

	if(IsPlayerInAnyVehicle(playerid))
		return SendErrorMessage(playerid, "Turun dari kendaraan terlebih dahulu!");

	if(IsPlayerDuty(playerid)) 
		return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Kamu sedang duty faction.");

	if(GetPlayerJob(playerid) != JOB_GUN_MAKER)
		return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Kamu tidak bekerja sebagai arms dealer.");

	if(PlayerData[playerid][pLevel] < 2)
		return SendErrorMessage(playerid, "They must level 2 to use this.");
		
	new string[300];

	strcat(string, "Weapon\tAmmo\tMaterial\n");
	for(new i = 0; i < sizeof(g_aWeaponItems); i++) {
		if(g_aWeaponItems[i][wep_auth] == PLAYER_NORMAL) {
			strcat(string, sprintf("%s\t%d\t%d\n", ReturnWeaponName(g_aWeaponItems[i][wep_model]), (g_aWeaponSlots[g_aWeaponItems[i][wep_model]] == 1) ? (1) : (((MaxGunAmmo[g_aWeaponItems[i][wep_model]] * 2)/10)), g_aWeaponItems[i][wep_material]));
		}

		if(GetFactionType(playerid) == FACTION_FAMILY)
		{
			if(g_aWeaponItems[i][wep_auth] == PLAYER_OFFICIAL) {
				strcat(string, sprintf("%s\t%d\t%d\n", ReturnWeaponName(g_aWeaponItems[i][wep_model]), (g_aWeaponSlots[g_aWeaponItems[i][wep_model]] == 1) ? (1) : (((MaxGunAmmo[g_aWeaponItems[i][wep_model]] * 2)/10)), g_aWeaponItems[i][wep_material]));
			}
		}
	}
	Dialog_Show(playerid, CreateGun, DIALOG_STYLE_TABLIST_HEADERS, "Create Gun", string, "Create", "Cancel");
	return 1;
}

CMD:buymaterials(playerid, params[]) 
{
	new 
		amount,
		id,
		price = Economy_MaterialPrice(),
		total = 0
	;

	if((id = Job_NearestPoint(playerid)) != -1 && JobData[id][jobType] == JOB_GUN_MAKER) 
	{   
		if(sscanf(params, "d", amount))
			return SendSyntaxMessage(playerid, "/buymaterials [jumlah (%s/mats)]", FormatMoney(price));

		if(amount < 0)
			return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Jumlah yang di masukkan tidak boleh kurang dari nol.");

		total = price * amount;

		if(GetMoney(playerid) < (total))
			return SendErrorMessage(playerid, "Uang yang kamu miliki kurang untuk membeli %d material (%s).", amount, FormatMoney(total));

		if((Inventory_Count(playerid, "Materials") + amount) > 10000)
			return SendErrorMessage(playerid, "Kamu hanya dapat membeli %d material.", (10000-Inventory_Count(playerid, "Materials")));

		if(Inventory_Add(playerid, "Materials", 11746, amount) == -1)
			return 1;

		SendServerMessage(playerid, "Berhasil membeli "YELLOW"%d material "WHITE"dengan total harga "GREEN"%s.", amount, FormatMoney((total)));
		GiveMoney(playerid, -(total));
	}
	else SendServerMessage(playerid, "Kamu tidak berada digudang penjualan material.");

	return 1;
}

CMD:checkserial(playerid, params[])
{
	new weaponid;
	if((weaponid = GetWeapon(playerid)) == 0) 
        return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Kamu tidak memegang senjata apapun.");

	new 
		serial_number[15],
		serial_id[10],
		length = strlen(PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_serial])
	;
	if(!strcmp(PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_serial], "0", true)) 
	{
		SendServerMessage(playerid, "Weapon Serial Number: "RED"None");
	}
	else
	{
		strmid(serial_number, PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_serial], 0, 4);
		strmid(serial_id, PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_serial], 4, length);
		SendServerMessage(playerid, "Weapon Serial Number: %s-%s", serial_number, serial_id);
	}
	return 1;
}
CMD:createammo(playerid, params[])
{
	if(IsPlayerInAnyVehicle(playerid))
		return SendErrorMessage(playerid, "Turun dari kendaraan terlebih dahulu!");

	if(IsPlayerDuty(playerid)) 
		return ShowPlayerFooter(playerid, "~r~ERROR~w~Kamu sedang duty faction.");

	if(GetPlayerJob(playerid) != JOB_GUN_MAKER)
		return ShowPlayerFooter(playerid, "~r~ERROR~w~Kamu tidak bekerja sebagai arms dealer.");

	if(!Inventory_HasItem(playerid, "Materials"))
		return ShowPlayerFooter(playerid, "~r~ERROR~w~Kamu tidak memiliki material.");

	new
		confirm[24],
		weaponid
	;

	if((weaponid = GetWeapon(playerid)) == 0) 
		return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Kamu tidak memegang senjata apapun.");

	if(g_aWeaponSlots[weaponid] == 1)
		return ShowPlayerFooter(playerid, "~g~INFO: ~w~Senjata ini tidak memerlukan amunisi.");

	if(sscanf(params, "s[24]", confirm)) {
		SendCustomMessage(playerid, "USAGE", "/createammo ['confirm']");
		SendCustomMessage(playerid, "DETAILS", "Weapon: "RED"%s"WHITE", current: "YELLOW"%d/%d ammo", ReturnWeaponName(weaponid), ReturnWeaponAmmo(playerid, weaponid), MaxGunAmmo[weaponid]);
		SendCustomMessage(playerid, "DETAILS", "Creating ammo: "YELLOW"%d"WHITE", material cost: "YELLOW"%d unit(s)", (MaxGunAmmo[weaponid]-ReturnWeaponAmmo(playerid, weaponid)), floatround(((MaxGunAmmo[weaponid]-ReturnWeaponAmmo(playerid, weaponid))/2)));
		return 1;
	}

	new amount = (MaxGunAmmo[weaponid]-ReturnWeaponAmmo(playerid, weaponid));

	if(!strcmp(confirm, "confirm"))
	{
		if(floatround(amount/2) > Inventory_Count(playerid, "Materials"))
			return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Jumlah material tidak mencukupi.");

		if(weaponid > 22 || weaponid < 38)
		{
			PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_ammo] += amount;
			Inventory_Remove(playerid, "Materials", floatround((amount/2)));

			SendCustomMessage(playerid, "AMMO","Sukses membuat "YELLOW"%d amunisi (%d material) "WHITE"untuk senjata "RED"%s.", amount, floatround((amount/2)), ReturnWeaponName(weaponid));
		}
		else ShowPlayerFooter(playerid, "~g~INFO: ~w~Senjata ini tidak memerlukan amunisi.");
	}
	return 1;
}

Dialog:CreateGun(playerid, response, listitem, inputtext[]) 
{
	if(response)
	{
		new 
			material = g_aWeaponItems[listitem][wep_material],
			model = g_aWeaponItems[listitem][wep_model]
		;
		if(Inventory_Count(playerid, "Materials") < material)
        	return SendErrorMessage(playerid, "You don't have enough materials.");

        if(PlayerHasWeapon(playerid, model))
			return SendErrorMessage(playerid, "You already have %s, /createammo to add more ammo.", ReturnWeaponName(model));

        if(PlayerHasWeaponInSlot(playerid, model))
        	return SendErrorMessage(playerid, "You already have weapon in the same slot.");

		GivePlayerWeaponEx(playerid, model, ((MaxGunAmmo[model] * 2)/10));
        Inventory_Remove(playerid, "Materials", material);
		SendCustomMessage(playerid, "WEAPON", "Berhasil membuat "RED"%s (%d ammo) "WHITE"dengan "YELLOW"%d material.", ReturnWeaponName(model), ((MaxGunAmmo[model] * 2)/10), material);
	}
	return 1;
}

Dialog:Crafting(playerid, response, listitem, inputtext[]) 
{
	if(response)
	{
		switch(listitem)
		{
			case 0:
			{
				Dialog_Show(playerid, CraftQuantity, DIALOG_STYLE_INPUT, "Crafting Amount", "Quantity / Amount you want to craft for this item?", "Craft", "Cancel");
			}
		}
	}
	return 1;
}

Dialog:CraftQuantity(playerid, response, listitem, inputtext[])
{
	if(response)
	{
		new jumlah = (20*strval(inputtext));

		if(!(1 <= strval(inputtext) <= 20))
			return Dialog_Show(playerid, CraftQuantity, DIALOG_STYLE_INPUT, "Crafting Amount", WHITE"(ERROR): Jumlah hanya di batasi 1 sampai 20!\n\nBerapa banyak quantity yang akan kamu buat? "YELLOW"(minimal 1 lockpick - maximal 20)", "Craft", "Cancel");
		
		if(Inventory_Count(playerid, "Materials") < jumlah)
			return Dialog_Show(playerid, CraftQuantity, DIALOG_STYLE_INPUT, "Crafting Amount", WHITE"(ERROR): Materials kamu kurang!\n\nBerapa banyak quantity yang akan kamu buat? "YELLOW"(minimal 1 lockpick - maximal 20)", "Craft", "Cancel");

		if(Inventory_Count(playerid, "Bobby Pin") > 20)
			return Dialog_Show(playerid, CraftQuantity, DIALOG_STYLE_INPUT, "Crafting Amount", WHITE"(ERROR): Anda tidak bisa menampung bobby pin lebih dari 20!\n\nBerapa banyak quantity yang akan kamu buat? "YELLOW"(minimal 1 lockpick - maximal 20)", "Craft", "Cancel");

		if(Inventory_Add(playerid, "Bobby Pin", 11746, strval(inputtext)) != -1)
		{
			Inventory_Remove(playerid, "Materials", jumlah);
			SendServerMessage(playerid, "Sukses membuat %d bobby pin menggunakan %d materials", strval(inputtext), jumlah);
		}
	}
	return 1;
}

Dialog:GiveOnly(playerid, response, listitem, inputtext[]) {

    if(response)
    {
        new weaponid = strval(inputtext);
        new userid = SelectedPlayer[playerid];
        if(IsPlayerInAnyVehicle(userid))
            return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Player tersebut harus turun terlebih dahulu dari kendaraan!.");

        if(PlayerHasWeaponInSlot(userid, weaponid))
            return ShowPlayerFooter(playerid, "~r~ERROR: ~w~Player tersebut memiliki senjata di slot yang sama.");

        GivePlayerWeaponEx(userid, weaponid, PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_ammo], PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_durability], PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_serial]);
        ResetWeaponID(playerid, weaponid);

        SendServerMessage(playerid, "Kamu telah memberi "RED"%s"WHITE" kepada "YELLOW"%s.", ReturnWeaponName(weaponid), ReturnName(userid));
        SendServerMessage(userid, ""YELLOW"%s"WHITE" memberikan "RED"%s"WHITE" kepadamu.", ReturnName(playerid), ReturnWeaponName(weaponid));
    }
    return 1;
}
Dialog:RepairOnly(playerid, response, listitem, inputtext[]) {

    if(response)
    {
        new weaponid = strval(inputtext); 
        new repairment = (WEAPON_DURABILITY - PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_durability]);

        if(Inventory_Count(playerid, "Materials") < repairment)
            return SendErrorMessage(playerid, "You don't have enough materials.");

        if(PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_durability] >= 500)
            return SendErrorMessage(playerid, "You only able to repair a weapon with durability below 500!");

        Inventory_Remove(playerid, "Materials", repairment);
        PlayerGuns[playerid][g_aWeaponSlots[weaponid]][weapon_durability] = WEAPON_DURABILITY;
        ShowPlayerFooter(playerid, "~g~INFO: ~w~Kamu telah memperbaiki durability senjata mu."); 
    }
    return 1;
}