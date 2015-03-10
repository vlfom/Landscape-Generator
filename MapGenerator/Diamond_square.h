void DS_Create_Mass_Map(float***& Map,int***& Cubes,int devisions);

int DS_Func(int a);

void DS_MakeBorders(float***& Map,int num);

void DS_Square(int s_i,int s_j,int f_i,int f_j,int kof,float***& Map,int rand_answer);

void DS_Diamond(int s_i,int s_j,int f_i,int f_j,int num,int kof,float***& Map,int rand_answer);

void DS_AddCubes(int s_i,int s_j,int f_i,int f_j,int index,int length,int***& Cubes);

void DS_CreateMap(float***& Map,int***& Cubes,int devisions,float x_start,float y_start,float z_start,float x_finish,float y_finish,float z_finish);
