#tryinclude <PawnPlus>

forward CallFunctionFromName(keys[], specifiers[], {Float, _}:...);

public CallFunctionFromName(keys[], specifiers[], {Float, _}:...)
{
	new totalPublics = amx_num_publics();
	for (new i = 0; i < totalPublics; i++) 
	{
		new publicName[MAX_FUNCTION_NAME];
		amx_public_name(i, publicName);

		if (strfind(publicName, keys) != 0){
			continue;
		}
		printf("*** [UtilsScripts] (Function) call function from name \"%s\"  with name \"%s\"" , keys, publicName);
		CallLocalFunction(publicName, specifiers, va_func<2>);
	}
	return 1;
}

stock memset(arr[], val = 0, size = sizeof (arr))
{
	new
		addr = 0;
	#emit LOAD.S.pri arr
	#emit STOR.S.pri addr
	return rawMemset(addr, val, size * 4);
}

stock rawMemset(iAddress /* 12 */, iValue /* 16 */, iSize /* 20 */)
{
	if (iSize < 4)
		return 0;
	if (iAddress < 0)
	{

		do
		{
			#emit LOAD.S.pri iValue
			#emit SREF.S.pri iAddress
			iAddress += 4;
		}
		while ((iSize -= 4) > 0);
		return 1;
	}
	else //if (Server_JITComplete())
	{
		while (iSize >= 4096)
		{
			#emit LOAD.S.pri iValue
			#emit LOAD.S.alt iAddress
			#emit FILL       4096
			iAddress += 4096;
			iSize -= 4096;
		}
		if (iSize & 2048)
		{
			#emit LOAD.S.pri iValue
			#emit LOAD.S.alt iAddress
			#emit FILL       2048
			iAddress += 2048;
		}
		if (iSize & 1024)
		{
			#emit LOAD.S.pri iValue
			#emit LOAD.S.alt iAddress
			#emit FILL       1024
			iAddress += 1024;
		}
		if (iSize & 512)
		{
			#emit LOAD.S.pri iValue
			#emit LOAD.S.alt iAddress
			#emit FILL       512
			iAddress += 512;
		}
		if (iSize & 256)
		{
			#emit LOAD.S.pri iValue
			#emit LOAD.S.alt iAddress
			#emit FILL       256
			iAddress += 256;
		}
		if (iSize & 128)
		{
			#emit LOAD.S.pri iValue
			#emit LOAD.S.alt iAddress
			#emit FILL       128
			iAddress += 128;
		}
		if (iSize & 64)
		{
			#emit LOAD.S.pri iValue
			#emit LOAD.S.alt iAddress
			#emit FILL       64
			iAddress += 64;
		}
		if (iSize & 32)
		{
			#emit LOAD.S.pri iValue
			#emit LOAD.S.alt iAddress
			#emit FILL       32
			iAddress += 32;
		}
		if (iSize & 16)
		{
			#emit LOAD.S.pri iValue
			#emit LOAD.S.alt iAddress
			#emit FILL       16
			iAddress += 16;
		}
		if (iSize & 8)
		{
			#emit LOAD.S.pri iValue
			#emit LOAD.S.alt iAddress
			#emit FILL       8
			iAddress += 8;
		}
		if (iSize & 4)
		{
			#emit LOAD.S.pri iValue
			#emit SREF.S.pri iAddress
		}
		return 1;
	}
}