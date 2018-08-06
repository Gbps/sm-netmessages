#pragma once

#pragma warning( push )
#pragma warning( disable : 4005)

#include <memory> 
#include "CSmObject.h"

#pragma warning( pop ) 

namespace SourceModNetMessages
{
	using namespace SourceMod;
	/**
	* \brief A SourceMod handle which creates and destroys a bf_write source engine object.
	* bf_write is a low-level interface for writing bits to the outgoing network.
	*/
	class CSmBitBuf : public CSmObject<bf_write>
	{
	protected:
		std::unique_ptr<char[]> m_Memory;

	public:
		CSmBitBuf(int size)
		{
			m_Memory = std::make_unique<char[]>(size);
			m_Object = std::make_unique<bf_write>(m_Memory.get(), size);
		}
	};

}

