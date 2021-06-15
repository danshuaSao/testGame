#include "state_code.h"



std::string state_code::message_def[1000] = {
	"����Ϣ",
	"��¼�ɹ�",
	"�ظ���¼",
	"������Ա",
	"���䲻����",
	"�㲻�Ƿ���",
	"����û����Ա",
	"���û��ȫ��׼��",
	"��Ϸ��ʼ",
	"û���ж���",
	"�����Ѿ�����",
	"Ŀ�����㲻��ͬһ�ص�",
	"�����ɹ�",
	"û���㹻���ӵ�",
	"û���㹻�ı���",
	"Ŀ��������",
	"�ƶ��ɹ�",
	"�����ƶ�����ǰλ��",
	"��������ƶ��غ�",
	"�����ƶ�������",
	"������ȴ��",
	"���Ƴɹ�",
	"Ŀ������ֵ����",
	"�ڿ�ɹ�",
	"û���㹻�Ľ��",
	"���۳ɹ�",
	"����ɹ�",
	"�յ�̽��",
	"̽���ɹ�",
	"̽��ʧ��"
};


void state_code::set(CODE _code)
{
	code = _code;
}

const std::string& state_code::message()
{
	return message_def[code];
}

state_code::operator int() const
{
	return code;
}

bool state_code::operator==(const state_code& sc) const
{
	return code == sc.code;
}

bool state_code::operator==(const CODE _code) const
{
	return code == _code;
}

bool state_code::operator!=(const CODE code) const
{
	return !(*this == code);
}
