FUNC::UpdatePlayerSkin(playerid, skinid)
{
	SetPlayerSkin(playerid, skinid);
	PlayerData[playerid][pSkin] = skinid;
    return 1;
}

function ResetVariable(playerid)
{
	for (new i = 0; i != MAX_INVENTORY; i ++)
	{
	    InventoryData[playerid][i][invExists] = false;
	    InventoryData[playerid][i][invModel] = 0;
	    InventoryData[playerid][i][invQuantity] = 0;
	}
	PlayerData[playerid][pEnergy] = 100;
	PlayerData[playerid][pMoney] = 0;
	PlayerData[playerid][pInHouse] = -1;
	PlayerData[playerid][pInBiz] = -1;
	PlayerData[playerid][pAdmin] = 0;
	PlayerData[playerid][pAduty] = false;
	PlayerData[playerid][pListitem] = -1;
	PlayerData[playerid][pAttempt] = 0;
	PlayerData[playerid][pCalling] = INVALID_PLAYER_ID;
	PlayerData[playerid][pSpawned] = false;
	PlayerData[playerid][pOnline] = 0;
	PlayerData[playerid][pEditPump] = -1;
	PlayerData[playerid][pEditStructure] = -1;
	PlayerData[playerid][pEditHouseStructure] = -1;
	StopRefilling(playerid);
	for(new i; i < sizeof struct_character; i++)
	{
		switch(struct_character[i][structType])
		{
			case SQL_INT: PlayerData[playerid][struct_character[i][structChar]] = struct_character[i][structDefault];
			case SQL_INVALID: PlayerData[playerid][struct_character[i][structChar]] = struct_character[i][structDefault];
		}
	}
	return 1;
}

function IsPlayerSpawn(playerid)
	return PlayerData[playerid][pSpawned];

GiveMoney(playerid, amount, E_ECONOMY_ACTION:action = ECONOMY_ACTION_NONE)
{
    if(playerid != INVALID_PLAYER_ID)
    {
        PlayerData[playerid][pMoney] += amount;
        GivePlayerMoney(playerid, amount);
        if(action == ECONOMY_TAKE_SUPPLY)
        {
			D:5("GiveMoney(%i,%i,ECONOMY_TAKE_SUPPLY)", playerid, abs(amount));
            Economy_RequestSupply(abs(amount));
        }
		if(action == ECONOMY_ADD_SUPPLY)
		{
			D:5("GiveMoney(%i,%i,ECONOMY_ADD_SUPPLY)", playerid, abs(amount));
			Economy_AddSupply(abs(amount));
		}
    }
	return 1;
}

function GetEnergy(playerid)
	return PlayerData[playerid][pEnergy];
	
function GetMoney(playerid)
{
	return PlayerData[playerid][pMoney];
}

ReturnName(playerid)
{
    static
        name[MAX_PLAYER_NAME + 1];

    GetPlayerName(playerid, name, sizeof(name));
    if(PlayerData[playerid][pMaskOn])
    {
        format(name, sizeof(name), "Mask_#%d", PlayerData[playerid][pMaskID]);
	}
	else
	{
	    for (new i = 0, len = strlen(name); i < len; i ++)
		{
	        if (name[i] == '_') name[i] = ' ';
		}
	}
    return name;
}

GetName(playerid)
{
	new name[MAX_PLAYER_NAME];
 	GetPlayerName(playerid,name,sizeof(name));
	return name;
}

FormatNumber(number, prefix[] = "$")
{
	static
		value[32],
		length;

	format(value, sizeof(value), "%d", (number < 0) ? (-number) : (number));

	if ((length = strlen(value)) > 3)
	{
		for (new i = length, l = 0; --i >= 0; l ++) {
		    if ((l > 0) && (l % 3 == 0)) strins(value, ",", i + 1);
		}
	}
	if (prefix[0] != 0)
	    strins(value, prefix, 0);

	if (number < 0)
		strins(value, "-", 0);

	return value;
}

function KickEx(playerid)
{
	SaveData(playerid);
	SetTimerEx("KickTimer", 1000, false, "d", playerid);
	return 1;
}

FUNC::KickTimer(playerid)
{
	Kick(playerid);
}

function CheckAccount(playerid)
{
	new query[256];
	format(query, sizeof(query), "SELECT * FROM `PlayerUCP` WHERE `UCP` = '%s' LIMIT 1;", GetName(playerid));
	mysql_tquery(sqlcon, query, "CheckPlayerUCP", "d", playerid);
	return 1;
}

FUNC::PlayerCheck(playerid, rcc)
{
	if(rcc != g_RaceCheck{playerid})
	    return Kick(playerid);
	    
	CheckAccount(playerid);
	return true;
}

stock CheckPin(playerid, pin)
{
    if(pin <= 0) return SendServerMessage(playerid, "Invalid Pin");

    new query[100], storedPin;
    format(query, sizeof(query), "SELECT `pin` FROM `PlayerUCP` WHERE `UCP` = '%s' LIMIT 1;", GetName(playerid));
    new Cache: cache = mysql_query(sqlcon, query);
    cache_get_value_name_int(0, "pin", storedPin);
	cache_delete(cache);
    return (storedPin == pin);
}

/*FUNC::CheckPlayerUCP(playerid)
{
	new rows = cache_num_rows(), password[BCRYPT_HASH_LENGTH];
	new str[256];
	if(!rows)
	{
		ShowPlayerDialog(playerid, INVALID_DIALOG_ID, DIALOG_STYLE_MSGBOX, "UCP", "Your account isn't registered yet, please register your account in our web", "Close", "");
		KickEx(playerid);
		return;
	}
	cache_get_value_name(0, "Password", password, sizeof(password));
	if(!strcmp(password, "", true))
	{
		ShowPlayerDialog(playerid, DIALOG_PIN, DIALOG_STYLE_INPUT, "PIN UCP", "Input your UCP Pin below", "Enter", "Cancel");
	}
	cache_get_value_name(0, "UCP", tempUCP[playerid]);
	format(str, sizeof(str), "{FFFFFF}UCP Account: {00FFFF}%s\n{FFFFFF}Attempts: {00FFFF}%d/5\n{FFFFFF}Password: {FF00FF}(Input Below)", GetName(playerid), PlayerData[playerid][pAttempt]);
	ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login to Reliazed", str, "Login", "Exit");
	{
	    cache_get_value_name(0, "UCP", tempUCP[playerid]);
	    format(str, sizeof(str), "{FFFFFF}UCP Account: {00FFFF}%s\n{FFFFFF}Attempts: {00FFFF}%d/5\n{FFFFFF}Password: {FF00FF}(Input Below)", GetName(playerid), PlayerData[playerid][pAttempt]);
		ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login to Reliazed", str, "Login", "Exit");
	}
	else
	{
	    format(str, sizeof(str), "{FFFFFF}UCP Account: {00FFFF}%s\n{FFFFFF}Attempts: {00FFFF}%d/5\n{FFFFFF}Create Password: {FF00FF}(Input Below)", GetName(playerid), PlayerData[playerid][pAttempt]);
		ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register to Reliazed", str, "Register", "Exit");
	}
	return 1;
}*/

FUNC::CheckPlayerUCP(playerid)
{
    new rows = cache_num_rows(), input_pin;
    new str[512];
    if(!rows)
    {
        ShowPlayerDialog(playerid, INVALID_DIALOG_ID, DIALOG_STYLE_MSGBOX, "UCP Registration", "It seems like you haven't registered your account yet.\nPlease visit our website to complete your registration and join our community!\n\nOnce registered, you'll be able to access all the exciting features we offer.", "Close", "");
        KickEx(playerid);
        return 0;
    }
    else
    {
        cache_get_value_name_int(0, "input_pin", input_pin);
        if(input_pin == 0)
        {
            ShowPlayerDialog(playerid, DIALOG_PIN, DIALOG_STYLE_INPUT, "Set Your UCP Pin", "To ensure the safety of your account, please create a PIN for your UCP.\n\nThis will be used to access sensitive areas of your account and protect your data. Enter your desired PIN below.", "Enter", "Cancel");
            return 1;
        }
        else
        {
            cache_get_value_name(0, "UCP", tempUCP[playerid]);
            format(str, sizeof(str), "{FFFFFF}Welcome back, {00FFFF}%s!\n\n{FFFFFF}Your UCP Account: {00FFFF}%s\n{FFFFFF}You have {00FFFF}%d/5 {FFFFFF}login attempts remaining.\n{FFFFFF}To continue, please enter your {FF00FF}UCP Password {FFFFFF}below.\n\n{FFFFFF}Make sure you keep your password secure. If you encounter any issues, feel free to reset it via the website.", GetName(playerid), tempUCP[playerid], PlayerData[playerid][pAttempt]);
            ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login to Reliazed", str, "Login", "Exit");
        }
    }
    return 1;
}


function SetupPlayerData(playerid)
{
    SetSpawnInfo(playerid, 0, PlayerData[playerid][pSkin], 1642.1681, -2333.3689, 13.5469, 0.0, 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid);
    GiveMoney(playerid, Data_GetInt("server data", "starterpack-money"));
	for(new i; i < 15; i++) SendClientMessage(playerid, -1, "");
	SendClientMessage(playerid, -1, "-> Selamat datang di kota "YELLOW"los santos"WHITE", untuk warga baru anda mendapatkan:");
	SendCustomMessage(playerid, "STARTERPACK", "mendapatkan uang sebanyak "GREEN"%s", FormatNumber(Data_GetInt("server data", "starterpack-money")));
    return 1;
}

function SaveData(playerid)
{
	new query[2512];
	if(PlayerData[playerid][pSpawned])
	{
		GetPlayerHealth(playerid, PlayerData[playerid][pHealth]);
		GetPlayerPos(playerid, PlayerData[playerid][pPos][0], PlayerData[playerid][pPos][1], PlayerData[playerid][pPos][2]);

		mysql_format(sqlcon, query, sizeof(query), "UPDATE `characters` SET ");
		mysql_format(sqlcon, query, sizeof(query), "%s`PosX`='%f', ", query, PlayerData[playerid][pPos][0]);
        mysql_format(sqlcon, query, sizeof(query), "%s`PosY`='%f', ", query, PlayerData[playerid][pPos][1]);
        mysql_format(sqlcon, query, sizeof(query), "%s`PosZ`='%f', ", query, PlayerData[playerid][pPos][2]);
	    mysql_format(sqlcon, query, sizeof(query), "%s`Health`='%f', ", query, PlayerData[playerid][pHealth]);
	    mysql_format(sqlcon, query, sizeof(query), "%s`World`='%d', ", query, GetPlayerVirtualWorld(playerid));
	    mysql_format(sqlcon, query, sizeof(query), "%s`Interior`='%d', ", query, GetPlayerInterior(playerid));
	    mysql_format(sqlcon, query, sizeof(query), "%s`Age`='%s', ", query, PlayerData[playerid][pAge]);
	    mysql_format(sqlcon, query, sizeof(query), "%s`Origin`='%s', ", query, PlayerData[playerid][pOrigin]);
	    mysql_format(sqlcon, query, sizeof(query), "%s`Gender`='%d', ", query, PlayerData[playerid][pGender]);
	    mysql_format(sqlcon, query, sizeof(query), "%s`Skin`='%d', ", query, PlayerData[playerid][pSkin]);
	    mysql_format(sqlcon, query, sizeof(query), "%s`Energy`='%d', ", query, PlayerData[playerid][pEnergy]);
	    mysql_format(sqlcon, query, sizeof(query), "%s`AdminLevel`='%d', ", query, PlayerData[playerid][pAdmin]);
	    mysql_format(sqlcon, query, sizeof(query), "%s`InBiz`='%d', ", query, PlayerData[playerid][pInBiz]);
	    mysql_format(sqlcon, query, sizeof(query), "%s`Money`='%d', ", query, PlayerData[playerid][pMoney]);
	    mysql_format(sqlcon, query, sizeof(query), "%s`InHouse`='%s', ", query, PlayerData[playerid][pInHouse]);
	    mysql_format(sqlcon, query, sizeof(query), "%s`UCP`='%s' ", query, PlayerData[playerid][pUCP]);
	    mysql_format(sqlcon, query, sizeof(query), "%sWHERE `pID` = %d", query, PlayerData[playerid][pID]);
		mysql_query(sqlcon, query, true);

		SQL_SaveCharacter(playerid);
	}
	return 1;
}

FUNC::LoadCharacterData(playerid)
{
	cache_get_value_name_int(0, "pID", PlayerData[playerid][pID]);
	cache_get_value_name(0, "Name", PlayerData[playerid][pName]);
	cache_get_value_name_float(0, "PosX", PlayerData[playerid][pPos][0]);
	cache_get_value_name_float(0, "PosY", PlayerData[playerid][pPos][1]);
	cache_get_value_name_float(0, "PosZ", PlayerData[playerid][pPos][2]);
	cache_get_value_name_float(0, "Health", PlayerData[playerid][pHealth]);
	cache_get_value_name_int(0, "Interior", PlayerData[playerid][pInterior]);
	cache_get_value_name_int(0, "World", PlayerData[playerid][pWorld]);
	cache_get_value_name(0, "Age", PlayerData[playerid][pAge]);
	cache_get_value_name(0, "Origin", PlayerData[playerid][pOrigin]);
	cache_get_value_name_int(0, "Gender", PlayerData[playerid][pGender]);
	cache_get_value_name_int(0, "Skin", PlayerData[playerid][pSkin]);
	cache_get_value_name(0, "UCP", PlayerData[playerid][pUCP]);
	cache_get_value_name_int(0, "Energy", PlayerData[playerid][pEnergy]);
	cache_get_value_name_int(0, "AdminLevel", PlayerData[playerid][pAdmin]);
	cache_get_value_name_int(0, "InHouse", PlayerData[playerid][pInHouse]);
	cache_get_value_name_int(0, "InBiz", PlayerData[playerid][pInBiz]);
	cache_get_value_name_int(0, "Money", PlayerData[playerid][pMoney]);
	
	MySQL_LoadCharacter(playerid);
	new invQuery[256];
    format(invQuery, sizeof(invQuery), "SELECT * FROM `inventory` WHERE `ID` = '%d'", PlayerData[playerid][pID]);
	mysql_tquery(sqlcon, invQuery, "LoadPlayerItems", "d", playerid);
	
    SetSpawnInfo(playerid, 0, PlayerData[playerid][pSkin], PlayerData[playerid][pPos][0], PlayerData[playerid][pPos][1], PlayerData[playerid][pPos][2], 0.0, 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid);
    SendServerMessage(playerid, "Successfully loaded your characters database!");
    LoadPlayerVehicle(playerid);
    return 1;
}

FUNC::HashPlayerPassword(playerid, hashid)
{
	new
		query[256],
		hash[BCRYPT_HASH_LENGTH]
	;

    bcrypt_get_hash(hash, sizeof(hash));

	GetPlayerName(playerid, tempUCP[playerid], MAX_PLAYER_NAME + 1);

	format(query,sizeof(query),"UPDATE `playerucp` SET `Password` = '%s' WHERE `UCP` = '%s'", hash, tempUCP[playerid]);
	mysql_tquery(sqlcon, query);

    SendServerMessage(playerid, "Your UCP is successfully registered!");
    CheckAccount(playerid);
	return 1;
}

ShowCharacterList(playerid)
{
	new name[256], count, sgstr[128];

	for (new i; i < MAX_CHARS; i ++) if(PlayerChar[playerid][i][0] != EOS)
	{
	    format(sgstr, sizeof(sgstr), "%s\n", PlayerChar[playerid][i]);
		strcat(name, sgstr);
		count++;
	}
	if(count < MAX_CHARS)
		strcat(name, "< Create Character >");

	ShowPlayerDialog(playerid, DIALOG_CHARLIST, DIALOG_STYLE_LIST, "Character List", name, "Select", "Quit");
	return 1;
}

FUNC::LoadCharacter(playerid)
{
	for (new i = 0; i < MAX_CHARS; i ++)
	{
		PlayerChar[playerid][i][0] = EOS;
	}
	for (new i = 0; i < cache_num_rows(); i ++)
	{
		cache_get_value_name(i, "Name", PlayerChar[playerid][i]);
	}
  	ShowCharacterList(playerid);
  	return 1;
}

FUNC::OnPlayerPasswordChecked(playerid, bool:success)
{
	new str[256];
    format(str, sizeof(str), "{FFFFFF}UCP Account: {00FFFF}%s\n{FFFFFF}Attempts: {00FFFF}%d/5\n{FFFFFF}Password: {FF00FF}(Input Below)", GetName(playerid), PlayerData[playerid][pAttempt]);
    
	if(!success)
	{
	    if(PlayerData[playerid][pAttempt] < 5)
	    {
		    PlayerData[playerid][pAttempt]++;
	        ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login to Reliazed", str, "Login", "Exit");
			return 1;
		}
		else
		{
		    SendServerMessage(playerid, "Kamu telah salah memasukan password sebanyak {FFFF00}5 kali!");
		    KickEx(playerid);
			return 1;
		}
	}
	new query[256];
	format(query, sizeof(query), "SELECT `Name` FROM `characters` WHERE `UCP` = '%s' LIMIT %d;", GetName(playerid), MAX_CHARS);
	mysql_tquery(sqlcon, query, "LoadCharacter", "d", playerid);
	return 1;
}

FUNC::InsertPlayerName(playerid, name[])
{
	new count = cache_num_rows(), query[145], Cache:execute;
	if(count > 0)
	{
        ShowPlayerDialog(playerid, DIALOG_MAKECHAR, DIALOG_STYLE_INPUT, "Create Character", "ERROR: This name is already used by the other player!\nInsert your new Character Name\n\nExample: Finn_Xanderz, Javier_Cooper etc.", "Create", "Back");
	}
	else
	{
		mysql_format(sqlcon,query,sizeof(query),"INSERT INTO `characters` (`Name`,`UCP`) VALUES('%e','%e')",name,GetName(playerid));
		execute = mysql_query(sqlcon, query);
		PlayerData[playerid][pID] = cache_insert_id();
	 	cache_delete(execute);
	 	SetPlayerName(playerid, name);
		format(PlayerData[playerid][pName], MAX_PLAYER_NAME, name);
	 	Dialog_Show(playerid, SelectAge, DIALOG_STYLE_INPUT, "Date of birth", "Enter the date of birth (date/month/year): 15/04/1998:", "Continue", "Cancel");
	}
	return 1;
}

Dialog:SelectAge(playerid, response, listitem, inputtext[])
{
	if(response)
	{
		new
			iDay,
			iMonth,
			iYear,
			day,
			month,
			year;

		getdate(year, month, day);

		static const
				arrMonthDays[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

		if(sscanf(inputtext, "p</>ddd", iDay, iMonth, iYear)) {
			Dialog_Show(playerid, SelectAge, DIALOG_STYLE_INPUT, "Date of birth", "Error! Invalid Input\nEnter the date of birth (date/month/year): 15/04/1998", "Pilih", "Batal");
		}
		else if(iYear < 1900  || iYear > year) {
			Dialog_Show(playerid, SelectAge, DIALOG_STYLE_INPUT, "Year of birth", "Error! Invalid Input\nEnter the date of birth (date/month/year): 15/04/1998", "Pilih", "Batal");
		}
		else if(iMonth < 1 ||  iMonth > 12) {
			Dialog_Show(playerid, SelectAge, DIALOG_STYLE_INPUT, "Born Month", "Error! Invalid Input\nEnter the date of birth (date/month/year): 15/04/1998", "Pilih", "Batal");
		}
		else if(iDay < 1 || iDay > arrMonthDays[iMonth - 1]) {
			Dialog_Show(playerid, SelectAge, DIALOG_STYLE_INPUT, "Date of birth", "Error! Invalid Input\nEnter the date of birth (date/month/year): 15/04/1998", "Pilih", "Batal");
		}
		else 
		{
			format(PlayerData[playerid][pAge], 50, inputtext);
			format(PlayerData[playerid][pOrigin], 50, "Los Santos");
			ShowPlayerDialog(playerid, DIALOG_GENDER, DIALOG_STYLE_LIST, "Character Gender", "Male\nFemale", "Continue", "Cancel");
		}
	}
	else
	{
		SendErrorMessage(playerid, "You cannot cancel the character making!");
		Dialog_Show(playerid, SelectAge, DIALOG_STYLE_INPUT, "Date of birth", "Enter the date of birth (date/month/year): 15/04/1998:", "Continue", "Cancel");
	}
	return 1;
}