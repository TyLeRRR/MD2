{
*****************************************************************************
*  Name    : ADC.bas                                                        *
*  Author  : David John Barker                                              *
*  Notice  : Copyright (c) 2007 Mecanique                                   *
*          : All Rights Reserved                                            *
*  Date    : 12/01/2007                                                     *
*  Version : 1.0                                                            * 
*          : 1.1 Corrected bitnames for 18F1220, 18F1320 devices            *
*  Notes   :                                                                *
*          :                                                                *
*****************************************************************************
}
Module ADC
  
// conversion time constants...
Public Const
   FOSC_2  = %000,
   FOSC_4  = %100,
   FOSC_8  = %001,
   FOSC_16 = %101,
   FOSC_32 = %010,
   FOSC_64 = %110,
   FRC     = %011

// channel select constants...
Public Const
   AN0 = $00,
   AN1 = $01,
   AN2 = $02,
   AN3 = $03,
   AN4 = $04,
   AN5 = $05,
   AN6 = $06
   
#if _adc = 8
Public Const
   AN7 = $07
#elseif _adc = 12
Public Const
   AN8 = $08,
   AN9 = $09,
   AN10 = $0A,
   AN11 = $0B
#elseif _adc = 16
Public Const
   AN12 = $0C,
   AN13 = $0D,
   AN14 = $0E,
   AN15 = $0F
#elseif _adc = 20
Public Const
   AN12 = $0C,
   AN13 = $0D,
   AN14 = $0E,
   AN15 = $0F,
   AN16 = $10,
   AN17 = $11,
   AN18 = $12,
   AN19 = $13
#endif   
            
Public Dim
#if _device in (18F1220, 18F1320)
   // ADCON0...
   VCFG1   As ADCON0.6,
   VCFG0   As ADCON0.5,
   CHS2    As ADCON0.4,
   CHS1    As ADCON0.3,
   CHS0    As ADCON0.2,
   GO_DONE As ADCON0.1,
   ADON    As ADCON0.0,

   // ADCON1...
   PCFG6 As ADCON1.6,
   PCFG5 As ADCON1.5,
   PCFG4 As ADCON1.4,
   PCFG3 As ADCON1.3,
   PCFG2 As ADCON1.2,
   PCFG1 As ADCON1.1,
   PCFG0 As ADCON1.0,  
   
   // ADCON2...
   ADFM As ADCON2.7,
   AQT2 As ADCON2.5,
   AQT1 As ADCON2.4,   
   AQT0 As ADCON2.3,
   ADCS2 As ADCON2.2,
   ADCS1 As ADCON2.1,
   ADCS0 As ADCON2.0, 

   // extended alias...
   Convert As ADCON0.Booleans(1),       // GO_DONE, POR = 0
   RightJustify As ADCON2.Booleans(7)   // ADFM, POR = false
#elseif _adc > 8
   // ADCON0...
   CHS3    As ADCON0.5,
   CHS2    As ADCON0.4,
   CHS1    As ADCON0.3,
   CHS0    As ADCON0.2,
   GO_DONE As ADCON0.1,
   ADON    As ADCON0.0,

   // ADCON1...
   VCFG1 As ADCON1.5,
   VCFG0 As ADCON1.4,
   PCFG3 As ADCON1.3,
   PCFG2 As ADCON1.2,
   PCFG1 As ADCON1.1,
   PCFG0 As ADCON1.0,  
   
   // ADCON2...
   ADFM As ADCON2.7,
   ADCS2 As ADCON2.2,
   ADCS1 As ADCON2.1,
   ADCS0 As ADCON2.0, 

   // extended alias...
   Convert As ADCON0.Booleans(1),       // GO_DONE, POR = 0
   RightJustify As ADCON2.Booleans(7)   // ADFM, POR = false
#else
   // ADCON0...
   ADCS1   As ADCON0.7,
   ADCS0   As ADCON0.6,
   CHS2    As ADCON0.5,
   CHS1    As ADCON0.4,
   CHS0    As ADCON0.3,
   GO_DONE As ADCON0.2,
   ADON    As ADCON0.0,

   // ADCON1...
   ADFM  As ADCON1.7,
   ADCS2 As ADCON1.6,
   PCFG3 As ADCON1.3,
   PCFG2 As ADCON1.2,
   PCFG1 As ADCON1.1,
   PCFG0 As ADCON1.0,

   // extended alias...
   Convert As ADCON0.Booleans(2),       // GO_DONE, POR = 0
   RightJustify As ADCON1.Booleans(7)   // ADFM, POR = false
#endif

// ADCON(x) extended alias...   
Public Dim
   Enabled As ADCON0.Booleans(0),       // ADON, POR = 0 
   ADResult As ADRESL.AsWord            // 16 bit ADC result

// local helper variables...
Dim 
   FAcquisitionTime As Byte            
{
****************************************************************************
* Name    : SetConvTime                                                    *
* Purpose : Set the conversion time. Valid parameters include FOSC_2,      *
*         : FOSC_4, FOSC_8, FOSC_16, FOSC_32, FOSC_64 and FRC              *
****************************************************************************
}
Public Sub SetConvTime(pValue As Byte)
   ADCS2 = pValue.2
   ADCS1 = pValue.1
   ADCS0 = pValue.0
End Sub 
{
****************************************************************************
* Name    : SetAcqTime                                                     *
* Purpose : Sets the ADC acquisition time, in microseconds (us)            *
****************************************************************************
}
Public Sub SetAcqTime(pATime As Byte)
   FAcquisitionTime = pATime
End Sub
{
****************************************************************************
* Name    : SetConfig                                                      *
* Purpose : Set configuration control bits                                 *
****************************************************************************
}
Public Sub SetConfig(pConfig As Byte) 
   #if _adc > 8 Or _device in (18F1220, 18F1320)
   ADCON1 = pConfig And $3F
   #else
   ADCON1 = ADCON1 And $F0
   pConfig = pConfig And $0F
   ADCON1 = ADCON1 Or pConfig 
   #endif
End Sub  
{
****************************************************************************
* Name    : Read                                                           *
* Purpose : Read currently uses delayus for the acquistion time, which has *
*         : the following minimum overheads                                *
*         : 4Mhz - 24us                                                    *
*         : 8Mhz - 12us                                                    *
*         : 10Mhz - 8us                                                    *
*         : 16Mhz - 5us                                                    *
*         : 20Mhz plus - 2us                                               * 
*         : If your acquistion time requirements fall below the above,     *
*         : just create a copy of this function in your main program (or   *
*         : another module) and replace delayms(ATime) with a constant     *
****************************************************************************
}
Public Function Read(pChannel As Byte) As Word

   // set channel... - this is commented out to allow the adc to work with all of the 18f26k22's adc pins
       'CHS0 = pChannel.0
       'CHS1 = pChannel.1
       'CHS2 = pChannel.2
       '#if _adc > 8
       'CHS3 = pChannel.3
       '#endif
       
       ' this next line allows the ADC to read all of the 18f26k22 ADC pins (which has more than normal 18f pics
       ADCON0 = (ADCON0 And $03) Or (pChannel << 2)
   
   // read ADC...
   If FAcquisitionTime = 0 Then
      Enabled = true  
      Convert = true
   Else   
      Enabled = true  
      DelayUS(FAcquisitionTime)
      Convert = true  
   EndIf  
   
   // wait for completion, then disable ADC...
   While Convert      
   Wend           
   Enabled = false   
   Result = ADResult
End Function

// adc read for 26K22
Public Function ReadNew(pChannel As Byte) As Word
   // set channel...
   ADCON0 = (ADCON0 And $03) Or (pChannel << 2)   
   
   // read ADC...
   If FAcquisitionTime = 0 Then
      Enabled = true  
      Convert = true
   Else   
      Enabled = true  
      DelayUS(FAcquisitionTime)
      Convert = true  
   EndIf  
   
   // wait for completion, then disable ADC...
   While Convert      
   Wend           
   Enabled = false   
   Result = ADResult
End Function
{
****************************************************************************
* Name    : ShellSort (PRIVATE)                                            *
* Purpose : Shell-Metzner sorting algorithm                                *
****************************************************************************
}
Sub ShellSort(ByRef pArray() As Word)
   Dim Finished	As Boolean
   Dim Jump, Index, LoopIndex As Word
   Dim SwapTemp As Word
   Jump = Bound(pArray)
   While Jump > 1
      Jump = Jump / 2
      Repeat
	     Finished = true
	     For LoopIndex = 0 To Bound(pArray) - Jump
            Index = LoopIndex + Jump
	        If pArray(LoopIndex) > pArray(Index) Then
  	           SwapTemp = pArray(LoopIndex)
  	           pArray(LoopIndex) = pArray(Index)
  	           pArray(Index) = SwapTemp
		        Finished = false
		    EndIf
         Next
     Until Finished
   Wend
End Sub   
{
****************************************************************************
* Name    : GetMedian (PRIVATE)                                            *
* Purpose : Returns the median of a sample data set                        *
****************************************************************************
}
Function GetMedian(ByRef pSamples() As Word) As Word
   ShellSort(pSamples)
   result = pSamples((Bound(pSamples) + 1) / 2)
End Function  
{
****************************************************************************
* Name    : ReadMedian                                                     *
* Purpose : Takes 64 ADC samples and returns the median average. Although  *
*         : computationally expensive, this routine is quite useful as the *
*         : result is not influenced by extreme high and low sample values *
****************************************************************************
}
Public Function ReadMedian(pChannel As Byte) As Word
   Dim Index As Byte
   Dim Sum(64) As Word 
   For index = 0 To Bound(Sum)
      Sum(Index) = Read(pChannel)
      DelayUS(20)
   Next
   Result = GetMedian(Sum) 
End Function
{
****************************************************************************
* Name    : ReadMean                                                       *
* Purpose : Takes pRange ADC samples and returns the mean average. Much    *
*         : quicker than median average, but can be influenced by extreme  *
*         : high and low sample values                                     *
****************************************************************************
}
Public Function ReadMean(pChannel As Byte, pRange As Byte = 64) As Word
   Dim Index As Byte
   Dim Sum As LongWord
   If pRange = 0 Then
      Result = 0
   Else   
      Sum = 0
      For index = 0 To pRange - 1
         Sum = Sum + Read(pChannel)
         DelayUS(20)
      Next
      Result = Sum / pRange
   EndIf   
End Function

// defaults
RightJustify = true
FAcquisitionTime = 20
