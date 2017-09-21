//�Ŵ��㷨

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<cmath>
#include<ctime>
#include<limits>
using namespace std;

int GroupScale;           //��Ⱥ��ģ
int ProblemScale;
int CrossoverNum;         //������
int MutationNum;          //������
int MaxGeneration = 100000;           //��

double CrossoverRate = 0.8;            //�������
double MutationRate = 0.001;           //�������

int * * InitPopulation(){
	int **a = new int*[GroupScale];
	for (int i = 0; i<GroupScale; i++){
		a[i] = new int[ProblemScale];
	}

	srand((unsigned)time(NULL));
	for (int i = 0; i<GroupScale; i++){
		for (int j = 0; j<ProblemScale; j++){
			a[i][j] = ProblemScale*rand() / (RAND_MAX + 1);     //rand_maxΪrand�����ķ���ֵ   a[i][j](0--ProblemScale-1)
			//rand()����0---RANG_MAX      ��a[i][j]=d��ʾ��j�е�d����һ���ʺ�
		}
	}
	return a;
}

void main(){
	clock_t start, finish;
	int i, j, n, min, temp;
	double total;

	cout << "��������Ⱥ��ģ�ͻʺ������ģ:" << endl;
	cin >> GroupScale >> ProblemScale;
	CrossoverNum = (int)(GroupScale * CrossoverRate);       //������
	MutationNum = (int)(GroupScale * MutationRate);         //������

	if (CrossoverNum % 2 == 1) CrossoverNum++;         //��֤������Ϊż��
	int *fitness = new int[GroupScale];            //��fitness[k]����¼Ⱥ���и�����Ӧ��
	double *value = new double[GroupScale];
	int *selection = new int[CrossoverNum];

	while (true){
		start = clock();

		//������ʼȺ��
		int **queen = InitPopulation();
		while (MaxGeneration--){                                         //����Ⱥ���и�����Ӧ��                 
			total = 0;
			min = INT_MAX;
			for (int k = 0; k<GroupScale; k++){
				fitness[k] = 0;
				for (i = 0; i<ProblemScale - 1; i++){
					for (j = i + 1; j<ProblemScale; j++){
						if (queen[k][i] == queen[k][j]) fitness[k]++;       //��fitness[k]����¼Ⱥ���и�����Ӧ��   
						if (abs(queen[k][i] - queen[k][j]) == j - i) fitness[k]++;  //�ж϶Խ���
					}
				}
				if (fitness[k]<min){
					n = k;
					min = fitness[k];
				}
			}

			//������Ӧ��
			if (min == 0) break;

			//������Ӧ��ѡ�����
			for (int k = 0; k<GroupScale; k++){
				value[k] = 1.0 / fitness[k];        //value[k]��ֵΪ�������Ӧ�ȣ�Խ��˵���˸���Խ����
				total += value[k];
			}
			for (int k = 0; k<GroupScale; k++){
				value[k] = value[k] / total;                            //���屻ѡ��ĸ��ʼ���
				if (k != 0) value[k] += value[k - 1];           //���۸���
			}

			//(�ö���ѡ��ѡ�����)
			for (i = 0; i<CrossoverNum; i++){
				double rate = rand()*1.0 / (RAND_MAX + 1);
				if (rate<value[0]) selection[i] = 0;
				else{
					for (j = 1; j<GroupScale; j++)
					if (rate >= value[j - 1] && rate<value[j]){
						selection[i] = j;                   //selction[i]�洢   ����ѡ����ʴ�ĸ�����н��深ֳ
						break;
					}
				}
			}

			//��ѡ�������н��深ֳ
			for (i = 0; i<CrossoverNum; i += 2){
				//���ѡ�񽻲�㣬Ȼ������໥����
				int k = ProblemScale*rand() / (RAND_MAX + 1);
				for (j = k; j<ProblemScale; j++){
					temp = queen[selection[i]][j];
					queen[selection[i]][j] = queen[selection[i + 1]][j];
					queen[selection[i + 1]][j] = temp;
				}
			}

			//��ֳ����������������
			for (int k = 0; k<MutationNum; k++){
				i = GroupScale*rand() / (RAND_MAX + 1);
				j = ProblemScale*rand() / (RAND_MAX + 1);
				queen[i][j] = ProblemScale*rand() / (RAND_MAX + 1);
			}

			//��̭

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
			cout << "��ʱ" << finish - start << endl;;
			return;
		}
		else{
			cout << "��ʱ" << finish - start << "      û�к��ʵĸ��屻�ҵ�" << endl;
			return;
		}
	}
	return;
}