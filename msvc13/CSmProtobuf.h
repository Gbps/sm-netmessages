#pragma once

#pragma warning( push )
#pragma warning( disable : 4005)

#include <memory> 
#include <google/protobuf/message.h>
#include "CSmObject.h"
#pragma warning( pop ) 

namespace SourceModNetMessages
{
	using namespace SourceMod;

	/**
	* \brief A SourceMod handle which creates and destroys a google::protobuf::Message object.
	* Protobuf is used in newer source games for netmessages
	*/
	class CSmProtobuf : public CSmObject<SMProtobufMessage>
	{
	public:
		CSmProtobuf(protobuf::Message* msg)
		{
			m_Object = std::unique_ptr<SMProtobufMessage>(new SMProtobufMessage(msg));
		}
	};
}

