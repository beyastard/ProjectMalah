#ifndef __AMODEL_WRAPER1_H__
#define __AMODEL_WRAPER1_H__


/***********************************************************************
 * FILE: AModelWraper1.H
 *
 * DESCRIPTION : wraper classes to introduce Engine Model class to 
 *				 application for easy using	
 *
 * CREATED BY: MengXin
 *
 * HISTORY:	2002 - 10 - 8
 *
 * Copyright (C) Beijing E-Pie Entertainment & Technology Co., Ltd.
 ************************************************************************/

#include "ADebug.h"
#include "ABaseA3D.h"
#include "AUString.h"
#include "AUList.h"
#include "AUnitInWorldInterface.h"
#include "StlHeader.h"

struct i_host_listener;

class  AModelWraper
{
public:
	
	//�õ�֧�ֵĶ�����
	static bool GetAnimationNameSet(const char * file_name,AListT<AUString>&res);
	//�õ�֧�ֵĹҵ���
	static bool GetHHNameSet(const char * file_name,AListT<HH_infor>& res);
	//�漰��ģ����
	static bool GetInvolvedModel(const char * file_name,AListT<AUString>& res);
	//�漰��Ч����
	static bool GetInvolvedGFX(const char * file_name,AListT<AUString>& res);
	//�߼��¼��ص�
	static void call_back_logic_event(A3DMODEL_LOGIC_EVENT * pLogicEvent, LPVOID pArg);
	//�ͼ����������ص�
	static void call_back_action_end(A3DModel * pNoUsing, LPVOID pArg);
	//����ģ�ͼ���
	static void SetConfigFileModelLevel(MODEL_LEVEL level);
	//ģ�ͼ���
	static MODEL_LEVEL model_level;

private:

	//�ڲ�ʹ�õİ������ȼ��Ķ�����Ŀ
	struct action_item
	{	
		string name;
		string loop_name;
		int	   loop_delay;
		int	   privilege;
		void   * p_outer_data;
		bool operator == (const action_item& v)
		{
			return v.name == name;
		}
		action_item():loop_delay(0),privilege(2),p_outer_data(0){}
	};

	int current_anim_privilege;
	//ѭ����
	string next_call_anim;
	//ѭ���ӳ�
	int    next_call_delay;
	//���������еĶ�����
	string	anim_in_engine;
		
private:

	//�����ӿ�,����Ϊ��
	i_host_listener * m_pHostListener;

	//����ӿ�,�������
	A3DEngine* m_pEngine;

	//�����ļ���
	string	config_file_name;
	
	//ǰһ��״̬��
	string	last_state;

	//��ǰ�����������Ҫִ�еĵͼ���������
	list<action_item>	current_action_queue;

	//�Ƿ���Բ�����һ�������Ŀ��أ��ɶ������ź����ر�
	//�����ص���������
	bool	play_anim_ready;
	//��ǰ״̬������
	int		current_state_index;
	
	//��ģ��
	A3DModel * pCurrentModel;

	//����ģ��
	map<string,A3DModel *>	assistant_model_map; 

	//����ģ�͹�����Ϣ
	map<string,pair<string,string> >	assistant_model_link_infor; 
	
	//�߼���̬��Ϣ
	A3DVECTOR3 last_pos;
	A3DVECTOR3 last_dir;
	A3DVECTOR3 last_up;
	//�ҵ�ĵļ���
	vector<string >		connect_name_vector;	
	vector<A3DFrame * >	connect_ptr_vector;	

public:
	//----------------------------------------------------------------------
	//״̬������״̬��Ӧ��ģ������ӳ��
	map<string,string>									state_model_map;

	//״̬������״̬��ӦЧ��������ҵ��ӳ��
	map<string,vector<pair<string,string> > >			state_gfx_map;

	//ǰһ��״̬����һ��״̬�ı仯 �ñ仯��Ӧ��Ч������ҵ�
	map<pair<string,string> , vector<pair<string,string> > >	convert_gfx_map;

	//�߼����������ͼ��������е�ӳ��
	map<string,list<action_item> >						action_serial_map;

	//���������ļ���ȡ�Ŀ�ѡ��״̬����
	vector<string>										state_queue;

	//���������ļ���ȡ�Ŀ�ѡ�Ķ�������
	vector<string>										anim_queue;

	//��ʼ��ʱ��״̬��
	string												init_state;

	//״̬�ĳ�������
	map<string,string>									state_init_anim;	
		
	//------------------------------------------------------------------------------
	
		
private:

	//������������������Ӧ���ж�,���Ǹ���Ψһ��Ҫÿ�� TICK �������õĺ���
	//���� TickAnimation ���ã�
	bool handleActionQueue();
	//װ��ģ�ͣ��ú�����û�н���Ԥװ�ص�����»��пɹ۵�ʱ�俪��
	bool loadModel(const char* model_name);
	//ж��ģ��
	void releaseModel();
	//����Ч�����ú�����û�н���Ԥװ�ص�����»��пɹ۵�ʱ�俪��
	bool playGFX(const char* gfx_name);
	//����Ч�����ú�����û�н���Ԥװ�ص�����»��пɹ۵�ʱ�俪��
	bool playGFX(const char* gfx_name,const A3DVECTOR3& pos);
	//����Ч�������ҵ㣩���ú�����û�н���Ԥװ�ص�����»��пɹ۵�ʱ�俪��
	bool playGFXonModel(const char* gfx_name,const char* link_point_name);
	//�ݹ�Ѱ�ҹҵ�
	void recursionFindConnectPoint(A3DFrame * pFrame);
	//��д�ҵ㼯��
	void fillConnectPointSet();

public:
	//��װ����ģ��
	bool AppendAssistantModel(const char* model_name,const char* hh_name = "" , const char* cc_name = "" );
	//ɾ������ģ��
	bool DeleteAssistantModel(const char* model_name);
	//֪ͨ����ģ�Ͳ��Ŷ���
	bool NoitfyAssistantModelPlayAnim(const char* model_name,const char * anim_name);
	//֪ͨ����ģ�Ͳ���Ч��
	bool NoitfyAssistantModelPlayGFX(const char* model_name,const char * gfx_name);
	//֪ͨ����ģ�Ͳ���Ч�������ҵ㣩
	bool NoitfyAssistantModelPlayGFXOnModel(const char* model_name,const char * gfx_name,const char * hh_name);
	//�õ�����ģ��
	A3DModel * GetAssistantModel(const char* model_name);
	//���Ŷ���
	bool PlayGFX(const char* gfx_name);
	bool PlayGFX(const char* gfx_name,const A3DVECTOR3& pos);
	bool PlayGFX(const char* gfx_name,const char* hh_name);

public:

	//�������
	A3DModel * GetA3DModel();
	A3DVECTOR3 GetPos();
	A3DVECTOR3 GetDir();
	A3DVECTOR3 GetUp();

	A3DVECTOR3 GetRootFramePos();
	A3DVECTOR3 GetRootFrameDir();
	A3DVECTOR3 GetRootFrameUp();

	void SetPos(const A3DVECTOR3& p);
	void SetDirAndUp(const A3DVECTOR3& d,const A3DVECTOR3& u);
	A3DVECTOR3 GetModelCenterPoint();
	A3DVECTOR3 GetModelSize();
	bool Render(A3DViewport * pCurrentViewport);
	bool TickAnimation();
//	bool AABBTrace(AABBTRACEINFO* pInfo, AABBTRACE* pTrace);
	bool RayTrace(const A3DVECTOR3& vecStart, const A3DVECTOR3& vecDelta, RAYTRACE * pRayTrace);

public:

	AModelWraper();
	~AModelWraper();
	//���ⱻ����ֵ����ʹ�ã�ֻ�������ȥʵ��
	AModelWraper operator = (const AModelWraper&);
	AModelWraper (const AModelWraper&);

public:

	//��������ӿںͷ����ӿ�
	void BindEngineAndListener(A3DEngine* pe,i_host_listener * pl = 0);

	//���������ļ����ú���ʱ�俪���ϴ�
	bool LoadConfigFile(const char * file_name);

	//���������ַ������ú���ʱ�俪���ϴ�
	bool LoadConfigString(const char * config_string);


	vector<string >&		EnumHHName();
	vector<A3DFrame * >&	EnumHHPtr();
	vector<string>&		EnumSupportAnimName();
	vector<string>&		EnumSupportStateName();

public:

	//ǰ��״̬���п���

	//����״̬���У�����״̬���ڵ�һ��״̬��������Դ
	bool Begin();

	//ԾǨ����һ��״̬��������Դ����յ�ǰִ�ж��У�����Ч��
	bool NextState();

	//�뿪״̬���У���״̬��Ϊ�գ��ͷ���Դ
	void End();

	//�õ�״̬����Ԫ�صĸ���������������Ԫ�ظ�����
	int  GetStateCount();

	//�õ���ǰ״̬�ǵڼ���������������Ԫ��������//index based 0
	int	 GetState();

	//���״̬���ƣ��������ʹ����Ϊ��Ӧ�ı任������û�ж����������

	//���õ�ָ����״̬
	bool SetStateByName(const char * state_name);

	//�õ���ǰ״̬����
	string GetCurrentStateName();

public:
	
	//��������

	//��ִ�ж�����ѹ��߼�������
	//���÷���Ҫ��ÿ��TICK��������ͬ�ĸ߼�������
	//����ģ��ֻ�����������������ǣ�����������ֻ��ʹִ�ж���
	//����ֹ�����ͣ�����ѭ���Զ�������һ�����ɣ����Ҳ��ᱻ���
	//��ѭ���Ķ���ÿ��ִ����϶��ᱻ�Ӷ��������

	bool AddActionToHead(const char * state_name,void * p_buffer = 0);
	//һ����������ǰһ���������ǿ�в���ĳ����
	bool AppendActionToTail(const char * state_name,void * p_buffer = 0);
	//���ִ�ж���
	void ClearActionQueue();
	//�����Ƿ�Ϊ��
	bool IsEmptyActionQueue();
	//ǿ�в���ĳ����
	bool ForcePlayAnim(const char * anim_name);
	//���ͼ��������벥�Ŷ���
	bool AppendLowLevelActionToTail(const char * state_name);
	//���ͼ������������ͷ��
	bool AddLowLevelActionToHead(const char * state_name);
	//��ǰ�����Ƿ���ѭ����
	bool IsInLoopAction();
	//��ǰ�������Ƿ����ѭ������
	bool HasLoopInAction();
	//�Ƿ����¿���
	bool IsIdle();
};

#endif//__AMODEL_WRAPER1_H__