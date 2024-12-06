#define va_args<%0> {Float, _}:...
#define va_start<%0> va_func:%0

#define va_func<%0> #VARIADIC_FORMAT_%0

stock va_callfunc(func[], const specifiers[], {Float,_}:...)
{
	new
		start,
		end,
		argument,
		string[128],
		args = numargs() * 4;

	GetVariadicString(GetVariadicAddress(2), string, sizeof(string));
	if (!strfind(string, "VARIADIC_FORMAT_", false))
	{
		start = strval(string[16]);

        #emit LOAD.S.pri 0
	    #emit ADD.C 8

	    #emit LOAD.I
		#emit STOR.S.pri args

		argument = (args / 4);

		while (--argument >= start)
		{
		    #emit LOAD.S.pri argument
		    #emit SMUL.C 4

		    #emit LOAD.S.alt 0
		    #emit ADD

		    #emit ADD.C 12
		    #emit LOAD.I
		    #emit PUSH.pri
		}
		#emit LOAD.S.pri args
		#emit ADD.C 8
		#emit STOR.S.pri args

		#emit PUSH.S specifiers
	    #emit PUSH.S func

	    #emit PUSH.S args
	    #emit SYSREQ.C org_CallLocalFunction

	    #emit STACK 12

		for (argument = (args / 4) - 3; argument >= start; argument --)
		{
		    #emit STACK 4
		}
	}
	else
	{
	    #emit ADDR.PRI specifiers
	    #emit STOR.S.pri start

	    for (end = start + (args - 8); end > start; end -= 4)
	    {
	        #emit LOAD.S.pri end
	        #emit LOAD.I
	        #emit PUSH.pri
	    }
	    #emit PUSH.S specifiers
	    #emit PUSH.S func

	    #emit PUSH.S args
	    #emit SYSREQ.C org_CallLocalFunction

	    #emit STACK 12

	    for (end = start + (args - 8); end > start; end -= 4)
	    {
	        #emit STACK 4
		}
	}
	return 1;
}

stock va_format(out[], size, const fmat[], va_func:STATIC_ARGS)
{
	static
		lHeap,
		lStack,
		lArgCount,
		lArgs[5];

	#emit LOAD.S.alt   STATIC_ARGS
	#emit DEC.alt
	#emit SHL.C.alt    2

	#emit LOAD.S.pri   0
	#emit ADD.C        8
	#emit LOAD.I
	#emit SUB
	#emit ADD.C        8
	#emit STOR.pri     lArgCount
	
	#emit LOAD.S.pri   0
	#emit ADD

	#emit CONST.alt    lArgs
	#emit MOVS         16

	#emit STACK        0
	#emit STOR.alt     lStack
	#emit ADD.C        16
	#emit SCTRL        4

	#emit PUSH.S       fmat
	#emit PUSH.S       size
	#emit PUSH.S       out
	#emit PUSH         lArgCount

	#emit HEAP         0
	#emit STOR.alt     lHeap
	#emit LCTRL        4
	#emit SCTRL        2

	#emit SYSREQ.C     format
	#emit STOR.pri     lArgCount

	#emit LOAD.pri     lHeap
	#emit SCTRL        2
	#emit STACK        0
	#emit CONST.pri    lArgs
	#emit MOVS         16
	#emit LOAD.pri     lStack
	#emit SCTRL        4

	#emit LOAD.pri     lArgCount
	#emit RETN

	return 0;
}