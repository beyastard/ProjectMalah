#include "AModelWraperSharedData.h"
#include "ADebug.h"

config_file_c_imp g_ConfigFileCImp;

bool config_file_c_imp::pseudo_open()
{
	try
	{
		_matters_files();
	}
	catch(const char* sz)
	{
		ADebug::Msg(0,sz);
		assert(0);
	}
	catch(...)
	{
		ADebug::Msg(1,"unknow error at : %s %d error",__FILE__,__LINE__);
		assert(0);
	}
	return true;
}

void config_file_c_imp::_matters_files()
{
file_begin("data/model/����.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/����.mod","��ת");

state_end();
/*
state_begin("");

	model("");

state_end();
*/
action_begin("��ת",true);
	anim("[��ת]"	,4		);
action_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/������.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/������_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();



//
action_begin("�ɶ׵�վ",false);

	anim("[�ɶ׵�վ]"		,2		);

action_end();

action_begin("�ɶ׵�վͶ��",false);

	anim("[�ɶ׵�վͶ��]"		,2		);

action_end();

action_begin("��վ����",false);

	anim("[��վ����]"		,2		);

action_end();

action_begin("�ɶ׵�վͶ���¶�_ѭ��",true);//�����ʱ

	anim("[�ɶ׵�վ]"		,2		);
	anim("[�ɶ׵�վͶ��]"		,2		);
	anim("[��վ����]"		,2		);

action_end();

action_begin("�ɶ׵�վͶ��_ѭ��",true);//�����ʱ

	anim("[�ɶ׵�վͶ��]"		,2		);

action_end();

action_begin("��ת��",false);

	anim("[��ת��]"		,2		);

action_end();

action_begin("��ת��Ͷ��",false);

	anim("[��ת��Ͷ��]"		,2		);

action_end();

action_begin("��������",false);

	anim("[��������]"		,2		);

action_end();

action_begin("��ת��Ͷ��������_ѭ��",true);//���ѭ��

	anim("[��ת��]"		,2		);
	anim("[��ת��Ͷ��]"		,2		);
	anim("[��������]"		,2		);

action_end();

action_begin("��ת��Ͷ��_ѭ��",true);//���ѭ��

	anim("[��ת��Ͷ��]"		,2		);

action_end();

action_begin("��ת��",false);

	anim("[��ת��]"		,2		);

action_end();

action_begin("��ת��Ͷ��",false);

	anim("[��ת��Ͷ��]"		,2		);

action_end();

action_begin("��������",false);

	anim("[��������]"		,2		);

action_end();

action_begin("��ת��Ͷ��������_ѭ��",true);//���ѭ��

	anim("[��ת��]"		,2		);
	anim("[��ת��Ͷ��]"		,2		);
	anim("[��������]"		,2		);

action_end();

action_begin("��ת��Ͷ��_ѭ��",true);//���ѭ��

	anim("[��ת��Ͷ��]"		,2		);

action_end();

action_begin("�ߴ�����",false);
	
	anim("[�ߴ�����]"	,4		);

action_end();

action_begin("�ߴ���������_ѭ��",true);
	
	anim("[�ߴ���������]"	,4		);

action_end(); 

action_begin("�ߴ��������",false);
	
	anim("[�ߴ��������]"	,4		);

action_end();

//11-18
action_begin("�¶�",false);
	
	anim("[�¶�]"	,2		);

action_end();

//11-22
action_begin("�ز��е�ײǽ����",false);
	
	anim("[�ز��е�ײǽ����]"	,2		);

action_end();

//12-21 ����
action_begin("�ߴ�����2",false);
	
	anim("[�ߴ�����2]"	,4		);

action_end();

action_begin("�ߴ���������2_ѭ��",true);
	
	anim("[�ߴ���������2]"	,4		);

action_end(); 

action_begin("�ߴ��������2",false);
	
	anim("[�ߴ��������2]"	,4		);

action_end();

file_end();
file_begin("data/model/����˨.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/����˨.mod");

state_end();

state_begin("");

	model("matters/����˨���.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/��˿��.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/��˿��.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/������.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/������.mod");

state_end();

state_begin("");

	model("");

state_end();

convert_begin("Ѱ��","");

	gfx("����_������.gfx","");

convert_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/ľ��1.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/��ͨľ��.mod");

state_end();


//-------------------------------------------------------------
file_end();
file_begin("data/model/ľ��2.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/��ͨľ��2.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/��ƿ01.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/��ƿ01.mod");

state_end();

state_begin("");

	model("");

state_end();

convert_begin("Ѱ��","");

	gfx("����_��ƿ.gfx","");

convert_end();
//-------------------------------------------------------------
file_end();
file_begin("data/model/�����04.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�����04.mod");

state_end();
state_begin("");

	model("");

state_end();

convert_begin("Ѱ��","");

	gfx("����_����.gfx","");

convert_end();
//-------------------------------------------------------------
file_end();
file_begin("data/model/��ҩ��.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Ammo character define file .
	Create By MengXin 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/��ҩ��.mod");

state_end();

state_begin("");

	model("");

state_end();

convert_begin("Ѱ��","");

	gfx("����_��ҩ��.gfx","");

convert_end();
//-------------------------------------------------------------
file_end();
file_begin("data/model/�ֵ�ɳ������01.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�ֵ�ɳ������01.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/�ֵ�ɳ������02.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�ֵ�ɳ������02.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/�ֵ�ɳ������03.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�ֵ�ɳ������03.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/��01.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/��01.mod");

state_end();

state_begin("");
	
	model("");

state_end();
convert_begin("Ѱ��","");

	gfx("����_����.gfx","");

convert_end();
//-------------------------------------------------------------
file_end();
file_begin("data/model/��02.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/��02.mod");

state_end();

state_begin("");
	
	model("");

state_end();
convert_begin("Ѱ��","");

	gfx("����_����.gfx","");

convert_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/����2.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/����2_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("�ٵ�ǰ��_ѭ��",true);
	
	anim("[�ٵ�ǰ��]"	,2		);

action_end(); 

action_begin("����ת��",false);

	anim("[���ܻӵ�]"		,2		);

action_end();

action_begin("�ٵ�",false);

	anim("[�ٵ�]"		,2		);

action_end();

action_begin("ǰ��",false);

	anim("[ǰ��]"		,2		);

action_end();

action_begin("ǰ��_ѭ��",true);//�����ʱ

	anim("[�ٿ�]"		,2		);

action_end();

action_begin("�߿����¿�",false);

	anim("[����]"		,2		);
	anim("[���ڿ���]"		,2		);
	anim("[���¿�]"		,2		);

action_end();

action_begin("ǰ����_ѭ��",true);
	
	anim("[ǰ����]"	,4		);

action_end(); 

action_begin("ǰ��",false);

	anim("[ǰ��]"		,2		);

action_end();

action_begin("ǰ��_ѭ��",true);//���ѭ��

	anim("[ǰ��]"		,2		);

action_end();

action_begin("��ת��ӵ�",false);

	anim("[��ת��ӵ�]"		,2		);

action_end();

action_begin("��ת��ӵ�����",false);

	anim("[��ת��ӵ�����]"		,2		);

action_end();

action_begin("��ת��ӵ�",false);

	anim("[��ת��ӵ�]"		,2		);

action_end();

action_begin("��ת��ӵ�����",false);

	anim("[��ת��ӵ�����]"		,2		);

action_end();

action_begin("���˾ٵ�",false);

	anim("[���˾ٵ�]"		,2		);

action_end();

action_begin("����ѭ��",true);

	anim("[����]"		,2		);

action_end();

action_begin("վ���ٵ�",false);

	anim("[վ���ٵ�]"		,2		);

action_end();

action_begin("�ٿ�",false);

	anim("[�ٿ�]"		,2		);

action_end();

action_begin("Ԥ������",false);

	anim("[Ԥ������]"		,2		);

action_end();

action_begin("����_ѭ��",true);

	anim("[�ᵶ������]"		,2		);

action_end();

action_begin("�߿�����",false);

	anim("[����2]"		,2		);
	anim("[���ڿ���2]"		,2		);
	anim("[���]"		,2		);

action_end();

action_begin("�����ڿ���",false);

	anim("[����2]"		,2		);
	anim("[���ڿ���2]"		,2		);

action_end();

action_begin("�������",false);

	anim("[���]"		,2		);

action_end();

action_begin("ǰ����",false);
	
	anim("[ǰ����]"	,4		);

action_end(); 

action_begin("�߿���������",false);

	anim("[����]"		,2		);

action_end();

action_begin("�߿������ڿ���",true);

	anim("[���ڿ���]"		,2		);

action_end();

action_begin("�߿��������¿�",false);

	anim("[���¿�]"		,2		);

action_end();

action_begin("�ߴ�����",false);
	
	anim("[�ߴ�����]"	,4		);

action_end();

action_begin("�ߴ���������_ѭ��",true);
	
	anim("[�ߴ���������]"	,4		);

action_end(); 

action_begin("�ߴ��������",false);
	
	anim("[�ߴ��������]"	,4		);

action_end();

//11-13 ������
action_begin("��������",false);
	
	anim("[��������]"	,4		);

action_end();

action_begin("������������_ѭ��",true);
	
	anim("[������������]"	,4		);

action_end(); 

action_begin("�����������",false);
	
	anim("[�����������]"	,4		);

action_end();

//11-13-2 ������
action_begin("�ᵶ����_ѭ��",true);
	
	anim("[�ᵶ������]"	,4		);

action_end(); 

//11-18
action_begin("�¶�",false);
	
	anim("[�¶�]"	,2		);

action_end();

//11-22
action_begin("�ز��е�ײǽ����",false);
	
	anim("[�ز��е�ײǽ����]"	,2		);

action_end();

//12-21 ����
action_begin("�ߴ�����2",false);
	
	anim("[�ߴ�����2]"	,4		);

action_end();

action_begin("�ߴ���������2_ѭ��",true);
	
	anim("[�ߴ���������2]"	,4		);

action_end(); 

action_begin("�ߴ��������2",false);
	
	anim("[�ߴ��������2]"	,4		);

action_end();

file_end();
file_begin("data/model/���濨���.txt");

state_begin("Ѱ��");
	model("matters/���濨���.mod","��ת");
state_end();

state_begin("");

	model("");

state_end();

action_begin("��ת",true);
	anim("[��ת]"	,4		);
action_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/���濨�����.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/���濨�����.mod","��ת");

state_end();

state_begin("");

	model("");

state_end();

action_begin("��ת",true);
	anim("[��ת]"	,4		);
action_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/�ݿ�ZB26���ǹ.txt");

state_begin("Ѱ��");

	model("matters/�ݿ�ZB26���ǹ.mod","��ת");

state_end();



action_begin("��ת",true);
	anim("[��ת]"	,4		);
action_end();

file_end();

file_begin("data/model/ëɪ�ѻ���ǹ.txt");
state_begin("Ѱ��");

	model("matters/ëɪ�ѻ���ǹ.mod","��ת");

state_end();

state_begin("");

	model("");

state_end();

action_begin("��ת",true);
	anim("[��ת]"	,4		);
action_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/��ķɭ���ǹ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/��ķɭ���ǹ.mod","��ת");

state_end();

state_begin("");

	model("");

state_end();

action_begin("��ת",true);
	anim("[��ת]"	,4		);
action_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/�������ӵ���.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�������ӵ���.mod","��ת");

state_end();

state_begin("");

	model("");

state_end();
action_begin("��ת",true);
	anim("[��ת]"	,4		);
action_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/������ػ�ǹ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/�Ŷ�ʽ�ػ�ǹ.mod");

state_end();

state_begin("");

	model("");

state_end();
//-------------------------------------------------------------
file_end();
file_begin("data/model/ս�Ϻ�_��ʽ����01.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/ս�Ϻ�_��ʽ����01.mod");

state_end();

state_begin("����");

	model("matters/ս�Ϻ�_��ʽ������01.mod");

state_end();

convert_begin("Ѱ��","����");

	gfx("����������Ч��.gfx","");

convert_end();


//-------------------------------------------------------------
file_end();
file_begin("data/model/ս�Ϻ�_��ʽ����03.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/ս�Ϻ�_��ʽ����03.mod");

state_end();

state_begin("");

	model("matters/ս�Ϻ�_��ʽ������03.mod");

state_end();

convert_begin("Ѱ��","����");

	gfx("����������Ч��.gfx","");

convert_end();
//-------------------------------------------------------------
file_end();
file_begin("data/model/ս�Ϻ�_��ʽ����04.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/ս�Ϻ�_��ʽ����04.mod");

state_end();

state_begin("����");

	model("matters/ս�Ϻ�_��ʽ������04.mod");

state_end();

convert_begin("Ѱ��","����");

	gfx("����������Ч��.gfx","");

convert_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/�ػ�ǹ��.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/�ػ�ǹ��_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();



//
action_begin("������ǹ��",false);

	anim("[������ǹ��]"		,2		);

action_end();

action_begin("���ܼ�ͣת��",false);

	anim("[���ܼ�ͣת��]"		,2		);

action_end();

action_begin("������ǹ��",false);

	anim("[������ǹ��]"		,2		);

action_end();

action_begin("���ܼ�ͣת��",false);

	anim("[���ܼ�ͣת��]"		,2		);

action_end();

action_begin("������׼_ѭ��",true);

	anim("[��ת����׼]"		,2		);

action_end();

action_begin("����",false);

	anim("[����]"		,2		);

action_end();

action_begin("���ڿ���_ѭ��",true);

	anim("[���ڿ���]"		,2		);

action_end();

action_begin("���",false);

	anim("[���]"		,2		);

action_end();

action_begin("վ����׼_ѭ��",true);

	anim("[������׼]"		,2		);

action_end();

action_begin("�ߴ�����",false);
	
	anim("[�ߴ�����]"	,4		);

action_end();

action_begin("�ߴ���������_ѭ��",true);
	
	anim("[�ߴ���������]"	,4		);

action_end(); 

action_begin("�ߴ��������",false);
	
	anim("[�ߴ��������]"	,4		);

action_end();

//11-13 ������
action_begin("��������",false);
	
	anim("[��������]"	,4		);

action_end();

action_begin("������������_ѭ��",true);
	
	anim("[������������]"	,4		);

action_end(); 

action_begin("�����������",false);
	
	anim("[�����������]"	,4		);

action_end();

//11-18
action_begin("�¶�",false);
	
	anim("[�¶�]"	,2		);

action_end();

//11-20
action_begin("������ǹ��_ѭ��",true);

	anim("[������ǹ��]"		,2		);

action_end();

action_begin("������ǹ��_ѭ��",true);

	anim("[������ǹ��]"		,2		);

action_end();

//11-22
action_begin("�ز��е�ײǽ����",false);
	
	anim("[�ز��е�ײǽ����]"	,2		);

action_end();

//12-21 ����
action_begin("�ߴ�����2",false);
	
	anim("[�ߴ�����2]"	,4		);

action_end();

action_begin("�ߴ���������2_ѭ��",true);
	
	anim("[�ߴ���������2]"	,4		);

action_end(); 

action_begin("�ߴ��������2",false);
	
	anim("[�ߴ��������2]"	,4		);

action_end();file_end();
file_begin("data/model/������.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/������_��Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("վ��_ѭ��",true);
	
	anim("[Ԥ������]"	,2		);

action_end(); 

action_begin("��������",false);
	
	anim("[��������]"	,2		);

action_end(); 

action_begin("��_ѭ��",true);
	
	anim("[��]"	,2		);

action_end(); 

action_begin("ǰ��",false);
	
	anim("[ǰ��]"	,2		);

action_end(); 

action_begin("ǰ��_ѭ��",true);
	
	anim("[ǰ��]"	,2		);

action_end(); 

action_begin("Ͷ��",false);
	
	anim("[Ͷ��]"	,2		);

action_end(); 

action_begin("Ͷ��_ѭ��",true);
	
	anim("[Ͷ��]"	,2		);

action_end(); 
file_end();
file_begin("data/model/�й�糵.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");
	model("characters/�й�糵.mod");
	gfx("","");
state_end();
state_begin("");
	model("");
	gfx("����_����.gfx","");
state_end();
convert_begin("Ѱ��","");
	gfx("��ը��Ӱ_��.gfx","");
convert_end();
//-------------------------------------------------------------
file_end();
file_begin("data/model/�Ϻ���ͷ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/�Ϻ���ͷ_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("��_ѭ��",true);
	
	anim("[��]"	,2		);

action_end(); 

action_begin("ת��",false);
	
	anim("[ת��]"	,2		);

action_end(); 

action_begin("��������_ѭ��",true);
	
	anim("[��������]"	,2		);

action_end(); 

action_begin("��ͷ����_ѭ��",true);
	
	anim("[��ͷ����]"	,2		);

action_end(); 

action_begin("�ɶ׵�վ",false);
	
	anim("[�ɶ׵�վ]"	,2		);

action_end(); 

action_begin("վ�����_ѭ��",true);
	
	anim("[վ�����]"	,2		);

action_end(); 

action_begin("��վ����",false);
	
	anim("[��վ����]"	,2		);

action_end(); 

action_begin("�����ڵ�",false);
	
	anim("[�����ڵ�]"	,2		);

action_end(); 

action_begin("���غ���_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("��������_ѭ��",true);
	
	anim("[��������]"	,2		);

action_end(); 

action_begin("�ߴ�����",false);
	
	anim("[�ߴ�����]"	,4		);

action_end();

action_begin("�ߴ���������_ѭ��",true);
	
	anim("[�ߴ���������]"	,4		);

action_end(); 

action_begin("�ߴ��������",false);
	
	anim("[�ߴ��������]"	,4		);

action_end();

//11-18
action_begin("�¶�",false);
	
	anim("[�¶�]"	,2		);

action_end();

//12-21 ����
action_begin("��ȸм�",false);
	
	anim("[��ȸм�]"	,2		);

action_end();

action_begin("��ͨ��_ѭ��",true);
	
	anim("[��ͨ��]"	,2		);

action_end();

file_end();
file_begin("data/model/����.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/����_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("�����ܷ���",false);
	
	anim("[�����ܷ���]"	,2		);

action_end(); 

action_begin("�����ܷ���",false);
	
	anim("[�����ܷ���]"	,2		);

action_end(); 

action_begin("�����ܷ���_ѭ��",true);
	
	anim("[�����ܷ���]"	,2		);

action_end(); 

action_begin("�����ܷ���_ѭ��",true);
	
	anim("[�����ܷ���]"	,2		);

action_end(); 

action_begin("��ǰ��_ѭ��",true);
	
	anim("[��ǰ��]"	,2		);

action_end(); 

action_begin("��ǰ�ܻӵ�",false);
	
	anim("[��ǰ�ܻӵ�]"	,2		);

action_end(); 

action_begin("��ǰ�ܻӵ�_ѭ��",true);
	
	anim("[��ǰ�ܻӵ�]"	,2		);

action_end(); 

action_begin("վ������",false);
	
	anim("[վ������]"	,2		);

action_end(); 

action_begin("վ�����ڿ���_ѭ��",true);
	
	anim("[վ�����ڿ���]"	,2		);

action_end(); 

action_begin("վ�����������¶�",false);
	
	anim("[���]"	,2		);

action_end(); 

action_begin("�����¶�_ѭ��",true);
	
	anim("[���׼������]"	,2		);

action_end(); 

action_begin("�¶׻ӵ�����",false);
	
	anim("[�¶׻ӵ�����]"	,2		);

action_end(); 

action_begin("�¶׻ӵ�����_ѭ��",true);
	
	anim("[�¶׻ӵ�����]"	,2		);

action_end(); 

action_begin("�¶��������",false);
	
	anim("[�¶��������]"	,2		);

action_end(); 

action_begin("�¶���������ڿ���_ѭ��",true);
	
	anim("[�¶�������˿���]"	,2		);

action_end(); 

action_begin("�¶�����������",false);
	
	anim("[�¶�����������]"	,2		);

action_end(); 

action_begin("��շ�",false);
	
	anim("[��շ�]"	,2		);

action_end(); 

action_begin("��շ�_ѭ��",true);
	
	anim("[��շ�]"	,2		);

action_end(); 


//11/08 14:25 ����
action_begin("����_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("����_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 


//11-13 ������
action_begin("��������",false);
	
	anim("[��������]"	,4		);

action_end();

action_begin("������������_ѭ��",true);
	
	anim("[������������]"	,4		);

action_end(); 

action_begin("�����������",false);
	
	anim("[�����������]"	,4		);

action_end();

//12-17 ������
action_begin("վ����շ�",false);
	
	anim("[վ����շ�]"	,4		);

action_end();

action_begin("�¶׺�շ�",false);
	
	anim("[�¶׺�շ�]"	,4		);

action_end();

action_begin("ǰ�շ�",false);
	
	anim("[ǰ�շ�]"	,4		);

action_end();
file_end();
file_begin("data/model/���˹�.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/���˹�_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("����_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("��_ѭ��",true);
	
	anim("[��ǰ��]"	,2		);

action_end(); 

action_begin("������ǹ",false);
	
	anim("[������ǹ]"	,2		);

action_end(); 

action_begin("��ǹ����_ѭ��",true);
	
	anim("[������ǹ����]"	,2		);

action_end(); 

action_begin("ǰ����ǹ˨",false);
	
	anim("[ǰ����ǹ˨]"	,2		);

action_end(); 

action_begin("��_ѭ��",true);
	
	anim("[��ǹ��ǰ��]"	,2		);

action_end(); 

//12-21����
action_begin("��ǹ��ת����",false);
	
	anim("[��ǹ��ת����]"	,2		);

action_end(); 


action_begin("��ǹ˨����",false);
	
	anim("[��ǹ˨����]"	,2		);

action_end(); 

action_begin("���ǿ�ǹ",false);
	
	anim("[���ǿ�ǹ]"	,2		);

action_end(); 

action_begin("���ǿ�ǹ_ѭ��",true);
	
	anim("[���ǿ�ǹ]"	,2		);

action_end(); 

action_begin("��Ծ����",false);
	
	anim("[��Ծ]"	,2		);

action_end();

action_begin("��Ծ�ڿ���_ѭ��",true);
	
	anim("[���ڿ���]"	,2		);

action_end();

action_begin("��Ծ���",false);
	
	anim("[���]"	,2		);

action_end();

//01-06 ����
action_begin("��ǹ����",false);
	
	anim("[��ǹ����]"	,2		);

action_end(); 

action_begin("��ǹ����_ѭ��",true);
	
	anim("[��ǹ����]"	,2		);

action_end(); 

//01-25����
action_begin("��ͨվ��",false);
	
	anim("[վ��]"	,2		);

action_end(); 

file_end();
file_begin("data/model/˾��.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/˾��_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("����_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("���Ž���_ѭ��",true);
	
	anim("[���Ž���]"	,2		);

action_end(); 

action_begin("���Ż���վ��ת��",false);
	
	anim("[����ת��]"	,2		);

action_end(); 

action_begin("��_ѭ��",true);
	
	anim("[��]"	,2		);

action_end(); 

action_begin("����",false);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("��ǹ",false);
	
	anim("[��ǹ]"	,2		);

action_end(); 

action_begin("��׼_ѭ��",true);
	
	anim("[��׼]"	,2		);

action_end(); 

action_begin("����",false);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("���ֿ�Ц",false);
	
	anim("[���ֿ�Ц]"	,2		);

action_end(); 
file_end();
file_begin("data/model/�ձ�������Ա.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/�ձ�������Ա_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("������",false);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("�����ڿ���_ѭ��",true);
	
	anim("[�����ڿ���]"	,2		);

action_end(); 

action_begin("�������",false);
	
	anim("[�������]"	,2		);

action_end(); 

action_begin("�����������_ѭ��",true);
	
	anim("[�����������]"	,2		);

action_end(); 

action_begin("��_ѭ��",true);
	
	anim("[��]"	,2		);

action_end(); 

action_begin("�¶�����",false);
	
	anim("[�¶�����]"	,2		);

action_end(); 

action_begin("�¶������ڿ���_ѭ��",true);
	
	anim("[�¶���������]"	,2		);

action_end(); 

action_begin("�¶��������",false);
	
	anim("[�¶��������]"	,2		);

action_end(); 

action_begin("Ͷ��",false);
	
	anim("[Ͷ��]"	,2		);

action_end(); 

action_begin("Ͷ��_ѭ��",true);
	
	anim("[Ͷ��]"	,2		);

action_end(); 

action_begin("��վ����",false);
	
	anim("[�¶�]"	,2		);

action_end(); 

action_begin("������",false);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("�����ڿ���_ѭ��",true);
	
	anim("[�����ڿ���]"	,2		);

action_end(); 

action_begin("�������",false);
	
	anim("[�������]"	,2		);

action_end(); 

action_begin("�����������_ѭ��",true);
	
	anim("[�����������]"	,2		);

action_end(); 

action_begin("�ߴ�����",false);
	
	anim("[�ߴ�����]"	,4		);

action_end();

action_begin("�ߴ���������_ѭ��",true);
	
	anim("[�ߴ���������]"	,4		);

action_end(); 

action_begin("�ߴ��������",false);
	
	anim("[�ߴ��������]"	,4		);

action_end();

//11-13 ������
action_begin("��������",false);
	
	anim("[��������]"	,4		);

action_end();

action_begin("������������_ѭ��",true);
	
	anim("[������������]"	,4		);

action_end(); 

action_begin("�����������",false);
	
	anim("[�����������]"	,4		);

action_end();

//11-18
action_begin("�¶�",false);
	
	anim("[�¶�����]"	,2		);

action_end();

//12-21 ����
action_begin("�ߴ�����2",false);
	
	anim("[�ߴ�����2]"	,4		);

action_end();

action_begin("�ߴ���������2_ѭ��",true);
	
	anim("[�ߴ���������2]"	,4		);

action_end(); 

action_begin("�ߴ��������2",false);
	
	anim("[�ߴ��������2]"	,4		);

action_end();


//01-04
action_begin("ը��",false);
	
	anim("[ը��4]"	,2		);

action_end();
file_end();
file_begin("data/model/��������.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Bus character define file .
	Create By MengXin 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	gfx("","");
	model("characters/ս�Ϻ�_��������.mod");

state_end();

state_begin("");

	model("");
	gfx("����_����.gfx","");
state_end();

convert_begin("Ѱ��","");

	gfx("��ը��Ӱ_��.gfx","");
convert_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/ϴ�����.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/ϴ�����_Ӳ����01.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("ת��",false);
	
	anim("[ת��]"	,2		);

action_end(); 

action_begin("ת����_ѭ��",true);
	
	anim("[ת����]"	,2		);

action_end(); 

action_begin("ת�����",false);
	
	anim("[ת�����]"	,2		);

action_end(); 

action_begin("ת���������_ѭ��",true);
	
	anim("[ת���������]"	,2		);

action_end(); 

action_begin("�����ڵ�",false);
	
	anim("[�����ڵ�]"	,2		);

action_end(); 

action_begin("���غ���_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("��������_ѭ��",true);
	
	anim("[��������]"	,2		);

action_end(); 
file_end();
file_begin("data/model/��Ƿ�.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("denote/��ʷѧ��.mod");

state_end();
file_end();
file_begin("data/model/��ʽ����02.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	gfx("","");
	model("characters/ս�Ϻ�_��ʽ����02.mod");

state_end();

state_begin("����");

	gfx("����_����.gfx","");
	model("");

state_end();
convert_begin("Ѱ��","����");

	gfx("��ը��Ӱ_��.gfx","");
convert_end();



//-------------------------------------------------------------
define_HH("HH_�ұ߳�Ա",0);
define_HH("HH_��߳�Ա",1);
define_HH("HH_����",2);
//-------------------------------------------------------------

action_begin("����_ѭ��",true);
	
	anim("[����]"	,4		);

action_end(); 

action_begin("����_ѭ��",true,10.0);
	
	anim("[����]"	,4		);

action_end(); 


action_begin("����",false);
	
	anim("[����]"	,4		);

action_end(); 

action_begin("ͣ��",false);
	
	anim("[ͣ��]"	,4		);

action_end(); 
file_end();
file_begin("data/model/�Ϻ�ŮְԱ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/�Ϻ�ŮְԱ_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("��_ѭ��",true);
	
	anim("[��]"	,2		);

action_end(); 

action_begin("ת��",false);
	
	anim("[ת��]"	,2		);

action_end(); 

action_begin("��������_ѭ��",true);
	
	anim("[��������]"	,2		);

action_end(); 

action_begin("��ͷ����_ѭ��",true);
	
	anim("[��ͷ����]"	,2		);

action_end(); 

action_begin("�ɶ׵�վ",false);
	
	anim("[�ɶ׵�վ]"	,2		);

action_end(); 

action_begin("վ�����_ѭ��",true);
	
	anim("[վ�����]"	,2		);

action_end(); 

action_begin("��վ����",false);
	
	anim("[��վ����]"	,2		);

action_end(); 

action_begin("�����ڵ�",false);
	
	anim("[�����ڵ�]"	,2		);

action_end(); 

action_begin("���غ���_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("��������_ѭ��",true);
	
	anim("[��������]"	,2		);

action_end(); 

action_begin("�ߴ�����",false);
	
	anim("[�ߴ�����]"	,4		);

action_end();

action_begin("�ߴ���������_ѭ��",true);
	
	anim("[�ߴ���������]"	,4		);

action_end(); 

action_begin("�ߴ��������",false);
	
	anim("[�ߴ��������]"	,4		);

action_end();

//11-18
action_begin("�¶�",false);
	
	anim("[�¶�]"	,2		);

action_end();

//12-21 ����
action_begin("��ȸм�",false);
	
	anim("[��ȸм�]"	,2		);

action_end();

action_begin("��ͨ��_ѭ��",true);
	
	anim("[��ͨ��]"	,2		);

action_end();

//01-25 ����
action_begin("��ͨվ��",false);
	
	anim("[��ͨվ��]"	,2		);

action_end();

file_end();
file_begin("data/model/½��.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/�ձ�½��_Ӳ����01.mod");

state_end();

state_begin("");

	model("");

state_end();




//++++++����+++++
action_begin("�����ƶ�_ѭ��",true);
	
	anim("[�¶��ƶ�]"	,2		);

action_end(); 

action_begin("������ǹ��_ѭ��",true);
	
	anim("[������ǹ��]"	,2		);

action_end();

action_begin("������ǹ��_ѭ��",true);
	
	anim("[������ǹ��]"	,2		);

action_end();

action_begin("׷��_ѭ��",true);

	anim("[׷��]"		,2		);

action_end();

action_begin("�ɶ׵�վ��׼_ѭ��",true);

	anim("[�ɶ׵�վ��׼]"		,2		);

action_end();

action_begin("վ����ǹ",false);

	anim("[վ����ǹ]"		,2		);

action_end();

action_begin("��ת����_ѭ��",true); //�����ʱ

	anim("[��ת����]"		,2		);

action_end();

action_begin("��ת����_ѭ��",true); //�����ʱ

	anim("[��ת����]"		,2		);

action_end();

action_begin("�ҷ���_ѭ��",true); //�����ʱ

	anim("[�ҹ���]"		,4		);

action_end();

action_begin("�󷭹�_ѭ��",true); //�����ʱ

	anim("[�����]"		,4		);

action_end();

action_begin("�����Ҳ�����¶���׼���_ѭ��",true);

	anim("[�ҹ���]"		,4		);
	anim("[�ҹ�����׼]"		,2		);
	anim("[�ҹ�������]"		,2		);
	anim("[�ҹ�����˨]"		,2		);

action_end();

action_begin("�����������¶���׼���_ѭ��",true);

	anim("[�����]"		,4			);
	anim("[�������׼]"		,2		);
	anim("[���������]"		,2		);
	anim("[�������ǹ˨]"		,2		);

action_end();


//+++++++++

//-------------------------------------------------------------
action_begin("��ǹ����Ѳ��_ѭ��",true);
	
	anim("[Ѳ����]"	,2		);

action_end(); 

action_begin("�ӳ�������",false);

	anim("[����]"			,2		);
	anim("[���ڿ���]"		,2		);
	anim("[���]"			,2		);

action_end(); 

action_begin("��_ѭ��",true);
	
	anim("[��]"	,2		);

action_end(); 

action_begin("��_ѭ��",true);
	
	anim("[��ǹǰ��]"	,2		);

action_end(); 

action_begin("��ǹ������ת",false);

	anim("[���ܼ�ͣת��]"	,2		);

action_end(); 

action_begin("��ǹ������ת",false);

	anim("[���ܼ�ͣת��]"	,2		);

action_end();

action_begin("��ǹ����",false);

	anim("[��ǹǰ��]"		,2		);

action_end();

action_begin("�ɶ׵�վ",false);

	anim("[�ɶ׵�վ]"		,2		);

action_end();

////


action_begin("�ɶ׵�վ����",false);

	anim("[�ɶ׵�վ����]"	,2		);

action_end();

action_begin("�ɶ׵�վ����_ѭ��",true);//�����ʱ

	anim("[�ɶ׵�վ����]"	,2		);

action_end();

action_begin("��վ����",false);

	anim("[�¶�̧ǹ]"		,2		);

action_end();

action_begin("�ɶ׵�վ_ѭ��",true);

	anim("[�ɶ׵�վ]"		,2		);
	anim("[�ɶ׵�վ��׼]"	,2		);
	anim("[�ɶ׵�վ����]"	,2		);
	anim("[��վ����]"		,2		);

action_end();

/////

action_begin("վ����׼�����ǹ˨",false);//�����ʱ

	anim("[վ����ǹ]"		,2		);
	anim("[վ����ǹ��׼]"	,2		);
	anim("[վ����ǹ����]"	,2		);

action_end();

action_begin("վ����׼�����ǹ˨_ѭ��",true);//�����ʱ

	anim("[վ����ǹ��׼]"	,2		);
	anim("[վ����ǹ����]"	,2		);
	anim("[վ����ǹ��ǹ˨]"	,2		);

action_end();

action_begin("��ת���ǹ��׼���",false);//�����ʱ

	anim("[��ת��̧ǹ]"		,2		);
	anim("[��ת����׼]"		,2		);
	anim("[��ת����]"		,2		);

action_end();

action_begin("��ת���ǹ��׼���������_ѭ��",true);

	anim("[��ת��̧ǹ]"		,2		);
	anim("[��ת����׼]"		,2		);
	anim("[��ת����]"		,2		);
	anim("[��������]"		,2		);

action_end();

/////////

action_begin("��ת���ǹ��׼���",false);//�����ʱ

	anim("[��ת��̧ǹ]"		,2		);
	anim("[��ת����׼]"		,2		);
	anim("[��ת����]"		,2		);

action_end();

action_begin("��ת���ǹ��׼���������_ѭ��",true);

	anim("[��ת��̧ǹ]"		,2		);
	anim("[��ת����׼]"		,2		);
	anim("[��ת����]"		,2		);
	anim("[��������]"		,2		);

action_end();

action_begin("�¶���׼�����ǹ˨",false);

	anim("[�¶�̧ǹ]"		,2		);
	anim("[�¶���׼]"		,2		);
	anim("[�¶׿���]"		,2		);

action_end();

action_begin("�¶���׼�����ǹ˨_ѭ��",true);//�����ʱ

	anim("[�¶���׼]"		,2		);
	anim("[�¶׿���]"		,2		);
	anim("[�¶���ǹ˨]"		,2		);

action_end();

//////

action_begin("�Ҳ�����¶���׼���",false);//�����ʱ

	anim("[�ҹ�����׼]"		,2		);
	anim("[�ҹ�������]"		,2		);

action_end();

action_begin("�������¶���׼���",false);

	anim("[�������׼]"		,2		);
	anim("[���������]"		,2		);

action_end();

action_begin("�Ҳ�����¶���׼���_ѭ��",true,0.5);//�����ʱ

	anim("[�ҹ�����׼]"		,2		);
	anim("[�ҹ�������]"		,2		);
	anim("[�ҹ�����˨]"		,2		);

action_end();

action_begin("�������¶���׼���_ѭ��",true,0.5);//�����ʱ

	anim("[�������׼]"		,2		);
	anim("[���������]"		,2		);
	anim("[�������ǹ˨]"		,2		);

action_end();

/////
action_begin("è����ǹ�ƶ�_ѭ��",true);

	anim("[�¶��ƶ�]"		,2		);

action_end();


action_begin("׼��׷��",false);

	anim("[׼��׷��]"		,2		);

action_end();

action_begin("׷��",false);

	anim("[׷��]"		,2		);

action_end();

action_begin("�����ڿ���",false);

	anim("[����]"			,2		);
	anim("[���ڿ���]"		,2		);

action_end(); 

action_begin("�������",false);

	anim("[���]"			,2		);

action_end(); 

action_begin("�ҷ���",false); 

	anim("[�ҹ���]"		,4		);

action_end();

action_begin("�󷭹�",false); 

	anim("[�����]"		,4		);

action_end();

action_begin("��ת��̧ǹ",false);

	anim("[��ת��̧ǹ]"		,2		);

action_end();

action_begin("��ת����׼",false);

	anim("[��ת����׼]"		,2		);

action_end();

action_begin("��ת����",false);

	anim("[��ת����]"		,2		);

action_end();

action_begin("��ת��̧ǹ",false);

	anim("[��ת��̧ǹ]"		,2		);

action_end();

action_begin("��ת����׼",false);

	anim("[��ת����׼]"		,2		);

action_end();

action_begin("��ת����",false);

	anim("[��ת����]"		,2		);

action_end();

action_begin("�ߴ�����",false);
	
	anim("[�ߴ�����]"	,4		);

action_end();

action_begin("�ߴ���������_ѭ��",true);
	
	anim("[�ߴ���������]"	,4		);

action_end(); 

action_begin("�ߴ��������",false);
	
	anim("[�ߴ��������]"	,4		);

action_end();

//11/08 15:44 ������
action_begin("Ѳ��ת��",false);
	
	anim("[Ѳ��ת��]"	,2		);

action_end();


//11-13 ������
action_begin("��������",false);
	
	anim("[��������]"	,4		);

action_end();

action_begin("������������_ѭ��",true);
	
	anim("[������������]"	,4		);

action_end(); 

action_begin("�����������",false);
	
	anim("[�����������]"	,4		);

action_end();

//11-18
action_begin("�¶�",false);
	
	anim("[�¶�]"	,2		);

action_end();

//11-22
action_begin("�ز��е�ײǽ����",false);
	
	anim("[�ز��е�ײǽ����]"	,2		);

action_end();

//12-21 ����
action_begin("����ƽ��",false);
	
	anim("[����ƽ�����]"	,2		);

action_end();

action_begin("����ƽ��_ѭ��",true);
	
	anim("[����ƽ�����]"	,2		);

action_end();

action_begin("����ƽ��",false);
	
	anim("[����ƽ�����]"	,2		);

action_end();

action_begin("����ƽ��_ѭ��",true);
	
	anim("[����ƽ�����]"	,2		);

action_end();

action_begin("��ת��",false);
	
	anim("[��ת��]"	,2		);

action_end();

action_begin("��ת��",false);
	
	anim("[��ת��]"	,2		);

action_end();

action_begin("�ߴ�����2",false);
	
	anim("[�ߴ�����2]"	,4		);

action_end();

action_begin("�ߴ���������2_ѭ��",true);
	
	anim("[�ߴ���������2]"	,4		);

action_end(); 

action_begin("�ߴ��������2",false);
	
	anim("[�ߴ��������2]"	,4		);

action_end();

action_begin("�¶��ƶ�����",false);
	
	anim("[�¶��ƶ�����]"	,4		);

action_end();

action_begin("�¶��ƶ�����_ѭ��",true);
	
	anim("[�¶��ƶ�����]"	,4		);

action_end();

action_begin("��ͨ��_ѭ��",true);
	
	anim("[��ͨ��]"	,4		);

action_end();file_end();
file_begin("data/model/����_ָ�ӵ�.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/����_ָ�ӵ�_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();




//
action_begin("�ᵶ������_ѭ��",true);
	
	anim("[�ᵶ������]"	,2		);

action_end(); 

action_begin("���ܻӵ�",false);
	
	anim("[���ܻӵ�]"	,2		);

action_end(); 

action_begin("�ᵶ������_ѭ��",true);
	
	anim("[�ᵶ������]"	,2		);

action_end(); 

action_begin("���ܻӵ�",false);
	
	anim("[���ܻӵ�]"	,2		);

action_end(); 

action_begin("��׼�ӵ�",false);
	
	anim("[��׼�ӵ�]"	,2		);

action_end(); 

action_begin("��׼վ���ӵ�ָ��_ѭ��",true);
	
	anim("[��׼վ���ӵ�ָ��]"	,2		);

action_end(); 

action_begin("�ֵ�վ��_ѭ��",true);
	
	anim("[�ֵ�վ��]"	,2		);

action_end(); 

action_begin("�ɶ׵�վ�ӵ�",false);
	
	anim("[�ɶ׵�վ]"	,2		);

action_end(); 

action_begin("�ɶ׵�վ�ӵ�ָ��_ѭ��",true);
	
	anim("[�ɶ׵�վ�ӵ�ָ��]"	,2		);

action_end(); 

action_begin("��վ����",false);
	
	anim("[��վ����]"	,2		);

action_end(); 

action_begin("��ת��ӵ�",false);
	
	anim("[��ת��ӵ�]"	,2		);

action_end();

action_begin("��ת��ӵ�ָ��_ѭ��",true);
	
	anim("[��ת��ӵ�ָ��]"	,2		);

action_end(); 

action_begin("��������",false);
	
	anim("[��������]"	,2		);

action_end();

action_begin("��ת��ӵ�",false);
	
	anim("[��ת��ӵ�]"	,2		);

action_end();

action_begin("��ת��ӵ�ָ��_ѭ��",true);
	
	anim("[��ת��ӵ�ָ��]"	,2		);

action_end(); 

action_begin("��������",false);
	
	anim("[��������]"	,2		);

action_end();

action_begin("�ߴ�����",false);
	
	anim("[�ߴ�����]"	,4		);

action_end();

action_begin("�ߴ���������_ѭ��",true);
	
	anim("[�ߴ���������]"	,4		);

action_end(); 

action_begin("�ߴ��������",false);
	
	anim("[�ߴ��������]"	,4		);

action_end();


//11/08 14:25 ����
action_begin("�յ�",false);
	
	anim("[�յ�]"	,2		);

action_end();
 
action_begin("�ӵ�ָ���յ�_ѭ��",true);
	
	anim("[��׼�ӵ�]"	,2		);
	anim("[�յ�]"	,2		);

action_end();

//11-18
action_begin("�¶�",false);
	
	anim("[�¶�]"	,2		);

action_end();
 
//11-22
action_begin("�ز��е�ײǽ����",false);
	
	anim("[�ز��е�ײǽ����]"	,2		);

action_end();

//12-21����
action_begin("������_ѭ��",true);
	
	anim("[������]"	,2		);

action_end();

action_begin("�ߴ�����2",false);
	
	anim("[�ߴ�����2]"	,4		);

action_end();

action_begin("�ߴ���������2_ѭ��",true);
	
	anim("[�ߴ���������2]"	,4		);

action_end(); 

action_begin("�ߴ��������2",false);
	
	anim("[�ߴ��������2]"	,4		);

action_end();

action_begin("��������",false);
	
	anim("[��������]"	,4		);

action_end();

action_begin("������������_ѭ��",true);
	
	anim("[������������]"	,4		);

action_end(); 

action_begin("�����������",false);
	
	anim("[�����������]"	,4		);

action_end();

file_end();


file_begin("data/model/����_��ǹ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/����_��ǹ_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();




//
action_begin("��ת��̧ǹ",false);
	
	anim("[��ת��̧ǹ]"	,2		);

action_end(); 

action_begin("��ת����׼_ѭ��",true);
	
	anim("[��ת����׼]"	,2		);

action_end(); 

action_begin("��ת����_ѭ��",true); //�����ʱ
	
	anim("[��ת����]"	,2		);

action_end(); 

action_begin("��ת��̧ǹ������_ѭ��",true); //�����ʱ

	anim("[��ת��̧ǹ]"	,2		);	
	anim("[��ת����׼]"	,2		);	
	anim("[��ת����]"	,2		);
	anim("[��������]"	,2		);	

action_end(); 

action_begin("��ת��̧ǹ",false);
	
	anim("[��ת��̧ǹ]"	,2		);

action_end(); 

action_begin("��ת����׼_ѭ��",true);
	
	anim("[��ת����׼]"	,2		);

action_end(); 

action_begin("��ת����_ѭ��",true); //�����ʱ
	
	anim("[��ת����]"	,2		);

action_end(); 

action_begin("��ת��̧ǹ������_ѭ��",true); //�����ʱ

	anim("[��ת��̧ǹ]"	,2		);	
	anim("[��ת����׼]"	,2		);	
	anim("[��ת����]"	,2		);
	anim("[��������]"	,2		);	

action_end(); 
file_end();
file_begin("data/model/������.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	gfx("","");
	model("characters/ս�Ϻ�_������.mod");

state_end();

state_begin("");

	gfx("����_����.gfx","");
	model("");

state_end();
convert_begin("Ѱ��","");

	gfx("��ը��Ӱ_��.gfx","");
convert_end();
//-------------------------------------------------------------
file_end();
file_begin("data/model/�Ϻ�Ůƽ��.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/�Ϻ�Ůƽ��_��Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("��_ѭ��",true);
	
	anim("[��]"	,2		);

action_end(); 

action_begin("ת��",false);
	
	anim("[ת��]"	,2		);

action_end(); 

action_begin("��������_ѭ��",true);
	
	anim("[��������]"	,2		);

action_end(); 

action_begin("��ͷ����_ѭ��",true);
	
	anim("[��ͷ����]"	,2		);

action_end(); 

action_begin("�ɶ׵�վ",false);
	
	anim("[�ɶ׵�վ]"	,2		);

action_end(); 

action_begin("վ�����_ѭ��",true);
	
	anim("[վ�����]"	,2		);

action_end(); 

action_begin("��վ����",false);
	
	anim("[��վ����]"	,2		);

action_end(); 

action_begin("�����ڵ�",false);
	
	anim("[�����ڵ�]"	,2		);

action_end(); 

action_begin("���غ���_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("��������_ѭ��",true);
	
	anim("[��������]"	,2		);

action_end(); 

action_begin("�ߴ�����",false);
	
	anim("[�ߴ�����]"	,4		);

action_end();

action_begin("�ߴ���������_ѭ��",true);
	
	anim("[�ߴ���������]"	,4		);

action_end(); 

action_begin("�ߴ��������",false);
	
	anim("[�ߴ��������]"	,4		);

action_end();

//11-18
action_begin("�¶�",false);
	
	anim("[�¶�]"	,2		);

action_end();

//12-21 ����
action_begin("��ȸм�",false);
	
	anim("[��ȸм�]"	,2		);

action_end();

action_begin("��ͨ��_ѭ��",true);
	
	anim("[��ͨ��]"	,2		);

action_end();

//01-25 ����
action_begin("��ͨվ��",false);
	
	anim("[��ͨվ��]"	,2		);

action_end();

file_end();
file_begin("data/model/��ƽ��.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/��ƽ��_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("��_ѭ��",true);
	
	anim("[��]"	,2		);

action_end(); 

action_begin("ת��",false);
	
	anim("[ת��]"	,2		);

action_end(); 

action_begin("��������_ѭ��",true);
	
	anim("[��������]"	,2		);

action_end(); 

action_begin("��ͷ����_ѭ��",true);
	
	anim("[��ͷ����]"	,2		);

action_end(); 

action_begin("�ɶ׵�վ",false);
	
	anim("[�ɶ׵�վ]"	,2		);

action_end(); 

action_begin("վ�����_ѭ��",true);
	
	anim("[վ�����]"	,2		);

action_end(); 

action_begin("��վ����",false);
	
	anim("[��վ����]"	,2		);

action_end(); 

action_begin("�����ڵ�",false);
	
	anim("[�����ڵ�]"	,2		);

action_end(); 

action_begin("���غ���_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("��������_ѭ��",true);
	
	anim("[��������]"	,2		);

action_end(); 

action_begin("�ߴ�����",false);
	
	anim("[�ߴ�����]"	,4		);

action_end();

action_begin("�ߴ���������_ѭ��",true);
	
	anim("[�ߴ���������]"	,4		);

action_end(); 

action_begin("�ߴ��������",false);
	
	anim("[�ߴ��������]"	,4		);

action_end();

//11-18
action_begin("�¶�",false);
	
	anim("[�¶�]"	,2		);

action_end();

//12-21 ����
action_begin("��ȸм�",false);
	
	anim("[��ȸм�]"	,2		);

action_end();

action_begin("��ͨ��_ѭ��",true);
	
	anim("[��ͨ��]"	,2		);

action_end();file_end();
file_begin("data/model/�Ϻ�������ְԱ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/�Ϻ�������ְԱ_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("��_ѭ��",true);
	
	anim("[��]"	,2		);

action_end(); 

action_begin("ת��",false);
	
	anim("[ת��]"	,2		);

action_end(); 

action_begin("��������_ѭ��",true);
	
	anim("[��������]"	,2		);

action_end(); 

action_begin("��ͷ����_ѭ��",true);
	
	anim("[��ͷ����]"	,2		);

action_end(); 

action_begin("�ɶ׵�վ",false);
	
	anim("[�ɶ׵�վ]"	,2		);

action_end(); 

action_begin("վ�����_ѭ��",true);
	
	anim("[վ�����]"	,2		);

action_end(); 

action_begin("��վ����",false);
	
	anim("[��վ����]"	,2		);

action_end(); 

action_begin("�����ڵ�",false);
	
	anim("[�����ڵ�]"	,2		);

action_end(); 

action_begin("���غ���_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("��������_ѭ��",true);
	
	anim("[��������]"	,2		);

action_end(); 

action_begin("�ߴ�����",false);
	
	anim("[�ߴ�����]"	,4		);

action_end();

action_begin("�ߴ���������_ѭ��",true);
	
	anim("[�ߴ���������]"	,4		);

action_end(); 

action_begin("�ߴ��������",false);
	
	anim("[�ߴ��������]"	,4		);

action_end();

//11-18
action_begin("�¶�",false);
	
	anim("[�¶�]"	,2		);

action_end();

//12-21 ����
action_begin("��ȸм�",false);
	
	anim("[��ȸм�]"	,2		);

action_end();

action_begin("��ͨ��_ѭ��",true);
	
	anim("[��ͨ��]"	,2		);

action_end();file_end();
file_begin("data/model/�ձ�Ħ�г���ǹ��.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/�վ�Ħ�г���ǹ��_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("��_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("��ǰ��ǹ_ѭ��",true);
	
	anim("[��ǰ��ǹ]"	,2		);

action_end(); 

action_begin("��ǰ��ǹ",false);
	
	anim("[��ǰ��ǹ]"	,2		);

action_end(); 

action_begin("���ת",false);
	
	anim("[���ת]"	,2		);

action_end(); 

action_begin("�����׼",false);
	
	anim("[�����׼]"	,2		);

action_end(); 

action_begin("���ǹ_ѭ��",true);
	
	anim("[���ǹ]"	,2		);

action_end(); 

action_begin("���ǹ",false);
	
	anim("[���ǹ]"	,2		);

action_end(); 

action_begin("��ǰת",false);
	
	anim("[��ǰת]"	,2		);

action_end(); 






file_end();
file_begin("data/model/�ձ�Ħ�г���ʻԱ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/�վ�Ħ�г���ʻԱ_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("����_ѭ��",true);
	
	anim("[��������]"	,2		);

action_end(); 

action_begin("����",false);
	
	anim("[�ұ���]"	,2		);

action_end(); 
file_end();
file_begin("data/model/�ư���.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�ư���.mod");

state_end();


//-------------------------------------------------------------
file_end();
file_begin("data/model/�Ŷ�ʽ�ػ�ǹ_վ��ʽ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/�Ŷ�ʽ�ػ�ǹ_վ��ʽ.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("����_ѭ��",true, 3.0);
	
	anim("[����]"	,4		);

action_end(); 

action_begin("����",false);
	
	anim("[����]"	,4		);

action_end(); 

action_begin("ֹͣ",false);
	
	anim("[ֹͣ]"	,4		);

action_end(); 
file_end();
file_begin("data/model/��ͨ��ͤ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/��ͨ��ͤ.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/�Ŷ�ʽ�ػ�ǹ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/�Ŷ�ʽ�ػ�ǹ.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("����_ѭ��",true, 3.0);
	
	anim("[����]"	,4		);

action_end(); 

action_begin("����",false);
	
	anim("[����]"	,4		);

action_end(); 

action_begin("ֹͣ",false);
	
	anim("[ֹͣ]"	,4		);

action_end(); 
file_end();
file_begin("data/model/��������.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/����������.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/���Ƴ�.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/���Ƴ�.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/�������ӵ���3.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�������ӵ���3.mod","��ת");

state_end();

state_begin("");

	model("");

state_end();
action_begin("��ת",true);
	anim("[��ת]"	,4		);
action_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/�������ӵ���2.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�������ӵ���2.mod","��ת");

state_end();

state_begin("");

	model("");

state_end();
action_begin("��ת",true);
	anim("[��ת]"	,4		);
action_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/С����.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/С����.mod");

state_end();

//-------------------------------------------------------------

define_HH("HH_ʿ��1",0);
define_HH("HH_ʿ��2",1);
define_HH("HH_ʿ��3",2);
define_HH("HH_ʿ��4",3);
define_HH("HH_ʿ��5",4);
define_HH("HH_ʿ��6",5);
define_HH("HH_ʿ��7",6);
file_end();
file_begin("data/model/����3.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/����3.mod","��ת");

state_end();
/*
state_begin("");

	model("");

state_end();
*/
action_begin("��ת",true);
	anim("[��ת]"	,4		);
action_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/����2.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/����2.mod","��ת");

state_end();
/*
state_begin("");

	model("");

state_end();
*/
action_begin("��ת",true);
	anim("[��ת]"	,4		);
action_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/Ѫ��01.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/Ѫ��01.mod","��ת");

state_end();

action_begin("��ת",true);
	anim("[��ת]"	,4		);
action_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/�Ƴ�3.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	damaged vehicle - NO.3 character define file .
	Create By MengXin 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�Ƴ�3.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/�Ƴ�2.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	damaged vehicle - NO.2 character define file .
	Create By MengXin 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�Ƴ�2.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/Ѫ��02.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/Ѫ��02.mod","��ת");

state_end();

action_begin("��ת",true);
	anim("[��ת]"	,4		);
action_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/�����˱���.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	gfx("","");
	model("characters/�����˱���.mod");

state_end();

convert_begin("Ѱ��","����");

	gfx("��ը��Ӱ_��.gfx","");
convert_end();

state_begin("����");
	gfx("����_����.gfx","");
	model("characters/�����˱�������.mod");

state_end();

//-------------------------------------------------------------

define_HH("HH_�������䳵��ը",0);
define_HH("HH_ʿ��05",1);
define_HH("HH_ʿ��04",2);
define_HH("HH_ʿ��01",3);
define_HH("HH_ʿ��03",4);
define_HH("HH_ʿ��02",5);
define_HH("HH_��ͨľ��01",6);
define_HH("HH_����Ͱ",7);


//11-21
action_begin("����_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("ͣ��",false);
	
	anim("[ͣ��]"	,2		);

action_end(); 





file_end();
file_begin("data/model/����.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/����_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("�ٵ�ǰ��_ѭ��",true);
	
	anim("[�ٵ�ǰ��]"	,2		);

action_end(); 

action_begin("����ת��",false);

	anim("[���ܻӵ�]"		,2		);

action_end();

action_begin("�ٵ�",false);

	anim("[�ٵ�]"		,2		);

action_end();

action_begin("ǰ��",false);

	anim("[ǰ��]"		,2		);

action_end();

action_begin("ǰ��_ѭ��",true);//�����ʱ

	anim("[�ٿ�]"		,2		);

action_end();

action_begin("�߿����¿�",false);

	anim("[����]"		,2		);
	anim("[���ڿ���]"		,2		);
	anim("[���¿�]"		,2		);

action_end();

action_begin("ǰ����_ѭ��",true);
	
	anim("[ǰ����]"	,4		);

action_end(); 

action_begin("ǰ��",false);

	anim("[ǰ��]"		,2		);

action_end();

action_begin("ǰ��_ѭ��",true);//���ѭ��

	anim("[ǰ��]"		,2		);

action_end();

action_begin("��ת��ӵ�",false);

	anim("[��ת��ӵ�]"		,2		);

action_end();

action_begin("��ת��ӵ�����",false);

	anim("[��ת��ӵ�����]"		,2		);

action_end();

action_begin("��ת��ӵ�",false);

	anim("[��ת��ӵ�]"		,2		);

action_end();

action_begin("��ת��ӵ�����",false);

	anim("[��ת��ӵ�����]"		,2		);

action_end();

action_begin("���˾ٵ�",false);

	anim("[���˾ٵ�]"		,2		);

action_end();

action_begin("����ѭ��",true);

	anim("[����]"		,2		);

action_end();

action_begin("վ���ٵ�",false);

	anim("[վ���ٵ�]"		,2		);

action_end();

action_begin("�ٿ�",false);

	anim("[�ٿ�]"		,2		);

action_end();

action_begin("Ԥ������",false);

	anim("[Ԥ������]"		,2		);

action_end();

action_begin("����_ѭ��",true);

	anim("[�ᵶ������]"		,2		);

action_end();

action_begin("�߿�����",false);

	anim("[����2]"		,2		);
	anim("[���ڿ���2]"		,2		);
	anim("[���]"		,2		);

action_end();

action_begin("�����ڿ���",false);

	anim("[����2]"		,2		);
	anim("[���ڿ���2]"		,2		);

action_end();

action_begin("�������",false);

	anim("[���]"		,2		);

action_end();

action_begin("ǰ����",false);
	
	anim("[ǰ����]"	,4		);

action_end(); 

action_begin("�߿���������",false);

	anim("[����]"		,2		);

action_end();

action_begin("�߿������ڿ���",true);

	anim("[���ڿ���]"		,2		);

action_end();

action_begin("�߿��������¿�",false);

	anim("[���¿�]"		,2		);

action_end();

action_begin("�ߴ�����",false);
	
	anim("[�ߴ�����]"	,4		);

action_end();

action_begin("�ߴ���������_ѭ��",true);
	
	anim("[�ߴ���������]"	,4		);

action_end(); 

action_begin("�ߴ��������",false);
	
	anim("[�ߴ��������]"	,4		);

action_end();

//11-13 ������
action_begin("��������",false);
	
	anim("[��������]"	,4		);

action_end();

action_begin("������������_ѭ��",true);
	
	anim("[������������]"	,4		);

action_end(); 

action_begin("�����������",false);
	
	anim("[�����������]"	,4		);

action_end();

//11-13-2
action_begin("�ᵶ����_ѭ��",true);
	
	anim("[�ᵶ������]"	,4		);

action_end(); 

//11-18
action_begin("�¶�",false);
	
	anim("[�¶�]"	,2		);

action_end();

//11-22
action_begin("�ز��е�ײǽ����",false);
	
	anim("[�ز��е�ײǽ����]"	,2		);

action_end();


//12-21 ����
action_begin("�ߴ�����2",false);
	
	anim("[�ߴ�����2]"	,4		);

action_end();

action_begin("�ߴ���������2_ѭ��",true);
	
	anim("[�ߴ���������2]"	,4		);

action_end(); 

action_begin("�ߴ��������2",false);
	
	anim("[�ߴ��������2]"	,4		);

action_end();file_end();
file_begin("data/model/97ʽ̹��.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	gfx("","");
	model("characters/97ʽ̹��.mod");

state_end();

state_begin("");

	gfx("����_����.gfx","");
	model("characters/97ʽ̹�˻���.mod");

state_end();
convert_begin("Ѱ��","");

	gfx("��ը��Ӱ_��.gfx","");
convert_end();

//
action_begin("����_ѭ��",true);
	
	anim("[����]"	,4		);

action_end(); 

action_begin("����_ѭ��",true,10.0);
	
	anim("[����]"	,4		);

action_end(); 


action_begin("����",false);
	
	anim("[����]"	,4		);

action_end(); 

action_begin("ͣ��",false);
	
	anim("[ͣ��]"	,4		);

action_end(); 

define_HH("HH_̹��������",2);

file_end();
file_begin("data/model/player1.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/t.mod");

state_end();

state_begin("");

	model("characters/t.mod");

state_end();


convert_begin("Ѱ��","�վ�");

	gfx("���񵯱�ը�͹�Ч��.gfx","");

convert_end();

//-------------------------------------------------------------

action_begin("����",false);
	//	  ��������	���ȼ�	
	//anim("[վ����]"	,1		);
	//anim("[����վ]"	,2		);
	//anim("[����Ϯ]"	,2		);
action_end(); 

action_begin("��",false);
	//	  ��������	���ȼ�	
	anim("[Ѳ��]"	,1		);
action_end();file_end();
file_begin("data/model/���ǹ��.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/���ǹ��Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();




//
action_begin("��_ѭ��",true);
	
	anim("[��]"	,2		);

action_end(); 

action_begin("�߿�����",false);

	anim("[����]"			,2		);
	anim("[���ڿ���]"		,2		);
	anim("[���]"			,2		);

action_end(); 

action_begin("������ǹ��_ѭ��",true);
	
	anim("[������ǹ��]"	,2		);

action_end();

action_begin("��ǹ������ת",false);

	anim("[���ܼ�ͣת��]"	,2		);

action_end(); 

action_begin("������ǹ��_ѭ��",true);
	
	anim("[������ǹ��]"	,2		);

action_end();

action_begin("��ǹ������ת",false);

	anim("[���ܼ�ͣת��]"	,2		);

action_end();

action_begin("�ɶ׵�վ",false);

	anim("[�ɶ׵�վ]"		,2		);

action_end();

action_begin("�ɶ׵�վ��׼_ѭ��",true);

	anim("[�ɶ׵�վ��׼]"		,2		);

action_end();

action_begin("�ɶ׵�վ����",false);

	anim("[�ɶ׵�վ����]"	,2		);

action_end();

action_begin("�ɶ׵�վ����_ѭ��",true);//�����ʱ

	anim("[�ɶ׵�վ����]"	,2		);

action_end();

action_begin("��վ����",false);

	anim("[��վ����]"		,2		);

action_end();

action_begin("�ɶ׵�վ_ѭ��",true);//�����ʱ

	anim("[�ɶ׵�վ]"		,2		);
	anim("[�ɶ׵�վ��׼]"	,2		);
	anim("[�ɶ׵�վ����]"	,2		);
	anim("[��վ����]"		,2		);

action_end();

action_begin("վ����ǹ",false);

	anim("[վ����ǹ]"		,2		);

action_end();

action_begin("վ����׼���",false);

	anim("[վ����ǹ]"		,2		);
	anim("[վ����ǹ��׼]"	,2		);
	anim("[վ����ǹ����]"	,2		);

action_end();

action_begin("վ����׼���_ѭ��",true);//�����ʱ

	anim("[վ����ǹ��׼]"	,2		);
	anim("[վ����ǹ����]"	,2		);

action_end();

action_begin("��ת���ǹ��׼���",false);

	anim("[��ת��̧ǹ]"		,2		);
	anim("[��ת����׼]"		,2		);
	anim("[��ת����]"		,2		);

action_end();

action_begin("��ת���ǹ��׼���������_ѭ��",true);//�����ʱ

	anim("[��ת��̧ǹ]"		,2		);
	anim("[��ת����׼]"		,2		);
	anim("[��ת����]"		,2		);
	anim("[��������]"		,2		);

action_end();

action_begin("��ת����_ѭ��",true); //�����ʱ

	anim("[��ת����]"		,2		);

action_end();

action_begin("��ת���ǹ��׼���",false);

	anim("[��ת��̧ǹ]"		,2		);
	anim("[��ת����׼]"		,2		);
	anim("[��ת����]"		,2		);

action_end();

action_begin("��ת���ǹ��׼���������_ѭ��",true);//�����ʱ

	anim("[��ת��̧ǹ]"		,2		);
	anim("[��ת����׼]"		,2		);
	anim("[��ת����]"		,2		);
	anim("[��������]"		,2		);

action_end();


action_begin("��ת����_ѭ��",true); //�����ʱ

	anim("[��ת����]"		,2		);

action_end();

action_begin("�����ڿ���",false);

	anim("[����]"			,2		);
	anim("[���ڿ���]"		,2		);

action_end(); 

action_begin("�������",false);

	anim("[���]"			,2		);

action_end(); 

action_begin("��ת��̧ǹ",false);

	anim("[��ת��̧ǹ]"		,2		);

action_end();

action_begin("��ת����׼",false);

	anim("[��ת����׼]"		,2		);

action_end();

action_begin("��ת����",false);

	anim("[��ת����]"		,2		);

action_end();

action_begin("��ת��̧ǹ",false);

	anim("[��ת��̧ǹ]"		,2		);

action_end();

action_begin("��ת����׼",false);

	anim("[��ת����׼]"		,2		);

action_end();

action_begin("��ת����",false);

	anim("[��ת����]"		,2		);

action_end();

action_begin("�ߴ�����",false);
	
	anim("[�ߴ�����]"	,4		);

action_end();

action_begin("�ߴ���������_ѭ��",true);
	
	anim("[�ߴ���������]"	,4		);

action_end(); 

action_begin("�ߴ��������",false);
	
	anim("[�ߴ��������]"	,4		);

action_end();


//11-13 ������
action_begin("��������",false);
	
	anim("[��������]"	,4		);

action_end();

action_begin("������������_ѭ��",true);
	
	anim("[������������]"	,4		);

action_end(); 

action_begin("�����������",false);
	
	anim("[�����������]"	,4		);

action_end();

//11-18
action_begin("�¶�",false);
	
	anim("[�¶�]"	,2		);

action_end();

//11-22
action_begin("�ز��е�ײǽ����",false);
	
	anim("[�ز��е�ײǽ����]"	,2		);

action_end();

//12-21 ����
action_begin("��_ѭ��",true);
	
	anim("[��ǹǰ��]"	,2		);

action_end();

action_begin("����ƽ��",false);
	
	anim("[����ƽ��]"	,2		);

action_end();

action_begin("����ƽ��_ѭ��",true);
	
	anim("[����ƽ��]"	,2		);

action_end();

action_begin("����ƽ��",false);
	
	anim("[����ƽ��]"	,2		);

action_end();

action_begin("����ƽ��_ѭ��",true);
	
	anim("[����ƽ��]"	,2		);

action_end();

action_begin("��ת��",false);
	
	anim("[��ͨ��ת��]"	,2		);

action_end();

action_begin("��ת��",false);
	
	anim("[��ͨ��ת��]"	,2		);

action_end();

action_begin("�ߴ�����2",false);
	
	anim("[�ߴ�����2]"	,4		);

action_end();

action_begin("�ߴ���������2_ѭ��",true);
	
	anim("[�ߴ���������2]"	,4		);

action_end(); 

action_begin("�ߴ��������2",false);
	
	anim("[�ߴ��������2]"	,4		);

action_end();



file_end();
file_begin("data/model/�Ƴ�1.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	damaged vehicle - NO.1 character define file .
	Create By MengXin 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�Ƴ�1.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/�绰ͤ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�绰ͤ.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/����.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/����.mod");

state_end();

state_begin("");

	model("matters/������.mod");

state_end();
//-------------------------------------------------------------
file_end();
file_begin("data/model/·��01.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/·��01.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/������ǹ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/������ǹģ��_����.mod");

state_end();
define_HH("HH_����",0);

//-------------------------------------------------------------
file_end();
file_begin("data/model/�����01.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�����01.mod");

state_end();
state_begin("");

	model("");

state_end();

convert_begin("Ѱ��","");

	gfx("����_����.gfx","");

convert_end();
//-------------------------------------------------------------
file_end();
file_begin("data/model/·��02.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/·��02.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/��������.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/��������ģ��.mod");

state_end();
//-------------------------------------------------------------
file_end();
file_begin("data/model/�˺���.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	damaged vehicle - NO.1 character define file .
	Create By MengXin 

--------------------------------------------------------------------------*/	
state_begin("Ѱ��");

	model("matters/����Ͱ.mod");

state_end();

convert_begin("Ѱ��","�վ�");

	gfx("����Ͱ��ըЧ��.gfx","");

convert_end();

state_begin("�վ�");

	model("");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/Ħ�г�.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	gfx("","");
	model("characters/Ħ�г�.mod");

state_end();

convert_begin("Ѱ��","����");

	gfx("��ը��Ӱ_��.gfx","");
convert_end();
state_begin("����");
	gfx("����_����.gfx","");
	model("characters/Ħ�г�����.mod");

state_end();

//-------------------------------------------------------------
define_HH("HH_Ħ�г���ը",0);
define_HH("HH_Ħ�б���ǹ��",1);
define_HH("HH_Ħ�г���ʻԱ",2);


//11-21
action_begin("����_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("ͣ��",false);
	
	anim("[ͣ��]"	,2		);

action_end(); file_end();
file_begin("data/model/����Ͱ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/����Ͱ.mod");

state_end();

convert_begin("Ѱ��","�վ�");
	gfx("����_����Ͱ.gfx","");
	gfx("��ը��Ӱ_��.gfx","");	
convert_end();

state_begin("�վ�");

	model("");

state_end();
//-------------------------------------------------------------
file_end();
file_begin("data/model/�˱�������.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	gfx("","");
	model("characters/�����˱���_����.mod");

state_end();

convert_begin("Ѱ��","����");

	gfx("��ը��Ӱ_��.gfx","");
convert_end();
state_begin("����");
	gfx("����_����.gfx","");
	model("characters/�����˱�������.mod");

state_end();

//-------------------------------------------------------------

define_HH("HH_�������䳵��ը",0);
define_HH("HH_ʿ��05",1);
define_HH("HH_ʿ��04",2);
define_HH("HH_ʿ��01",3);
define_HH("HH_ʿ��03",4);
define_HH("HH_ʿ��02",5);
define_HH("HH_��ͨľ��01",6);
define_HH("HH_����Ͱ",7);

action_begin("����_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("ͣ��",false);
	
	anim("[ͣ��]"	,2		);

action_end(); 



file_end();
file_begin("data/model/ɳ��04.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	damaged vehicle - NO.2 character define file .
	Create By MengXin 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�ֵ�ɳ������04.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/��װ��02.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	damaged vehicle - NO.2 character define file .
	Create By MengXin 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/��װ��02.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/��װ��03.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	damaged vehicle - NO.2 character define file .
	Create By MengXin 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/��װ��03.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/��װ��01.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	damaged vehicle - NO.2 character define file .
	Create By MengXin 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/��װ��01.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/����ͷĿ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("characters/����ͷĿ_Ӳ����.mod");

state_end();

state_begin("");

	model("");

state_end();

//
action_begin("�����ܷ���",false);
	
	anim("[�����ܷ���]"	,2		);

action_end(); 

action_begin("�����ܷ���",false);
	
	anim("[�����ܷ���]"	,2		);

action_end(); 

action_begin("�����ܷ���_ѭ��",true);
	
	anim("[�����ܷ���]"	,2		);

action_end(); 

action_begin("�����ܷ���_ѭ��",true);
	
	anim("[�����ܷ���]"	,2		);

action_end(); 

action_begin("��ǰ��_ѭ��",true);
	
	anim("[��ǰ��]"	,2		);

action_end(); 

action_begin("��ǰ�ܻӵ�",false);
	
	anim("[��ǰ�ܻӵ�]"	,2		);

action_end(); 

action_begin("��ǰ�ܻӵ�_ѭ��",true);
	
	anim("[��ǰ�ܻӵ�]"	,2		);

action_end(); 

action_begin("վ������",false);
	
	anim("[վ������]"	,2		);

action_end(); 

action_begin("վ�����ڿ���_ѭ��",true);
	
	anim("[վ�����ڿ���]"	,2		);

action_end(); 

action_begin("վ�����������¶�",false);
	
	anim("[���]"	,2		);

action_end(); 

action_begin("�����¶�_ѭ��",true);
	
	anim("[���׼������]"	,2		);

action_end(); 

action_begin("�¶׻ӵ�����",false);
	
	anim("[�¶׻ӵ�����]"	,2		);

action_end(); 

action_begin("�¶׻ӵ�����_ѭ��",true);
	
	anim("[�¶׻ӵ�����]"	,2		);

action_end(); 

action_begin("�¶��������",false);
	
	anim("[�¶��������]"	,2		);

action_end(); 

action_begin("�¶���������ڿ���_ѭ��",true);
	
	anim("[�¶�������˿���]"	,2		);

action_end(); 

action_begin("�¶�����������",false);
	
	anim("[�¶�����������]"	,2		);

action_end(); 

action_begin("��շ�",false);
	
	anim("[��շ�]"	,2		);

action_end(); 

action_begin("��շ�_ѭ��",true);
	
	anim("[��շ�]"	,2		);

action_end(); 


//11/08 14:25 ����
action_begin("����_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 

action_begin("����_ѭ��",true);
	
	anim("[����]"	,2		);

action_end(); 


//11-13 ������
action_begin("��������",false);
	
	anim("[��������]"	,4		);

action_end();

action_begin("������������_ѭ��",true);
	
	anim("[������������]"	,4		);

action_end(); 

action_begin("�����������",false);
	
	anim("[�����������]"	,4		);

action_end();

//12-17 ������
action_begin("վ����շ�",false);
	
	anim("[վ����շ�]"	,4		);

action_end();

action_begin("�¶׺�շ�",false);
	
	anim("[�¶׺�շ�]"	,4		);

action_end();

action_begin("ǰ�շ�",false);
	
	anim("[ǰ�շ�]"	,4		);

action_end();
file_end();
file_begin("data/model/���۰��濨���.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/���۰��濨���.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/������ķɭ���ǹ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/������ķɭ���ǹ.mod");

state_end();
define_HH("HH_����",0);

//-------------------------------------------------------------
file_end();
file_begin("data/model/�����ػ�ǹ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/�����ػ�ǹ.mod");

state_end();
define_HH("HH_����",0);

//-------------------------------------------------------------
file_end();
file_begin("data/model/����ëɪ��ǹ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/����ëɪ��ǹ.mod");

state_end();
define_HH("HH_����",0);

//-------------------------------------------------------------
file_end();
file_begin("data/model/����Ͱ2.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/����Ͱ2.mod");

state_end();

convert_begin("Ѱ��","�վ�");

	gfx("����_����Ͱ.gfx","");
	gfx("��ը��Ӱ_��.gfx","");
convert_end();

state_begin("�վ�");

	model("");

state_end();
//-------------------------------------------------------------
file_end();
file_begin("data/model/��������.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/��������.mod");

state_end();

//-------------------------------------------------------------
file_end();
file_begin("data/model/���۽ݿ����ǹ.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.7 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	model("matters/���۽ݿ����ǹ.mod");

state_end();
define_HH("HH_����",0);

//-------------------------------------------------------------
file_end();
file_begin("data/model/ǿ��װ�׳�.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	gfx("","");
	model("characters/ǿ��װ�׳�.mod");

state_end();

state_begin("");

	gfx("����_����.gfx","");
	model("characters/ǿ��װ�׳�����.mod");

state_end();
convert_begin("Ѱ��","");

	gfx("��ը��Ӱ_��.gfx","");
convert_end();
//-------------------------------------------------------------
define_HH("HH_ǿ��װ�׳���ը",0);


//
action_begin("����_ѭ��",true);
	
	anim("[����]"	,4		);

action_end(); 

action_begin("����_ѭ��",true,10.0);
	
	anim("[����]"	,4		);

action_end(); 


action_begin("����",false);
	
	anim("[����]"	,4		);

action_end(); 

action_begin("ͣ��",false);
	
	anim("[ͣ��]"	,4		);

action_end(); 

define_HH("HH_ǿ��װ�׳�_����",1);
file_end();
file_begin("data/model/װ�׳�.txt");
/*--------------------------------------------------------------------------

	Copy Right EpieGame 
 	Japanese soldier - NO.1 character define file .
	Create By MengXin 
	This is sample file 

--------------------------------------------------------------------------*/	

state_begin("Ѱ��");

	gfx("","");
	model("characters/װ�׳�.mod");

state_end();

convert_begin("Ѱ��","����");

	gfx("��ը��Ӱ_��.gfx","");
convert_end();
state_begin("����");
	gfx("����_����.gfx","");
	model("characters/װ�׳�����.mod");

state_end();

//
action_begin("����_ѭ��",true);
	
	anim("[����]"	,4		);

action_end(); 

action_begin("����_ѭ��",true,10.0);
	
	anim("[����]"	,4		);

action_end(); 


action_begin("����",false);
	
	anim("[����]"	,4		);

action_end(); 

action_begin("ͣ��",false);
	
	anim("[ͣ��]"	,4		);

action_end(); 

define_HH("HH_װ�׳�����",0);

file_end();

file_begin("data/model/����.txt");
state_begin("Ѱ��");

	model("matters/̫ʦ��.mod");

state_end();

state_begin("");

	model("");

state_end();

//-------------------------------------------------------------
file_end();

}