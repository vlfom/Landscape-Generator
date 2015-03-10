static float minH,maxH;

void HILL_ResizeMap(float***& Map,int size);

void HILL_zoom_Height_Map(float***& Map,float zoom_y);

void HILL_Centralize(float***& Map);

void HILL_GenerateHolme(float***& Map,int random_const,int random);

void HILL_CreateMap(int a,float***& Map,int random_const,int random,int zoom);

void CreateFunc(int a,float***& Map,std::string s);
