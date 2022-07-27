// one tab has many web
// move forward and backward between web
// process is a tab 
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <conio.h> 
#include <fstream>
#include <Windows.h>
#include <cstdlib>
using namespace std;
struct Node
{
	string url;
	Node*pNext;
	Node*pPrevious;	
};
struct LIST
{
	Node*pHead;
	Node*pTail;
	Node*pCurrent;
	string type;// normal private  
	string history[100];
	string bookMark[100]; 
	int topHistory; // the last index of history array 
	int topBookMark; // the last index of BookMark array 
};
static int index  = 0; // the number of tab 
static int indexCurrent = 0; // Check current tab 
static LIST tab[100]; // tab array // tab[0]
// change color 
void SetColor(int color) 
{
	WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
    	wColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}
void MainMenu(LIST &list);// proper type
bool CheckTab()// if remove the only tab left, then throw error whenever 
{		      // do action
	if(index==-1&&indexCurrent==-1)
		return true;
	return false;
}
void ChangePrivate(LIST &list)
{
	if(CheckTab()==true)
	{
		SetColor(12);// red 
		cout<<"Tab null, Create Tab first!!\n";
		SetColor(7);// white 
		return;
	}
	SetColor(11); 
	cout<<"You are in private type\n\n";
	SetColor(7); 
	list.type="private";
}
void ChangeNormal(LIST &list)
{
	if(CheckTab()==true)
	{
		SetColor(12);
		cout<<"Tab null, Create Tab first!!\n";
		SetColor(7);
		return;
	}
	SetColor(11); 
	cout<<"You are in normal type\n\n";
	SetColor(7); 
	list.type ="normal";
}
// load the data bookmark 
void GetBookMark(LIST &list,const char *filename)
{
	ifstream file;
	file.open(filename);
	while(!file.eof())	
	{
		getline(file,list.bookMark[++list.topBookMark]);
	}
	file.close();
}
void Init(LIST &list, string t)
{
	list.pHead=NULL;
	list.pTail=NULL;
	list.pCurrent=NULL;
	list.type=t;
	list.topHistory=-1;
	list.topBookMark=-1; 
}
LIST InitTab(LIST list)// tab[1], list = tab[0] 
{
	LIST tab;
	tab.pHead=NULL;
	tab.pTail=NULL;
	tab.pCurrent=NULL;
	tab.type="normal";
	tab.topBookMark = list.topBookMark;
	tab.topHistory = list.topHistory;
	for(int i=0;i<=list.topBookMark;i++)
	{
		tab.bookMark[i]=list.bookMark[i]; 
	} 
	for(int i=0;i<=list.topHistory;i++)
	{ 
		tab.history[i]=list.history[i];	
	}
	return tab; 
} 
Node*CreateNode(string x)
{
	Node*pNode=new Node;
	if (pNode!=NULL)
	{
		pNode->url=x;
		pNode->pNext=NULL;
		pNode->pPrevious=NULL;
		return pNode;
	}
	return pNode;
}
void AddWeb(LIST &list, string x)
{
	Node*pNode=CreateNode(x);
	if (list.pTail==NULL) 
	{
		list.pTail=pNode;
		list.pHead=pNode;
		list.pCurrent=pNode;
		if(list.type=="normal") 
			list.history[++list.topHistory]=x;
	}
	else // addTail 
	{
		list.pTail->pNext=pNode;
		pNode->pPrevious=list.pTail;
		list.pTail=pNode;
		list.pCurrent=list.pTail;
		if(list.type=="normal") 
			list.history[++list.topHistory]=x;
	}
}
// show history array
void ShowHistory(LIST list)
{
	if (list.topHistory==-1)
	{
		SetColor(12);
		cout<<"Null history, please access web to have browser history!\n\n";
		SetColor(7);
	}
	else
	{
		if (list.type=="normal")
		{
			SetColor(11);
			cout<<"Your history: "<<endl;
			SetColor(7);
			for (int i=0; i<=list.topHistory; i++)
			{
				cout<<i+1<<": "<<list.history[i]<<endl;
			}
			cout<<"\n"; 
		}
		else
		{
			SetColor(12);
			cout<<"You are in private type, cannot access browser history!\n\n";
			SetColor(7);
		}
	}
}
// Show Web ( Show all node in DLL) 
void ShowProcess(LIST list)
{
	if(list.pHead == NULL)
	{
		SetColor(12); 
		cout<<"None";
		SetColor(7); 
		return; 
	}		
	for(Node*k = list.pHead;k!=NULL;k=k->pNext)
	{ 
		if(k == list.pCurrent)
		{
			SetColor(9); 
			cout<<k->url;
			SetColor(7); 
		} 
		else
			cout<<k->url; 
		if(k!=list.pTail)
			cout<<" -> ";	
	} 
}
// Show Book Mark
void ShowBookMark(LIST list)
{
	cout<<"Book mark: " ;
	if(list.topBookMark == -1)
	{
		SetColor(12); 
		cout<<"None";
		SetColor(7); 
		return; 
	}		
	for(int i=0;i<=list.topBookMark;i++)
	{ 
		cout<<list.bookMark[i]; 
		if(i!=list.topBookMark)
			cout<<"  -  ";	
	} 
} 
// Show all Web ( show all node in DLL) 
void ShowTab()
{
	if(CheckTab()==true)
	{
		SetColor(12);
		cout<<"Tab null\n";
		SetColor(7);
		return;
	}
	for(int i=0;i<=index;i++)
	{	 
		if(i==indexCurrent)
		{
			SetColor(10);
			cout<<"Tab "<<i+1; 
			SetColor(7);
		}
		else
			cout<<"Tab "<<i+1;
		cout<<"("<<tab[i].type<<") : "; 
		ShowProcess(tab[i]);	
		cout<<endl;	 
	}
}  
// Show all web
int ShowWeb(LIST list)
{
	int i=1; 
	for(Node*k=list.pHead;k!=NULL;k=k->pNext)
	{
		cout<<i++<<": "<<k->url<<endl;
	}
	return --i;	
}  
// visit a new Web ( AddTail new node in DLL) 
void Visit(LIST &list, string url)
{
	if(CheckTab()==true)
	{
		SetColor(12);
		cout<<"Tab null, Create Tab first!!\n";
		SetColor(7);
		return;
	}
	AddWeb(list,url); 
	SetColor(11); 
	cout<<"You are in "<<url<<endl<<endl;
	SetColor(7); 
}
// Back to previous Web ( Current node -> pPrevious) 
void BackWard(LIST &list)
{
	if(CheckTab()==true)
	{
		SetColor(12);
		cout<<"Tab null, Create Tab first!!\n";
		SetColor(7);
		return;
	}
	if(list.pHead == NULL)
	{
		SetColor(12);
		cout<<"Don't have any page to Back!\n\n";
		SetColor(7); 
	}
	else if (list.pCurrent->pPrevious!=NULL)
	{ 
		list.pCurrent=list.pCurrent->pPrevious;
		SetColor(11); 
		cout<<"You are in "<<list.pCurrent->url<<endl<<endl;
		SetColor(7); 
	}
	else
	{
		SetColor(12);
		cout<<"It's the first page, cannot Back!\n\n";
		SetColor(7);
	}
}
// Forward to next Web ( current node -> pNext) 
void Forward(LIST &list)
{
	if(CheckTab()==true)
	{
		SetColor(12);
		cout<<"Tab null, Create Tab first!!\n";
		SetColor(7);
		return;
	}
	if(list.pHead == NULL)
	{
		SetColor(12);
		cout<<"Don't have any page to Forward!\n\n";
		SetColor(7); 
	}
	else if (list.pCurrent->pNext!=NULL)
	{
		list.pCurrent=list.pCurrent->pNext;
		SetColor(11); 
		cout<<"You are in "<<list.pCurrent->url<<endl<<endl;
		SetColor(7); 
	}
	else
	{
		SetColor(12);
		cout<<"It's the last page, cannot Forward!\n\n";
		SetColor(7); 
	}	
}
void ShowBookMarkList(LIST list)
{
	for(int i=0;i<=list.topBookMark;i++)
	{ 
		cout<<i+1<<": "<<list.bookMark[i]<<endl; 	
	}	
} 
// Book mark a web
void BookMark(LIST &list)
{
	if(CheckTab()==true)
	{
		SetColor(12);
		cout<<"Tab null, Create Tab first!!\n";
		SetColor(7);
		return;
	}
	if(list.pHead == NULL)
	{
		SetColor(12);
		cout<<"List NULL\n";
		SetColor(7);
		return;
	}
	int limit = ShowWeb(list); // show selection tab 
	int pos; // index want to bookmark 
	do
	{
		cout<<"Enter Web index to BookMark: ";
		cin>>pos; 
		pos--; 	
		if(pos<0||pos>limit-1)
		{
			SetColor(12);
			cout<<"Invalid index, enter again!\n";
			SetColor(7);
		}
	}while(pos<0||pos>limit-1);
	int i = 0;
	for(Node*k=list.pHead;k!=NULL;k=k->pNext)
	{
		if(i==pos)
		{
			for(int j=0;j<=list.topBookMark;j++)// check if already bookmark 
			{	
				if(list.bookMark[j]==k->url)
				{
				SetColor(12);
				cout<<"\nAlready BookMark this page\n\n";
				SetColor(7);
				return;	
				}	
			}	  
			list.bookMark[++list.topBookMark] = k->url;	
		}
		i++;	 
	}
} 
// unMark a website in BookMark bar
void UnMark(LIST &list)
{
	if(list.topBookMark==-1)
	{
		SetColor(12);
		cout<<"BookMark NULL\n\n";
		SetColor(7); 
		return; 
	} 
	ShowBookMarkList(list); 
	int pos; 
	do
	{
		cout<<"Enter Web index to Un-Mark: ";
		cin>>pos; 
		pos--; 	
		if(pos<0||pos>list.topBookMark)
		{
			SetColor(12);
			cout<<"Invalid index, enter again!\n";
			SetColor(7);
		}
	}while(pos<0||pos>list.topBookMark);
	for(int i=0;i<=list.topBookMark;i++)
	{
		if(i==pos)
		{	
			cout<<"BookMark ";
			SetColor(11); 
			cout<<pos+1<<": "; 
			cout<<list.bookMark[i];
			SetColor(7); 
			cout<<" is removed\n"<<endl; 
			SetColor(7); 
			for(int j=pos;j<list.topBookMark;j++)
				list.bookMark[j] = list.bookMark[j+1];
			list.topBookMark--;
			break;	
		}	
	}  
}
// Un all mark
void UnAllMark(LIST &list)
{
	if(list.topBookMark==-1)
	{
		SetColor(12);
		cout<<"BookMark NULL\n\n";
		SetColor(7); 
		return; 
	}
	while (list.topBookMark!=-1)
	{
		list.history[list.topBookMark--]="";
	}
	SetColor(12);
	cout<<"BookMark is Cleared\n\n"; 
	SetColor(7); 
	
} 
// visit a bookmark
void VisitBookMark(LIST &list)
{
	if(CheckTab()==true)
	{
		SetColor(12);
		cout<<"Tab null, Create Tab first!!\n";
		SetColor(7);
		return;
	}
	if(list.topBookMark==-1)
	{
		SetColor(12);
		cout<<"BookMark NULL\n\n";
		SetColor(7);
		return;
	}		
	ShowBookMarkList(list); 
	int pos; 
	do
	{
		cout<<"Enter BookMark index to visit: ";
		cin>>pos; 
		pos--; 	
		if(pos<0||pos>list.topBookMark)
		{
			SetColor(12);
			cout<<"Invalid index, enter again!\n";
			SetColor(7);
		}
	}while(pos<0||pos>list.topBookMark);
	Visit(list,list.bookMark[pos]);	
} 
// Clear all history ( clear history array) 
void ClearHistory(LIST &list)
{
	if(list.topHistory==-1)
	{
		SetColor(12);
		cout<<"Dont have any history to clear!\n\n";
		SetColor(7);
		return;
	} 
	if(list.type=="private")
	{
		SetColor(12);
		cout<<"You are in private type, cannot access browser history!\n\n";
		SetColor(7);
		return;
	}
	while (list.topHistory!=-1)
	{
		list.history[list.topHistory--]="";
	}
	SetColor(12);
	cout<<"History is Cleared\n\n"; 
	SetColor(7); 
}
void Update(int pos)// tab[0] need update 
{
	tab[pos].topHistory=-1;
	tab[pos].topBookMark=-1; 
	for(int i=0;i<=tab[indexCurrent].topHistory;i++)
	{
		tab[pos].topHistory++;
		tab[pos].history[i]=tab[indexCurrent].history[i];
	} 
	for(int i=0;i<=tab[indexCurrent].topBookMark;i++)
	{
		tab[pos].topBookMark++;
		tab[pos].bookMark[i]=tab[indexCurrent].bookMark[i];	
	} 
} 
// Clear all process / Web / DLL // RemoveAll
void ClearATab()
{
	if(CheckTab()==true)
	{
		SetColor(12);
		cout<<"Tab null, Create Tab first!!\n";
		SetColor(7);
		return;
	}
	for(int i=0;i<=index;i++)
	{
		cout<<i+1<<". Tab "<<i+1<<endl;	
	} 	
	int pos; 
	do
	{
		cout<<"Enter Tab index to clear: ";
		cin>>pos; 
		pos--; 	
		if(pos<0||pos>index)
		{
			SetColor(12);
			cout<<"Invalid index, enter again!\n";
			SetColor(7);
		}
	}while(pos<0||pos>index);
	// update the indexCurrent 
	if(indexCurrent>=pos)
	{
		if(pos==0&&index==0)// the only tab left
		{ 
			index=-1;
			indexCurrent=-1;
			return;	
		} 
		else if(pos==indexCurrent&&pos!=index) // pos != pTail 
		{
			Update(indexCurrent+1); // update the data in indexCurrent	
		} 
		else
		{
			Update(indexCurrent-1);
			indexCurrent--;	
		}
		 
	}
	for(int i=pos;i<index;i++)
		tab[i] = tab[i+1];	
	index--; 
	SetColor(11); 
	cout<<"Tab is cleared\n\n";
	SetColor(7); 
	// after clearing a tab, then call the menu for the currenttab 
	system("pause");
	system("cls");
	MainMenu(tab[indexCurrent]);
}
// Switch tab
// return tab index want to switch 
int SwitchTab()
{ 
	for(int i=0;i<=index;i++)
	{
		cout<<i+1<<". Tab "<<i+1<<endl;	
	} 
	int pos; 
	do
	{
		cout<<"Enter Tab index to visit: ";
		cin>>pos; 
		pos--; 	
		if(pos<0||pos>index)
		{
			SetColor(12);
			cout<<"Invalid index, enter again!\n";
			SetColor(7);
		}
	}while(pos<0||pos>index);
	if(pos==indexCurrent)
	{
		SetColor(12);
		cout<<"You already in tab "<<pos+1<<"\n";
		SetColor(7);
		system("pause");
		return -1;
	}
	Update(pos); // update history and bookmark for new tab
	indexCurrent = pos; 
	return pos; 
} 
int Size(LIST list)
{
	int n=0;
	Node *pTemp=list.pHead;
	while (pTemp!=NULL)
	{
		pTemp=pTemp->pNext;
		n++;
	}
	return n;
}
// Remove 1 Web ( Remove Web has 'pos' index) 
void RemoveAWeb(LIST &list)
{
	if(CheckTab()==true)
	{
		SetColor(12);
		cout<<"Tab null, Create Tab first!!\n";
		SetColor(7);
		return;
	}
	if (list.pHead == NULL)
	{
		SetColor(12);
		cout << "List NULL\n";
		SetColor(7);
		return;
	} 
	int limit = ShowWeb(list);
	int pos; // index want to remove 
	do
	{
		cout<<"Enter Web index to remove: ";
		cin>>pos; 
		pos--;	 
		if(pos<0||pos>limit-1)
		{
			SetColor(12);
			cout<<"Invalid index, Enter again\n";
			SetColor(7);	
		} 
	} while(pos<0||pos>limit-1);
	// update index Current 
	int indexPcurr=0; 
	for(Node*k=list.pHead;k!=NULL;k=k->pNext)
	{
		if(k==list.pCurrent)
		{
			break; 
		}
		indexPcurr++;	
	}  
	if(indexPcurr==pos)
	{
		if(list.pCurrent == list.pHead)
		{
			list.pCurrent = list.pCurrent->pNext;
		}
		else 
			list.pCurrent = list.pCurrent->pPrevious;
	}
	if (pos==0) // remove head  
	{
		if(list.pHead->pNext == NULL)
		{
			list.pHead = NULL;
			list.pTail = NULL;
			list.pCurrent = NULL;
			return;	
		} 
		Node* temp = list.pHead;
		list.pHead = list.pHead->pNext;
		list.pHead->pPrevious = NULL;
		delete(temp);		
		return; 
	}		
	if (pos==Size(list)-1) // remove tail 
	{ 
		list.pTail = list.pTail->pPrevious;
		delete(list.pTail->pNext);
		list.pTail->pNext = NULL;
		return; 
	}
	// Remove 'pos' index 
	Node *pPre=NULL;
	Node *pCurrent=list.pHead;
	int i=0;
	while (pCurrent!=NULL)
	{
		if (i==pos) break;
		pPre=pCurrent;
		pCurrent=pCurrent->pNext;
		i++;
	}
	pPre->pNext=pCurrent->pNext;
	pCurrent->pNext->pPrevious=pPre;
	pCurrent->pNext=NULL;
	pCurrent->pPrevious=NULL;
	delete pCurrent;
	pCurrent=NULL;
}
// Remove  'pos' index in history array 
void ClearXHistory(LIST &list)
{
	if(list.topHistory==-1)
	{
		SetColor(12);
		cout<<"Dont have any history to clear!\n\n";
		SetColor(7);
		return;
	} 
	if(list.type=="private")
	{
		SetColor(12);
		cout<<"You are in private type, cannot access browser history!\n\n";
		SetColor(7);
		return;
	}
	int pos; // index want to remove
	ShowHistory(list);
	do
	{
		cout<<"Enter history index that you want to clear: ";
		cin>>pos;	
		pos--; // reduce 1 value of x so that it will match  index started by 1	
		if(pos<0||pos>list.topHistory)
		{
			SetColor(12);
			cout<<"Invalid index, Enter again\n";
			SetColor(7);	
		} 
	} while(pos<0||pos>list.topHistory);
	for(int i=0;i<=list.topHistory;i++)
	{
		if(i==pos)
		{	
			cout<<"History ";
			SetColor(11); 
			cout<<pos+1<<": "; 
			cout<<list.history[i];
			SetColor(7); 
			cout<<" is removed"<<endl; 
			SetColor(7); 
			for(int j=pos;j<list.topHistory;j++)
				list.history[j] = list.history[j+1];
			list.topHistory--;
			break;	
		} 
	} 
	ShowHistory(list);
} 
// move to a history 
//( move to the node has url same as data in histoy array)
void MoveToHistory(LIST &list)
{
	if(CheckTab()==true)
	{
		SetColor(12);
		cout<<"Tab null, Create Tab first!!\n";
		SetColor(7);
		return;
	}
	if(list.topHistory==-1)
	{
		SetColor(12);
		cout<<"Dont have any history to visit!\n\n";
		SetColor(7);
		return;
	} 
	if(list.type=="private")
	{
		SetColor(12);
		cout<<"You are in private type, cannot access browser history!\n\n";
		SetColor(7);
		return;
	}
	ShowHistory(list);
	int pos;
	do
	{
		cout<<"Enter history index you want to move in: ";
		cin>>pos;
		pos--;	
		if(pos<0||pos>list.topHistory)
		{
			SetColor(12);
			cout<<"Invalid index, enter again!\n";
			SetColor(7);
		}
	}while(pos<0||pos>list.topHistory);
	Visit(list,list.history[pos]);		 
} 
// print goolge image in file to the console  
void PrintArt()
{
	ifstream file;
	file.open("AsciiArt.txt");  
	SetColor(11); 
	while(!file.eof())	
	{
		string str; 
		getline(file,str);
		cout<<str<<endl;
	}
	SetColor(7); // white 
	file.close(); 
} 
void PrintList(const char *filename)
{
	ifstream file;
	file.open(filename); 
	SetColor(9);  
	while(!file.eof())	
	{
		string str; 
		getline(file,str);
		cout<<str<<endl;
	}
	SetColor(7); 
	file.close(); 
} 
// covert string to int
int ConvertString(string str)
{
	int sum =0; 
	int len = str.length();
	for(int i=0;i<len;i++)
	{
		sum=sum*10+ (str[i]-'0');	
	} 
	return sum;	
} 
void MainMenu(LIST &list); 
// Show history Menu
void HistoryMenu(LIST &list)
{
	string optionTemp; // enter string not throw error 
	int option; // actually option 	
	do
	{
		cout<<"|--------------------------------------------------------------------------------------------------------|"<<endl;
		PrintArt();  
		PrintList("HistoryOption.txt");
		ShowTab();
		cout<<"|--------------------------------------------------------------------------------------------------------|"<<endl;  
		do
		{
			cout<<"Enter your option >> ";
			cin>>optionTemp;
			option=ConvertString(optionTemp); // convert to actually option 
			if(option<0||option>4)
			{
				SetColor(12);
				cout<<"Invalid option, Enter again!\n";
				SetColor(7);
			}
		}while(option<0||option>4); 
		switch (option)
		{
			case 1:
			{					
				ShowHistory(list);	
				cout<<endl; 
				system("pause");
				system("cls");
				break;	
			}
			case 2:
			{
				ClearXHistory(list);
				system("pause");
				system("cls");	
				break; 	
			} 
			case 3:
			{
				ClearHistory(list);
				system("pause");
				system("cls");
				break;	
			} 
			case 4:
			{
				MoveToHistory(list);	
				system("pause");
				system("cls");
				break;
			} 
			case 0:
			{
				return;	
			} 
		}	
	}while(option!=0);
}
void TabMenu(LIST &list)
{
	string optionTemp; // enter string not throw error 
	int option; // actually option 
	do
	{
		cout<<"|--------------------------------------------------------------------------------------------------------|"<<endl;
		PrintArt();  
		PrintList("TabOption.txt");
		ShowTab();
		ShowBookMark(list); 
		cout<<endl<<endl;  
		cout<<"|--------------------------------------------------------------------------------------------------------|"<<endl; 
		do
		{
			cout<<"Enter your option >> ";
			cin>>optionTemp;
			option=ConvertString(optionTemp); // convert to actually option 
			if(option<0||option>3)
			{
				SetColor(12);
				cout<<"Invalid option, Enter again!\n";
				SetColor(7);
			}
		}while(option<0||option>3); 
		switch (option)
		{	
			case 1:
			{
				system("cls");
				if(indexCurrent==index)
				{
				
					indexCurrent++; 
				}
				tab[++index] = InitTab(list);
				if(index==0)
				{ 
					tab[index] = list;	 
				} 
				MainMenu(tab[index]); 
				for(int i=0;i<=tab[index].topHistory;i++)
				{
					list.history[i]=tab[index].history[i];	
				} 
				for(int i=0;i<=tab[index].topBookMark;i++)
				{
					list.bookMark[i]=tab[index].bookMark[i];	
				} 
				system("pause");
				system("cls");
				break;		
			}
			case 2:
			{
				if(CheckTab()==true)
				{
					SetColor(12);
					cout<<"Tab null, Create Tab first!!\n";
					SetColor(7);
					system("pause");
					system("cls");
					break;
				}
				int pos= SwitchTab();
				system("cls");
				if(pos!=-1) //can't switch to the current tab
				{
					MainMenu(tab[pos]); 
					system("pause");
					system("cls");	
				}
				break;	
			} 
			case 3: 
			{
				ClearATab();	 
				system("pause");
				system("cls");
				break;	
			}  
			case 0:
				return; 
		}
	}while(option!=0);
}
// Book Mark menu
void BookMarkMenu(LIST &list)
{
	string optionTemp; // enter string not throw error 
	int option; // actually option 
	do
	{
		cout<<"|--------------------------------------------------------------------------------------------------------|"<<endl;
		PrintArt();  
		PrintList("BookMarkOption.txt");
		ShowTab();
		ShowBookMark(list); 
		cout<<endl<<endl;  
		cout<<"|--------------------------------------------------------------------------------------------------------|"<<endl; 
		do
		{
			cout<<"Enter your option >> ";
			cin>>optionTemp;
			option=ConvertString(optionTemp); // convert to actually option 
			if(option<0||option>4)
			{
				SetColor(12);
				cout<<"Invalid option, Enter again!\n";
				SetColor(7);
			}
		}while(option<0||option>4); 
		switch (option)
		{	
			case 1:
			{
				BookMark(list); 
				system("pause");
				system("cls");
				break; 
			}
			case 2:
			{
				UnMark(list);
				system("pause");
				system("cls");
				break;	
			} 
			case 3:
			{
				UnAllMark(list);
				system("pause");
				system("cls");
				break;
			}
			case 4:
			{ 
				VisitBookMark(list);
				system("pause");
				system("cls");
				break;	
			} 
			case 0:
				return; 
		}
	}while(option!=0);
}
// Show web menu 
void WebMenu(LIST &list)
{
	string optionTemp; // enter string not throw error 
	int option; // actually option 	
	do
	{
		cout<<"|--------------------------------------------------------------------------------------------------------|"<<endl;
		PrintArt();  
		PrintList("WebOption.txt");
		ShowTab();
		ShowBookMark(list); 
		cout<<endl<<endl; 
		cout<<"|--------------------------------------------------------------------------------------------------------|"<<endl; 
		do
		{
			cout<<"Enter your option >> ";
			cin>>optionTemp;
			option=ConvertString(optionTemp); // convert to actually option 
			if(option<0||option>6)
			{
				SetColor(12);
				cout<<"Invalid option, Enter again!\n";
				SetColor(7);
			}
		}while(option<0||option>6); 
		switch (option)
		{	
			case 1:
			{
				string url;
				if(CheckTab()==false)// only visit if there are a tab
				{
					cout<<"Enter the url: ";
					cin>>url;
				}
				Visit(list,url);
				system("pause");
				system("cls");
				break;	
			} 
			case 2:
			{
				Forward(list);
				system("pause");
				system("cls");
				break;		
			} 
			case 3:
			{
				BackWard(list);
				system("pause");
				system("cls");
				break;		
			}
			case 4:
			{
				RemoveAWeb(list); 
				system("pause");
				system("cls");
				break;	
			} 
			case 5:
			{
				system("cls");
				TabMenu(list); 
				system("cls");
				break;	
			} 	
			case 6:
			{
				system("cls");
				BookMarkMenu(list); 
				system("cls");
				break;	
			} 			
			case 0:
				return; 
		}	
	}while(option!=0);
} 
// show menu 
void MainMenu(LIST &list)
{ 
	string optionTemp; // enter string not throw error 
	int option; // actually option 
	do
	{
		cout<<"|--------------------------------------------------------------------------------------------------------|"<<endl;
		PrintArt();  
		PrintList("MainOption.txt");
		ShowTab();
		cout<<"|--------------------------------------------------------------------------------------------------------|"<<endl; 
		do
		{
			cout<<"Enter your option >> ";
			cin>>optionTemp;
			option=ConvertString(optionTemp); // convert to actually option 
			if(option<0||option>3)
			{
				SetColor(12);
				cout<<"Invalid option, Enter again!\n";
				SetColor(7);
			}
		}while(option<0||option>3); 
		switch (option)
		{
			case 1:
			{
				system("cls");
				WebMenu(list);
				system("cls");
				break;
			} 
			case 2:
			{
				system("cls");
				HistoryMenu(list);
				system("cls");
				break;
			} 
			case 3:
			{
				if(list.type=="normal")
					ChangePrivate(list);
				else
					ChangeNormal(list);
				system("pause");
				system("cls");
				break;	
			} 
			case 0:
				exit(0);
		}
	}while(option!=0);
}
int main()
{
	SetConsoleTitle("Browser History");
	SetConsoleOutputCP(65001); // allow unicode
	Init(tab[0],"normal");
	GetBookMark(tab[0],"BookMarkData.txt");
	MainMenu(tab[0]); 
	return 0;
}
