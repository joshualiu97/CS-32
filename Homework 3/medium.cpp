//#include <iostream>
//#include <string>
//using namespace std;
//
//enum CallType {
//	VOICE, TEXT
//};
//
///////////////////////////////////////////////////////////////////////////////////////
//
////Your declarations and implementations would go here

class Medium
{
public:
	Medium(string m);
	virtual string connect() const = 0;
	virtual string transmit(string msg) const;
	string id() const;
	virtual ~Medium();
private:
	string m_id;
};

class TwitterAccount : public Medium
{
public:
	TwitterAccount(string m);
	virtual string connect() const;
	~TwitterAccount();
};

class Phone : public Medium
{
public:
	Phone(string number, CallType voicetext);
	virtual string connect() const;
	virtual string transmit(string msg) const;
	~Phone();
private:
	string m_typemsg;
};

class EmailAccount : public Medium
{
public:
	EmailAccount(string m);
	virtual string connect() const;
	~EmailAccount();
};

Medium::Medium(string m)
	: m_id(m)
{}

TwitterAccount::TwitterAccount(string m)
	: Medium(m)
{}

Phone::Phone(string number, CallType voicetext)
	: Medium(number)
{
	if (voicetext == CallType::VOICE)
		m_typemsg = "voice";
	else
		m_typemsg = "text";
}

EmailAccount::EmailAccount(string m)
	: Medium(m)
{}

string TwitterAccount::connect() const
{
	return "Tweet";
}

string Phone::connect() const
{
	return "Call";
}

string EmailAccount::connect() const
{
	return "Email";
}

string Medium::id() const
{
	return m_id;
}

string Medium::transmit(string msg) const
{
	string combine = "text: " + msg;
	return combine;
}

string Phone::transmit(string msg) const
{
	string combine = m_typemsg + ": " + msg;
	return combine;
}

Medium::~Medium()
{}

TwitterAccount::~TwitterAccount()
{
	cout << "Destroying the Twitter account " + id() + "." << endl;
}

Phone::~Phone()
{
	cout << "Destroying the phone " + id() + "." << endl;
}

EmailAccount::~EmailAccount()
{
	cout << "Destroying the email account " + id() + "." << endl;
}

//Send the message.
//Tweet using id UCLABruiAlert, sending text : Major power outage in West L.A.
//Call using id (310) 825 3894, sending voice : Major power outage in West L.A.
//Call using id (213) 389 9060, sending text : Major power outage in West L.A.
//Email using id jbroon@ucla.edu, sending text : Major power outage in West L.A.
//Cleaning up.
//Destroying the Twitter account UCLABruinAlert.
//Destroying the phone(310) 825 3894.
//Destroying the phone(213) 389 9060.
//Destroying the email account jbroon@ucla.edu.

/////////////////////////////////////////////////////////////////////////////////////
//	
//void send(const Medium* m, string msg)
//{
//	cout << m->connect() << " using id " << m->id()
//		<< ", sending " << m->transmit(msg) << endl;
//}
//
//int main()
//{
//	Medium* media[4];
//	media[0] = new TwitterAccount("UCLABruinAlert");
//	// Phone users may prefer messages to be left as voice or text.
//	media[1] = new Phone("(310) 825 3894", VOICE);
//	media[2] = new Phone("(213) 389 9060", TEXT);
//	media[3] = new EmailAccount("jbroon@ucla.edu");
//
//	cout << "Send the message." << endl;
//	for (int k = 0; k < 4; k++)
//		send(media[k], "Major power outage in West L.A.");
//
//	// Clean up the media before exiting
//	cout << "Cleaning up." << endl;
//	for (int k = 0; k < 4; k++)
//		delete media[k];
//}