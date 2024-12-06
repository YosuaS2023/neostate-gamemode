#define II_RANGE(%0,%1,%2) (((%0)-((%1)+cellmin))<((%2)-((%1)+cellmin)))

enum e_HASH_TYPE:
{
	hash_bernstein
}

stock HashStr(const str[], bool:sensitive = true, e_HASH_TYPE:type = hash_bernstein, len = cellmax, pack = false)
{
	if (str[str[0] == '\1'] == '\0')
	{
		// NULL.
		switch (type)
		{
		case hash_bernstein:
			return 5381;
		}
	}
	len = min(len, strlen(str));
	new
		i = 0;
	if (str[0] > 255)
	{
		switch (type)
		{
			case hash_bernstein:
			{
				new
					hash = 5381;
				if (sensitive)
				{
					while (i != len)
					{
						hash = hash * 33 ^ str{i++};
					}
				}
				else
				{
					while (i != len)
					{
						pack = str{i++},
						hash = hash * 33 ^ (II_RANGE(pack, 'A', 'Z' + 1) ? pack | 0x20 : pack);
					}
				}
				return hash;
			}
		}
	}
	else
	{
		switch (type)
		{
			case hash_bernstein:
			{
				new
					hash = 5381;
				if (sensitive)
				{
					while (i != len)
					{
						hash = hash * 33 ^ str[i++];
					}
				}
				else
				{
					while (i != len)
					{
						pack = str[i++],
						hash = hash * 33 ^ (II_RANGE(pack, 'A', 'Z' + 1) ? pack | 0x20 : pack);
					}
				}
				return hash;
			}
		}
	}
	return -1;
}

#define HS (_:_H_1:_H_0
#define _I (_:_H_3:_H_2

#define _H_1:_H_0<%0> HASh@bernstein(%0))
#define _H_3:_H_2<%0> HAShi@bernstein(%0))


#define HASH  (_:_H_4:_H_5
#define HASHi (_:_H_6:_H_7

#define _H_4:_H_5:%0<%1> HASh@%0(%1))
#define _H_6:_H_7:%0<%1> HAShi@%0(%1))


#define HASh@bernstein(%9) H_H(0,1)(%9)
#define HAShi@bernstein(%9) H_I(0,1)(%9)


#define HASH_:%0(,@,@)%1) %1)

#define H_H(%0,%1)(%9) @B%0:@A0:@A1:@A2:@A3:@A4:@A5:@A6:@A7:@A8:@A9:@Aa:@Ab:@Ac:@Ad:@Ae:@Af:@Ag:@Ah:@Ai:@Aj:@Ak:@Al:@Am:@An:@Ao:@Ap:@Aq:@Ar:@As:@At:@Au:@Av:@Aw:@Ax:@Ay:@Az:@AA:@AB:@AC:@AD:@AE:@AF:@AG:@AH:@AI:@AJ:@AK:@AL:@AM:@AN:@AO:@AP:@AQ:@AR:@AS:@AT:@AU:@AV:@AW:@AX:@AY:@AZ:@A_:@A@:@B_:@B%1$(%9)$
#define H_I(%0,%1)(%9) @B%0:@A0:@lo A1:@A2:@A3:@A4:@A5:@A6:@A7:@A8:@A9:@Aa:@Ab:@Ac:@Ad:@Ae:@Af:@Ag:@Ah:@Ai:@Aj:@Ak:@Al:@Am:@An:@Ao:@Ap:@Aq:@Ar:@As:@At:@Au:@Av:@Aw:@Ax:@Ay:@Az:@Ba:@Bb:@Bc:@Bd:@Be:@Bf:@Bg:@Bh:@Bi:@Bj:@Bk:@Bl:@Bm:@Bn:@Bo:@Bp:@Bq:@Br:@Bs:@Bt:@Bu:@Bv:@Bw:@Bx:@By:@Bz:@A_:@A@:@B_:@B%1$(%9)$

#define @B0:%9$()$ 5381
#define @B1$(<%0>%1) @B2:%0@B3:<>%1
#define @B2:%9<%0>%1) %9(@B2:%0)*33^%1)
#define @B3:(%0)*%1$ 5381*%1



#define @A0:%9$(%10%0)$ @A0:%9$(<%1>'0'%0)$
#define @A1:%9$(%11%0)$ @A1:%9$(<%1>'1'%0)$
#define @A2:%9$(%12%0)$ @A2:%9$(<%1>'2'%0)$
#define @A3:%9$(%13%0)$ @A3:%9$(<%1>'3'%0)$
#define @A4:%9$(%14%0)$ @A4:%9$(<%1>'4'%0)$
#define @A5:%9$(%15%0)$ @A5:%9$(<%1>'5'%0)$
#define @A6:%9$(%16%0)$ @A6:%9$(<%1>'6'%0)$
#define @A7:%9$(%17%0)$ @A7:%9$(<%1>'7'%0)$
#define @A8:%9$(%18%0)$ @A8:%9$(<%1>'8'%0)$
#define @A9:%9$(%19%0)$ @A9:%9$(<%1>'9'%0)$

#define @Aa:%9$(%1a%0)$ @Aa:%9$(<%1>97%0)$
#define @Ab:%9$(%1b%0)$ @Ab:%9$(<%1>98%0)$
#define @Ac:%9$(%1c%0)$ @Ac:%9$(<%1>99%0)$
#define @Ad:%9$(%1d%0)$ @Ad:%9$(<%1>100%0)$
#define @Ae:%9$(%1e%0)$ @Ae:%9$(<%1>101%0)$
#define @Af:%9$(%1f%0)$ @Af:%9$(<%1>102%0)$
#define @Ag:%9$(%1g%0)$ @Ag:%9$(<%1>103%0)$
#define @Ah:%9$(%1h%0)$ @Ah:%9$(<%1>104%0)$
#define @Ai:%9$(%1i%0)$ @Ai:%9$(<%1>105%0)$
#define @Aj:%9$(%1j%0)$ @Aj:%9$(<%1>106%0)$
#define @Ak:%9$(%1k%0)$ @Ak:%9$(<%1>107%0)$
#define @Al:%9$(%1l%0)$ @Al:%9$(<%1>108%0)$
#define @Am:%9$(%1m%0)$ @Am:%9$(<%1>109%0)$
#define @An:%9$(%1n%0)$ @An:%9$(<%1>110%0)$
#define @Ao:%9$(%1o%0)$ @Ao:%9$(<%1>111%0)$
#define @Ap:%9$(%1p%0)$ @Ap:%9$(<%1>112%0)$
#define @Aq:%9$(%1q%0)$ @Aq:%9$(<%1>113%0)$
#define @Ar:%9$(%1r%0)$ @Ar:%9$(<%1>114%0)$
#define @As:%9$(%1s%0)$ @As:%9$(<%1>115%0)$
#define @At:%9$(%1t%0)$ @At:%9$(<%1>116%0)$
#define @Au:%9$(%1u%0)$ @Au:%9$(<%1>117%0)$
#define @Av:%9$(%1v%0)$ @Av:%9$(<%1>118%0)$
#define @Aw:%9$(%1w%0)$ @Aw:%9$(<%1>119%0)$
#define @Ax:%9$(%1x%0)$ @Ax:%9$(<%1>120%0)$
#define @Ay:%9$(%1y%0)$ @Ay:%9$(<%1>121%0)$
#define @Az:%9$(%1z%0)$ @Az:%9$(<%1>122%0)$

#define @AA:%9$(%1A%0)$ @AA:%9$(<%1>65%0)$
#define @AB:%9$(%1B%0)$ @AB:%9$(<%1>66%0)$
#define @AC:%9$(%1C%0)$ @AC:%9$(<%1>67%0)$
#define @AD:%9$(%1D%0)$ @AD:%9$(<%1>68%0)$
#define @AE:%9$(%1E%0)$ @AE:%9$(<%1>69%0)$
#define @AF:%9$(%1F%0)$ @AF:%9$(<%1>70%0)$
#define @AG:%9$(%1G%0)$ @AG:%9$(<%1>71%0)$
#define @AH:%9$(%1H%0)$ @AH:%9$(<%1>72%0)$
#define @AI:%9$(%1I%0)$ @AI:%9$(<%1>73%0)$
#define @AJ:%9$(%1J%0)$ @AJ:%9$(<%1>74%0)$
#define @AK:%9$(%1K%0)$ @AK:%9$(<%1>75%0)$
#define @AL:%9$(%1L%0)$ @AL:%9$(<%1>76%0)$
#define @AM:%9$(%1M%0)$ @AM:%9$(<%1>77%0)$
#define @AN:%9$(%1N%0)$ @AN:%9$(<%1>78%0)$
#define @AO:%9$(%1O%0)$ @AO:%9$(<%1>79%0)$
#define @AP:%9$(%1P%0)$ @AP:%9$(<%1>80%0)$
#define @AQ:%9$(%1Q%0)$ @AQ:%9$(<%1>81%0)$
#define @AR:%9$(%1R%0)$ @AR:%9$(<%1>82%0)$
#define @AS:%9$(%1S%0)$ @AS:%9$(<%1>83%0)$
#define @AT:%9$(%1T%0)$ @AT:%9$(<%1>84%0)$
#define @AU:%9$(%1U%0)$ @AU:%9$(<%1>85%0)$
#define @AV:%9$(%1V%0)$ @AV:%9$(<%1>86%0)$
#define @AW:%9$(%1W%0)$ @AW:%9$(<%1>87%0)$
#define @AX:%9$(%1X%0)$ @AX:%9$(<%1>88%0)$
#define @AY:%9$(%1Y%0)$ @AY:%9$(<%1>89%0)$
#define @AZ:%9$(%1Z%0)$ @AZ:%9$(<%1>90%0)$

#define @A_:%9$(%1_%0)$ @A_:%9$(<%1>95%0)$
#define @A@:%9$(%1@%0)$ @A@:%9$(<%1>64%0)$

#define @B_:%9$(%1\32;%0)$ @B_:%9$(<%1>32%0)$

#define @Ba:%9$(%1A%0)$ @Ba:%9$(<%1>97%0)$
#define @Bb:%9$(%1B%0)$ @Bb:%9$(<%1>98%0)$
#define @Bc:%9$(%1C%0)$ @Bc:%9$(<%1>99%0)$
#define @Bd:%9$(%1D%0)$ @Bd:%9$(<%1>100%0)$
#define @Be:%9$(%1E%0)$ @Be:%9$(<%1>101%0)$
#define @Bf:%9$(%1F%0)$ @Bf:%9$(<%1>102%0)$
#define @Bg:%9$(%1G%0)$ @Bg:%9$(<%1>103%0)$
#define @Bh:%9$(%1H%0)$ @Bh:%9$(<%1>104%0)$
#define @Bi:%9$(%1I%0)$ @Bi:%9$(<%1>105%0)$
#define @Bj:%9$(%1J%0)$ @Bj:%9$(<%1>106%0)$
#define @Bk:%9$(%1K%0)$ @Bk:%9$(<%1>107%0)$
#define @Bl:%9$(%1L%0)$ @Bl:%9$(<%1>108%0)$
#define @Bm:%9$(%1M%0)$ @Bm:%9$(<%1>109%0)$
#define @Bn:%9$(%1N%0)$ @Bn:%9$(<%1>110%0)$
#define @Bo:%9$(%1O%0)$ @Bo:%9$(<%1>111%0)$
#define @Bp:%9$(%1P%0)$ @Bp:%9$(<%1>112%0)$
#define @Bq:%9$(%1Q%0)$ @Bq:%9$(<%1>113%0)$
#define @Br:%9$(%1R%0)$ @Br:%9$(<%1>114%0)$
#define @Bs:%9$(%1S%0)$ @Bs:%9$(<%1>115%0)$
#define @Bt:%9$(%1T%0)$ @Bt:%9$(<%1>116%0)$
#define @Bu:%9$(%1U%0)$ @Bu:%9$(<%1>117%0)$
#define @Bv:%9$(%1V%0)$ @Bv:%9$(<%1>118%0)$
#define @Bw:%9$(%1W%0)$ @Bw:%9$(<%1>119%0)$
#define @Bx:%9$(%1X%0)$ @Bx:%9$(<%1>120%0)$
#define @By:%9$(%1Y%0)$ @By:%9$(<%1>121%0)$
#define @Bz:%9$(%1Z%0)$ @Bz:%9$(<%1>122%0)$

#define HASH@bernstein(%0) HASH_:_REHASH@b(%0,@,@)

#define _REHASH@b(%0,%1)%9) _HASH@b_%0(%1,)(5381))

#define _DOHASH@b(%0,%1,%2)(%8) _HASH@b_%1(%2)((%8*33^%0))

#define _HASH@b_(%0)(%8) _DOHASH@b(32,%0)(%8)

#define _HASH@b_0(%0)(%8) _DOHASH@b(48,%0)(%8)
#define _HASH@b_1(%0)(%8) _DOHASH@b(49,%0)(%8)
#define _HASH@b_2(%0)(%8) _DOHASH@b(50,%0)(%8)
#define _HASH@b_3(%0)(%8) _DOHASH@b(51,%0)(%8)
#define _HASH@b_4(%0)(%8) _DOHASH@b(52,%0)(%8)
#define _HASH@b_5(%0)(%8) _DOHASH@b(53,%0)(%8)
#define _HASH@b_6(%0)(%8) _DOHASH@b(54,%0)(%8)
#define _HASH@b_7(%0)(%8) _DOHASH@b(55,%0)(%8)
#define _HASH@b_8(%0)(%8) _DOHASH@b(56,%0)(%8)
#define _HASH@b_9(%0)(%8) _DOHASH@b(57,%0)(%8)

#define _HASH@b_A(%0)(%8) _DOHASH@b(65,%0)(%8)
#define _HASH@b_B(%0)(%8) _DOHASH@b(66,%0)(%8)
#define _HASH@b_C(%0)(%8) _DOHASH@b(67,%0)(%8)
#define _HASH@b_D(%0)(%8) _DOHASH@b(68,%0)(%8)
#define _HASH@b_E(%0)(%8) _DOHASH@b(69,%0)(%8)
#define _HASH@b_F(%0)(%8) _DOHASH@b(70,%0)(%8)
#define _HASH@b_G(%0)(%8) _DOHASH@b(71,%0)(%8)
#define _HASH@b_H(%0)(%8) _DOHASH@b(72,%0)(%8)
#define _HASH@b_I(%0)(%8) _DOHASH@b(73,%0)(%8)
#define _HASH@b_J(%0)(%8) _DOHASH@b(74,%0)(%8)
#define _HASH@b_K(%0)(%8) _DOHASH@b(75,%0)(%8)
#define _HASH@b_L(%0)(%8) _DOHASH@b(76,%0)(%8)
#define _HASH@b_M(%0)(%8) _DOHASH@b(77,%0)(%8)
#define _HASH@b_N(%0)(%8) _DOHASH@b(78,%0)(%8)
#define _HASH@b_O(%0)(%8) _DOHASH@b(79,%0)(%8)
#define _HASH@b_P(%0)(%8) _DOHASH@b(80,%0)(%8)
#define _HASH@b_Q(%0)(%8) _DOHASH@b(81,%0)(%8)
#define _HASH@b_R(%0)(%8) _DOHASH@b(82,%0)(%8)
#define _HASH@b_S(%0)(%8) _DOHASH@b(83,%0)(%8)
#define _HASH@b_T(%0)(%8) _DOHASH@b(84,%0)(%8)
#define _HASH@b_U(%0)(%8) _DOHASH@b(85,%0)(%8)
#define _HASH@b_V(%0)(%8) _DOHASH@b(86,%0)(%8)
#define _HASH@b_W(%0)(%8) _DOHASH@b(87,%0)(%8)
#define _HASH@b_X(%0)(%8) _DOHASH@b(88,%0)(%8)
#define _HASH@b_Y(%0)(%8) _DOHASH@b(89,%0)(%8)
#define _HASH@b_Z(%0)(%8) _DOHASH@b(90,%0)(%8)


#define _HASH@b__(%0)(%8) _DOHASH@b(95,%0)(%8)

#define _HASH@b_a(%0)(%8) _DOHASH@b(97,%0)(%8)
#define _HASH@b_b(%0)(%8) _DOHASH@b(98,%0)(%8)
#define _HASH@b_c(%0)(%8) _DOHASH@b(99,%0)(%8)
#define _HASH@b_d(%0)(%8) _DOHASH@b(100,%0)(%8)
#define _HASH@b_e(%0)(%8) _DOHASH@b(101,%0)(%8)
#define _HASH@b_f(%0)(%8) _DOHASH@b(102,%0)(%8)
#define _HASH@b_g(%0)(%8) _DOHASH@b(103,%0)(%8)
#define _HASH@b_h(%0)(%8) _DOHASH@b(104,%0)(%8)
#define _HASH@b_i(%0)(%8) _DOHASH@b(105,%0)(%8)
#define _HASH@b_j(%0)(%8) _DOHASH@b(106,%0)(%8)
#define _HASH@b_k(%0)(%8) _DOHASH@b(107,%0)(%8)
#define _HASH@b_l(%0)(%8) _DOHASH@b(108,%0)(%8)
#define _HASH@b_m(%0)(%8) _DOHASH@b(109,%0)(%8)
#define _HASH@b_n(%0)(%8) _DOHASH@b(110,%0)(%8)
#define _HASH@b_o(%0)(%8) _DOHASH@b(111,%0)(%8)
#define _HASH@b_p(%0)(%8) _DOHASH@b(112,%0)(%8)
#define _HASH@b_q(%0)(%8) _DOHASH@b(113,%0)(%8)
#define _HASH@b_r(%0)(%8) _DOHASH@b(114,%0)(%8)
#define _HASH@b_s(%0)(%8) _DOHASH@b(115,%0)(%8)
#define _HASH@b_t(%0)(%8) _DOHASH@b(116,%0)(%8)
#define _HASH@b_u(%0)(%8) _DOHASH@b(117,%0)(%8)
#define _HASH@b_v(%0)(%8) _DOHASH@b(118,%0)(%8)
#define _HASH@b_w(%0)(%8) _DOHASH@b(119,%0)(%8)
#define _HASH@b_x(%0)(%8) _DOHASH@b(120,%0)(%8)
#define _HASH@b_y(%0)(%8) _DOHASH@b(121,%0)(%8)
#define _HASH@b_z(%0)(%8) _DOHASH@b(122,%0)(%8)

#define _HASH@b_@(%0)(%8) %8

#define HASHi@bernstein(%0) HASH_:_REHASH@ib(%0,@,@)

#define _REHASH@ib(%0,%1)%9) _HASH@ib_%0(%1,)(5381))

#define _DOHASH@ib(%0,%1,%2)(%8) _HASH@ib_%1(%2)((%8*33^%0))

#define _HASH@ib_(%0)(%8) _DOHASH@ib(32,%0)(%8)

#define _HASH@ib_0(%0)(%8) _DOHASH@ib(48,%0)(%8)
#define _HASH@ib_1(%0)(%8) _DOHASH@ib(49,%0)(%8)
#define _HASH@ib_2(%0)(%8) _DOHASH@ib(50,%0)(%8)
#define _HASH@ib_3(%0)(%8) _DOHASH@ib(51,%0)(%8)
#define _HASH@ib_4(%0)(%8) _DOHASH@ib(52,%0)(%8)
#define _HASH@ib_5(%0)(%8) _DOHASH@ib(53,%0)(%8)
#define _HASH@ib_6(%0)(%8) _DOHASH@ib(54,%0)(%8)
#define _HASH@ib_7(%0)(%8) _DOHASH@ib(55,%0)(%8)
#define _HASH@ib_8(%0)(%8) _DOHASH@ib(56,%0)(%8)
#define _HASH@ib_9(%0)(%8) _DOHASH@ib(57,%0)(%8)

#define _HASH@ib_A(%0)(%8) _DOHASH@ib(97,%0)(%8)
#define _HASH@ib_B(%0)(%8) _DOHASH@ib(98,%0)(%8)
#define _HASH@ib_C(%0)(%8) _DOHASH@ib(99,%0)(%8)
#define _HASH@ib_D(%0)(%8) _DOHASH@ib(100,%0)(%8)
#define _HASH@ib_E(%0)(%8) _DOHASH@ib(101,%0)(%8)
#define _HASH@ib_F(%0)(%8) _DOHASH@ib(102,%0)(%8)
#define _HASH@ib_G(%0)(%8) _DOHASH@ib(103,%0)(%8)
#define _HASH@ib_H(%0)(%8) _DOHASH@ib(104,%0)(%8)
#define _HASH@ib_I(%0)(%8) _DOHASH@ib(105,%0)(%8)
#define _HASH@ib_J(%0)(%8) _DOHASH@ib(106,%0)(%8)
#define _HASH@ib_K(%0)(%8) _DOHASH@ib(107,%0)(%8)
#define _HASH@ib_L(%0)(%8) _DOHASH@ib(108,%0)(%8)
#define _HASH@ib_M(%0)(%8) _DOHASH@ib(109,%0)(%8)
#define _HASH@ib_N(%0)(%8) _DOHASH@ib(110,%0)(%8)
#define _HASH@ib_O(%0)(%8) _DOHASH@ib(111,%0)(%8)
#define _HASH@ib_P(%0)(%8) _DOHASH@ib(112,%0)(%8)
#define _HASH@ib_Q(%0)(%8) _DOHASH@ib(113,%0)(%8)
#define _HASH@ib_R(%0)(%8) _DOHASH@ib(114,%0)(%8)
#define _HASH@ib_S(%0)(%8) _DOHASH@ib(115,%0)(%8)
#define _HASH@ib_T(%0)(%8) _DOHASH@ib(116,%0)(%8)
#define _HASH@ib_U(%0)(%8) _DOHASH@ib(117,%0)(%8)
#define _HASH@ib_V(%0)(%8) _DOHASH@ib(118,%0)(%8)
#define _HASH@ib_W(%0)(%8) _DOHASH@ib(119,%0)(%8)
#define _HASH@ib_X(%0)(%8) _DOHASH@ib(120,%0)(%8)
#define _HASH@ib_Y(%0)(%8) _DOHASH@ib(121,%0)(%8)
#define _HASH@ib_Z(%0)(%8) _DOHASH@ib(122,%0)(%8)

#define _HASH@ib__(%0)(%8) _DOHASH@ib(95,%0)(%8)

#define _HASH@ib_a(%0)(%8) _DOHASH@ib(97,%0)(%8)
#define _HASH@ib_b(%0)(%8) _DOHASH@ib(98,%0)(%8)
#define _HASH@ib_c(%0)(%8) _DOHASH@ib(99,%0)(%8)
#define _HASH@ib_d(%0)(%8) _DOHASH@ib(100,%0)(%8)
#define _HASH@ib_e(%0)(%8) _DOHASH@ib(101,%0)(%8)
#define _HASH@ib_f(%0)(%8) _DOHASH@ib(102,%0)(%8)
#define _HASH@ib_g(%0)(%8) _DOHASH@ib(103,%0)(%8)
#define _HASH@ib_h(%0)(%8) _DOHASH@ib(104,%0)(%8)
#define _HASH@ib_i(%0)(%8) _DOHASH@ib(105,%0)(%8)
#define _HASH@ib_j(%0)(%8) _DOHASH@ib(106,%0)(%8)
#define _HASH@ib_k(%0)(%8) _DOHASH@ib(107,%0)(%8)
#define _HASH@ib_l(%0)(%8) _DOHASH@ib(108,%0)(%8)
#define _HASH@ib_m(%0)(%8) _DOHASH@ib(109,%0)(%8)
#define _HASH@ib_n(%0)(%8) _DOHASH@ib(110,%0)(%8)
#define _HASH@ib_o(%0)(%8) _DOHASH@ib(111,%0)(%8)
#define _HASH@ib_p(%0)(%8) _DOHASH@ib(112,%0)(%8)
#define _HASH@ib_q(%0)(%8) _DOHASH@ib(113,%0)(%8)
#define _HASH@ib_r(%0)(%8) _DOHASH@ib(114,%0)(%8)
#define _HASH@ib_s(%0)(%8) _DOHASH@ib(115,%0)(%8)
#define _HASH@ib_t(%0)(%8) _DOHASH@ib(116,%0)(%8)
#define _HASH@ib_u(%0)(%8) _DOHASH@ib(117,%0)(%8)
#define _HASH@ib_v(%0)(%8) _DOHASH@ib(118,%0)(%8)
#define _HASH@ib_w(%0)(%8) _DOHASH@ib(119,%0)(%8)
#define _HASH@ib_x(%0)(%8) _DOHASH@ib(120,%0)(%8)
#define _HASH@ib_y(%0)(%8) _DOHASH@ib(121,%0)(%8)
#define _HASH@ib_z(%0)(%8) _DOHASH@ib(122,%0)(%8)

#define _HASH@ib_@(%0)(%8) %8
