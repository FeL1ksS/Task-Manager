//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uStartP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TStartForm *StartForm;
//---------------------------------------------------------------------------
__fastcall TStartForm::TStartForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TStartForm::Timer1Timer(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
