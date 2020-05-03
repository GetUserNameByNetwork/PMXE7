//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditServer.h"
#include <System.SysUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmEditServer *frmEditServer;
//---------------------------------------------------------------------------
__fastcall TfrmEditServer::TfrmEditServer(TComponent* Owner,TListItem *AItem,int Status)
	: TForm(Owner)
{
  FItem= AItem;
  ModifStatus=Status;
  GetDataFromItem(FItem);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditServer::SpeedButton1Click(TObject *Sender)
{
  if(edFileName->Text.Trim()>" ")
	OpenDialog1->FileName=edFileName->Text;
  if(OpenDialog1->Execute(this->Handle))
   edFileName->Text= OpenDialog1->FileName;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditServer::GetDataFromItem(TListItem *AItem)
{
  edServerName->Text=FItem->Caption;
  edFileName->Text=FItem->SubItems->Strings[0];
  edDisplay->Text=FItem->SubItems->Strings[1];
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditServer::BitBtn1Click(TObject *Sender)
{
 if(edServerName->Text.Trim()=="")
 {
	 ShowMessage("���������������");
	 edServerName->SetFocus();
	 return;
 }
 if(edFileName->Text.Trim()=="")
 {
	 ShowMessage("��ѡ��ִ���ļ�");
	 edFileName->SetFocus();
	 return;
 }
 else if(!FileExists(edFileName->Text))
 {
	 ShowMessage("���ļ������ڣ�");
	 edFileName->SetFocus();
	 return;
 }
 if(edDisplay->Text.Trim()=="")
 {
	 ShowMessage("�������������ע");
	 edDisplay->SetFocus();
	 return;
 }
   FItem->Caption=edServerName->Text.Trim();
   FItem->SubItems->Strings[0]=edFileName->Text;
   FItem->SubItems->Strings[1]=edDisplay->Text;
   ModalResult=mrOk;
}
//---------------------------------------------------------------------------