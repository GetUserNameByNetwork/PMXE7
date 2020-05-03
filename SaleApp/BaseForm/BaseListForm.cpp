//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "BaseListForm.h"
#include "BaseBillForm.h"
#include "FSTMessageDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseForm"
#pragma resource "*.fmx"
TfrmBaseListForm *frmBaseListForm;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TfrmBaseListForm::TfrmBaseListForm(TComponent * Owner)
          :TfrmBaseForm(Owner)
{
  InitBaseListForm();
}
//---------------------------------------------------------------------------
__fastcall TfrmBaseListForm::TfrmBaseListForm(TComponent * Owner, TClientBroker * clBroker,
						int ModuleCode, String Param)
          :TfrmBaseForm(Owner, clBroker, ModuleCode, Param)
{
  InitBaseListForm();
  InitDevLocalize();
}
//---------------------------------------------------------------------------
__fastcall TfrmBaseListForm::TfrmBaseListForm(TComponent * Owner,TClientBroker * clBroker,
							 int ModuleCode, TZClientDataSet * FDataSet, String Param)
		  :TfrmBaseForm(Owner, clBroker,ModuleCode,FDataSet, Param)
{
  InitBaseListForm();
  InitDevLocalize();
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::InitBaseListForm()
{
  RelaFormSet=new TCZDataSet();
  RelaFormSet->AddField("Caption");
  RelaFormSet->AddField("ModuleCode");
  RelaFormSet->AddField("ImageIndex");
  RelaFormSet->AddField("CreateStyle");

  //���ô����ʼ״̬
  if(DataSet != NULL)
  {
    SetOptCtrlStatus(0);
  }
}
//---------------------------------------------------------------------------
__fastcall TfrmBaseListForm::~TfrmBaseListForm()
{
  delete RelaFormSet;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::ShowEditForm(int status)
{
  //������״̬��������޸Ĳ���ʱ
  if(status != 1)
  {
	if(RecordCount > 0)
    {
      if(Eof)
        DS_MoveLast();
	  else if(Bof)
		DS_MoveFirst();
    }
    else
	{
	  ShowMessage(_D("��ǰû�����ݣ��޷�ִ�в���"));
      return;
    }
  }
  //��������ʾ���ݱ༭����
	  TfrmBaseBillForm *EditForm;
	  try
	  {
		TForm *p = NewEditForm(DataSet, status);  //��ȡ�༭����ָ��
		if(p)
		{
			EditForm =dynamic_cast<TfrmBaseBillForm *>(p); //����ת��
			if(EditForm)
			{   //�����������У���Ҫ��Ȩ�����ݻ�ȡ̫�����Ѿ��ڹ��캯�����д���
				//����moduleCode��һ���Խ��Ȩ�޺ʹ���IDȡֵ
			   //	EditForm->FormModuleCode = FormModuleCode;
			   //	EditForm->FormID=this->FormID;
				EditForm->SetFormStatus(status); //���ñ༭�����ʼ״̬
				EditForm->Show();
			}
		}
		else
		 ShowMessage(L"δʵ�ֱ༭����!");
	  }
	 catch(Exception &e)
	  {
	   throw Exception(e.Message);
      }

}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::BrowseRecord()
{
  if(RecordCount > 0)
  {
    if(BeforeUpdateData((int)otBrowse) == true)
    {
      ShowEditForm(0);
    }
  }
  else
  ShowMessage(_D("�޿ɲ鿴����"));
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::AddData()
{
  if(BeforeUpdateData((int)otAdd) == true)
  {
     ShowEditForm(1);
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::EditData()
{
  if(RecordCount > 0)
  {
    if(BeforeUpdateData((int)otEdit) == true)
    {
      ShowEditForm(2);
    }
  }
  else
	 ShowMessage(_D("�޿ɱ༭����"));
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::DelData()
{
  if(RecordCount > 0)
  {
    if(BeforeUpdateData((int)otDel) == true)
	{
	  TMessageDialogEventProc msgDialogCloseProc=&DoDelData;
	  ShowMessageBox(_D("�Ƿ�Ҫɾ����ǰ����") ,msgDialogCloseProc);
 	}
  }
  else
	ShowMessage(_D("�޿�ɾ������"));
}
void __fastcall TfrmBaseListForm::DoDelData(System::Uitypes::TModalResult AResult)
{
  if(AResult==mrYes)
  {
		 DS_Delete();
		 if(DataSet != NULL)
		 {
			SetOptCtrlStatus(0);
			FormFillData();
		 }
  }
}
//---------------------------------------------------------------------------
TForm * __fastcall TfrmBaseListForm::NewEditForm(TZClientDataSet * m_DataSet, int status)
{
  return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::DoCheckBillProc(System::Uitypes::TModalResult AResult)
{
  if(AResult==mrYes)
  {
		DS_Edit();
		if(BeforeCheck())
		{
		  CheckBill();
		  AfterCheck();
		  SetOptCtrlStatus(0);
		}
	else
	  DS_Cancel();
  }
}
void __fastcall TfrmBaseListForm::DoUnCheckBillProc(System::Uitypes::TModalResult AResult)
{
  if(AResult==mrYes)
  {
      DS_Edit();
	  if(BeforeUnCheck())
	  {
		UnCheckBill();
		AfterUnCheck();
		SetOptCtrlStatus(0);
	  }
	  else
		DS_Cancel();

  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::CheckBillData()
{
  TMessageDialogEventProc msgDialogCloseProc=&DoCheckBillProc;
  ShowMessageBox(_D("�Ƿ�Ҫ��˵�ǰ���ݣ�"),msgDialogCloseProc);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::CheckBill()
{
  DS_Check(1);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::UnCheckBillData()
{
  TMessageDialogEventProc msgDialogCloseProc=&DoUnCheckBillProc;
  ShowMessageBox(_D("�Ƿ�Ҫ����˵�ǰ���ݣ�"),msgDialogCloseProc);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::UnCheckBill()
{
  DS_Check(0);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::RefreshDataSet()
{
	TDataSetNotifyEvent m_AfterScroll = DataSet->MasterDataSet->AfterScroll;
	try
	{
		DataSet->MasterDataSet->DisableControls();
		DataSet->MasterDataSet->AfterScroll = NULL;
		DS_Close();
		DS_Open();
		FormFillData();
		if(RecordCount > 0) DS_MoveFirst();
		SetOptCtrlStatus(0);
	}
	__finally
	{
		DataSet->MasterDataSet->AfterScroll = m_AfterScroll;
		DataSet->MasterDataSet->EnableControls();

		if(m_AfterScroll) //����һ�κõ�
			m_AfterScroll(DataSet->MasterDataSet);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::RefreshData(String Filters)
{
	MasterWhereString=Filters;
	RefreshDataSet();
}
//---------------------------------------------------------------------------
bool __fastcall  TfrmBaseListForm::LocateRecord(String KeyValues)
{
  bool retval;
  retval = DS_LocateMaster(KeyValues);
  SetOptCtrlStatus(0);
  return retval;
}
TForm * __fastcall TfrmBaseListForm::NewFilterForm(TOnFilteredDataSet AOnFilteredDataSet,TClientBroker * ABroker)
{
   return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::QueryData()
{
	  TOnFilteredDataSet  FileredDataSet=&RefreshData;
	  TForm* qp=NewFilterForm(FileredDataSet,ClientBroker);
	  if(qp)   //�Զ����ѯ����
		qp->Show();
	  else   //û�й��˹���
        RefreshDataSet();

}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseListForm::GetDetailValue(String fieldname)
{
  return DS_GetDetailValue(fieldname);
}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseListForm::GetDetailOldValue(String fieldname)
{
  return DS_GetDetailOldValue(fieldname);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::SetDetailValue(String fieldname, Variant value)
{
  DS_SetDetailValue(fieldname, value);
}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseListForm::GetFieldValue(String fieldname)
{
  return DS_GetMasterValue(fieldname);
}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseListForm::GetFieldOldValue(String fieldname)
{
  return DS_GetMasterOldValue(fieldname);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::SetFieldValue(String fieldname, Variant value)
{
  DS_SetMasterValue(fieldname, value);
}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseListForm::GetMasterValue(String Mastername)
{
  return DS_GetMasterValue(Mastername);
}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseListForm::GetMasterOldValue(String Mastername)
{
  return DS_GetMasterOldValue(Mastername);
}
//---------------------------------------------------------------------------
void __fastcall  TfrmBaseListForm::SetMasterValue(String Mastername, Variant value)
{
  DS_SetMasterValue(Mastername, value);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::AddNavItem(String sCaption,int iCode,
                                             int iStyle,int iImageIndex)
{
  RelaFormSet->AddNew();
  RelaFormSet->FieldValues["Caption"]=sCaption;
  RelaFormSet->FieldValues["ModuleCode"]=IntToStr(iCode);
  RelaFormSet->FieldValues["ImageIndex"]=IntToStr(iImageIndex);
  RelaFormSet->FieldValues["CreateStyle"]=IntToStr(iStyle);
  RelaFormSet->Update();
}
//---------------------------------------------------------------------------
//��ʾ���ҵ����
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::ShowRelationForm(int ModuleCode)
{
  String ParamStr = GetRelaFormParam(ModuleCode);   //��ȡ��������
//  ShowClientFormModal(ClientCommHandle,ModuleCode,ParamStr); //�Զ������ҵ����
  AfterRelaFormModal(ModuleCode);                       //�ر����ҵ�����Ĵ���
}
//---------------------------------------------------------------------------
//��ʾ�������
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::ShowNavPane(bool bShowPane)
{
 /* QuickPane->Clear();

  int iCode=0,iImageIndex;
  bool IsGroupCreate=false;

  RelaFormSet->First();   //���Ӹ�������
  while(!RelaFormSet->Eof)
  {
    if(RelaFormSet->FieldValues["createstyle"] == "2")
    {
      if(RelaFormSet->FieldValues["ModuleCode"] != "")
        iCode = StrToInt(RelaFormSet->FieldValues["ModuleCode"]);
      else
        iCode = 0;
      if(RelaFormSet->FieldValues["ImageIndex"] != "")
        iImageIndex = StrToInt(RelaFormSet->FieldValues["ImageIndex"]);
      else
        iImageIndex = -1;
      if(iImageIndex == -1)
        iImageIndex = 57; //Ĭ��ͼ��

      //����NavItem
      if(!IsGroupCreate)
      {
        QuickPane->AddGroup("�����б�",0,2);
        IsGroupCreate = true;
      }
      QuickPane->AddItem(QuickPane->GroupCount-1,RelaFormSet->FieldValues["Caption"],
                           iImageIndex,iCode);
    }
    RelaFormSet->Next();
  }

  IsGroupCreate = false;
  RelaFormSet->First();   //�������ҵ��˵���
  while(!RelaFormSet->Eof)
  {
    if(RelaFormSet->FieldValues["createstyle"] == "1" || RelaFormSet->FieldValues["createstyle"] == "3")
    {
      if(RelaFormSet->FieldValues["ModuleCode"] != "")
        iCode = StrToInt(RelaFormSet->FieldValues["ModuleCode"]);
      else
        iCode = 0;
      if(RelaFormSet->FieldValues["ImageIndex"] != "")
        iImageIndex = StrToInt(RelaFormSet->FieldValues["ImageIndex"]);
      else
        iImageIndex = -1;
      if(iImageIndex == -1)
        iImageIndex = 41; //Ĭ��ͼ��

      //����NavItem
      if(!IsGroupCreate)
      {
        QuickPane->AddGroup("ҵ����",0,1);
        IsGroupCreate = true;
      }
      QuickPane->AddItem(QuickPane->GroupCount-1,RelaFormSet->FieldValues["Caption"],
                           iImageIndex,iCode);
    }
    RelaFormSet->Next();
  }

  IsGroupCreate = false;
  RelaFormSet->First();   //����ϵͳ���ܲ˵���
  while(!RelaFormSet->Eof)
  {
    if(RelaFormSet->FieldValues["createstyle"] == "0")
    {
      if(RelaFormSet->FieldValues["ModuleCode"] != "")
        iCode = StrToInt(RelaFormSet->FieldValues["ModuleCode"]);
      else
        iCode = 0;
      if(RelaFormSet->FieldValues["ImageIndex"] != "")
        iImageIndex = StrToInt(RelaFormSet->FieldValues["ImageIndex"]);
      else
        iImageIndex = -1;
      if(iImageIndex == -1)
        iImageIndex = 7; //Ĭ��ͼ��

      //����NavItem
      if(!IsGroupCreate)
      {
        QuickPane->AddGroup("ϵͳ����",0,0);
        IsGroupCreate = true;
      }
      QuickPane->AddItem(QuickPane->GroupCount-1,RelaFormSet->FieldValues["Caption"],
                           iImageIndex,iCode);
    }
    RelaFormSet->Next();
  }
  if (bShowPane) // ��Ҫ��ʾ
  {
    QuickPane->DockPane();
    QuickPane->ShowPane();
  }
  */
}
//---------------------------------------------------------------------------
