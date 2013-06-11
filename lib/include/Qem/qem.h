#ifndef QEM_MACROS
#define QEM_MACROS

//----------------------------------------------------------------------------------------
// Class Declaration
//----------------------------------------------------------------------------------------
#define QEM_MODEL_CLASS_HEADER(NAME)														\
namespace Qem																				\
{																							\
	class NAME	: public QObject															\
	{																						\
		Q_OBJECT																			\
	public:																					\
		class Reader;																		\
		typedef Reader NAME##Reader;														\
																							\
		static Qem::ModelId& createModel()													\
			{ return Qem::Agency<NAME>::instance().createModel(); }							\
		static void destroyModel(const Qem::ModelId & id)									\
			{ Qem::Agency<NAME>::instance().destroyModel(id); }								\
	private:																				\
																							\
		friend class Qem::Agency<NAME>;														\
		explicit NAME( Qem::ModelId id ) : QObject(), m_modelId(id)							\
		{}																					\
		~NAME()																				\
		{}																					\
		Qem::ModelId											m_modelId;					\

#define QEM_MODEL_CLASS_FOOTER(NAME)														\
		private:																			\
			friend class Qem::NAME::Watcher;												\
			friend class Qem::NAME::Writer;													\
			friend class Qem::NAME::Reader;													\
	};																						\
}

#define QEM_WATCHER_CLASS_HEADER(NAME)														\
	public:																					\
		class Watcher																		\
		{																					\
			public:																			\
				Watcher(const Qem::ModelId & id)											\
				{																			\
					m_model = Qem::Agency<NAME>::model(id);									\
				}																			\
				bool isUpdated() const		{ return m_model->m_field.getBit(0); }			\
				void updateNotified()		{ m_model->m_field.reset(false);     }			\
			private:																		\
				NAME* m_model;

#define QEM_WATCHER_CLASS_FOOTER(NAME)														\
	};																						\

#define QEM_WRITER_CLASS_HEADER(NAME)														\
	public:																					\
		class Writer																		\
		{																					\
			public:																			\
				Writer(const Qem::ModelId & id)												\
				{																			\
					m_model = Qem::Agency<NAME>::model(id);									\
				}																			\
			private:																		\
				NAME* m_model;

#define QEM_WRITER_CLASS_FOOTER(NAME)														\
	};																						\

#define QEM_READER_CLASS_HEADER(NAME)														\
	public:																					\
		class Reader																		\
		{																					\
			public:																			\
				Reader(const Qem::ModelId & id)                                             \
				{																			\
					m_model = Qem::Agency<NAME>::model(id);									\
				}																			\
			private:																		\
				NAME* m_model;

#define QEM_READER_CLASS_FOOTER(NAME)														\
	};																						\


#define QEM_MODEL_AGGREGATOR_CLASS_HEADER(NAME)												\
namespace Qem																				\
{																							\
	class NAME	: public QObject															\
	{																						\
		Q_OBJECT																			\
	public:																					\
		static Qem::ModelId& createModel()													\
			{																				\
                return Qem::AggregatorAgency<NAME>::instance().createModel();				\
			}																				\
		static void destroyModel(const Qem::ModelId & id)									\
			{ Qem::AggregatorAgency<NAME>::instance().destroyModel(id); }					\
	private:																				\
																							\
		friend class Qem::AggregatorAgency<NAME>;											\
	explicit NAME( Qem::ModelId id ) : QObject(), m_modelId(id)								\
		{																					\
			this->initAggregates();															\
		}																					\
		~NAME()                         													\
        {																					\
			this->deleteAggregates();														\
        }																					\
																							\
		Qem::ModelId& getModelId( const char * name )										\
		{																					\
			return m_modelsId[name];														\
		}																					\
																							\
		Qem::ModelId							m_modelId;									\
		std::map< const char *,																\
			std::pair< std::function<Qem::ModelId &()>,										\
						std::function<void(const Qem::ModelId &)> > >						\
				m_modelDelegates;															\
		std::map< const char *, Qem::ModelId >	m_modelsId;									\
                                                                                            \
        void deleteAggregates()																\
        {																					\
            for(const auto& i : m_modelDelegates)											\
            {																				\
                i.second.second( m_modelsId[ i.first ] );                                   \
            }                                                                               \
            m_modelsId.clear();                                                             \
            m_modelDelegates.clear();                                                       \
        }                                                                                   \
		void initAggregates()																\
		{																					\
			using namespace std::placeholders;												\


#define QEM_MODEL_AGGREGATOR_READER(NAME)													\
			for(const auto& i : m_modelDelegates)											\
			{																				\
				auto id = i.second.first();													\
				m_modelsId.insert( std::make_pair( i.first, id ));							\
			}																				\
		}																					\
	public:																					\
		class Reader																		\
		{																					\
			public:																			\
				Reader(const Qem::ModelId & id)												\
				{																			\
					m_model = Qem::AggregatorAgency<NAME>::model(id);						\
				}																			\
			private:																		\
				NAME* m_model;																\

#define QEM_MODEL_AGGREGATOR_CLASS_FOOTER(NAME)												\
		};																					\
	};																						\
}

//----------------------------------------------------------------------------------------
// Bit field attribute
//----------------------------------------------------------------------------------------
#define QEM_BIT_FIELD_IMPL(N)																\
	private:																				\
		Qem::BitField<N> m_field;
//----------------------------------------------------------------------------------------
// Name of different components
//----------------------------------------------------------------------------------------
#define QEM_MODEL_FIELD_GETTER(NAME)				get##NAME
#define QEM_MODEL_FIELD_SETTER(NAME)				set##NAME
#define QEM_MODEL_FIELD_VALID(NAME)					is##NAME##Valid
#define QEM_MODEL_FIELD_UPDATED(NAME)				is##NAME##Updated
#define QEM_MODEL_BIND_FIELD_UPDATED(NAME)			bindOn##NAME##Updated
#define QEM_MODEL_FIELD_ATTRIBUTE(NAME)				m_mf##NAME
#define QEM_MODEL_FIELD_TYPE(TYPE)					m##TYPE
#define QEM_META_MODEL_FIELD_NAME(NAME)				MetaModelImpl##NAME
#define QEM_MODEL_FIELD_DATATYPE(NAME)				QEM_META_MODEL_FIELD_NAME(NAME)::data_type
#define QEM_MODEL_ID(NAME)							NAME##Id

#define QEM_MODEL_STR(NAME)							#NAME

#define QEM_WATCH_FIELD_UPDATED(NAME)				on##NAME##Updated
#define QEM_WATCH_FIELD_UPDATED_STR(NAME)			signalOn##NAME##Updated

#define QEM_AGGREGATOR_MODEL_READER(NAME)			NAME::Reader
#define QEM_AGGREGATOR_MODEL_FIELD_GETREADER(NAME)	get##NAME##Reader
#define QEM_AGGREGATOR_MODEL_WRITER(NAME)			NAME::Writer
#define QEM_AGGREGATOR_MODEL_FIELD_GETWRITER(NAME)	get##NAME##Writer
#define QEM_AGGREGATOR_MODEL_WATCHER(NAME)			NAME::Watcher
#define QEM_AGGREGATOR_MODEL_FIELD_GETWATCHER(NAME)	get##NAME##Watcher
#define QEM_AGGREGATOR_MODEL_FIELD_GETID(NAME)      get##NAME##Id

//----------------------------------------------------------------------------------------
// Meta model field (type + id of a given field)
//----------------------------------------------------------------------------------------
#define QEM_META_MODEL_FIELD_IMPL(ID, NAME, TYPE)											\
	public:																					\
	typedef Qem::MetaModelField< QEM_MODEL_FIELD_TYPE(TYPE)::data_type, ID >				\
						QEM_META_MODEL_FIELD_NAME(NAME);
//----------------------------------------------------------------------------------------
// Field method declaration and implementation
//----------------------------------------------------------------------------------------
#define QEM_MODEL_FIELD_IMPL(NAME, TYPE)													\
	public:																					\
		const QEM_MODEL_FIELD_DATATYPE(NAME)& QEM_MODEL_FIELD_GETTER(NAME)() const			\
		{																					\
			return QEM_MODEL_FIELD_ATTRIBUTE(NAME).value();									\
		}																					\
																							\
		void	QEM_MODEL_FIELD_SETTER(NAME)(const QEM_MODEL_FIELD_DATATYPE(NAME)& value)	\
		{																					\
			QEM_MODEL_FIELD_ATTRIBUTE(NAME).setValue(value);								\
			m_field.setBit(QEM_META_MODEL_FIELD_NAME(NAME)::update_id, true);				\
			m_field.setBit(0, true);														\
			emit on##NAME##Updated( m_modelId );											\
		}																					\
																							\
		bool  QEM_MODEL_FIELD_VALID(NAME)() const											\
		{																					\
			return QEM_MODEL_FIELD_ATTRIBUTE(NAME).isValid();								\
		}																					\
																							\
		bool  QEM_MODEL_FIELD_UPDATED(NAME)() const											\
		{																					\
			return m_field.getBit(QEM_META_MODEL_FIELD_NAME(NAME)::update_id);				\
		}																					\
																							\
		template<typename WATCHER>															\
		void QEM_MODEL_BIND_FIELD_UPDATED(NAME)(WATCHER* const instance,const char * slot)	\
		{																					\
				QObject::connect(this,														\
				SIGNAL( on##NAME##Updated( const Qem::ModelId & ) ),						\
								instance, slot);											\
		}																					\
																							\
	signals:																				\
		void QEM_WATCH_FIELD_UPDATED(NAME)( const Qem::ModelId & );							\
																							\
	private:																				\
		QEM_MODEL_FIELD_TYPE(TYPE) QEM_MODEL_FIELD_ATTRIBUTE(NAME);							\

#define QEM_WATCHER_MODEL_FIELD_IMPL(NAME, TYPE)											\
	public:																					\
		template<typename TARGET>															\
		void QEM_WATCH_FIELD_UPDATED(NAME)(TARGET* const instance,	const char * func)		\
		{																					\
				m_model->QEM_MODEL_BIND_FIELD_UPDATED(NAME)(instance, func);				\
		}																					\
		bool  QEM_MODEL_FIELD_VALID(NAME)() const											\
		{																					\
			return  m_model->QEM_MODEL_FIELD_ATTRIBUTE(NAME).isValid();						\
		}																					\
																							\
		bool  QEM_MODEL_FIELD_UPDATED(NAME)() const											\
		{																					\
			return  m_model->m_field.getBit(QEM_META_MODEL_FIELD_NAME(NAME)::update_id);	\
		}																					\

#define QEM_WRITER_MODEL_FIELD_IMPL(NAME, TYPE)												\
	public:																					\
		bool QEM_MODEL_FIELD_SETTER(NAME)(const QEM_MODEL_FIELD_DATATYPE(NAME)& value)		\
		{																					\
			m_model->QEM_MODEL_FIELD_SETTER(NAME)(value);									\
			return m_model->QEM_MODEL_FIELD_VALID(NAME)();                                  \
		}																					\

#define QEM_READER_MODEL_FIELD_IMPL(NAME, TYPE)												\
	public:																					\
		const QEM_MODEL_FIELD_DATATYPE(NAME)& QEM_MODEL_FIELD_GETTER(NAME)() const			\
		{																					\
			return m_model->QEM_MODEL_FIELD_ATTRIBUTE(NAME).value();						\
		}																					\
		bool  QEM_MODEL_FIELD_VALID(NAME)() const											\
		{																					\
			return  m_model->QEM_MODEL_FIELD_ATTRIBUTE(NAME).isValid();						\
		}																					\
																							\
		bool  QEM_MODEL_FIELD_UPDATED(NAME)() const											\
		{																					\
			return  m_model->m_field.getBit(QEM_META_MODEL_FIELD_NAME(NAME)::update_id);	\
		}																					\

#define QEM_AGGREGATOR_INIT_MODEL_FIELD(ID, NAME)											\
		m_modelDelegates.insert( std::make_pair( QEM_MODEL_STR(NAME),						\
				std::make_pair( &NAME::createModel,											\
                        std::bind(&NAME::destroyModel, _1) ) ) );							\

#define QEM_AGGREGATOR_MODEL_FIELD_IMPL(ID, NAME)											\
	public:																					\
		QEM_AGGREGATOR_MODEL_READER(NAME) QEM_AGGREGATOR_MODEL_FIELD_GETREADER(NAME)()		\
		{																					\
			return QEM_AGGREGATOR_MODEL_READER(NAME)(										\
						m_model->getModelId( QEM_MODEL_STR(NAME) ) );						\
		}																					\
		QEM_AGGREGATOR_MODEL_WRITER(NAME) QEM_AGGREGATOR_MODEL_FIELD_GETWRITER(NAME)()		\
		{																					\
			return QEM_AGGREGATOR_MODEL_WRITER(NAME)(										\
						m_model->getModelId( QEM_MODEL_STR(NAME) ) );						\
		}																					\
		QEM_AGGREGATOR_MODEL_WATCHER(NAME) QEM_AGGREGATOR_MODEL_FIELD_GETWATCHER(NAME)()	\
		{																					\
			return QEM_AGGREGATOR_MODEL_WATCHER(NAME)(										\
						m_model->getModelId( QEM_MODEL_STR(NAME) ) );						\
		}																					\
        Qem::ModelId& QEM_AGGREGATOR_MODEL_FIELD_GETID(NAME)()                              \
        {																					\
            return m_model->getModelId( QEM_MODEL_STR(NAME) );                              \
        }																					\



//----------------------------------------------------------------------------------------
// Complete model definitions
//----------------------------------------------------------------------------------------
#define QEM_MODEL_COMPLETION(NAME, N, ...)													\
	QEM_MODEL_CLASS_HEADER(NAME)															\
	PP_CAT(PP_RECURSIVE_IMPL, N)(QEM_META_MODEL_FIELD_IMPL, __VA_ARGS__)					\
	PP_CAT(PP_RECURSIVE_NOARG_IMPL, N)(QEM_MODEL_FIELD_IMPL, __VA_ARGS__)					\
	QEM_BIT_FIELD_IMPL(N + 1)																\
	QEM_WATCHER_CLASS_HEADER(NAME)															\
	PP_CAT(PP_RECURSIVE_NOARG_IMPL, N)(QEM_WATCHER_MODEL_FIELD_IMPL, __VA_ARGS__)			\
	QEM_WATCHER_CLASS_FOOTER(NAME)															\
	QEM_WRITER_CLASS_HEADER(NAME)															\
	PP_CAT(PP_RECURSIVE_NOARG_IMPL, N)(QEM_WRITER_MODEL_FIELD_IMPL, __VA_ARGS__)			\
	QEM_WRITER_CLASS_FOOTER(NAME)															\
	QEM_READER_CLASS_HEADER(NAME)															\
	PP_CAT(PP_RECURSIVE_NOARG_IMPL, N)(QEM_READER_MODEL_FIELD_IMPL, __VA_ARGS__)			\
	QEM_READER_CLASS_FOOTER(NAME)															\
	QEM_MODEL_CLASS_FOOTER(NAME)
//----------------------------------------------------------------------------------------
#define QEM_MODEL(...)																		\
	QEM_MODEL_COMPLETION(__VA_ARGS__)
//----------------------------------------------------------------------------------------
#define QEM_MODEL_AGGREGATOR_COMPLETION(NAME, N, ...)										\
	QEM_MODEL_AGGREGATOR_CLASS_HEADER(NAME)													\
	PP_CAT(PP_RECURSIVE_IMPL, N)(QEM_AGGREGATOR_INIT_MODEL_FIELD, __VA_ARGS__)				\
	QEM_MODEL_AGGREGATOR_READER(NAME)														\
	PP_CAT(PP_RECURSIVE_IMPL, N)(QEM_AGGREGATOR_MODEL_FIELD_IMPL, __VA_ARGS__)				\
	QEM_MODEL_AGGREGATOR_CLASS_FOOTER(NAME)
//----------------------------------------------------------------------------------------
#define QEM_MODEL_AGGREGATOR(...)															\
	QEM_MODEL_AGGREGATOR_COMPLETION(__VA_ARGS__)
//----------------------------------------------------------------------------------------

#if defined(_WIN32)
    #define QEM_EXPORT __declspec(dllexport)
    #define QEM_IMPORT __declspec(dllimport)

    #ifdef _MSC_VER
        #pragma warning(disable : 4251)
    #endif
#else
    #if __GNUC__ >= 4
        #define QEM_EXPORT __attribute__ ((__visibility__ ("default")))
        #define QEM_IMPORT __attribute__ ((__visibility__ ("default")))
    #else
        #define QEM_EXPORT
        #define QEM_IMPORT
    #endif
#endif

namespace Qem
{
    QEM_EXPORT void init();

    QEM_EXPORT void shutdown();
}

#endif //QEM_MACROS
