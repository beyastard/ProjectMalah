#ifndef __MODELUNITDATA__H__
#define __MODELUNITDATA__H__

#include "AData.h"
#include "ABaseA3D.h"
#include "AUString.h"

struct AMODEL_KERNEL_DATUM
{
	// do not move this var. be used as copy starting point.
	A3DVECTOR3			pos;					//��̬		
	// -----------------------------------------------------
	A3DVECTOR3			dir;					//��̬
	A3DVECTOR3			up;						//��̬
	bool				ray_trace_enable;		//�Ƿ�ɱ�ray_trace���
	bool				aabb_trace_enable;		//�Ƿ�ɱ�aabb_trace���
	bool				visibility;				//�Ƿ�ɼ�

	// do not move this var. be used as copy ending point.
	AUString			config_file;			//�����ļ���
	AUString			state;					//״̬

	AMODEL_KERNEL_DATUM():
	ray_trace_enable(true),
	aabb_trace_enable(true),
	visibility(true)
	{
	}

	bool Save(AArchive &ar)
	{
		if(!ar.Write(&pos, ((BYTE*)&config_file) - ((BYTE*)&pos)))
		{
			return false;
		}
		ar.WriteString(config_file);
		ar.WriteString(state);
		return true;
	}

	bool Load(AArchive &ar)
	{
		if(!ar.Read(&pos, ((BYTE*)&config_file) - ((BYTE*)&pos)))
		{
			return false;
		}
		ar.ReadString(config_file);
		ar.ReadString(state);
		return true;
	}
};
typedef AMODEL_KERNEL_DATUM* PAMODEL_KERNEL_DATUM;

class AModelUnitData : public AData
{
public:

	DECLARE_ASERIAL(AModelUnitData)

	STATICPROPERIES_BEGIN(AData, AModelUnitData)
		STATICPROPERTY(AModelUnitData, "λ��", A3DVECTOR3, m_model_datum.pos, NULL, NULL, WAY_DEFAULT, NULL)
		STATICPROPERTY(AModelUnitData, "����", A3DVECTOR3, m_model_datum.dir, NULL, NULL, WAY_DEFAULT, NULL)
		STATICPROPERTY(AModelUnitData, "ˮƽ", A3DVECTOR3, m_model_datum.up, NULL, NULL, WAY_DEFAULT, NULL)
		STATICPROPERTY(AModelUnitData, "�Ƿ���Ա�rayTrace", bool, m_model_datum.ray_trace_enable, NULL, NULL, WAY_DEFAULT, NULL)
		STATICPROPERTY(AModelUnitData, "�Ƿ���Ա�aabbTrace", bool, m_model_datum.aabb_trace_enable, NULL, NULL, WAY_DEFAULT, NULL)
		STATICPROPERTY(AModelUnitData, "�Ƿ�ɼ�", bool, m_model_datum.visibility, NULL, NULL, WAY_DEFAULT, NULL)
		STATICPROPERTY(AModelUnitData, "�����ļ���", AUString, m_model_datum.config_file, NULL, NULL, WAY_DEFAULT, NULL)
		STATICPROPERTY(AModelUnitData, "��ʼ״̬", AUString, m_model_datum.state, NULL, NULL, WAY_DEFAULT, NULL)
	STATICPROPERIES_END(AModelUnitData)

public:

	AMODEL_KERNEL_DATUM	m_model_datum;

public:

	AModelUnitData(){}
	virtual ~AModelUnitData()
	{
		INITIALIZE_STATICPROPERIES(AModelUnitData)
	}
	virtual bool Load(AArchive &ar)
	{
		if(!AData::Load(ar))
		{
			return false;
		}
		int nVersion;
		if(ar.Read(&nVersion,sizeof(nVersion))==false)
		{
			assert(0);
			return false;
		}
		//��������汾�Ž��а汾����

		return m_model_datum.Load(ar);
	}
	virtual bool Save(AArchive &ar)
	{
		if(!AData::Save(ar))
		{
			return false;
		}
		int nVersion = AModelUnitData::GetClassInfo()->GetVersion();
		if(ar.Write(&nVersion,sizeof(nVersion))==false)
		{
			assert(0);
			return false;
		}

		return m_model_datum.Save(ar);
	}

public:
	virtual void SetDefault();

	A3DVECTOR3 GetPos(){return m_model_datum.pos;}
	void SetPos(const A3DVECTOR3& p){m_model_datum.pos = p;}

	A3DVECTOR3 GetDir(){return m_model_datum.dir;}
	void SetDir(const A3DVECTOR3& d){m_model_datum.dir = d;}

	A3DVECTOR3 GetUp(){return m_model_datum.up;}
	void SetUp(const A3DVECTOR3& u){m_model_datum.up = u;}

	bool GetRayTraceEnable(){return m_model_datum.ray_trace_enable;}
	void SetRayTraceEnable(bool b){m_model_datum.ray_trace_enable = b;}

	bool GetAABBTraceEnable(){return m_model_datum.aabb_trace_enable;}
	void SetAABBTraceEnable(bool b){m_model_datum.aabb_trace_enable = b;}
	
	bool GetVisibility(){return m_model_datum.visibility;}
	void SetVisibility(bool b){m_model_datum.visibility = b;}

	AUString GetConfigFileName(){return m_model_datum.config_file;}

	AUString GetState(){return m_model_datum.state;}
	void SetState(const char * s){m_model_datum.state = s;}
};

#endif  // __MODELUNITDATA__H__