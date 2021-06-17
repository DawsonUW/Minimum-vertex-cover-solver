#include <unistd.h>
#include <stdlib.h>
#include <iostream>
//#include <iostream>
//#include <unistd.h>
#include <fstream>
#include <vector>
#include <string>
//#include <queue>
//#include "rand_gen.cpp"

unsigned int street_rand_num(unsigned int k=10) {
    std::ifstream urandom("/dev/urandom");
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    char ch = 'a';
    urandom.read(&ch, 1);
//    std::cout<<(unsigned int)ch<<std::endl;
    unsigned int rand = (unsigned int) ch;
//    rand=99;
    rand = rand % (k - 2 + 1) + 2;
    urandom.close();
    return rand;
}
unsigned int line_seg_rand_num(unsigned int k=5) {
//    std::cout<<k<<std::endl;
    std::ifstream urandom("/dev/urandom");
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    char ch = 'a';
    urandom.read(&ch, 1);
//    std::cout<<(unsigned int)ch<<std::endl;
    unsigned int rand = (unsigned int) ch;
//    rand=99;
    rand = rand % (k - 1 + 1) + 1;
//    std::cout<<rand<<std::endl;
    urandom.close();
    return rand;
}
int coordinate_rand_num(unsigned int k=20){
//    std::cout<<k<<std::endl;
    std::ifstream urandom("/dev/urandom");
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    char ch = 'a';
    urandom.read(&ch, 1);
//    std::cout<<(unsigned int)ch<<std::endl;
    unsigned int rand = (unsigned int) ch;
//    rand=99;
//    std::cout<<rand<<std::endl;
    int rand2 = (rand % (k - (-k) + 1)) + (-k);
    urandom.close();
//    std::cout<<rand<<std::endl;
//    std::cout<<rand2<<std::endl;
    return rand2;
}

unsigned int wait_rand_num(unsigned int k=5) {
    std::ifstream urandom("/dev/urandom");
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    char ch = 'a';
    urandom.read(&ch, 1);
//    std::cout<<(unsigned int)ch<<std::endl;
    unsigned int rand = (unsigned int) ch;
//    rand=99;
    rand = rand % (k - 5 + 1) + 5;
    urandom.close();
    return rand;
}
void create_street_name( std::vector <std::string> &street_name, int street_rand_num)
{
//    std::string name="A";
    std::cout<<street_rand_num<<std::endl;
    for (int i=1;i<=street_rand_num;++i)
    {
        std::cout<<'A'+std::to_string(i)+'B'<<std::endl;
        street_name.push_back('A'+std::to_string(i)+'B');
    }
//    std::cout<< *street_name<<std::endl;
}
bool exsit_0_len_seg(std::vector<std::string> segs)
{
    int segs_size=segs.size();
    for (int i=0;i<segs_size-1;++i){
//        std::cout<<segs[i]<<" "<<segs[i+1]<<std::endl;

        if (segs[i]==segs[i+1]){
            return true;
        }
    }
    return false;
}


int main (int argc, char **argv)
{
    std::string s_arg,n_arg,l_arg,c_arg;
    unsigned int s_arg_int=10, n_arg_int=5,l_arg_int=5,c_arg_int=20;
    std::vector<std::string>  seg;//example:(1,2) (3,4)
    int c,error=0;

//    std::queue<std::string> que_vertex,que_vertex_tot;
    while ((c=getopt(argc,argv,"s:n:l:c:"))!=-1)//  s ,n ,l, c all take a required argument
        switch(c)
        {
            case 's'://street number
                s_arg=optarg;
                s_arg_int=std::atoi(s_arg.c_str());
//                std::cout<<s_arg_int<<std::endl;

//                std::cout<<s_arg;
                break;
            case 'n'://line segments number
                n_arg=optarg;
                n_arg_int=std::atoi(n_arg.c_str());
                break;
            case 'l':
                l_arg=optarg;
                l_arg_int=std::atoi(l_arg.c_str());
                break;
            case 'c'://coordinate
                c_arg=optarg;
                c_arg_int=std::atoi(c_arg.c_str());
                break;
        }
    while (true)
    {
        //generate street names
//        create_street_name(street_name,street_rand_num(s_arg_int));
        unsigned int street_number=street_rand_num(s_arg_int);


        for (int i=1;i<=street_number;++i) {//for ALL streets
            seg.clear();
            std::string s,s2;

            int j=1;

            //
            int coordinate_x = coordinate_rand_num(c_arg_int);
            int coordinate_y = coordinate_rand_num(c_arg_int);
            s = "(" + std::to_string(coordinate_x) + "," + std::to_string(coordinate_y) + ")";
//            std::cout<<s<<std::endl;
//            std::cout<<"*************"<<std::endl;
            seg.push_back(s);
            j++;


            while (j <= line_seg_rand_num(n_arg_int) + 1) {//          for ONE street
                //generate ONE seg


                gen_a_vertex:
                int coordinate_x2 = coordinate_rand_num(c_arg_int);
                int coordinate_y2 = coordinate_rand_num(c_arg_int);
                s2 = "(" + std::to_string(coordinate_x2) + "," + std::to_string(coordinate_y2) + ")";
//                std::cout<<s2<<std::endl;
//                std::cout<<"$$$$$$"<<std::endl;
                if (s==s2){ //zero len_seg?

                    error++;
//                    std::cout<<"error:"<<error<<std::endl;
                    if (error==25)
                    {
                        std::cerr<<"Error: failed to generate valid input for 25 simultaneous attempts."<<std::endl;
                        exit(1);
                    }
                    else{
                        s=s2;
//                        goto gen_a_vertex;
                    }
                }
                else{
                    // overlap with previous all segs?
                    seg.push_back(s2);
                    s=s2;
                    j++;


                }




            }//generate ONE seg
            s = "a \"A";
            s += std::to_string(i);
            s += "B\"";
            std::cout << s;
            int seg_size = seg.size();
            for (int k = 0; k < seg_size; k++) {
                std::cout << " " << seg[k];
            }
            std::cout << std::endl;

        }//for ALL streets

        std::cout << "g" << std::endl;
        sleep(wait_rand_num(l_arg_int));
        for (int k = 1; k <= street_number; ++k) {
            std::string s2;
            s2 = "r \"A";
            s2 += std::to_string(k);
            s2 += "B\"";
            std::cout << s2 << std::endl;
        }

    }

}
