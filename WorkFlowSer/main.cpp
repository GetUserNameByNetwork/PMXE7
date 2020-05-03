//---------------------------------------------------------------------------
#include "main.h"
#include <registry.hpp>
#include "DecEnc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//��ʵ��ע�������� WorkFlowSer -install -instance=InstName;
//����һ��Ӧ�ó������ע�������������ƣ�ע����������Ŷ˿ڲ�Ҫ��ͻ
//һ����ñ��ݵ���ͬĿ¼���޸Ķ˿ڲ�����Ȼ��Ž��з���ע��
TXZWorkFlowSerXE *XZWorkFlowSerXE;
//---------------------------------------------------------------------------
__fastcall TXZWorkFlowSerXE::TXZWorkFlowSerXE(TComponent* Owner)
	: TService(Owner)
{
	 FClientBroker=new TwfClientBroker(this);
	 FInstanceName="";
	 DisplayName="Foresight Workflow Server";
}

TServiceController __fastcall TXZWorkFlowSerXE::GetServiceController(void)
{
	return (TServiceController) ServiceController;
}

void __stdcall ServiceController(unsigned CtrlCode)
{
	XZWorkFlowSerXE->Controller(CtrlCode);
}
//---------------------------------------------------------------------------
void __fastcall TXZWorkFlowSerXE::ServiceStart(TService *Sender, bool &Started)
{
    FClientBroker->ReadOption();
	Timer1->Enabled=true;
	Started=true;
}
//---------------------------------------------------------------------------
void __fastcall TXZWorkFlowSerXE::ServiceStop(TService *Sender, bool &Stopped)
{
	Timer1->Enabled=false;
	FClientBroker->UnRegCallBack();
	Stopped=true;

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TXZWorkFlowSerXE::ServiceDestroy(TObject *Sender)
{
	delete FClientBroker;
   //	OleUninitialize();
}
//---------------------------------------------------------------------------

void __fastcall TXZWorkFlowSerXE::ServiceAfterInstall(TService *Sender)
{
	TRegistry *reg;
	reg = new TRegistry();
	reg->RootKey = HKEY_LOCAL_MACHINE;
	try
	{
		reg->OpenKey("SYSTEM",false);
		reg->OpenKey("CurrentControlSet",false);
		reg->OpenKey("Services",false);
		reg->OpenKey(Name,true);
		reg->WriteString("DisplayName",DisplayName);
		reg->WriteString("Description","����������������̨����,ʵ��Ϊ"+Name);
		reg->CloseKey();
	}
	__finally
	{
		reg->Free();
	}
}
//---------------------------------------------------------------------------
void __fastcall TXZWorkFlowSerXE::ChangeServiceConfiguration()
{
  SC_HANDLE mngr;
  SC_HANDLE svc;
  String newpath;
  // �򿪷��������
  mngr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
  if (mngr == 0)
	RaiseLastOSError();
  try
  {
	// �򿪷���
	svc = OpenService(mngr, Name.c_str(), SERVICE_CHANGE_CONFIG);
	if (svc == 0)
	  RaiseLastOSError();
	try
	{
	  // �ı�����ִ�в���
	  newpath = ParamStr(0) + " " + INSTANCE_SWITCH+FInstanceName; // + any other cmd line params you fancy
	  ChangeServiceConfig(svc, SERVICE_NO_CHANGE, //  dwServiceType
							   SERVICE_NO_CHANGE, //  dwStartType
							   SERVICE_NO_CHANGE, //  dwErrorControl
							   newpath.c_str(),    //  <-- ֻҪ�ı�ִ��·���Ͳ���
							   NULL,               //  lpLoadOrderGroup
							   NULL,               //  lpdwTagId
							   NULL,               //  lpDependencies
							   NULL,               //  lpServiceStartName
							   NULL,               //  lpPassword
							   DisplayName.c_str());              //  lpDisplayName
	}
	__finally
	{
	  CloseServiceHandle(svc);
	}
  }
  __finally
  {
	CloseServiceHandle(mngr);
  }
}
void __fastcall TXZWorkFlowSerXE::SetInstanceName(String Value)
{
   if (FInstanceName != Value)
  {
	FInstanceName = Value;
	if (FInstanceName != "")
	{
	  Name = FInstanceName;
	  DisplayName = "Foresight Workflow Service("+FInstanceName+")";
	}
  }
}

void __fastcall TXZWorkFlowSerXE::ServiceBeforeInstall(TService *Sender)
{
  String inst=GetInstanceName();
  if(inst>" ")
	 InstanceName=inst;
}
//---------------------------------------------------------------------------

void __fastcall TXZWorkFlowSerXE::ServiceBeforeUninstall(TService *Sender)
{
  String inst=GetInstanceName();
  if(inst>" ")
	 InstanceName=inst;
}
//---------------------------------------------------------------------------

void __fastcall TXZWorkFlowSerXE::ServiceCreate(TObject *Sender)
{
  String inst=GetInstanceName();
  if(inst>" ")
	 InstanceName=inst;
}
//---------------------------------------------------------------------------
String __fastcall TXZWorkFlowSerXE::GetInstanceName()
{
  String result = "";
  int PaCount=System::ParamCount();
  for(int index = 1; index<=PaCount; index++)
  {
	String paramString=ParamStr(index);
	if (SameText(INSTANCE_SWITCH, paramString.SubString(1,INSTANCE_SWITCH.Length())))
	{
	  result =paramString.SubString(INSTANCE_SWITCH.Length() + 1, MaxInt);
	  break;
	}
  }
 return result;
}
void __fastcall TXZWorkFlowSerXE::Timer1Timer(TObject *Sender)
{
  if(!FClientBroker->Connected)
  {
   if(FClientBroker->InitClientComm())
	  FClientBroker->RegCallBack();
  }
}
//---------------------------------------------------------------------------
