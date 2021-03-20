#include <bits/stdc++.h>
#include "hj.h"
using namespace std;


vector<int> sizes1;
vector<int> sizes2;
int col_index =1;
int curr_scratch_file = 0;
int r_files = 0;
int s_files = 0;
int RPB = 100;
int RPS =  500;
class Tuple 
{
    public:
        string t;
        int index;
        Tuple(string t,int in): t(t),index(in){}
};

struct CompareTuples {
    bool operator()(Tuple const& p1, Tuple const& p2)
    {
        vector<string> elem1;
        vector<string> elem2;
        string elem1t = p1.t;
        string elem2t = p2.t;
        for(int i=0;i<sizes1.size();i++)
        {
            string token = elem1t.substr(0,sizes1[i]);
            elem1.push_back(token);
            elem1t.erase(0,  sizes1[i]+2);
        }

        for(int i=0;i<sizes1.size();i++)
        {
            string token = elem2t.substr(0,sizes1[i]);
            elem2.push_back(token);
            elem2t.erase(0,  sizes1[i]+2);
        }
        //cout<<elem1[1]<<" < "<<elem2[1]<<" false "<<" \n";

        return elem1[1] > elem2[1];
        
    }
};

struct CompareTuples2 {
    bool operator()(Tuple const& p1, Tuple const& p2)
    {
        vector<string> elem1;
        vector<string> elem2;
        string elem1t = p1.t;
        string elem2t = p2.t;
        for(int i=0;i<sizes2.size();i++)
        {
            string token = elem1t.substr(0,sizes2[i]);
            elem1.push_back(token);
            elem1t.erase(0,  sizes2[i]+2);
        }

        for(int i=0;i<sizes2.size();i++)
        {
            string token = elem2t.substr(0,sizes2[i]);
            elem2.push_back(token);
            elem2t.erase(0,  sizes2[i]+2);
        }
        //cout<<elem1[1]<<" < "<<elem2[1]<<" false "<<" \n";

        return elem1[0] > elem2[0];
        
    }
};


bool sortcol( const vector<string>& v1,const vector<string>& v2 )
{ 
    return v1[col_index] < v2[col_index];
    return true;
} 



void create_initial_runs( char* input_file, int run_size,int r_s) 
{ 

    fstream in;
    in.open(input_file,ios::in);
    bool moreinput = true;
    vector<int> s;
    if(r_s == 0){s = sizes1;col_index = 1;}
    else{s = sizes2;col_index = 0;}
    string line;
    int i;
    //std::vector<std::thread> th(no_of_threads);
    while(moreinput)
    {
        vector<vector<string>> tuples;

        for(int i=0;i<run_size;i++)
        {
        
        getline(in, line);
        //cout<<" line "<<line<<"\n";
        if(line.size()==0){moreinput=false;}//TODO

        //get all strings in a line
        vector<string> temp;
        for(int i=0;i<s.size();i++)//sizes2
        {
            string token = line.substr(0,s[i]);
            temp.push_back(token);
            line.erase(0,  s[i]+2);//here also sizes2
        }
        tuples.push_back(temp);

        }
        
        sort(tuples.begin(),tuples.end(),sortcol);

        
        fstream t;
        string append;
        if(r_s == 0){append = "r_";}
        else{append = "s_";}
        t.open((append + to_string(curr_scratch_file)),ios::out);

        for(int i=0;i<tuples.size();i++)
        {
            string  addend= "";
            for(int j=0;j<tuples[0].size();j++)
            {
                addend  = addend + tuples[i][j]+"  ";//convert to string and remove last 2 chars
            }
            
            string n_a= addend.substr(0,addend.size()-2)+"\n";
            if(n_a != "  \n"){t<<n_a;}
            
        }
        t.close();
        curr_scratch_file++;
         
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

void fill_priority_queue_and_merge(char* output_file)
{
    priority_queue<Tuple, vector<Tuple>, CompareTuples> QR;
    priority_queue<Tuple, vector<Tuple>, CompareTuples2> QS;
    vector<ifstream> in_r;
    vector<ifstream> in_s;
    vector<int> taken1(r_files,0);
    vector<int> taken2(s_files,0);
    // ofstream out;
    // out.open(output_file,std::ios_base::app);

    // FILE* in_r[r_files]; 

    for (int i = 0; i <r_files; i++)
    { 
        string fileName = "r_"+to_string(i);
        in_r.push_back(ifstream(fileName));
    } 
    // FILE* in_s[s_files]; 
    for (int i = 0; i <s_files; i++)
    { 
        // char fileName[10]; 
        string fileName = "s_"+to_string(i);
        in_s.push_back(ifstream(fileName));
    } 



    // FINAL OUTPUT FILE 
    FILE* out = openFile(output_file, "wa"); 
    
    int i; 
    // string s;
    string line;
    
    for (i = 0;i<r_files; i++)
    { 
        
        for(int z=0;z<RPB;z++)//todo
        {
            getline(in_r[i], line);
            
            if(!line.empty())
            {
               // cout<<"pushing  "<<line<<" into QR "<<"\n";
                Tuple* tt = new Tuple(line,i);
                QR.push(*tt); 
            }
            else
            {
                //r_files--;
                break;
            }
        }
    } 

    //cout<<"finished with r";
    int j;
    // string t;
    string line2;
    for (j = 0; j<s_files; j++)
    { 
        for(int z=0;z<RPB;z++)
        {
            getline(in_s[j], line2);
            
            //if(fgets(line2, sizeof(line2), in_s[i])!=NULL)
            if(!line2.empty())
            {
                //cout<<"my data is "<<line2<<"\n";
                //cout<<"pushing  "<<line2<<" into QS "<<"\n";
                Tuple* tt = new Tuple(line2,j);
                QS.push(*tt); 
            }
            else
            {
                //s_files--;
                //cout<<"i am empty"<<"\n";
                break;
            }
        }


    } 
    
    // cout<<"size of QR is "<<QR.size()<<"\n";
    // cout<<"size of QS is "<<QS.size()<<"\n";

    while(!QR.empty() && !QS.empty())
    {
        vector<string> elem1;
        vector<string> elem2;
        
        
        string elem1t = QR.top().t;
        //cout<<"first is "<<elem1t<<"\n";
        
        string elem2t = QS.top().t;
        //cout<<"second is "<<elem2t<<"\n";

        if(elem1t == "" ){QR.pop();continue;}
        if( elem2t == ""){QS.pop();continue;}
        
        for(int i=0;i<sizes1.size();i++)
        {
            string token = elem1t.substr(0,sizes1[i]);
            elem1.push_back(token);
            elem1t.erase(0,  sizes1[i]+2);
        }

        for(int i=0;i<sizes2.size();i++)
        {
            string token = elem2t.substr(0,sizes2[i]);
            elem2.push_back(token);
            elem2t.erase(0,  sizes2[i]+2);
        }
        // Tuple r_temp =  QR.top();
        // Tuple s_temp = QS.top();
        // cout<<"first string = "<<r_temp.t<<"\n";
        // cout<<"second string = "<<s_temp.t<<"\n";


        if(elem1[1] > elem2[0])
        {
            int fn = QS.top().index;
            //taken2[fn]++;
            //cout<<"first is greater\n";
            QS.pop();
            string ml;
            getline(in_s[fn], ml);

            //cout<<"pushing "<<ml<<"into QS"; 
            Tuple* tt = new Tuple(ml,fn);
            QS.push(*tt); 
            //s_temp = QS.top();
            
        }
        else if(elem1[1] < elem2[0])
        {
            int fn = QR.top().index;
            //taken1[fn]++;
            QR.pop();
            string ml;
            getline(in_r[fn], ml);

            //cout<<"pushing "<<ml<<"into QR"; 
            Tuple* tt = new Tuple(ml,fn);
            QR.push(*tt);

            //cout<<"second is greter\n";
            //append fron ==
                       
           // r_temp = QR.top();
        }
        else if(elem1[1] == elem2[0])
        {
            //cout<<"both are equal"<<"\n";
            vector<Tuple> rv;
            vector<Tuple> sv;

            string match = elem1[1];

            bool con1=true;

            while(!QR.empty() && con1)
            {
                string left_temp  = QR.top().t;
                vector<string> elem;
                for(int i=0;i<sizes1.size();i++)
                {
                    string token = left_temp.substr(0,sizes1[i]);
                    elem.push_back(token);
                    left_temp.erase(0,sizes1[i]+2);
                }
                string left_match = elem[1];

                if(left_match == match)
                {
                    string toput = QR.top().t+"\n";
                    //fputs(toput.c_str(),out);
                    //cout<<QR.top().t<<"\n";
                    int filenum = QR.top().index;
                    taken1[filenum]++;
                    rv.push_back(QR.top());
                    QR.pop();

                    if(taken1[filenum]>=RPB)
                    {   
                        for(int i=0;i<RPB;i++)
                        {
                            string l;
                            getline(in_r[filenum], l);
                                
                            if(!l.empty())
                            {
                                //cout<<"getting "<<l<<" from "<<filenum<<"\n"; 
                                //cout<<"my data is "<<line<<"\n";
                                Tuple* tt = new Tuple(l,filenum);
                                QR.push(*tt); 
                            }
                            else
                            {
                                //filenum++;
                                //r_files--;
                                //cout<<"i am empty"<<"\n";
                                break;
                            }

                        }
                        taken1[filenum]=0;
                    }

                }
                else
                {
                    con1 = false;
                }
            }
            bool con2 = true;
            while(!QS.empty() && con2)
            {
                string right_temp  = QS.top().t;
                vector<string> elem;
                for(int i=0;i<sizes2.size();i++)
                {
                    string token = right_temp.substr(0,sizes2[i]);
                    elem.push_back(token);
                    right_temp.erase(0,sizes2[i]+2);
                }
                
                string right_match = elem[0];

                if(right_match == match)
                {
                    string toput = QS.top().t+"\n";
                    //fputs(toput.c_str(),out);                    
                    //cout<<QS.top().t<<"\n";
                    int filenum = QS.top().index;
                    taken2[filenum]++;
                    sv.push_back(QS.top());
                    QS.pop();
                    if(taken2[filenum]>=RPB)
                    {   
                        //cout<<"file "<<filenum<<" has completed";
                        for(int i=0;i<RPB;i++)
                        {
                            string l;
                            getline(in_s[filenum], l);
                            
                            if(!l.empty())
                            {
                                //cout<<"getting "<<l<<" from "<<filenum<<"\n"; 
                                //cout<<"my data is "<<line<<"\n";
                                Tuple* tt = new Tuple(l,filenum);
                                QS.push(*tt); 
                            }
                            else
                            {
                                //filenum++;
                                //s_files--;
                                //cout<<"i am empty"<<"\n";
                                break;
                            }

                        }
                        taken2[filenum]=0;
                    }

                }
                else{con2 = false;}
            }
            for (int i=0;i<rv.size();i++)
            {
                string one = rv[i].t;
                //one = one + "  ";
                //fputs((one+"\n").c_str(),out);
                for(int j=0;j<sv.size();j++)
                {
                    string two = sv[j].t;
                    vector<string> temp;
                    for(int s=0;s<sizes2.size();s++)
                    {
                        string token = two.substr(0,sizes2[s]);
                        temp.push_back(token);
                        two.erase(0,sizes2[i]+2);
                    }
                    // string full = ;
                    string full = one+"  "+temp[1]+"\n";
                    //cout<<"full is "<<full<<"\n";
                    fputs((full).c_str(),out);
                }
            }
            // cout<<"size of rv is "<<rv.size()<<"\n";
            // cout<<"size of sv is "<<sv.size()<<"\n";

        }

    }

    // for (int i = 0; i <r_files; i++){fclose(in_r[i]);}
    // for (int i = 0; i <s_files; i++){fclose(in_s[i]);}
    // fclose(out);  

}


int main(int argc, char **argv)
{
    char* ifr = argv[1];
    char* ifs = argv[2];
    string p1 = ifr;
    string p2 = ifs;
    int B     = stoi(argv[4]);
    
    fstream in1;
    in1.open(ifr,ios::in);
    string f1l;
    getline(in1,f1l);
    int ind1 = f1l.find_first_of(" ");
    
    sizes1.push_back(ind1);
    f1l.erase(0,  ind1+2);
    
    int ind12 = f1l.size();
    sizes1.push_back(ind12);

    in1.close();


    FILE *p_file = NULL;
    p_file = fopen(ifr,"rb");
    fseek(p_file,0,SEEK_END);
    int size = ftell(p_file);
    cout<<"size is "<<size<<"\n";
    fclose(p_file);

    int tl = sizes1[0]+sizes1[1]+3;//make 2
    int tt1 = ceil(size/tl);

    RPS = 100*B;




    
    fstream in2;
    in2.open(ifs,ios::in);
    string f2l;
    getline(in2,f2l);
    int ind2 = f2l.find_first_of(" ");
    sizes2.push_back(ind2);
    
    f2l.erase(0,  ind2+2);
    int ind22 = f2l.size();
    sizes2.push_back(ind22);
    in2.close();
    

    FILE *p_file2 = NULL;
    p_file2 = fopen(ifs,"rb");
    fseek(p_file2,0,SEEK_END);
    int size2 = ftell(p_file2);
    cout<<"size is "<<size2<<"\n";
    fclose(p_file2);

    int tl2 = sizes2[0]+sizes2[1]+3;//make 2
    int tt2 = ceil(size2/tl2);

    if(tt1+tt2<(10000*B*B))
    {

    }
    else
    {
        cout<<"Cannot perform operation "<<"\n";
        return 0;
    }
    

    
    

    create_initial_runs(ifr,RPS,0) ;
    r_files = curr_scratch_file;
    curr_scratch_file = 0;
    create_initial_runs(ifs,RPS,1) ;
    s_files = curr_scratch_file;


    char* s_h = argv[3];
    string ffn = p1+"_"+p2+"_join.txt";
    

    char op_file[ffn.size() + 1];
    strcpy(op_file, ffn.c_str());
    fill_priority_queue_and_merge(op_file);

    //fill_priority_queue();
    return 0;
}