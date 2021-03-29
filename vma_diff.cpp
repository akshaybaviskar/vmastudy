#include<fstream>
#include<vector>
#include<iostream>
#include<sstream>
#include<cmath>
#include<sys/stat.h>
#include<unistd.h>
#include<algorithm>
#include<iomanip>

using namespace std;

// G - Grown
// S - Shrinked
// D - from Downward
// U - Upward
// B - Bothside

void increase_vmacounts(unsigned long);

//string bms[47] = {"bwaves_r", "bwaves_s","blender_r", "cactuBSSN_r", "cactuBSSN_s", "cam4_r","deepsjeng_r", "deepsjeng_s", "exchange2_r", "exchange2_s", "fotonik3d_r", "fotonik3d_s", "imagick_r", "imagick_s", "lbm_r", "lbm_s", "leela_r", "leela_s", "mcf_r", "mcf_s", "nab_r", "nab_s", "namd_r", "omnetpp_r",  "omnetpp_s", "parest_r", "perlbench_r", "perlbench_s", "pop2_s", "povray_r", "roms_r", "roms_s", "wrf_r", "wrf_s", "x264_r", "x264_s", "xalancbmk_r", "xalancbmk_s", "xz_r", "xz_s", "canneal", "bc", "bfs", "cc", "graph500", "pr", "sssp"};

string bms[10] = {"pr_twitter", "cc_twitter","bc_twitter", "pr_kron", "cc_kron", "cc_urand","tc_urandU", "bfs_urand", "canneal1", "btree_1000_100"};


vector<unsigned long int> startaddr_ul;
vector<unsigned long int> endaddr_ul;

void insert_str(string a, int f)
{
	unsigned long s;
	stringstream ss(a);
	ss>>hex>>s;
	
	if(f)
	{
		startaddr_ul.push_back(s);
	}
	else
	{
		endaddr_ul.push_back(s);
	}
}

int main(int argc, char** argv)
{
	int x = 1;
	struct stat buffer;

	//int fg = stoi(argv[1]);

	for(int k=0;k<10;k++)
	{
		string fname = bms[k] + "/" + "vma_" + to_string(x) +".txt";
		int x = 1;
		string ofname = bms[k] + "/d_r_" + bms[k] +".txt";
		ofstream outfile(ofname);

      if(outfile.is_open() == false){ cout<<"unable to create "<<ofname<<endl; exit(-1);}
		vector<unsigned long> prev_startaddr;
		vector<unsigned long> prev_endaddr;

		while(stat(fname.c_str(), &buffer) == 0 )
		{
		//	cout<<"*************************************Analyzing file : "<<x<<endl;
			ifstream infile(fname);
			//cout<<fname<<endl;
			outfile<<fname<<endl;

			vector<string> startaddr;
			vector<string> endaddr;

			startaddr_ul.clear();
			endaddr_ul.clear();
			vector<unsigned long> pages;
		   	string line;
			string lastend = "";
			int count = 0;

			while(getline(infile, line))
			{
				if(x==1)
				outfile<<line<<endl;
				if(count == 0)
				{
					count++;
					continue;
				}
				count++;
				
				if((line[8] == '-') && (line[17] == ' ')) 
				{
					if(lastend == "")
					{
						startaddr.push_back(line.substr(0,5));
						insert_str(line.substr(0,5), 1);
					}
					else
					{
						if(lastend.compare(line.substr(0,5)) != 0)
						{
							endaddr.push_back(lastend);
							startaddr.push_back(line.substr(0,5));
							insert_str(lastend, 0);
							insert_str(line.substr(0,5), 1);
						}
					}					
					lastend = line.substr(9,5);
				}
				else if((line[8] == '-') && (line[18] == ' ')) 
				{
					if(lastend == "")
					{
						startaddr.push_back(line.substr(0,5));
						insert_str(line.substr(0,5), 1);
					}
					else
					{
						if(lastend.compare(line.substr(0,5)) != 0)
						{
							endaddr.push_back(lastend);
							startaddr.push_back(line.substr(0,5));
							insert_str(lastend, 0);
							insert_str(line.substr(0,5), 1);
						}
					}					
					lastend = line.substr(9,6);
				}
				else if((line[8] == '-') && (line[19] == ' ')) 
				{
					if(lastend == "")
					{
						startaddr.push_back(line.substr(0,5));
						insert_str(line.substr(0,5), 1);
					}
					else
					{
						if(lastend.compare(line.substr(0,5)) != 0)
						{
							endaddr.push_back(lastend);
							startaddr.push_back(line.substr(0,5));
							insert_str(lastend, 0);
							insert_str(line.substr(0,5), 1);
						}
					}					
					lastend = line.substr(9,7);
				}
				else if((line[10] == '-') && (line[21] == ' ')) 
				{
					if(lastend == "")
					{
						startaddr.push_back(line.substr(0,7));
						insert_str(line.substr(0,7), 1);
					}
					else
					{
						if(lastend.compare(line.substr(0,7)) != 0)
						{
							endaddr.push_back(lastend);
							startaddr.push_back(line.substr(0,7));
							insert_str(lastend, 0);
							insert_str(line.substr(0,7), 1);
						}
					}					
					lastend = line.substr(11,7);
				}
				else if((line[12] == '-') && (line[25] == ' ')) 
				{		
					if(lastend.compare(line.substr(0,9)) != 0)
					{
						//cout<<"compared "<<lastend<<" and "<<line.substr(0,9)<<endl;
						endaddr.push_back(lastend);
						startaddr.push_back(line.substr(0,9));
						insert_str(lastend, 0);
						insert_str(line.substr(0,9), 1);
					}
					lastend = line.substr(13,9);
				}
				else if((line[16] == '-') && (line[33] == ' ')) 
				{
					if(lastend.compare(line.substr(0,12)) != 0)
					{
						endaddr.push_back(lastend);
						startaddr.push_back(line.substr(0,13));
						insert_str(lastend, 0);
						insert_str(line.substr(0,13), 1);
					}
					lastend = line.substr(17,13);
				}
				else if(line[0] == 'V')
				{
				}
				else
				{
					cout<<"--------------something new------ at line no "<<count<<endl;
					return -1;
				}
				
			}
			endaddr.push_back(lastend);
			insert_str(lastend, 0);

			if(x == 1)
			{ 
				prev_startaddr = startaddr_ul;
				prev_endaddr = endaddr_ul; 
				x++;	
				fname = bms[k] + "/" + "vma_" + to_string(x) +".txt";
				for(int m=0;m<startaddr.size();m++)
				{
					//cout<<hex<<setfill('0')<<setw(13)<<startaddr_ul[m]<<" - "<<setfill('0')<<setw(13)<<endaddr_ul[m]<<endl;
					outfile<<hex<<setfill('0')<<setw(13)<<startaddr_ul[m]<<" - "<<setfill('0')<<setw(13)<<endaddr_ul[m]<<endl;
				}

				//cout<<"_______________________________________________________________________________________________"<<endl;
				outfile<<"_______________________________________________________________________________________________"<<endl;
				continue;
			}

			int i = 0; //startaddr
			int j = 0; //prev_startaddr

			while(i<startaddr_ul.size() && j<prev_startaddr.size())
			{	
				if((startaddr_ul[i] == prev_startaddr[j]) && (endaddr_ul[i] == prev_endaddr[j]))
				{
					i++;
					j++;
					continue;
				}


				if(startaddr_ul[i] < prev_startaddr[j])
				{
					if(endaddr_ul[i] == prev_endaddr[j])   // 4 6 -> 3 6
					{
						//cout<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<"  GU  by ";
						//cout<<dec<<(prev_startaddr[j] - startaddr_ul[i])<<" ("<<(prev_startaddr[j] - startaddr_ul[i])/512<<")"<<endl;
						outfile<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<"  GU  by ";
						outfile<<dec<<(prev_startaddr[j] - startaddr_ul[i])<<" ("<<(prev_startaddr[j] - startaddr_ul[i])/512<<")"<<endl;
						/*<<setfill('0')<<setw(13)<<startaddr_ul[i]<<"-"<<setfill('0')<<setw(13)<<endaddr_ul[i]<<endl;*/
						i++;
						j++;
					}
					else if(endaddr_ul[i] < prev_startaddr[j]) // 4 6 -> 1 2
					{
						//cout<<hex<<setfill('0')<<setw(13)<<startaddr_ul[i]<<"-"<<setfill('0')<<setw(13)<<endaddr_ul[i]<<" added of size ";
						//cout<<dec<<(endaddr_ul[i]-startaddr_ul[i])<<" ("<<(endaddr_ul[i]-startaddr_ul[i])/512<<")"<<endl;
						outfile<<hex<<setfill('0')<<setw(13)<<startaddr_ul[i]<<"-"<<setfill('0')<<setw(13)<<endaddr_ul[i]<<" added of size ";
						outfile<<dec<<(endaddr_ul[i]-startaddr_ul[i])<<" ("<<(endaddr_ul[i]-startaddr_ul[i])/512<<")"<<endl;
						i++;
					}
					else if(endaddr_ul[i] < prev_endaddr[j]) // 4 6 -> 2 5
					{
						//cout<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" GU by ";
						//cout<<dec<<(prev_startaddr[j] - startaddr_ul[i])<<" ("<<(prev_startaddr[j] - startaddr_ul[i])/512<<")"<<endl;
						//cout<<" SD by  "<<(prev_endaddr[j] - endaddr_ul[i])<<" ("<<(prev_endaddr[j] - endaddr_ul[i])/512<<")"<<endl;
						outfile<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" GU by ";
						outfile<<dec<<(prev_startaddr[j] - startaddr_ul[i])<<" ("<<(prev_startaddr[j] - startaddr_ul[i])/512<<")"<<endl;
						outfile<<" SD by  "<<(prev_endaddr[j] - endaddr_ul[i])<<" ("<<(prev_endaddr[j] - endaddr_ul[i])/512<<")"<<endl;
						/*setfill('0')<<setw(13)<<startaddr_ul[i]<<"-"<<setfill('0')<<setw(13)<<endaddr_ul[i]<<endl;*/
						i++;j++;
					}
					else // 4 6 -> 2 8
					{
						//cout<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" GB by";
						//cout<<dec<<(startaddr_ul[i] - prev_startaddr[j])<<" ("<<(startaddr_ul[i] - prev_startaddr[j])/512<<")"<<endl;
						//cout<<" , "<<(endaddr_ul[i] - prev_endaddr[j])<<" ("<<(endaddr_ul[i] - prev_endaddr[j])/512<<")"<<endl;
						outfile<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" GB by";
						outfile<<dec<<(startaddr_ul[i] - prev_startaddr[j])<<" ("<<(startaddr_ul[i] - prev_startaddr[j])/512<<")"<<endl;
						outfile<<" , "<<(endaddr_ul[i] - prev_endaddr[j])<<" ("<<(endaddr_ul[i] - prev_endaddr[j])/512<<")"<<endl;
						/*setfill('0')<<setw(13)<<startaddr_ul[i]<<"-"<<setfill('0')<<setw(13)<<endaddr_ul[i]<<endl;*/
						i++;j++;
					}
					
				}
				else if(startaddr_ul[i] > prev_startaddr[j])
				{
					if(endaddr_ul[i] == prev_endaddr[j]) //3 5 -> 4 5
					{
						//cout<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" SU by ";
						//cout<<dec<<(startaddr_ul[i] - prev_startaddr[j])<<"("<<(startaddr_ul[i] - prev_startaddr[j])/512<<")"<<endl;
						outfile<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" SU by ";
						outfile<<dec<<(startaddr_ul[i] - prev_startaddr[j])<<"("<<(startaddr_ul[i] - prev_startaddr[j])/512<<")"<<endl;
						/*setfill('0')<<setw(13)<<startaddr_ul[i]<<"-"<<setfill('0')<<setw(13)<<endaddr_ul[i]<<endl;*/
						i++;
						j++;
					}
					else if(startaddr_ul[i] > prev_endaddr[j]) // 3 5 -> 6 10
					{
						//cout<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" deleted of size ";
						//cout<<dec<<(prev_endaddr[j] - prev_startaddr[j])<<"("<<(prev_endaddr[j] - prev_startaddr[j])/512<<")"<<endl;
						outfile<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" deleted of size ";
						outfile<<dec<<(prev_endaddr[j] - prev_startaddr[j])<<"("<<(prev_endaddr[j] - prev_startaddr[j])/512<<")"<<endl;
						j++;
					}
					else if(endaddr_ul[i] > prev_endaddr[j]) //3 5 -> 2 6
					{
						//cout<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" GD by ";
						//cout<<dec<<(endaddr_ul[i] - prev_endaddr[j])<<"("<<(endaddr_ul[i] - prev_endaddr[j])/512<<")";
						//cout<<" SU by "<<(startaddr_ul[i] > prev_startaddr[j])<<" ("<<(startaddr_ul[i] > prev_startaddr[j])/512<<")"<<endl;
						outfile<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" GD by ";
						outfile<<dec<<(endaddr_ul[i] - prev_endaddr[j])<<"("<<(endaddr_ul[i] - prev_endaddr[j])/512<<")";
						outfile<<" SU by "<<(startaddr_ul[i] > prev_startaddr[j])<<" ("<<(startaddr_ul[i] > prev_startaddr[j])/512<<")"<<endl;
						/*<<setfill('0')<<setw(13)<<startaddr_ul[i]<<"-"<<setfill('0')<<setw(13)<<endaddr_ul[i]<<endl;*/
						i++;j++;
					}
					else // 3 5 -> 4 4
					{
						//cout<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" SB by ";
						outfile<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" SB by ";
						//cout<<
						/*<<setfill('0')<<setw(13)<<startaddr_ul[i]<<"-"<<setfill('0')<<setw(13)<<endaddr_ul[i]<<endl;*/
						i++;j++;
					}
				}
				else //(startaddr[i] == prev_startaddr[j])
				{
					if(endaddr_ul[i]>prev_endaddr[j]) // 4 5 -> 4 6
					{
						//cout<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" GD by ";
						//cout<<dec<<(endaddr_ul[i]-prev_endaddr[j])<<"("<<(endaddr_ul[i]-prev_endaddr[j])/512<<")"<<endl;
						outfile<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" GD by ";
						outfile<<dec<<(endaddr_ul[i]-prev_endaddr[j])<<"("<<(endaddr_ul[i]-prev_endaddr[j])/512<<")"<<endl;

						/*<<setfill('0')<<setw(13)<<startaddr_ul[i]<<"-"<<setfill('0')<<setw(13)<<endaddr_ul[i]<<endl;*/
						i++;j++;
					}
					else if(endaddr_ul[i] < prev_endaddr[j]) // 4 5 -> 4 3
					{
						//cout<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" SD by ";
						//cout<<dec<<(prev_endaddr[j] - endaddr_ul[i])<<"("<<(prev_endaddr[j] - endaddr_ul[i])/512<<")"<<endl;
						outfile<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<" SD by ";
						outfile<<dec<<(prev_endaddr[j] - endaddr_ul[i])<<"("<<(prev_endaddr[j] - endaddr_ul[i])/512<<")"<<endl;
						/*<<setfill('0')<<setw(13)<<startaddr_ul[i]<<"-"<<setfill('0')<<setw(13)<<endaddr_ul[i]<<endl;*/
						i++;j++;
					}
				}
			}

			while(i<startaddr_ul.size())
			{
				//cout<<hex<<setfill('0')<<setw(13)<<startaddr_ul[i]<<"-"<<setfill('0')<<setw(13)<<endaddr_ul[i]<<"   added"<<endl;
				outfile<<hex<<setfill('0')<<setw(13)<<startaddr_ul[i]<<"-"<<setfill('0')<<setw(13)<<endaddr_ul[i]<<"   added"<<endl;
				i++;
			}

			
			while(j<prev_startaddr.size())
			{
				//cout<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<"   deleted"<<endl;
				outfile<<hex<<setfill('0')<<setw(13)<<prev_startaddr[j]<<"-"<<setfill('0')<<setw(13)<<prev_endaddr[j]<<"   deleted"<<endl;
				j++;
			}

			prev_startaddr = startaddr_ul;
			prev_endaddr = endaddr_ul;
					
			infile.close();
			x++;
			fname = bms[k] + "/" + "vma_" + to_string(x) +".txt";
			//cout<<"_______________________________________________________________________________________________"<<endl;
			outfile<<"_______________________________________________________________________________________________"<<endl;
		}
		outfile.close();
		//cout<<"============================================================================================================================="<<endl;
	}
	
	return 1;
}
