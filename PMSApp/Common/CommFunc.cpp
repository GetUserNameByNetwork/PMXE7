//---------------------------------------------------------------------------

#pragma hdrstop

#include "CommFunc.h"
#include "BaseGlobeVars.h"
#include "APIbase.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//ȡ��ͬ���ĵ�Ȩ��ʱ �����Զ�ȡ��ӦȨ�� �Ͷ�Ӧ Ŀ¼��Ȩ��  �����Ȩ��

int __fastcall GetDataRight(TZClientQuery *Query,const String &businessCode,TBusinessType businessType,String userGUID)
{
	int returnValue = 0;

	if(userGUID.IsEmpty())
		userGUID = GetGlobeVarValue("UserCode").UpperCase(); //Ա������

	switch (businessType)
	{
		case BtProject: // ��ĿȨ��
			Query->Close();
			Query->SQL->Text = "select 1 from tbProject where PrjPKID='"+businessCode+"' and PrjCreator='"+userGUID+"'";
			Query->Open();
			if(Query->RecordCount > 0)
				return 2;

			Query->Close();
			Query->SQL->Clear();
			Query->SQL->Text = " select SysDROpteType from vi_GetDataRight"
												 " where REEmpPK_FKID='"+userGUID+"' and SysDRDataID='"+businessCode+"' and SysDROrigin="+IntToStr(businessType)+
												 " order by SysDROpteType Desc";
			Query->Open();
			if(!Query->Eof)
				returnValue = Query->FieldByName("SysDROpteType")->AsInteger;
			Query->Close();
			break;
		case BtInviteProject: // �б���ĿȨ��
			Query->Close();
			Query->SQL->Text = "select 1 from tbBidProject where BidPrjPKID='"+businessCode+"' and BidPrjCreator='"+userGUID+"'";
			Query->Open();
			if(Query->RecordCount > 0)
				return 2;

			Query->Close();
			Query->SQL->Clear();
			Query->SQL->Text = " select SysDROpteType from vi_GetDataRight"
												 " where REEmpPK_FKID='"+userGUID+"' and SysDRDataID='"+businessCode+"' and SysDROrigin="+IntToStr(businessType)+
												 " order by SysDROpteType Desc";
			Query->Open();
			if(!Query->Eof)
				returnValue = Query->FieldByName("SysDROpteType")->AsInteger;
			Query->Close();
			break;
		case BtContractMenu: // ��ͬĿ¼Ȩ��
			Query->Close();
			Query->SQL->Text = "select 1 from tbContType where CTypePKID='"+businessCode+"' and CTypeCreator='"+userGUID+"'";
			Query->Open();
			if(Query->RecordCount > 0)
				return 2;

			Query->Close();
			Query->SQL->Clear();
			Query->SQL->Text = " select SysDROpteType from vi_GetDataRight"
												 " where REEmpPK_FKID='"+userGUID+"' and SysDRDataID='"+businessCode+"' and SysDROrigin="+IntToStr(businessType)+
												 " order by SysDROpteType Desc";
			Query->Open();
			if(!Query->Eof)
				returnValue = Query->FieldByName("SysDROpteType")->AsInteger;
			Query->Close();
			break;
		case BtContract: // ��ͬȨ��
		{
			String contractMenu;
			Query->Close();
			Query->SQL->Text = "select ContType from tbContract where ContPKID='"+businessCode+"' and ContCreator='"+userGUID+"'";
			Query->Open();
			if(Query->RecordCount > 0)
			{
				contractMenu = Query->FieldByName("ContType")->AsString;
				return 2;
			}

			Query->Close();
			Query->SQL->Clear();
			Query->SQL->Text = " select SysDROpteType from vi_GetDataRight"
												 " where REEmpPK_FKID='"+userGUID+"' and SysDRDataID='"+businessCode+"' and SysDROrigin="+IntToStr(businessType)+
												 " order by SysDROpteType Desc";
			Query->Open();
			if(!Query->Eof)
				returnValue = Query->FieldByName("SysDROpteType")->AsInteger;
			Query->Close();

			if(returnValue < 2)
			{
				int tempValue = 0;
				Query->Close();
				Query->SQL->Text = " select SysDROpteType from vi_GetDataRight"
													 " where REEmpPK_FKID='"+userGUID+"' and SysDRDataID='"+contractMenu+"' and SysDROrigin="+IntToStr(BtContractMenu)+
													 " order by SysDROpteType Desc";
				Query->Open();
				Query->First();
				if(!Query->Eof)
					tempValue = Query->FieldByName("SysDROpteType")->AsInteger;
				Query->Close();
				tempValue > returnValue ? returnValue = tempValue : returnValue;
      }
			break;
		}
		case BtdocumentMenu: // �ĵ�Ŀ¼Ȩ��
			Query->Close();
			Query->SQL->Text = "select 1 from tbDocContent where DocCttPKID='"+businessCode+"' and DocCttCreator='"+userGUID+"'";
			Query->Open();
			if(Query->RecordCount > 0)
				return 2;

			Query->Close();
			Query->SQL->Clear();
			Query->SQL->Text = " select SysDROpteType from vi_GetDataRight"
												 " where REEmpPK_FKID='"+userGUID+"' and SysDRDataID='"+businessCode+"' and SysDROrigin="+IntToStr(businessType)+
												 " order by SysDROpteType Desc";
			Query->Open();
			if(!Query->Eof)
				returnValue = Query->FieldByName("SysDROpteType")->AsInteger;
			Query->Close();
			break;
		case Btdocument: // �ĵ�Ȩ��
		{
			String documentMenu;
			Query->Close();
			Query->SQL->Text = "select DocFileFKID_Cls from tbDocFile where DocFilePKID='"+businessCode+"' and DocFileCreator='"+userGUID+"'";
			Query->Open();
			if(Query->RecordCount > 0)
			{
				documentMenu = Query->FieldByName("DocFileFKID_Cls")->AsString;
				return 2;
			}

			Query->Close();
			Query->SQL->Clear();
			Query->SQL->Text = " select SysDROpteType from vi_GetDataRight"
												 " where REEmpPK_FKID='"+userGUID+"' and SysDRDataID='"+businessCode+"' and SysDROrigin="+IntToStr(businessType)+
												 " order by SysDROpteType Desc";
			Query->Open();
			if(!Query->Eof)
				returnValue = Query->FieldByName("SysDROpteType")->AsInteger;
			Query->Close();

			if(returnValue < 2)
			{
				int tempValue = 0;
				Query->Close();
				Query->SQL->Text = " select SysDROpteType from vi_GetDataRight"
													 " where REEmpPK_FKID='"+userGUID+"' and SysDRDataID='"+documentMenu+"' and SysDROrigin="+IntToStr(BtdocumentMenu)+
													 " order by SysDROpteType Desc";
				Query->Open();
				Query->First();
				if(!Query->Eof)
					tempValue = Query->FieldByName("SysDROpteType")->AsInteger;
				Query->Close();
				tempValue > returnValue ? returnValue = tempValue : returnValue;
      }
			break;
		}
		case BtSign: // ����ӡ��Ȩ��
		{
			String SignPKID;
			Query->Close();
			Query->SQL->Text = "select SignPKID from tbSign where SignPKID='"+businessCode+"' and SignCreator='"+userGUID+"'";
			Query->Open();
			if(Query->RecordCount > 0)
			{
				SignPKID = Query->FieldByName("SignPKID")->AsString;
				return 2;
			}

			Query->Close();
			Query->SQL->Clear();
			Query->SQL->Text = " select SysDROpteType from vi_GetDataRight"
												 " where REEmpPK_FKID='"+userGUID+"' and SysDRDataID='"+businessCode+"' and SysDROrigin="+IntToStr(businessType)+
												 " order by SysDROpteType Desc";
			Query->Open();
			if(!Query->Eof)
				returnValue = Query->FieldByName("SysDROpteType")->AsInteger;
			Query->Close();

			if(returnValue < 2)
			{
				int tempValue = 0;
				Query->Close();
				Query->SQL->Text = " select SysDROpteType from vi_GetDataRight"
													 " where REEmpPK_FKID='"+userGUID+"' and SysDRDataID='"+SignPKID+"' and SysDROrigin="+IntToStr(BtSign)+
													 " order by SysDROpteType Desc";
				Query->Open();
				Query->First();
				if(!Query->Eof)
					tempValue = Query->FieldByName("SysDROpteType")->AsInteger;
				Query->Close();
				tempValue > returnValue ? returnValue = tempValue : returnValue;
      }
			break;
		}
		default:
			break;
	}
	return  returnValue;
}

//--------------------------------------------------------------------------
String __fastcall GetDataRightString(TZClientQuery *Query,TBusinessType businessType,String userGUID)
{
	String returnValue;
	if(userGUID.IsEmpty())
	userGUID =  GetGlobeVarValue("UserCode"); //Ա������

	switch(businessType)
	{
		case BtProject:  // ��Ŀ
			returnValue = "PrjCreator='"+userGUID+"' or PrjPKID";
			break;
		case BtContract:  //��ͬ
			returnValue = "ContCreator='"+userGUID+"' or ContPKID";
			break;
		case Btdocument:  //�ĵ�
			returnValue = "DocFileCreator='"+userGUID+"' or DocFilePKID";
			break;
		case BtContractMenu: //��ͬĿ¼
			returnValue = "CTypeCreator='"+userGUID+"' or CTypePKID";
			break;
		case BtdocumentMenu: //�ĵ�Ŀ¼
			returnValue = "DocCttCreator='"+userGUID+"' or DocCttPKID";
			break;
		case BtInviteProject: //�б���Ŀ
			returnValue = "BidPrjCreator='"+userGUID+"' or BidPrjMtnPKID";
			break;
		case BtBalance: //������Ŀ
			returnValue = "CalSubCreator='"+userGUID+"' or CalSubPKID";
			break;
		case BtSign:  // ����ӡ��
			returnValue = "signCreator='"+userGUID+"' or SignPKID";
			break;
		default: //Ĭ�� ��Ŀ
			returnValue = "PrjPKID";
			break;
	}

	returnValue += " in (select SysDRDataID from vi_GetDataRight where REEmpPK_FKID='"+userGUID+"' and (SysDROrigin="+IntToStr(businessType)+"))";
	if(businessType == BtContract)
		returnValue += " or ContType  in (select SysDRDataID from vi_GetDataRight where REEmpPK_FKID='"+userGUID+"' and (SysDROrigin= 4))";
	else if(businessType == Btdocument)
		returnValue += " or DocFileFKID_Cls  in (select SysDRDataID from vi_GetDataRight where REEmpPK_FKID='"+userGUID+"' and (SysDROrigin=5))";
	return returnValue;
}
//--------------------------------------------------------------------------
String __fastcall GetGlobeVarValue(String VarName)
{
  static String sysGlobeVarStr="";
  static String sysQtyPrecision;
  static String sysPricePrecision;
  static String sysAmtPrecision;
  static String sysRatePrecision;
  static String sysFloatFormatStyle;
  try
  {
	if(GlobeVarSet == NULL)
      GlobeVarSet = new TCZDataSet;
    if(sysGlobeVarStr != gGlobeVarsStr)
    {
      GlobeVarSet->LoadFromString(gGlobeVarsStr); //����װ��ȫ�ֱ���
      sysGlobeVarStr = gGlobeVarsStr;
      if(GlobeVarSet->RecordCount < 1)
		throw Exception(L"δ����ϵͳȫ�ֱ���");
      GlobeVarSet->First();
      //����ϵͳ���㾫�Ȳ��������ת��Ч��
      sysQtyPrecision = GlobeVarSet->FieldValues["QtyPrecision"];
      sysPricePrecision = GlobeVarSet->FieldValues["PricePrecision"];
      sysAmtPrecision = GlobeVarSet->FieldValues["AmtPrecision"];
      sysRatePrecision = GlobeVarSet->FieldValues["RatePrecision"];
      sysFloatFormatStyle = GlobeVarSet->FieldValues["FloatFormatStyle"];
    }
    if(GlobeVarSet->RecordCount < 1)
	  throw Exception(L"δ����ϵͳȫ�ֱ���");
    else if(UpperCase(VarName) == UpperCase("QtyPrecision"))
      return sysQtyPrecision;
    else if(UpperCase(VarName) == UpperCase("PricePrecision"))
      return sysPricePrecision;
    else if(UpperCase(VarName) == UpperCase("AmtPrecision"))
      return sysAmtPrecision;
    else if(UpperCase(VarName) == UpperCase("RatePrecision"))
      return sysRatePrecision;
    else if(UpperCase(VarName) == UpperCase("FloatFormatStyle"))
      return sysFloatFormatStyle;
    else if(UpperCase(VarName)=="CURDATE")
      return FormatDateTime("YYYY-MM-DD",Date());
    else if (UpperCase(VarName)==UpperCase("CurDateTime"))
      return FormatDateTime("YYYY-MM-DD HH:MM:SS",Now());
    else
      return GlobeVarSet->FieldValues[VarName];
  }
  catch(Exception &e)
  {
	throw Exception(L"��ȡϵͳȫ�ֱ�������"+e.Message);
  }
}
String __fastcall GetGUIDString()
{
    TGUID BillID;
	CreateGUID(BillID);
	return GUIDToString(BillID);
}
//�Զ���ĸ���ת��Ϊ�ַ��������������ѧ������
//---------------------------------------------------------------------------
String __fastcall myFloatToStr(double data)
{
  int iPoint,iExponent;
  String datastring,tmpstr,tmpstr2;
  datastring = FloatToStr(data); //���ñ�׼ת��
  if ((iPoint = datastring.Pos("E")) <= 0) //������ǿ�ѧ��������ֱ�ӷ����ַ���
    return datastring;
  tmpstr = datastring.SubString(iPoint+1,datastring.Length()-iPoint);
  if (tmpstr == "")
    iExponent = 0;
  else
    iExponent = StrToInt(tmpstr); //��ȡָ��λ
  tmpstr = datastring.SubString(1,iPoint-1); //��ȡ����

  if (iExponent > 0)  //ָ��������
  {
    if ((iPoint = tmpstr.Pos(".")) <= 0) //������С��
    {
      for (int i=0;i<iExponent;i++)
       tmpstr += "0";
    }
    else                                 //������С��
    {
      tmpstr2 = tmpstr;
      tmpstr = tmpstr2.SubString(1,iPoint-1); //��ȡ��������
      tmpstr2 = tmpstr2.SubString(iPoint+1,tmpstr2.Length()-iPoint); //��ȡС������
      if (tmpstr2.Length() > iExponent)
      {
        tmpstr += tmpstr2.SubString(1,iExponent);
        tmpstr += ".";                        //��λ������С����
        tmpstr += tmpstr2.SubString(iExponent+1,tmpstr2.Length());
      }
      else
      {
        tmpstr += tmpstr2;
        for (int i=0;i<iExponent-tmpstr2.Length();i++)
         tmpstr += "0";
      }
    }
  }
  if (iExponent < 0)  //ָ���Ǹ���
  {
    String sFlag="";
    iExponent = iExponent*-1; //ȡ�෴��
    if (tmpstr.SubString(1,1) == "+" || tmpstr.SubString(1,1) == "-") //��ȡλ����������
    {
      sFlag = tmpstr.SubString(1,1);
      tmpstr = tmpstr.SubString(2,tmpstr.Length()-1);
    }
    tmpstr2 = tmpstr;
    tmpstr = "";
    if ((iPoint = tmpstr2.Pos(".")) > 0) //������С��
    {
      tmpstr = tmpstr2.SubString(iPoint+1,tmpstr2.Length()-iPoint); //��ȡС������
      tmpstr2 = tmpstr2.SubString(1,iPoint-1); //��ȡ��������
    }
    if (tmpstr2.Length() > iExponent)
    {
      tmpstr = tmpstr2.SubString(iExponent+1,tmpstr2.Length()) + tmpstr;
      tmpstr = "." + tmpstr;                        //��λ������С����
      tmpstr = tmpstr2.SubString(1,iExponent) + tmpstr;
    }
    else
    {
      tmpstr = tmpstr2 + tmpstr;
      for (int i=0;i<iExponent-tmpstr2.Length();i++)
       tmpstr = "0" + tmpstr;
      tmpstr = "0." + tmpstr;
    }
    tmpstr = sFlag + tmpstr; //���Ϸ���
  }
  return tmpstr;
}
//---------------------------------------------------------------------------
//��ʽ�������������ַ������
//����data-��������iType-�������ͣ�ftQty:������ftPrice:���ۣ�ftAmt:���,ftRate:����ϵ����
//iStyle-С����ȡ��ʽ��0:�������룬1:�ضϣ�iPrecision-С�����ȣ�-1:ȡϵͳ����
//---------------------------------------------------------------------------
double __fastcall ConvertFloat(double data,TfstFloatType iType,int iPrecision,int iStyle)
{
   //if(data >= 1E12)
   //   throw Exception("������ֵλ������");
   String datastring=myFloatToStr(data);
   int iPoint=datastring.Pos("."); //С����λ��
   if(iPoint <= 0)
     return data;  //��С��ֱ�ӷ���

   int iPrec,iFormatStyle;
   //ȷ������
   if (iPrecision < 0) //ȡϵͳ����
   {
     switch(iType)
     {
       case ftQty:      //Qty Float
         if(GetGlobeVarValue("QtyPrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("QtyPrecision"));
         else
           iPrec = 2;
         break;
       case ftPrice:      //Price Float
         if(GetGlobeVarValue("PricePrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("PricePrecision"));
         else
           iPrec = 2;
         break;
       case ftAmt:      //Amt Float
         if(GetGlobeVarValue("AmtPrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("AmtPrecision"));
         else
           iPrec = 4;
         break;
       case ftRate:      //rate Float
         if(GetGlobeVarValue("RatePrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("RatePrecision"));
         else
           iPrec = 4;
         break;
       case ftsArea:      //Area Float
         if(GetGlobeVarValue("sAreaPrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("sAreaPrecision"));
         else
           iPrec = 2;
         break;
       case ftsPrice:      //Area Float
         if(GetGlobeVarValue("sPricePrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("sPricePrecision"));
         else
           iPrec = 2;
         break;
       case ftsAmt:      //Area Float
         if(GetGlobeVarValue("sAmtPrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("sAmtPrecision"));
         else
           iPrec = 4;
         break;
       case ftsCJPrice:      //Area Float
         if(GetGlobeVarValue("sCJPricePrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("sCJPricePrecision"));
         else
           iPrec = 2;
         break;
       case ftsCJTotalPrice:      //Area Float
         if(GetGlobeVarValue("sCJTotalPrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("sCJTotalPrecision"));
         else
           iPrec = 4;
         break;
       default:
         throw Exception("�����������ʹ���");
     }
   }
   else
   {
     iPrec = iPrecision; //user definite
   }

   if(iPoint+iPrec >= datastring.Length()) //����Ҫ��ȡ����
     return data;

   if(iStyle < 0)  //ȡϵͳĬ�Ͻ�ȡ��ʽ
   {
     if(GetGlobeVarValue("FloatFormatStyle") != "")
       iFormatStyle = StrToInt(GetGlobeVarValue("FloatFormatStyle"));
     else
       iFormatStyle = 0;
   }
   else if(iStyle <= 1)
     iFormatStyle = iStyle;
   else
     throw Exception("�����ʽ���ʹ���");

   double tmp=data,tmp2=0.5;
   tmp = tmp*pow(10,iPrec);
   if (iFormatStyle == 0)  //�������뷽ʽ
   {
     if(tmp >= 0)
       tmp += tmp2;
     else
       tmp -= tmp2;
   }
   datastring=myFloatToStr(tmp);
   if(datastring.Pos(".") > 0)
     datastring=datastring.SubString(1,datastring.Pos(".")-1);  //�ضϾ���֮���λ��
   if(datastring != "")
     tmp=GetFloatFromStr(datastring);
   else
     tmp=0;
   tmp = tmp/pow(10,iPrec);
   return tmp;
}
//---------------------------------------------------------------------------
//��ʽ�������������ַ������  panner 2002/11/12
//����data-��������iType-�������ͣ�ftQty:������ftPrice:���ۣ�ftAmt:���,ftRate:����ϵ����
//iStyle-С����ȡ��ʽ��1:�������룬2:�ضϣ�iPrecision-С�����ȣ�-1:ȡϵͳ����
//---------------------------------------------------------------------------
String __fastcall FormatFloatToStr(double data,TfstFloatType iType,int iPrecision,int iStyle)
{
   if(data == 0) return "0";
   int iPrec;
   //ȷ������
   if (iPrecision < 0) //ȡϵͳ����
   {
     switch(iType)
     {
       case ftQty:      //Qty Float
         if(GetGlobeVarValue("QtyPrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("QtyPrecision"));
         else
           iPrec = 2;
         break;
       case ftPrice:      //Price Float
         if(GetGlobeVarValue("PricePrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("PricePrecision"));
         else
           iPrec = 2;
         break;
       case ftAmt:      //Amt Float
         if(GetGlobeVarValue("AmtPrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("AmtPrecision"));
         else
           iPrec = 4;
         break;
       case ftRate:      //rate Float
         if(GetGlobeVarValue("RatePrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("RatePrecision"));
         else
           iPrec = 4;
         break;
       case ftsArea:      //Area Float
         if(GetGlobeVarValue("sAreaPrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("sAreaPrecision"));
         else
           iPrec = 2;
         break;
       case ftsPrice:      //Area Float
         if(GetGlobeVarValue("sPricePrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("sPricePrecision"));
         else
           iPrec = 2;
         break;
       case ftsAmt:      //Area Float
         if(GetGlobeVarValue("sAmtPrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("sAmtPrecision"));
         else
           iPrec = 4;
         break;
       case ftsCJPrice:      //Area Float
         if(GetGlobeVarValue("sCJPricePrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("sCJPricePrecision"));
         else
           iPrec = 2;
         break;
       case ftsCJTotalPrice:      //Area Float
         if(GetGlobeVarValue("sCJTotalPrecision") != "")
           iPrec = StrToInt(GetGlobeVarValue("sCJTotalPrecision"));
         else
           iPrec = 4;
         break;
       default:
         throw Exception("�����������ʹ���");
     }
   }
   else
   {
     iPrec = iPrecision; //user definite
   }
   //�����ʽ�ִ�
   bool ShowSeparator; //ÿ��λ��ʾ�ָ���
   switch(iType)
   {
     case 1:      //Qty Float
     case 2:      //Price Float
     case 3:      //Amt Float
     case 4:      //rate Float
     case 5:      //ftsArea Float
     case 6:      //ftsPrice Float
     case 7:      //ftsAmt Float
     case 8:      //ftsCJPrice Float
     case 9:      //ftsCJTotalPrice Float
       ShowSeparator = true;
       break;
     default:
       throw Exception("�����������ʹ���");
   }

  String stmp,stmp1,stmp2;
  stmp = myFloatToStr(ConvertFloat(data,iType,iPrecision,iStyle));
  if(stmp.Pos(".") > 0)
  {
    stmp1 = stmp.SubString(1,stmp.Pos(".")-1); //��ȡ��������
    stmp2 = stmp.SubString(stmp.Pos(".")+1,stmp.Length()-stmp.Pos(".")); //��ȡС������
  }
  else
  {
    stmp1 = stmp;
    stmp2 = "";
  }
  //С�����ִ���
  if(stmp2.Length() > iPrec)
    stmp2 = stmp2.SubString(1,iPrec);
  else
  {
    int tmp = iPrec-stmp2.Length();
    for(int i=0;i<tmp;i++)
      stmp2 = stmp2 + "0";
  }
  //�������ִ���
  if(ShowSeparator && stmp1.Length()>3)  //���ӷָ���
  {
    String stmp3="";
    int charcount=0;
	Char *pstr = stmp1.c_str();
    for(int i=stmp1.Length()-1;i>=0;i--)
    {
      if(pstr[i] >= '0' && pstr[i] <= '9')
      {
        charcount++;
		if(charcount > 3)
        {
          stmp3 = String(",")+stmp3;
          charcount = 1;
        }
      }
      stmp3 = String(pstr[i])+stmp3;
    }
    stmp1 = stmp3;
  }
  if(iPrec > 0)
    return stmp1+"."+stmp2;
  else
    return stmp1;
}
//���ִ�����ȡ������
//������str-�ִ�������ֵ����˫������
//---------------------------------------------------------------------------
double __fastcall GetFloatFromStr(String str)
{
  Char * ch=str.c_str();
  Char ch2[100];
  int j=0;
  bool bFirstPoint=true;
  double d;

  str = Trim(str);
  if(str.IsEmpty() || str == "-" || str == ".")  //�մ�����0 "-"����0 "."����0
    return 0;
  for (int i=0;i<str.Length();i++)
  {
    if (((ch[i] == '+' || ch[i] == '-') && i == 0) ||
        (ch[i] >= '0' && ch[i] <= '9') ||
        (ch[i] == '.' && bFirstPoint)  ||
         ch[i] == '\0')
    {
      if (ch[i] == '.')
        bFirstPoint = false;
      ch2[j]=ch[i];
      j++;
    }
  }
  if(ch2[j] != '\0')
    ch2[j] = '\0';
  try
  {
    d = StrToFloat(String(ch2));
  }
  catch(...)
  {
    throw Exception("����Ч��������");
  }
  return d;
}
//---------------------------------------------------------------------------

