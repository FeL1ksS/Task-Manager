//---------------------------------------------------------------------------

#ifndef UpdateModuleH
#define UpdateModuleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <IdHTTP.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

#ifdef INDY10
typedef __int64 IDHTTPINT;
#else
typedef const int IDHTTPINT;
#endif

//---------------------------------------------------------------------------
class TUpdateModuleForm : public TForm
{
__published:	// IDE-managed Components
		TProgressBar *ProgressBarUpdate;
private:	// User declarations
public:		// User declarations
	__fastcall TUpdateModuleForm(TComponent* Owner);
	void CheckUpdate(String sCurrentVersion, String sURLinf, bool bShowError = true, bool bAutoRenameToExe = true, bool bBlockForms = true);
	void __fastcall IdHTTPUpdateWorkBegin(TObject *ASender, TWorkMode AWorkMode, IDHTTPINT AWorkCountMax);
	void __fastcall IdHTTPUpdateWork(TObject *ASender, TWorkMode AWorkMode, IDHTTPINT AWorkCount);
	void __fastcall FuncOnThreadTerminate(TObject *ASender);
};

class TUpdateThread : public TThread
{
__published:	// IDE-managed Components
private:	// User declarations
protected:  // User declarations
	void __fastcall Execute();
public:		// User declarations
	__fastcall TUpdateThread(bool suspended);
	void __fastcall ShowUpdateFormAndBlockOther();
};

//---------------------------------------------------------------------------
extern PACKAGE TUpdateModuleForm *UpdateModuleForm;
//---------------------------------------------------------------------------
#endif
