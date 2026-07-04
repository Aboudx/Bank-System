#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <vector>
using namespace std;

enum eMenuOptions { eShow = 1, eAdd = 2, eDelete = 3, eUpdate = 4, eFind = 5, eTransactions = 6, eExit = 7 };

enum eMenuTransactions { eDeposit = 1, eWithDraw = 2, eTotalBalance = 3, eExitTrans = 4 };

const string ClientsFileName = "Clients.txt";

struct stClient
{
	string AcountNumber = "";
	string PINCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0.0000;
	bool MarkForDelete = false;
};

vector<string> splitString(string Text, string delim)
{
	vector<string>vSplit;

	short pos = 0;
	string sWord;

	while ((pos = Text.find(delim)) != std::string::npos)
	{
		sWord = Text.substr(0, pos);

		if (sWord != "")
		{
			vSplit.push_back(sWord);
		}

		Text.erase(0, pos + delim.length());
	}

	if (Text != "")
	{
		vSplit.push_back(Text);
	}
	return vSplit;
}

stClient ConvertLineToRecord(string Text, string seperator = "#//#")
{
	stClient Client;

	vector<string>vClient = splitString(Text, seperator);

	Client.AcountNumber = vClient[0];
	Client.PINCode = vClient[1];
	Client.Name = vClient[2];
	Client.Phone = vClient[3];
	Client.AccountBalance = stod(vClient[4]);

	return Client;

}

vector <stClient>LoadCleintsDataFromFile(string FileName)
{
	vector<stClient>vClients;
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		while (getline(MyFile, Line))
		{
			if (Line != "")
			{
				vClients.push_back(ConvertLineToRecord(Line));
			}
		}
		MyFile.close();
	}
	return vClients;
}

eMenuTransactions MenuTransactionsChoiceO()
{
	cout << "Choose what do you want to do? [1 to 4]? ";
	short Choice = 0; cin >> Choice;
	return eMenuTransactions(Choice);
}

eMenuOptions MenuMainChoice()
{
	cout << "Choose what do you want to do? [1 to 7]? ";
	short Choice = 0; cin >> Choice;
	return eMenuOptions(Choice);
}

void PrintClientRecord(stClient& Client)
{
	cout << "| " << setw(15) << left << Client.AcountNumber;
	cout << "| " << setw(12) << left << Client.PINCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(17) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector<stClient>& vClients)
{
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_______________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(12) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(17) << "Phone";
	cout << "| " << left << setw(12) << "Balance";

	cout << "\n_______________________________________________________";
	cout << "_______________________________________________\n" << endl;

	for (stClient& Client : vClients)
	{
		PrintClientRecord(Client);
		cout << endl;
	}
	cout << "_______________________________________________________";
	cout << "_______________________________________________\n" << endl;

}

void ShowMainMenue()
{
	system("cls");
	cout << "=========================================" << endl;
	cout << "\t\tMain Menue Screen\n";
	cout << "=========================================" << endl;

	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Exit.\n";

	cout << "=========================================" << endl;
}

void GoBackToMainMenue()
{
	cout << "\nPress Any Key To go back to Main Menue...";
	system("pause>0");
	system("cls");
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AcountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
		}
		return false;
	}
}

void ShowClientsScreen(vector<stClient>& vClients)
{
	system("cls");
	PrintAllClientsData(vClients);
	GoBackToMainMenue();
}

stClient ReadNewClient()
{
	stClient Client;

	cout << "Enter Account Number? ";
	getline(cin >> ws, Client.AcountNumber);

	while (ClientExistsByAccountNumber(Client.AcountNumber, ClientsFileName))
	{
		cout << "Client With [" << Client.AcountNumber << "] Exists, Enter Another Account Number . ";
		getline(cin >> ws, Client.AcountNumber);
	}

	cout << "Enter PIN Code? ";
	getline(cin, Client.PINCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}

string ConvertRecordToLine(stClient Client, string seperator = "#//#")
{
	string stClientRecord = "";

	stClientRecord += Client.AcountNumber + seperator;
	stClientRecord += Client.PINCode + seperator;
	stClientRecord += Client.Name + seperator;
	stClientRecord += Client.Phone + seperator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}

void AddDataToFile(string FileName, string Data)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << Data << endl;

		MyFile.close();
	}
}

void SaveClientsDataToFile(string FileName, vector<stClient>& vClients)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string DataLine;

		for (stClient& C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
}

void AddNewClient(vector<stClient>& vClients)
{
	stClient Client;
	Client = ReadNewClient();
	vClients.push_back(Client);

	AddDataToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddClients(vector<stClient>& vClients)
{
	char AddMore = 'Y';
	do
	{
		system("cls");
		cout << "Adding New Client :\n\n";

		AddNewClient(vClients);
		cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

void ShowAddScreen(vector<stClient>& vClients)
{
	system("cls");

	cout << "\n-------------------------------------------\n";
	cout << "Add New Clients\n";
	cout << "\n-------------------------------------------\n";
	AddClients(vClients);
	GoBackToMainMenue();
}

bool FindClientByAccountNumber(string AccountNumber, stClient& Client)
{
	vector<stClient>vClients;
	vClients = LoadCleintsDataFromFile(ClientsFileName);

	for (stClient sClient : vClients)
	{
		if (sClient.AcountNumber == AccountNumber)
		{
			Client = sClient;
			return true;
		}
	}

	return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	for (stClient& C : vClients)
	{
		if (C.AcountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

void PrintClientCard(stClient Client)
{
	cout << "\n\nThe following are the client details :\n\n";

	cout << "----------------------------------------------------\n";
	cout << "Account Number: " << Client.AcountNumber << endl;
	cout << "PIN Code: " << Client.PINCode << endl;
	cout << "Name: " << Client.Name << endl;
	cout << "Phone: " << Client.Phone << endl;
	cout << "Account Balance: " << Client.AccountBalance << endl;
	cout << "----------------------------------------------------\n";

}

bool DeleteClientByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, Client))
	{
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want delete this client? y/n ? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(ClientsFileName, vClients);

			vClients = LoadCleintsDataFromFile(ClientsFileName);

			cout << "\n\nClient Deleted Successfully.";
			return true;
		}

	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}

string ReadAccountNumber()
{
	cout << "\nEnter Acount Number ? ";
	string Acc; cin >> Acc;
	return Acc;
}

void ShowDeleteScreen(vector<stClient>& vClients)
{
	system("cls");
	cout << "\n-------------------------------------------\n";
	cout << "\t\tDelete Client Screen\n";
	cout << "\n-------------------------------------------\n";

	string AccountNumber = ReadAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
	GoBackToMainMenue();

}

stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;
	Client.AcountNumber = AccountNumber;

	cout << "Enter PIN Code ? ";
	getline(cin >> ws, Client.PINCode);

	cout << "Enter Name ? ";
	getline(cin, Client.Name);

	cout << "Enter Phone ? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance ? ";
	cin >> Client.AccountBalance;

	return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, Client))
	{
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want update this client? y/n ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			for (stClient& C : vClients)
			{
				if (C.AcountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
		}
		SaveClientsDataToFile(ClientsFileName, vClients);
		return true;
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}

void ShowUpdateScreen(vector<stClient>& vClients)
{
	system("cls");
	cout << "\n-------------------------------------------\n";
	cout << "\t\tUpdate Client Screen\n";
	cout << "\n-------------------------------------------\n";

	string AccountNumber = ReadAccountNumber();

	UpdateClientByAccountNumber(AccountNumber, vClients);
	GoBackToMainMenue();
}

void ShowFindScreen(vector<stClient>& vClients)
{
	system("cls");
	cout << "\n-------------------------------------------\n";
	cout << "\t\tFind Client Screen\n";
	cout << "\n-------------------------------------------\n";

	string AccountNumber = ReadAccountNumber();
	stClient C;

	if (FindClientByAccountNumber(AccountNumber, C))
	{
		PrintClientCard(C);
	}
	else
	{
		cout << "\n\nClient with Account Number (" << AccountNumber << ") is Not Found!";
	}
	GoBackToMainMenue();
}

void NewProccessToAccount(string AccountNumber, vector<stClient>& vClients, double amount, bool WithOrDepos = true)
{
	for (stClient& Client : vClients)
	{
		if (AccountNumber == Client.AcountNumber)
		{
			if (WithOrDepos)
			{
				Client.AccountBalance += amount;
				break;
			}
			else
			{
				while (amount > Client.AccountBalance)
				{
					cout << "Enter The Correct ammount. Your Amount is : (" << Client.AccountBalance << ")" << endl;
					cout << "Enter Amount To Withdraw? "; cin >> amount;
				}
				Client.AccountBalance -= amount;
				break;
			}
		}
	}
}

void DepositToAccountByAccountNumber(string& AccountNumber, vector<stClient>& vClients)
{
	stClient Client;
	while (!FindClientByAccountNumber(AccountNumber, Client))
	{
		cout << "Client With [" << AccountNumber << "] does not Exist." << endl;

		cout << "Enter Account Number ? ";
		cin >> AccountNumber;
	}
	system("cls");
	PrintClientCard(Client);

	cout << "Enter Deposit amount ? ";
	double amount = 0; cin >> amount;
	while (amount < 0)
	{
		cout << "\nPlease Enter A Positive amount. ";
		cin >> amount;
	}
	NewProccessToAccount(AccountNumber, vClients, amount);

	SaveClientsDataToFile(ClientsFileName, vClients);
	cout << "\nDeposit Successfully .\n" << endl;

}

void WithDrawToAccountByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	stClient Client;

	while (!FindClientByAccountNumber(AccountNumber, Client))
	{
		cout << "Client With [" << AccountNumber << "] does not Exist." << endl;

		cout << "\nEnter Account Number ? ";
		cin >> AccountNumber;
	}
	system("cls");
	PrintClientCard(Client);

	cout << "Enter WithDraw amount ? ";
	double amount = 0; cin >> amount;

	NewProccessToAccount(AccountNumber, vClients, amount, false);

	SaveClientsDataToFile(ClientsFileName, vClients);
	cout << "\nWithDraw Successfully .\n" << endl;
}

void ShowDepositScreen(vector<stClient>& vClients)
{
	system("cls");
	cout << "\n-------------------------------------------\n";
	cout << "\t\tDeposit Screen\n";
	cout << "\n-------------------------------------------\n";
	string Acc = ReadAccountNumber();
	DepositToAccountByAccountNumber(Acc, vClients);

	GoBackToMainMenue();

}

void ShowWithDrawScreen(vector<stClient>& vClients)
{
	system("cls");
	cout << "\n-------------------------------------------\n";
	cout << "\t\tWithDraw Screen\n";
	cout << "\n-------------------------------------------\n";
	string Acc = ReadAccountNumber();

	WithDrawToAccountByAccountNumber(Acc, vClients);

	GoBackToMainMenue();
}

void PrintClientBalanceRecord(stClient& Client)
{
	cout << "| " << setw(15) << left << Client.AcountNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

int AllAccountsBalance(vector<stClient>& vClients)
{
	int Balance = 0;
	for (stClient& Client : vClients)
	{
		Balance += Client.AccountBalance;
	}
	return Balance;
}

void ShowTotalBalanceScreen(vector<stClient>& vClients)
{
	system("cls");

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_______________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";

	cout << "\n_______________________________________________________";
	cout << "_______________________________________________\n" << endl;
	if (vClients.size() == 0)
	{
		cout << "\t\t\tNo Clients available In This System !";
	}
	else
	{
		for (stClient& Client : vClients)
		{
			PrintClientBalanceRecord(Client);
			cout << endl;
		}
		cout << "_______________________________________________________";
	}
	cout << "_______________________________________________\n" << endl;

	cout << "\n\n\t\t\t\t\t\tTotal Balance: " << AllAccountsBalance(vClients) << endl;

	GoBackToMainMenue();
}

void ShowEndScreen()
{
	cout << "\n-------------------------------------------\n";
	cout << "\t\tProgram  Ends (:\n";
	cout << "\n\t\tSee You Again (:\n";
	cout << "\n-------------------------------------------\n";
}

void ShowChoiceInTransactionsMenue(eMenuTransactions TransChoice, vector<stClient>& vClients)
{
	switch (TransChoice)
	{
	case eMenuTransactions::eDeposit:
		ShowDepositScreen(vClients);
		break;
	case eMenuTransactions::eWithDraw:
		ShowWithDrawScreen(vClients);
		break;
	case eMenuTransactions::eTotalBalance:
		ShowTotalBalanceScreen(vClients);
		break;
	case eMenuTransactions::eExitTrans:
		break;
	default:
		break;
	}
}

void ShowTransactionsScreen(vector<stClient>& vClients)
{
	system("cls");
	cout << "=========================================" << endl;
	cout << "\t\tTransactions Screen\n";
	cout << "=========================================" << endl;

	cout << "\t[1] Deposit.\n";
	cout << "\t[2] WithDraw\n";
	cout << "\t[3] TotalBalances.\n";
	cout << "\t[4] Exit.\n";

	cout << "=========================================" << endl;
	eMenuTransactions TransChoice = MenuTransactionsChoiceO();
	ShowChoiceInTransactionsMenue(TransChoice, vClients);
}

void ShowChoiceInMainMenue(eMenuOptions MenuChoice, vector<stClient>& vClients)
{
	switch (MenuChoice)
	{
	case eMenuOptions::eShow:
		ShowClientsScreen(vClients);
		break;
	case eMenuOptions::eAdd:
		ShowAddScreen(vClients);
		break;
	case eMenuOptions::eDelete:
		ShowDeleteScreen(vClients);
		break;
	case eMenuOptions::eUpdate:
		ShowUpdateScreen(vClients);
		break;
	case eMenuOptions::eFind:
		ShowFindScreen(vClients);
		break;
	case eMenuOptions::eTransactions:
		ShowTransactionsScreen(vClients);
		break;
	case eMenuOptions::eExit:
		ShowEndScreen();
		break;
	default:
		break;
	}
}

void StartBankSystem()
{
	vector<stClient>vClients;
	vClients = LoadCleintsDataFromFile(ClientsFileName);
	ShowMainMenue();

	eMenuOptions Choice;

	do
	{
		ShowMainMenue();
		Choice = MenuMainChoice();

		ShowChoiceInMainMenue(Choice, vClients);
	} while (Choice != eMenuOptions::eExit);
}

int main()
{
	StartBankSystem();
}