Atm_Load()
{
	static Cache:result, fmt_sql[300], fmt[400];
	mysql_format(sqlcon, fmt_sql, sizeof fmt_sql, "SELECT * FROM "ATM_TABLE_SQL);
	result = mysql_query(sqlcon, fmt_sql);
	if(cache_num_rows())
	{
		for(new i = 0; i < cache_num_rows(); i++)
		{
			Iter_Add(DynamicAtm, i);

			AtmData[i][AtmSQLID] = cache_get_field_content_int(i, "atmSQLID");
			cache_get_field_content(i, "atmPos", fmt, 128);
			sscanf(fmt, "p<|>ffffffii", AtmData[i][AtmPos][0], AtmData[i][AtmPos][1], AtmData[i][AtmPos][2],
			AtmData[i][AtmPos][3],
			AtmData[i][AtmPos][4],
			AtmData[i][AtmPos][5],
			AtmData[i][AtmPos][6],
			AtmData[i][AtmPos][7]
			);

			AtmData[i][AtmCash] = cache_get_field_content_int(i, "atmCash");

			Atm_Sync(i);
		}
		printf("*** [MySQL] Successfully loading Automatic Teller Machine as much as %d rows ", cache_num_rows());
	}
	cache_delete(result);
	return 1;
}

function GetClosestATM(playerid, Float:r)
{
	foreach(new atm : DynamicAtm) if(IsPlayerInRangeOfPoint(playerid, r, GetAtmDataArray(atm, AtmPos<0>), GetAtmDataArray(atm, AtmPos<1>), GetAtmDataArray(atm, AtmPos<2>)))
	{
		return atm;
	}
	return -1;
}

function Atm_Save(i)
{
	static fmt[400], fmt2[230];
	format(fmt2, sizeof fmt2, "%f|%f|%f|%f|%f|%f|%i|%i", AtmData[i][AtmPos][0], AtmData[i][AtmPos][1], AtmData[i][AtmPos][2],
			AtmData[i][AtmPos][3],
			AtmData[i][AtmPos][4],
			AtmData[i][AtmPos][5],
			AtmData[i][AtmPos][6],
			AtmData[i][AtmPos][7]);
	mysql_format(sqlcon, fmt, sizeof fmt, "UPDATE "ATM_TABLE_SQL" SET `atmPos` = '%s', `atmCash` = '%i' WHERE `atmSQLID` = '%i' LIMIT 1;", fmt2, AtmData[i][AtmCash], AtmData[i][AtmSQLID]);
	mysql_tquery(sqlcon, fmt);
	return 1;
}

function Atm_Sync(atm_id)
{
	static fmt[230];
	if(IsValidDynamicObject(AtmData[atm_id][AtmObject]))
		DestroyDynamicObject(AtmData[atm_id][AtmObject]);

	if(IsValidDynamic3DTextLabel(AtmData[atm_id][AtmLabel])) 
		DestroyDynamic3DTextLabel(AtmData[atm_id][AtmLabel]);
	
	formatex(fmt, "%s", FormatColor("- ATM\nUse '{f2ff00}/atm{ffffff}' to use atm."));

	AtmData[atm_id][AtmObject] = CreateDynamicObject(ATM_OBJECT_ID, AtmData[atm_id][AtmPos][0], AtmData[atm_id][AtmPos][1], AtmData[atm_id][AtmPos][2], AtmData[atm_id][AtmPos][3], AtmData[atm_id][AtmPos][4], AtmData[atm_id][AtmPos][5], _:AtmData[atm_id][AtmPos][6], _:AtmData[atm_id][AtmPos][7]);
	AtmData[atm_id][AtmLabel] = CreateDynamic3DTextLabel(fmt, COLOR_WHITE, AtmData[atm_id][AtmPos][0], AtmData[atm_id][AtmPos][1], AtmData[atm_id][AtmPos][2], 20.0, .interiorid = _:AtmData[atm_id][AtmPos][6]);
	return 1;
}

function Atm_Create(money, Float:x, Float:y, Float:z, interior, virtualworld)
{
	static iter_id, Float:rx, Float:ry, Float:rz;
	if((iter_id = Iter_Free(DynamicAtm)) != INVALID_ITER_ID)
	{
		static Cache:result;
		SetAtmDataArray(iter_id, AtmPos<0>, x);
		SetAtmDataArray(iter_id, AtmPos<1>, y);
		SetAtmDataArray(iter_id, AtmPos<2>, z);
		SetAtmDataArray(iter_id, AtmPos<3>, rx);
		SetAtmDataArray(iter_id, AtmPos<4>, ry);
		SetAtmDataArray(iter_id, AtmPos<5>, rz);
		
		AddAtmDataArray(iter_id, AtmPos<0>, (2.0 * floatsin(-GetAtmDataArray(iter_id, AtmPos<5>), degrees)));
		AddAtmDataArray(iter_id, AtmPos<1>, (2.0 * floatcos(-GetAtmDataArray(iter_id, AtmPos<5>), degrees)));
		
		SetAtmDataArray(iter_id, AtmPos<6>, interior);
		SetAtmDataArray(iter_id, AtmPos<7>, virtualworld);
		
		SetAtmData(iter_id, AtmCash, money);
	
		AtmData[iter_id][AtmObject] = CreateDynamicObject(ATM_OBJECT_ID, x, y, z, GetAtmDataArray(iter_id, AtmPos<3>), GetAtmDataArray(iter_id, AtmPos<4>), GetAtmDataArray(iter_id, AtmPos<5>), interior, virtualworld);
		
		static queries[400];
        mysql_format(sqlcon, queries, sizeof queries, "INSERT INTO "ATM_TABLE_SQL" (`atmPos`) VALUES ('%f|%f|%f|%f|%f|%f|%i|%i')", x, y, z, rx, ry, rz, interior, virtualworld);
		result = mysql_query(sqlcon, queries, true);

		SetAtmData(iter_id, AtmSQLID, cache_insert_id());
		Atm_Save(iter_id);
		Atm_Sync(iter_id);
		Iter_Add(DynamicAtm, iter_id);
		
		cache_delete(result);
		return iter_id;
	}
	return -1;
}

function Atm_Destroy(atm_id)
{
	if(IsValidDynamicObject(AtmData[atm_id][AtmObject])) DestroyDynamicObject(AtmData[atm_id][AtmObject]);
	if(IsValidDynamic3DTextLabel(AtmData[atm_id][AtmLabel])) DestroyDynamic3DTextLabel(AtmData[atm_id][AtmLabel]);
	mysql_tquery(sqlcon, sprintf("DELETE FROM "ATM_TABLE_SQL" WHERE `atmSQLID` = %i", AtmData[atm_id][AtmSQLID]));
	new tmp_Atm[E_ATM];
	AtmData[atm_id] = tmp_Atm;
	
	Iter_Remove(DynamicAtm, atm_id);

	return 1;
}

function ogmi_LoadAtm()
{
	Atm_Load();
	AccountAtm_Load();
}

function ogme_SaveAtm()
{
	foreach(new accountid : AccountAtm) AccountAtm_Save(accountid);
}

SSCANF:AtmSscanf(string[]) 
{
 	if(!strcmp(string,"create",true)) return 1;
	else if(!strcmp(string,"delete",true)) return 2;
	else if(!strcmp(string,"money",true)) return 3;
	else if(!strcmp(string,"select",true)) return 4;
 	return 0;
}

CMD:atmmenu(playerid, params[])
{
    if(!IsPlayerADuty(playerid, 3)) return PermissionError(playerid);

	static atm_id, NP[120], idx;
	if(sscanf(params, "k<AtmSscanf>S()[120]", idx, NP))
	{
		SendSyntaxMessage(playerid, "/atmmenu [opsi]");
		return SendServerMessage(playerid, "opsi: create, delete, select, money");
	}
	switch(idx)
	{
		case 1:
		{
			static money, Float:x, Float:y, Float:z;
			GetPlayerPos(playerid, x, y ,z);
			if(sscanf(NP, "i", money)) return SendSyntaxMessage(playerid, "/atmmenu create [money]");
			if((atm_id = Atm_Create(money, x, y, z, GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid))) != -1)
			{
				static frm[200];
				format(frm, sizeof frm, "You managed to make an ATM ID: %i", atm_id);
				SendClientMessage(playerid, -1, frm);
			}
			else return SendClientMessage(playerid, -1, "You cannot make an ATM because it has reached the limit.");
		}
		case 2:
		{
			if(sscanf(NP, "i", atm_id)) return SendSyntaxMessage(playerid, "/atmmenu delete [atmdid]");
			if(!Iter_Contains(DynamicAtm, atm_id)) return SendErrorMessage(playerid, "ATM machine with the ID does not exist.");
			Atm_Destroy(atm_id);
			SendServerMessage(playerid, "Successfully removing the ATM ID: %i", atm_id);
		}
		case 3:
		{
            if(!IsPlayerADuty(playerid, 4)) return PermissionError(playerid);
			static money;
			if(sscanf(NP, "ii", atm_id, money)) return SendSyntaxMessage(playerid, "/atmmenu money [atmid] [moneh]");
			if(!Iter_Contains(DynamicAtm, atm_id)) return SendErrorMessage(playerid, "ATM Server has met the maximum limit (MAX: "#MAX_ATMS")");
			SetAtmData(atm_id, AtmCash, money);
		}
		case 4:
		{
            if(!IsPlayerADuty(playerid, 4)) return  PermissionError(playerid);
			if(sscanf(NP, "i", atm_id)) return SendSyntaxMessage(playerid, "/atmmenu select [atmid]");
			if(!Iter_Contains(DynamicAtm, atm_id)) return SendErrorMessage(playerid, "ATM Id (%i) There isn't any.", atm_id);

            PlayerData[playerid][pEditingMode] = EDIT_OBJECT_ATM;
			PlayerData[playerid][pEditAtmID] = atm_id;
			EditDynamicObject(playerid, AtmData[atm_id][AtmObject]);
		}
	}
	return 1;
}

CMD:listaccountatm(playerid, params[])
{
    if(!IsPlayerADuty(playerid, DEVELOPER)) return PermissionError(playerid);
	
	new fmt[600] = "#\tAccount Name\tAccount Password\n";
	foreach(new i : AccountAtm)
	{
		format(fmt, sizeof fmt, "%s%i\t%s\t%s\n", fmt, i, AccountAtm[i][atmAccountName], AccountAtm[i][atmAccountPassword]);
	}
	return Dialog_Show(playerid, "Unused", DIALOG_STYLE_TABLIST_HEADERS, "Account ATM List", fmt,  "Close", "");
}

CMD:accountatm(playerid, params[])
{
    if(!IsPlayerADuty(playerid, OWNER)) return PermissionError(playerid);
	new names[32], accountid, amount;
	if(sscanf(params, "s[32]ii", names, accountid, amount)) return SendSyntaxMessage(playerid, "/accountatm [names] [account id] [amount]");
	if(!Iter_Contains(AccountAtm, accountid)) return 1;
	for(new i ; i < sizeof struct_accountatm; i++) if(!strcmp(names, struct_accountatm[i][structAtmName], false))
	{
		AccountAtm[accountid][struct_accountatm[i][structVar]] = amount;
		SendCustomMessage(playerid, "ATM", "Anda merubah %s menjadi %i", names, amount);
		return 1;
	}
	return SendSyntaxMessage(playerid, "/accountatm [names] [accountid] [amount]");
}

CMD:atm(playerid, params[])
{
	if(GetClosestATM(playerid, 3.5) != -1)
	{
		return BankShowMenu(playerid);
	}
	else return SendErrorMessage(playerid, "You are not close to any ATM." );
}

CMD:nearatm(playerid, params[])
{
	if(!IsPlayerADuty(playerid, 1))
		return PermissionError(playerid);

	static atm_id;
	if((atm_id = GetClosestATM(playerid, 3.5)) != -1)
	{
		return SendServerMessage(playerid, "ATMs that are close to you are ID: " YELLOW"%i", atm_id);
	}
	else return SendErrorMessage(playerid, "You are not close to any ATM." );
}

BankShowMenu(playerid)
{
    if(PlayerData[playerid][pAccountATM] == -1)
    {
        Dialog_Show(playerid, register_accountatm, DIALOG_STYLE_LIST, "Atm Menu", "Create Account\nLogin Account", "Select", "Close");
    }
    else
    {
        Dialog_Show(playerid, AtmMenu, DIALOG_STYLE_LIST, "Atm Menu", "Withdraw\nTransfer\nDeposit\n"RED"Logout", "Select", "Cancel");
    }
	return 1;
}

function AccountAtm_GetIDBySQLID(sqlid)
{
    foreach(new id : AccountAtm) if(AccountAtm[id][atmAccountSQLID] == sqlid) return id;
    return -1;
}

function AccountAtm_Load()
{
    new Cache:result = mysql_query(sqlcon, sprintf("SELECT * FROM `account_atm`"));
    {
        new rows = cache_num_rows();
        for(new i; i < rows; i++)
        {
            AccountAtm[i][atmAccountSQLID] = cache_get_field_content_int(i, "SQLID");
            AccountAtm[i][atmAccountMoney] = cache_get_field_content_int(i, "money");
            AccountAtm[i][atmAccountStatus] = cache_get_field_content_int(i, "status");
            
            cache_get_field_content(i, "username", AccountAtm[i][atmAccountName]);
            cache_get_field_content(i, "password", AccountAtm[i][atmAccountPassword]);
            Iter_Add(AccountAtm, i);
        }
        printf("*** [Account Atm] Loaded %d rows", rows);
    }
    cache_delete(result);
    return 1;
}

function AccountAtm_Save(accountid)
{
    new sqlid = AccountAtm[accountid][atmAccountSQLID];
    SQL_UpdateCustomInt(sqlid, "account_atm", "money", AccountAtm[accountid][atmAccountMoney]);
    SQL_UpdateCustomInt(sqlid, "account_atm", "status", AccountAtm[accountid][atmAccountStatus]);
    SQL_UpdateCustom(sqlid, "account_atm", "username", AccountAtm[accountid][atmAccountName]);
    SQL_UpdateCustom(sqlid, "account_atm", "password", AccountAtm[accountid][atmAccountPassword]);
    return 1;
}

function AccountAtm_Create(playerid, name_account[], password[])
{
    new accountid = Iter_Free(AccountAtm);
    if(accountid != -1)
    {
        new fmt[230];
        format(AccountAtm[accountid][atmAccountName], 32, name_account);
        format(AccountAtm[accountid][atmAccountPassword], 32, password);
        AccountAtm[accountid][atmAccountStatus] = true;
        AccountAtm[accountid][atmAccountOwner] = PlayerData[playerid][pID];
        mysql_format(sqlcon, fmt, sizeof fmt, "INSERT INTO `account_atm` SET `owner` = %d, `username` = '%s', `password` = '%s', `status` = '%i'", PlayerData[playerid][pID], name_account, password, true);
        new Cache:result = mysql_query(sqlcon, fmt);
        {
            AccountAtm[accountid][atmAccountSQLID] = cache_insert_id();
        }
        cache_delete(result);

        Iter_Add(AccountAtm, accountid);
        PlayerData[playerid][pAccountATM] = AccountAtm[accountid][atmAccountSQLID];
    }
    return accountid;
}

Dialog:register_accountatm(playerid, response, listitem, inputtext[])
{
    if(response)
    {
        switch(listitem)
        {
            case 0: // register
            {
                new dialog[dialog_data];
                await_arr(dialog) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_INPUT, "Register Account Atm", "Enter the account name you want to register.\nSample: Acc1, Rizzy", "Register","Cancel");
                if(dialog[Response])
                {
                    new username[32], password[32];
                    if(sscanf(dialog[Inputtext], "p<|>s[32]s[32]", username, password)) return SendErrorMessage(playerid, "Create an account like this: username|password");
                    if(strval(username) > 32) return SendErrorMessage(playerid, "You can't make a name above 32 letters.");
                    new accid = AccountAtm_Create(playerid, username, password);
                    if(accid == -1)
                        SendServerMessage(playerid, "An error occurs in creating your ATM account. |Please report to Server Manager / Developer");
                    else SendServerMessage(playerid, "Making your ATM account is successful!");
                }
            }
            case 1: // login
            {
                new dialog[dialog_data];
                await_arr(dialog) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_INPUT, "Login Account", "Masukkan nama akun anda, serta password nya\nContoh: Username|Password", "Select", "Cancel");
                if(dialog[Response])
                {
                    new username[32], password[32], temp_password[32];
                    if(sscanf(dialog[Inputtext], "p<|>s[32]s[32]", username, password)) return SendErrorMessage(playerid, "Enter in the format: Username|Password");
                    new Cache:result = mysql_query(sqlcon, sprintf("SELECT * FROM `account_atm` WHERE `username` = '%s'", username));
                    {
                        if(!cache_num_rows()) return SendServerMessage(playerid, "username %s nothing in the account list.", username);

                        cache_get_field_content(0, "password", temp_password);
                        if(!strcmp(password, temp_password, true))
                        {
                            PlayerData[playerid][pAccountATM] = cache_get_field_content_int(0, "SQLID");
                            SendCustomMessage(playerid, "ATM", "You have successfully logged in to the account %s", username);
                        }
                        else SendErrorMessage(playerid, "You enter the wrong password! ");
                    }
                    cache_delete(result);
                }
            }
        }
    }
    return 1;
}

Dialog:AtmMenu(playerid, response, listitem, inputtext[])
{
    if(response)
    {
        new accid = AccountAtm_GetIDBySQLID(PlayerData[playerid][pAccountATM]);
        if(!strcmp(inputtext, "Withdraw", false))
        {
            if(!AccountAtm[accid][atmAccountStatus]) return SendServerMessage(playerid, "Your account has been turned off by the authorities.");
            if(AccountAtm[accid][atmAccountMoney] <= 0) return SendServerMessage(playerid, "You don't have money in an ATM!");
            if(AtmData[GetClosestATM(playerid, 2.5)][AtmCash] <= 0) return SendServerMessage(playerid, "ATM Tidak memiliki uang di dalamnya.");

            Dialog_Show(playerid, withdraw_atm, DIALOG_STYLE_INPUT, sprintf("Withdraw(money: " GREEN"$%s"WHITE")", FormatNumber(AccountAtm[accid][atmAccountMoney])), "Enter the amount of money you want to withdraw.", "Withdraw", "Cancel");
        }
        if(!strcmp(inputtext, "Transfer", false))
        {
            if(!AccountAtm[accid][atmAccountStatus]) return SendServerMessage(playerid, "Your account has been turned off by the authorities.");

            Dialog_Show(playerid, transfer_atm, DIALOG_STYLE_INPUT, "Transfer", "Enter the username you want to transfer.", "Transfer", "Cancel");
        }
        if(!strcmp(inputtext, "Deposit", false))
        {
            if(!AccountAtm[accid][atmAccountStatus]) return SendServerMessage(playerid, "Your account has been turned off by the authorities.");

            Dialog_Show(playerid, deposit_atm, DIALOG_STYLE_INPUT, "Deposit", "Masukkan jumlah yang ingin anda masukkan ke dalam akun anda.", "Deposit", "Close");
        }
        if(!strcmp(inputtext, "Logout", false))
        {
            PlayerData[playerid][pAccountATM] = -1;
            SendCustomMessage(playerid, "ATM", "Anda telah logout dari akun anda.");
        }
    }
    return 1;
}

Dialog:transfer_atm(playerid, response, listitem, inputtext[])
{
    if(response)
    {
        new accid, Cache:result = mysql_query(sqlcon, sprintf("SELECT * FROM `account_atm` WHERE `username` = '%s'", inputtext));
        {
            if(!cache_num_rows()) return Dialog_Show(playerid, transfer_atm, DIALOG_STYLE_INPUT, "Transfer", "Enter the username you want to transfer.\nError: account %s not listed", "Transfer", "Cancel", inputtext);
            accid = cache_get_field_content_int(0, "SQLID");
        }
        cache_delete(result);
        accid = AccountAtm_GetIDBySQLID(accid);
        new dialog[dialog_data];
        await_arr(dialog) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_INPUT, sprintf("Transfer to %s", inputtext), "Enter the amount you want to send", "Send", "Cancel");

        if(dialog[Response])
        {
            new myaccount = AccountAtm_GetIDBySQLID(PlayerData[playerid][pAccountATM]), inputmoney = strval(dialog[Inputtext]);
            if(AccountAtm[myaccount][atmAccountMoney] < inputmoney) return SendErrorMessage(playerid, "You can't enter more than the amount of money in your account.");
            
            AccountAtm[myaccount][atmAccountMoney] -= inputmoney;
            AccountAtm[accid][atmAccountMoney] += inputmoney;

            SendServerMessage(playerid, "Anda telah mengirim sebanyak $%s ke akun %s", FormatNumber(inputmoney), inputtext);
        }
    }
    return 1;
}

Dialog:withdraw_atm(playerid, response, listitem, inputtext[])
{
    if(response)
    {
        new myaccount = AccountAtm_GetIDBySQLID(PlayerData[playerid][pAccountATM]), inputmoney = strval(inputtext);
        if(AccountAtm[myaccount][atmAccountMoney] < inputmoney) return SendErrorMessage(playerid, "You can't enter more than the amount of money in your account.");
        if(AtmData[GetClosestATM(playerid, 2.5)][AtmCash] < inputmoney) return SendServerMessage(playerid, "ATM ini tidak memiliki uang yang cukup.");

        AccountAtm[myaccount][atmAccountMoney] -= inputmoney;
        AtmData[GetClosestATM(playerid, 2.5)][AtmCash] -= inputmoney;
        GiveMoney(playerid, inputmoney);

        SendServerMessage(playerid, "Anda telah mengambil "GREEN"$%s", FormatNumber(inputmoney));
    }
    return 1;
}

Dialog:deposit_atm(playerid, response, listitem, inputtext[])
{
    if(response)
    {
        new money = strval(inputtext), accountid = GetAccountAtmID(playerid);
        if(GetMoney(playerid) < money) return SendErrorMessage(playerid, "Tidak dapat memasukan melebihi batas yang dimiliki.");

        AccountAtm[accountid][atmAccountMoney] += money;
        GiveMoney(playerid, -money);
        SendServerMessage(playerid, "Anda telah mendeposit sebanyak " GREEN"$%s", FormatNumber(money));
    }
    return 1;
}