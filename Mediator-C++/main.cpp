#include <iostream>
#include <locale>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <stdexcept>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

class Colleague;

class Mediator
{
public:
	virtual ~Mediator() = 0;
	virtual void finishedWork(Colleague* colleague) = 0;
};

class Colleague
{
public:
	Colleague(Mediator* mediator)
		: mediator{ mediator }
	{
	}

	void finishedWork()
	{
		mediator->finishedWork(this);
	}

private:
	Mediator* mediator;
};

Mediator::~Mediator() { }

class Architect : public Colleague
{
public:
	void architect()
	{
		std::this_thread::sleep_for(1s);
		std::cout << "������������� ������ �����������. ���� ������ ���!\n";
		finishedWork();
	}
};

class Developer : public Colleague
{
public:
	void develop()
	{
		std::this_thread::sleep_for(1s);
		std::cout << "����������� ������� ���. ���� ���������� �� ������������\n";
		finishedWork();
	}
};

class Tester : public Colleague
{
public:
	void test()
	{
		std::this_thread::sleep_for(1s);
		std::cout << "����������� �������� ������������.\n";
		finishedWork();
	}
};

class SoftwareDevelopmentMediator : public Mediator
{
public:
	SoftwareDevelopmentMediator()
		: architect{ this },
		dev{ this },
		tester{ this }
	{
	}

	virtual void finishedWork(Colleague* colleague) override
	{
		if (colleague == &architect)
		{
			dev.develop();
		}
		else if (colleague == &dev)
		{
			tester.test();
		}
		else if (colleague == &tester)
		{
			std::this_thread::sleep_for(1s);
			std::cout << "���������� �� ���������. �������� �������� �������!\n";
		}
		else
		{
			std::cout << "������ �������� �� ��������� � ���������� ��\n";
		}
	}

	void startDevelopment()
	{
		architect.architect();
	}

private:
	Architect architect;
	Developer dev;
	Tester tester;
};

int main()
{
	std::locale::global(std::locale(""));
	SoftwareDevelopmentMediator mediator;
	mediator.startDevelopment();
	std::system("pause");
}

/*class AddressAndName
{
public:
	AddressAndName(const std::string& name, const std::string& address)
		: name{ name },
		address{ address }
	{
	}

	const std::string& getName() const { return name; }
	const std::string& getAddress() const { return address; }

	std::string as_string() const
	{
		return "���: " + name + ", �����: " + address;
	}

private:
	std::string name;
	std::string address;
};

class LetterWasMoved : public std::runtime_error
{
public:
	LetterWasMoved()
		: std::runtime_error{ "���������� �������� ������ � ������, ����������� � ������ �����" }
	{
	}
};

class Letter
{
public:
	Letter(AddressAndName addressee, AddressAndName sender, std::string content)
		: addressee{ addressee },
		sender{ sender },
		content{ content }
	{
	}

	// ���������� ������������� ������ ������������ ��������� �����������
	// ��� ��������� �������� ������ �� ��� � ����.
	// ����� ����������� ������ ������ �� ����������� �������������
	Letter(const Letter&) = delete;

	// ������ ����� ��������� �� ������
	Letter* operator&() = delete;

	Letter& operator=(Letter&& other)
	{
		other.checkIsMoved();
		addressee = std::move(other.addressee);
		sender = std::move(other.sender);
		content = std::move(other.content);
		other.isMovedVal = true;
		isMovedVal = false;
		return *this;
	}

	Letter(Letter&& other)
		: addressee{ (other.checkIsMoved(), std::move(other.addressee)) },
		sender{ std::move(other.sender) },
		content{ std::move(other.content) }
	{
		other.isMovedVal = true;
	}

	const AddressAndName& getAddressee() const
	{
		checkIsMoved();
		return addressee;
	}

	const AddressAndName& getSender() const
	{
		checkIsMoved();
		return sender;
	}

	const std::string& getContent() const
	{
		checkIsMoved();
		return content;
	}

	bool isMoved() const
	{
		return isMovedVal;
	}

private:
	void checkIsMoved() const
	{
		if (isMovedVal)
		{
			throw LetterWasMoved();
		}
	}

	AddressAndName addressee, sender;
	std::string content;
	bool isMovedVal = false;
};

class AddresseeException : public std::runtime_error
{
public:
	AddresseeException(const std::string& message, Letter&& letter)
		: std::runtime_error{ message },
		letter{ std::move(letter) }
	{
	}

	const AddressAndName& getAddressee() const { return letter.getAddressee(); }
	Letter takeLetterBack() { return std::move(letter); }
	const Letter& showLetter() const { return letter; }

private:
	Letter letter;
};

class AddresseeNotFound : public AddresseeException
{
public:
	AddresseeNotFound(Letter&& letter)
		: AddresseeException{ "�� ������ �������. " + letter.getAddressee().as_string(), std::move(letter) }
	{
	}
};

class AddresseeDidntProceessedLetter : public AddresseeException
{
public:
	AddresseeDidntProceessedLetter(Letter&& letter)
		: AddresseeException{ "������� " + letter.getAddressee().as_string() + " �� ���� ���������� ���������", std::move(letter) }
	{
	}
};

class Subscriber;

// �������� ������
class MailService
{
public:
	MailService()
	{
	}

	void registerSubscriber(std::shared_ptr<Subscriber> subscriber, const std::string& address)
	{
		subscribers.insert({ address, subscriber });
	}

	void sendLetter(Letter letter);

private:
	// �������� ���������� ���������� ������� �� ������ ������
	std::multimap<std::string /*address*//*, std::shared_ptr<Subscriber>> subscribers;
};

/*abstract*//* class Subscriber
{
public:
	virtual ~Subscriber() = 0;

	const std::string& getName() const
	{
		return name;
	}

	virtual void receiveLetter(Letter letter) = 0;

	void sendLetter(Letter letter)
	{
		service->sendLetter(std::move(letter));
	}

	void changeMailService(MailService service)
	{
		this->service = &service;
	}

protected:
	Subscriber(const std::string& name, MailService& service)
		: name{ name },
		service{ &service }
	{
	}

private:
	std::string name;
	MailService* service;
};

Subscriber::~Subscriber() { }

void MailService::sendLetter(Letter letter)
{
	std::cout << "� �������� ��������� ������ ������ �� \"" << letter.getSender().as_string()
		<< "\" ��� \"" << letter.getAddressee().as_string() << '\"' << std::endl;
	auto subscriberIter = subscribers.find(letter.getAddressee().getAddress());
	if (subscriberIter == subscribers.end())
	{
		throw AddresseeNotFound{ std::move(letter) };
	}
	for (auto iter = subscriberIter; iter != subscribers.end(); iter++)
	{
		auto& somebody = iter->second;
		if (somebody->getName() == letter.getAddressee().getName())
		{
			try
			{
				somebody->receiveLetter(std::move(letter));
				return;
			}
			catch (AddresseeDidntProceessedLetter& ex)
			{
				std::cout << "������� c �������: \"" << ex.showLetter().getAddressee().as_string() << "\" �� ���� ���������� ���������" << std::endl;
				throw;
			}
		}
	}
	throw AddresseeNotFound(std::move(letter));
}

// ���������� � �������� ����������. ��������� ���� ������ � ������
class SubscriberWithCloudStorage : public Subscriber
{
public:
	SubscriberWithCloudStorage(const std::string& name, MailService& service)
		: Subscriber{ name, service }
	{
	}

	virtual void receiveLetter(Letter letter) override
	{
		std::cout << getName() << " ������� ������ �� �����, ������������ � �������� � ��� ������.\n���������� ������:\n\"" << letter.getContent() << '\"' << std::endl;
	}
};

// �������������� ����������, �� ������������ �������
class ConservativeSubscriber : public Subscriber
{
public:
	ConservativeSubscriber(const std::string& name, MailService& service)
		: Subscriber{ name, service }
	{
	}

	virtual void receiveLetter(Letter letter) override
	{
		std::cout << getName() << " ������� ������ �� �����, �� �� ��������� ���. �� ������ ���������� ����������..." << std::endl;
	}
};

// ����������, ������� �� ����� �������� ������, � ������ ����������
class RejectSubscriber : public Subscriber
{
public:
	RejectSubscriber(const std::string& name, MailService& service)
		: Subscriber{ name, service }
	{
	}

	virtual void receiveLetter(Letter letter) override
	{
		std::cout << "������ " << getName() << " ����������: \"���� � � ����������������� �� �����, �� �������� ������ �� ����! ������ ����������! ��������� ������ �����.\"" << std::endl;
		throw AddresseeDidntProceessedLetter(std::move(letter));
	}
};

int main()
{
	std::locale::global(std::locale(""));

	// ������ �������� ������
	MailService mailService;

	constexpr char borovaya7[] = "��. �������, �. 7";
	constexpr char borovaya8[] = "��. �������, �. 8";

	constexpr char peterName[] = "������ ϸ�� ��������";
	constexpr char ivanName[] = "������ ���� ��������";
	constexpr char sidrName[] = "������� ���� ���������";

	AddressAndName peterAddress{ peterName, borovaya7 };
	AddressAndName ivanAddress{ ivanName, borovaya7 };
	AddressAndName sidrAddress{ sidrName, borovaya8 };

	//������ ������������� ����� � ������������ �� �� �����
	std::shared_ptr<Subscriber> peter = std::make_shared<SubscriberWithCloudStorage>(peterName, mailService);
	mailService.registerSubscriber(peter, peterAddress.getAddress());
	std::shared_ptr<Subscriber> ivan = std::make_shared<ConservativeSubscriber>(ivanName, mailService);
	mailService.registerSubscriber(ivan, ivanAddress.getAddress());
	std::shared_ptr<Subscriber> sidr = std::make_shared<RejectSubscriber>(sidrName, mailService);
	mailService.registerSubscriber(sidr, sidrAddress.getAddress());

	// ���� ��� ������ �����
	Letter peterLetter{ peterAddress, ivanAddress, "������, ϸ��! �� �����." };
	peter->sendLetter(std::move(peterLetter));

	std::cout << std::endl;

	// ���� ��� ������ �����
	Letter sidrLetter{ sidrAddress, ivanAddress, "������, ����! ����, ��� �� �� �������� ���������" };
	try
	{
		peter->sendLetter(std::move(sidrLetter));
	}
	catch (AddresseeDidntProceessedLetter& ex)
	{
		sidrLetter = std::move(ex.takeLetterBack());
		std::cout << "���� �������� ������ �������.\n��������� ������: " << sidrLetter.getContent() << std::endl;
	}

	std::cout << std::endl;

	// ���� ��� ������ �����
	Letter ivanLetter{ ivanAddress, sidrAddress, "������, ����! ����� �� ��������!" };
	sidr->sendLetter(std::move(ivanLetter));

	std::cout << std::endl;

	std::system("pause");
	/*std::vector<std::shared_ptr<Subscriber>> subscribers;
	subscribers.push_back(peter);
	subscribers.push_back(ivan);
	subscribers.push_back(sidr);*//*
}*/