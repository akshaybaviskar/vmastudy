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

unsigned long max_contig_reg = 0;  /*maximum number of contigous regions*/
unsigned long max_contig_ps = 0;  // maximum size of contigous page size required
unsigned long max_total_ps = 0;  // maximum size of total memory required for page table
unsigned long max_mem_fp = 0;  // maximum total memory footprint by application
int max_vma_count = 0; // maximum number of total VMAsint vmas_2mb = 0;
int vmas_2mb = 0;
int max_vmas_2mb = 0;
vector<int> max_pte_pages_list; //stores pte page sizes when VMA had max memory footprint

vector<unsigned long int> startaddr_ul;
vector<unsigned long int> endaddr_ul;
vector<unsigned long int> startaddr_ul_2;
vector<unsigned long int> endaddr_ul_2;


int total_vma_updates = 0;
int maxcount = 0;

long avg_bloat = 0;
long max_bloat = 0;

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


void increase_vmacounts(unsigned long);

void print_2mb(string fname)
{
   unsigned int n = startaddr_ul_2.size();

   //	  cout<<"printing 2MB blocks"<<endl;
   unsigned long ptes = 262144;
   unsigned long int count = 0;
   unsigned int count00 = 0;
   unsigned int count7f = 0;
   unsigned int count55 = 0;
   unsigned int count56 = 0;
   unsigned int countff = 0;
   unsigned int countot = 0;
   unsigned long int last_s = 0;
   static int lastcount = 0;

   vector<unsigned long int> startaddr_ul_3;
   vector<unsigned long int> endaddr_ul_3;

   unsigned long int lastend = -1;
   for(int i = 0;i<n;i++)
   {
      unsigned long int s = startaddr_ul_2[i] & 0xfffffffffffc0000;
      unsigned long int e = endaddr_ul_2[i] & 0xfffffffffffc0000;

      if(startaddr_ul_3.size() == 0)
      {
         startaddr_ul_3.push_back(s);
      }
      else if(s != lastend)
      {
         endaddr_ul_3.push_back(lastend);
         startaddr_ul_3.push_back(s);
      }
      lastend = e;
   }
   endaddr_ul_3.push_back(lastend);

   n = startaddr_ul_3.size();

   for(int i = 0;i<n;i++)
   {
      long int s = startaddr_ul_3[i] & 0xfffffffffffc0000;
      long int e = endaddr_ul_3[i] & 0xfffffffffffc0000;

      count = abs(e - s);
      count = ceil((double)count/ptes);
      count += 1;

      //cout<<hex<<setfill('0')<<setw(13)<<startaddr_ul_2[i]<<" - "<<setfill('0')<<setw(13)<<endaddr_ul_2[i]<<"  ("<<dec<<count<<")"<<endl;
      //cout<<ceil(double(s-last_s)/(256*1024))<<" GB"<<endl;


      if((s|0x00fffffff) == 0x00fffffff)
      {
         count00 += count;
      }
      else if((s&0xff0000000) == 0xff0000000)
      {
         countff += count;
      }
      else if((s&0xff0000000) == 0x7f0000000)
      {
         count7f += count;
      }
      else if((s&0xff0000000) == 0x560000000)
      {
         count56 += count;
      }
      else if((s&0xff0000000) == 0x550000000)
      {
         count55 += count;
      }
      else
      {
         countot += count;
      }
      last_s = s;
   }
   
   count = count00 + count7f + count55 + count56 + countff + countot;
   if(lastcount != count)
   {
      cout<<fname<<" : "<<count<<","<<count00<<","<<count7f<<","<<count55<<","<<count56<<","<<countff<<","<<countot<<endl;
      lastcount = count;
   }
   if(count > maxcount)
   {
      total_vma_updates++;
      maxcount = count;
   }
   //  cout<<"total : "<<dec<<n<<endl;

   //  cout<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
   /*	  for(int i = 0;i<n;i++)
        {
        if(endaddr_ul_2[i] - startaddr_ul_2[i] <= 262143)
        {
        cout<<hex<<setfill('0')<<setw(13)<<startaddr_ul_2[i]<<" - "<<setfill('0')<<setw(13)<<endaddr_ul_2[i]<<endl;
        continue;
        }
        else
        {
        unsigned long int s = startaddr_ul_2[i];
        unsigned long int e = endaddr_ul_2[i];

        while(s<e)
        {
        cout<<hex<<setfill('0')<<setw(13)<<s<<" - ";

        unsigned long int ee;


        if(s+262143<=e)
        {
        ee = s+262143;
        }
        else
        {
        ee = e;
        }
        cout<<hex<<setfill('0')<<setw(13)<<ee<<endl;
        s += 262144;
        }
        }
        }
        cout<<"---------------------------------------------------------------------------"<<endl;*/
}


//string bms[47] = {"bwaves_r", "bwaves_s","blender_r", "cactuBSSN_r", "cactuBSSN_s", "cam4_r","deepsjeng_r", "deepsjeng_s", "exchange2_r", "exchange2_s", "fotonik3d_r", "fotonik3d_s", "imagick_r", "imagick_s", "lbm_r", "lbm_s", "leela_r", "leela_s", "mcf_r", "mcf_s", "nab_r", "nab_s", "namd_r", "omnetpp_r",  "omnetpp_s", "parest_r", "perlbench_r", "perlbench_s", "pop2_s", "povray_r", "roms_r", "roms_s", "wrf_r", "wrf_s", "x264_r", "x264_s", "xalancbmk_r", "xalancbmk_s", "xz_r", "xz_s", "canneal", "bc", "bfs", "cc", "graph500", "pr", "sssp"};

//string bms[66] = {"bwaves_r", "bwaves_s","blender_r", "cactuBSSN_r", "cactuBSSN_s", "cam4_r","deepsjeng_r", "deepsjeng_s", "exchange2_r", "exchange2_s", "fotonik3d_r", "fotonik3d_s", "imagick_r", "imagick_s", "lbm_r", "lbm_s", "leela_r", "leela_s", "mcf_r", "mcf_s", "nab_r", "nab_s", "namd_r", "omnetpp_r",  "omnetpp_s", "parest_r", "perlbench_r", "perlbench_s", "pop2_s", "povray_r", "roms_r", "roms_s", "wrf_r", "wrf_s", "x264_r", "x264_s", "xalancbmk_r", "xalancbmk_s", "xz_r", "xz_s", "canneal", "bc", "bfs", "cc", "graph500", "pr", "sssp","DA_master1", "DA_master2", "DA_slave01", "DA_slave02", "DA_slave11", "DA_slave12", "DCclient", "DCserver", "DSclient0", "DSclient1", "DSserver", "DSserverseed", "GraphAnalytics", "inmemory_analytics", "streamin_server_root", "str_server_wp1", "str_server_wp2", "str_server_wp3", "str_server_wp4" };
//string bms[25] = {"DA_master1", "DA_master2", "DA_slave01", "DA_slave02", "DA_slave11", "DA_slave12", "DCclient", "DCserver", "DSclient0", "DSclient1", "DSserver", "DSserverseed", "GraphAnalytics", "inmemory_analytics", "streamin_server_root", "str_server_wp1", "str_server_wp2", "str_server_wp3", "str_server_wp4" };

string bms[10] = {"pr_twitter", "cc_twitter","bc_twitter", "pr_kron", "cc_kron", "cc_urand","tc_urandU", "bfs_urand", "canneal1", "btree_1000_100"};
//string bms[1] = {"pr_twitter"};

int main(int argc, char** argv)
{
   int x = 1;
   struct stat buffer;

   for(int i=0;i<10;i++)
   {
      maxcount = 0;
      total_vma_updates = 0;
      string fname = bms[i] + "/" + "vma_" + to_string(x) +".txt";
      //cout<<fname<<endl;
      cout<<bms[i]<<endl;
      int x = 1;
      max_contig_reg = 0;
      max_contig_ps = 0;
      max_total_ps = 0; 
      max_mem_fp = 0;
      max_vmas_2mb = 0;

      max_vma_count = 0;

      vector<string> startaddr;
      vector<string> endaddr;

      startaddr_ul.clear();
      endaddr_ul.clear();

      while(stat(fname.c_str(), &buffer) == 0 )
      {
         //	cout<<"*************************************Analyzing file : "<<x<<endl;
         ifstream infile(fname);

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
            //	cout<<"lastend is "<<lastend<<" size"<<startaddr.size()<<" "<<endaddr.size()<<endl;

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
            else if((line[9] == '-') && (line[19] == ' ')) 
            {
               if(lastend == "")
               {
                  startaddr.push_back(line.substr(0,6));
                  insert_str(line.substr(0,6), 1);
               }
               else
               {
                  if(lastend.compare(line.substr(0,6)) != 0)
                  {
                     endaddr.push_back(lastend);
                     startaddr.push_back(line.substr(0,6));
                     insert_str(lastend, 0);
                     insert_str(line.substr(0,6), 1);
                  }
               }					
               lastend = line.substr(10,6);
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
               if(lastend == "")
               {
                  startaddr.push_back(line.substr(0,9));
                  insert_str(line.substr(0,9), 1);
               }
               else if(lastend.compare(line.substr(0,9)) != 0)
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
            /*	for(int i=0;i<startaddr.size() && i<endaddr.size() ; i++)
               {
               cout<<startaddr[i]<<"   "<<endaddr[i]<<"   "<<endl;
               }
               cout<<"+++++++++++++++++++++++++++++++++"<<endl;*/

         }
         endaddr.push_back(lastend);
         insert_str(lastend, 0);

         unsigned long totalmem = 0; // Total Virtual Memory Size
         unsigned long largest_contig_ps = 0; // Largest VMA region
         unsigned long s,e,numpages; // numpages = e - s -> Total number of PTEs
         unsigned long pte_pages = 0; //number of pages required to save PTEs = #PTEs*8/4096 = #PTEs/512
         vector<int> pte_pages_list; //stores list of different size PTE pages required
         for(int i=0;i<startaddr.size() && i<endaddr.size() ; i++)
         {
            int curvma_pte_pages = 0;
            //	cout<<startaddr[i]<<"   "<<endaddr[i]<<"   "<<endl;
            stringstream ss;
            string xyz = startaddr[i];
            ss.str(xyz);
            //	cout<<ss.str();
            ss>>hex>>s;
            //	cout<<"  start = "<<s<<endl;

            stringstream ss1;
            xyz = endaddr[i];
            ss1.str(xyz);
            //	cout<<ss1.str();
            ss1>>hex>>e;
            //	cout<<"  end = "<<e<<endl;

            numpages = e - s;
            //	cout<<"numpages = "<<numpages<<endl;
            totalmem += numpages; 
            curvma_pte_pages = ceil((double)numpages/512);
            pte_pages_list.push_back(curvma_pte_pages);
            pte_pages += curvma_pte_pages;
            //	cout<<"pte_pages = "<<pte_pages<<endl;

            increase_vmacounts(curvma_pte_pages);

            if(numpages > largest_contig_ps) largest_contig_ps = numpages;
            pages.push_back(numpages);
            //	cout<<ceil((double)numpages/512)<<"  "<<pagesize<<endl;
         }

         if(max_contig_reg < startaddr.size() ) max_contig_reg = startaddr.size();
         if(max_contig_ps < (largest_contig_ps/128)) max_contig_ps = (ceil((double)largest_contig_ps/512))*4;
         if(max_total_ps < (pte_pages*4)) max_total_ps = (pte_pages*4);
         if(max_mem_fp < totalmem*4)
         {
            max_pte_pages_list = pte_pages_list;
            max_mem_fp = totalmem * 4;
         }

         if(max_vma_count < (count - 3)) max_vma_count = count-3;

        // if(max_vmas_2mb < vmas_2mb)
         {
            max_vmas_2mb = vmas_2mb;
            startaddr_ul_2 = startaddr_ul;
            endaddr_ul_2 = endaddr_ul;
         }
         print_2mb(fname);

         //	cout<<vmas_2mb<<"  "<<vmas_4mb<<"  "<<vmas_8mb<<"  "<<vmas_16mb<<endl;

         //		cout<<startaddr.size()<<","<<(largest_contig_ps/128)<<","<<(pagesize*4)<<endl;

         /*	cout<<"Number of contigous regions : "<<startaddr.size()<<endl;
            cout<<"Largest contig PS = "<<(largest_contig_ps/128)<<" KB."<<endl;
            cout<<"Total PS = "<<(pagesize*4)<<"KB"<<endl;
            cout<<"-------------------------------------------------------------"<<endl;*/
         infile.close();
         x++;
         fname = bms[i] + "/" + "vma_" + to_string(x) +".txt";
      }
      /*		  cout<<bms[i]<<","<<max_vma_count<<","<<max_contig_reg<<","<<max_vmas_2mb<<","<<max_vmas_4mb<<","<<max_vmas_8mb<<","<<max_vmas_16mb<<","<<max_mem_fp<<","<<max_total_ps<<","<<max_contig_ps<<",sizes,";

              sort(max_pte_pages_list.begin(), max_pte_pages_list.end());
              for(auto i:max_pte_pages_list)
              {
              cout<<i<<",";
              }
              cout<<endl;*/
       //print_2mb();
         cout<<"total vma updates : "<<total_vma_updates<<endl;
   }


   cout<<"Max Bloat : "<<max_bloat*8<<"bytes."<<endl;
   cout<<"Avg Bloat : "<<avg_bloat*4/33<<"bytes."<<endl;

   return 1;
}


void increase_vmacounts(unsigned long pte_pages)
{
   // 	cout<<"called for "<<pte_pages<<"--"<<ceil((double)pte_pages/512)<<"--"<<ceil((double)pte_pages/1024)<<"--"<<ceil((double)pte_pages/2048)<<"--"<<ceil((double)pte_pages/4096)<<endl;
   vmas_2mb += ceil((double)pte_pages/512); 
}
