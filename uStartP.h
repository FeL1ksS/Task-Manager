//---------------------------------------------------------------------------

#ifndef uStartPH
#define uStartPH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.WinXCtrls.hpp>
//---------------------------------------------------------------------------
class TStartForm : public TForm
{
__published:	// IDE-managed Components
	TTimer *Timer1;
	TImage *Image1;
	TActivityIndicator *ActivityIndicator1;
	void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TStartForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TStartForm *StartForm;
//---------------------------------------------------------------------------
#endif
