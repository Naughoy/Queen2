//遗传算法

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<cmath>
#include<ctime>
#include<limits>
using namespace std;

int GroupScale;           //种群规模
int ProblemScale;
int CrossoverNum;         //交叉数
int MutationNum;          //变异数
int MaxGeneration = 100000;           //代

double CrossoverRate = 0.8;            //交叉比率
double MutationRate = 0.001;           //变异比率

int * * InitPopulation(){
	int **a = new int*[GroupScale];
	for (int i = 0; i<GroupScale; i++){
		a[i] = new int[ProblemScale];
	}

	srand((unsigned)time(NULL));
	for (int i = 0; i<GroupScale; i++){
		for (int j = 0; j<ProblemScale; j++){
			a[i][j] = ProblemScale*rand() / (RAND_MAX + 1);     //rand_max为rand函数的返回值   a[i][j](0--ProblemScale-1)
			//rand()生成0---RANG_MAX      若a[i][j]=d表示第j行第d列有一个皇后
		}
	}
	return a;
}

void main(){
	clock_t start, finish;
	int i, j, n, min, temp;
	double total;

	cout << "请输入种群规模和皇后问题规模:" << endl;
	cin >> GroupScale >> ProblemScale;
	CrossoverNum = (int)(GroupScale * CrossoverRate);       //交叉数
	MutationNum = (int)(GroupScale * MutationRate);         //变异数

	if (CrossoverNum % 2 == 1) CrossoverNum++;         //保证交叉数为偶数
	int *fitness = new int[GroupScale];            //用fitness[k]来记录群体中个体适应度
	double *value = new double[GroupScale];
	int *selection = new int[CrossoverNum];

	while (true){
		start = clock();

		//创建初始群体
		int **queen = InitPopulation();
		while (MaxGeneration--){                                         //计算群体中个体适应度                 
			total = 0;
			min = INT_MAX;
			for (int k = 0; k<GroupScale; k++){
				fitness[k] = 0;
				for (i = 0; i<ProblemScale - 1; i++){
					for (j = i + 1; j<ProblemScale; j++){
						if (queen[k][i] == queen[k][j]) fitness[k]++;       //用fitness[k]来记录群体中个体适应度   
						if (abs(queen[k][i] - queen[k][j]) == j - i) fitness[k]++;  //判断对角线
					}
				}
				if (fitness[k]<min){
					n = k;
					min = fitness[k];
				}
			}

			//评估适应度
			if (min == 0) break;

			//根据适应度选择个体
			for (int k = 0; k<GroupScale; k++){
				value[k] = 1.0 / fitness[k];        //value[k]的值为个体的适应度，越大说明此个体越优秀
				total += value[k];
			}
			for (int k = 0; k<GroupScale; k++){
				value[k] = value[k] / total;                            //个体被选择的概率计算
				if (k != 0) value[k] += value[k - 1];           //积累概率
			}

			//(用赌盘选择法选择个体)
			for (i = 0; i<CrossoverNum; i++){
				double rate = rand()*1.0 / (RAND_MAX + 1);
				if (rate<value[0]) selection[i] = 0;
				else{
					for (j = 1; j<GroupScale; j++)
					if (rate >= value[j - 1] && rate<value[j]){
						selection[i] = j;                   //selction[i]存储   方便选择概率大的个体进行交叉繁殖
						break;
					}
				}
			}

			//被选择个体进行交叉繁殖
			for (i = 0; i<CrossoverNum; i += 2){
				//随机选择交叉点，然后进行相互交叉
				int k = ProblemScale*rand() / (RAND_MAX + 1);
				for (j = k; j<ProblemScale; j++){
					temp = queen[selection[i]][j];
					queen[selection[i]][j] = queen[selection[i + 1]][j];
					queen[selection[i + 1]][j] = temp;
				}
			}

			//繁殖过程中引入变异机制
			for (int k = 0; k<MutationNum; k++){
				i = GroupScale*rand() / (RAND_MAX + 1);
				j = ProblemScale*rand() / (RAND_MAX + 1);
				queen[i][j] = ProblemScale*rand() / (RAND_MAX + 1);
			}

			//淘汰

		}

		finish = clock();
		if (min == 0){
			cout << endl;
			for (i = 0; i < ProblemScale; i++){
				for (j = 0; j < ProblemScale; j++){
					if (queen[n][i] == j)
						cout << "  #";
					else
						cout << "  *";
				}
				cout << endl;
			}
			cout << "耗时" << finish - start << endl;;
			return;
		}
		else{
			cout << "耗时" << finish - start << "      没有合适的个体被找到" << endl;
			return;
		}
	}
	return;
}