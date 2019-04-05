Module MinADtime 

Include "ADC.bas" 

{ You can include this option in your main program 
  to indicate the external impedance in OHMs 
    #option AD_RESIST = 2000 
} 
//---- Set Default AD_RESIST if needed ------------------------------ 
#if IsOption(AD_RESIST) 
  Const ADimpedance  = AD_RESIST 
#else 
  Const ADimpedance  = 10000 
#endif 
Const  MinAD_AQ As Byte = (((ADimpedance +1000)*(120*7))/1000000)+1 

//---- Find Minimum A/D oscillator requirements --------------------- 
#variable  ADoscRaw = 64/(40.0/_clock) 
#if ADoscRaw > 64 
     Const MinAD_OSC = FRC 
#elseIf ADoscRaw > 32 
     Const MinAD_OSC = FOSC_64 
#elseif ADoscRaw > 16 
     Const MinAD_OSC = FOSC_32 
#elseif ADoscRaw > 8 
     Const MinAD_OSC = FOSC_16 
#elseif ADoscRaw > 4 
     Const MinAD_OSC = FOSC_8 
#elseif ADoscRaw > 2 
     Const MinAD_OSC = FOSC_4 
#else 
     Const MinAD_OSC = FOSC_2 
#endif 

//------------------------------------------------------------------- 
Public Sub SetMinADtime() 
     SetConvTime(MinAD_OSC) 
     SetAcqTime(MinAD_AQ) 
End Sub 

Public Sub SetMinADtime(Mult As Byte) 
     SetConvTime(MinAD_OSC) 
     SetAcqTime(MinAD_AQ * Mult) 
End Sub 

//----[Initialize]--------------------------------------------------- 
SetMinADtime
