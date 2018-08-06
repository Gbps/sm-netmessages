#pragma once
#pragma warning( push )
#pragma warning( disable : 4005)

#include <memory> 
#include <IHandleSys.h>
#include "smsdk_ext.h"

#pragma warning( pop ) 

namespace SourceModNetMessages
{
	using namespace SourceMod;
	/**
	* \brief Helps delete handles
	*/
	template<typename TName>
	class CSmNewDeleter : public IHandleTypeDispatch
	{
	protected:
		~CSmNewDeleter() = default;
	public:
		inline void OnHandleDestroy(HandleType_t type, void* object) override
		{
			delete reinterpret_cast<TName*>(object);
		}
	};

	/**
	* \brief Helps delete handles
	*/
	template<typename TName>
	class CSmNoDelete : public IHandleTypeDispatch
	{
	protected:
		~CSmNoDelete() = default;
	public:
		inline void OnHandleDestroy(HandleType_t type, void* object) override
		{
			delete reinterpret_cast<TName*>(object);
		}
	};

	/**
	* \brief A SourceMod handle which creates and destroys a heap object.
	*/
	template<typename ObjectType>
	class CSmObject
	{
	protected:
		/**
		* \brief The object this wraps around
		*/
		std::unique_ptr<ObjectType> m_Object;

		/**
		* \brief The class that implements cleanup
		*/
		static CSmNewDeleter<ObjectType> Handler;

		CSmObject()
		{

		}
	public:

		/**
		* \brief The sourcemod handle type
		*/
		static SourceMod::HandleType_t HandleType;

		CSmObject(std::unique_ptr<ObjectType> obj)
		{
			m_Object = std::move(obj);
		}

		/**
		* \brief Create a SourceMod plugin handle to this object
		* \param pCtx The plugin context
		* \return A handle to this object or an error
		*/
		cell_t CreateHandle(IPluginContext* pCtx) const;

		/**
		* \brief Get the bitbuf from a handle
		* \param handle Handle to the SM object
		* \return The object or NULL on handle error
		*/
		static CSmObject<ObjectType>* FromHandle(cell_t handle);

		/**
		* \brief Get the wrapped object
		* \return Get the wrapped object
		*/
		ObjectType* Get();

		/**
		* \brief Initializes the SourceMod handle types
		* \param error_out SM Error message buffer
		* \param err_max err_max
		* \return
		*/
		static bool InitHandleType(const char* name, char* error_out, size_t err_max);

		/**
		* \brief Remove the handle type
		*/
		static void RemoveHandleType();

	};

	template <typename ObjectType>
	CSmNewDeleter<ObjectType> CSmObject<ObjectType>::Handler;

	template <typename ObjectType>
	SourceMod::HandleType_t CSmObject<ObjectType>::HandleType;

	template <typename ObjectType>
	cell_t CSmObject<ObjectType>::CreateHandle(IPluginContext* pCtx) const
	{
		HandleError err = HandleError_None;
		const auto hndl = handlesys->CreateHandle(HandleType, (void*)this, nullptr, myself->GetIdentity(), &err);
		if (err != HandleError_None || hndl == 0)
		{
			return pCtx->ThrowNativeError("Invalid NetMessages handle %x (error %d)", hndl, err);
		}

		return hndl;
	}

	template <typename ObjectType>
	CSmObject<ObjectType>* CSmObject<ObjectType>::FromHandle(cell_t handle)
	{
		const Handle_t hndl = static_cast<Handle_t>(handle);
		HandleError herr = HandleError_None;
		HandleSecurity sec;
		CSmObject<ObjectType> *pObject;

		sec.pOwner = nullptr;
		sec.pIdentity = myself->GetIdentity();

		if ((herr = handlesys->ReadHandle(hndl, HandleType, &sec, reinterpret_cast<void **>(&pObject)))
			!= HandleError_None)
		{
			return nullptr;
		}

		return pObject;
	}

	// Yeah... this is wrong. Don't judge me.
	template <typename ObjectType>
	ObjectType* CSmObject<ObjectType>::Get()
	{
		return m_Object.get();
	}

	template <typename ObjectType>
	bool CSmObject<ObjectType>::InitHandleType(const char* name, char* error_out, size_t err_max)
	{
		HandleAccess sec;

		handlesys->InitAccessDefaults(nullptr, &sec);
		sec.access[HandleAccess_Delete] = HANDLE_RESTRICT_IDENTITY;

		HandleError err = HandleError_None;

		HandleType = handlesys->CreateType(
			name,
			dynamic_cast<IHandleTypeDispatch*>(&Handler),
			0,
			nullptr,
			nullptr,
			myself->GetIdentity(),
			&err);

		if (err != HandleError_None && HandleType != 0)
		{
			sprintf(error_out, "Could not init Handle Type: %d", err);
			return false;
		}

		return true;
	}

	template <typename ObjectType>
	void CSmObject<ObjectType>::RemoveHandleType()
	{
		handlesys->RemoveType(HandleType, myself->GetIdentity());
	}

}

