#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <chrono>

using namespace std;
int ** graph;
float ** weight;
float **pheromones;
float **leave;
float alpha,beta,evaporation_l,intensity,evaporation_g;

class D
{
public:
    int number;
    int ants;
    D();
};
D::D()
{
    number=100;
    ants=40;
}

class Anthill:D
{
public:
    int all_cost;
    int start;
    int prev;
    int cur;
    int *visited;
    int *path;
    Anthill();
};
Anthill::Anthill()
{
    all_cost=0;
    start=-1;
    prev=-1;
    cur=start;
    visited=new int[number];
    for(int i=0;i<number;i++)
        visited[i]=0;
    path=new int[1000000];
    for(int i=0;i<1000000;i++)
        path[i]=-1;
}

class Solution
{
public:
    int path_cost;
    int *path;
    Solution();
};
Solution::Solution()
{
    path_cost=0;
    path=new int[1000000];
    for(int i=0;i<1000000;i++)
        path[i]=-1;
}

void make_graph(int number,int *degree)
{
    int i,j,next,w;

    graph=new int*[number];
    for(i=0;i<number;i++)
        graph[i]=new int[number];
    for(i=0;i<number;i++)
    {
        for(j=0;j<number;j++)
            graph[i][j]=0;
    }

    int which[number];
    for(i=0;i<number;i++)
        which[i]=0;

    for(i=0;i<number;i++)
    {
        w =(rand()%6)+1;
        degree[i]=w;
        for(j=0;j<number;j++)
        {
            next=(rand()%number)+0;
            if(graph[i][next]==0 && which[i]<degree[i] && which[next]<degree[next] && next!=i)
            {
                graph[i][next]=1;
                graph[next][i]=1;
                which[i]++;
                which[next]++;
            }
        }
    }
}

void where_next(int number,int beginning,int *allowed)
{
    int i;
    int n=0;
    for(i=0;i<number;i++)
        allowed[i]=-1;
    for(i=0;i<number;i++)
    {
        if(graph[beginning][i])
        {
            allowed[n]=i;
            n++;
        }
    }
}

void calc_probability(int here,int *allowed,float *probability,int number)
{
    int i,j,next,other;
    float a,b;
    for(i=0;i<number;i++)
        probability[i]=-1;

    for(i=0;i<6;i++)
    {
        if(allowed[i]>-1)
        {
            a=b=0.0;
            next=allowed[i];
            a=pow(pheromones[here][next],alpha)*pow(1/weight[here][next],beta);
            for(j=0;j<6;j++)
            {
                if(allowed[j]>-1)
                {
                    other=allowed[j];
                    b+=pow(pheromones[here][other],alpha)*pow(1/weight[here][other],beta);
                }
            }
            probability[i]=a/b;
        }
    }
}

void local_update(int number,int *path,int cost,int length)
{
    for(int i=1;i+1<length;i++)
        pheromones[path[i]][path[i+1]]=pheromones[path[i]][path[i+1]]*(1-evaporation_l)+evaporation_l/(number*cost);
}

void sum(int length,int number,int *path,int cost)
{
    int result=0;

    for(int i=0;i<number;i++)
    {
        for(int j=0;j<number;j++)
        {
            leave[i][j]=0.0;
        }
    }

    for(int i=0;i+1<length;i++)
    {
        leave[path[i]][path[i+1]]=intensity/cost;
        leave[path[i+1]][path[i]]=intensity/cost;
    }
}

void leave_pheromones(int number)
{
    for(int i=0;i<number;i++)
    {
        for(int j=0;j<number;j++)
        {
            pheromones[i][j]=(1-evaporation_g)*pheromones[i][j]+leave[i][j];
            pheromones[j][i]=(1-evaporation_g)*pheromones[j][i]+leave[j][i];
        }
    }
}

int main()
{
    srand(time(NULL));
    D aco;
    alpha=0.2;
    beta=0.5;
    intensity=1.0;
    evaporation_l=0.5;
    evaporation_g=0.5;
    int i,j,iter;

    int degree[aco.number];
    for(i=0;i<aco.number;i++)
        degree[i]=0;

    int consistent=iter=0;
    do
    {
        if(consistent==0 && iter>0)
        {
            for(i = 0; i < aco.number; i++) delete [ ] graph [ i ];
            delete [ ] graph;
        }
        make_graph(aco.number,degree);
        int v;
        consistent=0;
        bool * visited=new bool[aco.number];
        for(i=0;i<aco.number;i++)
            visited[i]=false;
        int v1;
        stack <int> S;
        for(j=0;j<aco.number;j++)
        {
            v=0;
            v1=j;
            S.push(v1);
            while(!S.empty())
            {
                v1=S.top();
                S.pop();
                if(!visited[v1])
                {
                    visited[v1]=true;
                    for(i=0;i<aco.number;i++)
                    {
                        if(graph[v1][i] && !visited[i])
                        {
                            S.push(i);
                        }
                    }
                    v++;
                    if(v==aco.number)
                        consistent=1;
                }
            }
        }
        iter=1;
        delete [] visited;
    }while(consistent==0);

    weight=new float*[aco.number];
    for(i=0;i<aco.number;i++)
        weight[i]=new float[aco.number];
    for(i=0;i<aco.number;i++)
    {
        for(j=0;j<aco.number;j++)
            weight[i][j]=0.0;
    }

    float weight1;
    if(consistent)
    {
        for(i=0;i<aco.number;i++)
        {
            for(j=0;j<aco.number;j++)
            {
                if(graph[i][j]==1)
                {
                    weight1=(rand()%100)+1;
                    weight[i][j]=weight1;
                    weight[j][i]=weight1;
                }
            }
        }
    }

    int ** how_many_times_visited;
    how_many_times_visited=new int*[aco.number];
    for(i=0;i<aco.number;i++)
        how_many_times_visited[i]=new int[aco.number];

    pheromones=new float*[aco.number];
    for(i=0;i<aco.number;i++)
        pheromones[i]=new float[aco.number];
    for(i=0;i<aco.number;i++)
    {
        for(j=0;j<aco.number;j++)
            pheromones[i][j]=1.0;
    }
    leave=new float*[aco.number];
    for(i=0;i<aco.number;i++)
        leave[i]=new float[aco.number];

    float prob[aco.number];
    int allowed[aco.number];

    int next,random_choice,new1,new2,visited;

    int iterations=30;

    Anthill ant_ant[aco.ants];
    Solution solution;
    Solution final_s;
    final_s.path_cost=1000000;

    long double alg=clock();
    while(clock()-alg<600000)
    {
        for(int it=0;it<iterations;it++)
        {
            int multiplier=10000;
            int table[multiplier];
            for(i=0;i<multiplier;i++)
                table[i]=-1;

            solution.path_cost=1000000;
            for(int ant=0;ant<aco.ants;ant++)
            {
                for(int i=0;i<1000000;i++)
                    ant_ant[ant].path[i]=-1;
                ant_ant[ant].all_cost=0;
                for(int i=0;i<aco.number;i++)
                    ant_ant[ant].visited[i]=0;

                ant_ant[ant].start=(rand()%aco.number)+0;
                ant_ant[ant].cur=ant_ant[ant].start;

                int done=1,spr=1,added_cost=0;
                visited=0;
                for(i=0;i<aco.number;i++)
                {
                    for(j=0;j<aco.number;j++)
                        how_many_times_visited[i][j]=0;
                }
                while(done)
                {
                    done=0;
                    where_next(aco.number,ant_ant[ant].cur,allowed);
                    calc_probability(ant_ant[ant].cur,allowed,prob,aco.number);

                    new2=0;
                    for(i=0;i<aco.number;i++)
                    {
                        if(prob[i]>-1 && prob[i]<1)
                        {
                            new1=prob[i]*multiplier;
                            if(new2+new1<multiplier)
                            {
                                for(int l=new2;l<new2+new1;l++)
                                    table[l]=allowed[i];
                                new2+=new1;
                            }
                        }
                        if(prob[i]==1)
                        {
                            for(int l=0;l<multiplier;l++)
                                table[l]=allowed[i];
                        }
                    }

                    do
                    {
                        int check=(rand()%2)+0;
                        if(check)
                        {
                            random_choice=(rand()%multiplier)+0;
                            next=table[random_choice];
                        }
                        else
                        {
                            random_choice=(rand()%6)+0;
                            next=allowed[random_choice];
                        }
                    }while(next<0);

                    local_update(aco.number,ant_ant[ant].path,ant_ant[ant].all_cost,visited-1);

                    how_many_times_visited[ant_ant[ant].cur][next]++;
                    how_many_times_visited[next][ant_ant[ant].cur]++;

                    added_cost=weight[ant_ant[ant].cur][next]*how_many_times_visited[ant_ant[ant].cur][next];
                    ant_ant[ant].all_cost+=added_cost;

                    ant_ant[ant].visited[ant_ant[ant].cur]++;
                    ant_ant[ant].path[visited]=ant_ant[ant].cur;
                    ant_ant[ant].prev=ant_ant[ant].cur;
                    ant_ant[ant].cur=next;
                    for(j=0;j<aco.number;j++)
                    {
                        if(ant_ant[ant].visited[j]==0)
                            done=1;
                    }
                    if(done==0)
                        ant_ant[ant].all_cost-=weight[ant_ant[ant].prev][ant_ant[ant].cur];
                    visited++;
                }

                if(ant_ant[ant].all_cost<solution.path_cost)
                {
                    solution.path_cost=ant_ant[ant].all_cost;
                    for(int pass_on=0;pass_on<1000000;pass_on++)
                        solution.path[pass_on]=ant_ant[ant].path[pass_on];
                }

                sum(visited-1,aco.number,ant_ant[ant].path,ant_ant[ant].all_cost);
                leave_pheromones(aco.number);

                if(solution.path_cost<=final_s.path_cost)
                {
                    final_s.path_cost=solution.path_cost;
                    for(int pass_on=0;pass_on<1000000;pass_on++)
                        final_s.path[pass_on]=solution.path[pass_on];
                    it=0;
                }
            }
            alg=clock()-alg;
            cout<<"SOLUTION\ndroga: ";
            for(i=0;i<1000000;i++)
                if(final_s.path[i]!=-1)
                    cout<<final_s.path[i]<<" ";
            cout<<"\ncost: "<<final_s.path_cost<<endl;
            if(final_s.path_cost==1000000)
                cout<<"unacceptable solution\n";
        }
    }

    for(i=0;i<aco.number;i++) delete [] graph[i];
    delete [] graph;
    for(i=0;i<aco.number;i++) delete [] pheromones[i];
    delete [] pheromones;
    for(i=0;i<aco.number;i++) delete [] weight[i];
    delete [] weight;

    return 0;
}
