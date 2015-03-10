void XPB_Resize(int a,float***& Mas);

void XPB_Copy(int a,float***& HeightMap,float***& Water,float***& Sediment,float****& Flux);

void XPB_Water_Increment_random(int a,float***& Water,float K_water,float Interval);

void XPB_Water_Increment(int a,float***& Water,float K_water,float Interval);

void XPB_Water_outflow(int a,float***& HeightMap,float***& Water,float****& Flux,float Interval,float S,float g,float l_X,float l_Y);

void XPB_Water_change(int a,float***& HeightMap,float***& Water,float****& Flux,float***& Vector,float Interval,float l_X,float l_Y);

void XPB_Water_erosion(int a,float***& HeightMap,float***& Sediment,float***& Vector,float critical_height,float K_sediment,float K_depositing,float K_dissolving);

void XPB_Water_sediment_transportation(int a,float***& Sediment,float***& Vector,float Interval);

void XPB_Water_evaporation(int a,float***& Water,float K_evaporation,float Interval);

void XPB_Water_erosion_main(int iterations, int a,float***& HeightMap,float***& Water,float***& Sediment,float****& Flux,float***& Vector,float Interval,float l,float S,float g,float K_water,float K_sediment,float K_depositing,float K_dissolving,float K_evaporation,float critical_height);