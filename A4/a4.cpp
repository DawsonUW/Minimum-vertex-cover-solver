#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <regex>
#include <minisat/core/SolverTypes.h>
#include <minisat/core/Solver.h>
#include <memory>


std::vector<int> findVC(int n, int k, std::vector<int> edges) {
    //input: n:number of vertices in graph;
    //      k:number of vertices in Vertex Cover
    //      edges:all vertices in  edges
    //output:vertices in vertex cover, if vertex cover exists
    //      {-1} ,if there is no vertex cover
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    Minisat::Lit matrix [n][k];

    //matrix initialization
    for (int i=0;i<n;i++)
        for (int j = 0; j < k; j++)
            matrix[i][j] = Minisat::mkLit(solver->newVar());



    // rule 1
    for (int i = 0; i < k; i++) {
        Minisat::vec<Minisat::Lit> literals;
        for (int j = 0; j < n; j++) {
            literals.push(matrix[j][i]);
        }
        solver->addClause(literals);
        literals.clear();
    }

    // rule 2
    for (int m = 0; m < n; m++)
        for (int p = 0; p < k; p++)
            for (int q = 0 ; q < k; q++)
                if (p<q)
                    solver->addClause(~matrix[m][p], ~matrix[m][q]);




    // rule 3
    for (int m = 0; m < k; m++)
        for (int p = 0; p < n ; p++)
            for (int q = 0; q < n; q++)
                if (p<q)
                    solver->addClause(~matrix[p][m], ~matrix[q][m]);
    if (edges.empty())
        return {-1};
    //rule 4
    while (!edges.empty()) {
        int j = edges.back();
        edges.pop_back();
        int i = edges.back();
        edges.pop_back();
        Minisat::vec <Minisat::Lit> literals;

        for (int a = 0; a < k; a++) {
            literals.push(matrix[j][a]);
            literals.push(matrix[i][a]);
        }
        solver->addClause(literals);
        literals.clear();
    }

    bool res = solver->solve();
//    std::cout << "the result is " << res << std::endl;

    if (res) {
        std::vector<int> result;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < k; j++) {
                if (solver->modelValue(matrix[i][j])==Minisat::l_True) {
//                    std::cout<<i<<std::endl;
                    result.push_back(i);
                }
            }
        }
        solver.reset(new Minisat::Solver());
        return result;


    }
    else {
        solver.reset(new Minisat::Solver());
        return {-1};
    }
//    solver.reset(new Minisat::Solver());


}
void PrintMinVC(int n,std::vector<int> edges){
    //input:n:number of vertices in graph;
    //        edges:all vertices in  edges
    //output: void, however print on the screen
    for (int k=1;k<=n;k++){
        std::vector<int> result=findVC(n,k,edges);
        if (result.front()!=-1){
            std::sort(result.begin(),result.end());
            for (auto i:result)
                std::cout<<i<<" ";
            std::cout<<std::endl;
            return;

        }
    }
    return;

}
int main() {
    int v;//number of vertices
    std::string line, temp;//line inputted from user
    std::vector<int> edges;

    while (!std::cin.eof()){//not read the EOF
 //part 1: parsing
        label1:
        getline(std::cin,line);

        if (line[0]=='V'&& !std::cin.eof()) {
            v = stoi(line.substr(2, line.size()));//subtract the number of vertices from the user's input
//            std::cout<<v<<std::endl;


            getline(std::cin,line);

            if (line[0] == 'E'&& !std::cin.eof()) {
                std::string temp;
                std::string pattern("\\<[0-9]+\\,[0-9]+\\>");//subtract the int pair with format: <1,2> firstly
                std::regex r(pattern);//regular expression
                for (std::sregex_iterator it(line.begin(), line.end(), r), end_it; it != end_it; ++it) {
                    temp = it->str();//find all int pairs
                    int x, y;
                    int flag = 1;

                    std::string p("[0-9]+");//subtract an int from an int pair
                    std::regex re(p);
                    // cout<<"-----------"<<endl;
                    for (std::sregex_iterator it2(temp.begin(), temp.end(), re), end_it2; it2 != end_it2; ++it2) {
                        x = stoi(it2->str());
                        if (flag == 1) {
                            y = x;//y is the first number of an int pair, x is the second number of an int pair
                            flag = 0;
                        }
                    }
//                    std::cout<<y<<","<<x<<std::endl;
                    if (y>=v || x>=v)
                    {
                        std::cout << "Error: Vertex does not exist. " << std::endl;
                        goto label1;

                    }
                    else {

                        edges.push_back(y);//add new a new element at the end of vector array
                        edges.push_back(x);//because of adding edge to undirected graph

                    }
                }
            }
            //part2:calculate and output Vertex Cover
            PrintMinVC(v,edges);
            edges.clear();

        }
    }
    return 0;
}


//V 5
//E {<0,4>,<4,1>,<0,3>,<3,4>,<3,2>,<1,3>}
//V 7
//E {<0,1>,<0,2>,<1,3>,<3,4>,<4,5>,<5,6>}
//V 6
//E {<0,1>,<0,2>,<1,3>,<1,2>,<4,2>,<5,4>,<2,3>}
//V 4
//E {<0,1>}
//output
//3 4
//0 3 5
//1 2 4
//1
