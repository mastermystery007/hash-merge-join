#include <bits/stdc++.h>
#include <functional> 
using namespace std;


vector<int> sizes1;
vector<int> sizes2;
hash<string> hasher;
int col_index =1;
int NB = 20;
int curr_scratch_file = 0;
int r_files = 0;
int s_files = 0;
int num_lists = 20;
class Tuple 
{
    public:
        string t;
        int index;
        Tuple(string t,int in): t(t),index(in){}
};


bool sortcol( const vector<string>& v1,const vector<string>& v2 )
{ 
    return v1[col_index] < v2[col_index];
    return true;
} 



void create_initial_runs( char* input_file, int run_size,int r_s) 
{ 

    fstream in;
    // vector<ifstream> in_r;
    // vector<ifstream> in_s;

    vector<ofstream> out_r;
    vector<ofstream> out_s;

    in.open(input_file,ios::in);

    bool moreinput = true;

    vector<int> s;
    if(r_s == 0){s = sizes1;col_index = 1;}
    else{s = sizes2;col_index = 0;}

    string line;
    int i;
    //std::vector<std::thread> th(no_of_threads);
    
    string append;
    if(r_s == 0){append = "r_";}
    else{append = "s_";}



    while(moreinput)
    {
        vector<vector<string>> tuples(20);

        for(int i=0;i<run_size;i++)
        {
        
            getline(in, line);
            string al = line;
            
            if(line.size()==0){moreinput=false;}

            //get all strings in a line
            vector<string> temp;
            for(int i=0;i<s.size();i++)//sizes2
            {
                string token = line.substr(0,s[i]);
                temp.push_back(token);
                line.erase(0,  s[i]+2);//here also sizes2
            }
            // tuples.push_back(temp);
            // //get hash value on index based on rs
            int h = hasher(temp[col_index])%(NB);

            //cout<<"hash value is "<<h;

            tuples[h].push_back(al);

        }

        for(int j=0;j<tuples.size();j++)
        {
            //cout<<"size is "<<tuples[j].size()<<"\n";
            if(tuples[j].size()!=0)
            {
                string fname ;
                ofstream t;
                fname = append + to_string(j);
                t.open((fname),std::ios_base::app);

                for(int k=0;k<tuples[j].size();k++)
                {                    
                    t << tuples[j][k]<<"\n";
                    
                }
                t.close();

            }

        }

         
    }
} 
inline bool file_exists (const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}

void merge_hashed_values(char* output_file)
{
    vector<ifstream> in_r;
    vector<ifstream> in_s;
    ofstream out;

    out.open(output_file,std::ios_base::app);

    for(int i=0;i<20;i++)
    {
        if(file_exists("r_"+to_string(i)) && file_exists("s_"+to_string(i)))
        {
            ifstream ri = ifstream("r_"+to_string(i));
            ifstream si = ifstream("s_"+to_string(i));
            vector<string> join_r;
            vector<string> join_s;
            vector<vector<string>> line_r;
            vector<vector<string>> line_s;
            string liner;
            string lines;
            
            
            
            bool moreinput = true;
            while(moreinput)
            {
                getline(ri, liner);
                if(liner.size()==0){moreinput=false;}

                //get all strings in a line
                vector<string> temp;
                for(int i=0;i<sizes1.size();i++)//sizes2
                {
                    string token = liner.substr(0,sizes1[i]);
                    temp.push_back(token);
                    liner.erase(0,  sizes1[i]+2);//here also sizes2
                }
                join_r.push_back(temp[1]);
                line_r.push_back(temp);
                
            }
            moreinput = true;
            while(moreinput)
            {
                getline(si, lines);
                if(lines.size()==0){moreinput=false;}

                //get all strings in a line
                vector<string> temp;
                for(int i=0;i<sizes2.size();i++)//sizes2
                {
                    string token = lines.substr(0,sizes2[i]);
                    temp.push_back(token);
                    lines.erase(0,  sizes2[i]+2);//here also sizes2
                }
                join_s.push_back(temp[0]);
                line_s.push_back(temp);
                
            }


            for(int i=0;i<join_r.size();i++)
            {
                for(int j=0;j<join_s.size();j++)
                {
                    if(join_r[i]==join_s[j])
                    {   
                        if(join_r[i]!="")
                        {
                            string line = line_r[i][0]+"  "+line_r[i][1]+"  "+line_s[j][1];
                            cout<<line<<"\n";
                            out<<line_r[i][0]<<"  "<<line_r[i][1]<<"  "<<line_s[j][1]<<"\n";
                        }
                        
                    }
                }
            }
        }
    }

}

FILE* openFile(char* fileName, char* mode) 
{ 
    FILE* fp = fopen(fileName, mode); 
    if (fp == NULL)
    { 
        perror("Error while opening the file.\n"); 
        exit(EXIT_FAILURE); 
    } 
    return fp; 
} 

string get_string(int index,int flag,string str)
{
    int i;
    vector<int> s;
    string ms = str;
    vector<string> elem;

    if(flag == 0){s = sizes1;}
    else if(flag == 1){s = sizes2;}    
    for(int i=0;i<s.size();i++)
        {
            string token = ms.substr(0,s[i]);
            elem.push_back(token);
            ms.erase(0,  s[i]+2);
        }
    return elem[index];
}



    // for (int i = 0; i <r_files; i++){fclose(in_r[i]);}
    // for (int i = 0; i <s_files; i++){fclose(in_s[i]);}
    // fclose(out);  




int main()
{
    char* ifr = "inputR";
    char* ifs = "inputS";

    sizes1.push_back(5);
    sizes1.push_back(7);

    sizes2.push_back(7);
    sizes2.push_back(3);

    create_initial_runs(ifr, 100,0) ;
    r_files = curr_scratch_file;
    curr_scratch_file = 0;
    create_initial_runs(ifs, 100,1) ;
    s_files = curr_scratch_file;

    
    //fill_priority_queue();
    merge_hashed_values("out2");
    return 0;
}