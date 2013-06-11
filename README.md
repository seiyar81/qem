# Qem

Qem stands for Qt Eventable Models.

It provides an easy way of creating and working with models only meant to store data.

# Why Qem ?

Qem is meant to provide an easy way of writing and working with models, but with one the main feature of Qt : the signals/slots mechanism.

The idea behind Qem is simple : you create a model, you write some data in it, and then you watch it.

# Building Qem

Clone this repository : 
	git clone git://github.com/seiyar81/qem.git
	cd qem

Qem uses CMake so : 
	mkdir build
	cd build
	cmake ..
	make
	sudo make install

Replace make with mingw32-make or nmake given your target OS.
Currently tested on : 
* Linux 32 bits with GCC 4.7.3
* Windows 7 64 bits with GCC 4.7.2
* Mac OS with GCC 4.7.2
* Working with Qt 4.8.1 to Qt 5.0.2

Build & Run tests : 
	-DWITH_TESTS=1

Build type : 
	-DQEM_BUILD_TYPE=SHARED/STATIC

# Using

Qem is meant to be used with CMake.
See the tests CMakeLists.txt.

# How does it work ?

## Macros

Qem mainly uses macros to provide a easy way of writing models.

This is great for providing developer-friendly and easy to use features, but it raises a few problems : 
* Auto-completion in your IDE, some work has been done to improve that point, and it should work fine now in most IDE
* Qt's MOC preprocessor doesn't expand macros and as it is the point of Qem it's kind of a big deal !

The solution to this is for Qem to have his own preprocessor generator.

qemPreproc is a simple tool that you need to call (there is also a CMake macro to ease your pain ;) ) before MOC on the header files which contains QEM macros.
This tool will generate empty class definitions with the needed signals to give MOC the expanded versions of our macros.

You can call the tool manually : 
	qcePreProc [-o<outputFile>] <inputFile>

The output can then be given to the MOC.

## Ids

A model must be declarated with a defined set of members, each defined with a name and a metatype.
An agency is then used to create and/or delete models.
But you cannot directly access a model object, only its id.

You will use this id to create the following workers : Reader, Writer, Watcher.

# Let's see some code

## Declaring a metatype

Qem provide a unique macro for that : QEM_MODEL_FIELD

With basic types : 

	QEM_MODEL_FIELD(int, int)
	QEM_MODEL_FIELD(unsigned long, ulong)
	QEM_MODEL_FIELD(bool, bool)

With std types : 

	QEM_MODEL_FIELD(std::string, string)
	QEM_MODEL_FIELD(std::vector<int>, vectorInt)

With Qt types : 
	
	QEM_MODEL_FIELD(QString, qString)	

Declaring those metatypes is mandatory to use Qem, you will understand why.

## Declaring a model

Qem provides the following macro QEM_MODEL.

It's easy to use : 

	QEM_MODEL(TestModel, 3,
		(Id,		int),
		(Name,		qString),
		(BirthDate,	ulong)
	)

Always take three arguments : Model name, Member count, Members list
Each model member is declarated by specifiying a name and a metatype as declared with the QEM_MODEL_FIELD macro..

## Declare a model aggregator

	QEM_MODEL_AGGREGATOR(ModelAggregator, 2,
        	(TestModel),
	        (TestModel2)
	)
        
A model aggregator simply works as a proxy for managing multiple models.

## Qem in use

Every proram using Qem should call Qem::init() before creating any models.
And Qem::shutdown() before closing the program.

Those methods make sure that we are creating and deleting our models in the right way.

### Getting an id

	Qem::ModelId id = Qem::TestModel::createModel();

### Write to the model

	Qem::TestModel::Writer writer(id);
	writer->setId(42);

### Read from the model

	Qem::TestModel::Reader reader(id);
	reader->getId();

### Watch the model for changes

The watcher part is still tricky but I'm working on having a proper way to connect to the model updates.
For now you will receive the id of the updated model. So it forces you to know which model have been updated.

First you must have a QObject based class with some slots defined.
Then you have to create an instance of TestModel::Watcher, and connect it to your object's slots.
A full example : 

	class TestModelWatcher : public QObject
	{
		Q_OBJECT
		public:	
			TestModelWatcher() : QObject()
			{}

		public slots:
			void onIdUpdated(const Qem::ModelId & id)
			{
				Qem::TestModel::Reader reader(id);
				qDebug() << "TestModelWatcher::onIdUpdated" << reader.getId();
			}
	};


	TestModelWatcher *modelWatcher = new TestModelWatcher();

	Qem::TestModel::Watcher watcher(id);
	watcher.onIdUpdated( modelWatcher, SLOT(onIdUpdated( const Qem::ModelId &)) );

	Qem::TestModel::Writer writer(id);
	writer.setId(42); // The onIdUpdated slot is called

# What's next

* I want the watcher feature to be more developer-friendly, which means, less code, more possibilities.
* Some fashion like : choosing your own namespace for the models, enable inheritance etc ...
* Any suggestions ;)
