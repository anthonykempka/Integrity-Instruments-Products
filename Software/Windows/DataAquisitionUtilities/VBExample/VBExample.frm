VERSION 5.00
Object = "{CBE068B4-C05C-49D7-9B1D-44AE4069AF4C}#1.0#0"; "INTEGR~1.OCX"
Begin VB.Form Form1 
   Caption         =   "VB Example"
   ClientHeight    =   4785
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6570
   LinkTopic       =   "Form1"
   ScaleHeight     =   4785
   ScaleWidth      =   6570
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnAbout 
      Caption         =   "About"
      Height          =   495
      Left            =   120
      TabIndex        =   12
      Top             =   1320
      Width           =   1575
   End
   Begin VB.Frame Frame1 
      Caption         =   "Analog I/O Control"
      Height          =   1815
      Left            =   2400
      TabIndex        =   5
      Top             =   720
      Width           =   3615
      Begin VB.CommandButton btnADC0In 
         Caption         =   "Get A/D"
         Height          =   375
         Left            =   1800
         TabIndex        =   11
         Top             =   1200
         Width           =   1335
      End
      Begin VB.TextBox txtADC0Input 
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0.0000"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   1
         EndProperty
         Height          =   375
         Left            =   240
         TabIndex        =   10
         Top             =   1200
         Width           =   1215
      End
      Begin VB.CommandButton btnDAC0Out 
         Caption         =   "Set D/A"
         Height          =   375
         Left            =   1800
         TabIndex        =   8
         Top             =   480
         Width           =   1335
      End
      Begin VB.TextBox txtDAC0Output 
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0.0000"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   1
         EndProperty
         Height          =   375
         Left            =   240
         TabIndex        =   6
         Text            =   "0.0"
         Top             =   480
         Width           =   1215
      End
      Begin VB.Label Label2 
         Caption         =   "A/D 0 input"
         Height          =   255
         Index           =   1
         Left            =   240
         TabIndex        =   9
         Top             =   960
         Width           =   1095
      End
      Begin VB.Label Label1 
         Caption         =   "D/A 0 output"
         Height          =   255
         Index           =   0
         Left            =   240
         TabIndex        =   7
         Top             =   240
         Width           =   1095
      End
   End
   Begin VB.CommandButton btnTogglePort 
      Caption         =   "Toggle P1.0"
      Height          =   495
      Left            =   2400
      TabIndex        =   3
      Top             =   120
      Width           =   1455
   End
   Begin VB.CommandButton btnStop 
      Caption         =   "Stop"
      Height          =   495
      Left            =   120
      TabIndex        =   2
      Top             =   720
      Width           =   1575
   End
   Begin VB.CommandButton btnStart 
      Caption         =   "Start"
      Height          =   495
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   1575
   End
   Begin INTEGRITYIOLib.IntegrityIO IntegrityIO1 
      Height          =   735
      Left            =   120
      TabIndex        =   0
      Top             =   3960
      Width           =   1575
      _Version        =   65536
      _ExtentX        =   2778
      _ExtentY        =   1296
      _StockProps     =   0
   End
   Begin VB.Label lblPort2In 
      Caption         =   "Off"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   24
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   4320
      TabIndex        =   4
      Top             =   120
      Width           =   735
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub btnAbout_Click()
    IntegrityIO1.AboutBox
End Sub

Private Sub btnADC0In_Click()
    
    
    ' Set the ADC control word
    ' ADC Channel 0 Single point conversion
    IntegrityIO1.lnADCControl = 8
    
    ' Get the A/D reading and update the
    ' text box. Format output to 3 decimal places
    txtADC0Input = Format(IntegrityIO1.dADCUnipolar, "0.000")
    
    
End Sub

Private Sub btnDAC0Out_Click()
    Dim dTemp As Double
    
    ' Set the control word to DAC 0
    IntegrityIO1.lnDACControl = 0
    
    ' Set the output voltage to whatever the user entered
    dTemp = Val(txtDAC0Output)
    
    ' Check to see if the output voltage is in range
    If (dTemp < 0) Or (dTemp > IntegrityIO1.dReferenceVoltage) Then
        ' Output voltage is out of range
        ' Tell the user then get out of the subroutine
        MsgBox "D/A output is out of range."
        Exit Sub
    End If
    
    ' Send the desired output voltage to the ADC-xxx
    IntegrityIO1.dDACOutput = dTemp
    
    ' Set the ADC control word
    ' ADC Channel 0 Single point conversion
    IntegrityIO1.lnADCControl = 8
    
    ' Get the A/D reading and update the
    ' text box. Format output to 3 decimal places
    txtADC0Input = Format(IntegrityIO1.dADCUnipolar, "0.000")
    
End Sub

Private Sub btnStart_Click()

    ' Set the serial port to COM1
    IntegrityIO1.lnComPort = 1
    
    ' Set the communication speed to 115200 Baud
    IntegrityIO1.lnBaudRate = 115200
    
    ' Open the serial port
    IntegrityIO1.bPortOpen = True
    
    ' Configure Port 2 bit 0 as an input
    ' Bit position = 0
    ' Bit Value = 1
    IntegrityIO1.SetDIOBitDDR 1, 0
    
    ' Configure Port 1 bit 0 as an output
    ' Bit Position = 8
    ' Bit Value = 0
    IntegrityIO1.SetDIOBitDDR 0, 8
    
    ' Set all Port1 outputs to 0
    IntegrityIO1.lnDigitalPort = 0
    
End Sub

Private Sub btnStop_Click()
    ' Close the communication port
    IntegrityIO1.bPortOpen = False
End Sub

Private Sub btnTogglePort_Click()
    ' Function assumes that Port 1 bit 0 is
    ' looped back to Port 2 bit 0
    
    ' Declare a long variable
    Dim p2In As Long
    
    ' Read in the state of P2 Bit 0
    p2In = IntegrityIO1.GetDIOBit(0)
    
    'Invert data bit
    p2In = p2In Xor 1
    
    ' Set Port 1 Bit 0 to the inverted value
    ' Of Port 2 Bit 0
    IntegrityIO1.SetDIOBit p2In, 8
    
    ' Read back the Value of Port 2 Bit 0
    p2In = IntegrityIO1.GetDIOBit(0)
    
    ' Update the display
    If p2In = 1 Then
        lblPort2In.Caption = "On"
    Else
        lblPort2In.Caption = "Off"
    End If
    
End Sub

