#ifndef AGENCY_H_INCLUDED
#define AGENCY_H_INCLUDED

#include "qem_modelfield.h"
#include <set>
#include <map>
#include <memory>

#include <iostream>
#include <typeinfo>

namespace Qem
{
	template<typename T>
	class Agency;
	template<typename T>
	class AggregatorAgency;

    class QEM_EXPORT AgencyManager
	{
		public:
			static void init();

			static void shutdown();

			static bool empty();

		private:
			template<typename T>
			friend class Agency;
			template<typename T>
			friend class AggregatorAgency;

			static void registerAgency( const Qem::ModelId & );

			static void unregisterAgency( const Qem::ModelId & );

			static std::set< Qem::ModelId > m_runningAgencies;
	};

	template<typename T>
    class QEM_EXPORT Agency
	{
		public:
			static Agency&							instance()
			{
				if(!m_instance)
					m_instance = new Agency<T>();
				QEM_ASSERT(m_instance, "Error while creating Agency instance");
				return *m_instance;
			}
			static void								clear()
			{
				QEM_ASSERT(m_instance, "Cannot clear Agency instance, instance is null");
				m_instance->clearModels();
			}
			Qem::ModelId& createModel()
			{
				T* model = new T(Qem::ModelId::random(++m_nextId));
				QEM_ASSERT(m_models.find(model->m_modelId) == m_models.end(), "Cannot create model, ID is already registered");
				m_models.insert( std::make_pair(model->m_modelId, model) );
				return model->m_modelId;
			}

			void destroyModel(const Qem::ModelId & id)
			{
				QEM_ASSERT(m_models.find(id) != m_models.end(), "Cannot destroy model, ID is not registered");
				T* t = m_models.find(id)->second;
				delete t;
				m_models.erase( m_models.find(id) );
				if(m_models.empty())
				{
					Agency<T>::m_instance = 0;
					delete this;
				}
			}
		private:
			friend class T::Watcher;
			friend class T::Writer;
			friend class T::Reader;

			Agency() : m_nextId( 0 )
			{
				m_agencyId = Qem::ModelId::random(++m_nextId);
				AgencyManager::registerAgency(m_agencyId);
			}

			~Agency()
			{
				checkModels();
				AgencyManager::unregisterAgency(m_agencyId);
			}

			void clearModels()
			{
				typename std::map<Qem::ModelId, T*>::iterator it(m_models.begin());
				while(it != m_models.end())
				{
					delete (*it).second;
					++it;
				}
				m_models.clear();
				Agency<T>::m_instance = 0;
				delete this;
			}

			void checkModels()
			{
				QEM_ASSERT(m_models.empty(), "Models map is not empty");
			}

			static T* model(const Qem::ModelId & id)
			{
				QEM_ASSERT(m_instance, "Cannot access agency, no model registered");
				QEM_ASSERT(m_instance->m_models.find(id) != m_instance->m_models.end(), "Cannot find model, ID is not registered");
				return (*m_instance->m_models.find(id)).second;
			}

			static Agency*							m_instance;
			std::map<Qem::ModelId, T*>				m_models;
			unsigned long							m_nextId;

			Qem::ModelId							m_agencyId;
	};
	template<typename T>
	Agency<T>* Agency<T>::m_instance = 0;

	template<typename T>
	class AggregatorAgency
	{
		public:
			static AggregatorAgency&							instance()
			{
				if(!m_instance)
					m_instance = new AggregatorAgency<T>();
				QEM_ASSERT(m_instance, "Error while creating Agency instance");
				return *m_instance;
			}
			static void								clear()
			{
				QEM_ASSERT(m_instance, "Cannot clear Agency instance, instance is null");
				m_instance->clearModels();
			}
			Qem::ModelId& createModel()
			{
				T* model = new T(Qem::ModelId::random(++m_nextId));
				QEM_ASSERT(m_models.find(model->m_modelId) == m_models.end(), "Cannot create model, ID is already registered");
				m_models.insert( std::make_pair(model->m_modelId, model) );
				return model->m_modelId;
			}

			void destroyModel(const Qem::ModelId & id)
			{
				QEM_ASSERT(m_models.find(id) != m_models.end(), "Cannot destroy model, ID is not registered");
				T* t = m_models.find(id)->second;
				t->deleteAggregates();
				delete t;
				m_models.erase( m_models.find(id) );
				if(m_models.empty())
				{
					AggregatorAgency<T>::m_instance = 0;
					delete this;
				}
			}
		private:
			friend class T::Reader;

			AggregatorAgency() : m_nextId( 0 )
			{
				m_agencyId = Qem::ModelId::random(++m_nextId);
				AgencyManager::registerAgency(m_agencyId);
			}

			~AggregatorAgency()
			{
				checkModels();
				AgencyManager::unregisterAgency(m_agencyId);
			}

			void clearModels()
			{
				typename std::map<Qem::ModelId, T*>::iterator it(m_models.begin());
				while(it != m_models.end())
				{
					delete (*it).second;
					++it;
				}
				m_models.clear();
				AggregatorAgency<T>::m_instance = 0;
				delete this;
			}

			void checkModels()
			{
				QEM_ASSERT(m_models.empty(), "Models map is not empty");
			}

			static T* model(const Qem::ModelId & id)
			{
				QEM_ASSERT(m_instance, "Cannot access agency, no model registered");
				QEM_ASSERT(m_instance->m_models.find(id) != m_instance->m_models.end(), "Cannot find model, ID is not registered");
				return (*m_instance->m_models.find(id)).second;
			}

			static AggregatorAgency*				m_instance;
			std::map<Qem::ModelId, T* >				m_models;
			unsigned long							m_nextId;

			Qem::ModelId							m_agencyId;
	};
	template<typename T>
	AggregatorAgency<T>* AggregatorAgency<T>::m_instance = 0;
}

#endif
