unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, StdCtrls, ComCtrls,
  Serial, dbugintf;

type

  { TForm1 }

  TForm1 = class(TForm)
    BackwardsTB: TButton;
    ForwardTB: TButton;
    FwdLeftTB: TButton;
    FwdRightTB: TButton;
    Label1: TLabel;
    LeftTB: TButton;
    NopTB: TButton;
    PortEdit: TEdit;
    DurationSlider: TTrackBar;
    RightTB: TButton;
    Stop1TB: TButton;
    StopTB: TButton;
    UpDown1: TUpDown;


    procedure DurationSliderChange(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure SetButtonSizes;
    procedure FwdLeftTBClick(Sender: TObject);

    procedure FwdRightTBClick(Sender: TObject);
    procedure LeftTBClick(Sender: TObject);
    procedure PortEditChange(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure ForwardTBClick(Sender: TObject);
    procedure MoveJoystick(xpos:integer;ypos:integer);
    procedure BackwardsTBClick(Sender: TObject);
    procedure RightTBClick(Sender: TObject);
    procedure StopTBClick(Sender: TObject);
    procedure Stop1TBClick(Sender: TObject);
    procedure UpDown1Click(Sender: TObject; Button: TUDBtnType);
    procedure Stop();

  private

  public

  end;

const
  maxpos = 8;    // maximum servo angle
  midpos = 4;   // middle position is at 4  (0-8)
  maxspeed = 4;  // speed 0-4
var
  Form1: TForm1;
  portname: String;   // serial port name
  speed,duration:integer;  // global setting , that affects joystick movement
  status: LongInt;
  Xposition,Yposition: Integer;  // numbers forwarded as parameters to Move Joystick procedure
  hoverdrive: boolean; // mouse click control or both click hover/eyegaze/
  // control. if charged we can move - charging done by activating charge button-this prevents stuck cursor driving
implementation

{$R *.lfm}

{ TForm1 }

procedure TForm1.ForwardTBClick(Sender: TObject);
begin

  Xposition := midpos - speed;
  Yposition := midpos;
  MoveJoystick(Xposition,Yposition);
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  Label1.Caption :=  IntToStr(UpDown1.Position);        // Display initial speed value
  speed := UpDown1.Position; // set from 0 to 4, affects both y and x positions of the joystick
  duration := DurationSlider.Position;    // after this time joystick returns to middle position
  portname := PortEdit.Text;
  hoverdrive := False; // if enabled, we activate move by either
             // mouse hover and click , if disabled, only click works
end;

procedure TForm1.PortEditChange(Sender: TObject);
begin
  portname := PortEdit.Text;
end;

procedure TForm1.DurationSliderChange(Sender: TObject);
begin
   duration := DurationSlider.Position;
end;



procedure TForm1.FormResize(Sender: TObject);
begin
   SetButtonSizes;
end;

procedure TForm1.SetButtonSizes;
var
  tilewidth, tileheight  : integer;
begin
   // adjust all buttonss sizes
  tileheight:= (Form1.Height) div 3;
  tilewidth := (Form1.Width - (UpDown1.Width + 40 + DurationSlider.Width)) div 3;
  //SendDebug(inttostr(tilewidth));
  //SendDebug(inttostr(tileheight));

  ForwardTB.Height:=tileheight;
  FwdLeftTB.Height:=tileheight;
  LeftTB.Height:=tileheight;
  Stop1TB.Height:=tileheight;
  BackwardsTB.Height:=tileheight;
  StopTB.Height:=tileheight;
  RightTB.Height:=tileheight;
  FwdRightTB.Height:=tileheight;
  NopTB.Height:=tileheight;

  ForwardTB.Width:=tilewidth;
  FwdLeftTB.Width:=tilewidth;
  LeftTB.Width:=tilewidth;
  Stop1TB.Width:=tilewidth;
  BackwardsTB.Width:=tilewidth;
  StopTB.Width:=tilewidth;
  RightTB.Width:=tilewidth;
  FwdRightTB.Width:=tilewidth;
  NopTB.Width:=tilewidth;

end;

procedure TForm1.FwdLeftTBClick(Sender: TObject);
begin
  //  forward left
  Xposition := midpos - speed;
  Yposition := midpos - speed;
  MoveJoystick(Xposition,Yposition);
end;


procedure TForm1.FwdRightTBClick(Sender: TObject);
begin

  //  forward right
  Xposition := midpos - speed;
  Yposition := midpos + speed;
  MoveJoystick(Xposition,Yposition);
end;

procedure TForm1.LeftTBClick(Sender: TObject);
begin
  // turn Left
  Xposition := midpos;
  Yposition := midpos - speed;
  MoveJoystick(Xposition,Yposition);
end;



procedure TForm1.MoveJoystick(xpos:integer;ypos:integer);
// this procedure opens serial port, sends message to arduino, closes the port
// the message tells to which x and y positions will servos move and for how long (DurationSlider)
var
  msglen, i: integer;
  serHandle: Tserialhandle; // Handle for serial port
  message: String;
  status: LongInt;
begin

    serHandle := SerOpen(portname); // Bei Windows 'COMx' // COM-Port Ã¶ffnen.
    // writeln(serHandle,' ',message,' ', portname);
    message := '(' + IntToStr(xpos) + ':' + IntToStr(ypos) + ':' + IntToStr(duration) + ')';
    SerSetParams(serHandle, 9600, 8, NoneParity, 1, []);
    msglen := length(message);
    for i := 1 to msglen do begin
        status := SerWrite(serHandle, message[i], 1); // Zeichen senden.
    end;

    message := '';
    if(status > 0) then begin
      // writeln('written to serial port');
    end else begin
      // writeln('can not write to serial port, quitting');
      SerSync(serHandle); { flush out any remaining before closure }
      SerFlushOutput(serHandle); { discard any remaining output }
      SerClose(serHandle);
      // report error
    end;
    //arduino response code here:
    //
    SerSync(serHandle); { flush out any remaining before closure }
    SerFlushOutput(serHandle); { discard any remaining output }
    SerClose(serHandle);            // COM-Port schliessen.

end;

procedure TForm1.BackwardsTBClick(Sender: TObject);
begin

  // back

  Xposition := midpos + speed;
  Yposition := midpos;
  MoveJoystick(Xposition,Yposition);

end;

procedure TForm1.RightTBClick(Sender: TObject);
begin

  // turn right
  Xposition := midpos;
  Yposition := midpos + speed;
  MoveJoystick(Xposition,Yposition);
end;

procedure TForm1.StopTBClick(Sender: TObject);
begin

  Stop();
end;

procedure TForm1.Stop1TBClick(Sender: TObject);
begin

   Stop();
end;
procedure TForm1.Stop();
begin
  Xposition := midpos;
  Yposition := midpos;
  MoveJoystick(Xposition,Yposition);
end;

procedure TForm1.UpDown1Click(Sender: TObject; Button: TUDBtnType);
begin
  Label1.Caption :=  IntToStr(UpDown1.Position);
  speed := UpDown1.Position;
end;
//procedure TForm1.ReleaseToggleboxes;
//begin
//  ForwardTB.State:=cbUnchecked;
//  FwdLeftTB.State:=cbUnchecked;
//  LeftTB.State:=cbUnchecked;
//  Stop1TB.State:=cbUnchecked;
//  BackwardsTB.State:=cbUnchecked;
//  StopTB.State:=cbUnchecked;
//  RightTB.State:=cbUnchecked;
//  FwdRightTB.State:=cbUnchecked;
//  NopTB.State:=cbUnchecked;
//end;
end.

