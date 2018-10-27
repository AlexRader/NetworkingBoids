/*
the following file was modified by
Alex Rader 1023256 Rosser Martinez 1020967
EGP-405-01/02 Lab 3 | due 10/3/2018
current date 10/3/2018

�We certify that this work is entirely our own. The assessor of this project
may reproduce this project and provide copies to other academic staff, and/or
communicate a copy of this project to a plagiarism-checking service, which may
retain a copy of the project on its database.�
*/
#include "Proj2Events.h"
#include "Proj2Queue.h"
///


//BASE EVENT
Event::Event()
{

}
Event::~Event()
{

}
void Event::execute()
{
	printf("\n base event firing \n");
}
///

//JOIN SERVER EVENT
JoinEvent::JoinEvent()
{
	mEventType = JOIN_EVENT;
}
JoinEvent::JoinEvent(RakNet::RakPeerInterface *peer, char* ip, unsigned short port)
{
	mPeer = peer;
	strcpy(mIP, ip);
	mPort = port;

	printf("\n JOIN EVENT CREATED! \n");
}

JoinEvent::~JoinEvent()
{

}
void JoinEvent::execute()
{
	//join server with member information
	mPeer->Connect(mIP, mPort, 0, 0);
}


//PRINT CAT EVENT
PrintCatEvent::PrintCatEvent()
{
	mEventType = PRINT_CAT_EVENT;
}
PrintCatEvent::~PrintCatEvent()
{

}
void PrintCatEvent::execute()
{
	//woooOOOOooo print SPOOKY HALLOWEEN CAT
	printf("\n");
printf("                                                            .-----.		   \n");
printf("                                                          ,' __/|_ `.	   \n");
printf("                                                         ,    {~)    :	   \n");
printf("                                        /:\              :   //~_)  _:__   \n");
printf("                            __,---.__ /::::\             : ~~~/' \~~====   \n");
printf("                           `-.__     \:::::/              :   ~;;~  .'	   \n");
printf("                             ;;:\--.__`--.--._             `._____.'	   \n");
printf("                           ,;;'` `    `--.__  `-._						   \n");
printf("                           `,  ,\       /,  `--.__;						   \n");
printf("                           <   (o) ___ (o)   >							   \n");
printf("                          <        \:/        >							   \n");
printf("                           <     ._/\\_.     >							   \n");
printf("                   _.---._  `-.    ~~~    .-'							   \n");
printf("                 .'._.--. `.   `~:~~~~~~:'								   \n");
printf("                 `-'     `. `.  :        :								   \n");
printf("                          :__: :________  :___							   \n");
printf("                      ;'xx:XXxxxxxxxxxx:xxxXXX:xx:						   \n");
printf("                    :::xx:XXXX:xxxxxxxxx:XXXXXX:xxx:.					   \n");
printf("                   ::xxx:XXX/X;xxxxxxxxxx:XXXXXX:xxx:.					   \n");
printf("           |||    ::xxx:XXX// xxxxxxxxxx// XXXXXX:xxx:.         []		   \n");
printf("         ||||||  ||xxxx:XX//   xxxxxxxx//   XXXXXX:xx||     .:||:|:||.	   \n");
printf("    ___    |||   ||xxx:XX//  0  xxxxxx// 0   XXXXX:xx||    .:||^:|:^||.	   \n");
printf("  ,'   `.. |||   `::xx:XXXXX:xxxx/ \\xxxxx:XXXXXXXX:xx:'   ::|:::V:::|:	   \n");
printf("  |     || |||    `::xx:XXXXX:xxx___Xxxx:XXXXXXXX:xx:'     `::|UUUUU:|'	   \n");
printf("  |R.I.P|| |||     `::xx:XXXXXxxxxxxxxxxxXXXXXXX:x::'       `::|::::|'	   \n");
printf("  |     ||""""      ':xx:XX \\/  \\/  \\/ \\/XXXXX:xx:'       """"""""""   \n");
printf(" """"""""             '.x:XXXXxxxxxxxxxxXXXXxx;'''						   \n");
printf("""""""""""""""""""""""   ~~~~~~~~~~~~~~~~~~~~~~   """""""""""""""""""""""" \n");
printf("\n");


}

//
RussianRouletteEvent::RussianRouletteEvent()
{
	mEventType = RUSSIAN_ROULETTE_EVENT;
}
RussianRouletteEvent::~RussianRouletteEvent()
{

}

void RussianRouletteEvent::execute()
{

	//russian roulette to disconnect and kill your client

	int trigger = rand() % 5; //6 chambers, 1 bullet

	if (trigger == 0)
	{
		//haha anime
		printf("\n omae wa mou shindeiru \n NANI!?!?!?!?");
		system("Pause");
		std::exit(EXIT_SUCCESS);
	}
	else
	{
		//not disconnecting your client
		printf("\n hey great job not shooting yourself \n");
	}

}

//Eventmanager with member queue
EventManager::EventManager()
{
	mEventQ = new Queue;
}

EventManager::~EventManager()
{
	while (!mEventQ->isEmpty())
	{
		mEventQ->deQueue();
	}
	delete mEventQ;
}

//enqueues pointer to event
void EventManager::enqueueEvent(Event *eventToQ)
{
	mEventQ->enQueue(eventToQ);
}

//execute all events
void EventManager::executeQueuedEvents()
{
	Event* tmp;

	if (!mEventQ->isEmpty())
	{
		tmp = mEventQ->deQueue();

		tmp->execute();
	}
}