//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tlhelp32.h>
#include <windows.h>
#include <psapi.h>
#include <stdlib.h>
#include "Uproc.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

class PInf:TObject
 {
 public:

  String Name;
  DWORD Usage,
		Threads,
		PriClassBase,
		ID;
  HANDLE H;
  bool Suspended;
  WORD V1,V2;

 };



TForm1 *Form1;
TListItem * Item;


//---------------------------------------------------------------------------
DWORD GetPIDbyName(LPTSTR szProcessName) //Получение PID процесса по имени
{
        HANDLE hSnapshot;
        PROCESSENTRY32 pe = {sizeof(pe)};

		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE)
                return 0;

        if (!Process32First(hSnapshot, &pe))
                return 0;

        do
                if(!lstrcmpi(pe.szExeFile,szProcessName))
                        return pe.th32ProcessID;
        while (Process32Next(hSnapshot, &pe));
        return 0;
}
//---------------------------------------------------------------------------
int __fastcall TForm1::ProcessList()
{
	ListView1->Clear();
    HANDLE InfoProcess=0;
    HANDLE hToken=0;
    DWORD PID;
    bool EnumProcess;
    bool EnumThread;
    bool EnumModule;
    wchar_t pathP[500];
    TOKEN_USER* pUserInfo;
    char name[256];
    char domain[256];
    char *result;
    DWORD dwSize = 256;
    int iUse;
    int ColP=0;

//Сником процессов
    HANDLE ProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//Снимок модулей
    HANDLE ModuleSnap  = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,  0);
//Снимок потоков
//структура для вывода
    PROCESSENTRY32 Process;
    MODULEENTRY32  Module;
//установим ее размер, это необходимое действие
    Process.dwSize = sizeof(PROCESSENTRY32);
    Module.dwSize  = sizeof(MODULEENTRY32);
//Определение первого элемента
    EnumProcess = Process32First (ProcessSnap, &Process);
    EnumModule  = Module32First  (ModuleSnap,  &Module);

    while (EnumProcess)
    {

		PID=Process.th32ProcessID;
		AnsiString Fontdr=Process.szExeFile;

		if((PID==4)||(PID==0)||(Fontdr=="fontdrvhost.exe"))
        {
         EnumProcess = Process32Next(ProcessSnap, &Process);
        }
        else
        {
        ListView1->Items->Add();
        Item = ListView1->Items->Item[ListView1->Items->Count-1];
//-------------------------------Сбор информации о процессах---------------------------------------

        InfoProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE,
        Process.th32ProcessID);
    if (!(OpenProcessToken(InfoProcess, TOKEN_QUERY, &hToken)))
    {
		ListBox3->Items->Add("OpenToken " + SysErrorMessage(GetLastError()));
    }

    pUserInfo = (PTOKEN_USER) GlobalAlloc(GPTR, dwSize);
    if (!(GetTokenInformation(hToken, TokenUser, pUserInfo, dwSize, &dwSize)))
	{
		ListBox3->Items->Add("GetTokenInfo " + AnsiString(GetLastError()));
	}

    // Ошибка получения имени пользователя
    if (!(LookupAccountSidA(0, pUserInfo->User.Sid, name, &dwSize, domain,
        &dwSize, (PSID_NAME_USE) & iUse)))
	{
		ListBox3->Items->Add("Lookup " + SysErrorMessage(GetLastError()));
    }
		

		Item->Caption=PID;// Ид процесса
		Item->ImageIndex=0;//картинка
		Item->SubItems->Add(Process.szExeFile); // Имя процесса

        switch (GetPriorityClass(InfoProcess))
            {
              case 256: Item->SubItems->Add("Реальный"); break;
              case 128:  Item->SubItems->Add("Высокий"); break;
              case  32:  Item->SubItems->Add("Нормальный"); break;
              case  64:  Item->SubItems->Add("Низкий");  break;
              case  32768:  Item->SubItems->Add("Нормальный+"); break;
              case  16384:  Item->SubItems->Add("Низкий+");  break;
            }

        Item->SubItems->Add(name);//локальная группа процесса
        Item->SubItems->Add(Module.szModule); //Модуль процесса
        Item->SubItems->Add(Process.cntThreads); //кол-во потоков

        if(GetModuleFileNameEx(InfoProcess, NULL, pathP, MAX_PATH))  //путь процесса записывается в pBuf
             Item->SubItems->Add(pathP);
        else
            Item->SubItems->Add("Недоступно");

        EnumProcess = Process32Next(ProcessSnap, &Process);
        EnumModule  = Module32Next (ModuleSnap,  &Module);

        Label1->Caption=IntToStr(ColP++);
      }
    }
    CloseHandle(ProcessSnap);
    CloseHandle(ModuleSnap);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::SetPriority(DWORD PRIORITY_CLASS)
{
	bool rezult;
	int NumP;
	HANDLE PriorityProcess;
	DWORD ExitCode;
	AnsiString NameProc;


	if (ListView1->Selected)
		{
	   NumP=StrToInt(ListView1->ItemFocused->Caption);

	   NameProc= ListView1->Items->Item[ListView1->ItemIndex]->SubItems->Strings[0];
	   StatusBar1->Panels->Items[0]->Text =NameProc;
		}

	PriorityProcess = OpenProcess(THREAD_ALL_ACCESS, true, NumP);
		if (PriorityProcess)
				{
			SetPriorityClass(PriorityProcess,PRIORITY_CLASS);

				}

//CloseHandle(ProcessSnap);
CloseHandle(PriorityProcess);
}



//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
void __fastcall TForm1::CloseProcessByPID() //Завершение процесса
{

	bool rez;
	int NumProc;
	HANDLE closeProcess;
	DWORD ExitCode;
	AnsiString NameProc;

	 if (ListView1->Selected)
	  {
	NumProc=StrToInt(ListView1->ItemFocused->Caption); //Выделенный процесс в списке
	NameProc= ListView1->Items->Item[ListView1->ItemIndex]->SubItems->Strings[0];
	  }

	closeProcess = OpenProcess(PROCESS_ALL_ACCESS, true, NumProc); //открытие полного доступа к процессу
		if (closeProcess)
				{
				GetExitCodeProcess(closeProcess, &ExitCode);
				rez=TerminateProcess(closeProcess, ExitCode);//завершение процесса и его потомков
				}
		if(rez==true)
		{
			StatusBar1->Panels->Items[1]->Text ="Завершен";
		   //	ListBox1->Items->Add(IntToStr(NumProc)+" "+NameProc+" Завершен");
		}
		else
		{
			StatusBar1->Panels->Items[1]->Text ="Ошибка закрытия процесса";
		}
CloseHandle(closeProcess);
  ProcessList();
}
//---------------------------------------------------------------------------
BOOL TForm1::SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
// lookup privilege on local system. privilege to lookup. receives LUID of privilege
	if ( !LookupPrivilegeValue(NULL, lpszPrivilege, &luid ) )
	{
		AnsiString ErrorLookup=GetLastError();
		ListBox1->Items->Add("LookupPrivilegeValue error: "+ErrorLookup);
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if ( !AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES) NULL, (PDWORD) NULL) )
	{
		  AnsiString Error=GetLastError();
		  ListBox1->Items->Add("!AdjustTokenPrivileges "+Error);
		  return FALSE;
	}
	else
	{
		 AnsiString Error=GetLastError();
		 ListBox1->Items->Add("AdjustTokenPrivileges OK. code("+Error+")");
    }


	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

	{
		  ListBox1->Items->Add("The token does not have the specified privilege.");
		  return FALSE;
	}

	return TRUE;

	//SetPrivilege(hToken, lpszPrivilege, bEnablePrivilege);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	ProcessList();//функция получения выполняющихся процессов
}

//---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	CloseProcessByPID();
}

//---------------------------------------------------------------------------


void __fastcall TForm1::N1Click(TObject *Sender)
{
	ProcessList();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{

	BOOL bEnablePrivilege=TRUE;
	ChekPrivilege(bEnablePrivilege);
	ProcessList();

}

//---------------------------------------------------------------------------


void __fastcall TForm1::Button2Click(TObject *Sender)
{
	ListBox1->Visible=true;
	ListBox3->Visible=true;
	Form1->Width=775;
	Button3->Visible=true;
}

//---------------------------------------------------------------------------




BOOL TForm1::ChekPrivilege(BOOL bEnablePrivilege)
{
		HANDLE hToken;
		LPCTSTR lpszPrivilege = SE_DEBUG_NAME;


		if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
		{
			AnsiString ErrorOpenProcess=GetLastError();
			ListBox1->Items->Add("OpenProcessToken() error"+GetLastError());
			ListBox1->Items->Add("OpenProcessToken() error"+ErrorOpenProcess);

		}
		else
		{
			AnsiString ErrorOpenProcess=GetLastError();
			ListBox1->Items->Add("OpenProcessToken(OK) "+ErrorOpenProcess);
		}

		if(!SetPrivilege(hToken, lpszPrivilege, bEnablePrivilege))
			ListBox1->Items->Add("Privilege Error");
		else
			ListBox1->Items->Add("Privilege +");
		return  GetLastError();
}
//---------------------------------------------------------------------------



void __fastcall TForm1::N8Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListView1SelectItem(TObject *Sender, TListItem *Item, bool Selected)
{
	StatusBar1->Panels->Items[1]->Text =" ";
	AnsiString NameProc;
	if (ListView1->Selected)
		{
	   NameProc= ListView1->Items->Item[ListView1->ItemIndex]->SubItems->Strings[0];
	   StatusBar1->Panels->Items[0]->Text =NameProc;
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N11Click(TObject *Sender)
{
	SetPriority(REALTIME_PRIORITY_CLASS);
	StatusBar1->Panels->Items[1]->Text ="Реальный приоритет";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N12Click(TObject *Sender)
{
	SetPriority(HIGH_PRIORITY_CLASS);
	StatusBar1->Panels->Items[1]->Text ="Высокий приоритет";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N13Click(TObject *Sender)
{
	SetPriority(ABOVE_NORMAL_PRIORITY_CLASS);
	StatusBar1->Panels->Items[1]->Text ="Выше среднего";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N14Click(TObject *Sender)
{
	SetPriority(NORMAL_PRIORITY_CLASS);
	StatusBar1->Panels->Items[1]->Text ="Средний приоритет";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N15Click(TObject *Sender)
{
	SetPriority(BELOW_NORMAL_PRIORITY_CLASS);
	StatusBar1->Panels->Items[1]->Text ="Ниже среднего";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N16Click(TObject *Sender)
{
	SetPriority(IDLE_PRIORITY_CLASS);
	StatusBar1->Panels->Items[1]->Text ="Низкий приоритет";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N3Click(TObject *Sender)
{
	CloseProcessByPID();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	ListBox1->Visible=false;
	ListBox3->Visible=false;
	Form1->Width=500;
	Button3->Visible=false;
}
//---------------------------------------------------------------------------



