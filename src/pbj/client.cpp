////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\src\pbj\client.cpp
///
/// \brief	Implements the client class.
////////////////////////////////////////////////////////////////////////////////
#ifndef CLIENT_H_
#include "pbj/client.h"
#endif

using namespace pbj;

/// \brief	The client instance pointer.
Client* Client::_instance = 0;

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Client::init(U32 fps)
///
/// \brief	Initialises this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	fps	The FPS.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool Client::init(U32 fps)
{
	return init(nullptr, fps);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Client::init(const U8* const addr, U32 fps)
///
/// \brief	Initialises this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	addr	A null-terminated string containing the server's address.
/// \param	fps 	The FPS.
///
/// \return	true if it succeeds, false if it fails.
/// \details	This will do all things necessary to initializing the client,
/// 			including initializing the Transport layer.  If the passed addr
/// 			is \c NULL then it is assumed that we want to check a lobby for
/// 			servers.  Otherwise an attempt is made to connect directly, and
/// 			if it fails then the client closes.  On another note, I don't
/// 			think it's killing us to do all of this init as a static method,
/// 			but a part of me thinks that the static should reference an
/// 			internal startup.
/// 			TODO: add in engine, push this to a non-static method
////////////////////////////////////////////////////////////////////////////////
bool Client::init(const U8* const addr, U32 fps)
{
	assert(!_instance);

	if(!Transport::init())
	{
		PBJ_LOG(pbj::VError) << "Failed to initialize transport layer"
								<< PBJ_LOG_END;
		return false;
	}
	
	_instance = new Client();

	_instance->_transport = Transport::create();

	if(!_instance->_transport)
	{
		PBJ_LOG(pbj::VError) << "Could not create transport" << PBJ_LOG_END;
		return false;
	}
	Transport::Config cfg = _instance->_transport->getConfig();
	PBJ_LOG(pbj::VInfo) << "Transport configuration:" << PBJ_LOG_NL
						<< "Mesh Port:\t"<< cfg.meshPort << PBJ_LOG_NL
						<< "Client Port:\t"<< cfg.clientPort << PBJ_LOG_NL
						<< "Server Port:\t"<< cfg.serverPort << PBJ_LOG_NL
						<< "Beacon Port:\t"<< cfg.beaconPort << PBJ_LOG_NL
						<< "Listener Port:\t"<< cfg.listenerPort << PBJ_LOG_NL
						<< "Proto Id:\t"<< cfg.protoId << PBJ_LOG_NL
						<< "Mesh Send Rate:\t"<< cfg.meshSendRate << PBJ_LOG_NL
						<< "Timeout:\t"<< cfg.timeout << PBJ_LOG_NL
						<< "Max Nodes:\t"<< cfg.maxNodes << PBJ_LOG_END;
	
	//if an address was passed, try direct connect; otherwise enter lobby
	if(addr != nullptr)
	{
		_instance->_transport->connectClient(addr);
		_instance->_state = Connecting;
	}
	else
	{
		_instance->_transport->enterLobby();
		_instance->_state = Searching;
	}
	
	_instance->_dt = 1.0f/fps;
	
	//this goes away when doing more than a demo
	_instance->_sendMsgTimer = 0.0f;
	
	//TODO: fit game engine in
	// Engine init stuff
	
	_instance->_window.registerContextResizeListener([=](I32 width, I32 height) { _instance->onContextResized(width, height); });

    InputController::registerKeyUpListener([&](I32 keycode, I32 scancode, I32 modifiers)
    {
		
        _instance->_running = !(keycode == GLFW_KEY_ESCAPE);
    });

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	_instance->_running = true;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 Client::run()
///
/// \brief	Runs the main loop for the client.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The exit code for the client.
////////////////////////////////////////////////////////////////////////////////
I32 Client::run()
{
	while(_instance->_running)
	{
		net::waitSeconds(_instance->_dt);
		_instance->update();
		
		_instance->draw();
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Client::stop()
///
/// \brief	Stops this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Client::stop()
{
	if(_instance != nullptr)
	{
		if(_instance->_transport != nullptr)
		{
			Transport::destroy(_instance->_transport);
			_instance->_transport = nullptr;
		}
		delete _instance;
		_instance = nullptr;
	}
	Transport::shutdown();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Client::Client()
///
/// \brief	Default constructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
Client::Client() :
	_transport(nullptr),
	_dt(0.0f),
	_state(Disconnected),
	_sendMsgTimer(0.0f),
	_engine(getEngine()),
	_batcher(getEngine().getBatcher()),
	_builtIns(getEngine().getBuiltIns()),
	_window(*getEngine().getWindow())
{}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Client::~Client()
///
/// \brief	Destructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
Client::~Client()
{}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Client::update()
///
/// \brief	Updates this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if the next loop should happen; false otherwise.  This is being
/// 		changed to a void eventually.
////////////////////////////////////////////////////////////////////////////////
bool Client::update()
{
	_transport->update(_dt);

	//I don't know that we actually want network state to end the game.  We may,
	//but for now I'm not implementing it.  Need to figure out drawing and such
	//anyway.
	switch(_state)
	{
	case Searching:
		displayLobby();
		break;
	case Connecting:
		doConnecting();
		break;
	case Connected:
		doConnected();
		break;
	default:
		return false; //don't want infinite loops if the state is bad
		break;
	}
	
	// TODO:
	// fit game engine in here
	// remove stderr output and the like in favor of actual graphics
	glfwPollEvents();

	if(_window.isClosePending() && _running)
		_running = false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Client::displayLobby()
///
/// \brief	Displays a lobby.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool Client::displayLobby()
{
	I32 entryCount = _transport->getLobbyEntryCount();
	Transport::LobbyEntry entry;
	std::cerr << "-------------------------------------------------------------"
				<<"-------------------"<<std::endl
				<<"Available Servers"<<std::endl;
	for(I32 i=0;i<entryCount;++i)
	{
		if(_transport->getLobbyEntryAtIndex(i, entry))
			std::cerr << "\t" << (i+1)<< ". " << entry.name << " ("
						<< entry.address << ")" << std::endl;
	}
	std::cerr << "Enter 'R' to refresh, 'Q' to quit or the number of the "
				<< "server." << std::endl << "Choice" << std::endl;
	
	std::string choice;

	//I'm expect this to halt all execution of the loop in main while it waits
	//for input
	//std::getline(std::cin,choice);
	if(choice.c_str()[0]=='R' || choice.c_str()[0]=='r')
	{
		return true;
	}
	else if(choice.c_str()[0]=='Q' || choice.c_str()[0]=='q')
	{
		return false;
	}
	else
	{
		I32 iChoice = atoi(choice.c_str());
		if(iChoice<=entryCount)
		{
			_transport->getLobbyEntryAtIndex(iChoice-1,entry);
			joinServer(entry);
		}
		//and why not just refresh if invalid input.
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Client::doConnecting()
///
/// \brief	Executes the connecting operation.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool Client::doConnecting()
{
	if(_transport->connectFailed())
	{
		PBJ_LOG(pbj::VError) << "Connect failed" << PBJ_LOG_END;
		return false;
	}

	if(_transport->isConnected())
		_state = Connected;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Client::doConnected()
///
/// \brief	Executes the connected operation.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool Client::doConnected()
{
	//first receive any information we might need
	const I32 maxSize = 512;
	I32 nodeId;
	U8 data[maxSize];
	I32 bytesReceived = _transport->receivePacket(nodeId, data, maxSize);
	if(bytesReceived != 0)
	{ //we actually got something!
		U32 protoId;
		net::readInteger(data, protoId);
		if(_transport->getConfig().protoId == protoId)
		{ //even better, it's the right protocol!
			//because this is just a test, I will ignore other checks.
			U8* msg = new U8[bytesReceived-4];
			strncpy_s((char*)msg, bytesReceived-4, (char*)(data+6),
						bytesReceived-4);
			std::cerr << "Client: received message:\n\t" << msg << std::endl;
		}
	}

	//now send messages to the server
	_sendMsgTimer+=_dt;
	if(_sendMsgTimer >= 1.5f)
	{
		U8 packet[512];
		net::writeInteger(packet, _transport->getConfig().protoId);
		packet[4] = 0x03; //type = 3, which is nothing for the time being
		packet[5] = 0;
		
		//the 38 here is exact, so for more arbitrary messages this should be
		//a variable
		strcpy_s((char*)(packet+6), 38+1, 
					"This is just a test (client to server)");
		if(_transport->sendPacket(0,packet,512))
			std::cerr<<"Sent packet to server " << (_transport->getNodeAt(0))
						<< std::endl;
		_sendMsgTimer -= 1.5f;
	}

	if(!_transport->isConnected())
	{
		PBJ_LOG(pbj::VInfo) << "Disconnected" << PBJ_LOG_END;
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Client::joinServer(Transport::LobbyEntry entry)
///
/// \brief	Join server.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	entry	The LobbyEntry containing the server information.
////////////////////////////////////////////////////////////////////////////////
void Client::joinServer(Transport::LobbyEntry entry)
{
	if(!_transport->leaveLobby())
	{
		std::cerr<<"Leaving lobby failed!"<<std::endl;
		return;
	}
	_transport->connectClient(entry.address);
	_state = Connecting;
}

void Client::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Transform trans = Transform();

	const gfx::Mesh& testMesh = _builtIns.getMesh(Id("Mesh.std_quad"));
	gfx::Mesh* useMesh = (Mesh*)&testMesh;
	gfx::SceneShaderProgram ssp = gfx::SceneShaderProgram();
	ssp.init(_builtIns.getShader(Id("Shader.UIBox.vertex")),
			_builtIns.getShader(Id("Shader.UIBox.fragment")));
	gfx::EntityMaterial mat = EntityMaterial();
	mat.setShaderProgram(&ssp);

	scene::Entity e = scene::Entity();
	e.setMaterial(&mat);
	e.setMesh(useMesh);
	e.setTransform(trans);

	e.generateBatcherTask();
	_batcher.submit(*(e.getBatcherTask()));
	_batcher.draw();

	glfwSwapBuffers(_window.getGlfwHandle());
}

void Client::onContextResized(I32 width, I32 height)
{
	//nothing doing for now
}