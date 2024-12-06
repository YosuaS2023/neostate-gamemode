#include <UtilsScripts\als.inc>

forward ShowPlayerSalary(playerid);
forward Salary_GetTime(playerid);

AddPlayerSalary(playerid, wages) 
{
    if(!IsPlayerConnected(playerid)) 
    {
        return 0;
    }
    
    if(!PlayerData[playerid][pSalaryClaim])
    {
        PlayerData[playerid][pSalaryClaim] = gettime() + (60 * 1);
    }

    if(wages < 0) 
    {
        wages = 0;
    }

    new query[255];
    mysql_format(sqlcon, query,sizeof(query),"INSERT INTO `salary` SET `LinkID`='%d', `Money`='%d', `Date`=UNIX_TIMESTAMP()", PlayerData[playerid][pID], wages);
    mysql_tquery(sqlcon, query, "OnSalaryAdded", "d", playerid);
    return 1;
}

GetSalaryMoney(playerid, &money)
{
    new
        Cache:salary_cache,
        query[128];

    mysql_format(sqlcon, query, sizeof(query), "SELECT IFNULL(SUM(Money), 0) As `Total` FROM `salary` WHERE `LinkID`='%d'", GetPlayerSQLID(playerid));
    salary_cache = mysql_query(sqlcon, query);

    if(cache_num_rows()) {
        money = cache_get_field_int(0, "Total");
    }
    cache_delete(salary_cache);
    return 1;
}

ShowPlayerSalary(playerid) 
{
    new string[3000], totalwage, wage_after_tax;
    GetSalaryMoney(playerid, totalwage);
    wage_after_tax = totalwage - Economy_GetSalesTax(totalwage);

    strcat(string, "===============[Salary]===============");
    strcat(string, sprintf("\nSalary : "GREEN"$%s"WHITE"\n", FormatNumber(totalwage)));
    strcat(string, sprintf("Tax : "RED"-$%s(%i Percent)\n"WHITE, FormatNumber(Economy_GetSalesTax(totalwage)), EconomyData[TAX_RATE]));
    strcat(string, sprintf("Salary after tax : "GREEN"$%s"WHITE"\n", FormatNumber(wage_after_tax)));
    strcat(string, "===============[Salary]===============\n");

    if(totalwage > 0) Dialog_Show(playerid, unused, DIALOG_STYLE_MSGBOX, "My salary", string, "Close", "");
    else SendErrorMessage(playerid, "You don't have a salary.");
    return 1;
}

ClearPlayerWages(playerid) {
    mysql_tquery(sqlcon, sprintf("DELETE FROM `salary` WHERE `LinkID`='%d';", GetPlayerSQLID(playerid))); 
    return 1;
}

/*
    Callback
*/
function OnSalaryAdded(playerid) {
    return SendCustomMessage(playerid, "SALARY", "Your salary has been added, please check at your nearest ATM.");

}

/*
    Commands
*/
CMD:salary(playerid, params[])
{
    ShowPlayerSalary(playerid);
    return 1;
}

CMD:addsalary(playerid, params[])
{  
    if(!IsPlayerADuty(playerid, OPERATIONS_LEAD))
        return PermissionError(playerid);

    static
        userid,
        salary;

    if(sscanf(params, "ud", userid, salary))
        return SendSyntaxMessage(playerid, "/addsalary [playerid] [salary]");

    if(!IsPlayerConnected(userid))
        return SendErrorMessage(playerid, "That player isn't connected!");

    if(salary < 1 || salary > 100_000)
        return SendErrorMessage(playerid, "Refund salary can't be more than 100.000 or below than 1!");

    
    AddPlayerSalary(userid, salary);
    SendCustomMessage(playerid, "SALARY", "You have sent a salary with nominal "GREEN"$%d"WHITE" to "YELLOW"%s", salary, ReturnName(userid));
    //WriteLog("salary_log.txt", sprintf("%s has added salary [$%d] to: %s.", ReturnName(playerid), salary, ReturnName(userid)));
    return 1;
}