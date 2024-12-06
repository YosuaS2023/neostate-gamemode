/* Enums */

enum
{
    GROUP_JOB_NONE = 0,
    GROUP_JOB_CLEANING
}

enum _:ADMIN_RANK
{
	UNKOWN_ADMIN = 0,
	STAFF,
	SUPERVISOR,
	OPERATIONS_LEAD,
	OWNER,
	DEVELOPER
}

enum _:E_SIDEJOB
{
	SIDEJOB_NONE = 0,
	SIDEJOB_SWEEPER,
	SIDEJOB_MOWER
}

enum E_ECONOMY_ACTION
{
	ECONOMY_ACTION_NONE = 0,
	ECONOMY_ADD_SUPPLY,
	ECONOMY_TAKE_SUPPLY
}

enum _:EDIT_OBJECT_MODE
{
	NOT_EDIT_OBJECT = 0,
	EDIT_OBJECT_HOUSE_STRUCTURE,
	EDIT_OBJECT_PUMP,
	EDIT_OBJECT_ATM,
	EDIT_OBJECT_TRASH
}

enum
{
	VEHICLE_TYPE_SWEEPER = 1,
}

enum e_faction
{
	FACTION_LSPD,
	FACTION_LSES,
	FACTION_LSN,
	FACTION_LSG
};

enum inventoryData
{
	invExists,
	invID,
	invItem[32 char],
	invModel,
	invQuantity
};

new InventoryData[MAX_PLAYERS][MAX_INVENTORY][inventoryData];

	
enum e_InventoryItems
{
	e_InventoryItem[32],
	e_InventoryModel
};


new const g_aInventoryItems[][e_InventoryItems] =
{
	{"Component", 2912},
	{"GPS", 18875},
	{"Cellphone", 18867},
	{"Medkit", 1580},
	{"Portable Radio", 19942},
	{"Mask", 19036},
	{"Snack", 2768},
	{"Water", 2958}
};

/* vehicle */
enum vCore
{
	vehFuel, // bensin kendaraan
	vehPlanting, // mode tanam kendaraan untuk id 531
	vehType[50], // tipe kendaraan
	vehTrash // simpan trash dalam kendaraan
};

enum vData
{
	vID,
	vOwner,
	vColor[2],
	vModel,
	vLocked,
	vInsurance,
	vInsuTime,
	vPlate[16],
	Float:vHealth,
	Float:vPos[4],
	vWorld,
	vInterior,
	vFuel,
	vVehicle,
	vDamage[4],
	bool:vExists,
	vRental,
	vRentTime,
};
new VehicleData[MAX_PLAYER_VEHICLE][vData];
new Iterator:OwnableVehicles[MAX_PLAYERS]<MAX_PLAYER_VEHICLE>;
new VehCore[MAX_VEHICLES][vCore];
	

enum droppedItems
{
	droppedID,
	droppedItem[32],
	droppedPlayer[24],
	droppedModel,
	droppedQuantity,
	Float:droppedPos[3],
	droppedWeapon,
	droppedAmmo,
	droppedInt,
	droppedWorld,
	droppedObject,
	Text3D:droppedText3D
};

new DroppedItems[MAX_DROPPED_ITEMS][droppedItems];

enum
{
	INVALID_DIALOG_ID,
	DIALOG_REGISTER,
	DIALOG_LOGIN,
	DIALOG_PIN,
	DIALOG_MAKECHAR,
	DIALOG_ORIGIN,
	DIALOG_GENDER,
	DIALOG_CHARLIST,
	DIALOG_NONE,
	DIALOG_BIZBUY,
	DIALOG_INVENTORY,
	DIALOG_GIVEITEM,
	DIALOG_DROPITEM,
	DIALOG_USEITEM,
	DIALOG_GIVEAMOUNT,
	DIALOG_INVACTION,
	DIALOG_BUYSKINS,
	DIALOG_INSURANCE,
	DIALOG_BUYINSURANCE,
	DIALOG_RENTAL,
	DIALOG_RENTTIME,
	DIALOG_BIZMENU,
	DIALOG_BIZNAME,
	DIALOG_BIZPROD,
	DIALOG_BIZPRODSET,
	DIALOG_BIZPRICE,
	DIALOG_BIZPRICESET,
	DIALOG_BIZCARGO
};

/*
	Enumerated player
*/

enum e_player_data
{
	pID,
	pUCP[22],
	pName[MAX_PLAYER_NAME],
	Float:pPos[3],
	pOrigin[50],
	pAge[50],
	pWorld,
	pInterior,
	pSkin,
	pAttempt,
	pGender,
	bool:pMaskOn,
	pMaskID,
	bool:pSpawned,
	pChar,
	pLevel,
	Float:pHealth,
	pEnergy,
	pMoney,
	pBank,
	pInHouse,
	pInBiz,
	pListitem,
	pStorageSelect,
	pAdmin,
	pAduty,
	pPhoneNumber,
	pCredit,
	pSkinPrice,
	pVehKey,
	pRenting,
	/*
		gettime + time
	*/
	pIDCardTime,
	pBizLicense,
	pHouseLicense,
	pSalaryClaim,

	pAccountATM,

	/*
		Editing Mode
	*/
	pEditingMode,
	pEditAtmID,
	pEditStructure,
	pEditHouseStructure,
	pEditPump,
	pEditTrash,
	/*
		Not saved
	*/
	pLastVehicle, // menampng id vehicle terakhir dinaiki
	pCalling, // menampung id player yg  kita telpon
	pTarget, // menampung id player yang di targetkan
	pFreeze, // status freeze
	pFreezeTimer, // status freeze timer id
	pOnline, // online timer
	pGasPump, // gas id
	pGasStation, // bisnis gas id
	pRefill, // kendaraan yang di isi ulang
	pRefillPrice, // harga isi bensin
	pGPS, // gps aktif
	pGroupJob,
	/*
		sidejob
	*/
	pSidejob, // core 
	pSidejobTime,
	PlayerText:pSidejobText,
	MowerVehicle,
	SweeperVehicle,
	/*
		sweeper
	*/
	SweeperUpdate,
	SweeperDistance,
	Float:SweeperLastPos[3],
	/*
		mower
	*/
	MowerUpdate,
	MowerGrass,
};

enum E_GROUP_DATA
{
	GROUP_VEHICLEID,
}

new PlayerData[MAX_PLAYERS][e_player_data];
new GroupData[MAX_GROUPS][E_GROUP_DATA];

/*
	biz data
*/
enum e_biz_data
{
	bizID,
	bizName[32],
	bizOwner,
	bizOwnerName[MAX_PLAYER_NAME],
	bool:bizExists,
	Float:bizInt[3],
	Float:bizExt[3],
	Float:bizPoint[3], // point beli
	bizWorld,
	bizInterior,
	bizVault,
	bizPrice,
	bizLocked,
	bizFuel,
	bizProduct[7],
	bizType,
	bizStock,
	STREAMER_TAG_PICKUP:bizFuelPickup,
	STREAMER_TAG_3D_TEXT:bizFuelText,
	STREAMER_TAG_PICKUP:bizDeliverPickup,
	STREAMER_TAG_3D_TEXT:bizDeliverText,
	STREAMER_TAG_PICKUP:bizPickup,
	STREAMER_TAG_3D_TEXT_LABEL:bizText,
	STREAMER_TAG_3D_TEXT_LABEL:bizTextPoint,
	STREAMER_TAG_CP:bizCP,
};

new BizData[MAX_BUSINESS][e_biz_data];
new ProductName[MAX_BUSINESS][7][24];

enum e_rental
{
	rentID,
	bool:rentExists,
	Float:rentPos[3],
	Float:rentSpawn[4],
	rentModel[2],
	rentPrice[2],
	STREAMER_TAG_3D_TEXT_LABEL:rentText,
	STREAMER_TAG_PICKUP:rentPickup,
};

new RentData[MAX_RENTAL][e_rental];

enum E_TRASH
{
    TRASH_SQLID,
    TRASH_TYPE[30],
    TRASH_OBJECTID,
    TRASH_COOLDOWN,
    Float:TRASH_POS[3],
    Float:TRASH_ROT[3],
    STREAMER_TAG_OBJECT:TRASH_OBJECT,
    STREAMER_TAG_3D_TEXT_LABEL:TRASH_3DTEXT,
	bool:TRASH_CLEAN
}

new TrashData[MAX_TRASH][E_TRASH];
new Iterator:Trash<MAX_TRASH>;

enum cdInfo
{
	cdID,
    cdOwned,
	cdOwner[MAX_PLAYER_NAME],
	Float:cdEntranceX,
	Float:cdEntranceY,
	Float:cdEntranceZ,
	Float:cdExitX,
	Float:cdExitY,
	Float:cdExitZ,
	cdMessage[128],
	cdTill,
	cdInterior,
	Float: cdRadius,
	cdPrice,
	cdType,
	cdPickupID,
	Text3D:cdTextLabel,

    /* Vehicle Display */
	Text3D:cdVehicleLabel[MAX_DEALERSHIPVEHICLES],
	cdVehicleDBID[MAX_DEALERSHIPVEHICLES],
	cdVehicleModel[MAX_DEALERSHIPVEHICLES],
	cdVehicleCost[MAX_DEALERSHIPVEHICLES],
	cdVehicleId[MAX_DEALERSHIPVEHICLES],
	Float: cdVehicleSpawnX[MAX_DEALERSHIPVEHICLES],
	Float: cdVehicleSpawnY[MAX_DEALERSHIPVEHICLES],
	Float: cdVehicleSpawnZ[MAX_DEALERSHIPVEHICLES],
	Float: cdVehicleSpawnAngle[MAX_DEALERSHIPVEHICLES],
	Float: cdVehicleSpawn[4]
};
new CarDealershipInfo[MAX_CARDEALERSHIPS][cdInfo],
	Iterator: Dealership<MAX_CARDEALERSHIPS>;

enum pumpData {
	pumpID,
	pumpExists,
	pumpBusiness,
	Float:pumpPos[4],
	pumpFuel,
	pumpObject,
	Text3D:pumpText3D
};
new PumpData[MAX_GAS_PUMPS][pumpData];
new Iterator:Pump<MAX_GAS_PUMPS>;

enum E_ATM
{
	AtmSQLID,
	Float:AtmPos[8],
	AtmCash,
    STREAMER_TAG_OBJECT:AtmObject,
	STREAMER_TAG_3D_TEXT_LABEL:AtmLabel
};

enum e_account_atm
{
	atmAccountSQLID,
	atmAccountOwner, // player sql-id
	atmAccountName[32],
	atmAccountPassword[32],
	atmAccountMoney,
	atmAccountStatus
};
new AtmData[MAX_ATMS][E_ATM];
new AccountAtm[MAX_ACCOUNT_ATM][e_account_atm];
new Iterator:DynamicAtm<MAX_ATMS>;
new Iterator:AccountAtm<MAX_ACCOUNT_ATM>;

enum
{
	THREAD_INSERT_ECONOMY
}

enum e_economy
{
	NONE_ECONOMY,
	SUPPLY,
	RESERVE,
	Float:INFLATION_RATE,
	TAX_RATE,
	MATERIAL_PRICE
};

enum e_economy_data
{
	economySQLID,
	economyName[32],
	economyPrice
};

enum temp_economyData
{
	e_economy:economyVar,
	economyName[30]
};

new EconomyData[e_economy];
new EconomyServer[MAX_ECONOMY][e_economy_data];
new PlayerEconomyList[MAX_PLAYERS][20];
new Iterator:Economy<MAX_ECONOMY>;

enum
{
	PLANT_WHEAT = 1
};

enum e_plant
{
	plantID,
	plantModel,
	Float:plantPos[4],
	plantType,
	Float:plantGrowthMillisecond,
	STREAMER_TAG_OBJECT: plantObject
};
new PlantData[MAX_PLANT][e_plant];
new Iterator:PlantIds<MAX_PLANT>;

#include "../modules/property/house/core.inc"