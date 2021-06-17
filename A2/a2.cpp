#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <regex>
#include <climits>
using namespace std;
void add_edge(vector<int> adj[], int src, int dest)
{
    adj[src].push_back(dest);
    adj[dest].push_back(src);
}

bool BFS(vector<int> adj[],int src, int dest,int v, int pred[],int dist[])
{
    list<int> queue;
    bool visited[v];
    for (int i=0; i<v;i++)
    {
        visited[i]=false;
        dist[i]=INT_MAX;
        pred[i]=-1;
    }
    visited[src]=true;
    dist[src]=0;
    queue.push_back(src);

    while(!queue.empty())
    {
        int u=queue.front();
        queue.pop_front();
        for (int i=0;i<adj[u].size();i++)
        {
            if (visited[adj[u][i]]==false)
            {
                visited[adj[u][i]]=true;
                dist[adj[u][i]]=dist[u]+1;
                pred[adj[u][i]]=u;
                queue.push_back(adj[u][i]);
                if (adj[u][i]==dest)
                    return true;
            }
        }
    }
    return false;
}
void print_shortest_distance(vector<int> adj[],int s, int dest, int v)
{
    int pred[v],dist[v];
    if (BFS(adj,s,dest,v,pred,dist)==false)
    {
        cout<<"Error: No path between source and destination. Enter 'V command' first, please."<<endl;
        return;
    }
    vector<int> path;
    int crawl=dest;
    path.push_back(crawl);
    while(pred[crawl]!=-1)
    {
        path.push_back(pred[crawl]);
        crawl=pred[crawl];
    }
//    cout<<"Path: "<<endl;
    for (int i=path.size()-1;i>0;i--)
        cout<<path[i]<<"-";
    cout<<path[0]<<endl;
}
int main() {
//    int v=15;
//    vector<int> adj[v];
//    add_edge(adj, 2,6);
//    add_edge(adj, 2,8);
//    add_edge(adj, 2,5);
//    add_edge(adj,6,5);
//    add_edge(adj,5,8);
//    add_edge(adj,6,10);
//    add_edge(adj,10,8);
//    int src=2, dest=10;
//    print_shortest_distance(adj,src,dest,v);
    int v;
    string line, temp;

    while (!cin.eof())
    {
        label_1:
        getline(cin,line);
        if (line[0]=='V') {
            v = stoi(line.substr(2, line.size()));
//          cout<<v<<endl;
            vector<int> adj[v];
            getline(cin,line);
            if (line[0] == 'E') {
                string temp;
                string pattern("\\<[0-9]+\\,[0-9]+\\>");
                regex r(pattern);
                for (sregex_iterator it(line.begin(), line.end(), r), end_it; it != end_it; ++it) {
                    temp = it->str();
                    int x, y;
                    int flag = 1;

                    string p("[0-9]+");
                    regex re(p);
                    // cout<<"-----------"<<endl;
                    for (sregex_iterator it2(temp.begin(), temp.end(), re), end_it2; it2 != end_it2; ++it2) {
                        x = stoi(it2->str());
                        if (flag == 1) {
                            y = x;
                            flag = 0;
                        }
                    }
                    if (y>=v || x>=v) {
                        cout << "Error: Edge does not exist. Enter 'V command' first, please. " << endl;
                        goto label_1;
                    }
                    else
                        add_edge(adj, y, x);
                }
                getline(cin,line);
                if (line[0]=='s')
                {
                        int x, y;
                        int flag = 1;

                        string p("[0-9]+");
                        regex re(p);
                        // cout<<"-----------"<<endl;
                        for (sregex_iterator it2(line.begin(), line.end(), re), end_it2; it2 != end_it2; ++it2) {
                            x = stoi(it2->str());
                            if (flag == 1) {
                                y = x;
                                flag = 0;
                            }
                        }
                        if (y>=v || x>=v)
                        {
                            cout << "Error: Vertex does not exist. Enter 'V command' first, please. " << endl;
                            goto label_1;
                        }
                        else
                            print_shortest_distance(adj,y,x,v);
                }


            }
        }
//    int v=5;

//    add_edge(adj,0,2);
//    add_edge(adj,2,1);
//    add_edge(adj,2,3);
//    add_edge(adj,3,4);
//    add_edge(adj,4,1);
//        int src = 4, dest = 0;
//        print_shortest_distance(adj, src, dest, v);

    }
    return 0;
}

//V 15
//E {<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>}
//s 2 10
//V 5
//E {<0,2>,<2,1>,<2,3>,<3,4>,<4,1>}
//s 4 0
