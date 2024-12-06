#define Bit_FastSet(%0,%1,%2) ((%2)?(Bit_Let(%0,(%1))):(Bit_Vet(%0,(%1))))
#define BitArray:%1<%2> Bit:%1[%2]

#if cellbits == 32
	#define CELLSHIFT (5)
#else
	#if cellbits == 64
		#define CELLSHIFT (6)
	#else
		#if cellbits == 16
			#define CELLSHIFT (4)
		#else
			#error Unkown cell size
		#endif
	#endif
#endif

#define Bit_Bits(%1) (((%1)+cellbits-1)/cellbits)
#define Bit_Slot(%1) ((_:%1)>>>CELLSHIFT)
#define Bit_Mask(%1) (Bit:(1<<((_:%1)&cellbits-1)))
#define Bit_GetBit(%1,%2) (%1[(%2)>>>CELLSHIFT]>>>Bit:((%2)&cellbits-1)&Bit:1)
#define Bit_Get(%1,%2) bool:Bit_GetBit(%1,_:%2)
#define Bit_Let(%1,%2) (%1[(%2)>>>CELLSHIFT]|=Bit:(1<<((%2)&cellbits-1)))
#define Bit_Vet(%1,%2) (%1[(%2)>>>CELLSHIFT]&=Bit:~(1<<((%2)&cellbits-1)))

/* 
    bit function
*/
stock Bit_Set(BitArray:array<>, slot, bool:set)//, size = sizeof (array))
{
	//if (slot >>> CELLSHIFT >= size) return;
	if (set) Bit_Let(array, slot);
	else Bit_Vet(array, slot);
}

stock Bit_SetAll(BitArray:array<>, bool:set, size = sizeof (array))
{
	memset(_:array, set ? 0xFFFFFFFF : 0, size);
}

stock Bit_Display(const BitArray:array<>, size = sizeof (array))
{
	new
		ret[130],
		val = 0;
	while (size--)
	{
		val = _:array[size];
		format(ret, sizeof (ret), "%s%016b%016b", ret, val >>> 16, val & 0xFFFF);
	}
	return ret;
}

#define bitsof(%0) (sizeof(%0)*cellbits)

#define bits<%1> Bit_Bits(%1) 

#undef BitArray
#define BitArray:%1<%2> Bit:%1[bits<%2>]

