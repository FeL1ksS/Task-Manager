//---------------------------------------------------------------------------

#ifndef uCmdFormH
#define uCmdFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.DBCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
class TcmdForm : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TSaveDialog *SaveDialog1;
	TMemo *Memo1;
	TMemo *MemoOutput;
	TMemo *MemoInput;
	TImage *Image1;
	TImage *ImMenuDown;
	void __fastcall MemoInputKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Image1Click(TObject *Sender);
	void __fastcall ImMenuDownMouseEnter(TObject *Sender);
	void __fastcall ImMenuDownClick(TObject *Sender);
	void __fastcall ImMenuDownMouseLeave(TObject *Sender);

private:	// User declarations
	void Dos2MemoF(const AnsiString &sCommandLine);
public:		// User declarations
	__fastcall TcmdForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TcmdForm *cmdForm;
//---------------------------------------------------------------------------
#endif
