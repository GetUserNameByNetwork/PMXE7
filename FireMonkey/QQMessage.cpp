//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "QQMessage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmQQMessage *frmQQMessage;
//---------------------------------------------------------------------------
__fastcall TfrmQQMessage::TfrmQQMessage(TComponent* Owner,TClientBroker *AClientBroker,TQQUserRecord *AQQUserRecord)
	: TForm(Owner)
{
    FClientBroker=AClientBroker;
	FQQUserRecord=AQQUserRecord;
	FQQUserRecord->iMessageFormID=int(this);
	FChatRoomServerMethodsClient=new TChatRoomServerMethods(FClientBroker->DBXConnection,false);
	Caption=FQQUserRecord->Name;
	Timer1->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmQQMessage::SendCommang(String cmdText)
{
    switch(FQQUserRecord->UserType)
  {
	  case qutDepartment:
		FChatRoomServerMethodsClient->SendCMDToDepartment(cmdText,FQQUserRecord->ID);
	  break;
	  case qutUser:
		FChatRoomServerMethodsClient->SendCMDToUser(cmdText,FQQUserRecord->ID);
	   break;
	  case qutGroup:
		 FChatRoomServerMethodsClient->SendCMDToGroup(cmdText,FQQUserRecord->ID);
	   break;
	  default:
		FChatRoomServerMethodsClient->SendCMDToAll(cmdText);
  }
}
void __fastcall TfrmQQMessage::SetLastRecMessage(TJSONObject* Value)
{
   //�յ���Ϣ����һЩ���汾����Ϣ�ļ�����
   FLastRecMessage=Value;
   String notificationType=FLastRecMessage->Get("notificationType")->JsonValue->Value();
   if(notificationType=="message")
   {
	String sUser=FLastRecMessage->Get("fromusrname")->JsonValue->Value();
	String DateTime=FLastRecMessage->Get("datetime")->JsonValue->Value();
	String MsgText=sUser+DateTime+" ";
	memMsg->Lines->Add(MsgText);
	MsgText=FLastRecMessage->Get("message")->JsonValue->Value();
	memMsg->Lines->Add(MsgText);
//	PlaySound(L"tweet.wav",NULL,SND_ASYNC|SND_FILENAME);
   }
   else if(notificationType=="cmd")
   {
	 String cmdStr=FLastRecMessage->Get("cmd")->JsonValue->Value();
	 String DateTime=FLastRecMessage->Get("datetime")->JsonValue->Value();
	 String MsgText=FLastRecMessage->Get("fromusrname")->JsonValue->Value()+DateTime+"����:";
	 if(cmdStr=="ring")
	 {
	  //����
	  MsgText=MsgText+lbRing->Text;
	  memMsg->Lines->Add(MsgText);
	 // PlaySound(L"cmd.wav",NULL,SND_ASYNC|SND_FILENAME);
	 }
	 else if(cmdStr=="vibrate")
	 {
		 //��
		MsgText=MsgText+lbvirate->Text;
		memMsg->Lines->Add(MsgText);
		Viration();
	   //	PlaySound(L"cmd.wav",NULL,SND_ASYNC|SND_FILENAME);

	 }
   }
}
void __fastcall TfrmQQMessage::bbtSendClick(TObject *Sender)
{
  String msgText=memMsgEdit->Text.Trim();
  if(msgText==EmptyStr)
	return;
  bool resute=false;
  switch(FQQUserRecord->UserType)
  {
	  case qutDepartment:
		resute=FChatRoomServerMethodsClient->SendMessageToDepartment(msgText,FQQUserRecord->ID);
	  break;
	  case qutUser:
		resute=FChatRoomServerMethodsClient->SendMessageToUser(msgText,FQQUserRecord->ID);
	   break;
	  case qutGroup:
		 resute=FChatRoomServerMethodsClient->SendMessageToGroup(msgText,FQQUserRecord->ID);
	   break;
	  default:
		resute=FChatRoomServerMethodsClient->SendMessageToAll(msgText);
  }
  if(resute)
  {
  		if(FQQUserRecord->UserType!=qutCompany)
		{
			String MsgText=FClientBroker->UserName+Now().FormatString("YYYY-mm-DD HH:mm:ss")+" ";
			memMsg->Lines->Add(MsgText);
			memMsg->Lines->AddStrings(memMsgEdit->Lines);
		}
	    memMsgEdit->Text="";
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmQQMessage::FormClose(TObject *Sender, TCloseAction &Action)
{
  delete FChatRoomServerMethodsClient;
  FQQUserRecord->iMessageFormID=0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmQQMessage::sbVirationClick(TObject *Sender)
{
  SendCommang("vibrate");
}
//---------------------------------------------------------------------------
void __fastcall TfrmQQMessage::sbRingClick(TObject *Sender)
{
 SendCommang("ring");
}
//---------------------------------------------------------------------------
void __fastcall TfrmQQMessage::memMsgEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift)
{
  if(Key==13 && Shift.Contains(ssCtrl))
  {
	 bbtSendClick(NULL);
	 Key=0;
	 memMsgEdit->Text="";
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmQQMessage::Timer1Timer(TObject *Sender)
{
 if(FQQUserRecord->UserType==qutUser)
	{
	sbRing->Enabled=(FQQUserRecord->State==1);
	sbViration->Enabled=(FQQUserRecord->State==1);
	if(FQQUserRecord->State==1)
		Caption=FQQUserRecord->Name+"(On)";
	else
		Caption=FQQUserRecord->Name+"(Off)";
	}
	else
	{
	  Caption=FQQUserRecord->Name;
	  sbRing->Enabled=true;
	  sbViration->Enabled=true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmQQMessage::Viration()
{
  // int itop= this->Top;
 // int ileft=this->Top;
  this->Top=Top+20;
  this->Left=Left+20;
  Sleep(100);
  this->Top=Top-20;
  this->Left=Left-20;
  Sleep(100);
  this->Top=Top+20;
  this->Left=Left+20;
  Sleep(100);
  this->Top=Top-20;
  this->Left=Left-20;
}