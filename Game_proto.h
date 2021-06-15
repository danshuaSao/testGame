/*
* Game_proto���Զ���Ӧ�ò�Э��
*
* ����ʹ��std::string��Ϊ���ݻ�������
*
* ���л���
* ���л�ʹ��boost::serialization��
* ����������Ҫ���л����࣬�䶼Ӧ��boost�ĵ���ָ����һ�������Լ��Ľӿڱ�¶��boost����ָ��Ҫ���л������ݡ�
* �����
* friend class boost::serialization::access
* template<typename Archive>
* void serialize(Archive& ar, const unsigned int version)
*
* Э��ͷ���干64λ������8λ�汾�ţ�8λУ��ħ����16λ����id��32λ������
* ͨ����Э������Ϊ������������serialize_obj�����������л���Э���塣
* ��Ҫ�������ݰ���ͨ������Proto_msg::encode���ɽ����ݰ�ת�����ֽ��������벢����std::string��
* ���ݰ��Ľ��ܶ������ļ�Tcp_connection.h�У���Ҫ�ֶ������ͷ����ɰ�ͷ�����������д��䡣
* decode�������޾�������
*/


#pragma once
#include <cstdint>
#include <boost/asio.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/endian.hpp>
#include <vector>
#include "state_code.h"

using namespace boost::asio;

template<typename T>
void deserialize_obj0(boost::archive::text_iarchive& ia, T& obj)
{
	ia >> obj;
}
template<typename T, typename ...Args>
void deserialize_obj0(boost::archive::text_iarchive& ia, T& obj, Args&... args)
{
	ia >> obj;
	deserialize_obj0(ia, args...);
}
/*
* Desc:	�������л���Ҫ������Ϸ�
* data:	��ʾ�������л��ֽ����Ļ�����
* args:	��ʾ�����л��󸲸ǵĶ���
*/
template<typename ...Args>
void deserialize_obj(const std::string& data, Args&... args)
{
	std::stringstream is(data);
	boost::archive::text_iarchive ia(is);
	deserialize_obj0(ia, args...);
}

template<typename T>
void serialize_obj0(boost::archive::text_oarchive& oa, std::stringstream& os, std::string& buffer, T& obj)
{
	oa << obj;
	buffer.resize(os.str().size());
	for (int i = 0; i < os.str().size(); i++)
		os.get(buffer[i]);
}
template<typename T, typename ...Args>
void serialize_obj0(boost::archive::text_oarchive& oa, std::stringstream& os, std::string& buffer, T& obj, Args&&... args)
{
	oa << obj;
	serialize_obj0(oa, os, buffer, args...);
}
/*
* Desc:		�������л���Ҫ������������boost���л����е����л�������
* buffer:	��ʾ���л���ĳ��string���������������Զ��Ի��������ݣ����䲢����ִ��shrink������
* args:		��ʾҪ���л��Ķ����б�
*/
template<typename ...Args>
void serialize_obj(std::string& buffer, Args&&... args)
{
	std::stringstream os;
	boost::archive::text_oarchive oa(os);
	buffer.clear();
	serialize_obj0(oa, os, buffer, std::forward<Args&>(args)...);
}

/*
* Desc:	������ʹ��boost::endian::big_uint8_tʵ��
* Tag:	serializable
*/
#pragma pack(push,4)
const boost::endian::big_uint8_t PROTO_MAGIC = 0x12;
struct Proto_head
{
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& version;
		ar& magic;
		ar& service;
		ar& len;
	}
	boost::endian::big_uint8_t version;//Э��汾
	boost::endian::big_uint8_t magic;//����У��ħ��
	boost::endian::big_uint16_t service;//����id
	boost::endian::big_uint32_t len;//���峤��
	Proto_head();
	Proto_head(uint16_t _version);
	Proto_head(uint16_t _version, uint16_t _service);
};

struct Proto_msg
{
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& head;
		ar& body;
	}
	Proto_head head;//��ͷ
	std::string body;//����
	std::shared_ptr<std::string> encode();
	void encode(std::string& buffer);
	int decode(const std::string& data);
	Proto_msg();
	Proto_msg(boost::endian::big_uint8_t _version) :head(_version) {}
	Proto_msg(boost::endian::big_uint8_t _version, boost::endian::big_uint16_t _service) :head(_version, _service) {}
	Proto_msg(const std::string& data);
};
#pragma pack(pop)