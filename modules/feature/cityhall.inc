CMD:licensemenu(playerid, params[])
{
    static actorid;
    if((actorid = Actor_Nearest(playerid, 5.0)) != INVALID_ITER_ID)
    {
        if(Actor_Type(actorid, "license-cityhall"))
        {
            Dialog_CityHall(playerid);
        }
    }
    else
    {
        SendErrorMessage(playerid, "You are not in the place of making a license.");
    }
    return 1;
}

function Dialog_CityHall(playerid)
{
    yield 1;
    static dialog[dialog_data];
    await_arr(dialog)  ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_LIST, "License Menu - City Hall", "Create identity card\nCreate license house\nCreate license business", "Select", "Cancel");

    if(dialog[Response])
    {
        switch(HashStr(dialog[Inputtext]))
        {
            case HS<Create identity card>:
            {
                if(GetMoney(playerid) < @economy get<create_identity_card>)
                    return SendErrorMessage(playerid, "You need %s to create an ID card.", FormatNumber(@economy get<create_identity_card> - GetMoney(playerid)));

                if(PlayerData[playerid][pIDCardTime] != 0)
                    return SendErrorMessage(playerid, "You already have an ID Card!");

                static dialog_iden[dialog_data];
                await_arr(dialog_iden) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_MSGBOX, "Identitas Menu", "You want to create an identity?\nPlease click 'Yes' if you want to create and 'No' if you want to cancel", "Yes","No");
                if(dialog_iden[Response])
                {
                    static dialog_origin[dialog_data];
                    static origin_player[24];
                    await_arr(dialog_origin) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_INPUT, "Input Origin", "Please enter your region data.", "Inpu", "Cancel");

                    if(dialog_origin[Response])
                    {
                        format(origin_player, sizeof(origin_player), "%s", dialog_origin[Inputtext]);
                        static dialog_finish[dialog_data];
                        static desc[230], year, month, day;
                        sscanf(PlayerData[playerid][pAge], "p</>iii", day, month, year);
                        format(desc, sizeof(desc), "Your ID Card data\nName: %s\nDate of birth: %s\nAge: %i\nOrigin: %s\n Would you like to create an ID Card with this data?", PlayerData[playerid][pName], PlayerData[playerid][pAge], CalculateAge(year, month, day), origin_player);
                        await_arr(dialog_finish) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_MSGBOX, "Confirm", desc, "Ya", "Tidak");
                        
                        if(dialog_finish[Response])
                        {
                            format(PlayerData[playerid][pOrigin], 24, origin_player);
                            PlayerData[playerid][pIDCardTime] = (gettime() + 360);
                            GiveMoney(playerid, -@economy get<create_identity_card>);
                            underline(playerid);
                            SendServerMessage(playerid, "Origin : %s", PlayerData[playerid][pOrigin]);
                            SendServerMessage(playerid, "Date of birth : %s", PlayerData[playerid][pAge]);
                            SendServerMessage(playerid, "Age : %i", CalculateAge(year, month, day));
                            SendServerMessage(playerid, "Cost "RED"-%s", FormatNumber(@economy get<create_identity_card>));
                            SendServerMessage(playerid, "The time period has expired "RED"%s", ReturnTimelapse(gettime(), PlayerData[playerid][pIDCardTime]));
                            underline(playerid);
                            SendClientMessage(playerid, -1, "");

                        }
                        else SendServerMessage(playerid, "You have canceled ID Card creation");
                    }
                    else
                    {
                        SendServerMessage(playerid, "You have canceled ID Card creation");
                    }
                }
            }
            case HS<Create license business>:
            {
                if(GetMoney(playerid) < @economy get<create_lic_biz>)
                    return SendErrorMessage(playerid, "Anda membutuhkaan %s untuk membuat lisensi bisnis.", FormatNumber(@economy get<create_lic_biz> - GetMoney(playerid)));

                task_yield(1);
                static dialog_bizlic[dialog_data];
                await_arr(dialog_bizlic) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_MSGBOX, "Create Business License", "You want to create an business license?\nRequirements: \n\t- You must create an identity card first\n\t- You must be at level 7 and above\n"WHITE"\nPlease click 'Yes' if you want to create and 'No' if you want to cancel", "Yes","No");
                if(dialog_bizlic[Response])
                {
                    if(PlayerData[playerid][pIDCardTime] == 0)
                        return SendErrorMessage(playerid, "You must complete the approval to create a business license!");
                    
                    if(PlayerData[playerid][pLevel] <= 2)
                        return SendErrorMessage(playerid,"Level must be 3 or above.");
                    
                    PlayerData[playerid][pBizLicense] = (gettime() + EXPIRED_BUSINESS_LICENSE);
                    GiveMoney(playerid, -@economy get<create_lic_biz>);
                    Dialog_Show(playerid, unused, DIALOG_STYLE_MSGBOX, "Success Created License Business", sprintf("You successfully created a business licence\nExpired: %s\nPrice: "RED"%s "WHITE"\nNote: "GREEN"Now you can choose any business", ReturnTimelapse(gettime(), PlayerData[playerid][pBizLicense]), FormatNumber(@economy get<p_bizlic>)), "Close", "");
                }
                else Dialog_CityHall(playerid);
            }
            case HS<Create license house>:
            {
                yield 1;
                static dialog_bizlic[dialog_data];
                await_arr(dialog_bizlic) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_MSGBOX, "Create House License", "You want to create an house license?\nRequirements: \n\t- You must create an identity card first\n\t- You must be at level 7 and above\n"WHITE"\nPlease click 'Yes' if you want to create and 'No' if you want to cancel", "Yes","No");
                if(dialog_bizlic[Response])
                {
                    if(PlayerData[playerid][pHouseLicense] != 0)
                        return SendServerMessage(playerid, "You already have a license house.");

                    if(PlayerData[playerid][pIDCardTime] == 0)
                        return SendErrorMessage(playerid, "You must complete the approval to create a house license!");
                    
                    static fmt[200];
                    formatex(fmt, FormatColor(sprintf("Need ~g~%s~w~ to create a house license.", FormatNumber((@economy get<create_license_house> - GetMoney(playerid))))));

                    if(GetMoney(playerid) < @economy get<create_license_house>)
                        return SendErrorMessage(playerid, "Need %s to create a house license.", FormatNumber((@economy get<create_license_house> - GetMoney(playerid))));

                    if(PlayerData[playerid][pLevel] < 3)
                        return SendErrorMessage(playerid,"Level must be 3 or above.");
                    
                    PlayerData[playerid][pHouseLicense] = (gettime() + EXPIRED_HOUSE_LICENSE);
                    GiveMoney(playerid, -@economy get<create_license_house>);
                    Dialog_Show(playerid, unused, DIALOG_STYLE_MSGBOX, "Success Created License House", sprintf("You successfully created a house licence\nExpired: %s\nPrice: "RED"%s "WHITE"\nNote: "GREEN"Now you can choose any house", ReturnTimelapse(gettime(), PlayerData[playerid][pHouseLicense]), FormatNumber(@economy get<create_license_house>)), "Close", "");
                }
                else Dialog_CityHall(playerid);
            }
        }
    }
    return 1;
}