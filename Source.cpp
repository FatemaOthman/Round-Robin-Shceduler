#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<fstream>
using namespace std;
/*

*/
class CicularQueue
{
private:
	string *Queue;
	int size;
	int Head;
	int Tail;
public:
	CicularQueue(int s)
	{
		Head = -1;
		Tail = -1;
		if (s > 0)
		{
			size = s;
			Queue = new string[s];
		}
		else
			size = -1;
	}
	int enQueue(string x)
	{
		if (size == -1)
			return -1;
		if ((Head == 0 && Tail == size - 1) || (Tail == (Head - 1) % (size - 1)))
		{
			return -1;
		}
		if (Head == -1)
		{
			Head = 0;
			Tail = 0;
			Queue[Tail] = x;
			
			return 0;


		}
		if (Tail == size - 1 && Head != 0)
		{
			Tail = 0;
			Queue[Tail] = x;
			return 0;
		}
		else
		{
			Tail++;
			Queue[Tail] = x;
			return 0;
		}

	}
	int deQueue(string &x)
	{
		if (Head == -1)
			return -1;

		x = Queue[Head];
		Queue[Head] = -1;
		if (Head == Tail)
		{
			Head = -1;
			Tail = -1;
		}
		else if (Head == size - 1)
		{
			Head = 0;
		}
		else
		{
			Head++;
		}

		return 0;

	}
	void PrintQueue(ofstream &MyFile)
	{
		for (int i = 0;i < 50;i++)
			MyFile << "=";
		MyFile << endl;
		MyFile << "Queue ";
		if (Tail >= Head)
		{
			for (int i = Head;i <= Tail;i++)
				MyFile << Queue[i] << " ";
			MyFile << endl;
		}
		else
		{
			for (int i = Head;i < size;i++)
				MyFile << Queue[i] << " ";
			for (int i = 0;i <= Tail;i++)
				MyFile << Queue[i] << " ";
			MyFile << endl;

		}

	}
	bool IsEmpty()
	{
		return (Head == -1 && Tail == -1);
	}
	int Peek(string &s)
	{
		if (size == -1 || Head == -1 || Tail == -1)
			return -1;
		else
			s = Queue[Head];
		return 0;
	}



};
void DoTheMathAndTheOutput(vector<string> IDs,
	map <string, int> RunTime,
	map<string, int>ArrivalTime,
	map<string, int>StartTime,
	map<string, int>FinishingTime,
	string name,
	int T
)
{
	map<string, int>TurnAroundTime;
	map<string, float>WeightedTA;
	ofstream MyFile;
	MyFile.open(name + "_output.txt");
	for (int i = 0;i < 200;i++)
		MyFile << "=";
	MyFile <<  "\n\nProcess_ID\t Run_Time\t Arriva_time\t Start_Time\t Finish_Time\t TA_time\t Weight_TA_Time\t\n\n" ;
	for (int i = 0;i < 200;i++)
		MyFile << "=";
	MyFile << "\n\n";

	float AVGWeightedTATime = 0;
	float AVGwaitingTime = 0;
	float STDWightedTime = 0;
	float UtlizationTime = 0;
	for (int i = 0; i < IDs.size();i++)
	{
		UtlizationTime += RunTime[IDs[i]];

		TurnAroundTime[IDs[i]] = FinishingTime[IDs[i]] - ArrivalTime[IDs[i]];

		WeightedTA[IDs[i]] = (1.00*TurnAroundTime[IDs[i]]) / (1.00 *RunTime[IDs[i]]);

		AVGWeightedTATime += WeightedTA[IDs[i]];

		AVGwaitingTime += (StartTime[IDs[i]] - ArrivalTime[IDs[i]]);

		MyFile << IDs[i] << "\t\t " << RunTime[IDs[i]] << "\t\t " << ArrivalTime[IDs[i]] << "\t\t " << StartTime[IDs[i]] << "\t\t " << FinishingTime[IDs[i]] << "\t\t " << TurnAroundTime[IDs[i]] << "\t\t " << WeightedTA[IDs[i]] << "\t" << endl;

	}

	AVGWeightedTATime /= (1.00*IDs.size());
	AVGwaitingTime /= (1.00*IDs.size());
	UtlizationTime /= (1.00*T);
	UtlizationTime *= 100;
	for (int i = 0;i < IDs.size();i++)
	{
		STDWightedTime += (WeightedTA[IDs[i]] - AVGWeightedTATime)*(WeightedTA[IDs[i]] - AVGWeightedTATime);
	}
	STDWightedTime /= (1.00)*IDs.size();
	for (int i = 0;i < 200;i++)
		MyFile << "=";
	MyFile << "\n\nAverage Weighted TA time : " << AVGWeightedTATime << endl;
	MyFile << "Standard Deviation Weighted TA time : " << sqrt(STDWightedTime) << endl;
	MyFile << "Average Waiting Time : " << AVGwaitingTime << endl;
	MyFile << "CPU utilization : " << UtlizationTime;
	MyFile.close();

}
void SearchAndInsert(int t, CicularQueue &Q, map<string, int>ArrivalTime, vector <string>ID, int n, int &start)
{
	for (int i = start;i < n;i++)
	{
		if (ArrivalTime[ID[i]] <= t)
		{
			Q.enQueue(ID[i]);
			start++;

		}
		else break;
	}
}
void RoundRobin(vector<string> IDs,
				map <string,int> &RemainTime,
				map <string, int> RunTime,
				map<string,int>ArrivalTime,
				map<string, int>&StartTime,
				map<string, int>&FinishingTime,
				int N,
				int Quantum,
				int SwitchTime,
				string Name)
{
	ofstream MyFile;
	MyFile.open(Name + "_log.txt");
	bool Switching = false;
	bool Processing = false;
	bool CurrProcessDone = false;
	bool ResumeProcess = false;
	CicularQueue ProcessQueue(N);
	int T = 0, Q = 0, S = 0, Finished = 0, start = 0, StartProcess = 0, StartSwitch = 0;
	SearchAndInsert(T, ProcessQueue, ArrivalTime, IDs, N, start);
	while (ProcessQueue.IsEmpty())
	{
		T++;
		SearchAndInsert(T, ProcessQueue, ArrivalTime, IDs, N, start);
		
	}
	string CurrProcess="NULL";
	while (Finished!=N)
	{

		if (!Processing && !Switching)
		{
			while (ProcessQueue.IsEmpty())
			{
				T++;
				SearchAndInsert(T, ProcessQueue, ArrivalTime, IDs, N, start);
				
				
			}
			ProcessQueue.PrintQueue(MyFile);

			ProcessQueue.deQueue(CurrProcess);
		
			if (RemainTime[CurrProcess] != RunTime[CurrProcess])
			{
				Switching = true;
				ResumeProcess = true;
				StartSwitch = T;
			}
			else
			{
				
				StartProcess = T;
				Processing = true;
				StartTime[CurrProcess] = T;
			}

		}
		if (Processing)
		{
			if (Q == Quantum)
			{
				Q = 0;
				Switching = true;
				Processing = false;
				StartSwitch = T + 1;
				SearchAndInsert(T, ProcessQueue, ArrivalTime, IDs, N, start);
				MyFile << "Executing Process: " << CurrProcess << " Started At :" << StartProcess << " Stopped At : " << T << " Remaining Time :" << RemainTime[CurrProcess] << endl; 
				T++;
				continue;

			}
			else
			{
				
				if (RemainTime[CurrProcess] == 0)
				{
					CurrProcessDone = true;
					Finished++;
					Processing = false;
					Q = 0;
					MyFile << "Executing Process: " << CurrProcess << " Started At : " << StartProcess << " Finished At : " << T << endl;
					FinishingTime[CurrProcess] = T;

				}

				else
				{
					RemainTime[CurrProcess]--;
					Q++;
				}
				SearchAndInsert(T, ProcessQueue, ArrivalTime, IDs, N, start);
			}
		}
		if(Switching)
		{
			if (S == SwitchTime)
			{
				Switching = false;
				S = 0;
				if (ResumeProcess)
				{
					Processing = true;
					StartProcess = T+1;
					ResumeProcess = false;
				}
				else
				{
					ProcessQueue.enQueue(CurrProcess);
					CurrProcess = "NULL";
				}
				MyFile << "Process Switching :   Started At : " << StartSwitch << "   Finished At : " << T << endl;
				SearchAndInsert(T, ProcessQueue, ArrivalTime, IDs, N, start);
				
			}
			else
			S++;
		}
		
	
		T++;
	
		SearchAndInsert(T, ProcessQueue, ArrivalTime, IDs, N, start);
		

	}
	MyFile.close();
	DoTheMathAndTheOutput(IDs,
		RunTime,
		ArrivalTime,
		StartTime,
		FinishingTime,
		Name,
		T
	);
}


int main()
{
	vector<string> PID;
	map <string, int>RunTime;
	map<string, int>ArrivalTime;
	map<string, int>RemainingTime;
	map<string, int>StartTime;
	map<string, int>FinishingTime;
	string name;
	int Quantum, Switch;
	cin >> name;
	ifstream MyFile;
	MyFile.open(name+".txt");
	while (!MyFile.is_open())
	{
		cout << "Sorry,No file exists with this name, could you please re-enter the name?";
		cin >> name;
		MyFile.open(name + ".txt");
		
	}
	int i = 0;
	string Buffer;
	
	if (MyFile.is_open())
	{
		MyFile >> Buffer;
		MyFile >> Quantum;
		MyFile >> Buffer;
		MyFile >> Switch;
		MyFile >> Buffer;
		MyFile >> Buffer;
		MyFile >> Buffer;

		do
		{
			MyFile >> Buffer;
			if (Buffer == "NULL")
				break;
			PID.push_back(Buffer);
			MyFile >> RunTime[PID[i]];
			MyFile >> ArrivalTime[PID[i]];
			RemainingTime[PID[i]] = RunTime[PID[i]];
			Buffer = "NULL";
			i++;
		} while (!MyFile.eof());
	}
	MyFile.close();
	for (int i = 0;i < 8;i++)
		cout << PID[i] << " " << ArrivalTime[PID[i]] << " " << RunTime[PID[i]] << endl;
	RoundRobin(PID, RemainingTime, RunTime, ArrivalTime, StartTime, FinishingTime,PID.size(), Quantum, Switch,name);
	for (int i = 0;i < 8;i++)
		cout << PID[i] << " " << StartTime[PID[i]] << " " << FinishingTime[PID[i]] << endl;
	int s;
	cin >> s;

	
	
	return 0;
}
