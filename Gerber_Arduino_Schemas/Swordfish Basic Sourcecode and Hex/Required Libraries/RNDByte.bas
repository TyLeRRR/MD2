{
*****************************************************************************
*  Name    : RndByte.BAS                                                    *
*  Author  : David Eather                                                   *
*  Notice  : This code is placed into the Public Domain                     *
*          :                                                                *
*  Date    : 19/07/2011                                                     *
*  Version : 1.0                                                            *
*  Notes   : call InitializeRND(pValue) with a value between 0-255          *
*          : call GetRND() to get a Byte 0 to 255                           *
*****************************************************************************
}


Module Rand

 Dim LCG,GLFSR As Byte

Public Function GetRND() As Byte
 'LCG
 LCG=(7*LCG+17) 

 'Galios LFSR
 If (GLFSR And 1) = 1 Then
  GLFSR = GLFSR Xor 135 '135 is the tap 
  GLFSR = (GLFSR >> 1) Or $80
 Else
  GLFSR = (GLFSR >> 1)
 End If
    result = GLFSR Xor LCG 
End Function

Public Sub InitializeRND(ByVal ReSeed As Byte)
 LCG = ReSeed
 GLFSR = LCG Xor $55 'just making the start values very different - not realy important
 If GLFSR = 0 Then   'except that GLFSR must not be zero
  GLFSR=1 
 EndIf
End Sub

 GLFSR=1
 LCG=84
 End
