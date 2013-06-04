#ifndef MODEL_FIELD_H_INCLUDED
#define MODEL_FIELD_H_INCLUDED

#include <Qem/qem.h>

#define QEM_MODELID_INVALID static_cast<unsigned long>(-1)

namespace Qem
{

    class QEM_EXPORT ModelId
	{
		public:
			ModelId();
            ModelId(const unsigned long &);
            ModelId(const ModelId &);
			ModelId(ModelId && id);

			static Qem::ModelId random(const unsigned long & base = 0);

			//-----------------------------------------------------------------------------
			bool isValid() const;

			ModelId& operator=(ModelId&&);

			bool operator<(const ModelId & second) const;

			bool operator>(const ModelId & second) const;

			bool operator==(const ModelId & second) const;

            bool operator!=(const ModelId & second) const;

		private:
			unsigned long m_id;
	};

	template<typename T>
	class ModelField
	{
		public:
						ModelField();
						ModelField(const T& value);

			void		reset();
			void		setValue(const T& value);
			bool		isValid() const;
			bool		isUpdated() const;
			const T&	value() const;

			typedef T data_type;

		private:
			bool		m_valid;
			bool        m_updated;
			T			m_value;
	};

	template<typename T>
	ModelField<T>::ModelField()
	: m_valid(false), m_updated(false), m_value()
	{}

	template<typename T>
	ModelField<T>::ModelField(const T& value)
	: m_valid(true), m_updated(false), m_value(value)
	{}

	template<typename T>
	void ModelField<T>::reset()
	{
		m_valid = false;
		m_updated = false;
	}

	template<typename T>
	void ModelField<T>::setValue(const T& value)
	{
		m_valid = true;
		m_updated = true;
		m_value = value;
	}

	template<typename T>
	bool ModelField<T>::isValid() const
	{
		return m_valid;
	}

	template<typename T>
	bool ModelField<T>::isUpdated() const
	{
		return m_updated;
	}

	template<typename T>
	const T& ModelField<T>::value() const
	{
        QEM_ASSERT(this->isValid(), "Cannot access ModelField value, value is not valid");
		return m_value;
	}

	template<typename T, unsigned int UpdateId >
	struct MetaModelField
	{
		typedef T data_type;
		static const unsigned int update_id = UpdateId;
	};

}

#define QEM_MODEL_FIELD(TYPE, NAME) \
	typedef Qem::ModelField< TYPE >         m##NAME;	\
	typedef Qem::ModelField< TYPE* >		m##NAME##Ptr;	\

#endif//MODEL_FIELD_H_INCLUDED
