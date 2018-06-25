#include <stdio.h>
#include <string.h>
#include <math.h>


typedef struct coord{
	int cx;
	int cy;
}coord;

typedef struct optimo{
    int valor;
    int vertice;
}optimo;


int main(){
    FILE *file=NULL;
	char buff[60];
	char first[10];
	char second[10];
	char third[10];
	int v;
	int x;
	int y;
	int cont=0;
	int size;
	int i,j;
	int xd,yd;
	int highest_cost;
	///////////////////////////////////////////////////////////////////////////

	file = fopen("eil51.tsp","r");
	if(file!=NULL){

		printf("\nFILE OPENED!!!\n\n");

	}else{
		printf("\nFILE NOT OPENED!!!\n Inclua arquivo de instancia junto ao codigo\n\n");
	}
	while(fgets(buff,60,file) != NULL){
		sscanf(buff,"%s %s %s",first,second,third);
		if(strcmp(first,"DIMENSION")==0){
            printf("\n%s %s %s",first,second,third);
            sscanf(third,"%d",&y);
            size = y;
			break;
		}
   	}
   	printf("\n[%d]\n",size);
   	coord vertices[size];
   	int matriz_vert[size][size];//MATRIZ
	while(fgets(buff, 60, file) != NULL){
		cont++;
		sscanf(buff,"%s %s %s",first,second,third);
		if(strcmp(first,"EOF")==0){
			break;
		}
		if(cont>=3){
			sscanf(first,"%d",&v);
			sscanf(second,"%d",&x);
			sscanf(third,"%d",&y);
			vertices[v-1].cx=x;
			vertices[v-1].cy=y;
		}
   	}
   	fclose(file);

   	//gerar matriz/grafo

   	for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            if(i==j){
                matriz_vert[i][j]=0;
            }else{
                xd = vertices[i].cx - vertices[j].cx;
                yd = vertices[i].cy - vertices[j].cy;
                matriz_vert[i][j] = (int)(sqrt((xd*xd)+(yd*yd)));
                if(i==0 && j==0){
                    highest_cost = matriz_vert[i][j];
                }else{
                    if(matriz_vert[i][j]>highest_cost){
                        highest_cost=matriz_vert[i][j];
                    }
                }
            }
        }
   	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////PROGRAMACAO DINAMICA////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int origem ;
    int lowest_cost;
    int best_vert;
    int etapa = 0;
    int sum=0;
    int cicle=0;
    int k,a;
    optimo matriz_opt[size][size];

    //define randomicamente a origem;
    srand(time(NULL));
    origem = 0;//rand()%size;
    printf("ORIGEM: %d",origem+1);
    //

    for(i=0;i<size;i++){
        if(i!=origem){
            matriz_opt[etapa][i].valor=matriz_vert[origem][i];
            matriz_opt[etapa][i].vertice=origem;
        }else{
            matriz_opt[etapa][i].valor=-1;
            matriz_opt[etapa][i].vertice=-1;
        }
    }
    do{
        etapa++;
        for(i=0;i<size;i++){//J = coluna I = linha
            sum = -1;
            best_vert=-1;
            lowest_cost=-1;
            for(j=0;j<size;j++){//somas
                cicle=0;
                if((i!=j)&&(i!=origem)&&(j!=origem)){
                    for(k=etapa-1;k>0;k--){//verifica ciclos //K=etapa
                        if(k==etapa-1){
                            a=j;
                        }
                        if(matriz_opt[k][a].vertice==i){
                            cicle=1;
                            break;
                        }
                        a=matriz_opt[k][a].vertice;
                    }
                    if(cicle==0 && matriz_opt[etapa-1][j].valor != -1){
                        if(sum==-1){
                            sum = matriz_vert[i][j]+matriz_opt[etapa-1][j].valor;
                            lowest_cost=sum;
                            best_vert=j;
                        }else{
                            sum = matriz_vert[i][j]+matriz_opt[etapa-1][j].valor;
                            if(sum<lowest_cost){
                                lowest_cost=sum;
                                best_vert=j;
                            }
                        }
                    }
                    matriz_opt[etapa][i].valor=lowest_cost;
                    matriz_opt[etapa][i].vertice=best_vert;
                }else if((i==origem)){
                    matriz_opt[etapa][i].valor=-1;
                    matriz_opt[etapa][i].vertice=-1;
                }
            }
        }
    }while(etapa<size-2);
    ////////////ultima etapa
    etapa++;
    sum = -1;
    for(j=0;j<size;j++){//melhor valor para saida da origem | ultima etapa
        if(matriz_opt[etapa-1][j].valor != -1){
            if(sum==-1){
                sum = matriz_vert[origem][j]+matriz_opt[etapa-1][j].valor;
                lowest_cost=sum;
                best_vert=j;
            }else{
                sum = matriz_vert[origem][j]+matriz_opt[etapa-1][j].valor;
                if(sum<lowest_cost){
                    lowest_cost=sum;
                    best_vert=j;
                }
            }
        }
        matriz_opt[etapa][origem].valor=lowest_cost;
        matriz_opt[etapa][origem].vertice=best_vert;
        if(j!= origem){
            matriz_opt[etapa][j].valor=-1;
            matriz_opt[etapa][j].vertice=-1;
        }
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

        FILE *output;
        output = fopen("output.txt","w");

        fprintf(output,"     ");
        for(i=0;i<size;i++){
            fprintf(output,"%2d ",i+1);
        }

        fprintf(output,"\n\n");
        for(i=0;i<size;i++){
            fprintf(output,"%2d   ",i+1);
            for(j=0;j<size;j++){
                fprintf(output,"%2d ",matriz_vert[i][j]);
            }
            fprintf(output,"\n");
        }

        fprintf(output,"\n\n");
        fprintf(output,"ORIGEM: %d\n\n",origem+1);
        for(i=0;i<size;i++){//vertice
            fprintf(output,"%2d   ",i+1);
            for(j=0;j<size;j++){//etapa
                if((matriz_opt[j][i].valor == -1) || matriz_opt[j][i].vertice == -1){
                    fprintf(output,"*** - ***   ");
                }else{
                    fprintf(output,"%3d - %3d   ",matriz_opt[j][i].valor,matriz_opt[j][i].vertice+1);
                }
            }
            fprintf(output,"\n");
        }
        fprintf(output,"\n\n");
        //Find path
        fprintf(output,"[%2d] ",origem+1);
        best_vert=matriz_opt[etapa][origem].vertice;
        fprintf(output,"[%2d] ",best_vert+1);

        while(etapa>0){
            etapa--;
            best_vert=matriz_opt[etapa][best_vert].vertice;
            fprintf(output,"[%2d] ",best_vert+1);
        }
        fprintf(output,"\nCUSTO TOTAL: %d",matriz_opt[etapa+size-1][origem].valor);
        fclose(output);
        printf("\nDONE!!!\n\n");
   	return 0;
}

