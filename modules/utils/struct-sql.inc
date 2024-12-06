enum STRUCT_CHARACTER
{
    structName[38],
	structDisplay[38],
    e_player_data:structChar,
	structDefault,
	SQL_TYPE:structType
};

enum STRUCT_ACCOUNTATM
{
	structAtmName[38],
	e_account_atm:structVar
};

new const struct_character[][STRUCT_CHARACTER] =
{
	{"AccountATM", "Account Atm ID", pAccountATM, -1, SQL_INT},
	{"InHouse", "Enter House ID", pInHouse, -1, SQL_INT},
	{"InBiz", "Enter Business ID", pInBiz, -1, SQL_INT},
	// not saved
	{"no_sql", "Status Spanwed", pSpawned, false, SQL_INVALID},
	{"no_sql", "Vehicle Sweeper", SweeperVehicle, INVALID_VEHICLE_ID, SQL_INVALID},
	{"no_sql", "Vehicle Mower", MowerVehicle, INVALID_VEHICLE_ID, SQL_INVALID},
	{"no_sql", "Sidejob ID", pSidejob, SIDEJOB_NONE, SQL_INVALID},
	{"no_sql", "Sidejob Time Exit Vehicle", pSidejobTime, -1, SQL_INVALID},
	{"no_sql", "Sweeper Distance", SweeperDistance, 0, SQL_INVALID},
	{"no_sql", "Sweeper Update", SweeperUpdate, 0, SQL_INVALID},
	{"no_sql", "Editing Mode", pEditingMode, NOT_EDIT_OBJECT, SQL_INVALID},
	{"no_sql", "Editing Atm ID", pEditAtmID, -1, SQL_INVALID},
	{"no_sql", "Editing Pump ID", pEditPump, -1, SQL_INVALID},
	{"no_sql", "Editing House Structure ID", pEditHouseStructure, -1, SQL_INVALID},
	{"no_sql", "Editing Structure House ID", pEditStructure, -1, SQL_INVALID},
	{"no_sql", "Online Time Per Second", pOnline, 0, SQL_INVALID},
	{"no_sql", "Attemp Login", pAttempt, 0, SQL_INVALID},
	{"no_sql", "Status freeze", pFreeze, false, SQL_INVALID},
	{"no_sql", "Freeze Timer ID", pFreezeTimer, -1, SQL_INVALID},
	{"no_sql", "Enter Gas Pump ID", pGasPump, -1, SQL_INVALID},
	{"no_sql", "Gas Station from Biz ID", pGasStation, -1, SQL_INVALID},
	{"no_sql", "Refill Fuel VehicleID", pRefill, INVALID_VEHICLE_ID, SQL_INVALID},
	{"no_sql", "Refill Price", pRefillPrice, 0, SQL_INVALID}
};

new const struct_accountatm[][STRUCT_ACCOUNTATM] =
{
	{"money", atmAccountMoney},
	{"status", atmAccountStatus}
};


new arr_economyData[][temp_economyData] =
{
	{SUPPLY, "Supply"},
	{RESERVE, "Reserve"},
	{TAX_RATE, "Tax rate"},
	{INFLATION_RATE, "Infilation rate"}
};